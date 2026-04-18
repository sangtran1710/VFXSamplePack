# WorldAlignedBlend

- Path: `/Engine/Functions/Engine_MaterialFunctions01/AlphaBlend/WorldAlignedBlend.WorldAlignedBlend`
- Category: Engine_MaterialFunctions01/AlphaBlend
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `In World Vector` [FunctionInput_Vector3] - Vector for Blend. Default = 0,0,1
- `Blend Bias` [FunctionInput_Scalar] - Shift the alpha either up or down.
- `Blend Sharpness` [FunctionInput_Scalar] - Blend Sharpness. Increasing this will cause the alpha to be brighter facing the vector.
- `In Explicit Normal` [FunctionInput_Vector3] - Explicit Normal. Useful for when blending normal maps and you still want a noisy blend
- `Clamped?` [FunctionInput_StaticBool] - Only needed when you want to use this data downstream to modulate another alpha and negatives values are desired.
- `Alpha` [FunctionInput_Scalar] - Lets you specify an alpha to apply on top of the world. Good use would be a Vertex Color.Default= 1 which means no alpha.

## Outputs

- `Alpha` - Default Output uses PixelNormalWS. Gives the best result but won't allow you to blend normal maps, since it would be a reentrant expression. Use one of the other two outputs for blending Normals.
- `w/ Explicit Normal` - Useful if you want a specific normal map to affect the blend. Useful if you need the added detail and need to blend a normal map. For a cliff/grass material use this and specify the Rock's normal on the input side.
- `w/Vertex Normals` - The simplest blend that will only be based on the mesh's vertices, not any normal maps. Will work anywhere.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 1 | expression | `MaterialExpressionClamp` | Clamp |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input In World Vector (Vector3) | In World Vector |
| 8 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Blend Bias (Scalar) | Blend Bias |
| 12 | expression | `MaterialExpressionConstant` | -1 |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Blend Sharpness (Scalar) | Blend Sharpness |
| 14 | expression | `MaterialExpressionConstant` | 2 |  |
| 15 | expression | `MaterialExpressionPixelNormalWS` | PixelNormalWS |  |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output w/ Explicit Normal | w/ Explicit Normal |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 20 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 21 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output w/Vertex Normals | w/Vertex Normals |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input In Explicit Normal (Vector3) | In Explicit Normal |
| 24 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 28 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 29 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 30 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 31 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 32 | expression | `MaterialExpressionClamp` | Clamp |  |
| 33 | expression | `MaterialExpressionClamp` | Clamp |  |
| 34 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 35 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 36 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 37 | function_input | `MaterialExpressionFunctionInput` | Input Clamped? (StaticBool) | Clamped? |
| 38 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 39 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 40 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 42 | expression | `MaterialExpressionConstant` | 1 |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 45 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 49 | expression | `MaterialExpressionAdd` | Add |  |
| 50 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 51 | expression | `MaterialExpressionAdd` | Add |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 53 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 54 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output Alpha

- `A` <= [34] Switch output 0

### [1] Clamp

- `Input` <= [51] Add output 0

### [2] Lerp

- `A` <= [13] Input Blend Sharpness (Scalar) output 0
- `B` <= [11] Input Blend Bias (Scalar) output 0
- `Alpha` <= [45] Multiply output 0

### [3] Add

- `A` <= [5] Multiply output 0
- `B` <= [4] 0.5 output 0

### [5] Multiply

- `A` <= [40] Dot output 0
- `B` <= [4] 0.5 output 0

### [6] Normalize

- `VectorInput` <= [7] Input In World Vector (Vector3) output 0

### [7] Input In World Vector (Vector3)

- `Preview` <= [8] 0,0,1 output 0

### [9] Normalize

- `VectorInput` <= [10] Tangent Space to World Space TransformVector output 0

### [10] Tangent Space to World Space TransformVector

- `Input` <= [8] 0,0,1 output 0

### [11] Input Blend Bias (Scalar)

- `Preview` <= [12] -1 output 0

### [13] Input Blend Sharpness (Scalar)

- `Preview` <= [14] 2 output 0

### [16] Output w/ Explicit Normal

- `A` <= [36] Switch output 0

### [17] Multiply

- `A` <= [21] Dot output 0
- `B` <= [19] 0.5 output 0

### [18] Add

