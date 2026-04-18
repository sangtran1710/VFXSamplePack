from __future__ import annotations

import re
from dataclasses import dataclass, field
from difflib import SequenceMatcher
from typing import Any, Iterable, Mapping

from ..models.critic_models import CriticFix
from ..models.planner_models import NormalizedOpType
from .graph_influence_analyzer import GraphInfluenceReport, ParameterInfluence, analyze_graph_influence

BLOCKED_FUZZY_OPS: set[NormalizedOpType] = {
    "connect_nodes",
    "disconnect_nodes",
    "delete_expression",
    "set_material_output",
}

KNOWN_MATERIAL_PINS = {
    "emissive": "EmissiveColor",
    "basecolor": "BaseColor",
    "base": "BaseColor",
    "opacitymask": "OpacityMask",
    "opacity": "Opacity",
    "normal": "Normal",
    "roughness": "Roughness",
    "metallic": "Metallic",
    "worldpositionoffset": "WorldPositionOffset",
}


@dataclass(slots=True)
class ResolvedTarget:
    tier: str
    op_type: NormalizedOpType
    target_kind: str
    identifier: str
    display_name: str
    confidence: float
    current_value: Any = None
    metadata: dict[str, Any] = field(default_factory=dict)


def _normalize(text: str) -> str:
    return re.sub(r"[^a-z0-9]+", "", text.lower())


def _tokenize(text: str) -> set[str]:
    return {token for token in re.split(r"[^a-z0-9]+", text.lower()) if token}


def _iter_parameter_entries(asset_context: Mapping[str, Any], kind: str) -> list[dict[str, Any]]:
    parameters = asset_context.get("parameters", {})
    if isinstance(parameters, Mapping):
        entries = parameters.get(kind, [])
        if isinstance(entries, list):
            normalized_entries: list[dict[str, Any]] = []
            for entry in entries:
                if isinstance(entry, Mapping):
                    enriched = dict(entry)
                    enriched.setdefault("kind", kind)
                    normalized_entries.append(enriched)
            return normalized_entries
    legacy_key = f"{kind}_parameters"
    legacy_entries = asset_context.get(legacy_key, [])
    if isinstance(legacy_entries, list):
        normalized_entries = []
        for entry in legacy_entries:
            if isinstance(entry, Mapping):
                enriched = dict(entry)
                enriched.setdefault("kind", kind)
                normalized_entries.append(enriched)
        return normalized_entries
    return []


def _parameter_current_value(entry: Mapping[str, Any]) -> Any:
    if "value" in entry:
        return entry.get("value")
    if "default_value" in entry:
        return entry.get("default_value")
    if "default_value_rgba" in entry:
        return entry.get("default_value_rgba")
    if "texture_path" in entry:
        return entry.get("texture_path")
    if "default_texture" in entry:
        return entry.get("default_texture")
    return None


def _iter_expression_entries(asset_context: Mapping[str, Any]) -> list[dict[str, Any]]:
    expressions = asset_context.get("expressions", [])
    if isinstance(expressions, list):
        return [entry for entry in expressions if isinstance(entry, Mapping)]
    return []


def _candidate_phrases(fix: CriticFix) -> list[str]:
    phrases: list[str] = []
    for value in (fix.target_hint, fix.instruction, fix.rationale, fix.expected_effect, fix.axis):
        if value:
            phrases.append(value)
    return phrases


def _parameter_kind_for_op(op_type: NormalizedOpType) -> str | None:
    if op_type == "set_scalar":
        return "scalar"
    if op_type == "set_vector":
        return "vector"
    if op_type == "set_texture":
        return "texture"
    return None


def _best_exact_parameter_match(
    phrases: Iterable[str],
    entries: list[dict[str, Any]],
) -> dict[str, Any] | None:
    normalized_phrases = {_normalize(phrase) for phrase in phrases if phrase}
    for entry in entries:
        name = str(entry.get("name", ""))
        if _normalize(name) in normalized_phrases:
            return entry
    return None


