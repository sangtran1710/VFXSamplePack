# Niagara Authoring Tooling V2 Plan

## Goal
Rebuild Niagara tooling around the same workflow a senior FX artist would use in the editor:

1. Create an empty Niagara system
2. Add an empty emitter
3. Author the emitter stack from scratch
4. Put modules in the correct stage and order
5. Expose important controls through Emitter Summary
6. Compile, verify, and only then promote the asset

This replaces the previous regression-only direction that duplicated an existing Niagara system such as `NS_Smoke_1` and edited it in place.

## Why V2 Is Needed
The previous tooling pass proved that stack mutation works, but it also exposed a workflow problem:

- Duplicating an existing Niagara system pulls in inherited modules and hidden assumptions
- Template-specific items such as `Set (PARTICLES) UVScale` survive into the new asset
- Module dependency warnings become harder to reason about
- The resulting system is valid for tooling regression, but it is not a clean authored Niagara effect

The new direction must produce Niagara assets that are readable, clean, and intentionally built.

## Core Authoring Rules

### Rule 1: No more system-first duplication for authored work
Do not use `duplicate_niagara_system` as the default path for new authored effects.

Allowed uses of duplication:
- migration
- forensic debugging
- regression testing

Not allowed as the default for authored Niagara:
- smoke
- embers
- flame
- burst
- hand aura
- production candidate FX

### Rule 2: Author from an empty system
The default authored path must be:

1. `create_empty_niagara_system`
2. `add_empty_niagara_emitter`
3. add modules intentionally by stage
4. set values
5. expose summary controls
6. compile and validate

### Rule 3: Stage discipline matters
Tooling must treat Niagara as four meaningful sections:

- `Emitter Spawn`
- `Emitter Update`
- `Particle Spawn`
- `Particle Update`
- plus `Render`

The stack must be authored top-to-bottom with valid dependency order.

### Rule 4: Emitter Summary is required
Authored Niagara assets should expose the important knobs through `Emitter Summary` so another artist or client can tweak the effect without digging through the full stack.

## V2 Tooling Scope

### A. System creation tools
These are the highest-priority missing tools.

#### `create_empty_niagara_system`
Create a brand new empty Niagara system at a target content path.

Input:
- `save_path`
- `system_name`
- optional `loop_behavior`
- optional `loop_duration`

Output:
- created system path
- compile state

#### `add_empty_niagara_emitter`
Add a clean empty emitter into an existing Niagara system.

Input:
- `system_path`
- `emitter_name`
- optional `simulation_target` (`cpu` default, `gpu` optional later)
- optional `life_cycle_mode` (`self` or `system`)
- optional `loop_duration`

Output:
- emitter name
- emitter handle id if available
- compile state

### B. Stack authoring tools
These tools build the stack intentionally instead of inheriting template clutter.

#### `add_niagara_module_to_stage`
Already exists conceptually and should remain the core primitive, but V2 must tighten it:
- add by exact script asset
- add to an explicit stage
- insert at an explicit index when requested
- return actual stage and actual insertion index

#### `move_niagara_module_in_stage`
Keep it, but position it as a cleanup/reorder tool, not the primary authoring tool.

#### `remove_niagara_module_from_stage`
Keep it for cleanup and corrective authoring.

### C. Input editing tools
These remain valuable and should stay:

- `get_niagara_module_inputs`
- `set_niagara_module_input_constant`
- `set_niagara_module_input_curve`
- `set_niagara_module_input_dynamic`
- `reset_niagara_module_input`

But V2 must add one more authoring-oriented layer:

#### `set_niagara_module_display_name`
Rename module label for artist readability when needed.

#### `set_niagara_input_display_name`
Optional tool to improve artist-facing exposed labels in summary-driven workflows.

### D. Emitter Summary tools
This is the biggest missing authoring feature and is required for V2.

#### `show_niagara_input_in_emitter_summary`
Expose one module input to the Emitter Summary.

Input:
- `system_path`
- `emitter_name`
- `module_key`
- `input_key`
- optional `summary_category`
- optional `sort_order`

#### `hide_niagara_input_from_emitter_summary`
Remove one input from summary.

#### `set_niagara_summary_display_name`
Give the exposed summary entry a clean artist-facing name.

Examples:
- `Spawn Count`
- `Initial Velocity Cone`
- `Smoke Drag`
- `Smoke Darkness`
- `Glow Amount`

### E. Stack hygiene tools
These are needed to keep authoring clean.

