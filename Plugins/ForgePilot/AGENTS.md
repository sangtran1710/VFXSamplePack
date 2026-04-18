# ForgePilot - AI-Powered Unreal Editor Automation for Unreal Engine 5.7

## Project Overview
UE5 plugin that automates Unreal Editor creative workflows through AI agents, with VFX as the strongest first-class domain.
AI reads domain knowledge and recipes -> generates execution plans -> calls Unreal tools via MCP bridge -> creates or edits Niagara systems, materials, blueprints, scene content, animation-adjacent assets, and level workflows.

**Owner**: Senior VFX Artist, AAA game development  
**Engine**: Unreal Engine 5.7 active target; UE5.3 is legacy context  
**Target**: Project editor target detected from `Source/*Editor.Target.cs` unless explicitly overridden

## UE5.7 Migration Rule
Before refactoring, rebuilding, or migrating this plugin for UE5.7, read and follow:
- `Docs/Plans/UE57_REBUILD_RULES.md`

That document is the operating contract for preserving ForgePilot's product intent, bridge/tool architecture, dry-run safety, mutation controls, and Material/Niagara workflow purpose during the UE5.7 rebuild.

## Architecture

```text
User Prompt -> AI Agent -> Knowledge Loader -> Execution Plan -> Tool Registry -> MCP Bridge -> Unreal Editor
                           |                                       |
                           v                                       v
                 Knowledge/Effects/*.json              VFXToolRegistry_*.cpp
                 Knowledge/Principles/*.json
```

## Critical Files

### Plugin Source (C++)
| File | Purpose |
|------|---------|
| `VFXToolRegistry.h` | Main tool registry header - all tool declarations |
| `VFXToolRegistry.cpp` | Core registration logic |
| `VFXToolRegistry_Niagara.cpp` | Niagara tools (create system, emitters, modules) |
| `VFXToolRegistry_Material.cpp` | Material tools (create, set params, expressions) |
| `VFXToolRegistry_Blueprint.cpp` | Blueprint tools (graph nodes, variables, pins) |
| `VFXToolRegistry_Scene.cpp` | Scene tools (spawn, probe, knowledge loader) |
| `VFXToolRegistry_Animation.cpp` | Animation tools |
| `VFXKnowledgeLoader.h/cpp` | Reads Knowledge Base JSONs and resolves effect plans |
| `ForgePilotMCPBridgeSubsystem.h/cpp` | MCP bridge HTTP server inside editor |
| `VFXMutationAudit.h/cpp` | Tracks all changes for rollback |
| `ForgePilotCache.h/cpp` | LRU cache for tool results |
| `ForgePilotRateLimiter.h/cpp` | Rate limiting for API calls |
| `ForgePilotMetrics.h/cpp` | Performance metrics |

### Knowledge Base (JSON)
| Path | Purpose |
|------|---------|
| `Knowledge/Effects/<Type>/recipe.json` | Full Niagara setup recipe per effect type |
| `Knowledge/Principles/timing_rules.json` | Universal VFX timing (lifetime, spawn, curves) |
| `Knowledge/Principles/color_palettes.json` | Color palettes per effect type |
| `Knowledge/Principles/scale_reference.json` | Real-world scale references in UE units |
| `Knowledge/Materials/Techniques/*.json` | Material techniques (dissolve, fresnel, erosion, panning) |
| `Knowledge/Niagara/Modules/*.json` | Niagara module reference |
| `Knowledge/effect_synonyms.json` | Maps user terms to effect types |

### Scripts (PowerShell)
| Script | Purpose |
|--------|---------|
| `Tools/Validation/invoke_bridge.ps1` | Call any tool via MCP bridge |
| `Tools/Validation/build_unreal_with_recovery.ps1` | Build with auto-recovery |
| `Tools/Validation/forgepilot_recovery.ps1` | Crash recovery helper |
| `Tools/Validation/run_effect_v2_eval.ps1` | Generic V2 eval harness for orb-family workflows |
| `Tools/Validation/run_suction_core_v2_eval.ps1` | V2 eval harness for suction-core workflow |

### MCP Server (Python)
| File | Purpose |
|------|---------|
| `Tools/MCP/forgepilot_mcp_server.py` | MCP server exposing tools to AI |

## External VFX Research

Before creating, rebuilding, or deeply debugging a visual VFX effect, research RealTimeVFX first.

Primary reference:
- `https://realtimevfx.com/search?q=<topic>`

Because the RealTimeVFX search page is JavaScript-driven, prefer direct domain searches such as:
- `site:realtimevfx.com fire flipbook niagara unreal`
- `site:realtimevfx.com smoke flipbook erosion material`
- `site:realtimevfx.com explosion flipbook unreal niagara`
- `site:realtimevfx.com projectile trail ribbon niagara`

Follow the compact workflow in:
- `Docs/Guides/REALTIMEVFX_RESEARCH_WORKFLOW.md`

Skip this research step only for pure ForgePilot infrastructure work, such as C++ tool registration, bridge bugs, PowerShell validation scripts, or repo cleanup.

## Key Tools Available via Bridge

### Knowledge
- `load_vfx_recipe` - Load effect recipe and principles by type
- `resolve_effect_plan` - V1 generic text prompt to ordered tool call sequence
- `resolve_effect_plan_v2` - V2 multi-family planner for `suction_core_energy`, `electric_arc_orb`, `hero_fire_orb_sweep`, `arcane_pull_orb`, `impact_pulse_core`, and `explosion_flash_core`

