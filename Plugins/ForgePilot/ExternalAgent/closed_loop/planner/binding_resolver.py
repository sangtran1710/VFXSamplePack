from __future__ import annotations

from typing import Any

from ..models.planner_models import McpBinding, NormalizedOp


def _stringify(value: Any) -> str:
    if isinstance(value, bool):
        return "true" if value else "false"
    return str(value)


def resolve_mcp_binding(op: NormalizedOp) -> McpBinding:
    params = op.params
    if op.op_type == "set_scalar":
        return McpBinding(
            tool_name="set_material_scalar_param",
            arguments={
                "material_path": params["material_path"],
                "param_name": params["param_name"],
                "default_value": _stringify(params["default_value"]),
                "apply_changes": "true",
            },
        )
    if op.op_type == "set_vector":
        return McpBinding(
            tool_name="set_material_vector_param",
            arguments={
                "material_path": params["material_path"],
                "param_name": params["param_name"],
                "default_value": params["default_value"],
                "apply_changes": "true",
            },
        )
    if op.op_type == "set_texture":
        return McpBinding(
            tool_name="set_material_texture_param",
            arguments={
                "material_path": params["material_path"],
                "param_name": params["param_name"],
                "texture_path": params["texture_path"],
                "apply_changes": "true",
            },
        )
    if op.op_type == "add_expression":
        arguments = {
            "material_path": params["material_path"],
            "expression_type": params["expression_type"],
            "apply_changes": "true",
        }
        if params.get("param_name"):
            arguments["param_name"] = params["param_name"]
        return McpBinding(tool_name="add_material_expression", arguments=arguments)
    if op.op_type == "set_material_output":
        return McpBinding(
            tool_name="connect_material_nodes",
            arguments={
                "material_path": params["material_path"],
                "source_index": _stringify(params["source_index"]),
                "target_type": "material_pin",
                "target_pin": params["target_pin"],
                "apply_changes": "true",
            },
        )
    if op.op_type == "connect_nodes":
        return McpBinding(
            tool_name="connect_material_nodes",
            arguments={
                "material_path": params["material_path"],
                "source_index": _stringify(params["source_index"]),
                "target_type": "expression",
                "target_index": _stringify(params["target_index"]),
                "target_input_index": _stringify(params.get("target_input_index", 0)),
                "apply_changes": "true",
            },
        )
    raise ValueError(f"Unsupported MCP binding for op_type={op.op_type}")
