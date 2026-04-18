# Fire Flipbook Dynamic Erosion Workflow

## Purpose

Use this workflow when building UE 5.3 fire flipbooks where Niagara `Scale Color Alpha` is unreliable, too opaque for debugging, or not friendly enough for AI-generated emitters.

This is the recommended collaboration path for ForgePilot fire flipbook work:

- AI creates the safe material and Niagara scaffold.
- Human artists tune the look through familiar Niagara modules.
- The material owns erosion fade, edge shaping, and soft intersection.
- Niagara owns spawn, size, lifetime, motion, color, and the erosion control curve.

## Human And AI Layer Split

Keep the first production workflow boring in the right places.

Niagara should own the physical and timing layer:

- spawn rate
- random size
- random velocity
- speed
- drag
- acceleration
- curl or wind forces
- gravity
- collision
- base sprite size at spawn
- optional scale size over life
- sprite rotation and rotation rate
- lifetime
- SubUV playback timing

Material should own the look and shaping layer:

- color shaping
- fade in and fade out
- erosion and dissolve
- alpha contrast
- soft edge shaping
- noise breakup
- distortion or refraction
- emissive shaping
- depth fade
- camera fade
- UV-space detail that should stay visually coherent with the flipbook

Dynamic Parameters are the bridge between the two layers.

Use Dynamic Parameters when the artist needs Niagara-side randomization or curves for material behavior:

- random color variation
- per-particle fade amount
- erosion amount
- noise strength
- distortion strength
- refraction amount
- edge width
- edge brightness
- opacity multiplier
- luma/alpha threshold

Avoid Niagara Scratch Scripts in the first pass unless the task explicitly requires them. Scratch scripts are powerful, but they increase authoring complexity, debugging cost, and AI failure risk. Use ordinary Niagara modules first. Add Scratch Scripts later only after the normal module workflow is stable and the effect needs optimized or highly stylized motion.

## Research Summary

RealTimeVFX references point toward a conservative baseline:

- Use Niagara/SubUV or `ParticleSubUV` for ordinary fire flipbooks before attempting custom UV playback.
- Use Niagara `Sub UVAnimation` for frame playback and variants when possible.
- Keep material-side custom flipbook logic for special cases, not the first pass.
- Use material-side dissolve/erosion controls when the fade needs to be more art-directable than simple alpha.
- Treat motion-vector flipbook interpolation as an advanced pass, not the baseline.

Useful search entrypoint:

- `https://realtimevfx.com/search?q=fire`

Useful domain searches:

- `site:realtimevfx.com fire flipbook niagara unreal`
- `site:realtimevfx.com random row flipbook animation unreal`
- `site:realtimevfx.com sub uv material tex coords`
- `site:realtimevfx.com fire niagara motion vectors flipbook`

## Production Decision

For the current project, prefer:

- `ParticleSubUV` in the material for runtime atlas sampling.
- Niagara `Sub UVAnimation` for frame playback.
- `Dynamic Parameter` index 0 for material-side erosion fade.
- Niagara `Scale Color` for RGB tint only unless alpha has been proven to work.
- `DepthFade` in the material for world intersection softening.

Do not start with:

- `FlipBook_MotionVectors`
- custom material-only flipbook playback
- normal atlas lighting on an unlit fire card
- large procedural node graphs that hide simple tuning controls from the artist

## Material Contract

Material properties:

- Blend Mode: `Translucent`
- Shading Model: `Unlit`
- Two Sided: `true`

Runtime sample:

- `ParticleSubUV`

Runtime color:

- `ParticleSubUV RGB * ParticleColor RGB`

Emissive:

- `runtime color * EmissiveIntensity`
- optionally boost the erosion edge with `EdgeIntensity`

Opacity:

- derive mask from `ParticleSubUV A`
- if alpha is weak or empty, derive mask from RGB luma instead
- shape mask with erosion logic
- multiply by dynamic opacity multiplier
- run through `DepthFade`

Important type rule:

