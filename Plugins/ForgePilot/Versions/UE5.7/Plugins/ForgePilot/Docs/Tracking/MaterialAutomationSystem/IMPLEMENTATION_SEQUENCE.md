# Implementation Sequence

## Purpose
Define the build order for `ForgePilot` so the system grows in a stable, verifiable way.

This file is the master build brief for AI implementers.

It answers:
- what to build first
- what to leave alone
- what must be verified before moving on

## Core Rule
Do not build the most visually impressive thing first.

Build the system in the order that preserves:
- reliability
- determinism
- critique quality
- restartability
- collaboration value

## Current Situation
Right now:
- `Material` is the strongest lane
- `Niagara` now has a bounded workflow engine with critique/session/restart and decomposition
- `Blueprint` is not the primary value lane

Therefore the implementation order must keep reinforcing:
1. `Material`
2. `Niagara`
3. `Material + Niagara orchestration`

## Phase Order
### Phase 1: Harden Material
Goal:
- make Material more reliable on hard-but-common cases

Focus areas:
- improve image-aware understanding
- improve critique quality
- improve correction mapping
- improve support for harder techniques:
  - translucency
  - refraction
  - distortion
  - panner/flow
  - POM

Do not:
- chase final hero `ice_mass` yet
- brute-force hard optical visuals without critique support

Exit criteria:
- Material first-pass direction improves on difficult cases
- family-specific critique becomes more actionable
- hard failure tags are more useful and less generic

### Phase 2: Build Niagara Workflow Foundation
Goal:
- turn Niagara from a technical editor lane into a workflow lane

Focus areas:
- Niagara analyze/plan/build structure
- workflow entrypoint
- structured outputs
- validation on repeatable effect families

Do not:
- jump straight into hero spell effects
- treat random emitter-edit helpers as sufficient progress

Exit criteria:
- there is a real Niagara workflow path, not only low-level edits
- at least one bounded Niagara flow works on validation families

### Phase 3: Add Niagara Critique + Session + Restart
Goal:
- give Niagara the same control-loop maturity Material already has

Focus areas:
- critique
- decision logic
- session/history
- restart/fork behavior

Exit criteria:
- failed Niagara directions can be retried or restarted cleanly
- critique produces bounded revision logic

### Phase 4: Improve Niagara Effect Decomposition
Goal:
- turn reference understanding into safer bounded emitter/layer planning

Focus areas:
- bounded decomposition inside Niagara workflow outputs
- primary/supporting layer identification
- material-vs-Niagara layer hints for later combined work
- deterministic heuristics on validation families

Exit criteria:
- Niagara decomposition materially improves first-pass layer choice on validation families
- decomposition can safely inform later combined orchestration

### Phase 5: Material + Niagara Combined Workflow
Goal:
- let one effect brief drive both lanes together

Focus areas:
- effect decomposition as shared source of truth
- material layer selection
- Niagara layer selection
- combined validation
- branch comparison

Exit criteria:
- one bounded effect family can run through both lanes coherently

### Phase 6: Return To Hard Hero Cases
Goal:
- revisit difficult hero references only after the system is strong enough

Examples:
- `ice_mass`
- complex water shells
- optical-heavy shields
- advanced fire/lava vortexes

Exit criteria:
- Material and Niagara both have mature critique/restart behavior
- combined workflow exists
- difficult cases fail intelligently instead of collapsing chaotically

## Validation Families By Phase
### Phase 1 Material validation
- `lava_crack`
- `forcefield`
- `stylized_water_ribbon`
- `projectile_material`

### Phase 2-3 Niagara validation
- `projectile`
- `impact`
- `shield`
- `ribbon`
- `smoke_trail`

### Phase 5 combined validation
- `energy_projectile`
- `shield_hit`
- `lava_burst`
- `magic_ribbon_spell`

Only after these are working should the system revisit:
- `ice_mass`
- multi-layer hero spell effects

## Required Gate At End Of Every Phase
Before moving to the next phase:
1. the relevant workflow builds cleanly
2. bridge verification succeeds
3. the lane has a clear status improvement
4. tracking docs are updated
5. non-regression checks pass

If those conditions are not met, the phase is not done.

## What Not To Do
- do not let flashy hero references drive architecture too early
- do not over-expand Blueprint
- do not stack unrelated changes in one patch
- do not skip critique and restart support in favor of more editing power

## Practical Next Step
The immediate next step after this planning state is:
1. harden decomposition and second-pass quality on the bounded validation families
2. harden the combined Material + Niagara loop across more than one pass
3. do not return to hero references until those refinements verify cleanly

That is the current best path.

## Bottom Line
This sequence exists to ensure `ForgePilot` becomes:
- a stable Material co-pilot first
- then a stable Niagara co-pilot
- then a combined VFX collaborator

That order is intentional and should not be casually bypassed.
