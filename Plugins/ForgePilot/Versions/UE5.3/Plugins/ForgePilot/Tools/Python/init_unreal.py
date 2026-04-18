import traceback
import os
import sys

import unreal


def _bootstrap():
    try:
        this_dir = os.path.dirname(os.path.abspath(__file__))
        if this_dir not in sys.path:
            sys.path.insert(0, this_dir)
        import start_forgepilot_bridge
        start_forgepilot_bridge.start_bridge()
        unreal.log("[ForgePilotBridge] init_unreal bootstrap complete")
    except Exception as exc:
        unreal.log_warning(f"[ForgePilotBridge] bootstrap failed: {exc}")
        unreal.log_warning(traceback.format_exc())


_bootstrap()
