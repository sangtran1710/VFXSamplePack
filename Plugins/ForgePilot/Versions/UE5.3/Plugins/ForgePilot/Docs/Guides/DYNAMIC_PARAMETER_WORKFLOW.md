# Dynamic Parameter Workflow

## Purpose

Use this guide when ForgePilot needs a real-time data bridge from Niagara to a Material.

For curve timing, biased random distribution, and linked-randomness patterns on the Niagara side, also read `Docs/Guides/NIAGARA_CURVE_LOGIC_PATTERNS.md`.

Think of `Dynamic Parameter` as a per-particle control bus:

- Niagara writes values on the particle side.
- The Material reads those values on the GPU side.
- The artist uses those values to control material-only behavior such as erosion, distortion, edge width, emissive punch, UV offsets, alpha contrast, or custom HLSL inputs.

Do not treat Dynamic Parameter as a generic replacement for every particle property. If the effect only needs RGB tint and alpha, start with `Particle Color`. Use Dynamic Parameter for controls that are not already represented cleanly by built-in particle attributes.

## Material-Side Contract

Create a `Dynamic Parameter` node in the Material and configure it before building the graph around it.

Required authoring rules:

- Use meaningful channel names. Examples: `ErodeAmount`, `EdgeWidth`, `OpacityMult`, `EdgeIntensity`, `DistortionStrength`, `GlowIntensity`, `UVRandomOffset`.
- Connect named scalar outputs directly to their real material use. Do not route from `RGBA` by default.
- Keep one `Dynamic Parameter` node to four packed scalar controls whenever possible.
- Use `Parameter Index = 0` for the first four channels.
- Add another `Dynamic Parameter` with `Parameter Index = 1`, `2`, or `3` only when the first four channels are genuinely not enough.
- Compile with `compile_material_and_get_diagnostics` before reporting success.

Correct named-output example:

```text
DynamicParameter.ErodeAmount -> erosion subtract/threshold input
DynamicParameter.EdgeWidth -> max(edge width, 0.001)
DynamicParameter.OpacityMult -> opacity multiply
DynamicParameter.EdgeIntensity -> edge emissive multiply
```

Bad default pattern:

```text
DynamicParameter.RGBA -> ComponentMask R/G/B/A -> controls
```

Use the `RGBA -> ComponentMask` fallback only when a tool cannot preserve named-output masks. If this fallback is necessary, fix the tool rather than shipping a confusing graph.

## Niagara-Side Contract

In Niagara, add `Dynamic Material Parameters` to the emitter stage that owns the behavior.

Stage choice:

- Use `Particle Spawn` for per-particle initial randomization that should not change over life.
- Use `Particle Update` for curves, age-driven erosion, flicker, fade, distortion changes, or time-varying values.

Setup checklist:

- Assign the target material or material instance to the renderer first.
- Add `Dynamic Material Parameters`.
- Enable the required `Write Parameter Index`.
- For index 0, wire or set the four channels that correspond to the Material's named outputs.
- Prefer constants, random range floats, and float curves over Scratch Scripts for first-pass work.
- Verify the Niagara module displays useful names. If it still shows generic `Param0`, inspect whether the Material named outputs are connected to actual downstream logic.

Value source patterns:

- Constant: use for stable controls such as base `EdgeWidth` or `EdgeIntensity`.
- Random Range Float: use at spawn for per-particle variation, for example distortion strength, glow intensity, UV offset, or erosion bias.
- Float from Curve: use over `Particles.NormalizedAge` for fade in/out, erosion, or flicker over life.
- Linked attribute: use only when the value should follow an existing Niagara attribute cleanly.

Important timing note:

- Use `Particles.NormalizedAge` when the material behavior should evolve per particle.
- Use `Emitter.NormalizedLoopAge` when the whole emitter should pulse or breathe together.
- If multiple DP channels should feel intentionally related, drive them from one shared random index and multiple remap curves instead of rolling separate random values for each channel.

## Dynamic Parameter Vs Particle Color

Use `Particle Color` when the Material only needs:

- RGB tint
- alpha fade
- simple color over life
- simple opacity over life

Use `Dynamic Parameter` when the Material needs:

- erosion amount
- alpha threshold or contrast
- depth fade distance or camera fade tuning
- distortion or refraction strength
- noise strength or noise seed
- edge width or edge intensity
- UV panning speed, UV offset, or tiling variation
- custom HLSL input values
- style-specific switches that must vary per particle

Good split for sprite VFX:

```text
ParticleColor.RGB -> tint
ParticleColor.A -> ordinary alpha when it works
DynamicParameter.R -> erosion or threshold
DynamicParameter.G -> edge width or depth fade distance
DynamicParameter.B -> distortion/noise/opacity multiplier
DynamicParameter.A -> edge emissive or contrast
```

