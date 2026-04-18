# End-to-End Visual Material Loop

## Purpose
Define the full system needed for `ForgePilot` to take:
- one or more reference images
- an optional text goal
- a target material archetype

and run a bounded, inspectable loop from analysis to build to critique to correction.

This document is not a prompt note.
It is the system contract for making the visual material pipeline work end-to-end.

## Target Flow
```text
[ Reference Image ]
        ↓
[ Visual Analyzer ]
        ↓
[ Visual Plan (semantic) ]
        ↓
[ Material Builder ]
        ↓
[ Render / Probe Capture ]
        ↓
[ Visual Critic (semantic) ]
        ↓
[ Fix Loop ]
```

## What "End-to-End" Means
The system is only considered end-to-end when all of the following are true:

1. `Reference Understanding`
- reads the reference image as structured visual intent
- identifies material archetype and layer decomposition
- distinguishes core shape from shell, breakup, rim, and color identity

2. `Semantic Planning`
- converts visual intent into deterministic build stages
- chooses the right graph lane before mutating anything
- prefers structural fixes before parameter-only tweaks when structure is wrong

3. `Graph Construction`
- builds or repairs the material graph in Unreal through MCP
- keeps graph readable and stage-scoped
- saves changes safely with snapshot support

4. `Visual Measurement`
- renders the material through a stable probe
- captures consistent images for comparison
- preserves per-stage and per-iteration artifacts

5. `Semantic Critique`
- compares current output against the reference
- scores correctness by axes, not only one scalar
- identifies the actual failure class, not just generic "improve X"

6. `Bounded Correction Loop`
- chooses the next correction type deterministically
- avoids repeating ineffective actions
- stops when blocked, regressing, or complete

## Core Design Rule
Do not let the system jump directly from:
- `reference image`

to:
- `random graph mutation`

without producing:
- a semantic decomposition
- a material plan
- a bounded stage or fix target

## Non-Goals
- fully autonomous "art director" behavior
- infinite optimization loops
- black-box graph improvisation
- compile-pass-only validation
- overfitting the system to one orb or one material

## System Layers

### 1. Visual Analyzer
The analyzer converts a reference into structured semantic intent.

It should answer:
- what archetype is this material
- what are the dominant visual layers
- what is the lane order
- what is the likely construction strategy
- what are the critical non-negotiable reads

Expected outputs:
- `archetype`
- `layer stack`
- `visual cues`
- `dominant read`
- `stage plan`
- `failure-sensitive constraints`

For the energy orb case, a good analyzer output would look like:
- `archetype = emissive energy orb`
- `core = white-hot center`
- `body = orange plasma volume`
- `breakup = thin bright filament lines`
- `secondary depth = softer internal turbulence`
- `rim = subtle edge accent`
- `color identity = hot white center -> orange body`
- `do_not = cloudy white sphere, flat tint, thick white outline`

### 2. Visual Plan (Semantic)
The plan layer translates analyzer output into deterministic build stages.

The plan is not graph code yet.
It is a constrained semantic recipe.

Required properties:
- deterministic stage order
- one stage goal at a time
- allowed node families per stage
- explicit stop conditions
- stage-specific validation rules

Example stage plan for the energy orb:
1. core mask
2. primary filament breakup
3. secondary turbulence
4. rim accent
5. heat gradient / color identity

### 3. Material Builder
The builder executes the semantic plan in Unreal.

Responsibilities:
- create material or clone baseline
- add expressions
- connect nodes
- set parameters
- save assets
- capture snapshots before mutating

Builder constraints:
- mutate only the current stage
- preserve approved prior stages
- stop immediately on compile failure or wrong-stage outcome
- emit graph inventory after each stage

### 4. Render / Probe Capture
This layer turns the current material into a stable image.

Requirements:
- stable probe mesh
- stable framing
- stable background
- consistent capture path
- deterministic naming

Artifacts required:
- `stage_N.png` for staged builds
- `iter_N_pre.png`
- `iter_N_post.png`

### 5. Visual Critic
The critic compares the current material against the reference and emits semantic failure data.

It should output:
- `overall_score`
- `axis_scores`
- `summary`
- `top_fixes`
- `failure_tags`
- `directional deltas`

Required critic behavior:
- image-aware
- semantic, not just textual
- JSON-only
- schema-strict
- raw response stored

### 6. Fix Loop
The fix loop chooses the next bounded correction.

It must decide between:
- `continue`
- `change stage`
- `repair current stage`
- `rollback`
- `stop`

The fix loop should not always choose parameter tuning.
It must know when the failure is:
- structural
- shaping-related
- color-only
- saturation-related
- capture-related

## Required Data Contracts

### Analyzer Output
Minimum fields:
- `archetype`
- `material_family`
- `layer_stack`
- `dominant_read`
- `secondary_reads`
- `stage_recipe`
- `hard_constraints`
- `anti_patterns`

### Semantic Plan Output
Minimum fields:
- `stage_id`
- `goal`
- `allowed_ops`
- `forbidden_ops`
- `validation_rules`
- `expected_visual_change`

### Critic Output
Already largely present in the current system:
- `summary`
- `overall_score`
- `axis_scores`
- `top_fixes`
- `deltas`

### Fix Decision Output
Minimum fields:
- `failure_class`
- `target_stage`
- `target_lane`
- `target_nodes_or_params`
- `change_type`
- `expected_effect`
- `risk`

## Failure Taxonomy
The fix loop must classify failure before changing the graph.

