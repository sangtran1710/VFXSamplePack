# Blueprint Debug Tooling Plan

Updated: 2026-03-23

## Goal

Give ForgePilot enough Blueprint introspection to solve graph-driven bug hunts in a general Unreal project without relying on manual graph copy/paste.

The target workflow is:

1. Find which Blueprint is using a suspicious asset.
2. Identify whether the asset is used in node pins, class defaults, or component/template defaults.
3. Trace the exec chain from a suspicious event or node to see exactly what will fire next.
4. Inspect array defaults directly so wrong pooled assets can be spotted quickly.

## Scope

- Plugin-only changes inside `Plugins/ForgePilot`
- No changes to host-project game code under `Source/<HostProject>/*`
- No gameplay logic edits
- No character texture edits

## Planned Tools

### 1. `find_blueprint_asset_usage`

Purpose:
- Given a Blueprint path and an asset path, return all known usage points inside that Blueprint.

Must cover:
- node pin default object matches
- node pin default string/path matches
- class default object property matches
- array/default element matches
- component template property matches where possible

Success criteria:
- Can explain a case where a suspicious Niagara system is being used inside a gameplay Blueprint
- Returns enough context to point at the graph/default location quickly

Status:
- Implemented
- Verified on live bridge against a scratch gameplay Blueprint with a known Niagara-system reference
- Also verified project-wide lookup after optimizing with AssetRegistry referencers

### 2. `trace_blueprint_exec_chain`

Purpose:
- Given a Blueprint path plus an event/function/node query, return the forward exec chain as ordered node paths.

Must cover:
- seed by node guid
- seed by event/function/title query
- forward traversal over exec pins only
- cycle protection
- multiple branches

Success criteria:
- Can show a chain like `BeginPlay -> SpawnSystemAttached -> Delay -> SpawnSystemAtLocation -> DestroyActor`

Status:
- Implemented
- Verified on live bridge against a scratch Blueprint `Event BeginPlay`

### 3. `inspect_blueprint_array_defaults`

Purpose:
- Inspect array defaults on the Blueprint class default object, especially arrays of asset references.

Must cover:
- filter by array name
- list all arrays if no filter is provided
- export element values and object paths cleanly

Success criteria:
- Can inspect arrays of asset references and show mismatched assets in the pool

Status:
- Implemented
- Verified on live bridge against a scratch Blueprint array of asset references

## Implementation Order

1. Add helper functions for Blueprint property export and asset/path matching.
2. Register the 3 new tools in the Blueprint registry.
3. Implement `inspect_blueprint_array_defaults` first.
4. Implement `find_asset_usage_in_blueprint`.
5. Implement `trace_blueprint_exec_chain`.
6. Build plugin.
7. Run validation against a Blueprint-driven bug workflow.

## Implementation Notes

- Project-wide asset lookup initially timed out when loading many Blueprint assets directly.
- The final implementation now asks AssetRegistry for referencer packages first, then inspects only candidate Blueprint assets.
- This keeps the “which Blueprint is using this asset?” workflow responsive enough for live bridge use.

## Verification Plan

### Compile Gate

- Active host-project editor target `Win64 Development` build passes

### Tool Gate

- `inspect_blueprint_array_defaults` returns structured array/default data from a live Blueprint asset
- `find_asset_usage_in_blueprint` finds a known asset reference inside a Blueprint
- `trace_blueprint_exec_chain` returns ordered node paths from a known event/query

### Workflow Gate

Use a general Blueprint debugging flow as the acceptance test:

1. Find a suspicious asset reference such as `NS_Test_BloodBurst`
2. Ask which Blueprint uses it
3. Trace the event chain from the suspicious node/event
4. Inspect any related effect arrays/defaults

If these tools can explain that workflow without manual graph text export, this task is done.

## Result

Done.

Built and verified:

1. `find_blueprint_asset_usage`
2. `trace_blueprint_exec_chain`
3. `inspect_blueprint_array_defaults`

Validation completed:

- Plugin build passed in the active host-project editor target
- Tools loaded through the editor bridge
- `find_blueprint_asset_usage` found a known Niagara-system reference inside a scratch Blueprint
- `trace_blueprint_exec_chain` returned an ordered exec path from a known entry event
- `inspect_blueprint_array_defaults` returned clean exported array entries from a scratch Blueprint

## Update Rule

If implementation reality differs from this plan, update this file instead of silently drifting scope.
