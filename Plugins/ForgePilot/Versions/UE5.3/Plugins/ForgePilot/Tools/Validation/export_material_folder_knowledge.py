#!/usr/bin/env python3
import argparse
import json
import os
import socket
import sys
from collections import Counter, defaultdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

SCRIPT_ROOT = Path(__file__).resolve().parent
if str(SCRIPT_ROOT) not in sys.path:
    sys.path.insert(0, str(SCRIPT_ROOT))

from invoke_bridge import add_expected_bridge_identity, get_default_bridge_port  # noqa: E402


DEFAULT_HOST = os.environ.get("FORGEPILOT_BRIDGE_HOST", "127.0.0.1")
DEFAULT_PORT = get_default_bridge_port()
DEFAULT_FOLDER = "/Game/VFX/EnergyBeam/_GenericSource/Material"
DEFAULT_OUTPUT = Path("Output/KnowledgeSnapshots/energybeam_material_knowledge.json")
DEFAULT_NIAGARA_SNAPSHOT = Path("Output/KnowledgeSnapshots/energybeam_ns_knowledge.json")


def bridge_call(host: str, port: int, tool_name: str, payload: dict[str, Any], timeout_seconds: float = 60.0) -> dict[str, Any]:
    request = {
        "action": "execute_tool",
        "tool_name": tool_name,
        "input": payload,
    }
    request = add_expected_bridge_identity(request)
    with socket.create_connection((host, port), timeout=timeout_seconds) as sock:
        sock.settimeout(timeout_seconds)
        sock.sendall((json.dumps(request, ensure_ascii=False) + "\n").encode("utf-8"))

        chunks: list[bytes] = []
        while True:
            chunk = sock.recv(65536)
            if not chunk:
                break
            chunks.append(chunk)
            if b"\n" in chunk:
                break

    raw = b"".join(chunks).decode("utf-8", "ignore").splitlines()[0]
    response = json.loads(raw)
    if not response.get("ok"):
        raise RuntimeError(json.dumps(response, ensure_ascii=False))
    result = response.get("result", {})
    if isinstance(result, dict) and result.get("success") is False:
        raise RuntimeError(json.dumps(result, ensure_ascii=False))
    return result


def normalize_token(value: str) -> str:
    return "".join(ch.lower() for ch in value if ch.isalnum())


def load_niagara_usage(snapshot_path: Path) -> dict[str, list[dict[str, Any]]]:
    if not snapshot_path.exists():
        return {}
    knowledge = json.loads(snapshot_path.read_text(encoding="utf-8"))
    usage: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for system in knowledge.get("systems", []):
        for emitter in system.get("emitters", []):
            for renderer in emitter.get("renderers", []):
                for material_path in renderer.get("used_materials", []):
                    usage[material_path].append(
                        {
                            "system_name": system.get("name"),
                            "system_path": system.get("path"),
                            "emitter_name": emitter.get("emitter_name"),
                            "renderer_name": renderer.get("renderer_name"),
                            "renderer_type": renderer.get("renderer_type"),
                        }
                    )
    return dict(usage)


def summarize_expression_classes(expressions: list[dict[str, Any]]) -> dict[str, int]:
    counter = Counter()
    for expression in expressions:
        counter[expression.get("class", "Unknown")] += 1
    return dict(sorted(counter.items()))


