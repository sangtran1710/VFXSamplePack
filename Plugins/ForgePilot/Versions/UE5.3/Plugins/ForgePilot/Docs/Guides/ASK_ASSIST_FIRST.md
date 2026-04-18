# Ask + Assist First

ForgePilot now defaults to `Ask + Assist` before `Build`.

## What this means

- Start from the real project state, not from a speculative build plan.
- Inspect, trace, review, and explain first.
- Use content operations for deterministic tasks.
- Treat Material/Niagara/V2 automation as advanced opt-in paths.

## Grounding order

1. Current editor/project state
2. Direct asset inspection/review tools
3. Runtime/project knowledge assets
4. Internal docs and guides

## Product Surface

For the clean grouped inventory of the current tool surface, see [TOOL_SURFACE.md](./TOOL_SURFACE.md).

## First examples

Find assets:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "search_assets" -ArgsJson '{"query":"fire trail","class_names":"Material,NiagaraSystem","max_results":20}'
```

Inspect what is selected:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "get_context"
```

Trace references and dependencies:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "trace_asset_relationships" -ArgsJson '{"asset_path":"/Game/ForgePilot_Test/Materials/M_FireTrail.M_FireTrail"}'
```

Organize content safely:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "create_asset_folder" -ArgsJson '{"folder_path":"/Game/ForgePilot_Test/Assist","apply_changes":true}'
```

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "move_asset" -ArgsJson '{"asset_path":"/Game/ForgePilot_Test/Assist/M_Copy.M_Copy","destination_path":"/Game/ForgePilot_Test/Assist/Moved","apply_changes":true}'
```

Focus an asset in the editor without opening a toolkit tab:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "browse_to_asset" -ArgsJson '{"asset_path":"/Game/ForgePilot_Test/Materials/M_FireTrail.M_FireTrail"}'
```

Explain a Niagara or Material setup:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "review_niagara_system" -ArgsJson '{"system_path":"/Game/VFX/NS_MySystem.NS_MySystem"}'
```

## Advanced Build

Use the advanced build guides only when the user explicitly wants authoring or workflow automation:

- [ORB_FAMILIES_V2_PLATFORM.md](./ORB_FAMILIES_V2_PLATFORM.md)
- [SUCTION_CORE_V2_WORKFLOW.md](./SUCTION_CORE_V2_WORKFLOW.md)

`open_asset` is intentionally not part of the default bridge flow. In this editor environment, opening toolkit tabs through the bridge can destabilize Unreal, so `browse_to_asset` is the safe default.
