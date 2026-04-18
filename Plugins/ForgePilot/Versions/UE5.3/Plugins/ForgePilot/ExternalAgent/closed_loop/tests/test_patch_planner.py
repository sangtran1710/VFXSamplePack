from __future__ import annotations

from ExternalAgent.closed_loop.models.critic_models import CriticAxisScores, CriticFix, CriticResult
from ExternalAgent.closed_loop.planner.graph_influence_analyzer import analyze_graph_influence
from ExternalAgent.closed_loop.planner.fix_selector import select_fixes
from ExternalAgent.closed_loop.planner.lane_analyzer import assign_semantic_lane
from ExternalAgent.closed_loop.planner.op_generator import (
    classify_fix_intent_family,
    ensure_target_compatible_op_type,
    generate_normalized_ops,
    infer_fix_op_type,
)
from ExternalAgent.closed_loop.planner.patch_planner import generate_patch_plan
from ExternalAgent.closed_loop.planner.target_resolver import ResolvedTarget, resolve_target


def make_critic_result() -> CriticResult:
    return CriticResult(
        summary="Planner test critic result.",
        overall_score=0.55,
        axis_scores=CriticAxisScores(
            {
                "value_match": 0.4,
                "color_match": 0.52,
                "contrast_match": 0.31,
            }
        ),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="raise_emissive",
                axis="value_match",
                instruction="Increase emissive intensity on the glow scalar.",
                rationale="The probe is too dark.",
                priority=1,
                confidence=0.92,
                risk="medium",
                target_hint="EmissiveIntensity",
                expected_effect="Raise the overall brightness.",
            ),
            CriticFix(
                fix_id="boost_contrast",
                axis="contrast_match",
                instruction="Increase contrast in the mask multiply chain.",
                rationale="The material reads too flat.",
                priority=2,
                confidence=0.85,
                risk="medium",
                target_hint="GlowMaskMultiply",
                expected_effect="Sharpen bright-dark separation.",
            ),
            CriticFix(
                fix_id="rebalance_tint",
                axis="color_match",
                instruction="Reduce saturation in the shell tint color.",
                rationale="The hue is too hot.",
                priority=2,
                confidence=0.8,
                risk="low",
                target_hint="ShellTintColor",
                expected_effect="Pull the palette back toward the target.",
            ),
            CriticFix(
                fix_id="secondary_value",
                axis="value_match",
                instruction="Increase secondary value scalar.",
                rationale="Another value lane could be brighter.",
                priority=3,
                confidence=0.7,
                risk="low",
                target_hint="SecondaryValue",
                expected_effect="Lift supporting luminance.",
            ),
        ],
        passed=False,
        critic_version="v2",
    )


ASSET_CONTEXT = {
    "material_path": "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
    "parameters": {
        "scalar": [
            {"name": "EmissiveIntensity", "value": 2.0},
            {"name": "GlowPower", "value": 4.0},
        ],
        "vector": [
            {"name": "ShellTintColor", "value": [1.0, 0.5, 0.2, 1.0]},
        ],
        "texture": [
            {"name": "NoiseTex", "value": "/Game/VFX/T_Noise.T_Noise"},
        ],
    },
    "expressions": [
        {"index": 11, "name": "GlowMaskMultiply", "type": "Multiply", "desc": "mask multiply chain"},
        {"index": 7, "name": "FresnelEdge", "type": "Fresnel", "desc": "rim edge"},
    ],
    "root_inputs": {
        "EmissiveColor": {"source_index": 11},
    },
}