def infer_material_techniques(asset_name: str, material_info: dict[str, Any]) -> list[dict[str, str]]:
    expressions = material_info.get("expressions", [])
    expression_classes = {expr.get("class", "") for expr in expressions}
    function_paths = [str(expr.get("function_path", "")) for expr in expressions if expr.get("class") == "MaterialExpressionMaterialFunctionCall"]
    joined_name = asset_name.lower()

    techniques: list[dict[str, str]] = []

    def add(label: str, evidence: str, why: str, value: str) -> None:
        techniques.append(
            {
                "label": label,
                "evidence": evidence,
                "why_it_exists": why,
                "benefit": value,
            }
        )

    if "MaterialExpressionParticleColor" in expression_classes:
        add(
            "particle_color_driven_tint",
            "Material uses ParticleColor input.",
            "Parent material is designed to inherit color or alpha directly from Niagara rather than baking tint in the graph.",
            "Artists can recolor one material family from Niagara or user params without duplicating the material.",
        )

    if "MaterialExpressionDepthFade" in expression_classes:
        add(
            "depth_fade_intersection_softening",
            "Material uses DepthFade.",
            "This prevents hard sprite cut lines when translucent particles intersect level geometry or other meshes.",
            "Helps impacts, foggy edges, and energy sprites sit in the world more naturally.",
        )

    if "MaterialExpressionTextureSampleParameter2D" in expression_classes:
        add(
            "instance_swappable_shape_texture",
            "Graph exposes TextureSampleParameter2D.",
            "Author wants one parent graph to support many masks, bolts, rings, or flipbook sources through instances.",
            "Reduces graph duplication and makes it cheap to create many variants from one master material.",
        )

    if "MaterialExpressionStaticSwitch" in expression_classes or "MaterialExpressionStaticBoolParameter" in expression_classes:
        add(
            "compile_time_feature_toggles",
            "Graph contains StaticSwitch / StaticBoolParameter nodes.",
            "Material is built as a reusable template with optional branches such as edge treatment, fade path, UV orientation, or alternate blending logic.",
            "Lets a single parent cover multiple looks while still compiling away unused branches in instances.",
        )

    if "MaterialExpressionPanner" in expression_classes or "pan" in joined_name:
        add(
            "uv_flow_animation",
            "Graph or asset naming suggests panning UVs.",
            "Used when the material needs directional motion for beams, energy trails, water flow, or aura movement.",
            "Adds perceived motion even on simple geometry or sprites.",
        )

    if "MaterialExpressionMaterialFunctionCall" in expression_classes and any("swizzle" in path.lower() for path in function_paths):
        add(
            "uv_axis_remap",
            "Graph calls the Swizzle function.",
            "The author wants to flip or remap UV orientation without rebuilding the texture content.",
            "Useful for sprite orientation fixes, mirrored assets, and fast reuse of the same texture in multiple layouts.",
        )

    if "distortion" in joined_name or "MaterialExpressionSceneTexture" in expression_classes or "Refraction" in json.dumps(material_info, ensure_ascii=False):
        add(
            "distortion_or_refraction_layer",
            "Asset naming or graph metadata points to distortion/refraction.",
            "This material is meant to bend the background rather than only draw emissive color.",
            "Good for heat haze, shockwaves, water energy, and air displacement around beams.",
        )

    if "flipbook" in joined_name or "subuv" in joined_name:
        add(
            "atlas_animation_material",
            "Asset naming references flipbook or SubUV usage.",
            "The material is intended to show animated frames from a texture atlas rather than a static mask.",
            "Supports richer impact bursts, electrical arcs, sparks, or hand-authored frame animation.",
        )

    if "ribbon" in joined_name:
        add(
            "ribbon_specific_rendering",
            "Asset naming references ribbons.",
            "This material is specialized for trail or beam topology where UVs stretch along a path.",
            "Helps preserve directional streaks and beam readability over ribbon geometry.",
        )

    if "decal" in joined_name:
        add(
            "surface_mark_projection",
            "Asset naming references decals.",
            "Material exists to project a mark onto surfaces after an impact or spell event.",
            "Separates hit-mark logic from airborne particle logic and improves scene storytelling.",
        )

    if "cyltwist" in joined_name or "tornado" in joined_name:
        add(
            "cylindrical_volume_flow",
            "Asset naming references cylindrical twist or tornado motion.",
            "This family is built to fake rotational volume motion around a cylinder or beam axis.",
            "Useful for tornadoes, vortex beams, and helical energy effects without heavy simulation cost.",
        )

    if "MaterialExpressionSmoothStep" in expression_classes:
        add(
            "soft_threshold_shaping",
            "Graph uses SmoothStep.",
            "Author wants a controllable softness band instead of a harsh clip when shaping alpha or emissive transitions.",
            "Makes circles, halos, and dissolves feel more polished and easier to tune.",
        )

    if not techniques:
        add(
            "generic_vfx_parent",
            "No standout specialized technique was detected from heuristics alone.",
            "Material likely acts as a reusable baseline for a family of VFX looks.",
            "Still valuable as a stable parent for instances and consistent art controls.",
        )

    return techniques


