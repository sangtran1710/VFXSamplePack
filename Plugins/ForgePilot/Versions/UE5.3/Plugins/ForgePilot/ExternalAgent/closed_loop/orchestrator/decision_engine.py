from __future__ import annotations

from dataclasses import dataclass, field

from ..models.orchestrator_models import IterationRecord, LoopConfig, LoopState


@dataclass(slots=True)
class LoopDecision:
    action: str
    reason: str
    status: str
    metadata: dict[str, float | int | str | bool] = field(default_factory=dict)


def _axis_drift_detected(record: IterationRecord, config: LoopConfig) -> bool:
    if record.pre_critic is None or record.post_critic is None:
        return False
    pre_scores = dict(record.pre_critic.axis_scores.root)
    post_scores = dict(record.post_critic.axis_scores.root)
    for axis_name, pre_value in pre_scores.items():
        post_value = post_scores.get(axis_name)
        if post_value is None:
            continue
        if (post_value - pre_value) < -config.axis_regression_threshold:
            return True
    return False


def pre_decide(pre_score: float, state: LoopState, config: LoopConfig) -> LoopDecision:
    if pre_score >= config.pass_threshold:
        return LoopDecision(action="stop", reason="pass_threshold", status="passed", metadata={"pre_score": pre_score})
    if state.cumulative_steps >= config.max_total_steps:
        return LoopDecision(action="stop", reason="budget", status="stopped", metadata={"cumulative_steps": state.cumulative_steps})
    if state.iteration_count >= config.max_iterations:
        return LoopDecision(action="stop", reason="iteration_limit", status="stopped", metadata={"iteration_count": state.iteration_count})
    return LoopDecision(action="continue", reason="within_budget", status="running", metadata={"pre_score": pre_score})


def post_decide(record: IterationRecord, state: LoopState, config: LoopConfig) -> LoopDecision:
    post_score = record.post_score if record.post_score is not None else record.pre_score
    score_delta = post_score - record.pre_score

    if post_score >= config.pass_threshold:
        return LoopDecision(action="stop", reason="pass", status="passed", metadata={"post_score": post_score})
    if post_score < record.pre_score - config.regression_threshold:
        return LoopDecision(action="rollback", reason="regression", status="regressed", metadata={"score_delta": score_delta})
    if abs(score_delta) < config.plateau_threshold:
        return LoopDecision(action="stop", reason="plateau", status="stopped", metadata={"score_delta": score_delta})
    if score_delta > 0.0 and _axis_drift_detected(record, config):
        return LoopDecision(action="stop", reason="drift", status="stopped", metadata={"score_delta": score_delta})
    if state.empty_plan_streak >= 2:
        return LoopDecision(action="stop", reason="blocked", status="blocked", metadata={"empty_plan_streak": state.empty_plan_streak})
    if state.cumulative_steps >= config.max_total_steps:
        return LoopDecision(action="stop", reason="budget", status="stopped", metadata={"cumulative_steps": state.cumulative_steps})
    return LoopDecision(action="continue", reason="improving", status="running", metadata={"score_delta": score_delta})