def _influence_report(asset_context: Mapping[str, Any]) -> GraphInfluenceReport:
    cached = asset_context.get("graph_influence")
    if isinstance(cached, GraphInfluenceReport):
        return cached
    report = analyze_graph_influence(asset_context)
    try:
        asset_context["graph_influence"] = report  # type: ignore[index]
    except Exception:
        pass
    return report


def _intent_tokens(fix: CriticFix) -> set[str]:
    tokens = set().union(*(_tokenize(phrase) for phrase in _candidate_phrases(fix) if phrase))
    axis_tokens = {
        "value_match": {"emissive", "glow", "brightness", "bright", "intensity", "value"},
        "color_match": {"color", "tint", "hue", "rgb", "saturation", "glow", "basecolor"},
        "contrast_match": {"contrast", "mask", "roughness", "glow", "emissive"},
    }
    tokens.update(axis_tokens.get(fix.axis, set()))
    return tokens


def _parameter_candidate_score(
    fix: CriticFix,
    entry: Mapping[str, Any],
    influence: ParameterInfluence | None,
    *,
    parameter_kind: str | None,
) -> tuple[int, float]:
    score = 0
    if influence is not None:
        if influence.level == "high":
            score += 2
        elif influence.level == "medium":
            score += 1

    name = str(entry.get("name", ""))
    name_tokens = _tokenize(name)
    intent_tokens = _intent_tokens(fix)
    if name_tokens & intent_tokens:
        score += 2

    entry_kind = str(entry.get("kind", ""))
    if parameter_kind and entry_kind == parameter_kind:
        score += 1

    similarity = max((SequenceMatcher(None, _normalize(name), _normalize(phrase)).ratio() for phrase in _candidate_phrases(fix) if phrase), default=0.0)
    return score, similarity


def _best_influence_parameter_match(
    fix: CriticFix,
    entries: list[dict[str, Any]],
    influence_map: Mapping[str, ParameterInfluence],
    *,
    parameter_kind: str | None,
    required_level: str | None = None,
) -> dict[str, Any] | None:
    best_entry: dict[str, Any] | None = None
    best_rank: tuple[int, float] | None = None
    for entry in entries:
        name = str(entry.get("name", ""))
        influence = influence_map.get(name)
        if required_level is not None and (influence is None or influence.level != required_level):
            continue
        rank = _parameter_candidate_score(fix, entry, influence, parameter_kind=parameter_kind)
        if best_rank is None or rank > best_rank:
            best_entry = entry
            best_rank = rank
    return best_entry


def _parameter_entry_to_target(
    entry: Mapping[str, Any],
    *,
    op_type: NormalizedOpType,
    tier: str,
    confidence: float,
    influence_map: Mapping[str, ParameterInfluence],
) -> ResolvedTarget:
    name = str(entry.get("name", ""))
    influence = influence_map.get(name)
    return ResolvedTarget(
        tier=tier,
        op_type=op_type,
        target_kind="parameter",
        identifier=name,
        display_name=name,
        confidence=confidence,
        current_value=_parameter_current_value(entry),
        metadata={**dict(entry), "influence": influence.level if influence else "none"},
    )


def _best_fuzzy_parameter_match(
    phrases: Iterable[str],
    entries: list[dict[str, Any]],
) -> dict[str, Any] | None:
    best_entry: dict[str, Any] | None = None
    best_score = 0.0
    phrase_tokens = set().union(*(_tokenize(phrase) for phrase in phrases if phrase))
    for entry in entries:
        name = str(entry.get("name", ""))
        normalized_name = _normalize(name)
        if not normalized_name:
            continue
        similarity = max((SequenceMatcher(None, normalized_name, _normalize(phrase)).ratio() for phrase in phrases if phrase), default=0.0)
        overlap = len(phrase_tokens & _tokenize(name))
        score = similarity + overlap * 0.08
        if score > best_score:
            best_score = score
            best_entry = entry
    if best_score >= 0.74:
        return best_entry
    return None


