# Orb Families V2 Platform

This is the generic V2 production path for the orb-centric families covered in this milestone.

Supported archetypes:
- `suction_core_energy`
- `electric_arc_orb`
- `hero_fire_orb_sweep`
- `arcane_pull_orb`
- `impact_pulse_core`
- `explosion_flash_core`

Use this path for:
- suction orb / pull-to-core energy orb
- electric arc orb with charged filaments snapping into a hot center
- hero fire orb with a dominant sweep around a burning core
- arcane pull orb with magical wisps feeding a bright core
- impact pulse core with a sharp burst read and bright impact flash
- explosion flash core with a white-hot detonation center and outward blast streaks

Do not use it for:
- non-orb families outside this milestone
- low-level tool debugging
- generic Niagara experiments

## Required entrypoints

- `resolve_effect_plan_v2`
- `run_effect_workflow_v2`
- `review_effect_failure_taxonomy_v2`

Compatibility/example wrappers still exist for suction-core:
- `run_suction_core_v2_workflow`
- `review_suction_core_failure_taxonomy`

## Supported layer models

### `suction_core_energy`
- `MainAttractionStreaks`
- `SecondaryFilaments`
- `CoreTurbulence`
- `Sparks`
- `ShockGlow`

### `hero_fire_orb_sweep`
- `HeroFireSweep`
- `SecondaryFireFilaments`
- `CoreFlameMass`
- `Embers`
- `BindGlow`

### `electric_arc_orb`
- `HeroElectricArc`
- `SecondaryElectricFilaments`
- `CoreElectricMass`
- `SparkBursts`
- `BindGlow`

### `arcane_pull_orb`
- `HeroArcaneSweep`
- `SecondaryArcaneFilaments`
- `CoreArcaneMass`
- `SparkMotes`
- `BindGlow`

### `impact_pulse_core`
- `HeroImpactBurst`
- `SecondaryImpactStreaks`
- `CoreImpactFlash`
- `DebrisSparks`
- `BindGlow`

### `explosion_flash_core`
- `HeroExplosionBurst`
- `SecondaryBlastStreaks`
- `CoreFlashMass`
- `DebrisSparks`
- `ShockGlow`

## Critic philosophy

The V2 critic is rules-based and proxy-based, not screenshot-judging.

It checks:
- compile/log cleanliness
- emitter presence and renderer family
- stage/module grammar
- spawn topology
- force mix
- size/width/lifetime ratios
- layer dominance proxies
- workflow-authored material-family hints

Expected critic outputs:
- `archetype`
- `finding_count`
- `failure_labels`
- `findings`
- `repair_decision`
- `layer_proxy_metrics`
- `hard_gate_state`

## Hard gate behavior

Each archetype has a hero-layer hard rebuild gate.

If the hero layer fails motion-family review for two consecutive iterations, the workflow must:
- activate the hard rebuild gate
- block scalar-only polish on the hero layer
- schedule the archetype-specific rebuild op

Examples:
- `suction_core_energy` -> `rebuild_main_attraction_layer`
- `electric_arc_orb` -> `rebuild_hero_electric_arc_layer`
- `hero_fire_orb_sweep` -> `rebuild_hero_fire_sweep_layer`
- `arcane_pull_orb` -> `rebuild_hero_arcane_sweep_layer`
- `impact_pulse_core` -> `rebuild_hero_impact_burst_layer`
- `explosion_flash_core` -> `rebuild_hero_explosion_burst_layer`

## Production hygiene rules

- no parent emitters
- no copy-from-old-effect workflow
- no custom-expression fallback in semantic V2 ops
- compile/log clean required
- use raw Niagara/material tools for inspection, debugging, and targeted follow-up edits only

## Validation path

Build first:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/build_unreal_with_recovery.ps1"
```

Verify bridge connectivity:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -Ping
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ListTools
```

Resolve a V2 plan:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "resolve_effect_plan_v2" -ArgsJson '{"description":"explosion orb with a white hot flash core and outward blast streaks"}'
```

Run the generic V2 workflow:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "run_effect_workflow_v2" -ArgsJson '{"description":"explosion orb with a white hot flash core and outward blast streaks","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/V2Platform","system_name":"NS_OrbFamilies_V2_Workflow_Guide","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true,"keep_assets":false}'
```

Run the generic eval harness:

```powershell
powershell -ExecutionPolicy Bypass -File ".\Plugins\ForgePilot\Tools\Validation\run_effect_v2_eval.ps1" -RunSemanticOps -HeroMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01" -CoreMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01"
```

Run the suction compatibility eval harness:

```powershell
powershell -ExecutionPolicy Bypass -File ".\Plugins\ForgePilot\Tools\Validation\run_suction_core_v2_eval.ps1" -RunSemanticOps -HeroMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01" -CoreMaterialPath "/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01"
```

## Expected outputs and artifacts

The workflow should return or create:
- created Niagara system path
- created hero material instance path
- created core material instance path
- critic output
- repair decision
- thumbnail artifact path

The generic eval harness writes reports under:

```text
Saved/ForgePilot/EffectV2Eval/<timestamp>/report.json
```

The suction compatibility harness writes reports under:

```text
Saved/ForgePilot/SuctionCoreV2Eval/<timestamp>/report.json
```
