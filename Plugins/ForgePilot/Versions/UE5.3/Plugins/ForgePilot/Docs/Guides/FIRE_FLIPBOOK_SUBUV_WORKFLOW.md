# Fire Flipbook SubUV Workflow

## Purpose

This guide exists to keep future fire flipbook work fast and low-risk.

Use this workflow first for UE 5.3 fire cards before escalating to:

- `FlipBook_MotionVectors`
- normal-atlas lighting tricks
- Dynamic Parameter-driven shader control

If Niagara `Scale Color Alpha` is unreliable or the user asks for material-side fade control, use:

- `Docs/Guides/FIRE_FLIPBOOK_DYNAMIC_EROSION_WORKFLOW.md`

## Recommended baseline

### Material

- `Translucent`
- `Unlit`
- `Two Sided = true`
- runtime sampling:
  - `ParticleSubUV`
- runtime color:
  - `ParticleSubUV RGB * ParticleColor RGB`
- runtime emissive:
  - previous result `* EmissiveIntensity`
- runtime opacity:
  - texture alpha or luma fallback `* ParticleColor A`
  - then `DepthFade`

### Niagara

- `Sprite Renderer`
- correct `SubImage Size`
- `SubUV Animation`
- `Scale Color`
- `Initialize Particle`
- `Add Velocity`
- `Curl Noise Force`
- `Drag`
- `Solve Forces and Velocity`

## Hard rules

### 1. Compile gate is mandatory

After meaningful material graph changes, run:

- `compile_material_and_get_diagnostics`

Do not treat topology inspection as final proof.

### 2. Preview fallback must change sampling path

If artist preview is needed outside Niagara:

- do not only lerp color values
- do not only lerp alpha values
- do change the actual sampling path

Good:

- runtime branch = `ParticleSubUV`
- preview branch = `FlipBook` function or ordinary texture sampling

Bad:

- runtime branch = `ParticleSubUV`
- preview branch still = `ParticleSubUV` but with different color/alpha

### 3. Dry-run is a real footgun

Several mutation tools default to dry-run behavior.

For real graph changes, always pass:

- `apply_changes = true`

Especially for:

- `connect_material_nodes`
- `spawn_niagara_at_location`

## Fast failure triage

If the fire is invisible:

1. Confirm the asset actually mutated.
2. Run `compile_material_and_get_diagnostics`.
3. If the shader error mentions `float3` or `float4`, add explicit `ComponentMask`.
4. Check whether preview is using `ParticleSubUV` outside particle context.
5. Check Niagara:
   - material binding
   - `SubImage Size`
   - `SubUV Animation`
   - `Scale Color` alpha
   - sprite size

If `Scale Color` alpha still does not visibly fade the fire, do not keep fighting the same path. Switch to the dynamic erosion workflow and drive opacity through `Dynamic Parameter` index `0`.

## Use this before reasoning deeply

If a future AI is about to spend many steps debugging fire flipbook setup, it should:

1. Read `Knowledge/Materials/Techniques/particle_subuv_flipbook_fire_smoke.json`
2. Read this guide
3. If erosion fade is needed, read `Docs/Guides/FIRE_FLIPBOOK_DYNAMIC_EROSION_WORKFLOW.md`
4. Run the validation harness:
   - `Tools/Validation/run_fire_flipbook_subuv_probe.ps1`

Only after that should it start deeper reasoning or tool changes.
