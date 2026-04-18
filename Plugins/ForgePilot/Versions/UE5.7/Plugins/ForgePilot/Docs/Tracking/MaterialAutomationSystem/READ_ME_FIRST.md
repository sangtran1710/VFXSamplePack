# Read Me First

## Purpose
This folder is the planning and control center for the `ForgePilot` material automation system.

It is now intentionally material-focused.
The active scope is:
- reference understanding
- semantic visual planning
- material graph build
- render / probe capture
- semantic critique
- bounded fix loop

## Recommended Reading Order
### 1. Start with the material system target
- [MATERIAL_AUTOMATION_TARGET.md](./MATERIAL_AUTOMATION_TARGET.md)

Read this first to understand:
- what the material automation system is trying to become
- what success looks like
- what is intentionally not the focus right now

### 2. Read the end-to-end loop spec
- [END_TO_END_VISUAL_MATERIAL_LOOP.md](./END_TO_END_VISUAL_MATERIAL_LOOP.md)

Read this to understand:
- the full target flow from reference image to fix loop
- what already exists
- what semantic layers are still missing

### 3. Read decomposition rules
- [EFFECT_DECOMPOSITION_PLAN.md](./EFFECT_DECOMPOSITION_PLAN.md)

Read this to understand:
- how visual references should be decomposed before graph mutation
- which layer vocabulary the system should use

### 4. Read build order and invariants
- [IMPLEMENTATION_SEQUENCE.md](./IMPLEMENTATION_SEQUENCE.md)
- [SYSTEM_GUARDRAILS.md](./SYSTEM_GUARDRAILS.md)

Read these to understand:
- what to build next
- what must not be broken while extending the system

### 5. Read quality standards
- [DEFINITION_OF_DONE.md](./DEFINITION_OF_DONE.md)
- [CHANGE_PROTOCOL.md](./CHANGE_PROTOCOL.md)
- [PRODUCTION_HYGIENE_STANDARD.md](./PRODUCTION_HYGIENE_STANDARD.md)
- [PERFORMANCE_VALIDATION_STANDARD.md](./PERFORMANCE_VALIDATION_STANDARD.md)

Read these to understand:
- what counts as done
- how changes should be introduced
- what quality, hygiene, and validation still apply

## Practical Reading Path
For most current work, use this order:
1. `MATERIAL_AUTOMATION_TARGET`
2. `END_TO_END_VISUAL_MATERIAL_LOOP`
3. `EFFECT_DECOMPOSITION_PLAN`
4. `IMPLEMENTATION_SEQUENCE`
5. `SYSTEM_GUARDRAILS`

## Current Priority
The active priority is:
1. strengthen the material visual pipeline end-to-end
2. add the missing semantic middle
3. keep the Unreal runtime path deterministic and inspectable

## Key Rule
Do not chase isolated flashy results at the cost of system integrity.

Every major change should make the workflow:
- clearer
- safer
- more deterministic
- more useful in real collaboration

## Bottom Line
This folder should stay small, current, and decision-driving.

If a file no longer changes implementation decisions, merge it or delete it.
