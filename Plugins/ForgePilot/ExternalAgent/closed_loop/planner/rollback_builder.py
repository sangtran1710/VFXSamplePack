from __future__ import annotations

from ..models.planner_models import NormalizedOp


def build_rollback_ops(op: NormalizedOp) -> list[NormalizedOp]:
    params = op.params
    rollback_id = f"{op.op_id}_rollback"
    if op.op_type == "set_scalar" and params.get("previous_value") is not None:
        return [
            NormalizedOp(
                op_id=rollback_id,
                op_type="set_scalar",
                target=op.target,
                params={
                    "material_path": params["material_path"],
                    "param_name": params["param_name"],
                    "default_value": params["previous_value"],
                },
                resolver_tier="direct_parameter",
            )
        ]
    if op.op_type == "set_vector" and params.get("previous_value") is not None:
        return [
            NormalizedOp(
                op_id=rollback_id,
                op_type="set_vector",
                target=op.target,
                params={
                    "material_path": params["material_path"],
                    "param_name": params["param_name"],
                    "default_value": params["previous_value"],
                },
                resolver_tier="direct_parameter",
            )
        ]
    if op.op_type == "set_texture" and params.get("previous_value") is not None:
        return [
            NormalizedOp(
                op_id=rollback_id,
                op_type="set_texture",
                target=op.target,
                params={
                    "material_path": params["material_path"],
                    "param_name": params["param_name"],
                    "texture_path": params["previous_value"],
                },
                resolver_tier="direct_parameter",
            )
        ]
    if op.op_type == "add_expression":
        return [
            NormalizedOp(
                op_id=rollback_id,
                op_type="delete_expression",
                target=op.target,
                params={
                    "material_path": params["material_path"],
                    "expression_index": f"$$result.{op.op_id}.expression_index",
                },
                resolver_tier="direct_expression",
            )
        ]
    if op.op_type == "set_material_output" and params.get("previous_source_index") is not None:
        return [
            NormalizedOp(
                op_id=rollback_id,
                op_type="set_material_output",
                target=op.target,
                params={
                    "material_path": params["material_path"],
                    "source_index": params["previous_source_index"],
                    "target_pin": params["target_pin"],
                },
                resolver_tier="direct_expression",
            )
        ]
    return []
