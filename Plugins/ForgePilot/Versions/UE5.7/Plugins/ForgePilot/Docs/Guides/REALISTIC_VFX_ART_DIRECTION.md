# Realistic VFX Art Direction

## Purpose

This guide captures visual taste rules for realistic VFX authoring.

Use it before building fire, embers, smoke, dust, heat distortion, magic, or any layered effect where the agent must make artistic choices, not only technical node choices.

For non-realistic styles such as painterly, anime, mobile, or casino effects, first read:

- `Docs/Guides/VFX_ART_STYLE_TAXONOMY.md`

The goal is simple:

- Niagara handles readable physical motion.
- Materials handle visual shaping.
- The final effect should feel grounded, layered, and art-directable by a human VFX artist.

## Global Realism Rules

Realistic VFX should feel physically motivated even when it is stylized.

Core rules:

- Effects should have a clear source, force direction, and decay.
- Large shapes move slower than small shapes.
- Hot elements are bright, small, fast, and short-lived.
- Smoke and dust are broad, soft, slower, and longer-lived.
- Secondary elements should follow the main event with slight delay.
- Randomness should break repetition, not destroy the readable silhouette.
- Do not make every layer equally bright, equally fast, or equally dense.

Avoid:

- particles flying upward forever
- perfectly uniform spawn timing
- same-size particles everywhere
- same color over the whole lifetime
- overusing emissive bloom on smoke
- making embers too large or too slow to read as sparks
- making smoke fully opaque
- hiding basic artist controls inside custom scripts

## Layering Model

A grounded effect usually needs several layers with different jobs.

Typical fire stack:

- core flame body
- outer soft flame
- embers
- smoke
- heat distortion
- ground glow or contact light

Typical impact stack:

- instant flash
- debris or sparks
- dust/smoke puff
- lingering smoke
- decal or contact mark

Typical realistic magic stack:

- readable source or core
- aura/glow
- particles or motes
- trails/ribbons
- distortion/refraction
- impact or contact response

Do not ask one emitter to do every job. It becomes hard to tune and visually muddy.

## Fire

Realistic fire should feel hot, unstable, and buoyant, but not like a rocket plume unless the source is pressurized.

Visual read:

- hot core is near white/yellow
- body is orange
- outer edge shifts red-orange or red-brown
- lower source is denser and brighter
- upper flame thins out quickly
- flame should flicker and roll, not rise as a straight column

Niagara motion:

- upward velocity should be moderate
- horizontal noise should be enough to break symmetry
- drag should prevent particles from flying too high
- curl noise should create rolling motion
- lifetime should stay short for flame cards
- sprite size should vary per particle

Good starting ranges:

- Lifetime: `0.35-0.9s`
- Spawn Rate: `20-80/sec`
- Upward Velocity: `80-220 cm/s`
- Horizontal Velocity Random: `10-50 cm/s`
- Curl Noise Strength: `10-35`
- Drag: `0.8-2.0`
- Sprite Size Random: `0.6x-1.4x`
- Color variation: warm yellow-orange-red, never random rainbow for natural fire

Art notes:

- Fire should usually not travel too high above its source.
- If it rises too high, increase drag, reduce lifetime, reduce upward velocity, or make opacity erode faster.
- Keep the strongest brightness near the base or inner body.
- Use material erosion/fade to break the flame shape instead of only fading alpha linearly.

Common failures:

- too tall and floaty
- too uniform in size
- pure orange with no hot center
- alpha fades as a flat rectangle
- additive material makes fade controls feel broken

## Embers

Embers should feel like tiny hot particles ejected irregularly from fire.

Visual read:

- very bright
- small
- sharp or compact
- short bright life with longer dim fade
- irregular bursts, not perfectly even rain
- color shifts from white/yellow-orange to dim red-orange

Niagara motion:

- random burst or uneven spawn rate
- upward velocity with mild horizontal drift
- curl noise or wind to make them wander
- drag to slow them over time
- occasional faster particles for sparkle
- gravity can be slight or near zero depending on heat lift

Good starting ranges:

- Spawn Rate: `2-15/sec` for steady fire accents
- Burst Count: `3-12` for intermittent sparkle bursts
- Burst Interval: irregular, roughly `0.2-1.2s`
- Lifetime: `0.7-2.5s`
- Size: `1-5 cm`
- Size Random: `0.4x-1.8x`
- Upward Velocity: `80-260 cm/s`
- Horizontal Velocity Random: `20-90 cm/s`
- Curl Noise Strength: `5-25`
- Drag: `0.4-1.5`
- Emissive: strong enough to bloom, but not so large that particles become blobs

Art notes:

- Embers should not be too fast. They need time to sparkle and drift.
- Use random bursts for life. Continuous perfectly even embers look procedural.
- Keep most embers small. A few larger embers are okay as accents.
- Embers should be visibly brighter than smoke and often brighter than outer fire.

Common failures:

- embers read as orange dust because they are too dim
- embers read as fireflies because they float too slowly and evenly
- embers read as bullets because velocity is too high and straight
- all embers have identical size and lifetime

## Smoke

Realistic smoke is soft, broad, translucent, and slower than fire.

