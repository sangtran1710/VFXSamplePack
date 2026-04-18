from __future__ import annotations

from ExternalAgent.closed_loop.models.critic_models import CriticAxisScores, CriticResult
from ExternalAgent.closed_loop.models.orchestrator_models import IterationRecord, LoopConfig, LoopState
from ExternalAgent.closed_loop.orchestrator.decision_engine import post_decide, pre_decide


def _critic(score: float, value: float, color: float, contrast: float) -> CriticResult:
    return CriticResult(
        summary="decision-engine",
        overall_score=score,
        axis_scores=CriticAxisScores(
            {
                "value_match": value,
                "color_match": color,
                "contrast_match": contrast,
            }
        ),
        deltas=[],
        top_fixes=[],
        passed=False,
        critic_version="v2",
    )


def test_pre_decide() -> None:
    config = LoopConfig(pass_threshold=0.9, max_iterations=3, max_total_steps=5)
    state = LoopState(iteration_count=0, cumulative_steps=0)
    assert pre_decide(0.95, state, config).reason == "pass_threshold"

    state = LoopState(iteration_count=0, cumulative_steps=5)
    assert pre_decide(0.4, state, config).reason == "budget"


def test_post_decide_plateau_and_regression() -> None:
    config = LoopConfig(plateau_threshold=0.02, regression_threshold=0.05, axis_regression_threshold=0.08)
    state = LoopState()

    plateau_record = IterationRecord(iteration_index=0, pre_score=0.6, post_score=0.61, score_delta=0.01)
    assert post_decide(plateau_record, state, config).reason == "plateau"

    regression_record = IterationRecord(iteration_index=0, pre_score=0.6, post_score=0.5, score_delta=-0.1)
    assert post_decide(regression_record, state, config).reason == "regression"


def test_post_decide_drift_and_blocked() -> None:
    config = LoopConfig(plateau_threshold=0.01, regression_threshold=0.05, axis_regression_threshold=0.08)
    state = LoopState(empty_plan_streak=2)
    blocked_record = IterationRecord(iteration_index=0, pre_score=0.3, post_score=0.35, score_delta=0.05)
    assert post_decide(blocked_record, state, config).reason == "blocked"

    state = LoopState(empty_plan_streak=0)
    drift_record = IterationRecord(
        iteration_index=0,
        pre_score=0.5,
        post_score=0.6,
        score_delta=0.1,
        pre_critic=_critic(0.5, 0.6, 0.6, 0.6),
        post_critic=_critic(0.6, 0.45, 0.8, 0.82),
    )
    assert post_decide(drift_record, state, config).reason == "drift"


def run() -> None:
    test_pre_decide()
    test_post_decide_plateau_and_regression()
    test_post_decide_drift_and_blocked()


if __name__ == "__main__":
    run()
    print("test_decision_engine.py passed")
