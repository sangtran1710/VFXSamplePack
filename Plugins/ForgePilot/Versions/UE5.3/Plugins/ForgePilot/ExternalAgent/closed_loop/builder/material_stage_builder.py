from __future__ import annotations

from pathlib import Path
from typing import Any

from ..mcp.material_tools import MaterialTools
from ..mcp.probe_tools import ProbeTools
from ..mcp.ue_mcp_client import UeMcpClient
from ..models.semantic_models import SemanticStagePlan, StageBuildResult, StageMacroContext
from ..orchestrator.artifact_writer import ArtifactWriter, RunArtifacts
from .stage_macros import StageMacroRegistry, register_default_stage_macros


def _tools_from_context(context: dict[str, Any]) -> tuple[MaterialTools, ProbeTools]:
    material_tools = context.get("material_tools")
    probe_tools = context.get("probe_tools")
    if isinstance(material_tools, MaterialTools) and isinstance(probe_tools, ProbeTools):
        return material_tools, probe_tools

    client = context.get("mcp_client")
    if not isinstance(client, UeMcpClient):
        client = UeMcpClient()
    return MaterialTools(client), ProbeTools(client)


def _compile(material_tools: MaterialTools, asset_path: str) -> tuple[bool, dict[str, Any]]:
    try:
        response = material_tools.compile_material_and_get_errors(asset_path)
        success = bool(response.get("success", True))
        errors = response.get("errors") or response.get("compile_errors") or []
        if errors:
            success = False
        return success, response
    except Exception as exc:
        try:
            info = material_tools.get_material_info(asset_path)
            return bool(info.get("success", True)), {"success": bool(info.get("success", True)), "fallback": "get_material_info", "info": info, "error": str(exc)}
        except Exception as inner_exc:
            return False, {"success": False, "error": str(exc), "fallback_error": str(inner_exc)}


def _graph_artifacts(material_tools: MaterialTools, writer: ArtifactWriter, artifacts: RunArtifacts, asset_path: str, comment_name: str | None, stage_id: str) -> tuple[dict[str, str], dict[str, Any], dict[str, Any] | None, dict[str, Any] | None]:
    graph_captures: dict[str, str] = {}
    info = material_tools.get_material_info(asset_path)
    review = None
    wiring = None
    if comment_name:
        try:
            graph = material_tools.capture_material_graph_screenshot(
                asset_path,
                output_dir=str(artifacts.graphs_dir),
                file_name=f"{stage_id}_logic.png",
                comment_text=comment_name,
                image_width=1800,
                image_height=1100,
                padding=180,
            )
            output_path = graph.get("output_path")
            if isinstance(output_path, str) and Path(output_path).exists():
                graph_captures["logic"] = str(writer.copy_graph_capture(artifacts, stage_id=stage_id, source_image=output_path, variant="logic"))
        except Exception:
            pass
        try:
            wiring = material_tools.summarize_material_block_wiring(asset_path, comment_text=comment_name)
        except Exception:
            wiring = None
    try:
        review = material_tools.review_material_asset(asset_path, creative_mode=False)
    except Exception:
        review = None
    return graph_captures, info, review, wiring


def build_stage_from_semantic_plan(asset_path: str, stage_plan: SemanticStagePlan, context: dict[str, Any]) -> StageBuildResult:
    material_tools, probe_tools = _tools_from_context(context)
    del probe_tools
    registry = context.get("stage_macro_registry")
    if not isinstance(registry, StageMacroRegistry):
        registry = register_default_stage_macros()
    writer = context.get("artifact_writer")
    artifacts = context.get("run_artifacts")
    if not isinstance(writer, ArtifactWriter) or not isinstance(artifacts, RunArtifacts):
        raise RuntimeError("build_stage_from_semantic_plan requires artifact_writer and run_artifacts in context.")

    macro = registry.resolve(stage_plan)
    macro_context = StageMacroContext(
        asset_path=asset_path,
        stage_plan=stage_plan,
        material_context=dict(context.get("material_context", {})),
        preset=dict(context.get("probe_preset_config", {})),
        artifact_run_dir=str(artifacts.run_dir),
        actor_label=context.get("probe_actor_label"),
    )
    macro_result = macro(macro_context, {**context, "macro_tools": material_tools})

    compile_ok, compile_response = _compile(material_tools, asset_path)
    graph_captures, material_info, review, wiring = _graph_artifacts(
        material_tools,
        writer,
        artifacts,
        asset_path,
        macro_result.comment_name,
        stage_plan.stage_id,
    )
    asset_context = dict(macro_result.asset_context)
    asset_context["material_info"] = material_info
    asset_context["compile_response"] = compile_response
    if review is not None:
        asset_context["review"] = review
    if wiring is not None:
        asset_context["wiring_summary"] = wiring
    if macro_result.comment_name:
        asset_context["comment_name"] = macro_result.comment_name
    asset_context.setdefault("modified_layers", [stage_plan.target_layer])

    shared_material_context = context.setdefault("material_context", {})
    shared_material_context.update(asset_context)

    return StageBuildResult(
        stage_id=stage_plan.stage_id,
        asset_path=asset_path,
        success=bool(macro_result.success and compile_ok),
        compile_ok=bool(macro_result.compile_ok and compile_ok),
        summary=macro_result.summary,
        comment_name=macro_result.comment_name,
        expression_indices=list(macro_result.expression_indices),
        node_inventory=list(macro_result.node_inventory),
        connection_inventory=list(macro_result.connection_inventory),
        parameter_inventory=list(macro_result.parameter_inventory),
        probe_image=macro_result.probe_image,
        graph_captures=graph_captures,
        asset_context=shared_material_context,
        notes=list(macro_result.notes),
    )
