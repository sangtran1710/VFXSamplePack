# ForgePilot

ForgePilot is an Unreal Editor automation plugin with `UE5.7` as the active build and product target.

## Version Split

- Open `Plugins/ForgePilot/Versions/UE5.7/VFXSamplePack.uproject` for the active UE5.7 tool.
- Open `Plugins/ForgePilot/Versions/UE5.3/VFXSamplePack.uproject` only for legacy UE5.3 maintenance.
- The root `Plugins/ForgePilot` copy is a UE5.7 staging/shared copy, not the legacy branch.

## Branch Policy

- `Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot` is the canonical rebuild workspace.
- `Plugins/ForgePilot/Versions/UE5.3/Plugins/ForgePilot` is a legacy compatibility reference.
- Do not spend tokens trying to keep `UE5.3` feature parity with `UE5.7` unless the user explicitly asks.

## Practical Rule

When planning, coding, validating, or rebuilding:

1. Work on `UE5.7` first.
2. Treat `UE5.3` as legacy support only.
3. Port to `UE5.3` only for narrow compatibility fixes that are explicitly requested.
4. Do not mirror every new tool, refactor, or workflow to `UE5.3` by default.

## Why

This keeps ForgePilot focused on the forward product direction and avoids wasting context, tokens, and implementation time on dual-version lockstep maintenance.

## Canonical Commands

From the repository root, use the UE5.7 workspace scripts:

```powershell
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/run_forgepilot_healthcheck.ps1"
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/build_unreal_with_recovery.ps1"
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -Ping
```

Only run the UE5.3 workspace scripts when doing explicit legacy maintenance.

## Related Docs

- [Docs/README.md](./Docs/README.md)
- [Docs/Plans/VERSIONED_PLUGIN_LAYOUT.md](./Docs/Plans/VERSIONED_PLUGIN_LAYOUT.md)
- [Docs/Plans/UE57_REBUILD_RULES.md](./Docs/Plans/UE57_REBUILD_RULES.md)
