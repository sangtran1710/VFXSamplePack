from __future__ import annotations

from typing import Literal
from typing import Any, Mapping

from ..models.critic_models import CriticFix
from ..models.planner_models import NormalizedOp, NormalizedOpType
from .target_resolver import ResolvedTarget


IntentFamily = Literal["scalar_like", "vector_like", "graph_structural", "texture_like", "unknown"]

SCALAR_HINT_TOKENS = {
    "amount",
    "boost",
    "brightness",
    "contrast amount",
    "emissiveintensity",
    "glow amount",
    "glow strength",
    "intensity",
    "level",
    "multiplier",
    "power",
    "strength",
}

VECTOR_HINT_TOKENS = {
    "basecolor",
    "color",
    "glowtint",
    "hue",
    "rgb",
    "saturation",
    "tint",
    "value color",
    "vibrancy",
}

GRAPH_HINT_TOKENS = {
    "blend",
    "breakup",
    "distortion",
    "expression",
    "fresnel",
    "lerp",
    "mask",
    "multiply chain",
    "noise",
    "node",
    "panner",
}

POSITIVE_ACTION_TOKENS = {
    "boost",
    "brighten",
    "enhance",
    "increase",
    "improve",
    "lift",
    "more",
    "raise",
    "stronger",
}

NEGATIVE_ACTION_TOKENS = {
    "decrease",
    "lower",
    "reduce",
    "soften",
}

SMALL_PRESET = 0.16
MEDIUM_PRESET = 0.28
STRONG_PRESET = 0.42


def _fix_haystack(fix: CriticFix) -> str:
    return " ".join(filter(None, [fix.target_hint, fix.instruction, fix.rationale, fix.expected_effect, fix.axis])).lower()


def _contains_any(haystack: str, tokens: set[str]) -> bool:
    return any(token in haystack for token in tokens)


def classify_fix_intent_family(fix: CriticFix) -> IntentFamily:
    haystack = _fix_haystack(fix)
    if any(token in haystack for token in ("texture", "flipbook", "normal map", "noise map")):
        return "texture_like"
    if any(token in haystack for token in ("wire", "route", "material pin", "connect", "output pin", "reroute")):
        return "graph_structural"
    if _contains_any(haystack, SCALAR_HINT_TOKENS):
        return "scalar_like"
    if _contains_any(haystack, VECTOR_HINT_TOKENS):
        return "vector_like"
    if _contains_any(haystack, GRAPH_HINT_TOKENS):
        return "graph_structural"
    if fix.axis == "color_match":
        return "vector_like"
    if fix.axis in {"value_match", "contrast_match"}:
        return "scalar_like"
    return "unknown"


def _resolved_parameter_kind(resolved_target: ResolvedTarget) -> str | None:
    parameter_kind = str(resolved_target.metadata.get("kind", "")).lower()
    if parameter_kind in {"scalar", "vector", "texture"}:
        return parameter_kind
    if isinstance(resolved_target.current_value, (int, float)):
        return "scalar"
    if isinstance(resolved_target.current_value, list):
        return "vector"
    if isinstance(resolved_target.current_value, str):
        return "texture"
    return None


def infer_fix_op_type(fix: CriticFix, resolved_target: ResolvedTarget | None = None) -> str:
    if resolved_target is not None and resolved_target.target_kind == "parameter":
        parameter_kind = _resolved_parameter_kind(resolved_target)
        if parameter_kind == "scalar":
            return "set_scalar"
        if parameter_kind == "vector":
            return "set_vector"
        if parameter_kind == "texture":
            return "set_texture"

    haystack = _fix_haystack(fix)
    intent_family = classify_fix_intent_family(fix)
    if any(token in haystack for token in ("wire", "route", "material pin", "connect", "output pin", "reroute")):
        return "set_material_output"
    if intent_family == "texture_like":
        return "set_texture"
    if intent_family == "graph_structural":
        return "add_expression"
    if intent_family == "scalar_like":
        return "set_scalar"
    if intent_family == "vector_like":
        return "set_vector"
    if any(token in haystack for token in ("color", "tint", "hue", "saturation", "rgb", "vector")):
        return "set_vector"
    if any(token in haystack for token in ("fresnel", "panner", "noise", "node", "expression", "mask chain", "multiply chain", "lerp")):
        return "add_expression"
    return "set_scalar"


def _clamp(value: float, low: float, high: float) -> float:
    return max(low, min(high, value))


def _magnitude_preset(fix: CriticFix) -> float:
    if fix.priority <= 1 or fix.confidence >= 0.88:
        return STRONG_PRESET
    if fix.priority == 2 or fix.confidence >= 0.75:
        return MEDIUM_PRESET
    return SMALL_PRESET


