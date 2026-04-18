from __future__ import annotations

import inspect
from typing import Any

from ..analyzer.visual_analyzer import analyze_material_reference_visual as default_analyzer
from ..builder.material_stage_builder import build_stage_from_semantic_plan as default_stage_builder
from ..critic.visual_critic import evaluate_visual as default_critic_runner
from ..models.orchestrator_models import LoopConfig, utc_now
from ..models.semantic_models import (
    AnalyzerOutput,
    FailureClassification,
    GateResult,
    LocalValidationReport,
    ProbeCaptureResult,
    SemanticStagePlan,
    StageBuildResult,
    StageWorkflowRecord,
    VisualMaterialWorkflowConfig,
    VisualMaterialWorkflowResult,
)
from ..orchestrator.artifact_writer import ArtifactWriter
from ..planner.semantic_visual_plan import build_material_visual_plan as default_plan_builder
from .closed_loop_orchestrator import run_closed_loop
from .failure_classifier import classify_visual_failure as default_failure_classifier
from .local_validation_gates import run_pre_critic_gates as default_gate_runner
from .probe_preset import capture_stage_probe as default_capture_stage_probe


async def _maybe_await(value: Any) -> Any:
    if inspect.isawaitable(value):
        return await value
    return value


async def _capture_stage_probe(
    build_result: StageBuildResult,
    stage_plan: SemanticStagePlan,
    context: dict[str, Any],
    record: StageWorkflowRecord,
) -> ProbeCaptureResult:
    existing_probe = build_result.asset_context.get("probe_capture")
    if isinstance(existing_probe, dict):
        return ProbeCaptureResult.model_validate(existing_probe)
    if build_result.probe_image:
        return ProbeCaptureResult(
            success=True,
            actor_label=str(context.get("probe_actor_label", "mock_probe")),
            mesh_path=str(context.get("probe_mesh_path", "/Engine/BasicShapes/Sphere.Sphere")),
            material_path=build_result.asset_path,
            output_path=str(build_result.probe_image),
            output_dir=str(build_result.probe_image),
            capture_mode="provided",
            frame_actor=True,
            preset_id=str(context.get("probe_preset_id", "provided")),
            details={},
        )
    capture_provider = context.get("stage_capture_provider")
    if capture_provider is None:
        return default_capture_stage_probe(build_result.asset_path, build_result.asset_path, stage_plan.stage_id, context)
    captured = await _maybe_await(capture_provider(build_result, stage_plan, context, record))
    if isinstance(captured, ProbeCaptureResult):
        return captured
    if isinstance(captured, dict):
        return ProbeCaptureResult.model_validate(captured)
    return ProbeCaptureResult(
        success=True,
        actor_label=str(context.get("probe_actor_label", "custom_probe")),
        mesh_path=str(context.get("probe_mesh_path", "/Engine/BasicShapes/Sphere.Sphere")),
        material_path=build_result.asset_path,
        output_path=str(captured),
        output_dir=str(captured),
        capture_mode="custom",
        frame_actor=True,
        preset_id=str(context.get("probe_preset_id", "custom")),
        details={},
    )


async def _run_stage_fix_loop(
    *,
    reference_image: str,
    asset_path: str,
    build_result: StageBuildResult,
    stage_plan: SemanticStagePlan,
    classification: FailureClassification,
    workflow_config: VisualMaterialWorkflowConfig,
    context: dict[str, Any],
):
    custom_runner = context.get("stage_fix_runner")
    if custom_runner is not None:
        return await _maybe_await(
            custom_runner(
                reference_image=reference_image,
                asset_path=asset_path,
                build_result=build_result,
                stage_plan=stage_plan,
                classification=classification,
                workflow_config=workflow_config,
                context=context,
            )
        )

    if classification.change_type not in {"parameter_repair", "shaping_repair"} or workflow_config.max_fix_iterations <= 0:
        return None

    asset_context = dict(build_result.asset_context)
    asset_context.setdefault("material_path", asset_path)
    stage_context = dict(context)
    stage_context["asset_context"] = asset_context
    stage_context["build_result"] = build_result
    stage_context["pre_critic_gate_runner"] = context.get("gate_runner") or default_gate_runner
    if stage_context.get("capture_provider") is None:
        async def _stable_capture_provider(loop_asset_path: str, iteration_index: int, phase: str, state, capture_context: dict[str, Any]) -> str:
            del iteration_index, state
            probe = default_capture_stage_probe(loop_asset_path, loop_asset_path, f"{stage_plan.stage_id}_{phase}", capture_context)
            capture_context[f"{phase}_probe_capture"] = probe.model_dump(mode="json", exclude_none=True)
            return str(probe.output_path)

        stage_context["capture_provider"] = _stable_capture_provider
    loop_config = LoopConfig(
        pass_threshold=workflow_config.pass_threshold,
        max_iterations=workflow_config.max_fix_iterations,
        max_total_steps=max(2, workflow_config.max_fix_iterations * 3),
        output_root=workflow_config.output_root,
    )
    return await run_closed_loop(
        target_image=reference_image,
        asset_path=asset_path,
        config=loop_config,
        initial_context=stage_context,
    )


