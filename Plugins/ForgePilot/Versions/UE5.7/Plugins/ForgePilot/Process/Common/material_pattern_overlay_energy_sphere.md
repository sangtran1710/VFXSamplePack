# Material Pattern: Overlay Energy Sphere

## Purpose
Reusable note for future VFX work that needs:
- energy orb surfaces
- plasma shells
- molten or flame overlays on spherical forms
- emissive translucent volume shells

This note was extracted from analysis of:
- `/Game/VFX/VFX-Combat/FlameThrowerBreath/_GenericSource/Material/M_Vfx_Overlay_FireFlame.M_Vfx_Overlay_FireFlame`

## Why This Pattern Matters
The referenced material is not useful only for flame effects.
It encodes a broader pattern for:
- spherical surface flow
- emissive edge shaping
- shell breakup
- light-driven translucent overlays

This makes it relevant to:
- plasma orbs
- magic shells
- molten cores
- shield bubbles
- cursed energy spheres
- impact buildup shells

## High-Level Pattern
The reusable structure is:

1. `Volume / local-space mapping`
2. `Dual-noise animated flow`
3. `Edge shaping`
4. `Surface deformation`
5. `Emissive + opacity separation`

This is the important part to preserve.
Do not preserve the exact graph complexity blindly.

## Reusable Block 1: Local-Space Volume Mapping
Observed signal:
- uses `LocalPosition`
- masks channels from local/object space
- drives spherical surface behavior from object-relative coordinates

Reusable lesson:
- when building energy shells or spherical overlays, prefer local/object-space driven mapping over plain UV-only mapping

Useful for:
- orb shells
- shield domes
- sphere-contained fire/plasma

Avoid:
- relying only on flat panning UVs for spherical energy materials

## Reusable Block 2: Dual-Noise Flow Stack
Observed signal:
- two noise families
  - fire-like noise
  - cloud-like noise
- multiple panners
- tiling and speed are parameterized

Reusable lesson:
- use one noise for primary structure and another for breakup/distortion
- keep them independently tunable

Recommended pattern:
- `PrimaryNoise`
  - medium-scale shape / plasma pattern
- `SecondaryNoise`
  - finer breakup or distortion support

Useful for:
- plasma shells
- molten surfaces
- magical orb skins
- energy haze overlays

## Reusable Block 3: Edge Shaping
Observed signal:
- `Fresnel`
- `ValueStep`
- `CheapContrast_RGB`
- explicit edge controls like width and boost

Reusable lesson:
- edge readability should be treated as its own authored block
- do not bury edge shaping inside one big multiply chain

Reusable controls to expose:
- edge width
- edge boost
- edge contrast
- highlight power

Useful for:
- shell rims
- shield edges
- magical envelopes
- charged aura borders

## Reusable Block 4: Surface Deformation
Observed signal:
- `WorldPositionOffset`
- extrude / deform controls
- distortion-driven displacement

Reusable lesson:
- subtle WPO can make spherical energy feel alive
- use WPO to destabilize the surface silhouette lightly

Useful for:
- plasma shells
- molten bubbles
- cursed barriers

Avoid:
- heavy WPO on the core itself
- large deformation that breaks silhouette readability

## Reusable Block 5: Emissive / Opacity Separation
Observed signal:
- emissive is driven independently
- opacity is shaped through a separate thresholding path
- material is `Translucent + Unlit`

Reusable lesson:
- for energy shells, brightness and opacity must be controlled separately
- opaque-looking shells usually fail because emissive and opacity are tied together too early

Recommended control split:
- `ShellOpacity`
- `EdgeOpacity`
- `EmissiveIntensity`
- `HighlightIntensity`

Useful for:
- anything that needs to glow hard without becoming visually solid

## Reusable Parameter Design Lessons
Patterns worth keeping:
- separate `tiling`
- separate `speed`
- separate `distortion strength`
- separate `edge width`
- separate `edge brightness`
- separate `highlight color/intensity`

Bad pattern to avoid:
- one `Intensity` parameter trying to solve all visual problems

## Where To Reuse This
Strong fit:
- energy orb shell
- plasma shield shell
- lava / molten sphere overlay
- curse energy bubble
- fire overlay on volume-based FX meshes

Partial fit:
- additive cores
- ribbon arcs

Notes:
- `orb core` should borrow flow/highlight logic, but not the whole translucent shell structure
- `ribbon arcs` should borrow edge/highlight separation, but not sphere mapping

## Recommended Template Library Extraction
Future reusable template blocks for ForgePilot should include:

1. `MF_EnergySphere_LocalSpaceMap`
2. `MF_DualNoiseFlow`
3. `MF_EdgeShape_FresnelStep`
4. `MF_SurfaceDeform_LightWPO`
5. `MF_EmissiveOpacitySplit`

These should become smaller reusable blocks instead of one giant material graph.

## Guidance For Future Agents
When inspecting a production material that looks visually rich:
- do not copy the graph node-for-node
- identify reusable visual blocks
- identify parameter naming patterns
- decide which blocks are geometry-specific and which are general-purpose

For this pattern specifically:
- reuse on shells first
- reuse selectively on cores
- reuse minimally on ribbons

## Key Takeaway
The real knowledge is not "this flame material looks good."
The reusable knowledge is:
- local-space spherical mapping
- layered noise flow
- explicit edge shaping
- light WPO deformation
- separate opacity and emissive control

That pattern is broadly reusable across many energy/plasma VFX, not only the current orb.
