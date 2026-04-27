# ForgePilot Tool Surface Lanes

## Purpose
ForgePilot is a general Unreal Editor automation plugin with VFX as its strongest production lane.

The tool surface must not imply that every request is a VFX request. Agents should route work by Unreal domain first, then choose the safest mode and tool lane.

## Public Tool Metadata
Every tool exposed through the bridge/MCP surface should include these routing fields:

| Field | Purpose |
|------|---------|
| `tool_group` | The simple group agents should show/use first: `General`, `Blueprint`, `Material`, `Niagara`, `Scene`, or `Animation`. |
| `unreal_domain` | The Unreal area the tool acts on: `scene`, `content_browser`, `blueprint`, `material`, `niagara`, `animation`, `runtime_preview`, or `visual_effect`. |
| `workflow_lane` | The practical workflow lane: `scene_world`, `content_ops`, `blueprint_gameplay`, `material_authoring`, `niagara_authoring`, `animation_inspection`, `runtime_preview`, or `vfx_orchestration`. |
| `surface_area` | More specific surface label for UX/filtering, such as `asset_operations`, `material_authoring`, or `effect_orchestration`. |
| `primary_lane` | Broad creative lane used for high-level planning: `general_unreal`, `gameplay`, `material`, `niagara`, or `animation`. |
| `vfx_affinity` | How strongly the tool belongs to VFX work: `primary`, `supporting`, or `adjacent`. |
| `capability_tags` | Search/filter hints for agents and UI. |
| `mode` | `ask`, `assist`, or `build`. |
| `mutation_level` | Whether the tool is read-only, a safe content op, or advanced authoring. |
| `safe_default` | Whether it is safe to expose by default. |
| `project_grounded` | Whether the tool depends on current project/editor state. |
| `default_exposure` | `default` or `advanced`. |

## Lane Intent
### Simple Tool Groups
- `General`: context, Content Browser operations, runtime preview, screenshots, bridge helpers, and high-level multi-lane workflows.
- `Blueprint`: Blueprint inspection, component setup, graph nodes, pins, compile feedback, and Blueprint review.
- `Material`: material assets, material instances, graph editing, material probes, shader cleanup, and material QA.
- `Niagara`: Niagara systems, emitters, modules, user parameters, stack editing, spawning, compilation, and Niagara QA.
- `Scene`: level/world actor inspection, actor spawn/delete, selection context, and scene relationships.
- `Animation`: animation-adjacent inspection/review and future animation workflows.

### General Unreal lanes
- `scene_world`: inspect or edit world actors and scene context.
- `content_ops`: safe Content Browser operations such as search, browse, folder creation, rename, move, duplicate, and guarded delete.
- `blueprint_gameplay`: Blueprint inspection and graph/component edits.
- `animation_inspection`: animation-adjacent inspection/review, not full animation authoring yet.
- `runtime_preview`: PIE, viewport, screenshot, and probe-style validation.

### Strong VFX lanes
- `material_authoring`: core ForgePilot lane for shader/material authoring, inspection, cleanup, visual probes, and material QA.
- `niagara_authoring`: core ForgePilot lane for Niagara systems, emitters, modules, user parameters, stack edits, and Niagara QA.
- `vfx_orchestration`: high-level VFX planners and workflow runners that combine knowledge, material, Niagara, visual contract, critique, repair, and validation.

## Routing Rules For Agents
1. Start from the user's actual intent, not from the plugin's history.
2. Use `tool_group` first when presenting or choosing the broad tool family.
3. Use `unreal_domain` and `workflow_lane` when a task needs finer routing.
4. Use `Ask` tools first when the current project context is unknown.
5. Use `General` or `content_ops` for asset organization requests, even if the assets are visual-effect assets.
6. Use `Blueprint` for Blueprint/gameplay work unless the task explicitly needs a visual-effect attachment or runtime visual hookup.
7. Use `Material`, `Niagara`, or `vfx_orchestration` only when the request involves shader/material, particles, effects, visual polish, or visual-effect production.
8. VFX is the strongest lane, but it is not the default lane for every Unreal task.
9. Mutating tools must keep dry-run behavior and must respect global mutation settings.

## Naming Rule
Existing C++ types such as `FVFXToolRegistry` are legacy internal names and should not drive product behavior.

Do not rename the registry class in the same patch as behavior or surface changes. A class/file rename is allowed later only as its own focused compatibility slice after bridge, build, and validation are stable.