- Never multiply `float3` by `float4`.
- Use `ComponentMask RGB` for color.
- Use `ComponentMask A` or single-channel luma for opacity.
- When wiring `Dynamic Parameter`, connect the named scalar outputs directly: `ErodeAmount`, `EdgeWidth`, `OpacityMult`, and `EdgeIntensity`.
- Automation tools must use UE's `UMaterialExpression::ConnectExpression()` path, or otherwise copy `Mask / MaskR / MaskG / MaskB / MaskA` from the selected `FExpressionOutput` into the target `FExpressionInput`. Setting only `Expression + OutputIndex` can make UE compile the link as the wrong width and cause `float3`/`float4` arithmetic errors.
- Do not wire the `RGBA` output into mask nodes as the default authoring pattern. Use that only as a temporary manual fallback if a tool cannot preserve named-output masks.
- Every material build must pass `compile_material_and_get_diagnostics` before reporting done.

Graph authoring rule:

- Do not create Material comment boxes by default.
- Build the graph as clean left-to-right lanes: inputs, masks, erosion opacity, emissive, output.
- Prefer readable node placement over large colored comment regions.
- Use `layout_material_expressions`, or call `cleanup_material_graph` with `create_comments=false`.
- Review graph hygiene with `require_comment_coverage=false` unless the user explicitly asks for comment-box coverage.
- Comment boxes are opt-in only; use them for risky Custom/HLSL or intentional large master-material regions, not for ordinary fire flipbook graphs.

## Dynamic Parameter Contract

General DP rules live in `Docs/Guides/DYNAMIC_PARAMETER_WORKFLOW.md`. This section is the fire-specific channel contract.

Use `Dynamic Parameter` index `0`.

Recommended channel names:

- `R = ErodeAmount`
- `G = EdgeWidth`
- `B = OpacityMult`
- `A = EdgeIntensity`

Recommended default values:

- `R = 0.0`
- `G = 0.08`
- `B = 1.0`
- `A = 1.5`

Meaning:

- `ErodeAmount`: drives the flame disappearance. Animate this from low to high over particle life.
- `EdgeWidth`: controls how wide the hot edge band is during erosion.
- `OpacityMult`: overall opacity multiplier. Use this instead of relying on `Scale Color Alpha` when alpha does not respond.
- `EdgeIntensity`: brightness multiplier for the erosion edge.

Niagara setup:

- Add `Dynamic Material Parameters`.
- Enable `Write Parameter Index 0`.
- Drive `Param0` with an age curve for `ErodeAmount`.
- Drive `Param1` with a constant or curve for `EdgeWidth`.
- Drive `Param2` with a constant or curve for `OpacityMult`.
- Drive `Param3` with a constant or curve for `EdgeIntensity`.

Use clear Niagara-side labels/comments because UE may still show generic `Param0-0`, `Param1-0`, `Param2-0`, and `Param3-0` in some module details.

## Erosion Formula

Use this shape conceptually:

```text
mask = TextureAlpha or Luma(TextureRGB)
erode = saturate((mask - ErodeAmount) / max(EdgeWidth, 0.001))
edge = saturate(1 - abs(mask - ErodeAmount) / max(EdgeWidth, 0.001))

baseOpacity = erode * OpacityMult
finalOpacity = DepthFade(baseOpacity, DepthFadeDistance)

baseEmissive = TextureRGB * ParticleColorRGB * EmissiveIntensity
edgeEmissive = edge * EdgeIntensity
finalEmissive = baseEmissive + edgeEmissive
```

If the texture alpha is already the correct flame silhouette, use alpha. If alpha is missing or too weak, use RGB luma:

```text
luma = dot(TextureRGB, float3(0.299, 0.587, 0.114))
```

## Niagara Module Contract

Keep Niagara simple and artist-friendly.

Required modules:

- `Initialize Particle`
- `Spawn Rate`
- `Add Velocity`
- `Curl Noise Force`
- `Drag`
- `Scale Color`
- `Sub UVAnimation`
- `Dynamic Material Parameters`
- `Sprite Renderer`

Optional modules:

- `Scale Sprite Size`
  Use only when you intentionally want a life curve on top of the spawn size. Do not use it as the primary random-size authoring path for ordinary fire.

Renderer requirements:

