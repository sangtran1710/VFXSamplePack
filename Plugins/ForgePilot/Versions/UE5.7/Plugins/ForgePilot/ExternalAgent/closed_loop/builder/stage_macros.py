from __future__ import annotations

import json
from collections.abc import Callable
from dataclasses import dataclass
from typing import Any

from ..models.semantic_models import SemanticStagePlan, StageMacroContext, StageMacroResult


MacroFn = Callable[[StageMacroContext, dict[str, Any]], StageMacroResult]


@dataclass(slots=True)
class StageMacroRegistry:
    macros: dict[str, MacroFn]

    def register(self, key: str, macro: MacroFn) -> None:
        self.macros[key] = macro

    def resolve(self, stage_plan: SemanticStagePlan) -> MacroFn:
        keys = [
            stage_plan.stage_id.lower(),
            stage_plan.target_layer.lower(),
            stage_plan.label.lower().replace(" ", "_"),
        ]
        aliases = {
            "core": "core_mask",
            "stage_1_core": "core_mask",
            "breakup": "primary_filament_breakup",
            "stage_2_filament": "primary_filament_breakup",
            "depth": "secondary_turbulence",
            "stage_3_depth": "secondary_turbulence",
            "rim": "rim_accent",
            "stage_4_rim": "rim_accent",
            "color_identity": "heat_gradient",
            "stage_5_color": "heat_gradient",
        }
        for key in keys:
            resolved = aliases.get(key, key)
            if resolved in self.macros:
                return self.macros[resolved]
        raise KeyError(f"No stage macro registered for stage_id={stage_plan.stage_id} target_layer={stage_plan.target_layer}")


def _macro_tools(context: dict[str, Any]):
    return context["macro_tools"]


def _state(context: dict[str, Any]) -> dict[str, Any]:
    material_context = context.setdefault("material_context", {})
    return material_context.setdefault("macro_state", {})


def _add_expr(context: dict[str, Any], material_path: str, expression_type: str, *, x: int, y: int, param_name: str | None = None, default_value: str | None = None, texture_path: str | None = None, settings: dict[str, Any] | None = None) -> int:
    settings_json = json.dumps(settings, ensure_ascii=False) if settings is not None else None
    response = _macro_tools(context).add_material_expression(
        material_path,
        expression_type,
        param_name=param_name,
        default_value=default_value,
        texture_path=texture_path,
        settings_json=settings_json,
        editor_x=x,
        editor_y=y,
        apply_changes=True,
    )
    return int(response.get("expression_index", -1))


def _connect_expr(context: dict[str, Any], material_path: str, source_index: int, *, target_type: str, source_output_index: int = 0, target_pin: str | None = None, target_index: int | None = None, target_input_index: int | None = None, target_input_name: str | None = None) -> None:
    _macro_tools(context).connect_material_nodes(
        material_path,
        source_index,
        target_type,
        source_output_index=source_output_index,
        target_pin=target_pin,
        target_index=target_index,
        target_input_index=target_input_index,
        target_input_name=target_input_name,
        apply_changes=True,
    )


def _comment_block(context: dict[str, Any], material_path: str, *, comment_name: str, expression_indices: list[int], stage_index: int) -> None:
    x = -2600 + (stage_index * 240)
    y = -800 + (stage_index * 140)
    _macro_tools(context).create_material_comment(
        material_path,
        comment_text=comment_name,
        editor_x=x,
        editor_y=y,
        width=1400,
        height=1200,
        color_rgba=[0.9, 0.82, 0.28, 1.0],
        apply_changes=True,
    )
    _macro_tools(context).update_material_comment(
        material_path,
        comment_text=comment_name,
        wrap_expression_indices=expression_indices,
        padding=140,
        apply_changes=True,
    )