### Structural Failures
Examples:
- no core
- no shell
- no readable breakup lane
- missing rim branch

Action:
- rebuild or repair structure
- do not waste time on parameter tweaks

### Shaping Failures
Examples:
- noise too smooth
- filaments too broad
- rim too thick
- gradient too soft

Action:
- retune math shaping
- preserve graph structure

### Color Failures
Examples:
- mostly white
- flat orange
- no hot center
- no warm body separation

Action:
- retune color parameters first
- if ineffective, mark integration bottleneck

### Saturation / Response Failures
Examples:
- emissive clipping hides color
- vector tint changes not reading
- edge already saturated

Action:
- avoid repeated ineffective parameter moves
- prefer lower-luminance outer colors or different target lanes

### Probe / Capture Failures
Examples:
- framing drift
- wrong material on actor
- thumbnail fallback hiding differences

Action:
- fix render path before graph mutation

## Current System Status

### Already Working
- Unreal MCP bridge for real material mutation
- material graph expression add/connect/set operations
- snapshot and restore
- probe capture
- real vision critic path
- planner and orchestrator skeleton
- deterministic stage-by-stage manual build workflow

### Partially Working
- critic-to-plan conversion
- semantic fix ranking
- output-path-aware target resolution
- lane coordination
- limited real closed-loop iteration

### Missing or Insufficient
- a real `Visual Analyzer` that emits archetype + layer decomposition from reference
- a real `Semantic Visual Plan` that drives staged material construction
- structure-aware fix decisions
- graph-topology correction policies for when the material is "the wrong kind" of graph
- explicit failure taxonomy in the fix loop
- capture-aware quality gates

## What Blocked the Energy Orb Match
The current orb exercise exposed the real system gap clearly.

The issue is not:
- bridge
- compile
- snapshot
- probe capture
- basic graph mutation

The issue is:
- the system can build a clean orb graph
- but it does not yet know how to transform a soft cloudy breakup into thin plasma filaments
- and it does not yet know how to escalate from parameter tuning to structural breakup repair

In short:
- current system is good at `repairing an existing lane`
- current system is not yet good at `recognizing the wrong graph topology for the reference`

## Required Upgrade Path

### Phase 1 - Analyzer Layer
Add a dedicated `analyze_material_reference_visual()` step that produces:
- archetype
- layer stack
- lane order
- dominant read
- anti-patterns

Acceptance:
- given the energy orb reference, it must explicitly call out `thin plasma filaments` and reject `cloudy white orb`

### Phase 2 - Semantic Plan Layer
Add `build_material_visual_plan()` that turns analyzer output into:
- deterministic stage recipe
- stage goals
- allowed node families
- validation targets

Acceptance:
- plan says `Stage 2 = filament breakup`, not generic `add noise`

### Phase 3 - Failure Classification
Add a fix decision layer that classifies failures into:
- structure
- shaping
- color
- saturation
- capture

Acceptance:
- if output is still "cloudy", system must request breakup repair, not more color tuning

### Phase 4 - Structure-Aware Builder Policies
Add builder policies for:
- branch replacement within one stage
- targeted lane rebuild
- preserve-approved-stage rule

Acceptance:
- can rebuild Stage 2 without disturbing Stages 1, 3, 4

### Phase 5 - Critic-to-Fix Alignment
Improve critic prompts and planner interpretation so "filament readability" maps to:
- thresholded/high-contrast breakup repair
- not generic brightness/noise adjustments

### Phase 6 - End-to-End Bounded Loop
Only after the above are stable:
- enable closed correction loops for material archetypes
- keep hard stop conditions
- keep rollback

## End-to-End Done Criteria
The system is considered end-to-end for material visuals when it can do all of the following:

1. ingest a local reference image
2. emit a usable semantic decomposition
3. emit a deterministic semantic stage plan
4. build a first-pass graph in Unreal
5. render a stable probe
6. critique the result semantically
7. classify the failure correctly
8. choose either:
   - parameter repair
   - shaping repair
   - lane rebuild
9. converge or stop safely with artifacts

## Minimum Done Criteria for the Energy Orb Archetype
For the energy orb class, the system should be able to produce:
- centered white-hot core
- readable filament breakup
- secondary internal depth
- subtle rim
- warm orange outer body
- preserved breakup readability after color

And it must reject these outcomes:
- cloudy white sphere
- flat orange ball
- thick white outline
- color-only fixes on structurally wrong breakup

## Immediate Next Work
This is the shortest path to a real end-to-end system:

1. build `Visual Analyzer` for material references
2. add `semantic stage recipe` output
3. add `failure taxonomy` to critic/fix loop
4. add `stage-scoped rebuild policy`
5. validate on one archetype only:
   - `Energy Orb`

Do not expand to more archetypes until the orb loop can:
- detect wrong breakup type
- choose Stage 2 rebuild
- preserve approved stages
- recover color identity after structural correction

## Implementation Rule
For this system, "visual intelligence" must mean:
- structured decomposition
- bounded semantic planning
- stage-aware repair

It must not mean:
- freeform random mutation
- endless tuning
- art-direction guesses without graph reasoning

## Final Statement
The current plugin has enough real runtime plumbing to support this end-to-end system.

What is missing is not the Unreal bridge.
What is missing is the semantic middle:
- `Visual Analyzer`
- `Semantic Visual Plan`
- `Failure Classifier`
- `Stage-Scoped Fix Logic`

That is the gap between:
- a working toolchain

and:
- a usable end-to-end visual material system.