INFLUENCE_CONTEXT = {
    "material_path": "/Game/VFX/Test/M_Influence.M_Influence",
    "parameters": {
        "scalar": [
            {"name": "EmissiveIntensity", "value": 0.15, "expression_index": 1},
            {"name": "UnusedBrightness", "value": 2.0, "expression_index": 5},
        ],
        "vector": [
            {"name": "GlowTint", "value": [0.0, 0.6, 1.0, 1.0], "expression_index": 2},
            {"name": "UnusedTint", "value": [1.0, 0.0, 0.0, 1.0], "expression_index": 6},
        ],
    },
    "expressions": [
        {
            "index": 1,
            "name": "EmissiveIntensity",
            "type": "MaterialExpressionScalarParameter",
            "inputs": [],
        },
        {
            "index": 2,
            "name": "GlowTint",
            "type": "MaterialExpressionVectorParameter",
            "inputs": [],
        },
        {
            "index": 3,
            "name": "GlowMultiply",
            "type": "MaterialExpressionMultiply",
            "inputs": [
                {"connected": True, "source_index": 1},
                {"connected": True, "source_index": 2},
            ],
        },
        {
            "index": 5,
            "name": "UnusedBrightness",
            "type": "MaterialExpressionScalarParameter",
            "inputs": [],
        },
        {
            "index": 6,
            "name": "UnusedTint",
            "type": "MaterialExpressionVectorParameter",
            "inputs": [],
        },
    ],
    "root_inputs": [
        {"pin": "EmissiveColor", "connected": True, "source_index": 3},
    ],
}


SATURATED_EMISSIVE_CONTEXT = {
    "material_path": "/Game/VFX/Test/M_Saturated.M_Saturated",
    "parameters": {
        "scalar": [
            {"name": "EmissiveIntensity", "value": 0.12, "expression_index": 1},
        ],
        "vector": [
            {"name": "GlowTint", "default_value_rgba": [0.96, 0.97, 1.0, 1.0], "expression_index": 2, "kind": "vector"},
        ],
    },
    "expressions": [
        {"index": 1, "name": "EmissiveIntensity", "type": "MaterialExpressionScalarParameter", "inputs": []},
        {"index": 2, "name": "GlowTint", "type": "MaterialExpressionVectorParameter", "inputs": []},
        {
            "index": 3,
            "name": "GlowMultiply",
            "type": "MaterialExpressionMultiply",
            "inputs": [
                {"connected": True, "source_index": 1},
                {"connected": True, "source_index": 2},
            ],
        },
    ],
    "root_inputs": [
        {"pin": "EmissiveColor", "connected": True, "source_index": 3},
    ],
}


def test_fix_selection() -> None:
    critic_result = make_critic_result()
    selection = select_fixes(critic_result.top_fixes, critic_result.axis_scores.root)
    assert [fix.fix_id for fix in selection.selected] == [
        "raise_emissive",
        "boost_contrast",
        "rebalance_tint",
    ]


def test_target_resolution_tiers() -> None:
    critic_result = make_critic_result()
    direct_param = resolve_target(critic_result.top_fixes[0], ASSET_CONTEXT, "set_scalar")
    assert direct_param.tier == "direct_parameter"
    assert direct_param.display_name == "EmissiveIntensity"

    direct_expr = resolve_target(critic_result.top_fixes[1], ASSET_CONTEXT, "add_expression")
    assert direct_expr.tier == "direct_expression"
    assert direct_expr.identifier == "11"

    fuzzy_fix = CriticFix(
        fix_id="fuzzy_color",
        axis="color_match",
        instruction="Reduce saturation in the shell tint.",
        rationale="Color is too hot.",
        priority=2,
        confidence=0.8,
        risk="low",
        target_hint="ShellTint",
        expected_effect="Cool the tint.",
    )
    fuzzy = resolve_target(fuzzy_fix, ASSET_CONTEXT, "set_vector")
    assert fuzzy.tier == "fuzzy"

    create_fix = CriticFix(
        fix_id="new_scalar",
        axis="value_match",
        instruction="Increase the missing bloom scalar.",
        rationale="No existing scalar found.",
        priority=2,
        confidence=0.8,
        risk="low",
        target_hint="BloomBoost",
        expected_effect="Add a new scalar lane.",
    )
    created = resolve_target(create_fix, ASSET_CONTEXT, "set_scalar")
    assert created.tier == "create_fallback"


