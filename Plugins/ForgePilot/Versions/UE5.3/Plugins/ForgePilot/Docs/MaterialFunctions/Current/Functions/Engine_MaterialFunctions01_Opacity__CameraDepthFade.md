# CameraDepthFade

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/CameraDepthFade.CameraDepthFade`
- Category: Engine_MaterialFunctions01/Opacity
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Creates a gradient of 0 near the camera to white at Fade Length. Useful for preventing particles from camera clipping.0

## Inputs

- `Fade Length` [FunctionInput_Scalar] - How many units it takes to fade from 0 to 1
- `Fade Offset` [FunctionInput_Scalar] - Offsets the fade
- `For Vertex Shader` [FunctionInput_StaticBool] - Default = False. Computes depth using CameraPos instead of PixelDepth so that it is usable with WorldPositionOffset shaders

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 3 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Fade Length (Scalar) | Fade Length |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Fade Offset (Scalar) | Fade Offset |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | custom | `MaterialExpressionCustom` | Custom |  |
| 13 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input For Vertex Shader (StaticBool) | For Vertex Shader |
| 15 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | expression | `MaterialExpressionConstant3Vector` | 0,0,-1 |  |
| 18 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 19 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] Clamp output 0

### [4] Subtract

- `A` <= [3] PixelDepth output 0
- `B` <= [6] Input Fade Offset (Scalar) output 0

### [7] Divide

- `A` <= [4] Subtract output 0
- `B` <= [5] Input Fade Length (Scalar) output 0

### [8] Clamp

- `Input` <= [13] Switch output 0

### [9] Subtract

- `A` <= [2] Camera Position output 0
- `B` <= [1] Absolute World Position output 0

### [10] Subtract

- `A` <= [20] Multiply output 0
- `B` <= [6] Input Fade Offset (Scalar) output 0

### [11] Divide

- `A` <= [10] Subtract output 0
- `B` <= [5] Input Fade Length (Scalar) output 0

### [13] Switch

- `A` <= [11] Divide output 0
- `B` <= [7] Divide output 0
- `Value` <= [14] Input For Vertex Shader (StaticBool) output 0

### [14] Input For Vertex Shader (StaticBool)

- `Preview` <= [15] Static Bool (False) output 0

### [16] Normalize

- `VectorInput` <= [9] Subtract output 0

### [18] View Space to World Space TransformVector

- `Input` <= [17] 0,0,-1 output 0

### [19] Dot

- `A` <= [16] Normalize output 0
- `B` <= [18] View Space to World Space TransformVector output 0

### [20] Multiply

- `A` <= [12] Custom output 0
- `B` <= [19] Dot output 0

## Connection List

- [8] Clamp output 0 -> Output Result.A
- [3] PixelDepth output 0 -> Subtract.A
- [6] Input Fade Offset (Scalar) output 0 -> Subtract.B
- [4] Subtract output 0 -> Divide.A
- [5] Input Fade Length (Scalar) output 0 -> Divide.B
- [13] Switch output 0 -> Clamp.Input
- [2] Camera Position output 0 -> Subtract.A
- [1] Absolute World Position output 0 -> Subtract.B
- [20] Multiply output 0 -> Subtract.A
- [6] Input Fade Offset (Scalar) output 0 -> Subtract.B
- [10] Subtract output 0 -> Divide.A
- [5] Input Fade Length (Scalar) output 0 -> Divide.B
- [11] Divide output 0 -> Switch.A
- [7] Divide output 0 -> Switch.B
- [14] Input For Vertex Shader (StaticBool) output 0 -> Switch.Value
- [15] Static Bool (False) output 0 -> Input For Vertex Shader (StaticBool).Preview
- [9] Subtract output 0 -> Normalize.VectorInput
- [17] 0,0,-1 output 0 -> View Space to World Space TransformVector.Input
- [16] Normalize output 0 -> Dot.A
- [18] View Space to World Space TransformVector output 0 -> Dot.B
- [12] Custom output 0 -> Multiply.A
- [19] Dot output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [8] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


