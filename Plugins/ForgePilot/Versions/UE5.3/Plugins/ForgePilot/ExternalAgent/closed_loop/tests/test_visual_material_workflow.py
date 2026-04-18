from __future__ import annotations

import asyncio
from pathlib import Path

from ExternalAgent.closed_loop.models.critic_models import CriticAxisScores, CriticFix, CriticResult
from ExternalAgent.closed_loop.models.orchestrator_models import LoopResult
from ExternalAgent.closed_loop.models.semantic_models import (
    AnalyzerOutput,
    LocalValidationReport,
    StageBuildResult,
    StageRecipeItem,
    VisualCue,
    VisualLayerSpec,
    VisualMaterialWorkflowConfig,
)
from ExternalAgent.closed_loop.orchestrator.artifact_writer import ArtifactWriter
from ExternalAgent.closed_loop.orchestrator.visual_material_workflow import run_visual_material_workflow


def _mock_analyzer_output() -> AnalyzerOutput:
    return AnalyzerOutput(
        archetype="emissive energy orb",
        material_family="emissive_energy",
        layer_stack=[
            VisualLayerSpec(layer_id="core", label="Core", role="core", priority=1, description="hot center", desired_read="white-hot center", must_preserve=True),
            VisualLayerSpec(layer_id="breakup", label="Breakup", role="breakup", priority=2, description="filaments", desired_read="thin readable filaments", must_preserve=True),
        ],
        dominant_read="White-hot core with thin plasma filaments.",
        secondary_reads=["subtle depth"],
        stage_recipe=[
            StageRecipeItem(stage_id="stage_1_core", label="Core", goal="Build the core", target_layer="core", allowed_node_families=["radial_mask"], forbidden_node_families=["noise"], validation_rules=["core_visible"], expected_visual_change="center lights up", stop_conditions=["off_center"]),
            StageRecipeItem(stage_id="stage_2_filament", label="Filament", goal="Build the filament breakup", target_layer="breakup", allowed_node_families=["noise", "power"], forbidden_node_families=["rim"], validation_rules=["filament_readable"], expected_visual_change="filament lines appear", stop_conditions=["cloudy"]),
        ],
        hard_constraints=["keep orb centered"],
        anti_patterns=["cloudy white orb"],
        failure_sensitive_constraints=["do not solve cloudy breakup with color-only tuning"],
        visual_cues=[VisualCue(cue_id="core", description="center hottest", importance="critical", layer="core")],
    )


async def analyzer_runner(reference_images: list[str], context: dict) -> AnalyzerOutput:
    del reference_images, context
    return _mock_analyzer_output()


async def stage_builder(asset_path: str, stage_plan, context: dict) -> StageBuildResult:
    return StageBuildResult(
        stage_id=stage_plan.stage_id,
        asset_path=asset_path,
        success=True,
        compile_ok=True,
        summary=f"Built {stage_plan.stage_id}",
        node_inventory=[stage_plan.label],
        connection_inventory=["A->B"],
        parameter_inventory=["EmissiveIntensity"],
        probe_image=str(context["mock_image"]),
        asset_context={"parameters": {"scalar": [{"name": "EmissiveIntensity", "value": 4.0}]}, "capture_metadata": {"capture_mode": "stable_probe"}},
        notes=[],
    )


async def critic_runner(target_image: str, current_image: str, context: dict) -> CriticResult:
    del target_image, current_image
    phase = str(context["phase"])
    if phase == "stage_1_core":
        return CriticResult(
            summary="Core is centered and readable.",
            overall_score=0.95,
            axis_scores=CriticAxisScores({"value_match": 0.96, "color_match": 0.92, "contrast_match": 0.97}),
            deltas=[],
            top_fixes=[],
            passed=True,
        )
    return CriticResult(
        summary="The breakup is still too smooth and cloudy; readable filaments are missing.",
        overall_score=0.46,
        axis_scores=CriticAxisScores({"value_match": 0.8, "color_match": 0.72, "contrast_match": 0.24}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="tighten_filaments",
                axis="contrast_match",
                instruction="Increase breakup contrast so the filament lines read clearly.",
                rationale="The current breakup is too smooth and cloudy.",
                priority=1,
                confidence=0.93,
                risk="medium",
                target_hint="breakup lane",
                expected_effect="Readable thin filament lines.",
            )
        ],
        passed=False,
    )


async def stage_fix_runner(**kwargs) -> LoopResult:
    return LoopResult(
        asset_path=kwargs["asset_path"],
        target_image=kwargs["reference_image"],
        status="passed",
        final_score=0.93,
        best_score=0.93,
        restored_best_state=False,
    )


def gate_runner(stage_plan, build_result, current_probe, previous_probe, context: dict) -> LocalValidationReport:
    del stage_plan, build_result, current_probe, previous_probe, context
    return LocalValidationReport(should_call_critic=True)


def test_visual_material_workflow_smoke() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    writer = ArtifactWriter()
    result = asyncio.run(
        run_visual_material_workflow(
            reference_images=[str(mock_image)],
            asset_path="/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
            config=VisualMaterialWorkflowConfig(max_stage_repairs=1, max_fix_iterations=1),
            initial_context={
                "artifact_writer": writer,
                "mock_image": str(mock_image),
                "analyzer_runner": analyzer_runner,
                "stage_builder": stage_builder,
                "critic_runner": critic_runner,
                "stage_fix_runner": stage_fix_runner,
                "gate_runner": gate_runner,
                "probe_actor_label": "ForgePilot_TestProbe",
                "probe_mesh_path": "/Engine/BasicShapes/Sphere.Sphere",
                "probe_preset_id": "test",
            },
            text_goal="Match a white-hot energy orb with readable plasma filaments.",
        )
    )
    assert result.status == "passed"
    assert result.analyzer_output is not None
    assert result.semantic_plan is not None
    assert len(result.stages) == 2
    assert result.stages[0].stage_status == "approved"
    assert result.stages[1].failure_classification is not None
    assert result.stages[1].failure_classification.failure_class == "shaping"
    assert result.stages[1].fix_loop_result is not None
    assert result.stages[1].stage_status == "repaired"


def run() -> None:
    test_visual_material_workflow_smoke()


if __name__ == "__main__":
    run()
    print("test_visual_material_workflow.py passed")
