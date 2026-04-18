#!/usr/bin/env python3
import argparse
import base64
import json
import os
import sys
import time
from pathlib import Path


SCRIPT_ROOT = Path(__file__).resolve().parent
if str(SCRIPT_ROOT) not in sys.path:
    sys.path.insert(0, str(SCRIPT_ROOT))

from invoke_bridge import bridge_request, get_default_bridge_port  # noqa: E402


DEFAULT_HOST = os.environ.get("FORGEPILOT_BRIDGE_HOST", "127.0.0.1")
DEFAULT_PORT = get_default_bridge_port()
DEFAULT_SOURCE_SYSTEM = "/Game/ForgePilot_Test/Niagara/ToolingSandbox/NS_ForgePilot_SmokeBurst_FromScratch.NS_ForgePilot_SmokeBurst_FromScratch"
DEFAULT_MATERIAL = "/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial"
DEFAULT_TEXTURE = "/Engine/EngineResources/DefaultTexture.DefaultTexture"


def call_tool(host: str, port: int, tool_name: str, payload: dict, timeout: float) -> dict:
    request = {"action": "execute_tool", "tool_name": tool_name, "input": payload}
    response = bridge_request(host, port, request, timeout_seconds=timeout)
    if not response.get("ok"):
        raise RuntimeError(json.dumps(response, ensure_ascii=False))
    return response["result"]


def wait_for_bridge(host: str, port: int, timeout_seconds: float) -> None:
    deadline = time.time() + timeout_seconds
    last_error = "bridge did not respond"
    while time.time() < deadline:
        try:
            response = bridge_request(host, port, {"action": "ping"}, timeout_seconds=5.0)
            if response.get("ok"):
                return
            last_error = json.dumps(response, ensure_ascii=False)
        except Exception as exc:  # pragma: no cover - best effort helper
            last_error = str(exc)
        time.sleep(0.5)
    raise RuntimeError(f"Timed out waiting for bridge at {host}:{port}: {last_error}")


def filter_probe_params(payload: dict) -> list[dict]:
    params = payload.get("parameters", []) if isinstance(payload, dict) else []
    return [param for param in params if str(param.get("name", "")).startswith("User.ForgePilot")]


def save_capture_from_result(result: dict, output_path: Path) -> str | None:
    if not isinstance(result, dict):
        return None
    payload = result.get("image_base64")
    if not payload:
        return None
    output_path.write_bytes(base64.b64decode(payload))
    return str(output_path)