def test_fuzzy_restrictions() -> None:
    fuzzy_output_fix = CriticFix(
        fix_id="wire_output",
        axis="value_match",
        instruction="Connect shell glow output into emissive.",
        rationale="Output route is wrong.",
        priority=1,
        confidence=0.9,
        risk="medium",
        target_hint="ShellGlow",
        expected_effect="Wire the correct expression to the output.",
    )
    unresolved = resolve_target(fuzzy_output_fix, ASSET_CONTEXT, "set_material_output")
    assert unresolved.tier == "unresolved"

    fuzzy_scalar_fix = CriticFix(
        fix_id="fuzzy_scalar",
        axis="value_match",
        instruction="Increase emissive level.",
        rationale="Scalar needs a bump.",
        priority=1,
        confidence=0.9,
        risk="medium",
        target_hint="Emissive",
        expected_effect="Increase brightness.",
    )
    conservative = resolve_target(fuzzy_scalar_fix, ASSET_CONTEXT, "set_scalar", conservative_mode=True)
    assert conservative.tier == "create_fallback"


def test_patch_plan_shape() -> None:
    critic_result = make_critic_result()
    plan = generate_patch_plan(critic_result, ASSET_CONTEXT)
    assert set(plan.selected_fix_ids) == {"raise_emissive", "boost_contrast", "rebalance_tint"}
    assert len(plan.steps) == 3
    assert plan.steps[0].binding is not None
    assert plan.validation_checks[0]["check"] == "compile_material"
    assert "candidate_rankings" in plan.steps[0].op.params


def test_graph_influence_marks_connected_parameters_high() -> None:
    report = analyze_graph_influence(INFLUENCE_CONTEXT)
    assert report.parameter_influence["EmissiveIntensity"].level == "high"
    assert report.parameter_influence["GlowTint"].level == "high"
    assert report.parameter_influence["UnusedBrightness"].level == "low"


def test_connected_parameter_is_selected_before_fallback() -> None:
    brightness_fix = CriticFix(
        fix_id="boost_brightness",
        axis="value_match",
        instruction="Boost the main glow brightness.",
        rationale="The rendered result is too dim.",
        priority=1,
        confidence=0.9,
        risk="medium",
        target_hint=None,
        expected_effect="Brighter glow on the visible output path.",
    )
    resolved = resolve_target(brightness_fix, INFLUENCE_CONTEXT, "set_scalar")
    assert resolved.tier == "output_path_parameter"
    assert resolved.display_name == "EmissiveIntensity"

    plan = generate_patch_plan(
        CriticResult(
            summary="Influence-aware selection.",
            overall_score=0.35,
            axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.4, "contrast_match": 0.5}),
            deltas=[],
            top_fixes=[brightness_fix],
            passed=False,
            critic_version="v2",
        ),
        INFLUENCE_CONTEXT,
    )
    assert len(plan.steps) == 1
    assert plan.steps[0].op.params["param_name"] == "EmissiveIntensity"
    assert plan.steps[0].op.resolver_tier != "create_fallback"


def test_intensity_like_fix_prefers_scalar_op() -> None:
    fix = CriticFix(
        fix_id="intensity_fix",
        axis="value_match",
        instruction="Increase the EmissiveIntensity parameter to enhance brightness.",
        rationale="A higher emissive intensity will make the material appear more luminous.",
        priority=1,
        confidence=0.85,
        risk="medium",
        expected_effect="Stronger emissive glow.",
    )
    assert classify_fix_intent_family(fix) == "scalar_like"
    assert infer_fix_op_type(fix) == "set_scalar"


def test_tint_like_fix_prefers_vector_op() -> None:
    fix = CriticFix(
        fix_id="tint_fix",
        axis="color_match",
        instruction="Adjust the GlowTint parameter to a more vibrant blue.",
        rationale="To improve color vibrancy and tint separation.",
        priority=1,
        confidence=0.85,
        risk="medium",
        expected_effect="Brighter tint and saturation.",
    )
    assert classify_fix_intent_family(fix) == "vector_like"
    assert infer_fix_op_type(fix) == "set_vector"


def test_scalar_target_cannot_emit_vector_op() -> None:
    fix = CriticFix(
        fix_id="scalar_guard",
        axis="value_match",
        instruction="Increase emissive intensity on the glow scalar.",
        rationale="The probe is too dark.",
        priority=1,
        confidence=0.9,
        risk="medium",
        expected_effect="Raise brightness.",
    )
    resolved_target = ResolvedTarget(
        tier="output_path_parameter",
        op_type="set_vector",
        target_kind="parameter",
        identifier="EmissiveIntensity",
        display_name="EmissiveIntensity",
        confidence=0.9,
        current_value=0.15,
        metadata={"kind": "scalar"},
    )
    compatible = ensure_target_compatible_op_type(fix, resolved_target, "set_vector")
    assert compatible == "set_scalar"
    ops = generate_normalized_ops(fix, resolved_target, {"material_path": "/Game/VFX/Test/M_Scalar.M_Scalar"}, step_index=1)
    assert len(ops) == 1
    assert ops[0].op_type == "set_scalar"


