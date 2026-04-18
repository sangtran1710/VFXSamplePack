# System Guardrails

## Purpose
Define the non-negotiable rules that must remain true while `ForgePilot` is being extended.

This file exists to prevent:
- accidental architecture drift
- endless patching without structure
- regressions that make the system less reliable even when features increase

## Core Rule
Do not optimize for "one more feature" if it weakens:
- determinism
- restartability
- traceability
- session integrity
- workflow clarity

## Current Source of Truth
The system is built around this pipeline:
1. `analyze_material_reference`
2. `run_material_lookdev_iteration`
3. `critique_material_iteration`
4. `decide_material_next_step`
5. session/history tools
6. `run_material_reference_workflow`

Anything added should strengthen this loop, not bypass it.

## Invariants
### 1. Persisted session family is authoritative
- analysis may disagree
- critique may suggest strategy shifts
- but a session must not silently change family mid-branch

### 2. Preview benchmark is the primary visual checkpoint
- world probe is secondary validation only
- do not use noisy world capture as the canonical truth for pass success

### 3. A failed branch must not be endlessly salvaged
- failed directions must be:
  - retried in a bounded way
  - shifted deterministically
  - restarted when required

### 4. The system must stay deterministic
- avoid freeform AI improvisation in orchestration
- prefer bounded rules, explicit family logic, and structured outputs

### 5. New tools must integrate with the existing workflow
- do not add side-channel tools that skip:
  - analysis
  - critique
  - decision
  - session tracking

### 6. Material and Niagara are the priority lanes
- Blueprint remains secondary unless it directly unblocks VFX production value

## What Must Not Happen
- session ids silently creating new sessions
- branch history being lost on restart
- analysis outputs bypassing persisted branch truth
- generic graph edits that do not route through family-aware workflow
- image-based claims without bounded evidence
- "hero visual" hacks that do not improve the reusable system

## Safe Extension Pattern
When adding capability:
1. define where it fits in the pipeline
2. define what subsystem owns it
3. define what existing output it enriches
4. define what regression it could cause
5. define how it is verified

If any of those is unclear, the change is not ready.

## Priority Order For New Work
1. strengthen `Material`
2. bring `Niagara` up to workflow-engine maturity
3. connect `Material + Niagara`
4. only then broaden into harder hero cases

## Anti-Drift Rule
If a change does not improve at least one of these, it should be questioned:
- quality of first pass
- quality of critique
- safety of iteration
- restart/fork reliability
- reference understanding
- coordination between lanes

## Bottom Line
The system should evolve as a workflow engine, not as a pile of disconnected power tools.