def main() -> int:
    parser = argparse.ArgumentParser(description="Run a Niagara capability probe through the ForgePilot bridge.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--timeout", type=float, default=120.0)
    parser.add_argument("--source-system", default=DEFAULT_SOURCE_SYSTEM)
    parser.add_argument("--save-path", default="/Game/ForgePilot_Audit")
    parser.add_argument("--material-path", default=DEFAULT_MATERIAL)
    parser.add_argument("--texture-path", default=DEFAULT_TEXTURE)
    parser.add_argument("--output-dir", default=os.path.join(os.path.expanduser("~"), "Downloads", "ForgePilotNiagaraProbes"))
    parser.add_argument("--mode", choices=["full-chain", "asset-only", "renderer-only"], default="full-chain")
    parser.add_argument("--capture-screenshots", action="store_true")
    parser.add_argument("--keep-asset", action="store_true")
    parser.add_argument("--keep-actor", action="store_true")
    args = parser.parse_args()

    wait_for_bridge(args.host, args.port, timeout_seconds=args.timeout)

    stamp = time.strftime("%Y%m%d_%H%M%S")
    system_name = f"NS_FireHand_RuntimeUserParamProbe_{stamp}"
    actor_label = f"ForgePilotRuntimeProbe_{stamp}"
    asset_path = f"{args.save_path}/{system_name}"
    asset_object_path = f"{asset_path}.{system_name}"
    output_dir = Path(args.output_dir) / f"niagara_capability_probe_{stamp}"
    output_dir.mkdir(parents=True, exist_ok=True)

    summary = {
        "success": False,
        "run_id": stamp,
        "mode": args.mode,
        "source_system": args.source_system,
        "asset_path": asset_path,
        "asset_object_path": asset_object_path,
        "actor_label": actor_label,
        "steps": {},
        "captures": [],
        "cleanup": {},
    }

    param_specs = [
        {"name": "ForgePilotFloatParam", "type": "float", "default": "1.25", "runtime": "2.5"},
        {"name": "ForgePilotIntParam", "type": "int", "default": "7", "runtime": "11"},
        {"name": "ForgePilotBoolParam", "type": "bool", "default": "true", "runtime": "false"},
        {"name": "ForgePilotVectorParam", "type": "Vector", "default": "[1.0,2.0,3.0]", "runtime": "[10.0,20.0,30.0]"},
        {"name": "ForgePilotColorParam", "type": "LinearColor", "default": "[0.1,0.2,0.3,0.9]", "runtime": "[0.9,0.4,0.1,1.0]"},
        {"name": "ForgePilotMaterialParam", "type": "MaterialInterface", "default": args.material_path, "runtime": args.material_path},
        {"name": "ForgePilotTextureParam", "type": "Texture", "default": args.texture_path, "runtime": args.texture_path},
        {"name": "ForgePilotObjectParam", "type": "UObject", "default": args.material_path, "runtime": args.material_path},
    ]

    try:
        summary["steps"]["duplicate"] = call_tool(
            args.host,
            args.port,
            "duplicate_niagara_system",
            {
                "source_system_path": args.source_system,
                "new_system_name": system_name,
                "save_path": args.save_path,
                "apply_changes": True,
            },
            args.timeout,
        )

        for spec in param_specs:
            summary["steps"][f"add_{spec['name']}"] = call_tool(
                args.host,
                args.port,
                "add_niagara_user_parameter",
                {
                    "system_path": asset_path,
                    "param_name": spec["name"],
                    "param_type": spec["type"],
                    "apply_changes": True,
                },
                args.timeout,
            )

        summary["steps"]["defaults_before"] = call_tool(
            args.host,
            args.port,
            "get_niagara_user_parameter_defaults",
            {
                "system_path": asset_path,
            },
            args.timeout,
        )

        for spec in param_specs:
            summary["steps"][f"set_default_{spec['name']}"] = call_tool(
                args.host,
                args.port,
                "set_niagara_user_parameter_default",
                {
                    "system_path": asset_path,
                    "param_name": spec["name"],
                    "param_type": spec["type"],
                    "value": spec["default"],
                    "apply_changes": True,
                },
                args.timeout,
            )

        summary["steps"]["defaults_after_set_default"] = call_tool(
            args.host,
            args.port,
            "get_niagara_user_parameter_defaults",
            {
                "system_path": asset_path,
            },
            args.timeout,
        )

        if args.mode in ("full-chain", "renderer-only"):
            summary["steps"]["renderer_details_before"] = call_tool(
                args.host,
                args.port,
                "get_niagara_renderer_details",
                {
                    "system_path": asset_path,
                    "emitter_index": "0",
                    "renderer_index": "0",
                },
                args.timeout,
            )
            summary["steps"]["renderer_review_before"] = call_tool(
                args.host,
                args.port,
                "review_niagara_renderer",
                {
                    "system_path": asset_path,
                    "emitter_index": "0",
                    "renderer_index": "0",
                },
                args.timeout,
            )

        if args.mode == "renderer-only":
            summary["steps"]["renderer_material_user_param"] = call_tool(
                args.host,
                args.port,
                "set_niagara_renderer_material_user_param",
                {
                    "system_path": asset_path,
                    "emitter_index": "0",
                    "renderer_index": "0",
                    "user_param_name": "User.ForgePilotMaterialParam",
                    "apply_changes": True,
                },
                args.timeout,
            )
            summary["steps"]["renderer_details_after"] = call_tool(
                args.host,
                args.port,
                "get_niagara_renderer_details",
                {
                    "system_path": asset_path,
                    "emitter_index": "0",
                    "renderer_index": "0",
                },
                args.timeout,
            )
            summary["steps"]["renderer_review_after"] = call_tool(
                args.host,
                args.port,
                "review_niagara_renderer",
                {
                    "system_path": asset_path,
                    "emitter_index": "0",
                    "renderer_index": "0",
                },
                args.timeout,
            )

        if args.mode == "full-chain":
            summary["steps"]["spawn"] = call_tool(
                args.host,
                args.port,
                "spawn_niagara_at_location",
                {
                    "system_path": asset_path,
                    "location": [0, 0, 200],
                    "rotation": [0, 0, 0],
                    "actor_label": actor_label,
                    "auto_activate": "true",
                    "apply_changes": True,
                },
                args.timeout,
            )

            if args.capture_screenshots:
                summary["steps"]["capture_after_spawn"] = call_tool(
                    args.host,
                    args.port,
                    "capture_viewport_screenshot",
                    {},
                    args.timeout,
                )
                saved_path = save_capture_from_result(
                    summary["steps"]["capture_after_spawn"],
                    output_dir / "after_spawn.jpg",
                )
                if saved_path:
                    summary["captures"].append({"label": "after_spawn", "path": saved_path})

            summary["steps"]["runtime_before_override"] = call_tool(
                args.host,
                args.port,
                "get_niagara_actor_user_parameter_values",
                {
                    "actor_label": actor_label,
                },
                args.timeout,
            )

            for spec in param_specs:
                summary["steps"][f"set_runtime_{spec['name']}"] = call_tool(
                    args.host,
                    args.port,
                    "set_niagara_user_parameter",
                    {
                        "actor_label": actor_label,
                        "param_name": f"User.{spec['name']}",
                        "param_type": spec["type"],
                        "value": spec["runtime"],
                        "apply_changes": True,
                    },
                    args.timeout,
                )

            summary["steps"]["runtime_after_override"] = call_tool(
                args.host,
                args.port,
                "get_niagara_actor_user_parameter_values",
                {
                    "actor_label": actor_label,
                },
                args.timeout,
            )

            if args.capture_screenshots:
                summary["steps"]["capture_after_runtime_override"] = call_tool(
                    args.host,
                    args.port,
                    "capture_viewport_screenshot",
                    {},
                    args.timeout,
                )
                saved_path = save_capture_from_result(
                    summary["steps"]["capture_after_runtime_override"],
                    output_dir / "after_runtime_override.jpg",
                )
                if saved_path:
                    summary["captures"].append({"label": "after_runtime_override", "path": saved_path})

        summary["steps"]["audit"] = call_tool(
            args.host,
            args.port,
            "get_mutation_audit_entries",
            {
                "asset_path": asset_object_path,
            },
            args.timeout,
        )

        summary["analysis"] = {
            "defaults_before": filter_probe_params(summary["steps"].get("defaults_before", {})),
            "defaults_after_set_default": filter_probe_params(summary["steps"].get("defaults_after_set_default", {})),
            "runtime_before_override": filter_probe_params(summary["steps"].get("runtime_before_override", {})),
            "runtime_after_override": filter_probe_params(summary["steps"].get("runtime_after_override", {})),
            "renderer_details_before": summary["steps"].get("renderer_details_before", {}),
            "renderer_details_after": summary["steps"].get("renderer_details_after", {}),
            "renderer_review_before": summary["steps"].get("renderer_review_before", {}),
            "renderer_review_after": summary["steps"].get("renderer_review_after", {}),
        }

        summary["success"] = True
    except Exception as exc:
        summary["error"] = str(exc)
    finally:
        if not args.keep_actor and args.mode == "full-chain":
            try:
                summary["cleanup"]["destroy_actor"] = call_tool(
                    args.host,
                    args.port,
                    "destroy_actor_by_label",
                    {"actor_label": actor_label, "apply_changes": True},
                    args.timeout,
                )
            except Exception as exc:
                summary["cleanup"]["destroy_actor_error"] = str(exc)

        if not args.keep_asset:
            try:
                summary["cleanup"]["delete_asset"] = call_tool(
                    args.host,
                    args.port,
                    "delete_asset",
                    {"asset_path": asset_object_path, "apply_changes": True},
                    args.timeout,
                )
            except Exception as exc:
                summary["cleanup"]["delete_asset_error"] = str(exc)

    summary_path = output_dir / "summary.json"
    summary_path.write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding="utf-8")

    print(json.dumps({"success": summary["success"], "summary_path": str(summary_path), "run_id": stamp}, ensure_ascii=False, indent=2))
    return 0 if summary["success"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