def test_vector_target_cannot_emit_scalar_op() -> None:
    fix = CriticFix(
        fix_id="vector_guard",
        axis="color_match",
        instruction="Increase the glow tint vibrancy.",
        rationale="The color needs more saturation.",
        priority=1,
        confidence=0.9,
        risk="medium",
        expected_effect="More vibrant tint.",
    )
    resolved_target = ResolvedTarget(
        tier="output_path_parameter",
        op_type="set_scalar",
        target_kind="parameter",
        identifier="GlowTint",
        display_name="GlowTint",
        confidence=0.9,
        current_value=[0.0, 0.35, 0.8, 1.0],
        metadata={"kind": "vector"},
    )
    compatible = ensure_target_compatible_op_type(fix, resolved_target, "set_scalar")
    assert compatible == "set_vector"
    ops = generate_normalized_ops(fix, resolved_target, {"material_path": "/Game/VFX/Test/M_Vector.M_Vector"}, step_index=1)
    assert len(ops) == 1
    assert ops[0].op_type == "set_vector"


def test_lane_dedup_prefers_highest_impact_fix() -> None:
    critic_result = CriticResult(
        summary="Lane dedup test.",
        overall_score=0.3,
        axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.3, "contrast_match": 0.4}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="boost_emissive_primary",
                axis="value_match",
                instruction="Increase the EmissiveIntensity parameter strongly.",
                rationale="Primary brightness lane is too dim.",
                priority=1,
                confidence=0.95,
                risk="medium",
                expected_effect="Stronger glow.",
            ),
            CriticFix(
                fix_id="boost_emissive_secondary",
                axis="contrast_match",
                instruction="Increase emissive brightness slightly.",
                rationale="The same emissive lane could be brighter.",
                priority=3,
                confidence=0.7,
                risk="medium",
                expected_effect="Slightly brighter emissive glow.",
            ),
        ],
        passed=False,
        critic_version="v2",
    )
    plan = generate_patch_plan(critic_result, INFLUENCE_CONTEXT)
    assert "boost_emissive_primary" in plan.selected_fix_ids
    assert "boost_emissive_secondary" not in plan.selected_fix_ids
    assert len(plan.steps) == 1


def test_regression_history_dampens_next_magnitude() -> None:
    fix = CriticFix(
        fix_id="boost_brightness",
        axis="value_match",
        instruction="Increase the EmissiveIntensity parameter to enhance brightness.",
        rationale="The visible glow is too dim.",
        priority=1,
        confidence=0.9,
        risk="medium",
        expected_effect="Much brighter emissive glow.",
    )
    base_context = dict(INFLUENCE_CONTEXT)
    base_context["prior_plan_outcomes"] = []
    damped_context = dict(INFLUENCE_CONTEXT)
    damped_context["prior_plan_outcomes"] = [
        {
            "fix_id": "previous_emissive",
            "lane": "emissive_visual",
            "action_family": "scalar",
            "effect_family": "brightness",
            "direction": "increase",
            "status": "regressed",
            "overshoot": True,
        }
    ]
    base_plan = generate_patch_plan(
        CriticResult(
            summary="Base brightness push",
            overall_score=0.3,
            axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.5, "contrast_match": 0.5}),
            deltas=[],
            top_fixes=[fix],
            passed=False,
            critic_version="v2",
        ),
        base_context,
    )
    damped_plan = generate_patch_plan(
        CriticResult(
            summary="Damped brightness push",
            overall_score=0.3,
            axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.5, "contrast_match": 0.5}),
            deltas=[],
            top_fixes=[fix],
            passed=False,
            critic_version="v2",
        ),
        damped_context,
    )
    assert base_plan.steps[0].op.params["default_value"] > damped_plan.steps[0].op.params["default_value"]
    assert damped_plan.steps[0].op.params["magnitude_scale"] < 1.0


