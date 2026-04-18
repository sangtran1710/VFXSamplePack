# Niagara Pattern: Spread Spark Burst

## Purpose
Reusable note for future VFX work that needs:
- spark bursts
- ember pops
- magic particle sprays
- short-lived radial fallout
- "burst then fade" support particles around a hero core effect

This note was extracted from visual inspection of:
- `/Game/VFX/VFX-Combat/FlameThrowerBreath/_GenericSource/NE/NE_spread_spark.NE_spread_spark`

## Why This Pattern Matters
This emitter is not trying to be a hero layer.
It is a clean support-emitter pattern for:
- quick radial breakup
- readable spark spread
- low-complexity secondary motion

This is exactly the kind of emitter ForgePilot should learn as a reusable building block for:
- embers around fire
- spark spray around impacts
- magical debris around energy cores
- transform fallout around hands, weapons, and spell centers

## High-Level Pattern
The reusable structure is:

1. `Emitter-level one-shot timing`
2. `Burst + low sustained spawn`
3. `Spawn from a small spherical region`
4. `Cone velocity for spread direction`
5. `Force shaping in update`
6. `Color fade over life`
7. `Sprite size response to speed`
8. `Velocity-aligned sprite rendering`

This is the important part to preserve.

## What The Emitter Appears To Do
Observed from editor stack:

### Emitter Update
- `Emitter State`
- `Spawn Rate`
- `Spawn Burst Instantaneous`

### Particle Spawn
- `Set PARTICLES MySize`
- `Initialize Particle 001`
- `Initialize Particle`
- `Sphere Location 001`
- `Sphere Location`
- `Add Velocity in Cone`

### Particle Update
- `Update Age`
- `Gravity Force`
- `Drag`
- `Scale Color`
- `Solve Forces and Velocity`
- `Sprite Size Scale by Speed`

### Render
- `Sprite Renderer`
- material shown: `DefaultSpriteMaterial`
- alignment: `Velocity Aligned`
- facing mode: `Face Camera`
- sort mode: `View Distance`

## Reusable Block 1: One-Shot Emitter Timing
Observed signal:
- `Emitter State` set to a self-contained one-shot behavior
- paired with `Spawn Burst Instantaneous`

Reusable lesson:
- for sparks and embers, one-shot timing should live at the emitter level
- do not fake every burst using only an always-on spawn rate

Useful for:
- hand burst accents
- impact sparks
- orb pulse fallout
- spell release particles

Recommended rule:
- use `Emitter State` with explicit one-shot intent
- add a burst module for the visible release moment

## Reusable Block 2: Burst + Residual Spawn
Observed signal:
- both `Spawn Burst Instantaneous` and `Spawn Rate` are present

Reusable lesson:
- this is a strong pattern for support particles:
  - burst gives the readable first pop
  - low-rate spawn keeps the effect alive briefly after the burst

Useful for:
- embers around fireballs
- magical burst residue
- transform ignition particles

Design note:
- if the effect should feel explosive, bias toward burst
- if it should feel smoldering or sustained, bias toward spawn rate

## Reusable Block 3: Small-Volume Spawn Region
Observed signal:
- two `Sphere Location` modules are present

Reusable lesson:
- spreading sparks from a small spherical region avoids a dead-looking single-point origin
- layering multiple sphere-location passes can help break up uniformity

Useful for:
- energy cores
- palm-center emitters
- impact centers
- torch/fire mouth sources

Guidance:
- keep the spawn region compact
- let velocity create the spread, not giant initial placement radii

## Reusable Block 4: Cone Velocity Spread
Observed signal:
- `Add Velocity in Cone`

Reusable lesson:
- for sparks and embers, cone-based spread is usually more controllable than raw random velocity
- this gives a readable outward flow while preserving an overall directional bias if needed

Useful for:
- radial bursts
- directional spell sprays
- forward muzzle sparks
- palm-origin magical fallout

Design note:
- wide cone for radial breakup
- narrower cone for directed bursts

