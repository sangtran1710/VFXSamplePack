#!/usr/bin/env python3
import argparse
import json
import os
import socket
import sys
from hashlib import md5
from pathlib import Path
from typing import Any, Dict


def get_default_bridge_host() -> str:
    return os.environ.get("FORGEPILOT_BRIDGE_HOST", "127.0.0.1")


def _find_project_root(start: Path) -> Path:
    cursor = start.resolve()
    for candidate in [cursor, *cursor.parents]:
        if any(candidate.glob("*.uproject")):
            return candidate
    return Path.cwd().resolve()


def get_default_project_dir() -> str:
    return _find_project_root(Path(__file__).resolve().parent).as_posix().rstrip("/")


def get_default_project_name() -> str:
    project_root = Path(get_default_project_dir())
    for uproject in project_root.glob("*.uproject"):
        return uproject.stem
    return project_root.name


def _workspace_hash_hex() -> str:
    normalized_project_dir = get_default_project_dir().lower()
    return md5(normalized_project_dir.encode("utf-8")).hexdigest()


def get_default_workspace_id() -> str:
    return f"fpw_{_workspace_hash_hex()[:12]}"


def get_default_bridge_port() -> int:
    raw_env = os.environ.get("FORGEPILOT_BRIDGE_PORT", "").strip()
    if raw_env:
        try:
            parsed = int(raw_env)
        except ValueError:
            parsed = 0
        if 0 < parsed <= 65535:
            return parsed

    return 19080 + (int(_workspace_hash_hex()[:8], 16) % 1000)


def add_expected_bridge_identity(payload: Dict[str, Any]) -> Dict[str, Any]:
    result = dict(payload)
    result.setdefault("expected_workspace_id", get_default_workspace_id())
    result.setdefault("expected_project_dir", get_default_project_dir())
    result.setdefault("expected_project_name", get_default_project_name())
    return result


DEFAULT_HOST = get_default_bridge_host()
DEFAULT_PORT = get_default_bridge_port()


def bridge_request(host: str, port: int, payload: Dict[str, Any], timeout_seconds: float = 30.0) -> Dict[str, Any]:
    with socket.create_connection((host, port), timeout=timeout_seconds) as sock:
        sock.settimeout(timeout_seconds)
        stream = sock.makefile("rwb")
        stream.write((json.dumps(add_expected_bridge_identity(payload), ensure_ascii=False) + "\n").encode("utf-8"))
        stream.flush()
        line = stream.readline()
        if not line:
            raise RuntimeError("Bridge closed the connection.")
        result = json.loads(line.decode("utf-8", errors="replace"))
        if not isinstance(result, dict):
            raise RuntimeError("Bridge response is not a JSON object.")
        return result


def main() -> int:
    parser = argparse.ArgumentParser(description="Call the ForgePilot editor bridge.")
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument("--ping", action="store_true", help="Ping the editor bridge.")
    parser.add_argument("--list-tools", action="store_true", help="List available tool definitions.")
    parser.add_argument("--tool", help="Execute a named tool.")
    parser.add_argument("--input", default="{}", help="Tool input JSON object.")
    parser.add_argument("--timeout", type=float, default=30.0, help="Socket timeout in seconds. Increase this for long-running probe tools.")
    parser.add_argument("--pretty", action="store_true", help="Pretty-print JSON output.")
    args = parser.parse_args()

    actions = [args.ping, args.list_tools, bool(args.tool)]
    if sum(1 for item in actions if item) != 1:
        parser.error("Choose exactly one of --ping, --list-tools, or --tool.")

    if args.ping:
        payload = {"action": "ping"}
    elif args.list_tools:
        payload = {"action": "list_tools"}
    else:
        try:
            tool_input = json.loads(args.input)
        except Exception as exc:
            raise SystemExit(f"Invalid JSON for --input: {exc}")
        if not isinstance(tool_input, dict):
            raise SystemExit("--input must decode to a JSON object.")
        payload = {"action": "execute_tool", "tool_name": args.tool, "input": tool_input}

    try:
        response = bridge_request(args.host, args.port, payload, timeout_seconds=args.timeout)
    except Exception as exc:
        error = {"ok": False, "error": str(exc)}
        print(json.dumps(error, ensure_ascii=False, indent=2 if args.pretty else None))
        return 1

    print(json.dumps(response, ensure_ascii=False, indent=2 if args.pretty else None))
    return 0 if response.get("ok") else 1


if __name__ == "__main__":
    raise SystemExit(main())