#### `validate_niagara_stack_dependencies`
Inspect the stack and report:
- unmet dependencies
- outdated script versions
- missing solver modules
- module order issues
- duplicate standard modules

#### `fix_niagara_stack_dependencies`
Apply safe automatic fixes for common problems:
- ensure `Solve Forces and Velocity` is at the bottom of `Particle Update`
- ensure force modules sit before the solver
- optionally accept script upgrades

#### `normalize_niagara_stage_order`
For a known standard set of modules, reorder into sane senior-FX order.

## Standard Module Policy
V2 should explicitly support common sprite FX modules as first-class citizens.

### Emitter Update
- `Emitter State`
- `Spawn Rate`
- later: `Spawn Burst Instantaneous`

### Particle Spawn
- `Initialize Particle`
- `Shape Location`
- `Add Velocity`
- `Add Velocity in Cone`
- `Dynamic Material Parameters`
- optional authored setup modules such as `Set Variables`

### Particle Update
- `Particle State`
- `Drag`
- `Gravity Force`
- `Wind Force`
- `Curl Noise Force`
- `Scale Color`
- `Color`
- `Sub UV Animation`
- `Solve Forces and Velocity`
- optional `Camera Offset`

### Render
- `Sprite Renderer`

## Tooling Behavior Changes

### 1. Create, do not inherit, for authored Niagara
When the task is “make a new effect,” the agent should prefer:
- `create_empty_niagara_system`
- `add_empty_niagara_emitter`
- module-by-module authoring

It should only duplicate an existing Niagara system if the user explicitly asks to reuse or adapt a source effect.

### 2. Compile must stay strict
`compile_niagara_system` must continue to fail on:
- real compile errors
- invalid script diagnostics
- broken stack dependencies that produce compile failure

### 3. Warnings must be visible
Even when compile is green, tooling should surface:
- unmet module dependencies
- outdated script versions
- inherited template clutter

Compile green is necessary, but not sufficient, for “clean.”

## Validation Strategy

### Sandbox asset for V2
Create a brand new authored-from-empty sandbox asset:

- `/Game/ForgePilot_Test/Niagara/ToolingSandbox/NS_ForgePilot_SmokeBurst_FromScratch`

This asset must not come from `NS_Smoke_1` or any existing VFX pack system.

### Validation target effect
Use a simple smoke burst because it exercises:
- spawn
- lifetime
- size
- velocity
- cone velocity
- drag
- gravity
- curl
- color/opacity
- summary controls

### Validation steps
1. Create empty system
2. Add empty emitter
3. Set emitter life cycle
4. Add standard modules intentionally
5. Set values through tooling
6. Expose summary controls
7. Compile
8. Run dependency validation
9. Reopen asset
10. Compile again
11. Confirm no inherited junk modules exist

### Pass criteria
The V2 sandbox only passes if:
- compile is green
- stack dependency validation is clean or only has accepted upgrade notes
- no inherited template modules exist unless intentionally added
- stage order is readable
- Emitter Summary exposes the main controls

## Implementation Phases

### Phase 1: Empty authoring foundation
Build:
- `create_empty_niagara_system`
- `add_empty_niagara_emitter`
- stronger `add_niagara_module_to_stage`

Done when:
- a new empty smoke burst system can be created without duplicating `NS_Smoke_1`

### Phase 2: Stack hygiene and dependency control
Build:
- `validate_niagara_stack_dependencies`
- `fix_niagara_stack_dependencies`
- `normalize_niagara_stage_order`

Done when:
- force/solver ordering can be auto-validated and fixed

### Phase 3: Emitter Summary authoring
Build:
- `show_niagara_input_in_emitter_summary`
- `hide_niagara_input_from_emitter_summary`
- `set_niagara_summary_display_name`

Done when:
- important controls are available in summary with artist-friendly names

### Phase 4: Validation runner V2
Replace the current template-duplicate regression runner with a new one that:
- creates the asset from empty
- authors it entirely through tooling
- validates compile, dependency order, and summary exposure

## Anti-Goals
This V2 pass should not:
- build HellFire
- tune production assets tied to one specific game or character set
- focus on GPU fluids or data-interface-heavy systems
- duplicate marketplace Niagara systems as the default authoring path

## Immediate Next Task
The next Niagara task should be:

1. implement `create_empty_niagara_system`
2. implement `add_empty_niagara_emitter`
3. create `NS_ForgePilot_SmokeBurst_FromScratch`
4. add only the minimum sprite-smoke modules from scratch
5. expose 3-5 controls in `Emitter Summary`

That will be the first true proof that the tooling now matches the senior-FX workflow you want.