def core_mask(context_model: StageMacroContext, context: dict[str, Any]) -> StageMacroResult:
    material_path = context_model.asset_path
    stage_index = int(context.get("stage_index", 0))
    state = _state(context)
    tools = _macro_tools(context)
    tools.set_material_properties(material_path, shading_model="Unlit", apply_changes=True)

    texcoord = _add_expr(context, material_path, "TexCoord", x=-2200, y=120)
    center = _add_expr(context, material_path, "Constant2", x=-2200, y=320, default_value="0.5,0.5,0.0,1.0")
    subtract = _add_expr(context, material_path, "Subtract", x=-1900, y=160)
    dot = _add_expr(context, material_path, "DotProduct", x=-1620, y=160)
    one_minus = _add_expr(context, material_path, "OneMinus", x=-1360, y=160)
    clamp = _add_expr(context, material_path, "Clamp", x=-1120, y=160)
    power = _add_expr(context, material_path, "Power", x=-860, y=160)
    exponent = _add_expr(context, material_path, "Constant", x=-1120, y=340, default_value="4.0")
    emissive_intensity = _add_expr(context, material_path, "ScalarParameter", x=-640, y=320, param_name="EmissiveIntensity", default_value="15.0")
    multiply = _add_expr(context, material_path, "Multiply", x=-420, y=160)

    _connect_expr(context, material_path, texcoord, target_type="expression", target_index=subtract, target_input_index=0)
    _connect_expr(context, material_path, center, target_type="expression", target_index=subtract, target_input_index=1)
    _connect_expr(context, material_path, subtract, target_type="expression", target_index=dot, target_input_index=0)
    _connect_expr(context, material_path, subtract, target_type="expression", target_index=dot, target_input_index=1)
    _connect_expr(context, material_path, dot, target_type="expression", target_index=one_minus, target_input_index=0)
    _connect_expr(context, material_path, one_minus, target_type="expression", target_index=clamp, target_input_index=0)
    _connect_expr(context, material_path, clamp, target_type="expression", target_index=power, target_input_index=0)
    _connect_expr(context, material_path, exponent, target_type="expression", target_index=power, target_input_index=1)
    _connect_expr(context, material_path, power, target_type="expression", target_index=multiply, target_input_index=0)
    _connect_expr(context, material_path, emissive_intensity, target_type="expression", target_index=multiply, target_input_index=1)
    _connect_expr(context, material_path, multiply, target_type="material_pin", target_pin="EmissiveColor")

    expression_indices = [texcoord, center, subtract, dot, one_minus, clamp, power, exponent, emissive_intensity, multiply]
    comment_name = context_model.stage_plan.label
    _comment_block(context, material_path, comment_name=comment_name, expression_indices=expression_indices, stage_index=stage_index)

    state.update(
        {
            "texcoord": texcoord,
            "core_mask": power,
            "emissive_multiply": multiply,
            "emissive_root": multiply,
            "comment_core": comment_name,
        }
    )
    return StageMacroResult(
        success=True,
        compile_ok=True,
        summary="Built centered core mask and emissive scalar lane.",
        comment_name=comment_name,
        expression_indices=expression_indices,
        node_inventory=[
            "TexCoord",
            "Constant2(0.5,0.5)",
            "Subtract",
            "DotProduct",
            "OneMinus",
            "Clamp",
            "Power",
            "ScalarParameter(EmissiveIntensity)",
            "Multiply",
        ],
        connection_inventory=[
            "TexCoord -> Subtract.A",
            "Center -> Subtract.B",
            "Subtract -> Dot(A,B)",
            "Dot -> OneMinus -> Clamp -> Power",
            "Power * EmissiveIntensity -> EmissiveColor",
        ],
        parameter_inventory=["EmissiveIntensity"],
        asset_context={"macro_state": state},
    )


