# Orb Material Execution Plan

## Goal

Build a test orb material that matches the reference look:
- teal/green glowing sphere
- soft internal moving cloud/noise
- brighter rim
- brighter core
- controlled contrast and color variation
- clean graph lanes and MI-first tuning workflow

## Transcript-Derived Workflow

1. Use an `Unlit` material.
2. Start from a noise texture or noise-like texture.
3. Animate UV flow with:
   - `TextureCoordinate`
   - optional `Panner`
   - `Rotator`
   - if needed, add directional variation later
4. Shape contrast with:
   - `Power`
   - `Saturate`
5. Drive color variation using a blurred/noise mask and two green colors through `Lerp`.
6. Multiply the animated noise by the color result, then feed:
   - `BaseColor` or visual body color output
   - `EmissiveColor`
7. Add rim glow with:
   - `Fresnel`
   - `Power`
   - `Multiply` intensity scalar
8. Add a brighter core with:
   - second `Fresnel`
   - `OneMinus`
   - `Power`
   - `Saturate`
9. Add rim/core together into emissive shaping.
10. Optional depth:
   - `BumpOffset` using the same noise texture

## Graph Structure

### Lane 1 - UV / Motion
- `TexCoord`
- `ScalarParameter(Orb_Tiling)`
- `Multiply`
- `ScalarParameter(Orb_PanX)`
- `ScalarParameter(Orb_PanY)`
- `Panner`
- `ScalarParameter(Orb_RotationSpeed)`
- `Rotator`

### Lane 2 - Body Mask
- `TextureSampleParameter2D(Orb_NoiseTex)`
- `Power`
- `Saturate`

### Lane 3 - Color Variation
- `TextureSampleParameter2D(Orb_ColorVariationTex)` or reuse noise
- `Power`
- `Saturate`
- `VectorParameter(Orb_ColorA)`
- `VectorParameter(Orb_ColorB)`
- `Lerp`

### Lane 4 - Rim / Core
- `Fresnel`
- `Power`
- `ScalarParameter(Orb_RimIntensity)`
- `Multiply`

- second `Fresnel`
- `OneMinus`
- `Power`
- `Saturate`
- `ScalarParameter(Orb_CoreIntensity)`
- `Multiply`

### Lane 5 - Final Combine
- body color * body mask
- add rim emissive
- add core emissive
- optional extra multiply scalar
- connect to:
  - `EmissiveColor`
  - optional `BaseColor` if needed for preview readability

## MI-First Tuning

Tune in the MI before changing master logic:
- `Orb_Tiling`
- `Orb_PanX`
- `Orb_PanY`
- `Orb_RotationSpeed`
- `Orb_Contrast`
- `Orb_ColorContrast`
- `Orb_ColorA`
- `Orb_ColorB`
- `Orb_RimPower`
- `Orb_RimIntensity`
- `Orb_CorePower`
- `Orb_CoreIntensity`
- `Orb_GlobalIntensity`

## Self-Check Loop

After authoring each version:

1. Layout graph horizontally by lane.
2. Wrap comments:
   - `Orb Parameters`
   - `Orb UV Motion`
   - `Orb Color Body`
   - `Orb Rim and Core`
   - `Orb Final Output`
3. Capture graph screenshot.
4. Validate graph block for:
   - no missing required inputs
   - root pins connected
   - expected internal links present
5. Create MI preview variants:
   - low glow
   - balanced
   - hot glow
6. Spawn probe sphere and capture visual images.
7. Compare captures.
8. If visual mismatch remains:
   - first tune MI
   - only then change master graph

## Acceptance Criteria

The orb test is acceptable when:
- graph is readable and lane-based
- graph capture works
- validator passes
- MI variants produce visible differences
- final balanced MI visually matches the reference direction:
  - teal/green
  - cloudy inner movement
  - bright rim
  - bright soft core
  - no broken wiring
