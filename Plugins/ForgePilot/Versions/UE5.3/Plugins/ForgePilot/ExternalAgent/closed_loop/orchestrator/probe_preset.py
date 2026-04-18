from __future__ import annotations

from pathlib import Path
from typing import Any

from ..mcp.probe_tools import ProbeTools
from ..mcp.ue_mcp_client import UeMcpClient
from ..models.semantic_models import ProbeCaptureResult, StableProbePreset
from .artifact_writer import RunArtifacts


DEFAULT_STABLE_PROBE_PRESET = StableProbePreset(
    preset_id="universal_sphere_v1",
    mesh_path="/Engine/BasicShapes/Sphere.Sphere",
    actor_label_template="ForgePilot_StableProbe_{asset_token}",
    location=[0.0, 0.0, 120.0],
    rotation=[0.0, 0.0, 0.0],
    scale=[2.5, 2.5, 2.5],
    capture_mode_policy=["scene_capture", "editor_preview_thumbnail", "mesh_thumbnail_fallback"],
    frame_actor=True,
    wait_ms=200,
)


def _probe_tools(context: dict[str, Any]) -> ProbeTools:
    tools = context.get("probe_tools")
    if isinstance(tools, ProbeTools):
        return tools
    client = context.get("mcp_client")
    if not isinstance(client, UeMcpClient):
        client = UeMcpClient()
    return ProbeTools(client)


def _preset_from_context(context: dict[str, Any]) -> StableProbePreset:
    preset = context.get("probe_preset")
    if isinstance(preset, StableProbePreset):
        return preset
    if isinstance(preset, dict):
        return StableProbePreset.model_validate(preset)
    return DEFAULT_STABLE_PROBE_PRESET


def _asset_token(asset_path: str) -> str:
    tail = asset_path.rsplit("/", 1)[-1]
    token = tail.split(".")[-1] if "." in tail else tail
    cleaned = "".join(ch if ch.isalnum() or ch == "_" else "_" for ch in token)
    return cleaned or "Material"


def ensure_stable_probe_actor(asset_path: str, context: dict[str, Any]) -> str:
    preset = _preset_from_context(context)
    actor_label = context.get("probe_actor_label")
    if isinstance(actor_label, str) and actor_label:
        return actor_label
    actor_label = preset.actor_label_template.format(asset_token=_asset_token(asset_path))
    _probe_tools(context).spawn_material_probe_actor(
        actor_label=actor_label,
        mesh_path=preset.mesh_path,
        location=list(preset.location),
        rotation=list(preset.rotation),
        scale=list(preset.scale),
    )
    context["probe_actor_label"] = actor_label
    return actor_label


def capture_with_stable_probe_preset(asset_path: str, material_path: str, stage_id: str, context: dict[str, Any]) -> ProbeCaptureResult:
    preset = _preset_from_context(context)
    tools = _probe_tools(context)
    actor_label = ensure_stable_probe_actor(asset_path, context)
    tools.apply_material_to_probe_actor(actor_label, material_path)

    artifacts = context.get("run_artifacts")
    output_dir = str((artifacts.stage_probes_dir if isinstance(artifacts, RunArtifacts) else Path.cwd()))
    file_name = f"{stage_id}.png"

    response = tools.capture_material_visual_probe(
        actor_label=actor_label,
        output_dir=output_dir,
        file_name=file_name,
        frame_actor=preset.frame_actor,
        wait_ms=preset.wait_ms,
        prefer_editor_preview=True,
        force_thumbnail_fallback=False,
    )
    output_path = str(response.get("output_path", ""))
    capture_mode = str(response.get("capture_mode", ""))
    return ProbeCaptureResult(
        success=bool(response.get("success", False) and output_path),
        actor_label=actor_label,
        mesh_path=preset.mesh_path,
        material_path=material_path,
        output_path=output_path or None,
        output_dir=str(Path(output_path).parent) if output_path else None,
        capture_mode=capture_mode or None,
        frame_actor=preset.frame_actor,
        preset_id=preset.preset_id,
        details=response if isinstance(response, dict) else {"response": response},
    )


def capture_stage_probe(asset_path: str, material_path: str, stage_id: str, context: dict[str, Any]) -> ProbeCaptureResult:
    return capture_with_stable_probe_preset(asset_path, material_path, stage_id, context)
