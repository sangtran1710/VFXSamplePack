from __future__ import annotations

from typing import Any, Iterable

from ..models.critic_models import CriticResult
from ..models.planner_models import EstimatedImpact, PatchPlan, PatchStep
from .binding_resolver import resolve_mcp_binding
from .fix_selector import select_fixes
from .graph_influence_analyzer import analyze_graph_influence
from .lane_analyzer import LaneAssignment, assign_semantic_lane
from .op_generator import classify_fix_intent_family, generate_normalized_ops, infer_fix_direction, infer_fix_op_type
from .rollback_builder import build_rollback_ops
from .target_resolver import BLOCKED_FUZZY_OPS, ResolvedTarget, resolve_target_candidates


def _read_axis_scores(critic_result: CriticResult) -> dict[str, float]:
    if hasattr(critic_result.axis_scores, "root"):
        return dict(critic_result.axis_scores.root)
    return dict(critic_result.axis_scores)


def _estimate_impact(selected_fix_ids: list[str], critic_result: CriticResult) -> EstimatedImpact:
    axis_scores = _read_axis_scores(critic_result)
    selected = [fix for fix in critic_result.top_fixes if fix.fix_id in selected_fix_ids]
    if not selected:
        return EstimatedImpact(expected_score_delta=0.0, risk="low", rationale="No executable fixes selected.")
    expected_delta = round(
        sum((1.0 - axis_scores.get(fix.axis, 0.5)) * fix.confidence * 0.45 for fix in selected),
        4,
    )
    if any(fix.risk == "high" for fix in selected):
        risk = "high"
    elif any(fix.risk == "medium" for fix in selected):
        risk = "medium"
    else:
        risk = "low"
    return EstimatedImpact(
        expected_score_delta=expected_delta,
        risk=risk,
        rationale="Deterministic estimate from axis deficiencies and fix confidence.",
    )


def _fix_impact_score(fix, axis_scores: dict[str, float]) -> float:
    deficiency = 1.0 - axis_scores.get(fix.axis, 0.5)
    return deficiency * (0.7 + fix.confidence) + (6 - fix.priority) * 0.12


def _lane_dampening(
    lane_assignment: LaneAssignment,
    fix,
    prior_plan_outcomes: list[dict[str, Any]],
) -> float:
    scale = 1.0
    direction = infer_fix_direction(fix)
    for outcome in prior_plan_outcomes:
        if str(outcome.get("lane", "")) != lane_assignment.lane:
            continue
        same_direction = str(outcome.get("direction", "")) == direction
        same_action = str(outcome.get("action_family", "")) == lane_assignment.action_family
        same_effect = str(outcome.get("effect_family", "")) == lane_assignment.effect_family
        status = str(outcome.get("status", ""))
        overshoot = bool(outcome.get("overshoot", False))
        if status in {"regressed", "rolled_back"} and same_direction:
            scale *= 0.35 if same_action else 0.5
        elif overshoot and same_direction:
            scale *= 0.4 if same_effect else 0.55
        elif status in {"warning", "partial", "plateau"} and same_action:
            scale *= 0.75
    return scale


def _lane_conflict_reason(
    lane_assignment: LaneAssignment,
    lane_usage: dict[str, dict[str, Any]],
) -> str | None:
    lane_state = lane_usage.get(lane_assignment.lane)
    if lane_state is None:
        return None
    if lane_state["total_count"] >= 2:
        return "lane_budget"
    if lane_assignment.effect_family in lane_state["effect_families"]:
        return "lane_effect_duplicate"
    if lane_assignment.action_family == "scalar" and lane_state["scalar_count"] >= 1:
        return "lane_scalar_already_pushed"
    if lane_assignment.action_family == "vector" and lane_state["vector_count"] >= 1:
        return "lane_vector_already_adjusted"
    return None


def _register_lane_use(lane_assignment: LaneAssignment, lane_usage: dict[str, dict[str, Any]]) -> None:
    lane_state = lane_usage.setdefault(
        lane_assignment.lane,
        {"total_count": 0, "scalar_count": 0, "vector_count": 0, "effect_families": set()},
    )
    lane_state["total_count"] += 1
    if lane_assignment.action_family == "scalar":
        lane_state["scalar_count"] += 1
    if lane_assignment.action_family == "vector":
        lane_state["vector_count"] += 1
    lane_state["effect_families"].add(lane_assignment.effect_family)