## Reusable Block 5: Force Shaping
Observed signal:
- `Gravity Force`
- `Drag`
- `Solve Forces and Velocity`

Reusable lesson:
- the spark feel comes from update shaping, not only spawn
- gravity adds believable drop
- drag kills excessive travel and keeps the burst local

Useful for:
- embers that arc down after release
- sparks that slow out quickly
- magical motes that should not fly forever

Recommended pattern:
- low-to-medium drag
- light gravity unless the effect should feel heavy
- keep solver after force modules

## Reusable Block 6: Color Fade
Observed signal:
- `Scale Color`

Reusable lesson:
- support sparks rarely need complex color logic
- a clean color-over-life fade is usually enough

Useful for:
- hot-to-dim ember falloff
- magic spark fading
- short particle readability cleanup

Design note:
- use Niagara color over life first
- let material stay simple and reusable

## Reusable Block 7: Size Response To Speed
Observed signal:
- `Sprite Size Scale by Speed`

Reusable lesson:
- this is a strong trick for sparks
- faster particles read as longer/brighter streaks without needing a different renderer type

Useful for:
- sparks
- fast embers
- magical streak motes

Important note:
- this is especially valuable when using a simple sprite material
- it helps turn plain dots into convincing moving spark marks

## Reusable Block 8: Velocity-Aligned Sprite Rendering
Observed signal:
- renderer alignment is `Velocity Aligned`
- facing mode still respects camera readability

Reusable lesson:
- for sparks, velocity alignment is one of the cheapest quality wins
- it makes small sprites feel directional without moving to ribbons

Useful for:
- spark streaks
- tiny magical shards
- ember trails without actual trail renderers

## Why This Is Good Knowledge For ForgePilot
This emitter is a good default reference because it is:
- simple
- readable
- reusable
- modular
- appropriate as a support layer

It teaches the correct mindset for sparks:
- not too many modules
- not hero-level complexity
- motion clarity over graph complexity

## Good Use Cases
Strong fit:
- ember burst around a fire shell
- spark spray around arc impacts
- hand-transform fallout particles
- magical explosion support particles
- impact debris glow motes

Partial fit:
- sustained fireplace embers
- stylized magic dust

Not a good fit by itself:
- hero flame body
- smoke volume
- plasma shell
- ribbon lightning arcs

## Mapping Guidance For Future Effects
For embers:
- keep burst moderate
- increase drag
- use warmer color fade
- keep spawn region compact

For sparks:
- increase initial speed
- keep sprites velocity aligned
- use stronger size-by-speed response
- lighter drag than embers

For magic particles:
- lower gravity
- use color fade instead of realistic falloff
- optionally reduce cone spread if the motion should orbit or spiral

## Suggested Reusable Emitter Template Extraction
Future reusable emitter templates for ForgePilot should include:

1. `NE_SparkBurst_Base`
2. `NE_EmberBurst_Base`
3. `NE_MagicSpray_Base`

These should preserve:
- one-shot emitter state
- burst + optional low sustain
- sphere-location spawn
- cone velocity
- drag + gravity + solver
- scale color
- sprite size by speed
- velocity-aligned sprite renderer

## Authoring Guidance For Future Agents
When building spark or ember emitters:
- start from this support-emitter pattern
- do not jump to ribbons unless the motion truly needs long continuous arcs
- use a simple particle material first
- let Niagara handle spread, forces, and timing
- treat this as a secondary layer, not the hero silhouette

For ForgePilot specifically:
- this should become a recommended base pattern whenever the prompt includes:
  - sparks
  - embers
  - particle spray
  - magical fallout
  - burst debris

## Key Takeaway
The reusable knowledge is not just "this emitter throws yellow streaks."

The reusable knowledge is:
- sparks read well when burst timing, compact spawn, and velocity shaping work together
- embers and sparks can share the same structural emitter pattern
- velocity-aligned sprites plus size-by-speed are a cheap high-value combination
- support emitters should stay modular and lightweight

That makes this a strong reference pattern for future ember, spark, and magic particle emitters.