def infer_instance_intent(instance_info: dict[str, Any], usage_refs: list[dict[str, Any]]) -> dict[str, Any]:
    parent_path = str(instance_info.get("parent_path", ""))
    scalar_overrides = instance_info.get("overrides", {}).get("scalar", [])
    vector_overrides = instance_info.get("overrides", {}).get("vector", [])
    texture_overrides = instance_info.get("overrides", {}).get("texture", [])
    asset_name = str(instance_info.get("name", ""))
    name_lower = asset_name.lower()

    reasons = []
    if texture_overrides:
        reasons.append("Instance swaps texture content while reusing the parent graph.")
    if scalar_overrides or vector_overrides:
        reasons.append("Instance tunes brightness, tiling, fade, or shape controls without touching the parent material.")
    if usage_refs:
        reasons.append("Instance is referenced directly by Niagara systems, so it acts as a production-ready look preset.")
    if "water" in name_lower:
        reasons.append("Naming suggests the preset is tuned for water-like color and motion.")
    if "fire" in name_lower or "flame" in name_lower:
        reasons.append("Naming suggests the preset is tuned for fire energy.")
    if "light" in name_lower or "thunder" in name_lower or "electric" in name_lower:
        reasons.append("Naming suggests the preset is tuned for electrical/lightning readability.")

    artist_value = []
    if scalar_overrides:
        artist_value.append("Fast per-look tuning through scalar overrides.")
    if texture_overrides:
        artist_value.append("Shape language can change by swapping mask or atlas textures.")
    if vector_overrides:
        artist_value.append("Color family can change without forking the graph.")

    return {
        "parent_path": parent_path,
        "why_this_instance_exists": reasons or ["Instance exists to package a specific look preset from a reusable parent material."],
        "artist_control_value": artist_value or ["Instance gives artists a non-destructive tuning layer on top of the parent material."],
        "used_by_vfx_count": len(usage_refs),
        "used_by_vfx": usage_refs,
    }


def collect_material_assets(host: str, port: int, folder_path: str) -> dict[str, list[dict[str, Any]]]:
    return {
        "materials": bridge_call(
            host,
            port,
            "scan_assets",
            {"folder_path": folder_path, "asset_type": "Material", "max_results": 500},
        ).get("assets", []),
        "instances": bridge_call(
            host,
            port,
            "scan_assets",
            {"folder_path": f"{folder_path}/MI", "asset_type": "MaterialInstanceConstant", "max_results": 500},
        ).get("assets", []),
        "functions": bridge_call(
            host,
            port,
            "scan_assets",
            {"folder_path": f"{folder_path}/MF", "asset_type": "MaterialFunction", "max_results": 500},
        ).get("assets", []),
    }


def inspect_material_folder(host: str, port: int, folder_path: str, niagara_snapshot_path: Path) -> dict[str, Any]:
    assets = collect_material_assets(host, port, folder_path)
    niagara_usage = load_niagara_usage(niagara_snapshot_path)

    materials: list[dict[str, Any]] = []
    instances: list[dict[str, Any]] = []
    functions: list[dict[str, Any]] = []
    errors: list[dict[str, Any]] = []

    for asset in sorted(assets["materials"], key=lambda item: item["name"]):
        try:
            material_info = bridge_call(host, port, "get_material_info", {"material_path": asset["path"]})
            review = bridge_call(host, port, "review_material_asset", {"material_path": asset["path"]})
            material_entry = {
                "name": asset["name"],
                "path": asset["path"],
                "material_info": material_info,
                "review": review,
                "expression_class_counts": summarize_expression_classes(material_info.get("expressions", [])),
                "inferred_techniques": infer_material_techniques(asset["name"], material_info),
                "used_by_vfx": niagara_usage.get(asset["path"], []),
            }
            materials.append(material_entry)
        except Exception as exc:
            errors.append({"asset_type": "Material", "asset_path": asset["path"], "error": str(exc)})

    for asset in sorted(assets["instances"], key=lambda item: item["name"]):
        try:
            instance_info = bridge_call(
                host,
                port,
                "get_material_instance_info",
                {"material_instance_path": asset["path"]},
            )
            usage_refs = niagara_usage.get(asset["path"], [])
            instance_entry = {
                "name": asset["name"],
                "path": asset["path"],
                "instance_info": instance_info,
                "inferred_role": infer_instance_intent(instance_info, usage_refs),
            }
            instances.append(instance_entry)
        except Exception as exc:
            errors.append({"asset_type": "MaterialInstanceConstant", "asset_path": asset["path"], "error": str(exc)})

    for asset in sorted(assets["functions"], key=lambda item: item["name"]):
        try:
            function_info = bridge_call(
                host,
                port,
                "get_material_function_info",
                {"material_function_path": asset["path"]},
            )
            functions.append(
                {
                    "name": asset["name"],
                    "path": asset["path"],
                    "function_info": function_info,
                    "inferred_purpose": [
                        "Material function encapsulates reusable graph logic so parent materials can stay cleaner and share a consistent operation."
                    ],
                }
            )
        except Exception as exc:
            errors.append({"asset_type": "MaterialFunction", "asset_path": asset["path"], "error": str(exc)})

    return {
        "generated_at_utc": datetime.now(timezone.utc).isoformat(),
        "folder_path": folder_path,
        "material_count": len(materials),
        "instance_count": len(instances),
        "function_count": len(functions),
        "error_count": len(errors),
        "materials": materials,
        "instances": instances,
        "functions": functions,
        "errors": errors,
    }


