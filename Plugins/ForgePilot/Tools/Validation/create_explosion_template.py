#!/usr/bin/env python3
import argparse
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
SYSTEM_NAME = "NS_Explosion_Template"
SAVE_PATH = "/Game/VFX/Templates"
SYSTEM_PATH = f"{SAVE_PATH}/{SYSTEM_NAME}"
EMITTER_NAME = "ExplosionTemplateEmitter"


def wait_for_bridge(host: str, port: int, timeout_seconds: float) -> None:
    deadline = time.time() + timeout_seconds
    last_error = "bridge did not respond"
    while time.time() < deadline:
        try:
            response = bridge_request(host, port, {"action": "ping"}, timeout_seconds=5.0)
            if response.get("ok"):
                return
            last_error = json.dumps(response, ensure_ascii=False)
        except Exception as exc:
            last_error = str(exc)
        time.sleep(0.5)
    raise RuntimeError(f"Timed out waiting for bridge at {host}:{port}: {last_error}")


def call_tool(host: str, port: int, tool_name: str, payload: dict, timeout: float) -> dict:
    response = bridge_request(
        host,
        port,
        {"action": "execute_tool", "tool_name": tool_name, "input": payload},
        timeout_seconds=timeout,
    )
    if not response.get("ok"):
        raise RuntimeError(json.dumps(response, ensure_ascii=False))
    result = response.get("result", {})
    if not result.get("success", False):
        raise RuntimeError(json.dumps(result, ensure_ascii=False))
    return result


def try_get_system_info(host: str, port: int, timeout: float) -> dict | None:
    try:
        return call_tool(
            host,
            port,
            "get_niagara_system_info",
            {"system_path": SYSTEM_PATH},
            timeout,
        )
    except Exception:
        return None


def find_module_input_keys(system_modules: list[dict], module_script_path: str, input_tokens: list[str]) -> tuple[str, str]:
    wanted_module = module_script_path.lower()
    wanted_tokens = [token.lower() for token in input_tokens]
    for module in system_modules:
        script_path = str(module.get("script_asset_path", "")).lower()
        if script_path != wanted_module:
            continue
        module_key = str(module.get("module_key", ""))
        for input_item in module.get("inputs", []):
            haystacks = [
                str(input_item.get("display_name", "")).lower(),
                str(input_item.get("input_key", "")).lower(),
                str(input_item.get("value_string", "")).lower(),
            ]
            if any(token in hay for token in wanted_tokens for hay in haystacks):
                return module_key, str(input_item.get("input_key", ""))
    raise RuntimeError(
        f"Could not resolve input for module '{module_script_path}' with tokens {input_tokens}."
    )


