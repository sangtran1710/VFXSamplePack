from __future__ import annotations

from typing import Any

from .ue_mcp_client import UeMcpClient, bool_string


class MaterialTools:
    def __init__(self, client: UeMcpClient) -> None:
        self.client = client

    def get_material_editor_context(self) -> dict[str, Any]:
        return self.client.call_tool("get_material_editor_context", {})

    def get_material_info(self, asset_path: str) -> dict[str, Any]:
        return self.client.call_tool("get_material_info", {"material_path": asset_path})

    def compile_material_and_get_errors(self, asset_path: str) -> dict[str, Any]:
        return self.client.call_tool("compile_material_and_get_errors", {"material_path": asset_path})

    def set_material_scalar_param(
        self,
        material_path: str,
        param_name: str,
        default_value: float,
        *,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        return self.client.call_tool(
            "set_material_scalar_param",
            {
                "material_path": material_path,
                "param_name": param_name,
                "default_value": str(default_value),
                "apply_changes": bool_string(apply_changes),
            },
        )

    def set_material_vector_param(
        self,
        material_path: str,
        param_name: str,
        default_value: list[float],
        *,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        return self.client.call_tool(
            "set_material_vector_param",
            {
                "material_path": material_path,
                "param_name": param_name,
                "default_value": default_value,
                "apply_changes": bool_string(apply_changes),
            },
        )

    def set_material_texture_param(
        self,
        material_path: str,
        param_name: str,
        texture_path: str,
        *,
        editor_x: int | None = None,
        editor_y: int | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "param_name": param_name,
            "texture_path": texture_path,
            "apply_changes": bool_string(apply_changes),
        }
        if editor_x is not None:
            payload["editor_x"] = str(editor_x)
        if editor_y is not None:
            payload["editor_y"] = str(editor_y)
        return self.client.call_tool("set_material_texture_param", payload)

    def add_material_expression(
        self,
        material_path: str,
        expression_type: str,
        *,
        param_name: str | None = None,
        default_value: str | None = None,
        texture_path: str | None = None,
        function_path: str | None = None,
        settings_json: str | None = None,
        editor_x: int | None = None,
        editor_y: int | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "expression_type": expression_type,
            "apply_changes": bool_string(apply_changes),
        }
        if param_name is not None:
            payload["param_name"] = param_name
        if default_value is not None:
            payload["default_value"] = default_value
        if texture_path is not None:
            payload["texture_path"] = texture_path
        if function_path is not None:
            payload["function_path"] = function_path
        if settings_json is not None:
            payload["settings_json"] = settings_json
        if editor_x is not None:
            payload["editor_x"] = str(editor_x)
        if editor_y is not None:
            payload["editor_y"] = str(editor_y)
        return self.client.call_tool("add_material_expression", payload)

    def connect_material_nodes(
        self,
        material_path: str,
        source_index: int,
        target_type: str,
        *,
        source_output_index: int | None = None,
        target_pin: str | None = None,
        target_index: int | None = None,
        target_input_index: int | None = None,
        target_input_name: str | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "source_index": str(source_index),
            "target_type": target_type,
            "apply_changes": bool_string(apply_changes),
        }
        if source_output_index is not None:
            payload["source_output_index"] = str(source_output_index)
        if target_pin is not None:
            payload["target_pin"] = target_pin
        if target_index is not None:
            payload["target_index"] = str(target_index)
        if target_input_index is not None:
            payload["target_input_index"] = str(target_input_index)
        if target_input_name is not None:
            payload["target_input_name"] = target_input_name
        return self.client.call_tool("connect_material_nodes", payload)

    def cleanup_material_graph(
        self,
        material_path: str,
        *,
        root_pins: list[str] | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "apply_changes": bool_string(apply_changes),
        }
        if root_pins:
            payload["root_pins"] = root_pins
        return self.client.call_tool("cleanup_material_graph", payload)

    def create_material_comment(
        self,
        material_path: str,
        *,
        comment_text: str,
        editor_x: int,
        editor_y: int,
        width: int,
        height: int,
        color_rgba: list[float] | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "comment_text": comment_text,
            "editor_x": str(editor_x),
            "editor_y": str(editor_y),
            "width": str(width),
            "height": str(height),
            "apply_changes": bool_string(apply_changes),
        }
        if color_rgba is not None:
            payload["color_rgba"] = color_rgba
        return self.client.call_tool("create_material_comment", payload)

    def update_material_comment(
        self,
        material_path: str,
        *,
        comment_text: str,
        wrap_expression_indices: list[int] | None = None,
        padding: int | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "comment_text": comment_text,
            "apply_changes": bool_string(apply_changes),
        }
        if wrap_expression_indices is not None:
            payload["wrap_expression_indices"] = [int(index) for index in wrap_expression_indices]
        if padding is not None:
            payload["padding"] = str(padding)
        return self.client.call_tool("update_material_comment", payload)

    def capture_material_graph_screenshot(
        self,
        material_path: str,
        *,
        output_dir: str,
        file_name: str,
        comment_text: str | None = None,
        image_width: int | None = None,
        image_height: int | None = None,
        padding: int | None = None,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "output_dir": output_dir,
            "file_name": file_name,
        }
        if comment_text is not None:
            payload["comment_text"] = comment_text
        if image_width is not None:
            payload["image_width"] = str(image_width)
        if image_height is not None:
            payload["image_height"] = str(image_height)
        if padding is not None:
            payload["padding"] = str(padding)
        return self.client.call_tool("capture_material_graph_screenshot", payload)

    def validate_material_graph_block(
        self,
        material_path: str,
        *,
        comment_text: str | None = None,
        required_root_pins: list[str] | None = None,
        require_comment: bool | None = None,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
        }
        if comment_text is not None:
            payload["comment_text"] = comment_text
        if required_root_pins is not None:
            payload["required_root_pins"] = required_root_pins
        if require_comment is not None:
            payload["require_comment"] = bool_string(require_comment)
        return self.client.call_tool("validate_material_graph_block", payload)

    def summarize_material_block_wiring(
        self,
        material_path: str,
        *,
        comment_text: str | None = None,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
        }
        if comment_text is not None:
            payload["comment_text"] = comment_text
        return self.client.call_tool("summarize_material_block_wiring", payload)

    def review_material_asset(
        self,
        material_path: str,
        *,
        creative_mode: bool | str | None = None,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
        }
        if creative_mode is not None:
            if isinstance(creative_mode, bool):
                payload["creative_mode"] = bool_string(creative_mode)
            else:
                payload["creative_mode"] = creative_mode
        return self.client.call_tool("review_material_asset", payload)

    def set_material_properties(
        self,
        material_path: str,
        *,
        blend_mode: str | None = None,
        shading_model: str | None = None,
        two_sided: bool | None = None,
        thumbnail_primitive: str | None = None,
        preview_mesh_path: str | None = None,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_path": material_path,
            "apply_changes": bool_string(apply_changes),
        }
        if blend_mode is not None:
            payload["blend_mode"] = blend_mode
        if shading_model is not None:
            payload["shading_model"] = shading_model
        if two_sided is not None:
            payload["two_sided"] = bool_string(two_sided)
        if thumbnail_primitive is not None:
            payload["thumbnail_primitive"] = thumbnail_primitive
        if preview_mesh_path is not None:
            payload["preview_mesh_path"] = preview_mesh_path
        return self.client.call_tool("set_material_properties", payload)
