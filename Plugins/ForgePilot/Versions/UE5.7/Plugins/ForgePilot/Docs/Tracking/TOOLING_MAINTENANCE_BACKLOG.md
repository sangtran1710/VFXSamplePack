# ForgePilot Tooling Maintenance Backlog

Updated: 2026-03-23

## Restored now

1. Build/editor wrapper
   - `Tools/Validation/build_unreal_with_recovery.ps1`
   - Verified: active host-project editor target build passed.

2. Crash recovery helpers
   - `Tools/Validation/forgepilot_recovery.ps1`
   - `Tools/Validation/recover_unreal_after_crash.ps1`
   - Verified: analysis script runs and returns structured JSON.
   - Note: JSON state loading now falls back without `ConvertFrom-Json -AsHashtable`, so this no longer requires PowerShell 7+ just for state parsing.

3. Blueprint graph helper tools
   - `create_blueprint_variable_get_node`
   - `set_blueprint_node_pin_default`
   - Files:
     - `Source/ForgePilot/Public/VFXToolRegistry.h`
     - `Source/ForgePilot/Private/VFXToolRegistry_Blueprint.cpp`
   - Verified: plugin compiles successfully with tools registered in source.

4. Skeletal attachment inspector
   - `inspect_skeletal_attachment_context`
   - Files:
     - `Source/ForgePilot/Public/VFXToolRegistry.h`
     - `Source/ForgePilot/Private/VFXToolRegistry_Scene.cpp`
   - Verified: plugin compiles successfully with tool registered in source.

5. Blueprint debug tools for graph-driven bug hunts
   - `find_blueprint_asset_usage`
   - `trace_blueprint_exec_chain`
   - `inspect_blueprint_array_defaults`
   - Files:
     - `Source/ForgePilot/Public/VFXToolRegistry.h`
     - `Source/ForgePilot/Private/VFXToolRegistry.cpp`
     - `Source/ForgePilot/Private/VFXToolRegistry_Blueprint.cpp`
   - Verified:
     - build passes
     - tools load through live editor bridge
     - project-wide asset usage lookup works after AssetRegistry referencer prefilter

## Next verification passes

1. Launch editor and confirm bridge responds to `ping` and `list_tools`.
2. Dry-run `create_blueprint_variable_get_node` on a scratch blueprint.
3. Dry-run `set_blueprint_node_pin_default` on a scratch node.
4. Run `inspect_skeletal_attachment_context` on a selected skeletal actor in the level.
5. Run a generic runtime probe harness and confirm it writes captures and `summary.json`.

## Constraints

- Do not modify host-project game code under `Source/<HostProject>/*`.
- Do not touch character textures.
- Prefer plugin/tooling, Blueprint, Niagara, Material, and validation scripts only.