def _best_exact_expression_match(
    phrases: Iterable[str],
    entries: list[dict[str, Any]],
) -> dict[str, Any] | None:
    normalized_phrases = {_normalize(phrase) for phrase in phrases if phrase}
    for entry in entries:
        fields = [
            str(entry.get("name", "")),
            str(entry.get("type", "")),
            str(entry.get("comment", "")),
            str(entry.get("desc", "")),
        ]
        for field in fields:
            if _normalize(field) in normalized_phrases and _normalize(field):
                return entry
    return None


def _best_fuzzy_expression_match(
    phrases: Iterable[str],
    entries: list[dict[str, Any]],
) -> dict[str, Any] | None:
    best_entry: dict[str, Any] | None = None
    best_score = 0.0
    for entry in entries:
        haystack = " ".join(
            [
                str(entry.get("name", "")),
                str(entry.get("type", "")),
                str(entry.get("comment", "")),
                str(entry.get("desc", "")),
            ]
        )
        normalized_haystack = _normalize(haystack)
        similarity = max((SequenceMatcher(None, normalized_haystack, _normalize(phrase)).ratio() for phrase in phrases if phrase), default=0.0)
        if similarity > best_score:
            best_score = similarity
            best_entry = entry
    if best_score >= 0.76:
        return best_entry
    return None


def _infer_fallback_name(fix: CriticFix, op_type: NormalizedOpType | str) -> str:
    base = fix.target_hint or fix.axis or fix.fix_id
    cleaned = re.sub(r"[^A-Za-z0-9]+", " ", base).title().replace(" ", "")
    if op_type == "set_scalar":
        return cleaned if cleaned.lower().endswith("intensity") else f"{cleaned}Scalar"
    if op_type == "set_vector":
        return cleaned if cleaned.lower().endswith("color") else f"{cleaned}Color"
    if op_type == "set_texture":
        return cleaned if cleaned.lower().endswith("tex") else f"{cleaned}Tex"
    return cleaned or "GeneratedTarget"


def _resolve_material_pin(fix: CriticFix) -> str | None:
    joined = " ".join(_candidate_phrases(fix))
    normalized = _normalize(joined)
    for token, pin in KNOWN_MATERIAL_PINS.items():
        if token in normalized:
            return pin
    if fix.axis == "value_match":
        return "EmissiveColor"
    if fix.axis == "color_match":
        return "BaseColor"
    return None