- `A` <= [17] Multiply output 0
- `B` <= [19] 0.5 output 0

### [20] Lerp

- `A` <= [13] Input Blend Sharpness (Scalar) output 0
- `B` <= [11] Input Blend Bias (Scalar) output 0
- `Alpha` <= [44] Multiply output 0

### [21] Dot

- `A` <= [39] VertexNormalWS output 0
- `B` <= [6] Normalize output 0

### [22] Output w/Vertex Normals

- `A` <= [35] Switch output 0

### [23] Input In Explicit Normal (Vector3)

- `Preview` <= [24] 0,0,1 output 0

### [25] Multiply

- `A` <= [31] Dot output 0
- `B` <= [27] 0.5 output 0

### [26] Add

- `A` <= [25] Multiply output 0
- `B` <= [27] 0.5 output 0

### [28] Lerp

- `A` <= [13] Input Blend Sharpness (Scalar) output 0
- `B` <= [11] Input Blend Bias (Scalar) output 0
- `Alpha` <= [43] Multiply output 0

### [29] Tangent Space to World Space TransformVector

- `Input` <= [23] Input In Explicit Normal (Vector3) output 0

### [30] Normalize

- `VectorInput` <= [29] Tangent Space to World Space TransformVector output 0

### [31] Dot

- `A` <= [30] Normalize output 0
- `B` <= [6] Normalize output 0

### [32] Clamp

- `Input` <= [49] Add output 0

### [33] Clamp

- `Input` <= [47] Add output 0

### [34] Switch

- `A` <= [1] Clamp output 0
- `B` <= [51] Add output 0
- `Value` <= [37] Input Clamped? (StaticBool) output 0

### [35] Switch

- `A` <= [32] Clamp output 0
- `B` <= [49] Add output 0
- `Value` <= [37] Input Clamped? (StaticBool) output 0

### [36] Switch

- `A` <= [33] Clamp output 0
- `B` <= [47] Add output 0
- `Value` <= [37] Input Clamped? (StaticBool) output 0

### [37] Input Clamped? (StaticBool)

- `Preview` <= [38] Static Bool (True) output 0

### [40] Dot

- `A` <= [15] PixelNormalWS output 0
- `B` <= [6] Normalize output 0

### [41] Input Alpha (Scalar)

- `Preview` <= [42] 1 output 0

### [43] Multiply

- `A` <= [26] Add output 0
- `B` <= [41] Input Alpha (Scalar) output 0

### [44] Multiply

- `A` <= [18] Add output 0
- `B` <= [41] Input Alpha (Scalar) output 0

### [45] Multiply

- `A` <= [3] Add output 0
- `B` <= [41] Input Alpha (Scalar) output 0

### [46] Multiply

- `A` <= [43] Multiply output 0
- `B` <= [13] Input Blend Sharpness (Scalar) output 0

### [47] Add

- `A` <= [46] Multiply output 0
- `B` <= [53] Subtract output 0

### [48] Multiply

- `A` <= [18] Add output 0
- `B` <= [13] Input Blend Sharpness (Scalar) output 0

### [49] Add

- `A` <= [48] Multiply output 0
- `B` <= [53] Subtract output 0

### [50] Multiply

- `A` <= [3] Add output 0
- `B` <= [13] Input Blend Sharpness (Scalar) output 0

### [51] Add

- `A` <= [50] Multiply output 0
- `B` <= [53] Subtract output 0

### [52] Multiply(,0.5)

- `A` <= [13] Input Blend Sharpness (Scalar) output 0

### [53] Subtract

- `A` <= [11] Input Blend Bias (Scalar) output 0
- `B` <= [52] Multiply(,0.5) output 0

### [54] Divide

- `A` <= [11] Input Blend Bias (Scalar) output 0
- `B` <= [52] Multiply(,0.5) output 0

## Connection List

