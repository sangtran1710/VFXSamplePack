from __future__ import annotations

import inspect
from typing import Any, Awaitable, Callable

from ..critic.visual_critic import evaluate_visual as default_evaluate_visual
from ..models.critic_models import CriticAxisScores, CriticResult
from ..models.orchestrator_models import IterationRecord, LoopConfig, LoopResult, LoopState, utc_now
from ..orchestrator.artifact_writer import ArtifactWriter
from ..planner.patch_planner import generate_patch_plan as default_generate_patch_plan
from .decision_engine import post_decide, pre_decide
from .execution_engine import execute_plan
from .rollback_manager import restore_best_state, restore_snapshot, rollback_plan
from .validation_runner import run_validation_checks


def _score_from_critic(critic_result) -> float:
    return float(critic_result.overall_score)


async def _maybe_await(value):
    if inspect.isawaitable(value):
        return await value
    return value


async def _capture_image(capture_provider, asset_path: str, iteration_index: int, phase: str, state: LoopState, context: dict[str, Any]) -> str:
    if capture_provider is None:
        image_path = context.get(f"{phase}_image") or context.get("current_image")
        if not image_path:
            raise RuntimeError(f"Missing capture provider and no fallback {phase}_image/current_image in context.")
        return str(image_path)
    return str(await _maybe_await(capture_provider(asset_path, iteration_index, phase, state, context)))


def _critic_from_gate_report(phase: str, gate_report) -> CriticResult:
    notes = []
    for item in list(getattr(gate_report, "hard_failures", [])) + list(getattr(gate_report, "warnings", [])):
        gate_name = getattr(item, "gate_name", "gate")
        action = getattr(item, "recommended_action", None) or "warning"
        notes.append(f"{gate_name}:{action}")
    summary = f"Local validation skipped critic for {phase}: {', '.join(notes) or 'no details'}."
    return CriticResult(
        summary=summary,
        overall_score=0.0,
        axis_scores=CriticAxisScores({"value_match": 0.0, "color_match": 0.0, "contrast_match": 0.0}),
        deltas=[],
        top_fixes=[],
        passed=False,
        critic_version="v2",
        model_name="local-gates",
    )


async def _create_snapshot(snapshot_provider, asset_path: str, label: str, context: dict[str, Any]):
    if snapshot_provider is None:
        return None
    return await _maybe_await(snapshot_provider(asset_path, label, context))