def main() -> int:
    parser = argparse.ArgumentParser(description="Create a one-shot explosion Niagara template with User.VFX bindings.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--timeout", type=float, default=120.0)
    parser.add_argument("--hero-spawn-rate", type=float, default=120.0)
    parser.add_argument("--hero-velocity", type=float, default=900.0)
    parser.add_argument("--sparks-gravity", default="[0.0,0.0,-980.0]")
    args = parser.parse_args()

    wait_for_bridge(args.host, args.port, args.timeout)

    existing_info = try_get_system_info(args.host, args.port, args.timeout)
    if existing_info is not None:
        raise RuntimeError(
            f"Asset already exists at '{SYSTEM_PATH}'. Remove or rename it before re-running this one-shot script."
        )

    summary: dict[str, object] = {
        "system_path": SYSTEM_PATH,
        "emitter_name": EMITTER_NAME,
        "steps": {},
    }

    summary["steps"]["create_system"] = call_tool(
        args.host,
        args.port,
        "create_empty_niagara_system",
        {
            "system_name": SYSTEM_NAME,
            "save_path": SAVE_PATH,
            "apply_changes": True,
        },
        args.timeout,
    )

    summary["steps"]["add_emitter"] = call_tool(
        args.host,
        args.port,
        "add_empty_niagara_emitter",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
            "add_default_modules_and_renderer": True,
            "apply_changes": True,
        },
        args.timeout,
    )

    for param_name, param_type, value in [
        ("User.VFX.HeroSpawnRate", "float", str(args.hero_spawn_rate)),
        ("User.VFX.HeroVelocity", "float", str(args.hero_velocity)),
        ("User.VFX.SparksGravity", "Vector", args.sparks_gravity),
    ]:
        key = param_name.split(".")[-1]
        summary["steps"][f"add_param_{key}"] = call_tool(
            args.host,
            args.port,
            "add_niagara_user_parameter",
            {
                "system_path": SYSTEM_PATH,
                "param_name": param_name,
                "param_type": param_type,
                "apply_changes": True,
            },
            args.timeout,
        )
        summary["steps"][f"set_default_{key}"] = call_tool(
            args.host,
            args.port,
            "set_niagara_user_parameter_default",
            {
                "system_path": SYSTEM_PATH,
                "param_name": param_name,
                "param_type": param_type,
                "value": value,
                "apply_changes": True,
            },
            args.timeout,
        )

    summary["steps"]["ensure_spawn_rate"] = call_tool(
        args.host,
        args.port,
        "set_niagara_spawn_rate",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
            "value": args.hero_spawn_rate,
            "apply_changes": True,
        },
        args.timeout,
    )

    summary["steps"]["ensure_cone_velocity"] = call_tool(
        args.host,
        args.port,
        "set_niagara_cone_velocity",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
            "direction": [0.0, 0.0, 1.0],
            "angle": 180.0,
            "speed": args.hero_velocity,
            "apply_changes": True,
        },
        args.timeout,
    )

    summary["steps"]["ensure_gravity_force"] = call_tool(
        args.host,
        args.port,
        "set_niagara_gravity_force",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
            "value": [0.0, 0.0, -980.0],
            "apply_changes": True,
        },
        args.timeout,
    )

    module_inputs = call_tool(
        args.host,
        args.port,
        "get_niagara_module_inputs",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
        },
        args.timeout,
    )
    summary["steps"]["module_inputs_before_link"] = module_inputs
    modules = module_inputs.get("modules", [])

    spawn_rate_module_key, spawn_rate_input_key = find_module_input_keys(
        modules,
        "/Niagara/Modules/Emitter/SpawnRate.SpawnRate",
        ["spawnrate", "spawn rate"],
    )
    cone_speed_module_key, cone_speed_input_key = find_module_input_keys(
        modules,
        "/Niagara/Modules/Spawn/Velocity/AddVelocityInCone.AddVelocityInCone",
        ["velocity strength", "speed", "velocity"],
    )
    gravity_module_key, gravity_input_key = find_module_input_keys(
        modules,
        "/Niagara/Modules/Update/Forces/GravityForce.GravityForce",
        ["gravity force", "acceleration", "gravity"],
    )

    for label, module_key, input_key, linked_param_name in [
        ("spawn_rate", spawn_rate_module_key, spawn_rate_input_key, "User.VFX.HeroSpawnRate"),
        ("cone_speed", cone_speed_module_key, cone_speed_input_key, "User.VFX.HeroVelocity"),
        ("gravity", gravity_module_key, gravity_input_key, "User.VFX.SparksGravity"),
    ]:
        summary["steps"][f"link_{label}"] = call_tool(
            args.host,
            args.port,
            "set_niagara_module_input_linked",
            {
                "system_path": SYSTEM_PATH,
                "emitter_name": EMITTER_NAME,
                "module_key": module_key,
                "input_key": input_key,
                "linked_param_name": linked_param_name,
                "apply_changes": True,
            },
            args.timeout,
        )

    summary["steps"]["compile"] = call_tool(
        args.host,
        args.port,
        "compile_niagara_system",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
            "apply_changes": True,
        },
        args.timeout,
    )

    summary["steps"]["system_info"] = call_tool(
        args.host,
        args.port,
        "get_niagara_system_info",
        {
            "system_path": SYSTEM_PATH,
        },
        args.timeout,
    )

    summary["steps"]["module_inputs_after_link"] = call_tool(
        args.host,
        args.port,
        "get_niagara_module_inputs",
        {
            "system_path": SYSTEM_PATH,
            "emitter_name": EMITTER_NAME,
        },
        args.timeout,
    )

    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