def _candidate_op_types(fix) -> list[str]:
    primary = infer_fix_op_type(fix)
    if primary not in {"set_scalar", "set_vector"}:
        return [primary]
    alternate = "set_vector" if primary == "set_scalar" else "set_scalar"
    return [primary, alternate]


def _normalized_scalar(value: Any) -> float | None:
    if isinstance(value, (int, float)):
        return float(value)
    return None


def _normalized_vector(value: Any) -> list[float] | None:
    if isinstance(value, list) and len(value) >= 3:
        return [float(component) for component in value[:4]]
    return None


def _clamped_scalar_value(resolved_target: ResolvedTarget, value: float) -> float:
    slider_min = resolved_target.metadata.get("slider_min")
    slider_max = resolved_target.metadata.get("slider_max")
    lower = float(slider_min) if isinstance(slider_min, (int, float)) else 0.0
    if isinstance(slider_max, (int, float)) and float(slider_max) > lower:
        return round(max(lower, min(float(slider_max), value)), 4)
    return round(max(lower, value), 4)


def _clamped_vector_value(value: Iterable[float]) -> list[float]:
    values = [float(component) for component in value]
    while len(values) < 4:
        values.append(1.0)
    return [round(max(0.0, min(1.0, component)), 4) for component in values[:4]]


def _effective_delta(current_value: Any, clamped_value: Any) -> float:
    current_scalar = _normalized_scalar(current_value)
    clamped_scalar = _normalized_scalar(clamped_value)
    if current_scalar is not None and clamped_scalar is not None:
        return round(abs(clamped_scalar - current_scalar), 4)

    current_vector = _normalized_vector(current_value)
    clamped_vector = _normalized_vector(clamped_value)
    if current_vector is not None and clamped_vector is not None:
        delta = sum(abs(clamped_vector[index] - current_vector[index]) for index in range(3)) / 3.0
        return round(delta, 4)
    return 0.0


def _headroom_score(resolved_target: ResolvedTarget, direction: str) -> float:
    current_scalar = _normalized_scalar(resolved_target.current_value)
    if current_scalar is not None:
        slider_min = resolved_target.metadata.get("slider_min")
        slider_max = resolved_target.metadata.get("slider_max")
        lower = float(slider_min) if isinstance(slider_min, (int, float)) else 0.0
        if isinstance(slider_max, (int, float)) and float(slider_max) > lower:
            upper = float(slider_max)
            available = (upper - current_scalar) if direction == "increase" else (current_scalar - lower)
            total_range = max(0.001, upper - lower)
            return round(max(0.0, min(1.0, available / total_range)), 4)
        if direction == "increase":
            if current_scalar < 0.15:
                return 1.0
            if current_scalar < 0.4:
                return 0.85
            if current_scalar < 0.8:
                return 0.65
            if current_scalar < 1.5:
                return 0.4
            return 0.18
        return round(max(0.0, min(1.0, current_scalar / max(current_scalar + 0.5, 1.0))), 4)

    current_vector = _normalized_vector(resolved_target.current_value)
    if current_vector is not None:
        rgb = current_vector[:3]
        if direction == "decrease":
            return round(sum(rgb) / 3.0, 4)
        return round(sum(1.0 - component for component in rgb) / 3.0, 4)
    return 0.0


def _value_signature(value: Any) -> tuple[float, ...] | None:
    scalar = _normalized_scalar(value)
    if scalar is not None:
        return (round(scalar, 4),)
    vector = _normalized_vector(value)
    if vector is not None:
        return tuple(round(component, 4) for component in vector[:4])
    return None


def _is_same_effective_value(left: Any, right: Any) -> bool:
    left_signature = _value_signature(left)
    right_signature = _value_signature(right)
    if left_signature is None or right_signature is None or len(left_signature) != len(right_signature):
        return False
    return all(abs(left_signature[index] - right_signature[index]) <= 0.015 for index in range(len(left_signature)))


