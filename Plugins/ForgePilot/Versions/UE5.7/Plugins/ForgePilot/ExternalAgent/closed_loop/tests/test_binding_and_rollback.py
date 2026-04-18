from __future__ import annotations

from ExternalAgent.closed_loop.models.planner_models import NormalizedOp
from ExternalAgent.closed_loop.planner.binding_resolver import resolve_mcp_binding
from ExternalAgent.closed_loop.planner.rollback_builder import build_rollback_ops


def test_binding_resolution() -> None:
    scalar_op = NormalizedOp(
        op_id="step_1",
        op_type="set_scalar",
        target="EmissiveIntensity",
        params={
            "material_path": "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
            "param_name": "EmissiveIntensity",
            "default_value": 2.4,
            "previous_value": 2.0,
        },
        resolver_tier="direct_parameter",
    )
    binding = resolve_mcp_binding(scalar_op)
    assert binding.tool_name == "set_material_scalar_param"
    assert binding.arguments["default_value"] == "2.4"


def test_rollback_generation() -> None:
    scalar_op = NormalizedOp(
        op_id="step_1",
        op_type="set_scalar",
        target="EmissiveIntensity",
        params={
            "material_path": "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
            "param_name": "EmissiveIntensity",
            "default_value": 2.4,
            "previous_value": 2.0,
        },
        resolver_tier="direct_parameter",
    )
    rollback = build_rollback_ops(scalar_op)
    assert len(rollback) == 1
    assert rollback[0].params["default_value"] == 2.0

    add_expr_op = NormalizedOp(
        op_id="step_2",
        op_type="add_expression",
        target="GlowMaskMultiply",
        params={
            "material_path": "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
            "expression_type": "Multiply",
            "param_name": "GlowMaskMultiply",
        },
        resolver_tier="create_fallback",
    )
    rollback_expr = build_rollback_ops(add_expr_op)
    assert rollback_expr[0].op_type == "delete_expression"
    assert rollback_expr[0].params["expression_index"] == "$$result.step_2.expression_index"


def run() -> None:
    test_binding_resolution()
    test_rollback_generation()


if __name__ == "__main__":
    run()
    print("test_binding_and_rollback.py passed")
