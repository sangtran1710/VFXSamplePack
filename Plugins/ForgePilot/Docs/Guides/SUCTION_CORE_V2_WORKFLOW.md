# Suction Core V2 Workflow

This is the canonical production path for the `suction_core_energy` family.

Use it for:
- suction orb
- infernal pull orb
- plasma gather into hot core

Do not use it for:
- non-suction families
- low-level tool debugging
- generic Niagara experiments

## Required entrypoints

- `resolve_effect_plan_v2`
- `run_suction_core_v2_workflow`
- `review_suction_core_failure_taxonomy`

For this family, prefer the workflow runner over hand-assembled raw tool sequences.

## Expected layer model

- `MainAttractionStreaks`
- `SecondaryFilaments`
- `CoreTurbulence`
- `Sparks`
- `ShockGlow`

Expected read:
- Layer A is the dominant outside-to-core attraction sweep
- Layer B is thinner support filament motion
- Layer C carries dense hot-core turbulence
- Layer D adds sparks and embers
- Layer E binds the core with a controlled glow

## Hard rebuild rule

Layer A has a hard rebuild gate.

If the critic returns any Layer A hard-gate failure such as:
- `orbit_dominant`
- `no_outside_to_core_read`
- `hero_sweep_missing`
- `cage_silhouette_risk`

for two consecutive iterations, the workflow must:
- activate the rebuild gate
- block scalar-only Layer A polish
- schedule `rebuild_main_attraction_layer`

## Validation path

Build first:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/build_unreal_with_recovery.ps1"
```

Verify the bridge:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -Ping
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ListTools
```

Resolve the V2 plan:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "resolve_effect_plan_v2" -ArgsJson '{"description":"fire energy suction orb pulling streaks from all directions into a bright hot core"}'
```

Run the workflow:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "run_suction_core_v2_workflow" -ArgsJson '{"description":"fire energy suction orb pulling streaks from all directions into a bright hot core","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/CoreEnergy","system_name":"NS_SuctionCore_V2_Workflow_Guide","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true}'
```

Run the eval harness:

```powershell
powershell -ExecutionPolicy Bypass -File ".\Tools\Validation\run_suction_core_v2_eval.ps1" -RunSemanticOps -HeroMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01" -CoreMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01"
```

## Production hygiene rules

- no parent emitters
- no copy-from-old-effect workflow
- no custom-expression fallback in semantic V2 ops
- compile/log clean required
- use raw Niagara/material tools for inspection, debugging, and targeted follow-up edits only

## Expected outputs and artifacts

The workflow should return or create:
- created Niagara system path
- created hero material instance path
- created core material instance path
- critic output
- repair decision
- thumbnail artifact path

The eval harness writes reports under:

```text
Saved/ForgePilot/SuctionCoreV2Eval/<timestamp>/report.json
```
