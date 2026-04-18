# Effect Decomposition Plan

## Purpose
Define how `ForgePilot` should break a visual effect reference into structured technical layers before attempting to build anything.

This file exists because hero-quality VFX is almost never:
- one material
- one emitter
- one graph

It is usually a layered composition.

## Core Rule
Do not let the system jump directly from:
- `reference`

to:
- `graph mutation`

without first producing a decomposition plan.

## What Decomposition Means
Effect decomposition is the step where a reference is translated into layered technical intent.

The system should identify likely layers such as:
- `core element`
- `shell`
- `atmospheric layer`
- `dynamic accents`
- `secondary particles`
- `environment interaction`
- `timing hierarchy`

Not every effect has every layer, but every serious effect should be analyzed through this lens.

## Standard Layer Vocabulary
### 1. Core Element
The main readable shape or focal body.

Examples:
- projectile body
- fireball core
- energy orb center
- shield bubble shell

Usually solved with:
- mesh or sprite
- primary material
- clear silhouette read

### 2. Shell
The outer containment, edge definition, or readable boundary.

Examples:
- forcefield boundary
- water bubble shell
- lava crust edge
- magic aura border

Usually solved with:
- fresnel
- opacity shaping
- shell/core separation
- edge-emphasis material logic

### 3. Atmospheric Layer
The soft volume that gives presence and scale.

Examples:
- smoke
- mist
- glow haze
- fog volume

Usually solved with:
- sprite particles
- soft alpha breakup
- low-frequency motion

### 4. Dynamic Accents
High-frequency details that provide energy and readability.

Examples:
- sparks
- streaks
- rays
- embers
- small splashes

Usually solved with:
- Niagara secondary emitters
- emissive accents
- ribbon or sprite bursts

### 5. Secondary Particles
Support particles that reinforce the main read without taking over.

Examples:
- droplets
- ash
- magical motes
- debris fragments

### 6. Environment Interaction
The part of the effect that grounds it in the world.

Examples:
- decals
- ground ring
- distortion against scene depth
- contact smoke
- impact splash on floor

### 7. Timing Hierarchy
The temporal structure of the effect.

Examples:
- buildup
- peak
- dissipation
- lingering residue

This is not a material or emitter layer, but it is critical to planning.

## Material vs Niagara Split
For each identified layer, the system should decide whether it is primarily:
- `Material-driven`
- `Niagara-driven`
- `Shared`

### Mostly Material-driven
Examples:
- shell/core separation
- distortion
- fresnel edge logic
- POM or surface breakup
- emissive pattern shaping

### Mostly Niagara-driven
Examples:
- sparks
- trails
- mist
- bursts
- projectile motion

### Shared
Examples:
- ribbon effect with custom flowing material
- shield shell with Niagara shell particles and material edge logic
- lava effect with material crust and Niagara embers/smoke

## Decomposition Output Requirements
Every future decomposition output should eventually include:
- `effect_family`
- `dominant_read`
- `layer_list`
- `primary_layer`
- `supporting_layers`
- `material_layers`
- `niagara_layers`
- `environment_layers`
- `timing_notes`
- `risk_flags`

## Common Failure Modes If Decomposition Is Skipped
- forcing too much into one material
- building too many emitters without hierarchy
- making the effect visually noisy
- failing to establish a clear focal read
- overusing glow to hide weak structure

## Initial Validation Families
Decomposition should first be tested on:
- `projectile`
- `impact`
- `forcefield`
- `lava_crack`
- `stylized_water_ribbon`

Only after this is stable should it drive:
- `ice_mass`
- complex vortexes
- multi-stage hero spell effects

## Short-Term Build Goal
The immediate implementation target is not "perfect decomposition."

It is:
- enough structured decomposition to choose safer Material and Niagara build strategies

## Long-Term Goal
Effect decomposition becomes the shared source of truth for:
- Material workflow
- Niagara workflow
- critique expectations
- branch comparison

## Bottom Line
If `ForgePilot` cannot decompose an effect, it will keep trying to solve hero VFX as disconnected node edits.

Decomposition is the step that turns raw references into buildable technical intent.