- [34] Switch output 0 -> Output Alpha.A
- [51] Add output 0 -> Clamp.Input
- [13] Input Blend Sharpness (Scalar) output 0 -> Lerp.A
- [11] Input Blend Bias (Scalar) output 0 -> Lerp.B
- [45] Multiply output 0 -> Lerp.Alpha
- [5] Multiply output 0 -> Add.A
- [4] 0.5 output 0 -> Add.B
- [40] Dot output 0 -> Multiply.A
- [4] 0.5 output 0 -> Multiply.B
- [7] Input In World Vector (Vector3) output 0 -> Normalize.VectorInput
- [8] 0,0,1 output 0 -> Input In World Vector (Vector3).Preview
- [10] Tangent Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [8] 0,0,1 output 0 -> Tangent Space to World Space TransformVector.Input
- [12] -1 output 0 -> Input Blend Bias (Scalar).Preview
- [14] 2 output 0 -> Input Blend Sharpness (Scalar).Preview
- [36] Switch output 0 -> Output w/ Explicit Normal.A
- [21] Dot output 0 -> Multiply.A
- [19] 0.5 output 0 -> Multiply.B
- [17] Multiply output 0 -> Add.A
- [19] 0.5 output 0 -> Add.B
- [13] Input Blend Sharpness (Scalar) output 0 -> Lerp.A
- [11] Input Blend Bias (Scalar) output 0 -> Lerp.B
- [44] Multiply output 0 -> Lerp.Alpha
- [39] VertexNormalWS output 0 -> Dot.A
- [6] Normalize output 0 -> Dot.B
- [35] Switch output 0 -> Output w/Vertex Normals.A
- [24] 0,0,1 output 0 -> Input In Explicit Normal (Vector3).Preview
- [31] Dot output 0 -> Multiply.A
- [27] 0.5 output 0 -> Multiply.B
- [25] Multiply output 0 -> Add.A
- [27] 0.5 output 0 -> Add.B
- [13] Input Blend Sharpness (Scalar) output 0 -> Lerp.A
- [11] Input Blend Bias (Scalar) output 0 -> Lerp.B
- [43] Multiply output 0 -> Lerp.Alpha
- [23] Input In Explicit Normal (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [29] Tangent Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [30] Normalize output 0 -> Dot.A
- [6] Normalize output 0 -> Dot.B
- [49] Add output 0 -> Clamp.Input
- [47] Add output 0 -> Clamp.Input
- [1] Clamp output 0 -> Switch.A
- [51] Add output 0 -> Switch.B
- [37] Input Clamped? (StaticBool) output 0 -> Switch.Value
- [32] Clamp output 0 -> Switch.A
- [49] Add output 0 -> Switch.B
- [37] Input Clamped? (StaticBool) output 0 -> Switch.Value
- [33] Clamp output 0 -> Switch.A
- [47] Add output 0 -> Switch.B
- [37] Input Clamped? (StaticBool) output 0 -> Switch.Value
- [38] Static Bool (True) output 0 -> Input Clamped? (StaticBool).Preview
- [15] PixelNormalWS output 0 -> Dot.A
- [6] Normalize output 0 -> Dot.B
- [42] 1 output 0 -> Input Alpha (Scalar).Preview
- [26] Add output 0 -> Multiply.A
- [41] Input Alpha (Scalar) output 0 -> Multiply.B
- [18] Add output 0 -> Multiply.A
- [41] Input Alpha (Scalar) output 0 -> Multiply.B
- [3] Add output 0 -> Multiply.A
- [41] Input Alpha (Scalar) output 0 -> Multiply.B
- [43] Multiply output 0 -> Multiply.A
- [13] Input Blend Sharpness (Scalar) output 0 -> Multiply.B
- [46] Multiply output 0 -> Add.A
- [53] Subtract output 0 -> Add.B
- [18] Add output 0 -> Multiply.A
- [13] Input Blend Sharpness (Scalar) output 0 -> Multiply.B
- [48] Multiply output 0 -> Add.A
- [53] Subtract output 0 -> Add.B
- [3] Add output 0 -> Multiply.A
- [13] Input Blend Sharpness (Scalar) output 0 -> Multiply.B
- [50] Multiply output 0 -> Add.A
- [53] Subtract output 0 -> Add.B
- [13] Input Blend Sharpness (Scalar) output 0 -> Multiply(,0.5).A
- [11] Input Blend Bias (Scalar) output 0 -> Subtract.A
- [52] Multiply(,0.5) output 0 -> Subtract.B
- [11] Input Blend Bias (Scalar) output 0 -> Divide.A
- [52] Multiply(,0.5) output 0 -> Divide.B

## Output Trace Roots

- `Alpha` is driven by [34] Switch
- `w/ Explicit Normal` is driven by [36] Switch
- `w/Vertex Normals` is driven by [35] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
