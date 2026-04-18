from __future__ import annotations

from typing import Iterable

from ..models.critic_models import CriticResult
from ..models.semantic_models import FailureClassification, SemanticStagePlan


def _as_text_parts(*values: object) -> str:
    parts: list[str] = []
    for value in values:
        if isinstance(value, str):
            parts.append(value.lower())
        elif isinstance(value, Iterable) and not isinstance(value, (bytes, bytearray, dict)):
            for item in value:
                if isinstance(item, str):
                    parts.append(item.lower())
    return "\n".join(parts)


def _contains_any(text: str, keywords: Iterable[str]) -> bool:
    return any(keyword in text for keyword in keywords)


def classify_visual_failure(
    critic_result: CriticResult,
    stage_plan: SemanticStagePlan,
    *,
    dominant_read: str = "",
    anti_patterns: list[str] | None = None,
    capture_metadata: dict | None = None,
) -> FailureClassification:
    anti_patterns = anti_patterns or []
    capture_metadata = capture_metadata or {}
    evidence: list[str] = []
    merged_text = _as_text_parts(
        critic_result.summary,
        critic_result.deltas and [delta.issue for delta in critic_result.deltas],
        critic_result.deltas and [delta.observation for delta in critic_result.deltas],
        critic_result.top_fixes and [fix.instruction for fix in critic_result.top_fixes],
        critic_result.top_fixes and [fix.rationale for fix in critic_result.top_fixes],
        dominant_read,
        anti_patterns,
        capture_metadata.get("capture_mode", ""),
        capture_metadata.get("notes", []),
    )

    if critic_result.passed or critic_result.overall_score >= 0.92:
        return FailureClassification(
            failure_class="complete",
            target_stage=stage_plan.stage_id,
            target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
            change_type="advance_stage",
            expected_effect="Advance to the next semantic build stage.",
            risk="low",
            summary="Current stage reads close enough to the reference.",
            evidence=["critic passed threshold"],
            confidence=0.95,
        )

    if _contains_any(merged_text, ("thumbnail fallback", "wrong material", "framing drift", "capture drift", "background mismatch")):
        evidence.append("capture issue keywords detected")
        return FailureClassification(
            failure_class="capture",
            target_stage=stage_plan.stage_id,
            target_lane=None,
            change_type="fix_capture",
            expected_effect="Stabilize probe capture before mutating the graph again.",
            risk="low",
            summary="The rendered comparison path appears unreliable.",
            evidence=evidence,
            confidence=0.82,
        )

    if _contains_any(merged_text, ("missing rim", "missing core", "missing shell", "no readable breakup", "no core", "wrong graph", "missing branch")):
        evidence.append("structural absence keywords detected")
        return FailureClassification(
            failure_class="structural",
            target_stage=stage_plan.stage_id,
            target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
            target_nodes_or_params=[fix.target_hint for fix in critic_result.top_fixes if fix.target_hint],
            change_type="lane_rebuild",
            expected_effect="Repair or rebuild the missing structural lane for this stage.",
            risk="medium",
            summary="The material is missing a required visual structure for this stage.",
            evidence=evidence,
            confidence=0.9,
        )

    if _contains_any(merged_text, ("too smooth", "too broad", "too soft", "cloudy", "thick outline", "filament unreadable", "gradient too soft", "rim too thick")):
        evidence.append("shaping keywords detected")
        return FailureClassification(
            failure_class="shaping",
            target_stage=stage_plan.stage_id,
            target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
            target_nodes_or_params=[fix.target_hint for fix in critic_result.top_fixes if fix.target_hint],
            change_type="shaping_repair",
            expected_effect="Retune the shaping math while preserving the approved stage structure.",
            risk="medium",
            summary="The stage structure exists, but its shape/read is wrong.",
            evidence=evidence,
            confidence=0.86,
        )

    if _contains_any(merged_text, ("mostly white", "flat orange", "no hot center", "no warm body", "color separation", "orange body", "white-hot center")):
        evidence.append("color identity keywords detected")
        return FailureClassification(
            failure_class="color",
            target_stage=stage_plan.stage_id,
            target_lane="base_color" if "base_color" in stage_plan.preferred_lanes else (stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None),
            target_nodes_or_params=[fix.target_hint for fix in critic_result.top_fixes if fix.target_hint],
            change_type="parameter_repair",
            expected_effect="Retune the color/heat gradient without rebuilding the graph.",
            risk="low",
            summary="The main failure is color identity rather than missing structure.",
            evidence=evidence,
            confidence=0.83,
        )

    if _contains_any(merged_text, ("clipping", "saturated", "pinned", "too bright", "lighter than target", "oversaturated", "washed out")):
        evidence.append("saturation keywords detected")
        return FailureClassification(
            failure_class="saturation",
            target_stage=stage_plan.stage_id,
            target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
            target_nodes_or_params=[fix.target_hint for fix in critic_result.top_fixes if fix.target_hint],
            change_type="parameter_repair",
            expected_effect="Move away from saturated controls and reduce ineffective repeated pushes.",
            risk="low",
            summary="The current stage is response-limited by saturation or clipping.",
            evidence=evidence,
            confidence=0.8,
        )

    fix_targets = [fix.target_hint for fix in critic_result.top_fixes if fix.target_hint]
    return FailureClassification(
        failure_class="unknown",
        target_stage=stage_plan.stage_id,
        target_lane=stage_plan.preferred_lanes[0] if stage_plan.preferred_lanes else None,
        target_nodes_or_params=fix_targets,
        change_type="parameter_repair" if fix_targets else "stop",
        expected_effect="Apply the most bounded deterministic fix available.",
        risk="medium",
        summary="Failure could not be classified cleanly; falling back to bounded repair.",
        evidence=evidence or ["no strong taxonomy match"],
        confidence=0.55,
    )
