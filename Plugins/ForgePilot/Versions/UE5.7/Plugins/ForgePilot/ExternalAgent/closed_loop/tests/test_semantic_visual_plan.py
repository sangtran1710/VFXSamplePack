from __future__ import annotations

from ExternalAgent.closed_loop.models.semantic_models import AnalyzerOutput, StageRecipeItem, VisualCue, VisualLayerSpec
from ExternalAgent.closed_loop.planner.semantic_visual_plan import build_material_visual_plan


def test_semantic_visual_plan_builds_stage_lanes() -> None:
    analyzer_output = AnalyzerOutput(
        archetype="emissive energy orb",
        material_family="emissive_energy",
        layer_stack=[
            VisualLayerSpec(layer_id="core", label="Core", role="core", priority=1, description="hot center", desired_read="white-hot center", must_preserve=True),
            VisualLayerSpec(layer_id="breakup", label="Breakup", role="breakup", priority=2, description="filaments", desired_read="readable filaments", must_preserve=True),
        ],
        dominant_read="White-hot orb with readable filaments.",
        secondary_reads=[],
        stage_recipe=[
            StageRecipeItem(stage_id="stage_1_core", label="Core", goal="Build the core", target_layer="core", allowed_node_families=["radial_mask"], forbidden_node_families=[], validation_rules=["center"], expected_visual_change="core appears", stop_conditions=[]),
            StageRecipeItem(stage_id="stage_2_filament", label="Filament", goal="Build filaments", target_layer="breakup", allowed_node_families=["noise", "power"], forbidden_node_families=[], validation_rules=["filament"], expected_visual_change="filaments appear", stop_conditions=[]),
        ],
        hard_constraints=["keep center hottest"],
        anti_patterns=["cloudy orb"],
        failure_sensitive_constraints=["avoid color-only fixes"],
        visual_cues=[VisualCue(cue_id="core", description="center hottest", importance="critical", layer="core")],
    )
    plan = build_material_visual_plan(analyzer_output)
    assert len(plan.stages) == 2
    assert plan.stages[0].preferred_lanes == ["emissive_visual", "core_shape"]
    assert plan.stages[1].preferred_lanes == ["emissive_visual", "breakup"]


def run() -> None:
    test_semantic_visual_plan_builds_stage_lanes()


if __name__ == "__main__":
    run()
    print("test_semantic_visual_plan.py passed")
