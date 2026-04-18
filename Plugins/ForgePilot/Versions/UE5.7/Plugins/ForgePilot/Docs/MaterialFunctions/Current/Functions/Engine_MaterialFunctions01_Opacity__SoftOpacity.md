# SoftOpacity

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/SoftOpacity.SoftOpacity`
- Category: Engine_MaterialFunctions01/Opacity
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Softens the Opacity input by multiplying against Fresnel, DepthBiasedAlpha and Pixel Depth

## Inputs

- `OpacityIn` [FunctionInput_Scalar] - The Opacity Value you want to be Softened
- `FadeDistance` [FunctionInput_Scalar] - How close you should get to the surface before it starts fading out DEFAULT=512
- `DepthFadeDistance` [FunctionInput_Scalar]

## Outputs

- `OutputUsesDepthBias` - Cost 12 more instructions
- `OutputNoDepthBias` - Cost 12 fewer Instructions

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output OutputUsesDepthBias | OutputUsesDepthBias |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 4 | expression | `MaterialExpressionFresnel` | Fresnel |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input OpacityIn (Scalar) | OpacityIn |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 7 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input FadeDistance (Scalar) | FadeDistance |
| 9 | expression | `MaterialExpressionConstant` | 0 |  |
| 10 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output OutputNoDepthBias | OutputNoDepthBias |
| 12 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 13 | expression | `MaterialExpressionDepthFade` | Depth Fade |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input DepthFadeDistance (Scalar) | DepthFadeDistance |

## Exact Input Wiring

### [0] Output OutputUsesDepthBias

- `A` <= [13] Depth Fade output 0

### [1] Multiply

- `A` <= [6] 1-x output 0
- `B` <= [2] Multiply output 0

### [2] Multiply

- `A` <= [5] Input OpacityIn (Scalar) output 0
- `B` <= [3] 1-x output 0

### [3] 1-x

- `Input` <= [4] Fresnel output 0

### [4] Fresnel

- `Normal` <= [12] VertexNormalWS output 0

### [6] 1-x

- `Input` <= [7] SphereMask output 0

### [7] SphereMask

- `A` <= [10] PixelDepth output 0
- `B` <= [9] 0 output 0
- `Radius` <= [8] Input FadeDistance (Scalar) output 0

### [11] Output OutputNoDepthBias

- `A` <= [1] Multiply output 0

### [13] Depth Fade

- `InOpacity` <= [1] Multiply output 0
- `FadeDistance` <= [14] Input DepthFadeDistance (Scalar) output 0

## Connection List

- [13] Depth Fade output 0 -> Output OutputUsesDepthBias.A
- [6] 1-x output 0 -> Multiply.A
- [2] Multiply output 0 -> Multiply.B
- [5] Input OpacityIn (Scalar) output 0 -> Multiply.A
- [3] 1-x output 0 -> Multiply.B
- [4] Fresnel output 0 -> 1-x.Input
- [12] VertexNormalWS output 0 -> Fresnel.Normal
- [7] SphereMask output 0 -> 1-x.Input
- [10] PixelDepth output 0 -> SphereMask.A
- [9] 0 output 0 -> SphereMask.B
- [8] Input FadeDistance (Scalar) output 0 -> SphereMask.Radius
- [1] Multiply output 0 -> Output OutputNoDepthBias.A
- [1] Multiply output 0 -> Depth Fade.InOpacity
- [14] Input DepthFadeDistance (Scalar) output 0 -> Depth Fade.FadeDistance

## Output Trace Roots

- `OutputUsesDepthBias` is driven by [13] Depth Fade
- `OutputNoDepthBias` is driven by [1] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


