from __future__ import annotations

from typing import Iterable

from ..models.semantic_models import AnalyzerOutput, SemanticStagePlan, SemanticVisualPlan


_LAYER_TO_LANES = {
    "core": ["emissive_visual", "core_shape"],
    "body": ["emissive_visual", "base_color"],
    "breakup": ["emissive_visual", "breakup"],
    "depth": ["emissive_visual", "breakup"],
    "rim": ["edge_rim", "emissive_visual"],
    "color_identity": ["base_color", "emissive_visual"],
}


def _normalize_ops(values: Iterable[str]) -> list[str]:
    normalized: list[str] = []
    for value in values:
        cleaned = str(value).strip().lower().replace(" ", "_")
        if cleaned and cleaned not in normalized:
            normalized.append(cleaned)
    return normalized


def build_material_visual_plan(analyzer_output: AnalyzerOutput) -> SemanticVisualPlan:
    stages: list[SemanticStagePlan] = []
    for item in analyzer_output.stage_recipe:
        role = next((layer.role for layer in analyzer_output.layer_stack if layer.layer_id == item.target_layer), "other")
        stages.append(
            SemanticStagePlan(
                stage_id=item.stage_id,
                label=item.label,
                goal=item.goal,
                target_layer=item.target_layer,
                allowed_ops=_normalize_ops(item.allowed_node_families),
                forbidden_ops=_normalize_ops(item.forbidden_node_families),
                validation_rules=list(item.validation_rules),
                expected_visual_change=item.expected_visual_change,
                hard_constraints=list(analyzer_output.hard_constraints),
                anti_patterns=list(analyzer_output.anti_patterns),
                stop_conditions=list(item.stop_conditions),
                preferred_lanes=list(_LAYER_TO_LANES.get(role, [item.target_layer])),
            )
        )
    return SemanticVisualPlan(
        archetype=analyzer_output.archetype,
        material_family=analyzer_output.material_family,
        dominant_read=analyzer_output.dominant_read,
        stages=stages,
        hard_constraints=list(analyzer_output.hard_constraints),
        anti_patterns=list(analyzer_output.anti_patterns),
        plan_version="v1",
    )
