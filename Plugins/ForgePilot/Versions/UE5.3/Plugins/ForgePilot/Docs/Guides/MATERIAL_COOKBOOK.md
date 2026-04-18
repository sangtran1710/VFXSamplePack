# Material Cookbook

## Purpose

This file stores compact, reusable Material knowledge learned from actual build-and-verify runs in Unreal.

Use this file before starting a new Material task.

Goals:

- reduce repeated transcript reading
- reduce context usage
- preserve production-facing patterns
- preserve failure modes and validation rules

This file is intentionally short and operational. It is not a transcript archive.

## Material Execution Contract

Do not report a Material task as complete until all of these are true:

1. The graph compiles and root pins are connected as intended.
2. The graph is saved to disk and survives `close -> reopen`.
3. A graph screenshot exists and is readable enough to inspect lane layout.
4. A visual screenshot exists from an actual preview or level probe.
5. At least one MI tuning pass has been attempted before changing master logic again.
6. Iteration assets are cleaned up and only final review assets remain.

## Default Workflow

1. Read the target or tutorial and extract only the visual logic.
2. Build the smallest viable master graph.
3. Parameterize early for MI tuning.
4. Validate graph wiring immediately after mutation.
5. Capture graph.
6. Create MI variants.
7. Sweep MI values and capture visuals.
8. Classify failure:
   - tuning issue
   - graph readability issue
   - graph logic issue
9. Only if it is a graph logic issue, mutate the master graph again.
10. Save, close, reopen, and re-scan before reporting done.

## Layout Rules

- Parameters go in one lane.
- UV generation and texture sampling go in one lane.
- Mask generation goes in one lane.
- Color and emissive combine stay near the root.
- Avoid long cross-lane wires when a node can be moved closer.
- Prefer lane readability over aggressive auto-layout.
- Do not create comment boxes by default.
- Use node position, compact lanes, and short parameter names as the primary readability tools.
- Use `layout_material_expressions` for explicit node placement, or `cleanup_material_graph` with `create_comments=false`.
- Only create comment boxes when the user asks for them, when documenting a risky Custom/HLSL block, or when a large production master is intentionally organized into regions.
- If comments are used, they should name intent, not node type, and they must not hide messy wiring.

## Validation Rules

Always check:

- broken or unconnected math inputs
- root pin connectivity
- node count and link count after reopen
- texture references still present after reopen
- MI overrides apply to the intended parameters
- material graph hygiene with `require_comment_coverage=false` unless the user explicitly requested comment-box coverage

If the viewport looks correct but reopen loses the graph, treat the task as failed.

## Pattern: Orb

### Visual goal

- soft internal cloud motion
- bright rim
- brighter core contribution
- smooth, magical energy sphere feel

### Useful graph structure

- cloud/noise texture motion in 2 lanes
- color variation from texture-driven blend
- `Fresnel` for outer rim
- `OneMinus(Fresnel)` or equivalent for core weighting
- emissive-only or mostly emissive output

### Parameter groups

- `Orb_MainColor`
- `Orb_SecondaryColor`
- `Orb_RimColor`
- `Orb_RimIntensity`
- `Orb_CoreIntensity`
- `Orb_PanX`
- `Orb_PanY`
- `Orb_GlobalIntensity`

### Lessons learned

- A quick approximation can look acceptable, but if the tutorial relies on nodes like `BumpOffset`, `Rotator`, or a project Material Function, the result will look flatter without them.
- MI tuning is useful for orb brightness and color balance, but if the internal motion pattern is wrong, the fix belongs in the master graph.
- A world viewport can hide Material mistakes; use a preview or clean probe whenever possible.

### Failure signs

- flat or overly clean interior
- no parallax or depth feeling
- rim is present but body feels uniform
- result looks like a colored sphere, not an energy orb

## Pattern: Stylized Ring

### Visual goal

- broken stylized circular band
- layered posterized energy breakup
- hot bright accents inside the ring
- blue-white emissive falloff

### Useful graph structure

- radial UV remap
- ring mask from radial distance
- stylization texture multiplied into ring mask
- posterize chain:
  - `Multiply`
  - `Floor`
  - `Divide`