def _semantic_match_score(fix, resolved_target: ResolvedTarget, op_type: str) -> float:
    target_name = resolved_target.display_name.lower()
    hint = str(fix.target_hint or "").lower()
    intent_family = classify_fix_intent_family(fix)
    score = 0.0

    if hint and hint in target_name:
        score += 1.8
    if intent_family == "scalar_like":
        if any(token in target_name for token in ("intensity", "power", "strength", "amount", "multiplier", "emissive")):
            score += 2.0
        if op_type == "set_scalar":
            score += 0.9
        if op_type == "set_vector":
            score -= 1.0
    elif intent_family == "vector_like":
        if any(token in target_name for token in ("tint", "color", "hue", "rgb")):
            score += 2.0
        if op_type == "set_vector":
            score += 0.9
        if op_type == "set_scalar":
            score -= 1.0

    if fix.axis == "value_match" and any(token in target_name for token in ("emissive", "intensity", "power", "strength", "glow")):
        score += 1.35
    if fix.axis == "color_match" and any(token in target_name for token in ("tint", "color", "basecolor", "glow")):
        score += 1.25
    if fix.axis == "contrast_match" and any(token in target_name for token in ("contrast", "roughness", "power", "mask")):
        score += 1.0
    return round(score, 4)


def _influence_bonus(resolved_target: ResolvedTarget) -> float:
    influence = str(resolved_target.metadata.get("influence", "none"))
    if influence == "high":
        return 1.0
    if influence == "medium":
        return 0.45
    return 0.0


def _saturation_penalty(fix, resolved_target: ResolvedTarget, clamped_value: Any, effective_delta: float) -> float:
    penalty = 0.0
    current_vector = _normalized_vector(resolved_target.current_value)
    clamped_vector = _normalized_vector(clamped_value)
    if current_vector is not None and clamped_vector is not None:
        mean_value = sum(clamped_vector[:3]) / 3.0
        uniformity = max(clamped_vector[:3]) - min(clamped_vector[:3])
        explicit_white = any(token in str(fix.instruction or "").lower() for token in ("white", "whiter", "hotter"))
        if mean_value > 0.92:
            penalty += 1.0
        if mean_value > 0.88 and uniformity < 0.08 and not explicit_white:
            penalty += 1.6
        if effective_delta < 0.05:
            penalty += 1.15
    else:
        if effective_delta < 0.02:
            penalty += 1.15
    return round(penalty, 4)


def _repeated_failure_penalty(
    resolved_target: ResolvedTarget,
    lane_assignment: LaneAssignment,
    direction: str,
    clamped_value: Any,
    prior_plan_outcomes: list[dict[str, Any]],
) -> tuple[float, bool]:
    penalty = 0.0
    degenerate_repeat = False
    target_name = resolved_target.display_name
    for outcome in prior_plan_outcomes:
        same_target = str(outcome.get("param_name", outcome.get("target", ""))) == target_name
        if not same_target:
            continue
        status = str(outcome.get("status", ""))
        same_direction = str(outcome.get("direction", "")) == direction
        if status in {"regressed", "rolled_back"}:
            penalty += 1.35
        elif status in {"plateau", "warning", "partial"}:
            penalty += 0.65
        if bool(outcome.get("overshoot", False)):
            penalty += 0.85
        if same_direction:
            penalty += 0.35
        same_lane = str(outcome.get("lane", "")) == lane_assignment.lane
        if same_lane and same_direction and _is_same_effective_value(outcome.get("default_value"), clamped_value):
            penalty += 2.4
            degenerate_repeat = True
    return round(penalty, 4), degenerate_repeat


def _rank_candidate(
    *,
    fix,
    resolved_target: ResolvedTarget,
    lane_assignment: LaneAssignment,
    op_type: str,
    proposed_value: Any,
    clamped_value: Any,
    effective_delta: float,
    magnitude_scale: float,
    prior_plan_outcomes: list[dict[str, Any]],
) -> dict[str, Any]:
    direction = infer_fix_direction(fix)
    headroom = _headroom_score(resolved_target, direction)
    semantic_match = _semantic_match_score(fix, resolved_target, op_type)
    saturation_penalty = _saturation_penalty(fix, resolved_target, clamped_value, effective_delta)
    repeated_failure_penalty, degenerate_repeat = _repeated_failure_penalty(
        resolved_target,
        lane_assignment,
        direction,
        clamped_value,
        prior_plan_outcomes,
    )
    effective_delta_score = round(min(1.8, effective_delta * (4.0 if op_type == "set_vector" else 2.5)), 4)
    influence_bonus = _influence_bonus(resolved_target)
    headroom_component = round(headroom * 1.7, 4)
    final_rank_score = round(
        semantic_match
        + headroom_component
        + effective_delta_score
        + influence_bonus
        - saturation_penalty
        - repeated_failure_penalty,
        4,
    )
    return {
        "lane": lane_assignment.lane,
        "target_parameter": resolved_target.display_name,
        "direction": direction,
        "current_value": resolved_target.current_value,
        "proposed_value": proposed_value,
        "clamped_value": clamped_value,
        "effective_delta": effective_delta,
        "headroom_score": round(headroom, 4),
        "semantic_match": semantic_match,
        "influence_bonus": influence_bonus,
        "saturation_penalty": saturation_penalty,
        "repeated_failure_penalty": repeated_failure_penalty,
        "degenerate_repeat": degenerate_repeat,
        "final_rank_score": final_rank_score,
        "action_family": lane_assignment.action_family,
        "effect_family": lane_assignment.effect_family,
        "magnitude_scale": round(magnitude_scale, 4),
        "resolver_tier": resolved_target.tier,
        "op_type": op_type,
    }


