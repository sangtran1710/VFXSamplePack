#!/usr/bin/env python3
import argparse
import json
import os
import socket
import sys
from collections import defaultdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

SCRIPT_ROOT = Path(__file__).resolve().parent
if str(SCRIPT_ROOT) not in sys.path:
    sys.path.insert(0, str(SCRIPT_ROOT))

from invoke_bridge import add_expected_bridge_identity, get_default_bridge_port  # noqa: E402


DEFAULT_HOST = os.environ.get("FORGEPILOT_BRIDGE_HOST", "127.0.0.1")
DEFAULT_PORT = get_default_bridge_port()
DEFAULT_FOLDER = "/Game/VFX/EnergyBeam/NS"
DEFAULT_OUTPUT = Path("Output/KnowledgeSnapshots/energybeam_ns_knowledge.json")


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


def looks_like_material_instance(path: str) -> bool:
    name = path.rsplit(".", 1)[-1]
    return name.startswith("MI_") or "/MI/" in path


def parse_value_string(value: Any) -> Any:
    if not isinstance(value, str):
        return value
    trimmed = value.strip()
    if not trimmed:
        return value
    if trimmed.startswith("{") or trimmed.startswith("["):
        try:
            return json.loads(trimmed)
        except Exception:
            return value
    return value


def collect_semantic_inputs(module_inputs: list[dict[str, Any]]) -> dict[str, list[dict[str, Any]]]:
    semantic = defaultdict(list)
    for module in module_inputs:
        module_name = str(module.get("display_name", ""))
        script_path = str(module.get("script_asset_path", ""))
        stage_usage = str(module.get("stage_usage", ""))
        module_key = str(module.get("module_key", ""))

        for input_item in module.get("inputs", []):
            display_name = str(input_item.get("display_name", ""))
            input_key = str(input_item.get("input_key", ""))
            value_source = str(input_item.get("value_source", ""))
            value_string = input_item.get("value_string", "")

            record = {
                "module_display_name": module_name,
                "module_script_path": script_path,
                "module_key": module_key,
                "stage_usage": stage_usage,
                "input_display_name": display_name,
                "input_key": input_key,
                "type_name": input_item.get("type_name"),
                "value_source": value_source,
                "value_string": value_string,
                "parsed_value": parse_value_string(value_string),
            }

            tokens = {
                normalize_token(module_name),
                normalize_token(display_name),
                normalize_token(input_key),
            }
            joined = " ".join(filter(None, [module_name, display_name, input_key])).lower()

            if "spawnrate" in tokens or "spawn rate" in joined:
                semantic["spawn_rate"].append(record)
            if "lifetime" in joined:
                semantic["lifetime"].append(record)
            if any(term in joined for term in ("color", "alpha", "opacity", "intensity", "emissive", "brightness", "glow", "tint")):
                semantic["color_intensity"].append(record)
            if any(term in joined for term in ("velocity", "speed", "cone axis")):
                semantic["velocity"].append(record)
            if any(term in joined for term in ("gravity", "acceleration", "drag", "curl")):
                semantic["forces"].append(record)
            if value_source == "linked":
                semantic["linked_inputs"].append(record)
            if value_source == "dynamic":
                semantic["dynamic_inputs"].append(record)
            if value_source == "expression":
                semantic["curve_expression_inputs"].append(record)

    return dict(semantic)


def inspect_material_instances(
    host: str,
    port: int,
    used_materials: list[str],
    material_cache: dict[str, Any],
) -> list[dict[str, Any]]:
    details: list[dict[str, Any]] = []
    for material_path in used_materials:
        if material_path not in material_cache:
            if looks_like_material_instance(material_path):
                try:
                    material_cache[material_path] = bridge_call(
                        host,
                        port,
                        "get_material_instance_info",
                        {"material_instance_path": material_path},
                    )
                except Exception as exc:
                    material_cache[material_path] = {
                        "success": False,
                        "material_path": material_path,
                        "error": str(exc),
                    }
            else:
                material_cache[material_path] = {
                    "success": True,
                    "material_path": material_path,
                    "inspection_skipped": "not_a_material_instance",
                }
        details.append(material_cache[material_path])
    return details