def _operation_direction(fix: CriticFix) -> str:
    directive_haystack = " ".join(filter(None, [fix.target_hint, fix.instruction, fix.expected_effect])).lower()
    context_haystack = str(fix.rationale or "").lower()
    if _contains_any(directive_haystack, NEGATIVE_ACTION_TOKENS) or "dim " in directive_haystack or "dimmer" in directive_haystack:
        return "decrease"
    if _contains_any(directive_haystack, POSITIVE_ACTION_TOKENS):
        return "increase"
    if fix.axis == "value_match":
        return "increase"
    if _contains_any(context_haystack, POSITIVE_ACTION_TOKENS):
        return "increase"
    if _contains_any(context_haystack, NEGATIVE_ACTION_TOKENS):
        return "decrease"
    return "increase"


def infer_fix_direction(fix: CriticFix) -> str:
    return _operation_direction(fix)


def _compute_scalar_value(fix: CriticFix, current_value: Any) -> float:
    return _compute_scalar_value_scaled(fix, current_value, magnitude_scale=1.0)


def _compute_scalar_value_scaled(fix: CriticFix, current_value: Any, *, magnitude_scale: float) -> float:
    current = float(current_value) if isinstance(current_value, (int, float)) else 1.0
    preset = _magnitude_preset(fix)
    intensity_boost = 1.0
    if classify_fix_intent_family(fix) == "scalar_like":
        intensity_boost = 1.15
    step = max(preset, current * (0.9 if current < 0.25 else 0.45)) * intensity_boost * max(0.15, magnitude_scale)
    if _operation_direction(fix) == "increase":
        return round(current + step, 4)
    if _operation_direction(fix) == "decrease":
        return round(max(0.0, current - step), 4)
    return round(current, 4)


def _compute_vector_value(fix: CriticFix, current_value: Any) -> list[float]:
    return _compute_vector_value_scaled(fix, current_value, magnitude_scale=1.0)


def _compute_vector_value_scaled(fix: CriticFix, current_value: Any, *, magnitude_scale: float) -> list[float]:
    if isinstance(current_value, list) and len(current_value) >= 4:
        current = [float(component) for component in current_value[:4]]
    else:
        current = [1.0, 1.0, 1.0, 1.0]
    preset = _magnitude_preset(fix)
    scale = max(0.15, magnitude_scale)
    saturation_delta = min(0.5, max(0.18, preset * 0.7 * scale))
    value_delta = min(0.32, max(0.08, preset * 0.55 * scale))
    current_rgb = current[:3]
    current_max = max(current_rgb) if current_rgb else 1.0
    current_average = sum(current_rgb) / 3.0 if current_rgb else 1.0
    intent_family = classify_fix_intent_family(fix)
    brightness_like = intent_family == "scalar_like" or any(
        token in _fix_haystack(fix) for token in ("brightness", "emissive", "glow", "intensity", "value", "luminous")
    )
    explicitly_white = any(token in _fix_haystack(fix) for token in ("white", "whiter", "hotter", "washed"))
    if _operation_direction(fix) == "decrease":
        shifted = [_clamp(component - value_delta * 0.6, 0.0, 1.0) for component in current[:3]]
        shifted_average = sum(shifted) / 3.0
        rgb = [round(_clamp(shifted_average + (component - shifted_average) * (1.0 - saturation_delta), 0.0, 1.0), 4) for component in shifted]
    elif brightness_like and not explicitly_white:
        target_peak = min(0.94, current_max + value_delta * 0.75)
        scale_factor = target_peak / max(current_max, 0.12)
        boosted = [_clamp(component * scale_factor, 0.0, 1.0) for component in current_rgb]
        boosted_average = sum(boosted) / 3.0
        hue_preserve = max(0.12, 1.0 - saturation_delta * 0.45)
        rgb = [
            round(_clamp(boosted_average + (component - boosted_average) * (1.0 + hue_preserve), 0.0, 1.0), 4)
            for component in boosted
        ]
        if max(rgb) - min(rgb) < 0.05:
            dominant_index = max(range(3), key=lambda index: current_rgb[index])
            rgb[dominant_index] = round(_clamp(rgb[dominant_index] + min(0.1, value_delta * 0.45), 0.0, 1.0), 4)
        if sum(rgb) / 3.0 > max(0.9, current_average + 0.22):
            rgb = [round(_clamp(component * 0.92, 0.0, 1.0), 4) for component in rgb]
    else:
        brightened = [_clamp(component + value_delta, 0.0, 1.0) for component in current[:3]]
        bright_average = sum(brightened) / 3.0
        rgb = [round(_clamp(bright_average + (component - bright_average) * (1.0 + saturation_delta), 0.0, 1.0), 4) for component in brightened]
    return rgb + [round(current[3], 4)]


def ensure_target_compatible_op_type(fix: CriticFix, resolved_target: ResolvedTarget, inferred_op_type: str) -> str:
    if resolved_target.target_kind != "parameter":
        return inferred_op_type
    parameter_kind = _resolved_parameter_kind(resolved_target)
    if parameter_kind == "scalar":
        return "set_scalar"
    if parameter_kind == "vector":
        return "set_vector"
    if parameter_kind == "texture":
        return "set_texture"
    return inferred_op_type


def is_suspicious_intent_mapping(fix: CriticFix, resolved_target: ResolvedTarget, op_type: str) -> bool:
    intent_family = classify_fix_intent_family(fix)
    if intent_family == "scalar_like" and op_type != "set_scalar":
        return True
    if intent_family == "vector_like" and op_type != "set_vector":
        return True
    if "intensity" in _fix_haystack(fix) and op_type != "set_scalar":
        return True
    return False


