from __future__ import annotations

from datetime import datetime
from typing import Literal

from pydantic import Field, field_validator

from .critic_models import CriticResult
from .orchestrator_models import LoopResult
from .shared_types import JsonValue, RiskLevel, StrictJsonModel, utc_now


class VisualCue(StrictJsonModel):
    cue_id: str = Field(min_length=1)
    description: str = Field(min_length=1)
    importance: Literal["critical", "major", "supporting"] = "major"
    layer: str | None = None


class VisualLayerSpec(StrictJsonModel):
    layer_id: str = Field(min_length=1)
    label: str = Field(min_length=1)
    role: Literal["core", "body", "breakup", "depth", "rim", "color_identity", "other"]
    priority: int = Field(ge=1, le=10)
    description: str = Field(min_length=1)
    desired_read: str = Field(min_length=1)
    must_preserve: bool = False


class StageRecipeItem(StrictJsonModel):
    stage_id: str = Field(min_length=1)
    label: str = Field(min_length=1)
    goal: str = Field(min_length=1)
    target_layer: str = Field(min_length=1)
    allowed_node_families: list[str] = Field(default_factory=list)
    forbidden_node_families: list[str] = Field(default_factory=list)
    validation_rules: list[str] = Field(default_factory=list)
    expected_visual_change: str = Field(min_length=1)
    stop_conditions: list[str] = Field(default_factory=list)


class AnalyzerOutput(StrictJsonModel):
    archetype: str = Field(min_length=1)
    material_family: str = Field(min_length=1)
    layer_stack: list[VisualLayerSpec] = Field(default_factory=list)
    dominant_read: str = Field(min_length=1)
    secondary_reads: list[str] = Field(default_factory=list)
    stage_recipe: list[StageRecipeItem] = Field(default_factory=list)
    hard_constraints: list[str] = Field(default_factory=list)
    anti_patterns: list[str] = Field(default_factory=list)
    failure_sensitive_constraints: list[str] = Field(default_factory=list)
    visual_cues: list[VisualCue] = Field(default_factory=list)
    analyzer_version: str = "v1"
    model_name: str | None = None

    @field_validator("layer_stack", "stage_recipe")
    @classmethod
    def _must_not_be_empty(cls, value: list) -> list:
        if not value:
            raise ValueError("layer_stack and stage_recipe must not be empty.")
        return value


class SemanticStagePlan(StrictJsonModel):
    stage_id: str = Field(min_length=1)
    label: str = Field(min_length=1)
    goal: str = Field(min_length=1)
    target_layer: str = Field(min_length=1)
    allowed_ops: list[str] = Field(default_factory=list)
    forbidden_ops: list[str] = Field(default_factory=list)
    validation_rules: list[str] = Field(default_factory=list)
    expected_visual_change: str = Field(min_length=1)
    hard_constraints: list[str] = Field(default_factory=list)
    anti_patterns: list[str] = Field(default_factory=list)
    stop_conditions: list[str] = Field(default_factory=list)
    preferred_lanes: list[str] = Field(default_factory=list)


class SemanticVisualPlan(StrictJsonModel):
    archetype: str = Field(min_length=1)
    material_family: str = Field(min_length=1)
    dominant_read: str = Field(min_length=1)
    stages: list[SemanticStagePlan] = Field(default_factory=list)
    hard_constraints: list[str] = Field(default_factory=list)
    anti_patterns: list[str] = Field(default_factory=list)
    plan_version: str = "v1"

    @field_validator("stages")
    @classmethod
    def _validate_stages(cls, value: list[SemanticStagePlan]) -> list[SemanticStagePlan]:
        if not value:
            raise ValueError("stages must not be empty.")
        return value


class FailureClassification(StrictJsonModel):
    failure_class: Literal["complete", "structural", "shaping", "color", "saturation", "capture", "unknown"]
    target_stage: str | None = None
    target_lane: str | None = None
    target_nodes_or_params: list[str] = Field(default_factory=list)
    change_type: Literal["advance_stage", "repair_stage", "parameter_repair", "shaping_repair", "lane_rebuild", "rollback", "fix_capture", "stop"]
    expected_effect: str = Field(min_length=1)
    risk: RiskLevel = "medium"
    summary: str = Field(min_length=1)
    evidence: list[str] = Field(default_factory=list)
    confidence: float = Field(ge=0.0, le=1.0)


