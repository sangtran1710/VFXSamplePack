from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import Any, Iterable, Mapping


PRIMARY_OUTPUT_PINS = {
    "BaseColor",
    "EmissiveColor",
    "Opacity",
    "OpacityMask",
    "Normal",
    "Roughness",
    "Metallic",
    "WorldPositionOffset",
    "PixelDepthOffset",
}


@dataclass(slots=True)
class ParameterInfluence:
    name: str
    level: str
    score: int
    expression_index: int | None = None
    output_pins: list[str] = field(default_factory=list)
    min_distance: int | None = None


@dataclass(slots=True)
class GraphInfluenceReport:
    parameter_influence: dict[str, ParameterInfluence]
    reachable_expression_indices: dict[int, int]


def _normalize(text: str) -> str:
    return re.sub(r"[^a-z0-9]+", "", str(text).lower())


def _iter_parameter_entries(asset_context: Mapping[str, Any]) -> list[dict[str, Any]]:
    parameters = asset_context.get("parameters", {})
    entries: list[dict[str, Any]] = []
    if isinstance(parameters, Mapping):
        for kind in ("scalar", "vector", "texture"):
            values = parameters.get(kind, [])
            if isinstance(values, list):
                for entry in values:
                    if isinstance(entry, Mapping):
                        enriched = dict(entry)
                        enriched.setdefault("kind", kind)
                        entries.append(enriched)
    return entries


def _iter_expression_entries(asset_context: Mapping[str, Any]) -> list[dict[str, Any]]:
    expressions = asset_context.get("expressions", [])
    if isinstance(expressions, list):
        return [dict(entry) for entry in expressions if isinstance(entry, Mapping)]
    return []


def _iter_connections(asset_context: Mapping[str, Any]) -> list[dict[str, Any]]:
    connections = asset_context.get("connections", [])
    if isinstance(connections, list):
        return [dict(entry) for entry in connections if isinstance(entry, Mapping)]
    return []


def _iter_root_inputs(asset_context: Mapping[str, Any]) -> list[dict[str, Any]]:
    root_inputs = asset_context.get("root_inputs")
    if isinstance(root_inputs, list):
        return [dict(entry) for entry in root_inputs if isinstance(entry, Mapping)]
    if isinstance(root_inputs, Mapping):
        results: list[dict[str, Any]] = []
        for pin, value in root_inputs.items():
            if isinstance(value, Mapping):
                item = dict(value)
                item.setdefault("pin", pin)
                results.append(item)
        return results
    material_inputs = asset_context.get("material_inputs", [])
    if isinstance(material_inputs, list):
        return [dict(entry) for entry in material_inputs if isinstance(entry, Mapping)]
    return []


def _expression_index_from_parameter(entry: Mapping[str, Any], expressions: Iterable[Mapping[str, Any]]) -> int | None:
    raw_index = entry.get("expression_index")
    if isinstance(raw_index, int):
        return raw_index
    if isinstance(raw_index, str) and raw_index.isdigit():
        return int(raw_index)

    parameter_name = str(entry.get("name", ""))
    normalized_name = _normalize(parameter_name)
    if not normalized_name:
        return None

    for expression in expressions:
        fields = [
            expression.get("name"),
            expression.get("param_name"),
            expression.get("practical_name"),
            expression.get("caption"),
            expression.get("comment"),
            expression.get("desc"),
            expression.get("description"),
        ]
        if any(normalized_name and normalized_name in _normalize(field) for field in fields if field):
            index = expression.get("index")
            if isinstance(index, int):
                return index
            if isinstance(index, str) and index.isdigit():
                return int(index)
    return None


def _build_upstream_links(asset_context: Mapping[str, Any], expressions: list[dict[str, Any]]) -> tuple[dict[int, set[int]], dict[int, set[str]]]:
    upstream: dict[int, set[int]] = {}
    output_pins_by_source: dict[int, set[str]] = {}

    for expression in expressions:
        target_index = expression.get("index")
        if not isinstance(target_index, int):
            continue
        for input_entry in expression.get("inputs", []):
            if not isinstance(input_entry, Mapping) or not input_entry.get("connected"):
                continue
            source_index = input_entry.get("source_index")
            if isinstance(source_index, int):
                upstream.setdefault(target_index, set()).add(source_index)

    for connection in _iter_connections(asset_context):
        source_index = connection.get("source_index")
        if not isinstance(source_index, int):
            continue
        target_type = str(connection.get("target_type", ""))
        if target_type == "expression":
            target_index = connection.get("target_index")
            if isinstance(target_index, int):
                upstream.setdefault(target_index, set()).add(source_index)
        elif target_type == "material_input":
            target_pin = str(connection.get("target_pin", ""))
            if target_pin:
                output_pins_by_source.setdefault(source_index, set()).add(target_pin)

    for root_input in _iter_root_inputs(asset_context):
        if not root_input.get("connected"):
            continue
        source_index = root_input.get("source_index")
        target_pin = str(root_input.get("pin", ""))
        if isinstance(source_index, int) and target_pin:
            output_pins_by_source.setdefault(source_index, set()).add(target_pin)

    return upstream, output_pins_by_source


def analyze_graph_influence(asset_context: Mapping[str, Any]) -> GraphInfluenceReport:
    expressions = _iter_expression_entries(asset_context)
    upstream, output_pins_by_source = _build_upstream_links(asset_context, expressions)

    reachable_distances: dict[int, int] = {}
    reachable_pins: dict[int, set[str]] = {}
    queue: list[int] = sorted(output_pins_by_source.keys())

    for index in queue:
        reachable_distances[index] = 0
        reachable_pins[index] = set(output_pins_by_source.get(index, set()))

    queue_index = 0
    while queue_index < len(queue):
        current = queue[queue_index]
        current_distance = reachable_distances[current]
        current_pins = reachable_pins[current]
        queue_index += 1
        for source_index in sorted(upstream.get(current, set())):
            next_distance = current_distance + 1
            previous_distance = reachable_distances.get(source_index)
            if previous_distance is None or next_distance < previous_distance:
                reachable_distances[source_index] = next_distance
                reachable_pins[source_index] = set(current_pins)
                queue.append(source_index)
            elif next_distance == previous_distance:
                reachable_pins.setdefault(source_index, set()).update(current_pins)

    parameter_influence: dict[str, ParameterInfluence] = {}
    for entry in _iter_parameter_entries(asset_context):
        name = str(entry.get("name", ""))
        if not name:
            continue
        expression_index = _expression_index_from_parameter(entry, expressions)
        if expression_index is None:
            parameter_influence[name] = ParameterInfluence(name=name, level="none", score=0)
            continue

        output_pins = sorted(reachable_pins.get(expression_index, set()))
        min_distance = reachable_distances.get(expression_index)
        if min_distance is None:
            level = "low"
            score = 0
        elif any(pin in PRIMARY_OUTPUT_PINS for pin in output_pins):
            level = "high"
            score = 2
        else:
            level = "medium"
            score = 1

        parameter_influence[name] = ParameterInfluence(
            name=name,
            level=level,
            score=score,
            expression_index=expression_index,
            output_pins=output_pins,
            min_distance=min_distance,
        )

    return GraphInfluenceReport(
        parameter_influence=parameter_influence,
        reachable_expression_indices=reachable_distances,
    )