def inspect_folder(host: str, port: int, folder_path: str) -> dict[str, Any]:
    scan = bridge_call(
        host,
        port,
        "scan_assets",
        {
            "folder_path": folder_path,
            "asset_type": "NiagaraSystem",
            "max_results": 500,
        },
    )

    assets = sorted(scan.get("assets", []), key=lambda asset: asset.get("name", ""))
    material_cache: dict[str, Any] = {}
    systems: list[dict[str, Any]] = []
    errors: list[dict[str, Any]] = []

    for asset in assets:
        system_path = asset["path"]
        system_name = asset["name"]
        try:
            system_info = bridge_call(host, port, "get_niagara_system_info", {"system_path": system_path})
            user_defaults = bridge_call(host, port, "get_niagara_user_parameter_defaults", {"system_path": system_path})

            system_entry: dict[str, Any] = {
                "name": system_name,
                "path": system_path,
                "user_parameters": system_info.get("user_parameters", []),
                "user_parameter_defaults": user_defaults.get("parameters", user_defaults),
                "emitters": [],
            }

            for emitter in system_info.get("emitters", []):
                emitter_index = emitter["index"]
                emitter_details = bridge_call(
                    host,
                    port,
                    "get_niagara_emitter_details",
                    {"system_path": system_path, "emitter_index": str(emitter_index)},
                )
                module_inputs = bridge_call(
                    host,
                    port,
                    "get_niagara_module_inputs",
                    {"system_path": system_path, "emitter_index": str(emitter_index)},
                )

                renderer_entries: list[dict[str, Any]] = []
                for renderer in emitter.get("renderers", []):
                    renderer_index = renderer["index"]
                    renderer_details = bridge_call(
                        host,
                        port,
                        "get_niagara_renderer_details",
                        {
                            "system_path": system_path,
                            "emitter_index": str(emitter_index),
                            "renderer_index": str(renderer_index),
                        },
                    )
                    used_materials = renderer_details.get("used_materials", [])
                    renderer_details["material_instance_details"] = inspect_material_instances(
                        host,
                        port,
                        used_materials,
                        material_cache,
                    )
                    renderer_entries.append(renderer_details)

                emitter_entry = {
                    "emitter_index": emitter_index,
                    "emitter_name": emitter.get("name"),
                    "summary": emitter_details,
                    "renderers": renderer_entries,
                    "module_inputs": module_inputs.get("modules", []),
                    "semantic_controls": collect_semantic_inputs(module_inputs.get("modules", [])),
                    "standard_module_presence": module_inputs.get("standard_module_presence", {}),
                }
                system_entry["emitters"].append(emitter_entry)

            systems.append(system_entry)
        except Exception as exc:
            errors.append(
                {
                    "system_name": system_name,
                    "system_path": system_path,
                    "error": str(exc),
                }
            )

    return {
        "generated_at_utc": datetime.now(timezone.utc).isoformat(),
        "folder_path": folder_path,
        "system_count": len(systems),
        "error_count": len(errors),
        "systems": systems,
        "errors": errors,
    }


def build_compact_index(knowledge: dict[str, Any]) -> dict[str, Any]:
    systems_index: list[dict[str, Any]] = []
    for system in knowledge.get("systems", []):
        system_entry = {
            "name": system.get("name"),
            "path": system.get("path"),
            "user_parameters": system.get("user_parameters", []),
            "user_parameter_defaults": system.get("user_parameter_defaults", []),
            "emitters": [],
        }
        for emitter in system.get("emitters", []):
            renderer_summary = []
            for renderer in emitter.get("renderers", []):
                renderer_summary.append(
                    {
                        "renderer_index": renderer.get("renderer_index"),
                        "renderer_class": renderer.get("renderer_class"),
                        "renderer_name": renderer.get("renderer_name"),
                        "used_materials": renderer.get("used_materials", []),
                        "material_instance_details": renderer.get("material_instance_details", []),
                    }
                )

            system_entry["emitters"].append(
                {
                    "emitter_index": emitter.get("emitter_index"),
                    "emitter_name": emitter.get("emitter_name"),
                    "summary": {
                        "sim_target": emitter.get("summary", {}).get("sim_target"),
                        "renderer_count": emitter.get("summary", {}).get("renderer_count"),
                        "used_materials": emitter.get("summary", {}).get("used_materials", []),
                    },
                    "standard_module_presence": emitter.get("standard_module_presence", {}),
                    "semantic_controls": emitter.get("semantic_controls", {}),
                    "renderers": renderer_summary,
                }
            )
        systems_index.append(system_entry)

    return {
        "generated_at_utc": knowledge.get("generated_at_utc"),
        "folder_path": knowledge.get("folder_path"),
        "system_count": knowledge.get("system_count"),
        "error_count": knowledge.get("error_count"),
        "systems": systems_index,
        "errors": knowledge.get("errors", []),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Export a Niagara folder knowledge snapshot through the ForgePilot bridge.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--folder-path", default=DEFAULT_FOLDER)
    parser.add_argument("--output-path", default=str(DEFAULT_OUTPUT))
    args = parser.parse_args()

    output_path = Path(args.output_path)
    if not output_path.is_absolute():
        output_path = Path.cwd() / output_path
    output_path.parent.mkdir(parents=True, exist_ok=True)

    knowledge = inspect_folder(args.host, args.port, args.folder_path)
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
        "system_count": knowledge["system_count"],
        "error_count": knowledge["error_count"],
        "system_names": [system["name"] for system in knowledge["systems"]],
    }
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