async def run_visual_material_workflow(
    *,
    reference_images: list[str],
    asset_path: str,
    config: VisualMaterialWorkflowConfig,
    initial_context: dict[str, Any] | None = None,
    text_goal: str | None = None,
) -> VisualMaterialWorkflowResult:
    if not reference_images:
        raise ValueError("reference_images must contain at least one image.")

    context = dict(initial_context or {})
    artifact_writer = context.get("artifact_writer")
    if not isinstance(artifact_writer, ArtifactWriter):
        artifact_writer = ArtifactWriter(output_root=config.output_root)
    run_artifacts = context.get("run_artifacts") or artifact_writer.create_run(label="visual_material_workflow")
    context["artifact_writer"] = artifact_writer
    context["run_artifacts"] = run_artifacts
    context.setdefault("text_goal", text_goal)
    context.setdefault("target_archetype", context.get("target_archetype", "material"))

    analyzer_runner = context.get("analyzer_runner") or default_analyzer
    plan_builder = context.get("semantic_plan_builder") or default_plan_builder
    stage_builder = context.get("stage_builder") or default_stage_builder
    critic_runner = context.get("critic_runner") or default_critic_runner
    failure_classifier = context.get("failure_classifier") or default_failure_classifier
    gate_runner = context.get("gate_runner") or default_gate_runner
    shared_material_context: dict[str, Any] = dict(context.get("material_context", {}))
    previous_probe: ProbeCaptureResult | None = None

    result = VisualMaterialWorkflowResult(
        asset_path=asset_path,
        reference_images=list(reference_images),
        text_goal=text_goal,
        status="blocked",
        run_dir=str(run_artifacts.run_dir),
        started_at=utc_now(),
    )

    analyzer_context = dict(context)
    analyzer_context["reference_images"] = list(reference_images)
    analyzer_output: AnalyzerOutput = await _maybe_await(analyzer_runner(reference_images, analyzer_context))
    semantic_plan = plan_builder(analyzer_output)
    result.analyzer_output = analyzer_output
    result.semantic_plan = semantic_plan
    artifact_writer.write_json(run_artifacts.analysis_dir / "reference_analysis.json", analyzer_output)
    artifact_writer.write_semantic_plan(run_artifacts, semantic_plan)

    for stage_index, stage_plan in enumerate(semantic_plan.stages):
        record = StageWorkflowRecord(
            stage_id=stage_plan.stage_id,
            stage_goal=stage_plan.goal,
            stage_label=stage_plan.label,
        )
        stage_context = dict(context)
        stage_context["stage_index"] = stage_index
        stage_context["stage_plan"] = stage_plan
        stage_context["semantic_plan"] = semantic_plan
        stage_context["material_context"] = shared_material_context
        stage_context["probe_visual_change_threshold"] = config.probe_visual_change_threshold
        build_result = await _maybe_await(stage_builder(asset_path, stage_plan, stage_context))
        if not isinstance(build_result, StageBuildResult):
            build_result = StageBuildResult.model_validate(build_result)
        record.build_result = build_result
        record.notes.extend(build_result.notes)
        shared_material_context = dict(build_result.asset_context)

        if not build_result.success or not build_result.compile_ok:
            record.stage_status = "failed"
            result.stages.append(record)
            result.status = "failed"
            break

        probe_capture = await _capture_stage_probe(build_result, stage_plan, stage_context, record)
        build_result.asset_context["probe_capture"] = probe_capture.model_dump(mode="json", exclude_none=True)
        record.probe_image = probe_capture.output_path
        if probe_capture.output_path:
            artifact_writer.copy_stage_probe_image(run_artifacts, stage_id=stage_plan.stage_id, source_image=probe_capture.output_path, attempt_index=0)

        gate_report: LocalValidationReport = await _maybe_await(gate_runner(stage_plan, build_result, probe_capture, previous_probe, stage_context))
        artifact_writer.write_gate_report(run_artifacts, stage_id=stage_plan.stage_id, attempt_index=0, payload=gate_report)
        record.notes.extend(gate_report.notes)
        for gate in gate_report.warnings:
            if gate.recommended_action:
                record.notes.append(f"{gate.gate_name}: {gate.recommended_action}")

        if not gate_report.should_call_critic:
            override = gate_report.failure_class_override
            evidence = []
            for gate in gate_report.hard_failures + gate_report.warnings:
                if isinstance(gate, GateResult):
                    evidence.append(f"{gate.gate_name}:{gate.recommended_action or 'none'}")
            if override == "capture":
                classification = FailureClassification(
                    failure_class="capture",
                    target_stage=stage_plan.stage_id,
                    target_lane=None,
                    change_type="fix_capture",
                    expected_effect="Stabilize probe capture before graph mutation continues.",
                    risk="low",
                    summary="Local validation blocked the stage on probe or binding failure.",
                    evidence=evidence,
                    confidence=0.95,
                )
            elif override == "structural":
                classification = FailureClassification(
                    failure_class="structural",
                    target_stage=stage_plan.stage_id,
                    target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
                    change_type="lane_rebuild",
                    expected_effect="Repair the stage graph block before semantic critique.",
                    risk="medium",
                    summary="Local validation found a structural graph integrity issue.",
                    evidence=evidence,
                    confidence=0.92,
                )
            elif override == "failed":
                classification = FailureClassification(
                    failure_class="unknown",
                    target_stage=stage_plan.stage_id,
                    target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
                    change_type="stop",
                    expected_effect="Stop because the stage cannot compile or validate locally.",
                    risk="medium",
                    summary="Local validation found a hard failure before critic.",
                    evidence=evidence,
                    confidence=0.9,
                )
            elif override == "no_visible_change":
                classification = FailureClassification(
                    failure_class="unknown",
                    target_stage=stage_plan.stage_id,
                    target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
                    change_type="stop",
                    expected_effect="Skip critic because the build did not create enough visible change.",
                    risk="low",
                    summary="Local validation skipped critic due to near-zero probe delta.",
                    evidence=evidence,
                    confidence=0.85,
                )
            else:
                classification = FailureClassification(
                    failure_class="unknown",
                    target_stage=stage_plan.stage_id,
                    target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
                    change_type="stop",
                    expected_effect="Stop on local validation failure.",
                    risk="medium",
                    summary="Local validation blocked semantic critique.",
                    evidence=evidence,
                    confidence=0.7,
                )
            record.failure_classification = classification
            artifact_writer.write_fix_decision(run_artifacts, stage_id=stage_plan.stage_id, attempt_index=0, payload=classification)
            record.stage_status = "blocked" if classification.change_type in {"fix_capture", "stop"} else "failed"
            result.stages.append(record)
            result.status = "blocked" if record.stage_status == "blocked" else "failed"
            break

        critic_context = dict(stage_context)
        critic_context.update({"iteration_index": stage_index, "phase": stage_plan.stage_id})
        critic_result = await _maybe_await(critic_runner(reference_images[0], str(probe_capture.output_path), critic_context))
        record.critic_result = critic_result

        classification: FailureClassification = failure_classifier(
            critic_result,
            stage_plan,
            dominant_read=analyzer_output.dominant_read,
            anti_patterns=list(analyzer_output.anti_patterns),
            capture_metadata=build_result.asset_context.get("capture_metadata", {}),
        )
        record.failure_classification = classification
        artifact_writer.write_fix_decision(run_artifacts, stage_id=stage_plan.stage_id, attempt_index=0, payload=classification)
        previous_probe = probe_capture

        if classification.change_type == "advance_stage":
            record.stage_status = "approved"
            result.stages.append(record)
            result.status = "passed" if stage_index == len(semantic_plan.stages) - 1 else "stopped"
            continue

        if classification.change_type == "fix_capture":
            record.stage_status = "blocked"
            record.notes.append("Stage blocked by capture failure classification.")
            result.stages.append(record)
            result.status = "blocked"
            break

        if classification.change_type in {"lane_rebuild", "repair_stage"} and not config.allow_stage_repairs:
            record.stage_status = "blocked"
            record.notes.append("Stage repair required but disabled by workflow config.")
            result.stages.append(record)
            result.status = "blocked"
            break

        if classification.change_type in {"parameter_repair", "shaping_repair", "lane_rebuild", "repair_stage"}:
            fix_result = await _run_stage_fix_loop(
                reference_image=reference_images[0],
                asset_path=asset_path,
                build_result=build_result,
                stage_plan=stage_plan,
                classification=classification,
                workflow_config=config,
                context=stage_context,
            )
            record.fix_loop_result = fix_result
            if fix_result is not None:
                final_score = fix_result.final_score or 0.0
                if fix_result.status == "passed" or final_score >= config.pass_threshold:
                    record.stage_status = "repaired"
                    result.stages.append(record)
                    result.status = "passed" if stage_index == len(semantic_plan.stages) - 1 else "stopped"
                    continue
                record.notes.append(f"Fix loop ended with status={fix_result.status} score={final_score}.")

        record.stage_status = "blocked" if classification.failure_class == "unknown" and config.stop_on_unclassified_failure else "failed"
        result.stages.append(record)
        result.status = "blocked" if record.stage_status == "blocked" else "failed"
        break
    else:
        result.status = "passed"

    result.ended_at = utc_now()
    artifact_writer.write_loop_result(run_artifacts, result)
    return result
