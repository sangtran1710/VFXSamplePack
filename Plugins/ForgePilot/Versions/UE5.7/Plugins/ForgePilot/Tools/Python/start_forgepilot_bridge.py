import json
import os
import queue
import socket
import threading
import traceback
from hashlib import md5

import unreal


HOST = os.environ.get("FORGEPILOT_BRIDGE_HOST", "127.0.0.1")


def _resolve_bridge_port() -> int:
    raw_env_port = os.environ.get("FORGEPILOT_BRIDGE_PORT", "").strip()
    if raw_env_port:
        try:
            parsed = int(raw_env_port)
        except ValueError:
            parsed = 0
        if 0 < parsed <= 65535:
            return parsed

    project_dir = os.environ.get("FORGEPILOT_BRIDGE_PROJECT_DIR", "").strip()
    if project_dir:
        hash_hex = md5(project_dir.lower().encode("utf-8")).hexdigest()
        return 19080 + (int(hash_hex[:8], 16) % 1000)

    return 19080


PORT = _resolve_bridge_port()

_shutdown_event = threading.Event()
_request_queue = queue.Queue()
_server_thread = None
_server_socket = None
_tick_handle = None


def _json_error(message):
    return {"ok": False, "error": message}


def _log_bridge_error(message, exc=None):
    if exc is None:
        unreal.log_error(f"[ForgePilotBridge] {message}")
        return

    details = f"{type(exc).__name__}: {exc}"
    unreal.log_error(f"[ForgePilotBridge] {message}: {details}")
    unreal.log_error(traceback.format_exc(limit=3))


def _dispatch_on_editor_thread(request_line):
    try:
        subsystem = unreal.get_editor_subsystem(unreal.ForgePilotMCPBridgeSubsystem)
        if subsystem is None:
            return _json_error("ForgePilotMCPBridgeSubsystem is not available.")

        response_text = subsystem.handle_bridge_request(request_line)
        try:
            parsed = json.loads(response_text)
            if isinstance(parsed, dict):
                return parsed
            return {"ok": True, "result": parsed}
        except Exception:
            return {"ok": False, "error": "Bridge returned non-JSON response.", "raw": response_text}
    except Exception as exc:
        return {"ok": False, "error": str(exc), "traceback": traceback.format_exc(limit=1)}


def _tick_callback(_delta_time):
    processed = 0
    while processed < 20:
        try:
            request_line, response_queue = _request_queue.get_nowait()
        except queue.Empty:
            break

        response = _dispatch_on_editor_thread(request_line)
        try:
            response_queue.put_nowait(response)
        except Exception:
            pass
        processed += 1
    return True


def _client_loop(conn):
    with conn:
        stream = conn.makefile("rwb")
        while not _shutdown_event.is_set():
            line = stream.readline()
            if not line:
                break

            request_line = line.decode("utf-8", errors="replace").strip()
            if not request_line:
                continue

            response_queue = queue.Queue(maxsize=1)
            _request_queue.put((request_line, response_queue))

            try:
                response_obj = response_queue.get(timeout=60.0)
            except queue.Empty:
                response_obj = _json_error("Bridge timed out waiting for editor thread.")

            response_text = json.dumps(response_obj, ensure_ascii=False)
            stream.write((response_text + "\n").encode("utf-8"))
            stream.flush()


def _server_loop():
    global _server_socket

    sock = None
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((HOST, PORT))
        sock.listen(8)
        sock.settimeout(0.5)
        _server_socket = sock
        unreal.log(f"[ForgePilotBridge] listening on {HOST}:{PORT}")
    except OSError as exc:
        _log_bridge_error(f"failed to bind/listen on {HOST}:{PORT}", exc)
        if sock is not None:
            try:
                sock.close()
            except Exception:
                pass
        _server_socket = None
        return
    except Exception as exc:
        _log_bridge_error(f"unexpected bridge startup failure on {HOST}:{PORT}", exc)
        if sock is not None:
            try:
                sock.close()
            except Exception:
                pass
        _server_socket = None
        return

    try:
        while not _shutdown_event.is_set():
            try:
                conn, _addr = sock.accept()
            except socket.timeout:
                continue
            except OSError as exc:
                if not _shutdown_event.is_set():
                    _log_bridge_error("socket accept failed", exc)
                break

            worker = threading.Thread(target=_client_loop, args=(conn,), daemon=True)
            worker.start()
    finally:
        try:
            sock.close()
        except Exception:
            pass
        _server_socket = None
        unreal.log("[ForgePilotBridge] server stopped")


def start_bridge():
    global _server_thread, _tick_handle

    if _server_thread is not None and _server_thread.is_alive():
        unreal.log("[ForgePilotBridge] already running")
        return

    _shutdown_event.clear()

    if _tick_handle is None:
        try:
            _tick_handle = unreal.register_slate_post_tick_callback(_tick_callback)
        except Exception as exc:
            _log_bridge_error("failed to register slate tick callback", exc)
            return

    try:
        _server_thread = threading.Thread(target=_server_loop, daemon=True, name="ForgePilotBridgeServer")
        _server_thread.start()
    except Exception as exc:
        _log_bridge_error("failed to start bridge server thread", exc)
        _server_thread = None
        if _tick_handle is not None:
            try:
                unreal.unregister_slate_post_tick_callback(_tick_handle)
            except Exception:
                pass
            _tick_handle = None
        return
    unreal.log("[ForgePilotBridge] started")


def stop_bridge():
    global _server_thread, _tick_handle

    _shutdown_event.set()
    if _server_socket is not None:
        try:
            _server_socket.close()
        except Exception:
            pass

    if _tick_handle is not None:
        try:
            unreal.unregister_slate_post_tick_callback(_tick_handle)
        except Exception:
            pass
        _tick_handle = None

    _server_thread = None
    unreal.log("[ForgePilotBridge] stop requested")


if __name__ == "__main__":
    start_bridge()

