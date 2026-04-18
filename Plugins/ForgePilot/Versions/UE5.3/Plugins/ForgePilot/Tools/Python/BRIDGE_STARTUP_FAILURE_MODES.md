# ForgePilot Bridge Startup Failure Modes

The Python bridge listener is created in `start_forgepilot_bridge.py`, not inside the C++ subsystem.

Expected startup sequence:

1. `FForgePilotModule::StartupModule()` requests bridge bootstrap.
2. Unreal Python runs `init_unreal.py`.
3. `init_unreal.py` calls `start_forgepilot_bridge.start_bridge()`.
4. `start_bridge()` registers the slate tick callback and starts the server thread.
5. `_server_loop()` binds `HOST:PORT`, starts listening, and logs `[ForgePilotBridge] listening on ...`.

Explicit failure modes now logged:

- `failed to register slate tick callback`
  The Python bridge could not hook editor ticks, so requests would never reach the subsystem.
- `failed to start bridge server thread`
  Thread creation/start failed before socket startup.
- `failed to bind/listen on HOST:PORT`
  Socket creation reached bind/listen and failed. Common causes:
  - another process already owns the port
  - host/port env override is invalid
  - local firewall or policy blocks the bind
- `socket accept failed`
  The listener started, but later hit a socket-level runtime failure while accepting clients.

Recommended verification logs in Unreal:

- `[ForgePilotBridge] started`
- `[ForgePilotBridge] listening on 127.0.0.1:19080`
- any `[ForgePilotBridge] ... failed ...` `LogPython` error lines

If the listener is not reachable:

1. Confirm Unreal Editor is still running.
2. Check the active project log under `Saved/Logs/<ProjectName>.log` for the messages above.
3. Verify `Get-NetTCPConnection -LocalPort 19080 -State Listen`.
4. Verify `python Tools/Validation/invoke_bridge.py --ping --pretty`.
