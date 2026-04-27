#!/usr/bin/env python3
import json
import os
import socket
import sys
from pathlib import Path
from typing import Any, Dict


_VALIDATION_DIR = Path(__file__).resolve().parents[1] / "Validation"
if str(_VALIDATION_DIR) not in sys.path:
    sys.path.insert(0, str(_VALIDATION_DIR))

from invoke_bridge import add_expected_bridge_identity, get_default_bridge_host, get_default_bridge_port


BRIDGE_HOST = get_default_bridge_host()
BRIDGE_PORT = get_default_bridge_port()


class BridgeClient:
    def __init__(self, host: str, port: int) -> None:
        self.host = host
        self.port = port
        self.sock = None
        self.stream = None

    def connect(self) -> None:
        if self.sock is not None:
            return
        self.sock = socket.create_connection((self.host, self.port), timeout=5.0)
        self.stream = self.sock.makefile("rwb")

    def close(self) -> None:
        try:
            if self.stream is not None:
                self.stream.close()
        except Exception:
            pass
        try:
            if self.sock is not None:
                self.sock.close()
        except Exception:
            pass
        self.stream = None
        self.sock = None

    def request(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        self.connect()
        assert self.stream is not None
        self.stream.write((json.dumps(add_expected_bridge_identity(payload), ensure_ascii=False) + "\n").encode("utf-8"))
        self.stream.flush()
        line = self.stream.readline()
        if not line:
            raise RuntimeError("Bridge closed the connection.")
        try:
            parsed = json.loads(line.decode("utf-8", errors="replace"))
        except Exception as exc:
            raise RuntimeError(f"Bridge returned invalid JSON: {exc}")
        if not isinstance(parsed, dict):
            raise RuntimeError("Bridge response is not an object.")
        return parsed


def _jsonrpc_result(rid: Any, result: Any) -> Dict[str, Any]:
    return {"jsonrpc": "2.0", "id": rid, "result": result}


def _jsonrpc_error(rid: Any, code: int, message: str, data: Any = None) -> Dict[str, Any]:
    err = {"code": code, "message": message}
    if data is not None:
        err["data"] = data
    return {"jsonrpc": "2.0", "id": rid, "error": err}


def _read_stdio_message() -> Dict[str, Any] | None:
    """
    Claude Desktop speaks stdio MCP using Content-Length framed JSON-RPC messages.
    Keep a fallback for raw newline JSON to remain compatible with older local tests.
    """
    while True:
        first = sys.stdin.buffer.readline()
        if not first:
            return None
        if first in (b"\n", b"\r\n"):
            continue
        break

    if first.lower().startswith(b"content-length:"):
        try:
            content_length = int(first.split(b":", 1)[1].strip())
        except Exception as exc:
            raise RuntimeError(f"Invalid Content-Length header: {exc}")

        while True:
            header_line = sys.stdin.buffer.readline()
            if not header_line:
                raise RuntimeError("Unexpected EOF while reading MCP headers.")
            if header_line in (b"\n", b"\r\n"):
                break

        payload = sys.stdin.buffer.read(content_length)
        if len(payload) != content_length:
            raise RuntimeError("Unexpected EOF while reading MCP payload.")
        return json.loads(payload.decode("utf-8", errors="replace"))

    line = first.decode("utf-8", errors="replace").strip()
    if not line:
        return None
    return json.loads(line)


def _write_stdio_message(payload: Dict[str, Any]) -> None:
    sys.stdout.write(json.dumps(payload, ensure_ascii=False) + "\n")
    sys.stdout.flush()


def _mcp_tools_list(bridge: BridgeClient) -> Dict[str, Any]:
    response = bridge.request({"action": "list_tools"})
    if not response.get("ok"):
        raise RuntimeError(response.get("error", "list_tools failed"))

    out_tools = []
    for tool in response.get("tools", []):
        if not isinstance(tool, dict):
            continue
        out_tools.append(
            {
                "name": tool.get("name", ""),
                "description": tool.get("description", ""),
                "inputSchema": tool.get("input_schema", {"type": "object", "properties": {}}),
                "tool_group": tool.get("tool_group", ""),
                "unreal_domain": tool.get("unreal_domain", ""),
                "workflow_lane": tool.get("workflow_lane", ""),
                "surface_area": tool.get("surface_area", ""),
                "primary_lane": tool.get("primary_lane", ""),
                "vfx_affinity": tool.get("vfx_affinity", ""),
                "capability_tags": tool.get("capability_tags", []),
                "mode": tool.get("mode", ""),
                "mutation_level": tool.get("mutation_level", ""),
                "safe_default": bool(tool.get("safe_default", False)),
                "project_grounded": bool(tool.get("project_grounded", False)),
                "default_exposure": tool.get("default_exposure", ""),
            }
        )
    return {"tools": out_tools}


def _mcp_tools_call(bridge: BridgeClient, params: Dict[str, Any]) -> Dict[str, Any]:
    name = params.get("name")
    arguments = params.get("arguments", {})
    if not isinstance(name, str) or not name:
        raise RuntimeError("tools/call missing params.name")
    if not isinstance(arguments, dict):
        raise RuntimeError("tools/call params.arguments must be an object")

    response = bridge.request({"action": "execute_tool", "tool_name": name, "input": arguments})
    ok = bool(response.get("ok"))
    text = json.dumps(response, ensure_ascii=False)
    result = {"content": [{"type": "text", "text": text}], "structuredContent": response}
    if not ok:
        result["isError"] = True
    return result


def handle_jsonrpc(bridge: BridgeClient, msg: Dict[str, Any]) -> Dict[str, Any]:
    rid = msg.get("id")
    method = msg.get("method")
    params = msg.get("params", {})

    if method == "initialize":
        result = {
            "protocolVersion": "2024-11-05",
            "serverInfo": {"name": "forgepilot", "version": "0.1.0"},
            "capabilities": {"tools": {}},
        }
        return _jsonrpc_result(rid, result)

    if method == "ping":
        pong = bridge.request({"action": "ping"})
        return _jsonrpc_result(rid, pong)

    if method == "tools/list":
        return _jsonrpc_result(rid, _mcp_tools_list(bridge))

    if method == "tools/call":
        if not isinstance(params, dict):
            return _jsonrpc_error(rid, -32602, "Invalid params for tools/call")
        return _jsonrpc_result(rid, _mcp_tools_call(bridge, params))

    if method == "shutdown":
        return _jsonrpc_result(rid, {"ok": True})

    if method == "notifications/initialized":
        return {}

    return _jsonrpc_error(rid, -32601, f"Method not found: {method}")


def main() -> int:
    bridge = BridgeClient(BRIDGE_HOST, BRIDGE_PORT)
    try:
        while True:
            try:
                msg = _read_stdio_message()
            except Exception as exc:
                out = _jsonrpc_error(None, -32700, "Parse error", str(exc))
                _write_stdio_message(out)
                continue

            if msg is None:
                break

            if not isinstance(msg, dict):
                out = _jsonrpc_error(None, -32600, "Invalid Request")
                _write_stdio_message(out)
                continue

            try:
                out = handle_jsonrpc(bridge, msg)
            except Exception as exc:
                out = _jsonrpc_error(msg.get("id"), -32000, "Bridge error", str(exc))

            if out:
                _write_stdio_message(out)
    finally:
        bridge.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
