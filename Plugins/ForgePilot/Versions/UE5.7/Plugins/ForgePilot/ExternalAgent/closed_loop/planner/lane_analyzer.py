from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Mapping

from ..models.critic_models import CriticFix
from .graph_influence_analyzer import GraphInfluenceReport
from .op_generator import classify_fix_intent_family, infer_fix_direction
from .target_resolver import ResolvedTarget


@dataclass(slots=True)
class LaneAssignment:
    lane: str
    action_family: str
    effect_family: str
    target_name: str


def _parameter_output_pins(asset_context: Mapping[str, Any], target_name: str) -> list[str]:
    report = asset_context.get("graph_influence")
    if isinstance(report, GraphInfluenceReport):
        influence = report.parameter_influence.get(target_name)
        if influence is not None:
            return list(influence.output_pins)
    return []


def _infer_lane_from_name(target_name: str, output_pins: list[str]) -> str:
    normalized = target_name.lower()
    if any(token in normalized for token in ("emissive", "glow", "light", "bloom")):
        return "emissive_visual"
    if any(token in normalized for token in ("basecolor", "base_color", "albedo", "tint", "color")):
        return "base_color"
    if any(token in normalized for token in ("rim", "fresnel", "edge")):
        return "edge_rim"
    if any(token in normalized for token in ("roughness", "specular", "gloss", "contrast")):
        return "surface_response"
    if any(token in normalized for token in ("noise", "mask", "breakup", "distort", "blend", "panner")):
        return "surface_breakup"
    if "EmissiveColor" in output_pins:
        return "emissive_visual"
    if "BaseColor" in output_pins:
        return "base_color"
    return "generic_output"


def infer_effect_family(fix: CriticFix) -> str:
    haystack = " ".join(filter(None, [fix.target_hint, fix.instruction, fix.rationale, fix.expected_effect, fix.axis])).lower()
    if any(token in haystack for token in ("brightness", "emissive", "intensity", "value", "glow", "luminous", "light")):
        return "brightness"
    if any(token in haystack for token in ("color", "tint", "hue", "saturation", "vibrancy", "rgb", "blue", "red", "green")):
        return "vibrancy"
    if any(token in haystack for token in ("contrast", "roughness", "specular", "gloss")):
        return "contrast"
    if any(token in haystack for token in ("noise", "mask", "breakup", "blend", "distort", "fresnel", "rim")):
        return "structure"
    if any(token in haystack for token in ("texture", "flipbook", "normal map")):
        return "texture"
    return "generic"


def assign_semantic_lane(
    fix: CriticFix,
    resolved_target: ResolvedTarget,
    asset_context: Mapping[str, Any],
    op_type: str,
) -> LaneAssignment:
    target_name = resolved_target.display_name or resolved_target.identifier or fix.fix_id
    output_pins = _parameter_output_pins(asset_context, target_name)
    lane = _infer_lane_from_name(target_name, output_pins)
    action_family = {
        "set_scalar": "scalar",
        "set_vector": "vector",
        "set_texture": "texture",
        "add_expression": "graph",
        "set_material_output": "graph",
    }.get(op_type, "generic")
    intent_family = classify_fix_intent_family(fix)
    effect_family = infer_effect_family(fix)
    if intent_family == "graph_structural":
        effect_family = "structure"
    elif intent_family == "texture_like":
        effect_family = "texture"
    return LaneAssignment(
        lane=lane,
        action_family=action_family,
        effect_family=effect_family,
        target_name=target_name,
    )


def detect_overshoot_signal(critic_result) -> bool:
    summary = str(getattr(critic_result, "summary", "")).lower()
    overshoot_terms = ("too bright", "lighter than target", "oversaturated", "too saturated", "higher than target", "too light")
    if any(term in summary for term in overshoot_terms):
        return True
    for delta in getattr(critic_result, "deltas", []):
        issue = str(getattr(delta, "issue", "")).lower()
        observation = str(getattr(delta, "observation", "")).lower()
        direction = str(getattr(delta, "direction", "")).lower()
        if direction == "decrease" and any(term in f"{issue} {observation}" for term in ("lighter", "too bright", "oversaturated", "higher than")):
            return True
    return False


def build_prior_outcome(
    *,
    fix_id: str,
    lane_assignment: LaneAssignment,
    status: str,
    direction: str,
    overshoot: bool = False,
    magnitude_scale: float = 1.0,
) -> dict[str, object]:
    return {
        "fix_id": fix_id,
        "lane": lane_assignment.lane,
        "action_family": lane_assignment.action_family,
        "effect_family": lane_assignment.effect_family,
        "direction": direction,
        "status": status,
        "overshoot": overshoot,
        "magnitude_scale": magnitude_scale,
    }


def current_fix_direction(fix: CriticFix) -> str:
    return infer_fix_direction(fix)
