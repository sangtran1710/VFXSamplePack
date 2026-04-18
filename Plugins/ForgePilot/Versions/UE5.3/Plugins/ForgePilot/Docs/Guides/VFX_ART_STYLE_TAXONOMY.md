# VFX Art Style Taxonomy

## Purpose

Use this guide before creating or judging a VFX effect.

Technical correctness is not enough. A Niagara system can compile cleanly and still look wrong if the agent chooses the wrong art style. This document defines the major VFX style families ForgePilot should understand before authoring materials, Niagara systems, timing curves, and validation notes.

Default project assumption:

- Use `AAA Realistic` when the user asks for realism, believable fire, smoke, dust, explosion, impact, water, environmental effects, or does not specify a stylized direction.
- Switch style only when the user asks for stylized, anime, mobile, casino, magical, painterly, or a named reference.

## Style Selection Rule

Before building an effect, state the intended style:

- `AAA Realistic`
- `Stylized / Painterly`
- `Anime / Cel-Shaded`
- `Mobile Stylized`
- `iGaming / Casino`

Then adapt:

- texture choice
- material model
- alpha treatment
- emissive intensity
- timing
- shape language
- Niagara module choices
- post-process/bloom expectations
- validation criteria

Do not mix style rules accidentally. For example, realistic smoke should not use casino-level emissive, and anime slashes should not be judged by physically realistic smoke softness.

## AAA Realistic

Goal:

- immersion
- physical plausibility
- environmental integration
- believable energy, weight, and decay

References:

- The Last of Us Part II
- Call of Duty
- Cyberpunk 2077

Texture language:

- photo-sourced elements
- simulated flipbooks
- high-quality noise
- volumetric-looking masks
- soft alpha gradients

Material language:

- PBR where appropriate
- translucent lit smoke when affordable
- volumetric or volume-like shading
- depth fade and camera fade
- refraction/scattering for water, glass, heat, and mist
- restrained emissive except for genuinely hot or energetic elements

Motion language:

- inertia matters
- large masses move slower
- effects linger after the main event
- smoke and dust decay slowly
- particles are influenced by wind, drag, collision, buoyancy, and gravity

Shape language:

- amorphous
- irregular
- environment-dependent
- soft intersections
- no obvious repeated cards

Color language:

- physically motivated color temperature
- not overly saturated
- fire follows hot white/yellow to orange/red/dark
- smoke cools/desaturates over life
- effect light should influence the scene when possible through Lumen, local lights, or baked supporting glow

Niagara focus:

- random size and lifetime
- drag
- wind and curl noise
- collision
- steady continuous rates for sustained sources
- burst plus lingering secondary emitters for impacts/explosions

Material focus:

- DepthFade
- soft particles
- luma/alpha contrast shaping
- volume-style opacity
- subtle distortion/refraction
- Blackbody-style fire color ramps when relevant

Common failures:

- too saturated
- too fast and weightless
- no lingering smoke
- hard card intersections
- smoke too opaque
- fire climbs too high without a pressurized source

## Stylized / Painterly

Goal:

- clear artistic intention
- strong silhouettes
- appealing hand-authored shapes
- readable motion beats over physical accuracy

References:

- Elden Ring
- God of War magic attacks
- Diablo IV

Texture language:

- hand-painted flipbooks
- painterly masks
- brush strokes
- stylized noise
- designed silhouettes

Material language:

- dissolve and masking
- gradient remaps
- color ramps
- controlled alpha contrast
- fresnel/rim accents
- stylized emissive, usually less physically constrained

Motion language:

- snappy timing
- fast attack, slower decay
- keyframed-feeling curves
- clear anticipation, impact, and release
- motion exaggeration is allowed if the silhouette reads better

Shape language:

- S-curves
- arcs
- readable flame tongues
- cotton-like smoke puffs
- ornamental magic forms

Color language:

- complementary color accents
- more saturation than realistic
- palette should be designed rather than physically derived
- strong value grouping for readability