def test_brightness_overshoot_prevents_repeated_emissive_amplification() -> None:
    critic_result = CriticResult(
        summary="Overshoot control test.",
        overall_score=0.35,
        axis_scores=CriticAxisScores({"value_match": 0.25, "color_match": 0.35, "contrast_match": 0.45}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="increase_emissive",
                axis="value_match",
                instruction="Increase the EmissiveIntensity parameter strongly.",
                rationale="Boost brightness amount on the glow.",
                priority=1,
                confidence=0.92,
                risk="medium",
                expected_effect="Brighter glow.",
            ),
            CriticFix(
                fix_id="increase_glow_power",
                axis="contrast_match",
                instruction="Increase glow strength on the emissive lane.",
                rationale="The same glow lane still needs more punch.",
                priority=2,
                confidence=0.8,
                risk="medium",
                expected_effect="More emissive punch.",
            ),
        ],
        passed=False,
        critic_version="v2",
    )
    context = dict(INFLUENCE_CONTEXT)
    context["prior_plan_outcomes"] = [
        {
            "fix_id": "prior_emissive",
            "lane": "emissive_visual",
            "action_family": "scalar",
            "effect_family": "brightness",
            "direction": "increase",
            "status": "regressed",
            "overshoot": True,
        }
    ]
    plan = generate_patch_plan(critic_result, context)
    assert len(plan.steps) == 1
    assert plan.steps[0].op.params["param_name"] == "EmissiveIntensity"


def test_saturated_vector_loses_to_scalar() -> None:
    critic_result = CriticResult(
        summary="Saturated tint should lose to scalar.",
        overall_score=0.32,
        axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.4, "contrast_match": 0.45}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="boost_main_glow",
                axis="value_match",
                instruction="Boost the main glow brightness.",
                rationale="The emissive lane needs more energy.",
                priority=1,
                confidence=0.92,
                risk="medium",
                expected_effect="Brighter glow output.",
            )
        ],
        passed=False,
        critic_version="v2",
    )
    plan = generate_patch_plan(critic_result, SATURATED_EMISSIVE_CONTEXT)
    assert len(plan.steps) == 1
    assert plan.steps[0].op.params["param_name"] == "EmissiveIntensity"
    rankings = plan.steps[0].op.params["candidate_rankings"]
    assert rankings[0]["target_parameter"] == "EmissiveIntensity"
    assert any(entry["target_parameter"] == "GlowTint" for entry in rankings)


def test_effective_delta_penalty_down_ranks_near_zero_move() -> None:
    critic_result = CriticResult(
        summary="Near-clamped vector should be penalized.",
        overall_score=0.28,
        axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.3, "contrast_match": 0.5}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="boost_main_glow",
                axis="value_match",
                instruction="Boost the main glow brightness.",
                rationale="The emissive lane needs more energy.",
                priority=1,
                confidence=0.92,
                risk="medium",
                expected_effect="Brighter glow output.",
            )
        ],
        passed=False,
        critic_version="v2",
    )
    plan = generate_patch_plan(critic_result, SATURATED_EMISSIVE_CONTEXT)
    rankings = plan.steps[0].op.params["candidate_rankings"]
    scalar_entry = next(entry for entry in rankings if entry["target_parameter"] == "EmissiveIntensity")
    vector_entry = next(entry for entry in rankings if entry["target_parameter"] == "GlowTint")
    assert vector_entry["saturation_penalty"] > 0.0
    assert vector_entry["final_rank_score"] < scalar_entry["final_rank_score"]


def test_repeated_failed_target_suppression_prefers_alternative_target() -> None:
    critic_result = CriticResult(
        summary="Repeated failed vector target should be suppressed.",
        overall_score=0.28,
        axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.3, "contrast_match": 0.5}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="boost_main_glow",
                axis="value_match",
                instruction="Boost the main glow brightness.",
                rationale="The emissive lane needs more energy.",
                priority=1,
                confidence=0.92,
                risk="medium",
                expected_effect="Brighter glow output.",
            )
        ],
        passed=False,
        critic_version="v2",
    )
    context = dict(SATURATED_EMISSIVE_CONTEXT)
    context["prior_plan_outcomes"] = [
        {
            "lane": "emissive_visual",
            "param_name": "GlowTint",
            "direction": "increase",
            "action_family": "vector",
            "effect_family": "brightness",
            "status": "regressed",
            "overshoot": True,
            "default_value": [0.8888, 0.9064, 0.959, 1.0],
        }
    ]
    plan = generate_patch_plan(critic_result, context)
    assert plan.steps[0].op.params["param_name"] == "EmissiveIntensity"