def resolve_target(
    fix: CriticFix,
    asset_context: Mapping[str, Any],
    op_type: NormalizedOpType,
    *,
    conservative_mode: bool = False,
) -> ResolvedTarget:
    phrases = _candidate_phrases(fix)
    parameter_kind = _parameter_kind_for_op(op_type)
    if parameter_kind is not None:
        parameters = _iter_parameter_entries(asset_context, parameter_kind)
        influence_map = _influence_report(asset_context).parameter_influence
        exact = _best_exact_parameter_match(phrases, parameters)
        if exact is not None:
            influence = influence_map.get(str(exact.get("name", "")))
            return ResolvedTarget(
                tier="direct_parameter",
                op_type=op_type,
                target_kind="parameter",
                identifier=str(exact.get("name", "")),
                display_name=str(exact.get("name", "")),
                confidence=1.0,
                current_value=_parameter_current_value(exact),
                metadata={**dict(exact), "influence": influence.level if influence else "none"},
            )
        high_influence = _best_influence_parameter_match(
            fix,
            parameters,
            influence_map,
            parameter_kind=parameter_kind,
            required_level="high",
        )
        if high_influence is not None:
            influence = influence_map.get(str(high_influence.get("name", "")))
            return ResolvedTarget(
                tier="output_path_parameter",
                op_type=op_type,
                target_kind="parameter",
                identifier=str(high_influence.get("name", "")),
                display_name=str(high_influence.get("name", "")),
                confidence=0.86,
                current_value=_parameter_current_value(high_influence),
                metadata={**dict(high_influence), "influence": influence.level if influence else "high"},
            )
        if not conservative_mode and op_type not in BLOCKED_FUZZY_OPS:
            fuzzy = _best_fuzzy_parameter_match(phrases, parameters)
            if fuzzy is not None:
                influence = influence_map.get(str(fuzzy.get("name", "")))
                return ResolvedTarget(
                    tier="fuzzy",
                    op_type=op_type,
                    target_kind="parameter",
                    identifier=str(fuzzy.get("name", "")),
                    display_name=str(fuzzy.get("name", "")),
                    confidence=0.78,
                    current_value=_parameter_current_value(fuzzy),
                    metadata={**dict(fuzzy), "influence": influence.level if influence else "none"},
                )
        if any(item.level == "high" for item in influence_map.values()):
            best_connected = _best_influence_parameter_match(
                fix,
                parameters,
                influence_map,
                parameter_kind=parameter_kind,
            )
            if best_connected is not None:
                influence = influence_map.get(str(best_connected.get("name", "")))
                return ResolvedTarget(
                    tier="output_path_parameter",
                    op_type=op_type,
                    target_kind="parameter",
                    identifier=str(best_connected.get("name", "")),
                    display_name=str(best_connected.get("name", "")),
                    confidence=0.72,
                    current_value=_parameter_current_value(best_connected),
                    metadata={**dict(best_connected), "influence": influence.level if influence else "none"},
                )
        return ResolvedTarget(
            tier="create_fallback",
            op_type=op_type,
            target_kind="parameter",
            identifier=_infer_fallback_name(fix, op_type),
            display_name=_infer_fallback_name(fix, op_type),
            confidence=0.6,
        )

    if op_type == "set_material_output":
        expressions = _iter_expression_entries(asset_context)
        exact_expression = _best_exact_expression_match(phrases, expressions)
        output_pin = _resolve_material_pin(fix)
        if output_pin is None:
            return ResolvedTarget(
                tier="unresolved",
                op_type=op_type,
                target_kind="material_pin",
                identifier="",
                display_name="",
                confidence=0.0,
            )
        if exact_expression is not None:
            previous_source_index = None
            root_inputs = asset_context.get("root_inputs", {})
            if isinstance(root_inputs, Mapping):
                current = root_inputs.get(output_pin, {})
                if isinstance(current, Mapping):
                    previous_source_index = current.get("source_index")
            return ResolvedTarget(
                tier="direct_expression",
                op_type=op_type,
                target_kind="material_pin",
                identifier=output_pin,
                display_name=output_pin,
                confidence=1.0,
                current_value=exact_expression.get("index"),
                metadata={
                    "target_pin": output_pin,
                    "previous_source_index": previous_source_index,
                    "source_expression": dict(exact_expression),
                },
            )
        return ResolvedTarget(
            tier="unresolved",
            op_type=op_type,
            target_kind="material_pin",
            identifier=output_pin,
            display_name=output_pin,
            confidence=0.0,
        )

    expressions = _iter_expression_entries(asset_context)
    exact_expression = _best_exact_expression_match(phrases, expressions)
    if exact_expression is not None:
        return ResolvedTarget(
            tier="direct_expression",
            op_type=op_type,
            target_kind="expression",
            identifier=str(exact_expression.get("index", "")),
            display_name=str(exact_expression.get("name", exact_expression.get("type", "Expression"))),
            confidence=1.0,
            current_value=exact_expression.get("index"),
            metadata=dict(exact_expression),
        )

    if not conservative_mode and op_type not in BLOCKED_FUZZY_OPS:
        fuzzy_expression = _best_fuzzy_expression_match(phrases, expressions)
        if fuzzy_expression is not None:
            return ResolvedTarget(
                tier="fuzzy",
                op_type=op_type,
                target_kind="expression",
                identifier=str(fuzzy_expression.get("index", "")),
                display_name=str(fuzzy_expression.get("name", fuzzy_expression.get("type", "Expression"))),
                confidence=0.77,
                current_value=fuzzy_expression.get("index"),
                metadata=dict(fuzzy_expression),
            )

    if op_type == "add_expression":
        return ResolvedTarget(
            tier="create_fallback",
            op_type=op_type,
            target_kind="expression",
            identifier=_infer_fallback_name(fix, "add_expression"),
            display_name=_infer_fallback_name(fix, "add_expression"),
            confidence=0.6,
        )

    return ResolvedTarget(
        tier="unresolved",
        op_type=op_type,
        target_kind="expression",
        identifier="",
        display_name="",
        confidence=0.0,
    )