Niagara focus:

- scale curves
- color curves
- controlled bursts
- orbit/vortex/attraction for magical motion
- limited number of intentional layers

Material focus:

- gradient mapping
- CheapContrast
- SmoothStep
- erosion masks
- panning brush textures
- stylized dissolve edges

Common failures:

- too much realistic noise that muddies the silhouette
- color palette lacks intention
- timing is soft everywhere
- shape has no clear read

## Anime / Cel-Shaded

Goal:

- 2D animation energy inside 3D
- hard readability
- dynamic action
- frame-by-frame punch

References:

- Guilty Gear Strive
- Genshin Impact
- Zenless Zone Zero

Texture language:

- flat colors
- stepped gradients
- hard-edged masks
- slash shapes
- speed lines
- graphic impact frames

Material language:

- toon shader logic
- stepped alpha
- Step or SmoothStep with tight thresholds
- rim/outline accents
- gradient-map color bands
- mesh-based slashes and arcs are common

Motion language:

- limited animation is acceptable
- reduced apparent frame rate such as `12 FPS`
- snappy timing
- strong pose-to-pose motion
- impact frames lasting `1-2 frames`
- abrupt scale and opacity changes can be correct

Shape language:

- sharp slashes
- hard silhouettes
- graphic rays
- lightning-like angular shapes
- rings and bursts with clean boundaries

Color language:

- separated bands
- fewer gradients
- high contrast
- palette can be saturated, but should stay element-consistent

Niagara focus:

- bursts
- mesh renderers
- ribbon/mesh slash timing
- short-lived high-impact particles
- stepped curves instead of smooth organic easing

Material focus:

- Step
- SmoothStep with narrow range
- posterized gradients
- outline/rim materials
- mesh masks
- hard alpha cutoff

Common failures:

- too many soft translucent sprites
- gradients look too realistic
- no impact frame on strong hits
- motion is physically smooth when it should be graphic and snappy

## Mobile Stylized

Goal:

- readability
- performance
- clean silhouettes on small screens
- low overdraw

References:

- League of Legends: Wild Rift
- Brawl Stars

Texture language:

- optimized 2D textures
- simple masks
- few frames per flipbook
- large readable shapes
- minimal fine detail

Material language:

- cheap additive/translucent materials
- packed masks
- minimal texture samples
- simple panners
- limited dynamic branching
- mobile-safe feature choices

Motion language:

- fast and decisive
- short lifetime
- clear attack/release
- avoid long overdraw-heavy lingering unless necessary

Shape language:

- circles
- rays
- arcs
- cones
- simple ground indicators
- strong readable silhouettes

Color language:

- high saturation
- high value separation from environment
- effect identity must remain readable in crowded combat

Niagara focus:

- low particle counts
- few emitters
- strong size/color curves
- simple bursts
- avoid expensive collision unless necessary

Material focus:

- packed masks
- low sample count
- no expensive refraction unless explicitly allowed
- avoid heavy translucent stacks

Common failures:

- too many subtle details that disappear on screen
- overdraw too high
- particles live too long
- color does not pop from the background

## iGaming / Casino

Goal:

- excitement
- reward
- luxury
- immediate dopamine read

References:

- Pragmatic Play slot effects
- Arrise slot/game reward effects

Texture language:

- high gloss
- metallic coins
- gems
- sparkles
- radial bursts
- star glints
- light rays

Material language:

- additive
- high emissive
- strong bloom
- metallic/glossy supporting meshes
- panning sparkle overlays
- high contrast compositing

Motion language:

- juicy
- celebratory
- long enough to sell reward
- repeated bursts for `Big Win` moments
- coins/gems can bounce with playful physics
- sparkles should pop in clusters

Shape language:

- radial burst
- starburst
- rays
- coin arcs
- gem showers
- expanding rings
- glitter fields

Color language:

- high contrast
- gold, diamond, neon, jewel tones
- dark background values make emissive elements feel expensive
- saturation is allowed and often expected

