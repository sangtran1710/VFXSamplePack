from __future__ import annotations

from typing import Any

from .ue_mcp_client import UeMcpClient, bool_string


class ProbeTools:
    def __init__(self, client: UeMcpClient) -> None:
        self.client = client

    def spawn_material_probe_actor(
        self,
        *,
        actor_label: str | None = None,
        mesh_path: str | None = None,
        location: list[float] | None = None,
        rotation: list[float] | None = None,
        scale: list[float] | None = None,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {}
        if actor_label is not None:
            payload["actor_label"] = actor_label
        if mesh_path is not None:
            payload["mesh_path"] = mesh_path
        if location is not None:
            payload["location"] = location
        if rotation is not None:
            payload["rotation"] = rotation
        if scale is not None:
            payload["scale"] = scale
        return self.client.call_tool("spawn_material_probe_actor", payload)

    def apply_material_to_probe_actor(
        self,
        actor_label: str,
        material_path: str,
        *,
        material_index: int = 0,
    ) -> dict[str, Any]:
        return self.client.call_tool(
            "apply_material_to_probe_actor",
            {
                "actor_label": actor_label,
                "material_path": material_path,
                "material_index": str(material_index),
            },
        )

    def frame_probe_actor_for_capture(self, actor_label: str) -> dict[str, Any]:
        return self.client.call_tool("frame_probe_actor_for_capture", {"actor_label": actor_label})

    def capture_material_visual_probe(
        self,
        *,
        actor_label: str | None = None,
        output_dir: str | None = None,
        file_name: str | None = None,
        frame_actor: bool | None = None,
        wait_ms: int | None = None,
        force_thumbnail_fallback: bool | None = None,
        prefer_editor_preview: bool | None = None,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {}
        if actor_label is not None:
            payload["actor_label"] = actor_label
        if output_dir is not None:
            payload["output_dir"] = output_dir
        if file_name is not None:
            payload["file_name"] = file_name
        if frame_actor is not None:
            payload["frame_actor"] = bool_string(frame_actor)
        if wait_ms is not None:
            payload["wait_ms"] = str(wait_ms)
        if force_thumbnail_fallback is not None:
            payload["force_thumbnail_fallback"] = bool_string(force_thumbnail_fallback)
        if prefer_editor_preview is not None:
            payload["prefer_editor_preview"] = bool_string(prefer_editor_preview)
        return self.client.call_tool("capture_material_visual_probe", payload)

    def compare_material_probe_captures(
        self,
        *,
        image_a_path: str,
        image_b_path: str,
    ) -> dict[str, Any]:
        return self.client.call_tool(
            "compare_material_probe_captures",
            {
                "image_a_path": image_a_path,
                "image_b_path": image_b_path,
            },
        )
