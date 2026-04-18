# ForgePilot Smoke Checklist

Use this checklist before handoff or before starting a new rebuild session.

## Default Ask + Assist

For the canonical default product direction, start with `Docs/Guides/ASK_ASSIST_FIRST.md`.

Run the default Ask + Assist validation:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_ask_assist_validation.ps1
```

This validation covers:

- `list_tools` and MCP `tools/list` metadata
- core Ask tools grounded on current project state
- safe Assist content operations
- mutation audit coverage for Assist mutations
- advanced Build tools still exposed as advanced

## Advanced Build: Suction Core V2

For the canonical `suction_core_energy` production path, use `Docs/Guides/SUCTION_CORE_V2_WORKFLOW.md`.

Run the eval harness:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_suction_core_v2_eval.ps1 -RunSemanticOps -HeroMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01" -CoreMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01"
```

Optional direct workflow smoke run:

```powershell
powershell -File ".\Tools\Validation\invoke_bridge.ps1" -ToolName "run_suction_core_v2_workflow" -ArgsJson '{"description":"fire energy suction orb pulling streaks from all directions into a bright hot core","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/CoreEnergy","system_name":"NS_SuctionCore_V2_Workflow_Smoke","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true}'
```

## Advanced Build: Orb Families V2 Platform

For the generic 6-family V2 platform, use `Docs/Guides/ORB_FAMILIES_V2_PLATFORM.md`.

Run the generic V2 eval harness:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_effect_v2_eval.ps1 -RunSemanticOps -HeroMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01" -CoreMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01"
```

Optional direct generic workflow smoke run:

```powershell
powershell -File ".\Tools\Validation\invoke_bridge.ps1" -ToolName "run_effect_workflow_v2" -ArgsJson '{"description":"explosion orb with a white hot flash core and outward blast streaks","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/V2Platform","system_name":"NS_OrbFamilies_V2_Workflow_Smoke","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true,"keep_assets":false}'
```

## How to run

```powershell
cd <ProjectRoot>\Plugins\ForgePilot
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_smoke_harness.ps1
```

With build:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_smoke_harness.ps1 -RunBuild
```

## Expected phases

- `S0` Core files exist
- `S2` MCP server Python syntax
- `M1` Runtime validation scripts present and syntax-checked
- `M2` Niagara deep-inspect markers present in source
- `M3` Blueprint mutation safety markers present in source
- `M4` Material runtime trace markers present in source
- `M5` Animation-adjacent inspection markers present in source
- `M6` Mutation audit markers present in source
- `X` Validation + PIE tool names present
- `S3` Build check (optional if `-RunBuild` not passed)

## UE runtime follow-up (bridge-based)

Verify the bridge is running in Unreal, then run a runtime probe:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_pie_runtime_probe.ps1 -ScenarioName basic_viewport_runtime
```

Optional gameplay-style probe using the preset sequence:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_pie_runtime_probe.ps1 -ScenarioName transformation_baseline
```

Optional Niagara capability probe using the current bridge session:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_niagara_capability_probe.ps1
```

Optional Niagara live verification for `N7/N10/N11` after reloading the editor session:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_niagara_live_verify.ps1
```

Optional Material live verification for graph authoring, MI workflow, Material Function review, snapshot/restore, and runtime cross-trace:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\run_material_live_verify.ps1
```

Optional Niagara comparison between two probe runs:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\compare_niagara_probe_runs.ps1 `
  -BaselineSummary "C:\Users\ADMIN\Downloads\ForgePilotNiagaraProbes\<baseline>\summary.json" `
  -CandidateSummary "C:\Users\ADMIN\Downloads\ForgePilotNiagaraProbes\<candidate>\summary.json"
```

Then scan the latest editor log:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\check_latest_pie_errors.ps1
```

For visual comparisons between two probe runs:

```powershell
powershell -ExecutionPolicy Bypass -File .\Tools\Validation\compare_probe_captures.ps1 `
  -BaselineSummary "C:\Users\ADMIN\Downloads\ForgePilotRuntimeProbes\<baseline>\summary.json" `
  -CandidateSummary "C:\Users\ADMIN\Downloads\ForgePilotRuntimeProbes\<candidate>\summary.json"