def _candidate_sort_key(candidate: dict[str, Any], axis_scores: dict[str, float]) -> tuple[float, float, int, str]:
    debug = candidate["debug"]
    fix = candidate["fix"]
    return (
        float(debug["final_rank_score"]),
        _fix_impact_score(fix, axis_scores),
        -fix.priority,
        fix.fix_id.lower(),
    )


def _build_fix_candidate(
    fix,
    *,
    asset_context: dict[str, Any],
    conservative_mode: bool,
    prior_plan_outcomes: list[dict[str, Any]],
) -> dict[str, Any] | None:
    candidate_debug: list[dict[str, Any]] = []
    best_candidate: dict[str, Any] | None = None

    for op_type in _candidate_op_types(fix):
        resolved_candidates = resolve_target_candidates(
            fix,
            asset_context,
            op_type,
            conservative_mode=conservative_mode,
        )
        for resolved_target in resolved_candidates:
            if resolved_target.tier == "unresolved":
                continue
            if conservative_mode and resolved_target.tier == "fuzzy":
                continue
            if resolved_target.tier == "fuzzy" and op_type in BLOCKED_FUZZY_OPS:
                continue

            lane_assignment = assign_semantic_lane(fix, resolved_target, asset_context, op_type)
            magnitude_scale = _lane_dampening(lane_assignment, fix, list(prior_plan_outcomes))
            normalized_ops = generate_normalized_ops(
                fix,
                resolved_target,
                asset_context,
                step_index=1,
                conservative_mode=conservative_mode,
                magnitude_scale=magnitude_scale,
            )
            if not normalized_ops:
                continue

            op = normalized_ops[0]
            proposed_value = op.params.get("default_value", op.params.get("texture_path"))
            clamped_value = proposed_value
            if op.op_type == "set_scalar" and isinstance(proposed_value, (int, float)):
                clamped_value = _clamped_scalar_value(resolved_target, float(proposed_value))
            elif op.op_type == "set_vector" and isinstance(proposed_value, list):
                clamped_value = _clamped_vector_value(proposed_value)
            effective_delta = _effective_delta(resolved_target.current_value, clamped_value)
            debug = _rank_candidate(
                fix=fix,
                resolved_target=resolved_target,
                lane_assignment=lane_assignment,
                op_type=op.op_type,
                proposed_value=proposed_value,
                clamped_value=clamped_value,
                effective_delta=effective_delta,
                magnitude_scale=magnitude_scale,
                prior_plan_outcomes=prior_plan_outcomes,
            )
            candidate_debug.append(debug)
            candidate = {
                "fix": fix,
                "resolved_target": resolved_target,
                "lane_assignment": lane_assignment,
                "magnitude_scale": magnitude_scale,
                "debug": debug,
                "candidate_debug": candidate_debug,
            }
            if best_candidate is None or _candidate_sort_key(candidate, {fix.axis: 0.0}) > _candidate_sort_key(best_candidate, {fix.axis: 0.0}):
                best_candidate = candidate

    if best_candidate is None:
        return None
    best_candidate["candidate_debug"] = sorted(candidate_debug, key=lambda entry: float(entry["final_rank_score"]), reverse=True)
    return best_candidate


