from __future__ import annotations

from dataclasses import dataclass
from typing import Iterable, Mapping

from ..models.critic_models import CriticFix


@dataclass(slots=True)
class SkippedFix:
    fix: CriticFix
    reason: str


@dataclass(slots=True)
class SelectedFixSet:
    selected: list[CriticFix]
    skipped: list[SkippedFix]


def _failure_penalty(fix_id: str, prior_plan_outcomes: Iterable[Mapping[str, object]] | None) -> float:
    if not prior_plan_outcomes:
        return 0.0
    penalty = 0.0
    for outcome in prior_plan_outcomes:
        if str(outcome.get("fix_id", "")).strip().lower() != fix_id.lower():
            continue
        status = str(outcome.get("status", "")).strip().lower()
        if status in {"failed", "regressed", "blocked"}:
            penalty += 0.35
        elif status in {"warning", "partial"}:
            penalty += 0.15
    return penalty


def _fix_score(
    fix: CriticFix,
    axis_scores: Mapping[str, float],
    prior_plan_outcomes: Iterable[Mapping[str, object]] | None,
) -> float:
    deficiency = 1.0 - float(axis_scores.get(fix.axis, 0.5))
    priority_bonus = (6 - fix.priority) * 0.18
    confidence_bonus = fix.confidence * 0.45
    risk_penalty = {"low": 0.0, "medium": 0.05, "high": 0.15}[fix.risk]
    return deficiency * 1.8 + priority_bonus + confidence_bonus - risk_penalty - _failure_penalty(fix.fix_id, prior_plan_outcomes)


def select_fixes(
    top_fixes: list[CriticFix],
    axis_scores: Mapping[str, float],
    prior_plan_outcomes: Iterable[Mapping[str, object]] | None = None,
    *,
    conservative_mode: bool = False,
    max_selected: int = 3,
) -> SelectedFixSet:
    capped = min(max_selected, 2) if conservative_mode else max_selected
    ranked = sorted(
        top_fixes,
        key=lambda fix: (
            -_fix_score(fix, axis_scores, prior_plan_outcomes),
            fix.priority,
            fix.fix_id.lower(),
        ),
    )

    selected: list[CriticFix] = []
    skipped: list[SkippedFix] = []
    seen_axes: set[str] = set()
    for fix in ranked:
        if len(selected) >= capped:
            skipped.append(SkippedFix(fix=fix, reason="selection_budget"))
            continue
        if fix.axis in seen_axes:
            skipped.append(SkippedFix(fix=fix, reason="axis_already_covered"))
            continue
        if _failure_penalty(fix.fix_id, prior_plan_outcomes) >= 0.7:
            skipped.append(SkippedFix(fix=fix, reason="repeated_prior_failure"))
            continue
        selected.append(fix)
        seen_axes.add(fix.axis)

    return SelectedFixSet(selected=selected, skipped=skipped)
