from __future__ import annotations

import argparse
import asyncio
import json
import logging
from pathlib import Path

from ExternalAgent.closed_loop.analyzer.visual_analyzer import analyze_material_reference_visual
from ExternalAgent.closed_loop.critic.critic_adapter import build_default_adapter
from ExternalAgent.closed_loop.critic.visual_critic import evaluate_visual
from ExternalAgent.closed_loop.models.critic_models import CriticAxisScores, CriticFix, CriticResult
from ExternalAgent.closed_loop.models.semantic_models import LocalValidationReport, StageBuildResult, VisualMaterialWorkflowConfig
from ExternalAgent.closed_loop.orchestrator.artifact_writer import ArtifactWriter
from ExternalAgent.closed_loop.orchestrator.visual_material_workflow import run_visual_material_workflow


DEFAULT_TARGET_IMAGE = "Tools/Validation/ice_realistic_inspect.jpg"
DEFAULT_CURRENT_IMAGE = "Tools/Validation/MaterialProbe/ice_realistic_current_v6.png"


def _build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Closed-loop tooling entrypoint.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    smoke = subparsers.add_parser("critic-smoke", help="Run the Sprint 1 visual critic smoke test.")
    smoke.add_argument("--target-image", default=DEFAULT_TARGET_IMAGE)
    smoke.add_argument("--current-image", default=DEFAULT_CURRENT_IMAGE)
    smoke.add_argument("--mode", choices=["auto", "mock", "real", "openai"], default="mock")
    smoke.add_argument("--output-root", default=None)

    analyzer = subparsers.add_parser("analyzer-smoke", help="Run the reference visual analyzer smoke test.")
    analyzer.add_argument("--reference-image", default=DEFAULT_TARGET_IMAGE)
    analyzer.add_argument("--mode", choices=["auto", "mock", "real", "openai"], default="mock")
    analyzer.add_argument("--output-root", default=None)

    workflow = subparsers.add_parser("workflow-smoke", help="Run the semantic material workflow smoke test.")
    workflow.add_argument("--reference-image", default=DEFAULT_TARGET_IMAGE)
    workflow.add_argument("--probe-image", default=DEFAULT_CURRENT_IMAGE)
    workflow.add_argument("--output-root", default=None)
    return parser


async def _run_critic_smoke(args: argparse.Namespace) -> int:
    target_image = Path(args.target_image).resolve()
    current_image = Path(args.current_image).resolve()
    if not target_image.exists():
        raise SystemExit(f"Missing target image: {target_image}")
    if not current_image.exists():
        raise SystemExit(f"Missing current image: {current_image}")

    writer = ArtifactWriter(output_root=args.output_root)
    run_artifacts = writer.create_run(label="critic_smoke")
    writer.copy_probe_image(run_artifacts, iteration_index=0, phase="target", source_image=target_image)
    writer.copy_probe_image(run_artifacts, iteration_index=0, phase="current", source_image=current_image)

    context = {
        "asset_type": "material",
        "asset_family": "ice_realistic",
        "iteration_index": 0,
        "phase": "pre",
        "artifact_writer": writer,
        "run_artifacts": run_artifacts,
        "critic_adapter": build_default_adapter(None if args.mode == "auto" else args.mode),
    }
    result = await evaluate_visual(str(target_image), str(current_image), context)

    summary = {
        "status": "ok",
        "run_dir": str(run_artifacts.run_dir),
        "overall_score": result.overall_score,
        "passed": result.passed,
        "top_fix_count": len(result.top_fixes),
        "model_name": result.model_name,
    }
    writer.write_loop_result(run_artifacts, summary)
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0


async def _run_analyzer_smoke(args: argparse.Namespace) -> int:
    reference_image = Path(args.reference_image).resolve()
    if not reference_image.exists():
        raise SystemExit(f"Missing reference image: {reference_image}")
    writer = ArtifactWriter(output_root=args.output_root)
    run_artifacts = writer.create_run(label="analyzer_smoke")
    writer.copy_probe_image(run_artifacts, iteration_index=0, phase="reference", source_image=reference_image)
    context = {
        "artifact_writer": writer,
        "run_artifacts": run_artifacts,
        "analyzer_mode": None if args.mode == "auto" else args.mode,
        "target_archetype": "material",
        "text_goal": "Decompose the reference into a deterministic material build recipe.",
    }
    result = await analyze_material_reference_visual([str(reference_image)], context)
    summary = {
        "status": "ok",
        "run_dir": str(run_artifacts.run_dir),
        "archetype": result.archetype,
        "material_family": result.material_family,
        "stage_count": len(result.stage_recipe),
        "model_name": result.model_name,
    }
    writer.write_loop_result(run_artifacts, summary)
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0