def primary_filament_breakup(context_model: StageMacroContext, context: dict[str, Any]) -> StageMacroResult:
    material_path = context_model.asset_path
    stage_index = int(context.get("stage_index", 1))
    state = _state(context)
    texcoord = int(state["texcoord"])
    core_mask = int(state["core_mask"])
    emissive_multiply = int(state["emissive_multiply"])

    pan_speed = _add_expr(context, material_path, "Constant2", x=-1780, y=560, default_value="0.08,0.05,0.0,1.0")
    panner = _add_expr(context, material_path, "Panner", x=-1540, y=500)
    zero = _add_expr(context, material_path, "Constant", x=-1540, y=700, default_value="0.0")
    append = _add_expr(context, material_path, "AppendVector", x=-1280, y=540)
    noise = _add_expr(
        context,
        material_path,
        "Noise",
        x=-1020,
        y=540,
        settings={
            "noise_function": "SimplexTex",
            "scale": 18,
            "quality": 1,
            "levels": 4,
            "output_min": 0,
            "output_max": 1,
            "level_scale": 2,
            "turbulence": True,
            "tiling": False,
            "repeat_size": 128,
        },
    )
    power = _add_expr(context, material_path, "Power", x=-760, y=540)
    contrast = _add_expr(context, material_path, "Constant", x=-760, y=740, default_value="6.0")
    floor = _add_expr(context, material_path, "Constant", x=-760, y=920, default_value="0.28")
    ceiling = _add_expr(context, material_path, "Constant", x=-760, y=1080, default_value="1.0")
    lerp = _add_expr(context, material_path, "Lerp", x=-500, y=680)
    multiply = _add_expr(context, material_path, "Multiply", x=-220, y=420)

    _connect_expr(context, material_path, texcoord, target_type="expression", target_index=panner, target_input_index=0)
    _connect_expr(context, material_path, pan_speed, target_type="expression", target_index=panner, target_input_index=2)
    _connect_expr(context, material_path, panner, target_type="expression", target_index=append, target_input_index=0)
    _connect_expr(context, material_path, zero, target_type="expression", target_index=append, target_input_index=1)
    _connect_expr(context, material_path, append, target_type="expression", target_index=noise, target_input_name="Position")
    _connect_expr(context, material_path, noise, target_type="expression", source_output_index=0, target_index=power, target_input_index=0)
    _connect_expr(context, material_path, contrast, target_type="expression", target_index=power, target_input_index=1)
    _connect_expr(context, material_path, floor, target_type="expression", target_index=lerp, target_input_index=0)
    _connect_expr(context, material_path, ceiling, target_type="expression", target_index=lerp, target_input_index=1)
    _connect_expr(context, material_path, power, target_type="expression", target_index=lerp, target_input_index=2)
    _connect_expr(context, material_path, core_mask, target_type="expression", target_index=multiply, target_input_index=0)
    _connect_expr(context, material_path, lerp, target_type="expression", target_index=multiply, target_input_index=1)
    _connect_expr(context, material_path, multiply, target_type="expression", target_index=emissive_multiply, target_input_index=0)

    expression_indices = [pan_speed, panner, zero, append, noise, power, contrast, floor, ceiling, lerp, multiply]
    comment_name = context_model.stage_plan.label
    _comment_block(context, material_path, comment_name=comment_name, expression_indices=expression_indices, stage_index=stage_index)

    state.update(
        {
            "append_vector": append,
            "filament_mask": lerp,
            "filament_core_mul": multiply,
            "comment_filament": comment_name,
        }
    )
    return StageMacroResult(
        success=True,
        compile_ok=True,
        summary="Built primary filament breakup branch with preservation floor.",
        comment_name=comment_name,
        expression_indices=expression_indices,
        node_inventory=["Constant2(PanSpeed)", "Panner", "AppendVector", "Noise", "Power", "Lerp", "Multiply"],
        connection_inventory=[
            "TexCoord -> Panner -> AppendVector(Z=0) -> Noise.Position",
            "Noise -> Power -> Lerp(floor,1.0)",
            "CoreMask * FilamentMask -> Emissive core input",
        ],
        parameter_inventory=[],
        asset_context={"macro_state": state},
    )