def test_degenerate_repeat_prevention_avoids_same_failed_vector_action() -> None:
    critic_result = CriticResult(
        summary="Do not repeat the same failed saturated vector move.",
        overall_score=0.26,
        axis_scores=CriticAxisScores({"value_match": 0.2, "color_match": 0.35, "contrast_match": 0.45}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="boost_main_glow",
                axis="value_match",
                instruction="Boost the main glow brightness.",
                rationale="The emissive lane needs more energy.",
                priority=1,
                confidence=0.92,
                risk="medium",
                expected_effect="Brighter glow output.",
            )
        ],
        passed=False,
        critic_version="v2",
    )
    context = dict(SATURATED_EMISSIVE_CONTEXT)
    context["prior_plan_outcomes"] = [
        {
            "lane": "emissive_visual",
            "param_name": "GlowTint",
            "direction": "increase",
            "action_family": "vector",
            "effect_family": "brightness",
            "status": "regressed",
            "overshoot": True,
            "default_value": [0.8888, 0.9064, 0.959, 1.0],
        }
    ]
    plan = generate_patch_plan(critic_result, context)
    assert plan.steps[0].op.params["param_name"] != "GlowTint"
    rankings = plan.steps[0].op.params["candidate_rankings"]
    vector_entry = next(entry for entry in rankings if entry["target_parameter"] == "GlowTint")
    assert vector_entry["degenerate_repeat"] is True


def test_vector_shaping_avoids_full_white_push() -> None:
    fix = CriticFix(
        fix_id="vector_brightness",
        axis="value_match",
        instruction="Increase glow brightness on the tint lane.",
        rationale="The tint contribution should be brighter without washing out the hue.",
        priority=1,
        confidence=0.9,
        risk="medium",
        expected_effect="Brighter emissive tint.",
    )
    resolved_target = ResolvedTarget(
        tier="output_path_parameter",
        op_type="set_vector",
        target_kind="parameter",
        identifier="GlowTint",
        display_name="GlowTint",
        confidence=0.9,
        current_value=[0.0, 0.35, 0.8, 1.0],
        metadata={"kind": "vector", "influence": "high"},
    )
    ops = generate_normalized_ops(fix, resolved_target, {"material_path": "/Game/VFX/Test/M_Vector.M_Vector"}, step_index=1)
    assert len(ops) == 1
    assert ops[0].op_type == "set_vector"
    assert ops[0].params["default_value"] != [1.0, 1.0, 1.0, 1.0]
    assert sum(1 for component in ops[0].params["default_value"][:3] if component >= 0.999) < 3
    assert max(ops[0].params["default_value"][:3]) - min(ops[0].params["default_value"][:3]) > 0.05


def run() -> None:
    test_fix_selection()
    test_target_resolution_tiers()
    test_fuzzy_restrictions()
    test_patch_plan_shape()
    test_graph_influence_marks_connected_parameters_high()
    test_connected_parameter_is_selected_before_fallback()
    test_intensity_like_fix_prefers_scalar_op()
    test_tint_like_fix_prefers_vector_op()
    test_scalar_target_cannot_emit_vector_op()
    test_vector_target_cannot_emit_scalar_op()
    test_lane_dedup_prefers_highest_impact_fix()
    test_regression_history_dampens_next_magnitude()
    test_brightness_overshoot_prevents_repeated_emissive_amplification()
    test_saturated_vector_loses_to_scalar()
    test_effective_delta_penalty_down_ranks_near_zero_move()
    test_repeated_failed_target_suppression_prefers_alternative_target()
    test_degenerate_repeat_prevention_avoids_same_failed_vector_action()
    test_vector_shaping_avoids_full_white_push()


if __name__ == "__main__":
    run()
    print("test_patch_planner.py passed")
