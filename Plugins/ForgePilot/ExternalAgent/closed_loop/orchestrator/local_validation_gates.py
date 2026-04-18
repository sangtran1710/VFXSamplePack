from __future__ import annotations

from pathlib import Path
from typing import Any

from ..mcp.material_tools import MaterialTools
from ..mcp.probe_tools import ProbeTools
from ..mcp.ue_mcp_client import UeMcpClient
from ..models.semantic_models import GateResult, LocalValidationReport, ProbeCaptureResult, SemanticStagePlan, StageBuildResult


def _material_tools(context: dict[str, Any]) -> MaterialTools:
    tools = context.get("material_tools")
    if isinstance(tools, MaterialTools):
        return tools
    client = context.get("mcp_client")
    if not isinstance(client, UeMcpClient):
        client = UeMcpClient()
    return MaterialTools(client)


def _probe_tools(context: dict[str, Any]) -> ProbeTools:
    tools = context.get("probe_tools")
    if isinstance(tools, ProbeTools):
        return tools
    client = context.get("mcp_client")
    if not isinstance(client, UeMcpClient):
        client = UeMcpClient()
    return ProbeTools(client)


def _required_root_pins(stage_plan: SemanticStagePlan) -> list[str]:
    if stage_plan.target_layer == "color_identity":
        return ["EmissiveColor"]
    if stage_plan.target_layer == "rim":
        return ["EmissiveColor"]
    if stage_plan.target_layer in {"core", "breakup", "depth"}:
        return ["EmissiveColor"]
    return ["EmissiveColor"]


def _coerce_probe(value: ProbeCaptureResult | dict[str, Any] | None, asset_path: str) -> ProbeCaptureResult:
    if isinstance(value, ProbeCaptureResult):
        return value
    if isinstance(value, dict):
        payload = dict(value)
        payload.setdefault("success", True)
        payload.setdefault("actor_label", "unknown_probe")
        payload.setdefault("mesh_path", "/Engine/BasicShapes/Sphere.Sphere")
        payload.setdefault("material_path", asset_path)
        payload.setdefault("frame_actor", True)
        payload.setdefault("preset_id", "coerced")
        payload.setdefault("details", {})
        return ProbeCaptureResult.model_validate(payload)
    return ProbeCaptureResult(
        success=False,
        actor_label="unknown_probe",
        mesh_path="/Engine/BasicShapes/Sphere.Sphere",
        material_path=asset_path,
        frame_actor=True,
        preset_id="missing",
        details={},
    )


def _headroom_warning(build_result: StageBuildResult) -> GateResult | None:
    parameters = build_result.asset_context.get("material_info", {}).get("result", {}).get("parameters", {})
    scalar_params = parameters.get("scalar") if isinstance(parameters, dict) else None
    if not isinstance(scalar_params, list):
        return None
    high_values: list[str] = []
    for item in scalar_params:
        if not isinstance(item, dict):
            continue
        name = str(item.get("name", ""))
        value = item.get("value")
        if isinstance(value, (int, float)) and float(value) >= 10.0:
            high_values.append(name)
    if not high_values:
        return None
    return GateResult(
        gate_name="saturation_or_headroom_gate",
        severity="soft",
        passed=False,
        details={"high_scalar_params": high_values},
        recommended_action="saturated_control",
    )


