from __future__ import annotations

from ExternalAgent.closed_loop.models.critic_models import CriticAxisScores, CriticFix, CriticResult
from ExternalAgent.closed_loop.models.semantic_models import SemanticStagePlan
from ExternalAgent.closed_loop.orchestrator.failure_classifier import classify_visual_failure


def _stage(stage_id: str, target_layer: str) -> SemanticStagePlan:
    return SemanticStagePlan(
        stage_id=stage_id,
        label=stage_id,
        goal="test goal",
        target_layer=target_layer,
        allowed_ops=["noise"],
        forbidden_ops=[],
        validation_rules=["keep_core"],
        expected_visual_change="visible change",
        hard_constraints=["keep core"],
        anti_patterns=["cloudy white orb"],
        stop_conditions=["destroy core"],
        preferred_lanes=["emissive_visual"],
    )


def test_classifies_shaping_failure_for_cloudy_breakup() -> None:
    critic = CriticResult(
        summary="The orb still looks cloudy and too smooth; filament readability is missing.",
        overall_score=0.44,
        axis_scores=CriticAxisScores({"value_match": 0.6, "color_match": 0.5, "contrast_match": 0.2}),
        deltas=[],
        top_fixes=[
            CriticFix(
                fix_id="tighten_filaments",
                axis="contrast_match",
                instruction="Make the breakup thinner and more readable.",
                rationale="Current breakup is too smooth and cloudy.",
                priority=1,
                confidence=0.9,
                risk="medium",
                target_hint="breakup lane",
                expected_effect="Better filament readability.",
            )
        ],
        passed=False,
    )
    classification = classify_visual_failure(critic, _stage("stage_2_filament", "breakup"))
    assert classification.failure_class == "shaping"
    assert classification.change_type == "shaping_repair"


def test_classifies_color_failure_for_white_orb() -> None:
    critic = CriticResult(
        summary="The material is mostly white and lacks orange body separation.",
        overall_score=0.52,
        axis_scores=CriticAxisScores({"value_match": 0.7, "color_match": 0.2, "contrast_match": 0.66}),
        deltas=[],
        top_fixes=[],
        passed=False,
    )
    classification = classify_visual_failure(critic, _stage("stage_5_color", "color_identity"))
    assert classification.failure_class == "color"
    assert classification.change_type == "parameter_repair"


def run() -> None:
    test_classifies_shaping_failure_for_cloudy_breakup()
    test_classifies_color_failure_for_white_orb()


if __name__ == "__main__":
    run()
    print("test_failure_classifier.py passed")