def _expression_type_for_fix(fix: CriticFix) -> str:
    haystack = " ".join(filter(None, [fix.target_hint, fix.instruction, fix.rationale])).lower()
    if "fresnel" in haystack:
        return "Fresnel"
    if "panner" in haystack:
        return "Panner"
    if "noise" in haystack:
        return "Noise"
    if "lerp" in haystack:
        return "Lerp"
    return "Multiply"


def _target_pin_for_fix(fix: CriticFix) -> str:
    haystack = " ".join(filter(None, [fix.target_hint, fix.instruction, fix.rationale])).lower()
    if "opacity mask" in haystack:
        return "OpacityMask"
    if "opacity" in haystack:
        return "Opacity"
    if "normal" in haystack:
        return "Normal"
    if "roughness" in haystack:
        return "Roughness"
    if fix.axis == "color_match":
        return "BaseColor"
    return "EmissiveColor"


def generate_normalized_ops(
    fix: CriticFix,
    resolved_target: ResolvedTarget,
    asset_context: Mapping[str, Any],
    *,
    step_index: int,
    conservative_mode: bool = False,
    magnitude_scale: float = 1.0,
) -> list[NormalizedOp]:
    del conservative_mode
    material_path = str(asset_context.get("material_path", ""))
    op_id = f"step_{step_index}"
    op_type = ensure_target_compatible_op_type(fix, resolved_target, infer_fix_op_type(fix, resolved_target))

    if resolved_target.tier == "unresolved":
        return []
    if is_suspicious_intent_mapping(fix, resolved_target, op_type) and resolved_target.target_kind == "parameter":
        op_type = ensure_target_compatible_op_type(fix, resolved_target, op_type)

    if resolved_target.target_kind == "parameter":
        parameter_kind = _resolved_parameter_kind(resolved_target)
        if parameter_kind == "scalar" and op_type != "set_scalar":
            return []
        if parameter_kind == "vector" and op_type != "set_vector":
            return []
        if parameter_kind == "texture" and op_type != "set_texture":
            return []

    if op_type == "set_scalar":
        default_value = _compute_scalar_value_scaled(fix, resolved_target.current_value, magnitude_scale=magnitude_scale)
        return [
            NormalizedOp(
                op_id=op_id,
                op_type="set_scalar",
                target=resolved_target.display_name,
                params={
                    "material_path": material_path,
                    "param_name": resolved_target.display_name,
                    "default_value": default_value,
                    "previous_value": float(resolved_target.current_value) if isinstance(resolved_target.current_value, (int, float)) else None,
                },
                resolver_tier=resolved_target.tier,
            )
        ]

    if op_type == "set_vector":
        default_value = _compute_vector_value_scaled(fix, resolved_target.current_value, magnitude_scale=magnitude_scale)
        return [
            NormalizedOp(
                op_id=op_id,
                op_type="set_vector",
                target=resolved_target.display_name,
                params={
                    "material_path": material_path,
                    "param_name": resolved_target.display_name,
                    "default_value": default_value,
                    "previous_value": resolved_target.current_value if isinstance(resolved_target.current_value, list) else None,
                },
                resolver_tier=resolved_target.tier,
            )
        ]

    if op_type == "set_texture":
        texture_path = None
        if isinstance(resolved_target.current_value, str) and resolved_target.current_value:
            texture_path = resolved_target.current_value
        if not texture_path:
            texture_path = str(asset_context.get("recommended_texture_path", ""))
        if not texture_path:
            return []
        return [
            NormalizedOp(
                op_id=op_id,
                op_type="set_texture",
                target=resolved_target.display_name,
                params={
                    "material_path": material_path,
                    "param_name": resolved_target.display_name,
                    "texture_path": texture_path,
                    "previous_value": resolved_target.current_value if isinstance(resolved_target.current_value, str) else None,
                },
                resolver_tier=resolved_target.tier,
            )
        ]

    if op_type == "add_expression":
        return [
            NormalizedOp(
                op_id=op_id,
                op_type="add_expression",
                target=resolved_target.display_name,
                params={
                    "material_path": material_path,
                    "expression_type": _expression_type_for_fix(fix),
                    "param_name": resolved_target.display_name if resolved_target.target_kind == "expression" else None,
                },
                resolver_tier=resolved_target.tier,
            )
        ]

    if op_type == "set_material_output":
        if resolved_target.target_kind != "material_pin":
            return []
        source_index = resolved_target.current_value
        if source_index is None:
            return []
        return [
            NormalizedOp(
                op_id=op_id,
                op_type="set_material_output",
                target=resolved_target.display_name,
                params={
                    "material_path": material_path,
                    "source_index": int(source_index),
                    "target_pin": resolved_target.display_name or _target_pin_for_fix(fix),
                    "previous_source_index": resolved_target.metadata.get("previous_source_index"),
                },
                resolver_tier=resolved_target.tier,
            )
        ]

    return []