### V2 Orb-Family Production Path
- `run_effect_workflow_v2` - Preferred production entrypoint for the V2 orb families in this milestone
- `review_effect_failure_taxonomy_v2` - Rules-based V2 critic with family-specific failure taxonomy, proxy metrics, and hard-gate state
- `run_v2_material_op` - Generic V2 semantic material op wrapper
- `run_v2_niagara_op` - Generic V2 semantic Niagara op wrapper
- `run_suction_core_v2_workflow` - Compatibility/example wrapper for the `suction_core_energy` family
- `review_suction_core_failure_taxonomy` - Compatibility/example critic wrapper for the `suction_core_energy` family
- Raw Niagara/material tools remain available for inspection, debugging, and targeted authoring, but they are not the default production path for these V2 families

### Niagara
- `create_niagara_system` - Create new Niagara system asset
- `get_niagara_system_info` - Get system details
- `get_niagara_emitter_details` - Get emitter details
- `get_niagara_emitter_stack` - Get module stack
- `set_niagara_user_parameter` - Set user parameters
- `spawn_niagara_at_location` - Spawn in scene

### Material
- `create_material` - Create new material asset
- `add_material_expression` - Add node to material graph
- `connect_material_nodes` - Connect expressions
- `set_material_properties` - Set material properties
- `create_material_instance` - Create material instance

### Blueprint
- `create_blueprint` - Create new blueprint
- `add_variable_to_blueprint` - Add variable
- `create_blueprint_variable_get_node` - Create getter node
- `set_blueprint_node_pin_default` - Set pin default value

### Scene
- `get_scene_actors` - List scene actors
- `spawn_actor` - Spawn actor in scene
- `capture_viewport_screenshot` - Capture viewport
- `inspect_skeletal_attachment_context` - Inspect skeletal mesh

## Build & Test Commands

Run canonical validation from the UE5.7 workspace.

```powershell
# Build project
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/build_unreal_with_recovery.ps1"

# Test bridge is alive
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -Ping

# List all available tools
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ListTools

# Test knowledge loader
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "load_vfx_recipe" -ArgsJson '{"effect_type":"fire"}'

# Test V1 effect plan
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "resolve_effect_plan" -ArgsJson '{"description":"campfire with embers and smoke"}'

# Test V2 multi-family plan
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "resolve_effect_plan_v2" -ArgsJson '{"description":"explosion orb with a white hot flash core and outward blast streaks"}'

# Run the preferred generic V2 workflow
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "run_effect_workflow_v2" -ArgsJson '{"description":"impact pulse orb with a sharp shock burst and bright core flash","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/V2Platform","system_name":"NS_OrbFamilies_V2_Workflow_DocExample","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true,"keep_assets":false}'

# Run the suction-core compatibility/example wrapper
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "run_suction_core_v2_workflow" -ArgsJson '{"description":"fire energy suction orb pulling streaks from all directions into a bright hot core","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/CoreEnergy","system_name":"NS_SuctionCore_V2_Workflow_DocExample","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true}'

# Capture viewport
powershell -File "Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "capture_viewport_screenshot" -ArgsJson '{"filename":"test_capture"}'
```

For the V2 orb families in this milestone, prefer `resolve_effect_plan_v2` plus `run_effect_workflow_v2`. Use the suction-core wrapper when you specifically want the compatibility/example path for `suction_core_energy`. Use raw Niagara/material tools when you are doing tooling work, debugging, or targeted follow-up edits after the workflow runner has created the baseline assets.

## Rules

1. **NEVER modify** host project game code unless the user explicitly asks; ForgePilot work should stay under `Plugins/ForgePilot/`
2. **ALWAYS build** after C++ changes: use `build_unreal_with_recovery.ps1`
3. **ALWAYS verify** via bridge after adding new tools
4. **Save assets** after creation/modification
5. Knowledge JSON files are authored by the tech advisor (Claude), not auto-generated
6. `effect_synonyms.json` is the only place to add new effect type mappings
7. Tool registration pattern: declare in `VFXToolRegistry.h`, implement in `VFXToolRegistry_<Category>.cpp`
8. If the user asks to send/show an image in the thread/chat, embed it inline with Markdown image syntax using an absolute local file path (for example `![probe](C:/path/image.jpg)`), not a plain text path or link-only response
9. For V2 orb-family tasks, prefer `resolve_effect_plan_v2` + `run_effect_workflow_v2` over hand-assembled semantic-op or raw-tool sequences unless the task is explicitly tooling/debug work
10. For `suction_core_energy`, the compatibility/example path `run_suction_core_v2_workflow` remains supported and should behave consistently with the generic V2 planner
11. Keep the plugin root clean. Do not create loose `.md`, `.json`, or `.txt` files directly under `Plugins/ForgePilot/` unless the file is an explicit root-level `README` or a user-facing usage guide that the user specifically asked to keep at the root. Put guides/plans/templates under `Docs/`, validation data under `Tools/Validation/`, and scratch/generated review data under `Output/` or `Saved/`.
12. Treat `UE5.7` as the active product branch. Do not spend time or tokens keeping `UE5.3` in lockstep unless the user explicitly asks for legacy compatibility work.