```

Useful bridge tools to spot-check manually:

- `get_context`
- `get_selected_assets`
- `search_assets`
- `trace_asset_relationships`
- `create_asset_folder`
- `duplicate_asset`
- `rename_asset`
- `move_asset`
- `browse_to_asset`
- `delete_asset`
- `start_play_in_viewport`
- `stop_play_session`
- `get_pie_session_state`
- `send_pie_key_input`
- `capture_viewport_screenshot`
- `run_pie_probe_scenario`
- `destroy_actor_by_label`
- `get_mutation_audit_entries`
- Blueprint/material/Niagara/animation review tools already in the plugin

## Blueprint mutation safety notes

Current `M3` coverage checks that Blueprint write tools now expose safety markers in source:

- pre-mutation snapshot hook
- structured `patch_report`
- explicit `BP_SNAPSHOT_FAILED` error path
- snapshot listing and restore hooks

This phase currently covers the main mutate tools in `VFXToolRegistry_Blueprint.cpp`:

- `create_blueprint`
- `add_variable_to_blueprint`
- `add_component_to_blueprint`
- `edit_component_property`
- `get_blueprint_mutation_snapshots`
- `restore_blueprint_mutation_snapshot`

## Material runtime trace notes

Current `M4` coverage checks that the Blueprint-side material runtime trace tool exists and emits the main structured outputs needed for debugging:

- `summarize_blueprint_material_runtime`
- `material_parameter_writes`
- `dynamic_material_nodes`
- `parameter_groups`
- `material_instance_path`
- `material_parameter_nodes`
- `material_instance_overrides`
- `runtime_to_material_matches`

Use it when you need answers like:

- which Blueprint node writes `HotValue`
- where a dynamic material instance is created
- which graphs are driving runtime scalar/vector/texture params
- which MI override maps back to which master-material parameter node

Current `M4` authoring coverage now also includes:

- `get_material_editor_context`
- `set_material_texture_param`
- `create_material_parameter_pack`
- `create_material_comment`
- `update_material_comment`
- `layout_material_expressions`
- `create_material_pattern_block`
- `set_material_custom_node`
- `get_material_function_info`
- `review_material_function`
- `get_material_mutation_snapshots`
- `restore_material_mutation_snapshot`
- `get_material_mutation_audit_entries`
- `run_material_live_verify.ps1`

## Niagara deep-inspect notes

Current `M2` coverage checks that Niagara editor-context lookup and emitter-level deep inspection exist and emit the main structured outputs needed for debugging:

- `get_niagara_editor_context`
- `get_niagara_emitter_details`
- `get_niagara_renderer_details`
- `get_niagara_user_parameter_defaults`
- `get_niagara_actor_user_parameter_values`
- `get_niagara_mutation_snapshots`
- `restore_niagara_mutation_snapshot`
- `cleanup_niagara_audit_assets`
- `get_niagara_emitter_stack`
- `set_niagara_renderer_enabled`
- `set_niagara_renderer_material`
- `set_niagara_renderer_material_user_param`
- `set_niagara_user_parameter_default`
- `set_niagara_system_fixed_bounds`
- `set_niagara_emitter_fixed_bounds`
- `run_niagara_capability_probe.ps1`
- `run_niagara_live_verify.ps1`
- `review_niagara_renderer`
- `review_niagara_emitter`
- `preferred_system_path`
- `selected_systems`
- `opened_systems`
- `event_handler_count`
- `simulation_stage_count`
- `incompatible_renderer_count`
- `system_fixed_bounds`

Use it when you need answers like:

- system Niagara nao dang mo/chon trong editor luc nay
- this emitter is CPU hay GPU
- emitter dang local space hay world space
- renderer nao dang incompatible voi sim target
- renderer nay dang bind material gi, dang enable hay disable
- doi material cua renderer sprite/ribbon/mesh override ngay tu tool de verify nhanh
- bind renderer qua `User.*` material parameter de chuan bi workflow runtime-driven
- tao/set duoc Niagara user parameter kieu `MaterialInterface`, `Texture`, `UObject` cho runtime workflow
- doc duoc default value hien tai cua user params tren Niagara System asset
- doc duoc runtime user parameter values tren actor Niagara da spawn
- set duoc default value tren Niagara System asset truoc khi runtime override
- spawn cleanup actor tam theo label de runtime probe khong de lai rac trong level
- co script probe Niagara de duplicate -> add params -> read defaults -> set defaults -> spawn -> read runtime values -> set runtime params -> cleanup trong 1 lenh
- list/restore duoc snapshot cho Niagara mutation va query rieng audit Niagara
- mo ta duoc stack theo stage cua emitter de ho tro discussion/debug ky thuat
- emitter/system co fixed bounds hay khong
- emitter nao co risk cao va nen sua gi truoc
- renderer nao co risk cao va co nen tat/bat lai
- sua nhanh fixed bounds o system/emitter ngay sau khi review

## Animation inspection notes

Current `M5` coverage checks that animation-adjacent editor context, asset inspection, and review tools exist and emit the main structured outputs needed for debugging:

- `get_animation_editor_context`
- `get_animation_asset_info`
- `review_animation_asset`
- `preferred_asset_path`
- `notify_events`
- `montage_sections`
- `montage_slots`
- `curve_compression_settings`

Use it when you need answers like:

- animation asset nao dang mo/chon trong editor luc nay
- sequence nay co notify nao, track nao, curve nao
- montage nay co section/slot nao
- asset nay co root motion hay rate scale bat thuong khong
- animation nay co phu hop de key gameplay/VFX hay khong

## Mutation audit notes

Current `M6` coverage checks that ForgePilot writes a reusable mutation audit trail and exposes a query tool for recent entries:

- `VFXMutationAudit.cpp`
- `get_mutation_audit_entries`
- `audit_entry_path`
- `audit_recorded`

Current integration is strongest on Blueprint mutation tools, where successful mutate/restore operations now emit:

- `audit_recorded`
- `audit_entry_path`
- the existing `patch_report`

Use it when you need answers like:

- asset nao vua bi ForgePilot sua
- tool nao da cham vao asset do
- snapshot/package file cua lan sua nam o dau
- can soat lai chuoi mutate gan day truoc khi rebuild tiep

## Reporting format

Record each check as:

- `PASS`: verified and healthy
- `FAIL`: broken and must be fixed before handoff
- `BLOCKED`: requires UE interaction or unavailable dependency