def resolve_target_candidates(
    fix: CriticFix,
    asset_context: Mapping[str, Any],
    op_type: NormalizedOpType,
    *,
    conservative_mode: bool = False,
    max_candidates: int = 6,
) -> list[ResolvedTarget]:
    parameter_kind = _parameter_kind_for_op(op_type)
    if parameter_kind is None:
        resolved = resolve_target(fix, asset_context, op_type, conservative_mode=conservative_mode)
        return [resolved] if resolved.tier != "unresolved" else []

    phrases = _candidate_phrases(fix)
    parameters = _iter_parameter_entries(asset_context, parameter_kind)
    influence_map = _influence_report(asset_context).parameter_influence
    candidates: list[ResolvedTarget] = []
    seen: set[str] = set()
    has_high_influence = any(item.level == "high" for item in influence_map.values())

    exact = _best_exact_parameter_match(phrases, parameters)
    if exact is not None:
        name = str(exact.get("name", ""))
        candidates.append(
            _parameter_entry_to_target(
                exact,
                op_type=op_type,
                tier="direct_parameter",
                confidence=1.0,
                influence_map=influence_map,
            )
        )
        seen.add(name)

    ranked_entries = sorted(
        parameters,
        key=lambda entry: _parameter_candidate_score(
            fix,
            entry,
            influence_map.get(str(entry.get("name", ""))),
            parameter_kind=parameter_kind,
        ),
        reverse=True,
    )

    for entry in ranked_entries:
        name = str(entry.get("name", ""))
        if not name or name in seen:
            continue
        influence = influence_map.get(name)
        if influence is not None and influence.level == "high":
            candidates.append(
                _parameter_entry_to_target(
                    entry,
                    op_type=op_type,
                    tier="output_path_parameter",
                    confidence=0.86,
                    influence_map=influence_map,
                )
            )
            seen.add(name)

    if not conservative_mode and op_type not in BLOCKED_FUZZY_OPS:
        for entry in ranked_entries:
            name = str(entry.get("name", ""))
            if not name or name in seen:
                continue
            influence = influence_map.get(name)
            rank_score, similarity = _parameter_candidate_score(
                fix,
                entry,
                influence,
                parameter_kind=parameter_kind,
            )
            if similarity >= 0.74 or rank_score >= 3:
                candidates.append(
                    _parameter_entry_to_target(
                        entry,
                        op_type=op_type,
                        tier="fuzzy",
                        confidence=0.78,
                        influence_map=influence_map,
                    )
                )
                seen.add(name)

    if not candidates and has_high_influence:
        best_connected = _best_influence_parameter_match(
            fix,
            parameters,
            influence_map,
            parameter_kind=parameter_kind,
        )
        if best_connected is not None:
            candidates.append(
                _parameter_entry_to_target(
                    best_connected,
                    op_type=op_type,
                    tier="output_path_parameter",
                    confidence=0.72,
                    influence_map=influence_map,
                )
            )
            seen.add(str(best_connected.get("name", "")))

    if not candidates:
        fallback = resolve_target(fix, asset_context, op_type, conservative_mode=conservative_mode)
        return [fallback] if fallback.tier != "unresolved" else []
    return candidates[:max_candidates]