Visual read:

- starts tighter and warmer near the source
- expands as it rises
- cools and desaturates over life
- never fully opaque except in special heavy smoke
- large puffs move slower than embers and flame details
- silhouettes should billow, not pop as hard circles

Niagara motion:

- steady spawn rate for continuous smoke
- random size and lifetime
- upward velocity higher near hot fire, slower as smoke expands
- curl noise is mandatory for billowing
- drag prevents smoke from shooting upward
- wind can lean the whole plume

Good starting ranges:

- Spawn Rate: `5-25/sec`
- Lifetime: `2-6s`
- Initial Size: `30-100 cm`
- End Size: `2x-4x`
- Upward Velocity: `50-180 cm/s`
- Horizontal Velocity Random: `10-60 cm/s`
- Curl Noise Strength: `20-70`
- Curl Noise Frequency: `0.5-2.0`
- Drag: `0.2-0.8`
- Peak Opacity: `0.08-0.35`
- Color variation: warm gray/brown near source, cooler gray over life

Art notes:

- Smoke can rise higher than fire because it lingers.
- Rate should usually feel even for a continuous source.
- Variation should come from size, color, curl, and opacity, not chaotic spawn gaps unless the source is sputtering.
- Bigger smoke particles should generally be softer and less opaque.

Common failures:

- smoke looks like solid gray balls
- smoke is too emissive
- smoke does not grow over life
- smoke moves as fast as sparks
- no curl noise, so the plume looks dead

## Heat Distortion

Heat distortion is a realism layer, not the main effect.

Visual read:

- subtle shimmer above hot areas
- strongest near fire source
- fades quickly with height
- should be almost invisible until seen against background edges

Good starting ranges:

- Lifetime: `0.4-1.5s`
- Spawn Rate: `2-10/sec`
- Size: `40-150 cm`
- Upward Velocity: `60-180 cm/s`
- Distortion Strength: `0.01-0.08`
- Opacity or mask intensity: very low

Common failures:

- distortion is too strong and looks like water
- distortion covers the whole screen
- distortion particles are too visible as cards

## Dust And Debris

Dust should feel heavier and more grounded than smoke.

Visual read:

- starts near impact/contact point
- expands outward and upward briefly
- falls or settles sooner than smoke
- color should match the environment
- opacity fades faster than lingering smoke

Good starting ranges:

- Burst Count: `8-40`
- Lifetime: `0.6-2.5s`
- Initial Size: `10-50 cm`
- End Size: `2x-5x`
- Radial Velocity: `50-220 cm/s`
- Upward Velocity: `20-120 cm/s`
- Gravity: mild downward
- Drag: `0.5-2.0`
- Peak Opacity: `0.1-0.35`

Common failures:

- dust rises like smoke from heat
- dust color does not match surface
- particles are too uniform and spherical

## Realistic Magic

Magic can be non-physical, but it still needs internal rules.

Visual read:

- define an element palette first
- define a source, target, orbit, or flow direction
- make the motion intentional rather than random
- combine controlled geometry with organic particles
- use glow sparingly enough that shape remains readable

Element notes:

- Fire magic: warm, fast, turbulent, ember accents, heat distortion.
- Ice magic: cool, crisp, crystalline, slower mist, sharp highlights.
- Lightning magic: fast, irregular, high contrast, brief persistence.
- Holy magic: warm white/gold, smoother motion, soft rays, clean silhouettes.
- Dark magic: purple/black/green accents, tendrils, inward pull, smoky residue.
- Arcane magic: purple/pink/blue, geometric rings, motes, controlled orbit.
- Nature magic: green/gold, leaf/debris shapes, upward organic drift.
- Water magic: blue/teal, splash arcs, mist, ripples, refraction.

Good realistic-magic structure:

- core source
- aura glow
- secondary motes
- trail or ribbon
- contact/impact response
- optional distortion

Common failures:

- every magic effect becomes generic glowing particles
- colors mix without element logic
- no anticipation before cast
- no secondary residue after release
- motion is random instead of designed

## Human And AI Responsibilities

AI should set up:

- emitter layers
- safe default ranges
- material controls
- Dynamic Parameter mappings
- renderer bindings
- validation checks

Human artist should tune:

- exact spawn rate
- exact size ranges
- color taste
- curve timing
- silhouette density
- motion feel
- final brightness and bloom response

AI should not overfit one screenshot by hiding important controls in hardcoded scripts. Keep controls visible and boring until the baseline reads well.

## Realism Checklist

Before reporting an effect as visually acceptable, check:

- Does every layer have a clear job?
- Is the source of motion understandable?
- Are small particles faster or sharper than large particles?
- Are size, velocity, lifetime, color, and opacity randomized?
- Does the effect have a readable color temperature story?
- Does the opacity curve avoid flat cards?
- Does smoke stay translucent?
- Are embers bright, small, and irregular?
- Does fire stay near the source unless the prompt asks for a plume?
- Is curl noise used for smoke and organic flame motion?
- Are bursts irregular where nature should feel chaotic?
- Are continuous rates stable where the source should be steady?
- Can a human artist tune the result without editing a Scratch Script?
