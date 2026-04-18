# Material Pattern: Basic Soft-Particle Sprite

## Purpose
Reusable note for future VFX work that needs:
- embers
- sparks
- magic particles
- soft additive/translucent sprite particles
- generic Niagara sprite materials with safe artist controls

This note was extracted from analysis of:
- `/Game/VFX/VFX-Combat/FlameThrowerBreath/_GenericSource/Material/M_Vfx_Basic.M_Vfx_Basic`

## Why This Pattern Matters
This material is not a hero lookdev graph.
It is a clean base pattern for particle sprites:
- low complexity
- high maintainability
- easy to instance
- good artist-facing toggles

This makes it more valuable as reusable knowledge for ForgePilot than many richer but messier graphs.

## What This Material Really Is
At a high level, this is:
- `Unlit + Translucent`
- particle-color driven
- texture-mask driven
- optional edge shaping
- optional depth fade
- optional fresnel filtering

It is a strong base template for many particle types where the look comes mostly from:
- particle color from Niagara
- particle alpha from a simple mask texture
- a few optional shaping switches

## High-Level Pattern
The reusable structure is:

1. `UV utility block`
2. `Mask texture sampling`
3. `ParticleColor tinting`
4. `Alpha channel selection`
5. `Edge shaping`
6. `Depth fade`
7. `Optional fresnel filter`

This is the part ForgePilot should preserve.
Do not overcomplicate this pattern unless the effect truly needs it.

## Reusable Block 1: UV Utility
Observed signal:
- `TexCoord`
- `Swizzle`
- static switches for `SwapUV` and `FlipUV`

Reusable lesson:
- even a simple particle material benefits from lightweight UV utility controls
- UV orientation problems should be solvable inside the material instance, not by duplicating textures

Useful for:
- sparks with streak textures
- directional magical wisps
- particles reused across multiple emitters

Recommended controls:
- `SwapUV`
- `FlipUV`

## Reusable Block 2: Mask Texture Sampling
Observed signal:
- one `TextureSampleParameter2D`
- parameter name `mask`
- source texture `T_Vfx_BasicDot`

Reusable lesson:
- a single parameterized mask texture is often enough for a reusable sprite base
- keep texture swappability at the material-instance level

Useful mask types:
- round soft dot
- spark streak
- noisy ember blob
- magical rune speck

Recommended pattern:
- use one `TextureSampleParameter2D`
- let Niagara drive color, not the texture

## Reusable Block 3: Particle Color Tinting
Observed signal:
- `ParticleColor RGB` multiplies the sampled mask RGB for emissive
- `ParticleColor A` multiplies alpha shaping

Reusable lesson:
- keep tint and alpha ownership in Niagara whenever possible
- this lets one material support many looks:
  - orange embers
  - white sparks
  - blue magic dust
  - purple cursed motes

Key takeaway:
- a reusable particle base should be `ParticleColor-first`, not hard-coded-color-first

## Reusable Block 4: Alpha Channel Selection
Observed signal:
- static switch `R_as_alpha`
- can choose `R` or `A` from the mask texture

Reusable lesson:
- many particle textures are authored inconsistently
- a reusable material should tolerate either:
  - alpha stored in `A`
  - alpha stored in `R`

Useful for:
- borrowed marketplace textures
- old studio textures with packed channels
- quick experimentation without reauthoring textures

Recommended control:
- `UseRedAsAlpha` or `R_as_alpha`

## Reusable Block 5: Edge Shaping
Observed signal:
- static bools `EdgeDark` and `EdgeBright`
- scalar `EdgeControl`
- `SmoothStep`
- switchable edge treatment

Reusable lesson:
- edge feel on particle sprites matters a lot
- ForgePilot should treat edge shaping as a reusable authored block, even on simple particle materials

Typical usage:
- `EdgeDark`
  - darker rim, better for smoky or denser particles
- `EdgeBright`
  - brighter rim, better for magical glows or hot sparks
- `EdgeControl`
  - threshold to tighten or soften sprite silhouette

Useful for:
- embers that need a hotter center
- sparks that need harder falloff
- magic particles that need clean readable rims

## Reusable Block 6: Depth Fade
Observed signal:
- `DepthFade`
- static toggle `Fade`
- scalar `FadeDistance`

Reusable lesson:
- soft particles should usually have depth fade available by default
- do not force it always-on; make it a toggleable block

Useful for:
- smoke-adjacent particles
- sparks intersecting terrain
- wisps clipping through the hand or orb mesh

Recommended controls:
- `Fade`
- `FadeDistance`

## Reusable Block 7: Optional Fresnel Filter
Observed signal:
- `Fresnel`
- `Saturate`
- `OneMinus`
- `Power`
- static switch `UseFresnelFilter`
- scalar `FresnelFilterPOW`

Reusable lesson:
- fresnel on particles should be optional and lightweight
- it is useful as a shaping modifier, not as the whole look

Useful for:
- magic motes near camera
- shell-adjacent sparks
- particles that need subtle view-dependent filtering

Avoid:
- making fresnel mandatory for all sparks
- stacking heavy fresnel and heavy edge control unless the read is intentional

## Why This Is Good Knowledge For ForgePilot
This pattern is exactly the sort of graph ForgePilot should learn as a default base:
- small
- robust
- artist-tunable
- reusable across many effects

It is better as a starting template for embers/sparks than trying to build every particle material from scratch.

## Good Use Cases
Strong fit:
- ember sprites
- spark bursts
- magical dust
- tiny plasma motes
- aura flecks
- hand-fire fallout particles

Partial fit:
- flipbook fire sprites
- distortion particles

Not a good fit by itself:
- hero plasma shell
- energy orb surface
- ribbon lightning material
- large-volume hero smoke

## Parameter Design Lessons
Patterns worth keeping:
- one texture parameter for the mask
- particle color as the primary tint source
- a separate edge-control scalar
- a separate fade-distance scalar
- small static toggles for UV and alpha handling

Bad pattern to avoid:
- baking color and alpha assumptions directly into the texture
- creating a new material for every minor spark variant

## Suggested Reusable Template Extraction
Future reusable template blocks for ForgePilot should include:

1. `MF_SpriteMaskSample_Basic`
2. `MF_SpriteUVUtility`
3. `MF_SpriteEdgeShape`
4. `MF_SoftParticleDepthFade`
5. `MF_SpriteFresnelFilter_Light`

These should support building:
- `M_VFX_SpriteBasic_Trans`
- `M_VFX_SpriteBasic_Add`
- `MI_Ember_Base`
- `MI_Spark_Base`
- `MI_MagicMote_Base`

## Authoring Guidance For Future Agents
When building sprite particle materials for Niagara:
- start from a minimal particle-color + mask pattern
- add depth fade only if the effect intersects geometry
- add fresnel only if camera/view shaping improves readability
- expose UV and alpha interpretation toggles up front
- keep the graph small enough that artists can trust and instance it quickly

For ForgePilot specifically:
- use this pattern as the default recommendation for embers and sparks
- use material instances for color/look variation
- keep the material generic and let Niagara drive per-particle variation

## Key Takeaway
The reusable knowledge is not "this one white blob material works."

The reusable knowledge is:
- a sprite particle material should be particle-color driven
- mask texture sampling should be swappable
- alpha interpretation should be tolerant of packed-texture differences
- edge shaping should be optional and explicit
- depth fade should be available but not forced
- fresnel should be a light modifier, not the foundation

That is a strong general-purpose material pattern for embers, sparks, and many magic particles.