def secondary_turbulence(context_model: StageMacroContext, context: dict[str, Any]) -> StageMacroResult:
    material_path = context_model.asset_path
    stage_index = int(context.get("stage_index", 2))
    state = _state(context)
    filament_mask = int(state["filament_mask"])
    filament_core_mul = int(state["filament_core_mul"])

    secondary_noise = _add_expr(
        context,
        material_path,
        "Noise",
        x=-980,
        y=1180,
        settings={
            "noise_function": "GradientTex",
            "scale": 10,
            "quality": 1,
            "levels": 3,
            "output_min": 0,
            "output_max": 1,
            "level_scale": 2,
            "turbulence": True,
            "tiling": False,
            "repeat_size": 128,
        },
    )
    power = _add_expr(context, material_path, "Power", x=-720, y=1180)
    exponent = _add_expr(context, material_path, "Constant", x=-720, y=1360, default_value="4.2")
    floor = _add_expr(context, material_path, "Constant", x=-720, y=1520, default_value="0.62")
    ceiling = _add_expr(context, material_path, "Constant", x=-720, y=1680, default_value="1.0")
    lerp = _add_expr(context, material_path, "Lerp", x=-460, y=1320)
    multiply = _add_expr(context, material_path, "Multiply", x=-180, y=900)

    if state.get("append_vector") is not None:
        _connect_expr(context, material_path, int(state["append_vector"]), target_type="expression", target_index=secondary_noise, target_input_name="Position")
    else:
        texcoord = int(state["texcoord"])
        zero = _add_expr(context, material_path, "Constant", x=-1240, y=1350, default_value="0.0")
        append_vec = _add_expr(context, material_path, "AppendVector", x=-1240, y=1180)
        _connect_expr(context, material_path, texcoord, target_type="expression", target_index=append_vec, target_input_index=0)
        _connect_expr(context, material_path, zero, target_type="expression", target_index=append_vec, target_input_index=1)
        _connect_expr(context, material_path, append_vec, target_type="expression", target_index=secondary_noise, target_input_name="Position")
        state["append_vector"] = append_vec

    _connect_expr(context, material_path, secondary_noise, target_type="expression", target_index=power, target_input_index=0)
    _connect_expr(context, material_path, exponent, target_type="expression", target_index=power, target_input_index=1)
    _connect_expr(context, material_path, floor, target_type="expression", target_index=lerp, target_input_index=0)
    _connect_expr(context, material_path, ceiling, target_type="expression", target_index=lerp, target_input_index=1)
    _connect_expr(context, material_path, power, target_type="expression", target_index=lerp, target_input_index=2)
    _connect_expr(context, material_path, filament_mask, target_type="expression", target_index=multiply, target_input_index=0)
    _connect_expr(context, material_path, lerp, target_type="expression", target_index=multiply, target_input_index=1)
    _connect_expr(context, material_path, multiply, target_type="expression", target_index=filament_core_mul, target_input_index=1)

    expression_indices = [secondary_noise, power, exponent, floor, ceiling, lerp, multiply]
    comment_name = context_model.stage_plan.label
    _comment_block(context, material_path, comment_name=comment_name, expression_indices=expression_indices, stage_index=stage_index)

    state.update({"secondary_depth_mask": lerp, "secondary_depth_mul": multiply, "comment_depth": comment_name})
    return StageMacroResult(
        success=True,
        compile_ok=True,
        summary="Built secondary turbulence branch at a softer scale.",
        comment_name=comment_name,
        expression_indices=expression_indices,
        node_inventory=["Noise", "Power", "Lerp", "Multiply"],
        connection_inventory=["AppendVector -> Noise.Position", "Noise -> Power -> Lerp(0.62,1.0)", "FilamentMask * SecondaryDepth -> Stage2 mask input"],
        parameter_inventory=[],
        asset_context={"macro_state": state},
    )


