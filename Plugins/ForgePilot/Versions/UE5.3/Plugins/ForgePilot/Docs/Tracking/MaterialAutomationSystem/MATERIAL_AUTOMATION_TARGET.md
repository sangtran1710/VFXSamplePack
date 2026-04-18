# Material Automation System Target

## Purpose
Build a general-purpose material automation system for UE5.3 that lets AI agents take:
- a visual idea
- one or more reference images
- a target material family or material goal

and produce a material branch that is at least directionally correct, structurally clean, and ready for bounded iteration.

This is not an `ice` project or a `lava` project.
Those are only validation cases for the system.

## Main Goal
The real target is:

> Given `idea + reference`, the agent should understand the look, choose the right construction strategy, build the graph, critique the result, iterate across branches when needed, and reach at least `~80%` visual correctness for common material archetypes.

## Non-Goals
- Overfitting the system to one material such as `ice_mass`
- Chasing one-off visual polish without improving the pipeline
- Treating compile success as visual success
- Letting the agent improvise freely without deterministic guardrails

## Success Criteria
The system is considered successful when it can do all of the following on test materials under `/Game/ForgePilot_Test/`:

1. `Understand`
- infer material family reliably from text-first prompts
- extract bounded visual cues from local reference images
- surface cue conflicts instead of hiding them

2. `Plan`
- choose a deterministic lane order for the family
- choose a safe first pass
- choose bounded next steps after critique

3. `Build`
- mutate material graphs cleanly
- materialize the intended root pins for each pass
- keep graph layout readable

4. `Critique`
- inspect preview benchmark captures first
- identify family-specific failure modes
- emit deterministic failure tags and correction guidance

5. `Iterate`
- persist branch/session history
- escalate repeated failures
- fork/restart branches instead of endlessly salvaging dead paths

6. `Output Quality`
- common/stylized or structured materials: `80%+` visual correctness
- harder realistic archetypes: usable and clearly directed, even if not final

## System Scope
This system should support:
- opaque materials
- translucent materials
- emissive materials
- shell/core materials
- crack/fissure materials
- forcefield/shield materials
- dissolve/erosion materials
- wet organic materials
- optical techniques such as Fresnel, depth mask, refraction, and eventually harder techniques such as POM

## Current Phase
We are in:

`Phase 2: Understand + Critique Upgrade`

The goal of this phase is to stop relying only on:
- family recipes
- graph helpers
- compile checks

and move toward:
- image-aware understanding
- family-aware critique
- correction-driven iteration

## Current System Status
### Done
- deterministic workflow engine exists
- session/history exists
- restart/fork lifecycle exists
- top-level workflow entrypoint exists
- preview benchmark is the canonical baseline critic input
- `lava_crack` and `forcefield` have working first passes and next-pass flow
- `analyze_material_reference` is now text-first and image-aware for local images
- `critique_material_iteration` now emits structured family-specific critique fields
- decision/session layers now consume and persist candidate failure tags

### In Progress
- deepen `lava_crack` and `forcefield` family recipes beyond the first critical passes
- improve family-specific correction quality from critique tags
- improve cue interpretation quality from captures

### Deferred Until System Improves
- return to `ice_mass`
- POM-heavy materials
- more optical/translucent realism branches

## Priority Backlog
### P0
- finish `lava_crack` recipe end-to-end
- finish `forcefield` recipe end-to-end
- keep critique/decision aligned with those two families

### P1
- improve reference understanding from local images
- improve family-specific critic signals and corrective recommendations
- keep session summaries and failure history rich and deterministic

### P2
- revisit `ice_mass` with the upgraded understand/critique system
- add stronger handling for hard techniques like `POM`, `refraction`, and layered translucent workflows

### P3
- broader material grammar and more advanced branch search/comparison
- more families and more robust capability mapping

## Required Validation Families
Before reopening `ice_mass`, the following families must work stably through their intended sequence:

### lava_crack
- `crack_mask_lane`
- `shell_core_lane`
- `fresnel_lane`

Expected read:
- dark cooled crust
- hot fissures
- localized emissive, not full-surface glow

### forcefield
- `fresnel_lane`
- `depth_mask_lane`
- `crack_mask_lane`

Expected read:
- thin shell
- controlled opacity
- edge emphasis
- no opaque wall
- no front-view disappearance

## Reopen Criteria For ice_mass
Do not resume `ice_mass` until:
- `lava_crack` and `forcefield` can complete their bounded recipe sequence
- critique emits stable and believable family-specific failure tags
- decision layer reacts deterministically to those tags
- preview benchmark cues are reliable enough to distinguish:
  - shell/core present vs absent
  - opaque wall vs thin shell
  - localized emissive vs full-surface emissive

When `ice_mass` is reopened:
- start from a fresh branch
- stay opaque-first
- no optical trick pass until shell/core + roughness + normal read is already present

## Quality Bar For New Work
Every new material-pipeline change should answer:

1. Does it improve the system, not just one visual?
2. Does it make `understand`, `plan`, `build`, `critique`, or `iterate` stronger?
3. Does it reduce drift, false positives, or endless salvage loops?
4. Does it help the agent reach `~80%` quality on a broader set of materials?

## Next Recommended Work
1. Finish `lava_crack` bounded recipe and confirm critique stays stable through all passes.
2. Finish `forcefield` bounded recipe and confirm critique distinguishes `opaque_wall` from `shell_too_faint`.
3. Strengthen correction mapping before returning to `ice_mass`.
4. Reopen `ice_mass` only after the above three items are complete.
