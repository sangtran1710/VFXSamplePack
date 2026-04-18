# Niagara Curve Logic Patterns

## Purpose

Use this guide when shaping Niagara behavior with curves, dynamic inputs, or shared random indices.

This is not a button-by-button tutorial. It is the decision layer behind a good Niagara setup:

- what time domain should drive the curve
- when randomness should be independent
- when randomness should be linked
- how to bias distribution toward background or hero particles
- how to keep artist iteration fast without destroying the shape logic

For Dynamic Parameter packing and the Niagara-to-Material contract, see `Docs/Guides/DYNAMIC_PARAMETER_WORKFLOW.md`.

## Core Principle

Do not think of a curve as "animation data only."

In Niagara, a curve is usually one of three things:

- a time function
- a probability remap
- a correlation rule

Senior-level authoring comes from choosing the right role first, then wiring the curve.

## 1. Time Domain Selection

The first question is not "what should the curve look like?" It is "whose time is this?"

### Particle Time

Use `Particles.NormalizedAge` when each particle should run its own life cycle.

Best for:

- smoke fade
- ember cooling
- flame erosion
- sprite size over life
- alpha over life
- per-particle distortion rise/fall

Effect:

- each particle starts the curve when it spawns
- the system feels organic and naturally staggered
- overlap between particles creates texture and richness

### Emitter Time

Use `Emitter.NormalizedLoopAge` when the whole emitter should breathe together.

Best for:

- pulse waves
- slot or iGaming reward flashes
- rhythmic glow cycles
- shield hum or charging beats
- synchronized bursts where all particles should brighten or dim together

Effect:

- particles read the same timing phase
- the emitter behaves like one coordinated instrument
- easier to art direct for cadence-driven effects

### Rule Of Thumb

If the shot needs natural breakup, prefer particle time.

If the shot needs rhythm, ceremony, or readable beats, prefer emitter time.

## 2. Non-Uniform Distribution

Plain `Random Range Float` gives a uniform distribution. That is useful, but it often feels too democratic.

Real effects usually need a hierarchy:

- many support particles
- fewer accent particles
- rare hero particles

Use `Float from Curve` as a probability remap:

- index input: random 0-1
- output: biased value distribution

### What The Curve Means

If the curve stays low for most of the range and rises late:

- most particles get low values
- only a small fraction get high values

That is ideal for:

- a few fast sparks among many slow sparks
- rare bright smoke puffs among many soft ones
- a small number of long streaks inside a larger field of short motion

If the curve climbs early and flattens:

- many particles get mid-to-high values
- the effect feels fuller and less selective

### Use Cases

Good targets for biased distribution:

- velocity magnitude
- emissive multiplier
- sprite size
- curl noise strength
- erosion delay
- distortion amount
- DP-driven edge intensity

### Authoring Intent

Uniform random says "everything has equal chance."

Curved random says "this effect has a class structure."

That is usually closer to production art direction.

## 3. Correlation And Linked Randomness

This is the pattern that makes an effect feel designed instead of merely noisy.

Do not let important properties randomize independently unless that independence is visually useful.

If size, velocity, color, emissive, and erosion all roll separate random values, the effect often loses internal logic.

Instead:

1. Generate one shared random scalar at spawn, usually 0-1.
2. Store it in a particle attribute or named scratch/local value.
3. Use that same scalar as the index for multiple curves.

That shared scalar becomes the particle's identity.

### Example

One random index can define a full behavior family:

- low index -> small, fast, dim, cool
- high index -> large, slow, bright, hot

This gives you correlation without hardcoding one static look.

### Good Correlated Pairings

- `Size` with `Velocity`
- `Size` with `Opacity`
- `Size` with `Color temperature`
- `Brightness` with `Distortion`
- `Erode delay` with `Lifetime`
- `Hero brightness` with `Spawn burst offset`

### Design Value

Linked randomness is how you create:

- believable "heavy vs light" particles
- clear primary/secondary read
- reusable emitter behavior with a consistent personality

This is one of the strongest patterns for reusable Niagara modules and effect families.