async def run_closed_loop(
    target_image: str,
    asset_path: str,
    config: LoopConfig,
    initial_context: dict | None = None,
) -> LoopResult:
    context = dict(initial_context or {})
    artifact_writer = context.get("artifact_writer")
    if not isinstance(artifact_writer, ArtifactWriter):
        artifact_writer = ArtifactWriter(output_root=config.output_root)
    run_artifacts = context.get("run_artifacts") or artifact_writer.create_run(label="closed_loop")
    context["artifact_writer"] = artifact_writer
    context["run_artifacts"] = run_artifacts

    critic_runner = context.get("critic_runner") or default_evaluate_visual
    planner_func = context.get("planner_func") or default_generate_patch_plan
    capture_provider = context.get("capture_provider")
    snapshot_provider = context.get("snapshot_provider")
    mcp_client = context.get("mcp_client")
    gate_runner = context.get("pre_critic_gate_runner")

    state = LoopState()
    baseline_snapshot_path = await _create_snapshot(snapshot_provider, asset_path, "baseline", context)
    current_snapshot_path = baseline_snapshot_path

    loop_result = LoopResult(
        asset_path=asset_path,
        target_image=target_image,
        status="stopped",
        run_dir=str(run_artifacts.run_dir),
        started_at=utc_now(),
    )

    while True:
        pre_check = pre_decide(state.best_score if state.best_score is not None and state.iteration_count == 0 else -1.0, state, config)
        if state.iteration_count == 0:
            pass
        iteration_index = state.iteration_count

        pre_image = await _capture_image(capture_provider, asset_path, iteration_index, "pre", state, context)
        artifact_writer.copy_probe_image(run_artifacts, iteration_index=iteration_index, phase="pre", source_image=pre_image)
        pre_context = dict(context)
        pre_context.update({"iteration_index": iteration_index, "phase": "pre"})
        if gate_runner is not None and context.get("stage_plan") is not None and context.get("build_result") is not None:
            current_probe = pre_context.get("pre_probe_capture") or {"success": True, "output_path": pre_image, "material_path": asset_path, "capture_mode": "loop"}
            previous_probe = pre_context.get("previous_probe_capture")
            gate_report = await _maybe_await(gate_runner(context["stage_plan"], context["build_result"], current_probe, previous_probe, pre_context))
            if hasattr(artifact_writer, "write_gate_report"):
                artifact_writer.write_gate_report(run_artifacts, stage_id=str(getattr(context["stage_plan"], "stage_id", "loop_pre")), attempt_index=iteration_index, payload=gate_report)
            if not getattr(gate_report, "should_call_critic", True):
                pre_critic = _critic_from_gate_report("pre", gate_report)
            else:
                pre_critic = await _maybe_await(critic_runner(target_image, pre_image, pre_context))
        else:
            pre_critic = await _maybe_await(critic_runner(target_image, pre_image, pre_context))
        pre_score = _score_from_critic(pre_critic)

        pre_gate = pre_decide(pre_score, state, config)
        record = IterationRecord(iteration_index=iteration_index, pre_score=pre_score, pre_critic=pre_critic, status="running")

        if pre_gate.action == "stop":
            record.post_score = pre_score
            record.score_delta = 0.0
            record.status = pre_gate.status
            loop_result.iterations.append(record)
            loop_result.status = pre_gate.status
            loop_result.final_score = pre_score
            break

        asset_context = dict(context.get("asset_context", {}))
        asset_context.setdefault("material_path", asset_path)
        asset_context["conservative_mode"] = state.effective_risk == "conservative" or config.conservative_mode
        patch_plan = planner_func(pre_critic, asset_context)
        artifact_writer.write_json(run_artifacts.plans_dir / f"iter_{iteration_index}_plan.json", patch_plan)

        if not patch_plan.steps:
            state.empty_plan_streak += 1
            execution_results = []
            validations = []
        else:
            state.empty_plan_streak = 0
            pre_exec_snapshot = await _create_snapshot(snapshot_provider, asset_path, f"iter_{iteration_index}_pre_exec", context)
            execution_results = await execute_plan(patch_plan, state.effective_risk, mcp_client)
            state.cumulative_steps += sum(1 for result in execution_results if result.status == "success")
            validations = await run_validation_checks(patch_plan.validation_checks, asset_path, mcp_client)
            compile_failed = any(item.check_name == "compile_material" and not item.success for item in validations)
            if compile_failed:
                rollback_events = await rollback_plan(patch_plan, execution_results, mcp_client)
                record.rollback_events.extend(rollback_events)
                if pre_exec_snapshot:
                    snapshot_event = await restore_snapshot(asset_path, pre_exec_snapshot, mcp_client)
                    record.rollback_events.append(snapshot_event)
                    current_snapshot_path = pre_exec_snapshot
                state.effective_risk = "conservative"
            else:
                current_snapshot_path = await _create_snapshot(snapshot_provider, asset_path, f"iter_{iteration_index}_post_exec", context) or current_snapshot_path

        post_image = await _capture_image(capture_provider, asset_path, iteration_index, "post", state, context)
        artifact_writer.copy_probe_image(run_artifacts, iteration_index=iteration_index, phase="post", source_image=post_image)
        post_context = dict(context)
        post_context.update({"iteration_index": iteration_index, "phase": "post"})
        if gate_runner is not None and context.get("stage_plan") is not None and context.get("build_result") is not None:
            current_probe = post_context.get("post_probe_capture") or {"success": True, "output_path": post_image, "material_path": asset_path, "capture_mode": "loop"}
            previous_probe = pre_context.get("pre_probe_capture") or {"success": True, "output_path": pre_image, "material_path": asset_path, "capture_mode": "loop"}
            gate_report = await _maybe_await(gate_runner(context["stage_plan"], context["build_result"], current_probe, previous_probe, post_context))
            if hasattr(artifact_writer, "write_gate_report"):
                artifact_writer.write_gate_report(run_artifacts, stage_id=str(getattr(context["stage_plan"], "stage_id", "loop_post")), attempt_index=iteration_index + 1000, payload=gate_report)
            if not getattr(gate_report, "should_call_critic", True):
                post_critic = _critic_from_gate_report("post", gate_report)
            else:
                post_critic = await _maybe_await(critic_runner(target_image, post_image, post_context))
        else:
            post_critic = await _maybe_await(critic_runner(target_image, post_image, post_context))
        post_score = _score_from_critic(post_critic)

        record.post_critic = post_critic
        record.post_score = post_score
        record.score_delta = post_score - pre_score
        record.execution_results = execution_results
        record.validations = validations

        if state.best_score is None or post_score > state.best_score:
            state.best_score = post_score
            state.best_iteration_index = iteration_index
            state.best_snapshot_path = current_snapshot_path

        decision = post_decide(record, state, config)
        record.status = decision.status
        loop_result.iterations.append(record)

        if decision.reason == "regression":
            state.effective_risk = "conservative"
            rollback_events = await rollback_plan(patch_plan, execution_results, mcp_client)
            record.rollback_events.extend(rollback_events)
            if current_snapshot_path:
                snapshot_event = await restore_snapshot(asset_path, current_snapshot_path, mcp_client)
                record.rollback_events.append(snapshot_event)

        state.iteration_count += 1

        if decision.action == "stop":
            loop_result.status = decision.status
            loop_result.final_score = post_score
            break
        if decision.action == "rollback":
            if state.iteration_count >= config.max_iterations or state.cumulative_steps >= config.max_total_steps:
                loop_result.status = decision.status
                loop_result.final_score = post_score
                break
            continue
        if state.iteration_count >= config.max_iterations:
            loop_result.status = "stopped"
            loop_result.final_score = post_score
            break

    best_restore_event = None
    if loop_result.status != "passed":
        best_restore_event = await restore_best_state(
            asset_path,
            best_snapshot_path=state.best_snapshot_path,
            baseline_snapshot_path=baseline_snapshot_path,
            current_snapshot_path=current_snapshot_path,
            mcp_client=mcp_client,
        )
        if best_restore_event is not None and loop_result.iterations:
            loop_result.iterations[-1].rollback_events.append(best_restore_event)
            loop_result.restored_best_state = best_restore_event.success

    loop_result.best_score = state.best_score
    loop_result.ended_at = utc_now()
    artifact_writer.write_loop_result(run_artifacts, loop_result)
    return loop_result