## Packing Strategy

Pack related scalar controls into one index before adding more DP nodes.

Example fire erosion pack:

```text
Index 0 R = ErodeAmount
Index 0 G = EdgeWidth
Index 0 B = OpacityMult
Index 0 A = EdgeIntensity
```

Example distortion pack:

```text
Index 0 R = DistortionStrength
Index 0 G = NoiseScale
Index 0 B = PanSpeed
Index 0 A = RandomSeed
```

Example iGaming sparkle pack:

```text
Index 0 R = EmissiveFlicker
Index 0 G = SparkleTiling
Index 0 B = SparkleUVOffset
Index 0 A = Contrast
```

Only move to `Parameter Index = 1` when index 0 is already full and the added controls are worth the extra authoring and data cost.

## Art Style Usage

AAA realistic:

- Use Dynamic Parameter to drive erosion, opacity multiplier, depth fade distance, soft particle thresholds, heat distortion, and per-particle noise breakup.
- Do not describe DP as reading `SceneDepth`. `SceneDepth`, `PixelDepth`, `DepthFade`, or `CameraDepthFade` are evaluated in the material; DP should drive their thresholds, fade distance, intensity, or blend amount.
- Keep values physically restrained. Avoid extreme saturation unless the effect is intentionally stylized.

Stylized or painterly:

- Use DP to drive dissolve shape, edge width, edge color intensity, mask contrast, and hand-painted texture breakup.
- Use curves for snappy timing, for example fast appear then slower dissolve.
- Use random ranges to avoid every sprite eroding identically.

Anime or cel-shaded:

- Use DP to control `Step` or `SmoothStep` thresholds.
- Use DP to harden soft gradients into readable shapes.
- Use stepped values or low frame-rate SubUV playback when the style calls for limited animation.

iGaming or casino:

- Use DP to drive emissive multiplier, sparkle UV offset, sparkle tiling, flicker intensity, radial burst brightness, and contrast.
- Keep controls punchy and readable. The goal is excitement and reward clarity, not physical correctness.

## Custom HLSL Usage

Dynamic Parameter can feed `Custom` HLSL nodes.

Use this only when native Material nodes or Material Functions are not enough.

Examples:

```text
DynamicParameter.RandomSeed -> Custom noise seed
DynamicParameter.DistortionStrength -> Custom UV warp intensity
DynamicParameter.ShapeBias -> Custom signed-distance mask threshold
```

Rules:

- Document expected input ranges in the material guide or recipe.
- Keep defaults safe. Avoid values that cause NaN, divide-by-zero, or invisible output.
- Validate with `compile_material_and_get_diagnostics`.

## Tooling Requirements

Automation tools that connect material nodes must preserve output masks.

Correct UE path:

```cpp
SourceExpression->ConnectExpression(TargetInput, SourceOutputIndex);
```

If the tool sets links manually, it must also copy these fields from `FExpressionOutput` to `FExpressionInput`:

```text
Mask
MaskR
MaskG
MaskB
MaskA
```

Failure symptom:

```text
[SM6] (Node Multiply) Arithmetic between types float3 and float4 are undefined
```

If this appears after connecting a named DP output, inspect the connection metadata first. Do not ship an `RGBA` workaround graph if the real issue is a broken connection tool.

## Validation Gate

Before reporting a Dynamic Parameter setup as done:

1. Confirm the material's DP channel names are meaningful.
2. Confirm each named output is connected to real downstream logic.
3. Confirm no ordinary controls still read from `RGBA` fallback unless explicitly justified.
4. Run `compile_material_and_get_diagnostics`.
5. Confirm material error count is `0`.
6. Confirm Niagara renderer uses the material or material instance.
7. Confirm `Dynamic Material Parameters` exists in Niagara.
8. Confirm the correct `Write Parameter Index` is enabled.
9. Confirm Niagara compile diagnostics are clean.
10. Confirm the effect remains visible with nonzero opacity/emissive defaults.

## Common Failure Modes

- Material names the DP channels but never connects the named outputs, so Niagara still shows generic or unused parameters.
- Tool connects only `Expression + OutputIndex` and loses the channel mask, causing type-width compile errors.
- Agent uses `RGBA` output for convenience, hiding the real artist-facing channel mapping.
- Agent uses DP for tint and alpha when `Particle Color` would be simpler.
- Agent creates too many DP nodes instead of packing four scalar controls into one index.
- Agent places a changing DP in `Particle Spawn` when it needs to animate over life.
- Agent places a one-time random value in `Particle Update`, causing unwanted jitter or per-frame instability.
- Agent creates Scratch Scripts for controls that can be handled with constants, random ranges, or float curves.