## 4. Time And Correlation Together

The strongest setups often combine both systems:

- one shared random index for particle identity
- one time index for behavior over life

Example:

- particle identity controls size, base brightness, and motion class
- normalized age controls fade, erosion, and softening

That gives you:

- stable per-particle personality
- readable temporal evolution

The effect stops feeling like a pile of disconnected modules and starts behaving like a designed system.

## 5. Dynamic Parameter Implications

When a material uses Dynamic Parameters, Niagara curves become part of the shading logic, not just particle motion.

Good uses:

- `Particles.NormalizedAge -> Float from Curve -> DP.ErodeAmount`
- `Particles.NormalizedAge -> Float from Curve -> DP.OpacityMult`
- `Random 0-1 -> Float from Curve -> DP.EdgeIntensity`
- `Emitter.NormalizedLoopAge -> Float from Curve -> DP.EmissivePulse`

This is especially useful when:

- alpha must be shaped in the material instead of Niagara color
- erosion should be curve-driven
- smoke should pulse in sync with gameplay timing
- one emitter needs a shared beat but particles still keep local motion

Rule:

Use Niagara curves to drive material behavior when the visual change is fundamentally shading-side.

Do not push everything into Material Instances if the value truly varies per particle or per emitter phase.

## 6. Senior Pattern Library

### Organic Smoke

- time domain: `Particles.NormalizedAge`
- random bias: size and drag biased toward modest values
- linked randomness: larger puffs rise slower and hold opacity longer
- material bridge: DP for erosion, opacity multiplier, edge softness

### Pulse Energy

- time domain: `Emitter.NormalizedLoopAge`
- random bias: modest particle variation, synchronized brightness
- linked randomness: larger sprites can receive slightly later fade or lower frequency breakup
- material bridge: DP emissive pulse and threshold shaping

### Hero Spark Layer

- time domain: particle age
- random bias: strong remap so only a few particles reach top speed and brightness
- linked randomness: fastest particles are also longest and brightest
- result: readable accents over a calmer base layer

### iGaming Reward Burst

- time domain: emitter loop age for beat sync
- random bias: a few oversized, high-value particles for celebration accents
- linked randomness: brightest particles also use wider sparkle, larger scale, and stronger trail contribution
- result: coordinated excitement instead of random clutter

## 7. Curve Editor Iteration Trick

When editing keys in the Niagara curve editor, direct math entry is often faster than manual dragging.

Typical examples:

- `*1.5`
- `+50`
- `/2`

Use this when you want to:

- push intensity up without changing the overall shape
- lower an entire section cleanly
- preserve timing while scaling amplitude

This is a human iteration-speed trick, but it matters for ForgePilot workflow too:

- the agent should preserve authored curve shape when possible
- follow-up automation should avoid flattening or rebuilding artist-tuned curves unnecessarily

## 8. Automation Guidance For ForgePilot

When ForgePilot authors or edits Niagara curves, prefer these decisions:

1. Use native Niagara `Float from Curve` for float inputs whenever possible.
2. Use `Particles.NormalizedAge` for default natural behavior.
3. Switch to `Emitter.NormalizedLoopAge` only when the prompt implies pulse, sync, cycle, reward rhythm, or coordinated beats.
4. Prefer probability-remap curves over raw uniform random when the prompt implies hero accents, rare spikes, or primary/secondary layering.
5. Prefer linked random indices when multiple properties should feel intentionally related.
6. Avoid independent randomization across all properties by default.
7. Preserve artist-authored curve shapes unless the user explicitly asks for a rebuild.

## 9. Validation Questions

Before calling a Niagara curve setup "done," ask:

1. Is the curve driven by the correct time domain?
2. Should this variation be uniform, biased, or linked?
3. Are hero particles intentionally rare, or accidentally random?
4. Do multiple properties belong to the same particle identity?
5. Is the curve solving timing, probability, or correlation?
6. If this drives a material, is Niagara controlling the right shading-side parameter?

If these answers are unclear, the graph may still work, but the effect will usually feel generic.
