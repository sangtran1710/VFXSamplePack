from __future__ import annotations

from typing import Literal

from pydantic import Field

from .shared_types import JsonValue, RiskLevel, StrictJsonModel

NormalizedOpType = Literal[
    "set_scalar",
    "set_vector",
    "set_texture",
    "add_expression",
    "connect_nodes",
    "disconnect_nodes",
    "delete_expression",
    "set_material_output",
]


class NormalizedOp(StrictJsonModel):
    op_id: str = Field(min_length=1)
    op_type: NormalizedOpType
    target: str = Field(min_length=1)
    params: dict[str, JsonValue] = Field(default_factory=dict)
    depends_on: list[str] = Field(default_factory=list)
    resolver_tier: Literal["direct_parameter", "output_path_parameter", "direct_expression", "fuzzy", "create_fallback", "unresolved"] | None = None


class McpBinding(StrictJsonModel):
    tool_name: str = Field(min_length=1)
    arguments: dict[str, JsonValue] = Field(default_factory=dict)


class EstimatedImpact(StrictJsonModel):
    expected_score_delta: float | None = None
    risk: RiskLevel = "medium"
    rationale: str = Field(min_length=1)


class PatchStep(StrictJsonModel):
    step_id: str = Field(min_length=1)
    description: str = Field(min_length=1)
    op: NormalizedOp
    binding: McpBinding | None = None
    preconditions: list[dict[str, JsonValue]] = Field(default_factory=list)
    rollback_ops: list[NormalizedOp] = Field(default_factory=list)
    risk: RiskLevel = "medium"
    compound_group: str | None = None


class PatchPlan(StrictJsonModel):
    selected_fix_ids: list[str] = Field(default_factory=list)
    skipped_fix_ids: list[str] = Field(default_factory=list)
    steps: list[PatchStep] = Field(default_factory=list)
    validation_checks: list[dict[str, JsonValue]] = Field(default_factory=list)
    estimated_impact: EstimatedImpact | None = None
    conservative_mode: bool = False