async def _workflow_smoke_stage_builder(asset_path: str, stage_plan, context: dict) -> StageBuildResult:
    return StageBuildResult(
        stage_id=stage_plan.stage_id,
        asset_path=asset_path,
        success=True,
        compile_ok=True,
        summary=f"Built {stage_plan.stage_id}",
        node_inventory=[stage_plan.label],
        connection_inventory=["A->B"],
        parameter_inventory=["EmissiveIntensity"],
        probe_image=str(context["probe_image"]),
        asset_context={"capture_metadata": {"capture_mode": "stable_probe"}},
        notes=[],
    )


async def _workflow_smoke_critic_runner(target_image: str, current_image: str, context: dict) -> CriticResult:
    del target_image, current_image
    phase = str(context["phase"])
    if phase.endswith("core"):
        return CriticResult(
            summary="Core reads correctly.",
            overall_score=0.94,
            axis_scores=CriticAxisScores({"value_match": 0.95, "color_match": 0.92, "contrast_match": 0.95}),
            deltas=[],
            top_fixes=[],
            passed=True,
        )
    return CriticResult(
        summary="The breakup is still too smooth and cloudy.",
        overall_score=0.46,
        axis_scores=CriticAxisScores({"value_match": 0.8, "color_match": 0.71, "contrast_match": 0.22}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="tighten_filaments",
                axis="contrast_match",
                instruction="Increase breakup contrast so filament lines read clearly.",
                rationale="Current breakup is too smooth and cloudy.",
                priority=1,
                confidence=0.92,
                risk="medium",
                target_hint="breakup lane",
                expected_effect="Readable thin filament lines.",
            )
        ],
        passed=False,
    )


async def _workflow_smoke_stage_fix_runner(**kwargs):
    from ExternalAgent.closed_loop.models.orchestrator_models import LoopResult

    return LoopResult(
        asset_path=kwargs["asset_path"],
        target_image=kwargs["reference_image"],
        status="passed",
        final_score=0.93,
        best_score=0.93,
        restored_best_state=False,
    )


def _workflow_smoke_gate_runner(stage_plan, build_result, current_probe, previous_probe, context):
    del stage_plan, build_result, current_probe, previous_probe, context
    return LocalValidationReport(should_call_critic=True)


async def _run_workflow_smoke(args: argparse.Namespace) -> int:
    reference_image = Path(args.reference_image).resolve()
    probe_image = Path(args.probe_image).resolve()
    if not reference_image.exists():
        raise SystemExit(f"Missing reference image: {reference_image}")
    if not probe_image.exists():
        raise SystemExit(f"Missing probe image: {probe_image}")

    writer = ArtifactWriter(output_root=args.output_root)
    result = await run_visual_material_workflow(
        reference_images=[str(reference_image)],
        asset_path="/Game/VFX/Test/M_WorkflowSmoke.M_WorkflowSmoke",
        config=VisualMaterialWorkflowConfig(output_root=args.output_root, max_stage_repairs=1, max_fix_iterations=1),
        initial_context={
            "artifact_writer": writer,
            "probe_image": str(probe_image),
            "stage_builder": _workflow_smoke_stage_builder,
            "critic_runner": _workflow_smoke_critic_runner,
            "stage_fix_runner": _workflow_smoke_stage_fix_runner,
            "gate_runner": _workflow_smoke_gate_runner,
            "analyzer_mode": "mock",
            "probe_actor_label": "ForgePilot_WorkflowSmokeProbe",
            "probe_mesh_path": "/Engine/BasicShapes/Sphere.Sphere",
            "probe_preset_id": "workflow-smoke",
        },
        text_goal="Build a semantic material plan and bounded stage repair workflow.",
    )
    print(json.dumps(result.model_dump(mode="json", exclude_none=True), ensure_ascii=False, indent=2))
    return 0


def main() -> int:
    logging.basicConfig(level=logging.INFO, format="%(levelname)s %(name)s %(message)s")
    parser = _build_parser()
    args = parser.parse_args()
    if args.command == "critic-smoke":
        return asyncio.run(_run_critic_smoke(args))
    if args.command == "analyzer-smoke":
        return asyncio.run(_run_analyzer_smoke(args))
    if args.command == "workflow-smoke":
        return asyncio.run(_run_workflow_smoke(args))
    raise SystemExit(f"Unsupported command: {args.command}")


if __name__ == "__main__":
    raise SystemExit(main())
