# UE5.7 Rebuild Rules

## Purpose
This document is the operating contract for rebuilding `ForgePilot` from its original UE5.3, project-specific form into a UE5.7-ready, project-agnostic VFX authoring system.

The goal is not to make the code compile by any means necessary. The goal is to preserve and strengthen the system's original purpose:

- help a senior VFX artist ask, inspect, plan, build, critique, and iterate inside Unreal Editor
- expose deterministic editor tools to external AI agents through a bridge/MCP surface
- automate Material and Niagara authoring without turning the project into disconnected one-off scripts
- keep every mutating action reviewable, bounded, and recoverable

## Non-Negotiable Product Intent
`ForgePilot` must remain a workflow engine for VFX production.

Every refactor must support at least one of these outcomes:

- better project understanding
- safer tool execution
- more reliable Material authoring
- more reliable Niagara authoring
- clearer critique and iteration
- better restart, recovery, or packaging
- cleaner multi-project support

If a change only makes an isolated script or helper work while weakening the workflow, it is the wrong change.

## Source Of Truth
The UE5.7 rebuild must preserve this high-level architecture:

```text
User/AI Prompt
  -> MCP client or local validation script
  -> Tools/MCP/forgepilot_mcp_server.py
  -> Unreal Python socket bridge
  -> UForgePilotMCPBridgeSubsystem
  -> FVFXToolRegistry
  -> Unreal Editor APIs
  -> structured result, artifact, report, or asset mutation
```

The bridge and registry are the system boundary. Do not bypass them for normal production workflows.

## Required Migration Outcomes
The rebuild is not complete until these are true:

1. The plugin targets UE5.7 cleanly.
2. The plugin no longer assumes any specific project name or editor target.
3. Build scripts detect or accept the current `.uproject`, editor target, and UE root.
4. The bridge can start only when settings allow it.
5. The MCP surface can ping, list tools, and execute read-only tools.
6. Mutating tools remain dry-run by default unless explicitly told to apply changes.
7. Asset-modifying tools can be globally disabled through settings.
8. Material and Niagara smoke workflows are verified in UE5.7.
9. Generated artifacts are kept out of source/package output.
10. Documentation reflects UE5.7 as the active target and UE5.3 as legacy context.

## Branch Priority Rule

ForgePilot is rebuilt for `UE5.7` first.

- `UE5.7` is the product branch.
- `UE5.3` is legacy support, not a parity requirement.
- Codex should not spend tokens keeping both branches synchronized unless the user explicitly asks for dual-version work.
- When a change is only needed for forward progress, it is acceptable to land it in `UE5.7` only.

## Hard Safety Rules
### 1. Do not collapse dry-run semantics
Any tool that can modify assets, levels, graphs, components, files, or editor state must keep an explicit dry-run path.

Required behavior:

- `apply_changes=false` plans or reports only
- `apply_changes=true` mutates only after validation
- result JSON must state whether the call was a dry run
- result JSON must state what would change or what did change

### 2. Enforce global mutation settings
`bAllowAssetModification=false` must prevent mutating tool execution.

This must be enforced centrally, preferably in `FVFXToolRegistry::ExecuteTool`, not scattered only inside individual tools.

### 3. Bridge startup settings must be real
The module must not silently re-enable bridge startup.

Required behavior:

- `bEnableMCPBridge=false` means no bridge bootstrap
- `bStartMCPServerOnStartup=false` means no automatic socket bridge startup
- manual connect may start the bridge only if bridge is enabled
- UI must show disabled/waiting/error states honestly

### 4. No project-specific paths in reusable defaults
Default scripts must not hardcode:

- a legacy project name
- a legacy editor target name
- a legacy absolute project path
- `UE_5.3`

Project-specific paths are allowed only as explicit optional parameters, examples, or archived legacy docs.

### 5. Do not weaken structured tool results
All bridge/tool responses must remain JSON-object based and predictable.

Required failure fields:

- `success` or `ok`
- `error_code`
- `error_message`
- `debug_context` when useful

Text-only errors are allowed only as fallback, never as the preferred contract.

### 6. Do not remove recovery/audit behavior
Mutation audit, snapshots, rollback, cleanup, and validation reports are part of the product.

Do not remove them to simplify migration. If an API changed, adapt the layer or disable the specific feature with an explicit capability flag and report.

### 7. Do not make AI improvisation authoritative
AI analysis may suggest; persisted session state, workflow contracts, and deterministic rules decide.

For workflow/orchestration code:

- bounded iteration counts must remain
- failure states must be explicit
- restart/fork behavior must be traceable
- family/style decisions must not silently change mid-branch

## Architecture Rules
### Tool Registry
`FVFXToolRegistry` remains the authoritative registry for editor tools.

Every new or migrated tool must define:

- name
- description
- input schema
- mode: `ask`, `assist`, or `build`
- mutation level
- safe-default status
- project-grounded status
- default exposure