class StageBuildResult(StrictJsonModel):
    stage_id: str = Field(min_length=1)
    asset_path: str = Field(min_length=1)
    success: bool
    compile_ok: bool = True
    summary: str = Field(min_length=1)
    comment_name: str | None = None
    expression_indices: list[int] = Field(default_factory=list)
    node_inventory: list[str] = Field(default_factory=list)
    connection_inventory: list[str] = Field(default_factory=list)
    parameter_inventory: list[str] = Field(default_factory=list)
    probe_image: str | None = None
    graph_captures: dict[str, str] = Field(default_factory=dict)
    asset_context: dict[str, JsonValue] = Field(default_factory=dict)
    notes: list[str] = Field(default_factory=list)


class StageWorkflowRecord(StrictJsonModel):
    stage_id: str = Field(min_length=1)
    stage_goal: str = Field(min_length=1)
    stage_label: str = Field(min_length=1)
    build_result: StageBuildResult | None = None
    critic_result: CriticResult | None = None
    failure_classification: FailureClassification | None = None
    fix_loop_result: LoopResult | None = None
    stage_status: Literal["pending", "approved", "repaired", "failed", "blocked"] = "pending"
    probe_image: str | None = None
    notes: list[str] = Field(default_factory=list)


class VisualMaterialWorkflowConfig(StrictJsonModel):
    output_root: str | None = None
    critic_mode: str | None = None
    pass_threshold: float = Field(default=0.9, ge=0.0, le=1.0)
    max_stage_repairs: int = Field(default=1, ge=0, le=4)
    max_fix_iterations: int = Field(default=2, ge=0, le=6)
    stop_on_unclassified_failure: bool = True
    allow_stage_repairs: bool = True
    probe_visual_change_threshold: float = Field(default=0.75, ge=0.0)


class StageMacroContext(StrictJsonModel):
    asset_path: str = Field(min_length=1)
    stage_plan: SemanticStagePlan
    material_context: dict[str, JsonValue] = Field(default_factory=dict)
    preset: dict[str, JsonValue] = Field(default_factory=dict)
    artifact_run_dir: str | None = None
    actor_label: str | None = None


class StageMacroResult(StrictJsonModel):
    success: bool
    compile_ok: bool = True
    summary: str = Field(min_length=1)
    comment_name: str | None = None
    expression_indices: list[int] = Field(default_factory=list)
    node_inventory: list[str] = Field(default_factory=list)
    connection_inventory: list[str] = Field(default_factory=list)
    parameter_inventory: list[str] = Field(default_factory=list)
    probe_image: str | None = None
    graph_captures: dict[str, str] = Field(default_factory=dict)
    asset_context: dict[str, JsonValue] = Field(default_factory=dict)
    notes: list[str] = Field(default_factory=list)


class StableProbePreset(StrictJsonModel):
    preset_id: str = Field(min_length=1)
    mesh_path: str = Field(min_length=1)
    actor_label_template: str = Field(min_length=1)
    location: list[float] = Field(default_factory=list)
    rotation: list[float] = Field(default_factory=list)
    scale: list[float] = Field(default_factory=list)
    capture_mode_policy: list[str] = Field(default_factory=list)
    frame_actor: bool = True
    wait_ms: int = Field(default=150, ge=0)


class ProbeCaptureResult(StrictJsonModel):
    success: bool
    actor_label: str = Field(min_length=1)
    mesh_path: str = Field(min_length=1)
    material_path: str = Field(min_length=1)
    output_path: str | None = None
    output_dir: str | None = None
    capture_mode: str | None = None
    frame_actor: bool = True
    preset_id: str = Field(min_length=1)
    details: dict[str, JsonValue] = Field(default_factory=dict)


class GateResult(StrictJsonModel):
    gate_name: str = Field(min_length=1)
    severity: Literal["hard", "soft"]
    passed: bool
    details: dict[str, JsonValue] = Field(default_factory=dict)
    recommended_action: str | None = None


class LocalValidationReport(StrictJsonModel):
    hard_failures: list[GateResult] = Field(default_factory=list)
    warnings: list[GateResult] = Field(default_factory=list)
    should_call_critic: bool = True
    failure_class_override: Literal["capture", "structural", "failed", "saturation", "no_visible_change"] | None = None
    notes: list[str] = Field(default_factory=list)


class VisualMaterialWorkflowResult(StrictJsonModel):
    asset_path: str = Field(min_length=1)
    reference_images: list[str] = Field(default_factory=list)
    text_goal: str | None = None
    status: Literal["passed", "stopped", "failed", "blocked"]
    analyzer_output: AnalyzerOutput | None = None
    semantic_plan: SemanticVisualPlan | None = None
    stages: list[StageWorkflowRecord] = Field(default_factory=list)
    run_dir: str | None = None
    started_at: datetime = Field(default_factory=utc_now)
    ended_at: datetime | None = None