- Assign the fire material or material instance.
- Set `SubImage Size` to the exact flipbook grid.
- Enable SubUV blending only if the sheet benefits from frame interpolation.

Human-tuned controls:

- Spawn Rate
- Lifetime
- Sprite Size
- Velocity
- Curl Noise
- Drag
- Scale Color RGB
- SubUV playback timing
- Dynamic Parameter erosion curve

Spawn-size authoring rule:

- Put base fire size in `Initialize Particle -> Uniform Sprite Size`.
- Use a random range there if you want per-particle size variation.
- Do not author ordinary fire size through `Set (PARTICLES)` assignment modules.
- Do not use `Scale Sprite Size` with a random dynamic input as a substitute for spawn size, because it can evaluate in update and make the flame feel jittery or unstable.

AI should not hide these controls inside hardcoded custom expressions when equivalent Niagara module settings already exist.

Scratch Script rule:

- Do not create Scratch Scripts for ordinary fire spawn, size, velocity, drag, curl, lifetime, or color randomization.
- Prefer existing Niagara modules and expose their values clearly.
- Only create Scratch Scripts for second-pass work such as custom vortex motion, nonstandard particle state machines, optimized multi-control packing, or style-specific motion that cannot be expressed cleanly with stock modules.

## Artist-Friendly Tuning Defaults

Good first-pass fire values:

- Lifetime: `0.5-0.9`
- Spawn Rate: `20-60`
- Sprite Size: `45-120`
- Upward Velocity: `120-260`
- Curl Noise: `10-35`
- Drag: `0.8-2.0`
- ErodeAmount over normalized age: starts near `0.0`, ends near `1.0`
- EdgeWidth: `0.04-0.14`
- OpacityMult: `0.8-1.5`
- EdgeIntensity: `0.8-3.0`

## Preview Strategy

Material Instance preview can be misleading because `ParticleSubUV` expects particle context.

If artist-side MI preview is required, add a debug switch that changes the sample path:

- Runtime path: `ParticleSubUV`
- Preview path: ordinary texture sample or `FlipBook` material function with `PreviewPhase`

Bad preview switch:

- still samples through `ParticleSubUV`
- only changes color or opacity after the sample

Good preview switch:

- bypasses `ParticleSubUV`
- samples the atlas directly
- uses `PreviewPhase`, `PreviewColor`, and `PreviewAlpha`

Default the preview switch off for Niagara use.

## Validation Gate

Before saying the asset is done:

1. Compile the material with `compile_material_and_get_diagnostics`.
2. Confirm diagnostics are clean.
3. Confirm no `float3`/`float4` arithmetic error remains.
4. Confirm Niagara renderer material binding.
5. Confirm Niagara `SubImage Size`.
6. Confirm `Sub UVAnimation` exists.
7. Confirm `Dynamic Material Parameters` writes index `0`.
8. Confirm viewport visibility with a nonzero sprite size and nonzero opacity multiplier.

If any of these fail, do not report success.

## Common Failure Modes

- Material preview is blank because `ParticleSubUV` has no particle context.
- Niagara is invisible because `SubImage Size` is missing or wrong.
- Fade does not work because the material uses Additive or ignores opacity.
- `Scale Color Alpha` appears not to work because opacity is dominated by another material branch.
- Shader compile fails because a `float4` output is multiplied with a `float3` branch.
- Dynamic Parameter channels look scalar in the editor, but an automated connection can still compile as a wider vector if the tool does not preserve the output mask. Fix the tool; do not silently ship an `RGBA` workaround graph.
- Dynamic Parameter labels in the material do not appear clearly in Niagara details if the named outputs are not connected. The agent must connect each named output to its real material use so Niagara can display artist-friendly names.
- Motion-vector or normal textures add complexity before the base flame is proven visible.

## Agent Rule

For future fire flipbook tasks, do not spend long reasoning loops inside broken assets.

Use this order:

1. Research RealTimeVFX.
2. Read this guide.
3. Create a minimal clean material.
4. Compile and inspect diagnostics.
5. Create a material instance.
6. Create a clean Niagara system.
7. Bind material and SubUV settings.
8. Add Dynamic Material Parameters for erosion.
9. Validate visibility.
10. Only then add production polish.