def build_compact_index(knowledge: dict[str, Any]) -> dict[str, Any]:
    compact_materials = []
    for material in knowledge.get("materials", []):
        info = material.get("material_info", {})
        compact_materials.append(
            {
                "name": material.get("name"),
                "path": material.get("path"),
                "blend_mode": info.get("blend_mode"),
                "shading_model": info.get("shading_model"),
                "two_sided": info.get("two_sided"),
                "expression_count": len(info.get("expressions", [])),
                "expression_class_counts": material.get("expression_class_counts", {}),
                "parameter_summary": {
                    "scalar_parameter_count": material.get("review", {}).get("scalar_parameter_count"),
                    "vector_parameter_count": material.get("review", {}).get("vector_parameter_count"),
                    "texture_parameter_count": material.get("review", {}).get("texture_parameter_count"),
                    "function_call_count": material.get("review", {}).get("function_call_count"),
                },
                "review_summary": {
                    "maintainability_score": material.get("review", {}).get("maintainability_score"),
                    "finding_count": material.get("review", {}).get("finding_count"),
                    "findings": material.get("review", {}).get("findings", []),
                },
                "inferred_techniques": material.get("inferred_techniques", []),
                "used_by_vfx": material.get("used_by_vfx", []),
            }
        )

    compact_instances = []
    for instance in knowledge.get("instances", []):
        info = instance.get("instance_info", {})
        compact_instances.append(
            {
                "name": instance.get("name"),
                "path": instance.get("path"),
                "parent_path": info.get("parent_path"),
                "override_counts": info.get("override_counts", {}),
                "overrides": info.get("overrides", {}),
                "has_emissive_related_overrides": info.get("has_emissive_related_overrides"),
                "inferred_role": instance.get("inferred_role", {}),
            }
        )

    compact_functions = []
    for function in knowledge.get("functions", []):
        info = function.get("function_info", {})
        compact_functions.append(
            {
                "name": function.get("name"),
                "path": function.get("path"),
                "function_name": info.get("name", function.get("name")),
                "summary": info,
                "inferred_purpose": function.get("inferred_purpose", []),
            }
        )

    return {
        "generated_at_utc": knowledge.get("generated_at_utc"),
        "folder_path": knowledge.get("folder_path"),
        "material_count": knowledge.get("material_count"),
        "instance_count": knowledge.get("instance_count"),
        "function_count": knowledge.get("function_count"),
        "error_count": knowledge.get("error_count"),
        "materials": compact_materials,
        "instances": compact_instances,
        "functions": compact_functions,
        "errors": knowledge.get("errors", []),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Export material knowledge for a VFX material folder through the ForgePilot bridge.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--folder-path", default=DEFAULT_FOLDER)
    parser.add_argument("--output-path", default=str(DEFAULT_OUTPUT))
    parser.add_argument("--niagara-snapshot-path", default=str(DEFAULT_NIAGARA_SNAPSHOT))
    args = parser.parse_args()

    output_path = Path(args.output_path)
    if not output_path.is_absolute():
        output_path = Path.cwd() / output_path
    output_path.parent.mkdir(parents=True, exist_ok=True)

    niagara_snapshot_path = Path(args.niagara_snapshot_path)
    if not niagara_snapshot_path.is_absolute():
        niagara_snapshot_path = Path.cwd() / niagara_snapshot_path

    knowledge = inspect_material_folder(args.host, args.port, args.folder_path, niagara_snapshot_path)
    output_path.write_text(json.dumps(knowledge, ensure_ascii=False, indent=2), encoding="utf-8")

    compact_output_path = output_path.with_name(f"{output_path.stem}_index{output_path.suffix}")
    compact_output_path.write_text(
        json.dumps(build_compact_index(knowledge), ensure_ascii=False, indent=2),
        encoding="utf-8",
    )

    summary = {
        "success": True,
        "output_path": str(output_path),
        "compact_output_path": str(compact_output_path),
        "folder_path": args.folder_path,
        "material_count": knowledge["material_count"],
        "instance_count": knowledge["instance_count"],
        "function_count": knowledge["function_count"],
        "error_count": knowledge["error_count"],
    }
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
