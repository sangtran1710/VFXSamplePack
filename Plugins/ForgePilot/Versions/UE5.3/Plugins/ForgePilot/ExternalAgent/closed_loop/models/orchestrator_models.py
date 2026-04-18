from __future__ import annotations

from datetime import datetime
from typing import Literal

from pydantic import Field

from .critic_models import CriticResult
from .shared_types import ExecutionStatus, JsonValue, RiskLevel, StrictJsonModel, ValidationStatus, utc_now


class LoopConfig(StrictJsonModel):
    pass_threshold: float = 0.9
    max_iterations: int = Field(default=4, ge=1)
    max_total_steps: int = Field(default=12, ge=1)
    plateau_threshold: float = Field(default=0.02, ge=0.0)
    regression_threshold: float = Field(default=0.05, ge=0.0)
    axis_regression_threshold: float = Field(default=0.08, ge=0.0)
    conservative_mode: bool = False
    output_root: str | None = None


class ExecutionResult(StrictJsonModel):
    step_id: str = Field(min_length=1)
    status: ExecutionStatus
    tool_name: str | None = None
    request: dict[str, JsonValue] = Field(default_factory=dict)
    response: dict[str, JsonValue] = Field(default_factory=dict)
    error: str | None = None
    started_at: datetime = Field(default_factory=utc_now)
    finished_at: datetime | None = None
    risk: RiskLevel = "medium"


class ValidationResult(StrictJsonModel):
    check_name: str = Field(min_length=1)
    status: ValidationStatus
    success: bool
    details: dict[str, JsonValue] = Field(default_factory=dict)
    error: str | None = None


class RollbackEvent(StrictJsonModel):
    scope: Literal["step", "plan", "snapshot", "best_state"]
    success: bool
    reason: str = Field(min_length=1)
    details: dict[str, JsonValue] = Field(default_factory=dict)
    created_at: datetime = Field(default_factory=utc_now)


class IterationRecord(StrictJsonModel):
    iteration_index: int = Field(ge=0)
    pre_score: float
    post_score: float | None = None
    score_delta: float | None = None
    pre_critic: CriticResult | None = None
    post_critic: CriticResult | None = None
    execution_results: list[ExecutionResult] = Field(default_factory=list)
    validations: list[ValidationResult] = Field(default_factory=list)
    rollback_events: list[RollbackEvent] = Field(default_factory=list)
    status: str = "pending"
    notes: list[str] = Field(default_factory=list)
    created_at: datetime = Field(default_factory=utc_now)


class LoopState(StrictJsonModel):
    iteration_count: int = 0
    cumulative_steps: int = 0
    best_score: float | None = None
    best_iteration_index: int | None = None
    effective_risk: Literal["normal", "conservative"] = "normal"
    empty_plan_streak: int = 0
    status: str = "running"
    best_snapshot_path: str | None = None


class LoopResult(StrictJsonModel):
    asset_path: str = Field(min_length=1)
    target_image: str = Field(min_length=1)
    status: Literal["passed", "stopped", "failed", "blocked", "regressed"]
    iterations: list[IterationRecord] = Field(default_factory=list)
    final_score: float | None = None
    best_score: float | None = None
    restored_best_state: bool = False
    run_dir: str | None = None
    started_at: datetime = Field(default_factory=utc_now)
    ended_at: datetime | None = None
