# ForgePilot Multi-Project Strategy

## Purpose

ForgePilot is not a one-project helper.

It is a growing Unreal automation and knowledge system with a strong visual-effect lane, intended to become useful across many Unreal Engine projects, art directions, teams, and engine versions.

The current plugin implementation is validated against the active host-project target:

- Unreal Engine `5.7`
- Win64 Development
- `<DetectedProjectEditorTarget>`

But the knowledge base and authoring philosophy should be written so they can evolve toward newer Unreal versions, including future production targets such as UE `5.7`, without trapping the agent in one project's assumptions.

## Product Direction

ForgePilot should grow in layers:

1. Project-safe fundamentals.
2. Repeatable material and Niagara workflows.
3. Style-aware art direction.
4. Better validation and diagnostics.
5. Smarter use of Unreal version-specific features.
6. Advanced custom motion, scratch scripts, and optimization.

Do not try to make the agent clever by skipping the fundamentals.

The best long-term agent is boringly reliable first, then increasingly expressive.

## Multi-Project Rule

Never assume one project style is universal.

Every visual-effect task should identify:

- project context
- engine version
- target platform
- art style
- performance budget
- required fidelity
- available textures/materials
- whether the effect is gameplay-critical or cosmetic

If the user does not specify these, infer conservatively and state the assumption.

Default assumptions for the current repo sandbox:

- engine: UE `5.7`
- style: project-defined; if unknown, start with grounded realistic assumptions for fire/smoke/dust/water/explosion
- target: desktop/Win64
- workflow: use safe stock Niagara modules and validated material graphs first

## Engine Version Policy

Current C++ and bridge tooling should remain compatible with the active project target unless the user explicitly asks for another engine branch.

Knowledge docs should be version-aware:

- mark UE `5.3`-specific workflows clearly
- avoid claiming a node/tool exists in all versions
- prefer stable engine concepts over version-fragile shortcuts
- note when a feature may differ in newer engine versions
- isolate future-version notes from current production instructions

For future engine versions:

- do not assume API names are identical
- re-probe tool support before generating assets
- re-run validation harnesses
- update material node/function catalogs
- update Niagara module docs
- preserve older UE `5.3` recipes until the team explicitly retires the legacy branch

## Style-Aware Knowledge

The agent must separate effect type from art style.

Example:

- fire can be AAA realistic, stylized, anime, mobile, or casino
- smoke can be realistic volumetric-looking smoke or stylized graphic puffs
- magic can be painterly, anime, realistic supernatural, or casino reward-like

Before authoring visual assets, load:

- `Docs/Guides/VFX_ART_STYLE_TAXONOMY.md`
- `Docs/Guides/REALISTIC_VFX_ART_DIRECTION.md` when the selected style is realistic or physically grounded
- the relevant effect recipe under `Knowledge/Effects/`
- the relevant material technique docs under `Knowledge/Materials/Techniques/`

## Human And AI Collaboration Model

Human artist owns:

- taste
- art direction
- reference judgement
- final curves
- color balance
- timing feel
- density and silhouette
- project-specific constraints

AI owns:

- scaffolding
- first-pass setup
- repetitive wiring
- docs lookup
- validation gates
- diagnostics
- safe defaults
- iteration support
- cleanup and reproducibility

AI should make the artist faster, not replace the artist's eye.

## Niagara And Material Responsibility Split

Use Niagara for physical and timing controls:

- spawn
- random size
- random velocity
- speed
- drag
- wind
- curl noise
- collision
- lifetime
- scale size
- sprite rotation
- renderer setup
- SubUV playback

Use materials and Dynamic Parameters for visual shaping:

- fade in/out
- erosion
- dissolve
- material shape
- noise breakup
- distortion
- refraction
- emissive shaping
- color ramps
- alpha thresholding
- style-specific edge treatment

Use Niagara Scratch Scripts later, after ordinary module workflows are stable or when a style explicitly needs complex custom motion.

## Knowledge Growth Rule

Every time a task teaches a reusable lesson, write it down.

Good knowledge entries include:

- when to use the workflow
- when not to use it
- expected material contract
- expected Niagara contract
- style assumptions
- engine version assumptions
- validation commands
- common failures
- recovery path

Bad knowledge entries:

- one-off notes that only describe what happened
- claims without validation
- hardcoded project-only asset paths with no explanation
- hidden assumptions about art style
- instructions that require a specific open editor layout

## Validation Culture

Do not say an asset is done because it exists.

For visual VFX, completion should include:

- material compile diagnostics clean
- Niagara system compiles
- renderer material binding verified
- relevant parameters visible or documented
- screenshot or viewport capture when practical
- style checklist reviewed
- known limitations stated

If validation tooling is missing, improve the tooling before trusting repeated manual guessing.

## Long-Term Roadmap

Phase 1:

- stable material graph creation
- stable Niagara module setup
- compile diagnostics
- clear docs and recipes
- style-aware defaults

Phase 2:

- reusable master materials
- reusable Niagara archetypes
- material instance preview modes
- validation probes for common nodes and effects
- RealTimeVFX research summaries per effect family

Phase 3:

- controlled Niagara Scratch Scripts
- optimized packed material functions
- version-aware engine feature maps
- higher-level art direction critics
- automated screenshot comparison
- project profile presets

Phase 4:

- multi-project style libraries
- engine-version adapters
- platform/performance profiles
- advanced procedural motion systems
- production-ready visual QA loops

## Agent Rule

When the user teaches a reusable principle, preserve it in docs or knowledge.

Prefer compact, durable guidance over long reasoning in chat.

The agent should become smarter over time because the project remembers what worked.