Niagara focus:

- burst sequences
- radial velocity
- gravity and bounce for coins/gems
- sparkle emitters
- repeated reward pulses
- layered timing for escalation

Material focus:

- high emissive multiply
- panning sparkle texture
- glint masks
- additive layers
- radial gradients
- animated shine sweeps

Common failures:

- too subtle
- no layered reward escalation
- flat gold without glints
- no bounce or playful secondary motion
- effect ends too quickly for a win moment

## Master Material Implications

When building reusable master materials, prefer style switches or parameter groups that can support these families without rewriting the whole graph.

Current repo tooling is validated on UE `5.3`, but the material design should remain portable. If a newer Unreal version exposes better nodes, functions, Substrate behavior, Niagara features, or renderer options, add version-specific notes instead of deleting the UE `5.3` path.

AAA Realistic:

- prioritize `DepthFade`
- soft particles
- subtle distortion/refraction
- lit translucent smoke when affordable
- lower saturation
- physically motivated color ramps

Stylized / Painterly:

- prioritize dissolve/masking
- gradient remap
- brush/noise texture breakup
- stronger silhouette controls
- art-directed color ramps

Anime / Cel-Shaded:

- prioritize Step/SmoothStep alpha
- stepped gradient color
- rim/outline options
- hard alpha cutoff
- mesh/ribbon compatibility

Mobile Stylized:

- prioritize cheap packed masks
- low sample count
- simple additive/translucent output
- short-lived particles
- explicit overdraw limits

iGaming / Casino:

- prioritize high emissive
- additive glints
- sparkle overlays
- radial burst masks
- shine sweep controls
- high contrast and bloom response

## Style Comparison Table

| Feature | AAA Realistic | Stylized / Painterly | Anime / Cel-Shaded | Mobile Stylized | iGaming / Casino |
| --- | --- | --- | --- | --- | --- |
| Texture | Photo-sourced, simulated, noise | Hand-painted, brush masks | Flat, stepped, graphic | Optimized 2D, simple masks | Gloss, metallic, sparkles |
| Material | PBR, volumetric, translucent lit | Dissolve, masking, ramps | Toon, rim, hard alpha | Cheap additive/translucent | Additive, high emissive |
| Motion | Fluid, inertial, lingering | Keyframed curves, snappy | Stepped, impact frames | Fast, decisive | Juicy, bouncing, celebratory |
| Shape | Amorphous, soft | S-curves, strong silhouette | Sharp, graphic | Simple, readable | Radial, rays, glints |
| Color | Natural, lower saturation | Designed contrast | Banded, high contrast | Saturated, readable | Gold/neon/jewel, high contrast |
| Vibe | Immersion | Artistic | Dynamic | Clarity | Reward |

## Agent Workflow

Before authoring:

1. Identify requested style or infer default.
2. State the style in the plan.
3. Load this guide.
4. Load the effect-specific recipe.
5. Load material technique docs.
6. Choose Niagara module complexity based on style.
7. Choose material style controls based on style.
8. Validate against the style, not only against compile success.

If the user does not specify style:

- natural fire, smoke, dust, explosion, water, heat = `AAA Realistic`
- combat spell with no realism request = `Stylized / Painterly`
- manga/anime/2D/action slash = `Anime / Cel-Shaded`
- phone/mobile readability/performance = `Mobile Stylized`
- win celebration/slot/reward/coins/gems = `iGaming / Casino`

## Senior Advice

A good master material does not force every effect into one look.

Expose style-safe parameters:

- Softness
- AlphaHardness
- EmissiveIntensity
- Saturation
- DepthFadeDistance
- DistortionStrength
- ErosionAmount
- EdgeWidth
- EdgeIntensity
- SparkleIntensity
- GradientSteps
- PreviewMode

But keep defaults conservative. The artist should be able to push the material toward AAA, stylized, anime, mobile, or casino without starting from a broken shader.