def run_pre_critic_gates(
    stage_plan: SemanticStagePlan,
    build_result: StageBuildResult,
    current_probe: ProbeCaptureResult | dict[str, Any],
    previous_probe: ProbeCaptureResult | dict[str, Any] | None,
    context: dict[str, Any],
) -> LocalValidationReport:
    material_tools = _material_tools(context)
    probe_tools = _probe_tools(context)
    report = LocalValidationReport()
    current_probe = _coerce_probe(current_probe, build_result.asset_path)
    previous_probe = _coerce_probe(previous_probe, build_result.asset_path) if previous_probe is not None else None

    compile_response: dict[str, Any]
    try:
        compile_response = material_tools.compile_material_and_get_errors(build_result.asset_path)
        compile_errors = compile_response.get("errors") or compile_response.get("compile_errors") or []
        compile_ok = bool(compile_response.get("success", True)) and not compile_errors
    except Exception as exc:
        fallback_response = build_result.asset_context.get("compile_response")
        if isinstance(fallback_response, dict):
            compile_response = fallback_response
            compile_ok = bool(
                compile_response.get("compiled", compile_response.get("success", False))
            ) and not (compile_response.get("errors") or compile_response.get("compile_errors") or [])
        else:
            compile_response = {"success": False, "error": str(exc)}
            compile_ok = False
    if not compile_ok:
        report.hard_failures.append(
            GateResult(
                gate_name="compile_gate",
                severity="hard",
                passed=False,
                details=compile_response,
                recommended_action="failed",
            )
        )
        report.should_call_critic = False
        report.failure_class_override = "failed"
        return report

    comment_name = build_result.comment_name or stage_plan.label
    try:
        validation = material_tools.validate_material_graph_block(
            build_result.asset_path,
            comment_text=comment_name,
            required_root_pins=_required_root_pins(stage_plan),
            require_comment=True,
        )
    except Exception as exc:
        validation = {"success": False, "error": str(exc)}
    if not bool(validation.get("success", False)):
        report.hard_failures.append(
            GateResult(
                gate_name="graph_integrity_gate",
                severity="hard",
                passed=False,
                details=validation,
                recommended_action="structural",
            )
        )
        report.should_call_critic = False
        report.failure_class_override = "structural"
        return report

    output_path = current_probe.output_path
    capture_mode = str(current_probe.capture_mode or "")
    allowed_modes = current_probe.details.get("allowed_modes") or context.get("probe_preset", {}).get("capture_mode_policy") or ["scene_capture", "editor_preview_thumbnail", "mesh_thumbnail_fallback"]
    if not output_path or not Path(output_path).exists() or (capture_mode and capture_mode not in allowed_modes):
        report.hard_failures.append(
            GateResult(
                gate_name="probe_capture_gate",
                severity="hard",
                passed=False,
                details={"output_path": output_path, "capture_mode": capture_mode, "allowed_modes": allowed_modes},
                recommended_action="capture",
            )
        )
        report.should_call_critic = False
        report.failure_class_override = "capture"
        return report

    if current_probe.material_path != build_result.asset_path:
        report.hard_failures.append(
            GateResult(
                gate_name="asset_binding_gate",
                severity="hard",
                passed=False,
                details={"probe_material_path": current_probe.material_path, "expected_material_path": build_result.asset_path},
                recommended_action="capture",
            )
        )
        report.should_call_critic = False
        report.failure_class_override = "capture"
        return report

    if previous_probe and previous_probe.output_path and current_probe.output_path:
        try:
            diff = probe_tools.compare_material_probe_captures(
                image_a_path=previous_probe.output_path,
                image_b_path=current_probe.output_path,
            )
            mean_abs_diff = float(diff.get("mean_absolute_difference", 0.0))
            threshold = float(context.get("probe_visual_change_threshold", 0.75))
            if mean_abs_diff < threshold:
                report.warnings.append(
                    GateResult(
                        gate_name="visual_change_gate",
                        severity="soft",
                        passed=False,
                        details={"mean_absolute_difference": mean_abs_diff, "threshold": threshold},
                        recommended_action="no_visible_change",
                    )
                )
                report.should_call_critic = False
                report.failure_class_override = "no_visible_change"
        except Exception as exc:
            report.warnings.append(
                GateResult(
                    gate_name="visual_change_gate",
                    severity="soft",
                    passed=False,
                    details={"error": str(exc)},
                    recommended_action="warning",
                )
            )

    saturation_warning = _headroom_warning(build_result)
    if saturation_warning is not None:
        report.warnings.append(saturation_warning)

    modified_layers = build_result.asset_context.get("modified_layers")
    if isinstance(modified_layers, list) and stage_plan.target_layer not in modified_layers:
        report.warnings.append(
            GateResult(
                gate_name="stage_scope_gate",
                severity="soft",
                passed=False,
                details={"modified_layers": modified_layers, "expected_layer": stage_plan.target_layer},
                recommended_action="wrong_stage_scope_warning",
            )
        )

    return report
