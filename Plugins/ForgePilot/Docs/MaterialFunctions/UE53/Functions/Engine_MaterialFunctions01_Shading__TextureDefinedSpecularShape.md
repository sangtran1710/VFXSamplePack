# TextureDefinedSpecularShape

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/TextureDefinedSpecularShape.TextureDefinedSpecularShape`
- Category: Engine_MaterialFunctions01/Shading
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Aligns the Highlight shape to the reflected light

## Inputs

- `HighlightShape` [FunctionInput_Texture2D] - Should have clamped address and use the red channel
- `SpecularSharpness` [FunctionInput_Scalar] - Goes from 0-10
- `Normal` [FunctionInput_Vector3]
- `SpecularIntensity` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant` | 0 |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionConstant` | -1 |  |
| 4 | expression | `MaterialExpressionConstant` | 1 |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionConstant` | 1.5 |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 15 | expression | `MaterialExpressionLightVector` | Light Vector |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input HighlightShape (Texture2D) | HighlightShape |
| 17 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 18 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input SpecularSharpness (Scalar) | SpecularSharpness |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input SpecularIntensity (Vector3) | SpecularIntensity |
| 23 | expression | `MaterialExpressionClamp` | Clamp |  |
| 24 | expression | `MaterialExpressionClamp` | Clamp |  |
| 25 | expression | `MaterialExpressionClamp` | Clamp |  |
| 26 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 27 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [21] Multiply output 0

### [2] Lerp

- `A` <= [1] 0 output 0
- `B` <= [3] -1 output 0
- `Alpha` <= [27] Clamp output 0

### [5] Lerp

- `A` <= [4] 1 output 0
- `B` <= [6] 1.5 output 0
- `Alpha` <= [27] Clamp output 0

### [7] Lerp

- `A` <= [24] Clamp output 0
- `B` <= [5] Lerp output 0
- `Alpha` <= [18] Texture Sample output 1

### [9] Multiply

- `A` <= [8] 0.5 output 0
- `B` <= [13] Subtract output 0

### [10] Add

- `A` <= [9] Multiply output 0
- `B` <= [14] 0.5 output 0

### [11] Add

- `A` <= [26] Camera Vector output 0
- `B` <= [15] Light Vector output 0

### [12] Mask ( R G )

- `Input` <= [10] Add output 0

### [13] Subtract

- `A` <= [26] Camera Vector output 0
- `B` <= [20] Input Normal (Vector3) output 0

### [16] Input HighlightShape (Texture2D)

- `Preview` <= [17] Texture Object output 0

### [18] Texture Sample

- `Coordinates` <= [25] Clamp output 0
- `TextureObject` <= [16] Input HighlightShape (Texture2D) output 0

### [21] Multiply

- `A` <= [23] Clamp output 0
- `B` <= [22] Input SpecularIntensity (Vector3) output 0

### [23] Clamp

- `Input` <= [7] Lerp output 0

### [24] Clamp

- `Input` <= [2] Lerp output 0

### [25] Clamp

- `Input` <= [12] Mask ( R G ) output 0

### [27] Clamp

- `Input` <= [19] Input SpecularSharpness (Scalar) output 0

## Connection List

- [21] Multiply output 0 -> Output Result.A
- [1] 0 output 0 -> Lerp.A
- [3] -1 output 0 -> Lerp.B
- [27] Clamp output 0 -> Lerp.Alpha
- [4] 1 output 0 -> Lerp.A
- [6] 1.5 output 0 -> Lerp.B
- [27] Clamp output 0 -> Lerp.Alpha
- [24] Clamp output 0 -> Lerp.A
- [5] Lerp output 0 -> Lerp.B
- [18] Texture Sample output 1 -> Lerp.Alpha
- [8] 0.5 output 0 -> Multiply.A
- [13] Subtract output 0 -> Multiply.B
- [9] Multiply output 0 -> Add.A
- [14] 0.5 output 0 -> Add.B
- [26] Camera Vector output 0 -> Add.A
- [15] Light Vector output 0 -> Add.B
- [10] Add output 0 -> Mask ( R G ).Input
- [26] Camera Vector output 0 -> Subtract.A
- [20] Input Normal (Vector3) output 0 -> Subtract.B
- [17] Texture Object output 0 -> Input HighlightShape (Texture2D).Preview
- [25] Clamp output 0 -> Texture Sample.Coordinates
- [16] Input HighlightShape (Texture2D) output 0 -> Texture Sample.TextureObject
- [23] Clamp output 0 -> Multiply.A
- [22] Input SpecularIntensity (Vector3) output 0 -> Multiply.B
- [7] Lerp output 0 -> Clamp.Input
- [2] Lerp output 0 -> Clamp.Input
- [12] Mask ( R G ) output 0 -> Clamp.Input
- [19] Input SpecularSharpness (Scalar) output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [21] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
