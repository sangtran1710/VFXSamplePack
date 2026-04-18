from __future__ import annotations

from typing import Literal

from pydantic import Field, RootModel, field_validator, model_validator

from .shared_types import StrictJsonModel

CriticDirection = Literal["increase", "decrease", "preserve", "shift"]


class CriticAxisScores(RootModel[dict[str, float]]):
    @field_validator("root")
    @classmethod
    def validate_scores(cls, value: dict[str, float]) -> dict[str, float]:
        if not value:
            raise ValueError("axis_scores must contain at least one scored axis.")
        normalized: dict[str, float] = {}
        for key, score in value.items():
            cleaned = key.strip()
            if not cleaned:
                raise ValueError("axis_scores keys must be non-empty.")
            normalized[cleaned] = float(score)
        return normalized

    def to_dict(self) -> dict[str, float]:
        return dict(self.root)


class CriticDelta(StrictJsonModel):
    axis: str = Field(min_length=1)
    issue: str = Field(min_length=1)
    observation: str = Field(min_length=1)
    direction: CriticDirection
    severity: float | None = None
    confidence: float | None = None


class CriticFix(StrictJsonModel):
    fix_id: str = Field(min_length=1)
    axis: str = Field(min_length=1)
    instruction: str = Field(min_length=1)
    rationale: str = Field(min_length=1)
    priority: int = Field(ge=1, le=5)
    confidence: float = Field(ge=0.0, le=1.0)
    risk: Literal["low", "medium", "high"] = "medium"
    target_hint: str | None = None
    expected_effect: str | None = None


class CriticResult(StrictJsonModel):
    summary: str = Field(min_length=1)
    overall_score: float
    axis_scores: CriticAxisScores
    deltas: list[CriticDelta] = Field(default_factory=list)
    top_fixes: list[CriticFix] = Field(default_factory=list)
    passed: bool = False
    critic_version: str = "v2"
    model_name: str | None = None

    @field_validator("top_fixes")
    @classmethod
    def validate_top_fixes(cls, value: list[CriticFix]) -> list[CriticFix]:
        if len(value) > 5:
            raise ValueError("top_fixes must contain at most 5 items.")
        return value

    @model_validator(mode="after")
    def validate_axis_coverage(self) -> "CriticResult":
        known_axes = set(self.axis_scores.root.keys())
        for delta in self.deltas:
            if delta.axis not in known_axes:
                raise ValueError(f"delta axis '{delta.axis}' is not present in axis_scores.")
        for fix in self.top_fixes:
            if fix.axis not in known_axes:
                raise ValueError(f"fix axis '{fix.axis}' is not present in axis_scores.")
        return self