def generate_patch_plan(
    critic_result: CriticResult,
    asset_context: dict[str, Any],
) -> PatchPlan:
    asset_context = dict(asset_context)
    asset_context.setdefault("graph_influence", analyze_graph_influence(asset_context))
    conservative_mode = bool(asset_context.get("conservative_mode", False))
    axis_scores = _read_axis_scores(critic_result)
    prior_plan_outcomes = asset_context.get("prior_plan_outcomes", [])
    selection = select_fixes(
        critic_result.top_fixes,
        axis_scores,
        prior_plan_outcomes,
        conservative_mode=conservative_mode,
        max_selected=3,
    )

    candidate_pool: list[dict[str, Any]] = []
    skipped_fix_ids = [entry.fix.fix_id for entry in selection.skipped]
    for fix in selection.selected:
        candidate = _build_fix_candidate(
            fix,
            asset_context=asset_context,
            conservative_mode=conservative_mode,
            prior_plan_outcomes=list(prior_plan_outcomes),
        )
        if candidate is None:
            skipped_fix_ids.append(fix.fix_id)
            continue
        candidate_pool.append(candidate)

    candidate_pool.sort(key=lambda candidate: _candidate_sort_key(candidate, axis_scores), reverse=True)

    max_steps = 2 if conservative_mode else 4
    steps: list[PatchStep] = []
    selected_fix_ids: list[str] = []
    lane_usage: dict[str, dict[str, Any]] = {}

    for candidate in candidate_pool:
        fix = candidate["fix"]
        lane_assignment = candidate["lane_assignment"]
        conflict_reason = _lane_conflict_reason(lane_assignment, lane_usage)
        if conflict_reason is not None:
            skipped_fix_ids.append(fix.fix_id)
            continue

        resolved_target = candidate["resolved_target"]
        magnitude_scale = candidate["magnitude_scale"]
        normalized_ops = generate_normalized_ops(
            fix,
            resolved_target,
            asset_context,
            step_index=len(steps) + 1,
            conservative_mode=conservative_mode,
            magnitude_scale=magnitude_scale,
        )
        if not normalized_ops:
            skipped_fix_ids.append(fix.fix_id)
            continue
        if len(steps) + len(normalized_ops) > max_steps:
            skipped_fix_ids.append(fix.fix_id)
            continue

        candidate_debug = list(candidate["candidate_debug"])
        debug = dict(candidate["debug"])
        for op in normalized_ops:
            op.params.setdefault("lane", lane_assignment.lane)
            op.params.setdefault("action_family", lane_assignment.action_family)
            op.params.setdefault("effect_family", lane_assignment.effect_family)
            op.params.setdefault("magnitude_scale", round(magnitude_scale, 4))
            op.params["current_value"] = resolved_target.current_value
            op.params["proposed_value"] = debug["proposed_value"]
            op.params["clamped_value"] = debug["clamped_value"]
            op.params["effective_delta"] = debug["effective_delta"]
            op.params["headroom_score"] = debug["headroom_score"]
            op.params["semantic_match"] = debug["semantic_match"]
            op.params["saturation_penalty"] = debug["saturation_penalty"]
            op.params["repeated_failure_penalty"] = debug["repeated_failure_penalty"]
            op.params["final_rank_score"] = debug["final_rank_score"]
            op.params["degenerate_repeat"] = debug["degenerate_repeat"]
            op.params["candidate_rankings"] = candidate_debug
            steps.append(
                PatchStep(
                    step_id=op.op_id,
                    description=fix.instruction,
                    op=op,
                    binding=resolve_mcp_binding(op),
                    rollback_ops=build_rollback_ops(op),
                    risk=fix.risk,
                )
            )
        _register_lane_use(lane_assignment, lane_usage)
        selected_fix_ids.append(fix.fix_id)

    validation_checks: list[dict[str, Any]] = []
    if steps:
        validation_checks.append({"check": "compile_material", "severity": "critical"})
    for step in steps:
        if step.op.op_type in {"set_scalar", "set_vector", "set_texture"}:
            validation_checks.append(
                {
                    "check": "readback_param",
                    "param_name": step.op.params["param_name"],
                    "expected_value": step.op.params.get("default_value", step.op.params.get("texture_path")),
                }
            )

    return PatchPlan(
        selected_fix_ids=selected_fix_ids,
        skipped_fix_ids=sorted(set(skipped_fix_ids)),
        steps=steps,
        validation_checks=validation_checks,
        estimated_impact=_estimate_impact(selected_fix_ids, critic_result),
        conservative_mode=conservative_mode,
    )
