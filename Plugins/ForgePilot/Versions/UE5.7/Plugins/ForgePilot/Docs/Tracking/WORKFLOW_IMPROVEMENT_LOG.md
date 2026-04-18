# Workflow Improvement Log

Updated: 2026-03-23

## Current recovery note

- GitHub Desktop discard/reset removed uncommitted local tool code from the working tree.
- Asset autosaves still exist under `Saved/Autosaves`, but plugin code had to be rebuilt manually.

## Lessons

1. Commit or stash plugin/tool changes in smaller batches.
2. Keep build/recovery scripts inside plugin and verify them after every rebuild.
3. Treat editor automation and gameplay capture as separate verification problems.
4. Prefer restoring tool capability first; Unreal assets can often be recreated faster than code.

## Rebuilt after loss

- `build_unreal_with_recovery.ps1`
- `forgepilot_recovery.ps1`
- `recover_unreal_after_crash.ps1`
- Blueprint graph helper tools
- Skeletal attachment inspector tool

## Follow-up fixes

- Fixed probe step/capture accumulation to use script scope, so `summary.json` now records the full runtime sequence.
- Added bridge-helper preflight in the transformation probe so missing `invoke_bridge.ps1` fails early and clearly.
- Removed the practical PowerShell 7 dependency from recovery-state parsing by adding a hashtable fallback for Windows PowerShell 5.1 style sessions.

## Blueprint debug tooling

- Added `find_blueprint_asset_usage` for asset-to-Blueprint usage tracing.
- Added `trace_blueprint_exec_chain` for forward exec-chain tracing from event/function/node seeds.
- Added `inspect_blueprint_array_defaults` for direct inspection of Blueprint CDO array defaults like `BloodEffects`.
- Important implementation lesson: project-wide asset usage lookup must prefilter with AssetRegistry referencers before loading Blueprints, otherwise the editor bridge can time out waiting on the editor thread.