- intensity boost
- saturate
- dark-to-mid color blend
- hot highlight driven by `Power`
- final add into emissive

### Parameter groups

- `TX`
- `TY`
- `RingThickness`
- `Layers`
- `IntensityMul`
- `OpacityIntensity`
- `HotPower`
- `DarkColor`
- `MidColor`
- `HotColor`
- `AuraTex`

### Lessons learned

- Pure procedural concentric rings are not enough. A stylization texture or breakup layer is what makes the ring look usable.
- If the graph survives only in-session and not after reopen, the test is invalid even if the screenshot looks right.
- Manual lane layout is better than flattening this graph after the fact.
- The ring should be tuned through MI after the first graph pass. Do not keep changing the math if the problem is only sharpness, layer count, or brightness.

### Failure signs

- concentric target-like bands
- no breakup around the circumference
- ring looks like a debug pattern instead of VFX
- graph persists badly after reopen

## Pattern: Fire Flipbook SubUV

### Visual goal

- readable flame body from a real sprite sheet
- Niagara owns playback and fade timing
- material stays simple, robust, and MI-friendly
- artists can preview the look without opening Niagara every time

### Default baseline

- `Blend Mode = Translucent`
- `Shading Model = Unlit`
- `Two Sided = true`
- runtime sheet lookup uses `ParticleSubUV`
- color comes from `ParticleSubUV RGB * ParticleColor RGB`
- emissive comes from the color branch times one scalar intensity
- opacity comes from texture alpha or a luma-style fallback times `ParticleColor A`
- `DepthFade` softens world intersections

### Niagara contract

- Sprite Renderer must have correct `SubImage Size`
- `SubUV Animation` must exist in the emitter
- `Scale Color` must drive both RGB and Alpha
- Niagara owns:
  - playback
  - lifetime
  - size
  - color and fade

### Preview rule

- `ParticleSubUV` is fine for runtime but not enough for artist preview outside particle context
- if MI preview is required, add a dedicated preview branch that changes the sample path too
- preview fallback must not only swap color/alpha values
- a practical pattern is:
  - runtime branch = `ParticleSubUV`
  - preview branch = `FlipBook` material function or ordinary texture sampling
  - switch between them with a scalar or static preview control

### Compile rule

- do not trust graph topology alone
- after meaningful graph edits, always run `compile_material_and_get_diagnostics`
- if the error mentions `float3` and `float4`, fix the width explicitly with `ComponentMask`

### Failure signs

- MI preview is black even though the graph compiles
- Niagara compiles but particles appear invisible
- `ParticleSubUV` path works only in Niagara and artist preview remains unusable
- graph keeps growing because preview and runtime concerns are mixed without a clear switch

### Fast triage order

1. Check whether the graph actually mutated or a tool ran in dry-run mode.
2. Compile with `compile_material_and_get_diagnostics`.
3. Check preview mesh and sample path.
4. Check Niagara renderer material binding and `SubImage Size`.
5. Check `Scale Color` alpha and sprite size.
6. Only then reason about changing the master graph.

## When To Use Custom Nodes

Use a Custom node only when at least one of these is true:

- it replaces a long block of repeated scalar math cleanly
- the node chain would otherwise become unreadable
- the tutorial logic depends on compact math that is easier to verify in HLSL

Do not use a Custom node just to hide graph complexity.

If a tutorial is meant to teach node workflow, prefer native nodes first.

## When To Return To The Master Graph

Return to the master graph only if:

- MI tuning cannot reach the target look
- the breakup pattern is fundamentally wrong
- the graph lacks a control needed for tuning
- the graph is too messy to maintain

Do not return to the master graph just because one MI variant looks weak.

## Cleanup Rules

- Keep runtime artifacts in `Downloads`, not in the plugin.
- Keep only final review assets in `/Game/ForgePilot_Audit`.
- Delete intermediate materials and MI variants once a final pair is chosen.
- Do not leave probe actors in the level.

## Reuse Rules For Future Tutorials

Before reading a new tutorial transcript in full:

1. Check whether the target effect already matches an existing cookbook pattern.
2. Reuse the nearest pattern as the baseline graph.
3. Only extract the delta from the tutorial.
4. Add new knowledge back into this file after the task if it changes the default workflow.
