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
DEFAULT_SOURCE_SYSTEM = "/Game/ForgePilot_Test/Niagara/ToolingSandbox/NS_ForgePilot_SmokeBurst_FromScratch.NS_ForgePilot_SmokeBurst_FromScratch"

EXPECTED_TOOLS = [
    "get_niagara_user_parameter_defaults",
    "get_niagara_emitter_stack",
    "get_niagara_mutation_snapshots",
    "restore_niagara_mutation_snapshot",
    "cleanup_niagara_audit_assets",
]


def bridge_call(host: str, port: int, payload: dict, timeout: float) -> dict:
    response = bridge_request(host, port, payload, timeout_seconds=timeout)
    if not response.get("ok"):
        raise RuntimeError(json.dumps(response, ensure_ascii=False))
    return response


def call_tool(host: str, port: int, tool_name: str, tool_input: dict, timeout: float) -> dict:
    response = bridge_call(
        host,
        port,
        {"action": "execute_tool", "tool_name": tool_name, "input": tool_input},
        timeout,
    )
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
        except Exception as exc:
            last_error = str(exc)
        time.sleep(0.5)
    raise RuntimeError(f"Timed out waiting for bridge at {host}:{port}: {last_error}")


def extract_tool_names(list_tools_response: dict) -> list[str]:
    tools = list_tools_response.get("tools", [])
    names: list[str] = []
    for item in tools:
        if isinstance(item, dict):
            name = str(item.get("name", "")).strip()
            if name:
                names.append(name)
    return names


def find_param(parameters: list[dict], name: str) -> dict | None:
    for item in parameters or []:
        if str(item.get("name", "")) == name:
            return item
    return None


def main() -> int:
    parser = argparse.ArgumentParser(description="Live-verify Niagara plan tools through the ForgePilot bridge.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--timeout", type=float, default=120.0)
    parser.add_argument("--source-system", default=DEFAULT_SOURCE_SYSTEM)
    parser.add_argument("--save-path", default="/Game/ForgePilot_Audit")
    parser.add_argument("--output-dir", default=os.path.join(os.path.expanduser("~"), "Downloads", "ForgePilotNiagaraProbes"))
    args = parser.parse_args()

    wait_for_bridge(args.host, args.port, args.timeout)

    stamp = time.strftime("%Y%m%d_%H%M%S")
    temp_name = f"NS_ForgePilotLiveVerify_{stamp}"
    asset_path = f"{args.save_path}/{temp_name}"
    asset_object_path = f"{asset_path}.{temp_name}"
    param_name = "User.ForgePilotLiveVerifyFloat"
    output_dir = Path(args.output_dir) / f"niagara_live_verify_{stamp}"
    output_dir.mkdir(parents=True, exist_ok=True)

    summary = {
        "success": False,
        "run_id": stamp,
        "source_system": args.source_system,
        "asset_path": asset_path,
        "asset_object_path": asset_object_path,
        "tool_presence": {},
        "steps": {},
        "checks": {},
        "cleanup": {},
    }

    try:
        list_tools_response = bridge_call(args.host, args.port, {"action": "list_tools"}, args.timeout)
        loaded_tool_names = extract_tool_names(list_tools_response)
        missing_tools = [name for name in EXPECTED_TOOLS if name not in loaded_tool_names]
        summary["tool_presence"] = {
            "loaded_tool_count": len(loaded_tool_names),
            "missing_tools": missing_tools,
            "expected_tools": EXPECTED_TOOLS,
        }
        if missing_tools:
            raise RuntimeError(f"Missing Niagara live-verify tools in bridge session: {', '.join(missing_tools)}")

        preferred_system = args.source_system
        try:
            context = call_tool(args.host, args.port, "get_niagara_editor_context", {}, args.timeout)
            summary["steps"]["editor_context"] = context
            preferred_system = context.get("preferred_system_path") or preferred_system
        except Exception as exc:
            summary["steps"]["editor_context_error"] = str(exc)

        summary["steps"]["defaults_source"] = call_tool(
            args.host,
            args.port,
            "get_niagara_user_parameter_defaults",
            {"system_path": preferred_system},
            args.timeout,
        )
        summary["steps"]["emitter_stack"] = call_tool(
            args.host,
            args.port,
            "get_niagara_emitter_stack",
            {"system_path": preferred_system, "emitter_index": "0"},
            args.timeout,
        )

        summary["steps"]["duplicate"] = call_tool(
            args.host,
            args.port,
            "duplicate_niagara_system",
            {
                "source_system_path": preferred_system,
                "new_system_name": temp_name,
                "save_path": args.save_path,
                "apply_changes": True,
            },
            args.timeout,
        )

        summary["steps"]["add_param"] = call_tool(
            args.host,
            args.port,
            "add_niagara_user_parameter",
            {
                "system_path": asset_path,
                "param_name": "ForgePilotLiveVerifyFloat",
                "param_type": "float",
                "apply_changes": True,
            },
            args.timeout,
        )

        summary["steps"]["defaults_after_add"] = call_tool(
            args.host,
            args.port,
            "get_niagara_user_parameter_defaults",
            {"system_path": asset_path},
            args.timeout,
        )

        summary["steps"]["snapshots"] = call_tool(
            args.host,
            args.port,
            "get_niagara_mutation_snapshots",
            {"system_path": asset_path},
            args.timeout,
        )

        summary["steps"]["restore"] = call_tool(
            args.host,
            args.port,
            "restore_niagara_mutation_snapshot",
            {
                "system_path": asset_path,
                "use_latest": True,
                "apply_changes": True,
            },
            args.timeout,
        )

        summary["steps"]["defaults_after_restore"] = call_tool(
            args.host,
            args.port,
            "get_niagara_user_parameter_defaults",
            {"system_path": asset_path},
            args.timeout,
        )

        defaults_after_add = summary["steps"]["defaults_after_add"].get("parameters", [])
        defaults_after_restore = summary["steps"]["defaults_after_restore"].get("parameters", [])
        summary["checks"] = {
            "stack_stage_count": summary["steps"]["emitter_stack"].get("stage_count"),
            "stack_finding_count": summary["steps"]["emitter_stack"].get("finding_count"),
            "snapshot_count": summary["steps"]["snapshots"].get("count"),
            "param_present_after_add": find_param(defaults_after_add, param_name) is not None,
            "param_present_after_restore": find_param(defaults_after_restore, param_name) is not None,
        }

        if summary["checks"]["snapshot_count"] < 1:
            raise RuntimeError("Expected at least one Niagara mutation snapshot after asset mutation.")
        if not summary["checks"]["param_present_after_add"]:
            raise RuntimeError("Live verify param was not present after add_niagara_user_parameter.")
        if summary["checks"]["param_present_after_restore"]:
            raise RuntimeError("Live verify param still existed after restore_niagara_mutation_snapshot.")

        summary["success"] = True
    except Exception as exc:
        summary["error"] = str(exc)
    finally:
        try:
            summary["cleanup"]["cleanup_audit_assets"] = call_tool(
                args.host,
                args.port,
                "cleanup_niagara_audit_assets",
                {
                    "folder_path": args.save_path,
                    "name_prefix": temp_name,
                    "apply_changes": True,
                },
                args.timeout,
            )
        except Exception as exc:
            summary["cleanup"]["cleanup_audit_assets_error"] = str(exc)

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
