from __future__ import annotations

import json
import logging
import os
import socket
from hashlib import md5
from pathlib import Path
from typing import Any

LOGGER = logging.getLogger(__name__)


class UeMcpError(RuntimeError):
    pass


def _find_project_root(start: Path) -> Path:
    cursor = start.resolve()
    for candidate in [cursor, *cursor.parents]:
        if any(candidate.glob("*.uproject")):
            return candidate
    return Path.cwd().resolve()


def _get_default_project_dir() -> str:
    return _find_project_root(Path(__file__).resolve().parent).as_posix().rstrip("/")


def _get_default_workspace_hash_hex() -> str:
    return md5(_get_default_project_dir().lower().encode("utf-8")).hexdigest()


def _get_default_bridge_port() -> int:
    raw_env = os.environ.get("FORGEPILOT_BRIDGE_PORT", "").strip()
    if raw_env:
        try:
            parsed = int(raw_env)
        except ValueError:
            parsed = 0
        if 0 < parsed <= 65535:
            return parsed
    return 19080 + (int(_get_default_workspace_hash_hex()[:8], 16) % 1000)


def _add_expected_bridge_identity(payload: dict[str, Any]) -> dict[str, Any]:
    result = dict(payload)
    result.setdefault("expected_workspace_id", f"fpw_{_get_default_workspace_hash_hex()[:12]}")
    result.setdefault("expected_project_dir", _get_default_project_dir())
    return result


class UeMcpClient:
    def __init__(
        self,
        host: str | None = None,
        port: int | None = None,
        *,
        timeout_seconds: float = 30.0,
    ) -> None:
        self.host = host or os.environ.get("FORGEPILOT_BRIDGE_HOST", "127.0.0.1")
        self.port = port if port is not None else _get_default_bridge_port()
        self.timeout_seconds = timeout_seconds

    def ping(self) -> dict[str, Any]:
        return self._request({"action": "ping"})

    def list_tools(self) -> dict[str, Any]:
        return self._request({"action": "list_tools"})

    def call_tool(
        self,
        tool_name: str,
        arguments: dict[str, Any] | None = None,
        *,
        timeout_seconds: float | None = None,
    ) -> dict[str, Any]:
        payload = {
            "action": "execute_tool",
            "tool_name": tool_name,
            "input": arguments or {},
        }
        return self._request(payload, timeout_seconds=timeout_seconds)

    def _request(
        self,
        payload: dict[str, Any],
        *,
        timeout_seconds: float | None = None,
    ) -> dict[str, Any]:
        effective_timeout = timeout_seconds or self.timeout_seconds
        LOGGER.info(
            "UE MCP request host=%s port=%s payload=%s",
            self.host,
            self.port,
            json.dumps(payload, ensure_ascii=False, sort_keys=True),
        )
        identity_payload = _add_expected_bridge_identity(payload)
        with socket.create_connection((self.host, self.port), timeout=effective_timeout) as sock:
            sock.settimeout(effective_timeout)
            stream = sock.makefile("rwb")
            stream.write((json.dumps(identity_payload, ensure_ascii=False) + "\n").encode("utf-8"))
            stream.flush()
            line = stream.readline()
        if not line:
            raise UeMcpError("Bridge closed the connection.")
        try:
            parsed = json.loads(line.decode("utf-8", errors="replace"))
        except json.JSONDecodeError as exc:
            raise UeMcpError(f"Bridge returned invalid JSON: {exc}") from exc
        if not isinstance(parsed, dict):
            raise UeMcpError("Bridge response is not a JSON object.")
        LOGGER.info("UE MCP response=%s", json.dumps(parsed, ensure_ascii=False, sort_keys=True))
        return parsed


def bool_string(value: bool) -> str:
    return "true" if value else "false"