def rim_accent(context_model: StageMacroContext, context: dict[str, Any]) -> StageMacroResult:
    material_path = context_model.asset_path
    stage_index = int(context.get("stage_index", 3))
    state = _state(context)
    current_root = int(state["emissive_root"])

    fresnel = _add_expr(context, material_path, "Fresnel", x=60, y=-220)
    power = _add_expr(context, material_path, "Power", x=320, y=-220)
    exponent = _add_expr(context, material_path, "Constant", x=320, y=-40, default_value="2.0")
    rim_intensity = _add_expr(context, material_path, "ScalarParameter", x=320, y=120, param_name="RimIntensity", default_value="2.0")
    multiply = _add_expr(context, material_path, "Multiply", x=580, y=-220)
    add = _add_expr(context, material_path, "Add", x=820, y=-40)

    _connect_expr(context, material_path, fresnel, target_type="expression", target_index=power, target_input_index=0)
    _connect_expr(context, material_path, exponent, target_type="expression", target_index=power, target_input_index=1)
    _connect_expr(context, material_path, power, target_type="expression", target_index=multiply, target_input_index=0)
    _connect_expr(context, material_path, rim_intensity, target_type="expression", target_index=multiply, target_input_index=1)
    _connect_expr(context, material_path, current_root, target_type="expression", target_index=add, target_input_index=0)
    _connect_expr(context, material_path, multiply, target_type="expression", target_index=add, target_input_index=1)
    _connect_expr(context, material_path, add, target_type="material_pin", target_pin="EmissiveColor")

    expression_indices = [fresnel, power, exponent, rim_intensity, multiply, add]
    comment_name = context_model.stage_plan.label
    _comment_block(context, material_path, comment_name=comment_name, expression_indices=expression_indices, stage_index=stage_index)

    state.update({"rim_add": add, "emissive_root": add, "comment_rim": comment_name})
    return StageMacroResult(
        success=True,
        compile_ok=True,
        summary="Built rim accent branch and added it to emissive.",
        comment_name=comment_name,
        expression_indices=expression_indices,
        node_inventory=["Fresnel", "Power", "ScalarParameter(RimIntensity)", "Multiply", "Add"],
        connection_inventory=["Fresnel -> Power -> Multiply(RimIntensity)", "Previous emissive + rim -> EmissiveColor"],
        parameter_inventory=["RimIntensity"],
        asset_context={"macro_state": state},
    )


def heat_gradient(context_model: StageMacroContext, context: dict[str, Any]) -> StageMacroResult:
    material_path = context_model.asset_path
    stage_index = int(context.get("stage_index", 4))
    state = _state(context)
    current_root = int(state["emissive_root"])
    core_mask = int(state["core_mask"])

    outer = _add_expr(context, material_path, "VectorParameter", x=1120, y=40, param_name="OuterColor", default_value="0.58,0.12,0.0,1.0")
    hot = _add_expr(context, material_path, "VectorParameter", x=1120, y=220, param_name="HotCoreColor", default_value="1.18,1.08,1.0,1.0")
    lerp = _add_expr(context, material_path, "Lerp", x=1380, y=140)
    multiply = _add_expr(context, material_path, "Multiply", x=1640, y=140)

    _connect_expr(context, material_path, outer, target_type="expression", target_index=lerp, target_input_index=0)
    _connect_expr(context, material_path, hot, target_type="expression", target_index=lerp, target_input_index=1)
    _connect_expr(context, material_path, core_mask, target_type="expression", target_index=lerp, target_input_index=2)
    _connect_expr(context, material_path, current_root, target_type="expression", target_index=multiply, target_input_index=0)
    _connect_expr(context, material_path, lerp, target_type="expression", target_index=multiply, target_input_index=1)
    _connect_expr(context, material_path, multiply, target_type="material_pin", target_pin="EmissiveColor")

    expression_indices = [outer, hot, lerp, multiply]
    comment_name = context_model.stage_plan.label
    _comment_block(context, material_path, comment_name=comment_name, expression_indices=expression_indices, stage_index=stage_index)

    state.update({"heat_gradient_mul": multiply, "emissive_root": multiply, "comment_color": comment_name})
    return StageMacroResult(
        success=True,
        compile_ok=True,
        summary="Built white-hot core to orange body heat gradient.",
        comment_name=comment_name,
        expression_indices=expression_indices,
        node_inventory=["VectorParameter(OuterColor)", "VectorParameter(HotCoreColor)", "Lerp", "Multiply"],
        connection_inventory=["OuterColor <-> HotCoreColor driven by core mask", "Previous emissive * heat gradient -> EmissiveColor"],
        parameter_inventory=["OuterColor", "HotCoreColor"],
        asset_context={"macro_state": state},
    )


def register_default_stage_macros() -> StageMacroRegistry:
    registry = StageMacroRegistry(macros={})
    registry.register("core_mask", core_mask)
    registry.register("primary_filament_breakup", primary_filament_breakup)
    registry.register("secondary_turbulence", secondary_turbulence)
    registry.register("rim_accent", rim_accent)
    registry.register("heat_gradient", heat_gradient)
    return registry