### Compatibility Layer
UE5.7-specific C++ API changes should be isolated behind compatibility helpers where practical.

Prefer:

- `ForgePilotEngineCompat.h`
- small wrapper functions
- category-specific compatibility helpers

Avoid scattering version checks throughout large tool bodies unless the difference is tiny.

### Capability Discovery
The rebuilt plugin should expose a capability/status tool that reports:

- engine version
- plugin version
- bridge status
- available editor modules
- loaded tool categories
- disabled tools or feature flags
- active safety settings

Agents should inspect capabilities before running advanced workflows.

### Python Bridge
The Python socket bridge should remain thin:

- transport
- queueing
- dispatch onto editor thread
- JSON response handling

Do not move Unreal authoring logic into the bridge script unless it is only bridge bootstrapping.

### MCP Server
The MCP server should remain an adapter:

- `initialize`
- `ping`
- `tools/list`
- `tools/call`

It must not duplicate tool logic already owned by the Unreal plugin.

## Migration Order
Codex must work in this order unless explicitly instructed otherwise:

1. Source hygiene and project-agnostic script defaults.
2. Descriptor/settings cleanup for UE5.7.
3. Central safety enforcement.
4. UE5.7 compile pass.
5. Bridge smoke tests.
6. Tool list and read-only tool verification.
7. Material smoke workflow.
8. Niagara smoke workflow.
9. Packaging/source cleanup.
10. Documentation update.

Do not start Material/Niagara feature work before the bridge and safety layer are stable.

## Change Scope Rules
Make changes in small, reviewable slices.

Do not mix these in a single patch unless unavoidable:

- project-agnostic script cleanup
- C++ API migration
- bridge behavior changes
- safety enforcement
- Material feature changes
- Niagara feature changes
- knowledge base redesign
- visual critique/orchestration changes

Each slice must have its own verification.

## Verification Gates
### Gate 1: Source Hygiene
Required checks:

- no reusable default hardcodes a legacy project identifier
- no reusable default hardcodes `UE_5.3`
- no package/source output includes `Binaries`, `Intermediate`, `Output`, `Saved`, or `__pycache__`

### Gate 2: Syntax
Required checks:

- PowerShell scripts parse
- Python files compile
- JSON knowledge files parse

### Gate 3: Build
Required checks:

- UE5.7 editor target builds
- generated project files are refreshed if needed
- warnings caused by migration are reviewed, not ignored blindly

### Gate 4: Bridge
Required checks:

- bridge disabled setting prevents startup
- bridge startup setting prevents automatic startup
- manual connect starts bridge when enabled
- `invoke_bridge.ps1 -Ping` succeeds when editor bridge is running
- `list_tools` succeeds

### Gate 5: Safety
Required checks:

- read-only tool works with default settings
- mutating tool with `apply_changes=false` dry-runs
- mutating tool with `apply_changes=true` works only when modifications are allowed
- mutating tool is blocked when global modification setting is disabled

### Gate 6: Material
Required checks:

- inspect material info
- create or modify a test material in a dedicated `/Game/ForgePilot_Test/UE57` path
- compile material and capture diagnostics
- cleanup or explicitly keep test assets with report

### Gate 7: Niagara
Required checks:

- create or inspect a test Niagara system in a dedicated `/Game/ForgePilot_Test/UE57` path
- set or inspect user parameters
- compile or report compile diagnostics
- spawn/capture only if bridge/editor state supports it
- cleanup or explicitly keep test assets with report

## Done Criteria
The UE5.7 rebuild is done only when:

- UE5.7 build succeeds
- bridge smoke passes
- tool list passes
- at least one read-only scene/context tool passes
- at least one dry-run mutating tool passes
- global mutation block is verified
- Material smoke passes or has a documented API blocker
- Niagara smoke passes or has a documented API blocker
- hardcoded legacy defaults are removed
- docs tell the next agent how to run the new checks

If any API blocker remains, the rebuild may be marked `blocked`, not `done`.

## Stop Conditions
Codex must stop and report instead of continuing when:

- a change would remove dry-run support
- a change would bypass the bridge/registry for normal workflows
- a migration error affects asset mutation safety
- a tool can mutate without a clear `apply_changes=true`
- a build fix requires deleting a major workflow lane
- UE5.7 API replacement is uncertain and would require guessing inside production mutation code

When stopped, report:

- failing file/function
- expected behavior
- observed blocker
- safest next options

## Documentation Requirements
Every meaningful migration patch must update at least one of:

- this file
- `Docs/README.md`
- relevant workflow guide
- relevant validation script help
- migration report under `Output/Reports`

Documentation must distinguish:

- UE5.7 active behavior
- UE5.3 legacy behavior
- project-specific examples
- reusable plugin defaults

## Final Rule
Do not optimize for a fast port. Optimize for a dependable VFX authoring system that a senior artist can trust after the port.
