# CameraVectorWithWPOOptions

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/CameraVectorWithWPOOptions.CameraVectorWithWPOOptions`
- Category: Engine_MaterialFunctions02/Utility
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Retrieves the camera vector with or without vertex shader offsets.

## Inputs

- `Use World Position Offsets` [FunctionInput_StaticBool] - Defaults to true

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 8 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Use World Position Offsets (StaticBool) | Use World Position Offsets |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Normalize output 0

### [2] Subtract

- `A` <= [3] Camera Position output 0
- `B` <= [1] Absolute World Position output 0

### [4] Normalize

- `VectorInput` <= [8] Switch output 0

### [6] Subtract

- `A` <= [3] Camera Position output 0
- `B` <= [5] Absolute World Position (Excluding Material Offsets) output 0

### [8] Switch

- `A` <= [2] Subtract output 0
- `B` <= [6] Subtract output 0
- `Value` <= [9] Input Use World Position Offsets (StaticBool) output 0

### [9] Input Use World Position Offsets (StaticBool)

- `Preview` <= [7] Static Bool (True) output 0

## Connection List

- [4] Normalize output 0 -> Output Result.A
- [3] Camera Position output 0 -> Subtract.A
- [1] Absolute World Position output 0 -> Subtract.B
- [8] Switch output 0 -> Normalize.VectorInput
- [3] Camera Position output 0 -> Subtract.A
- [5] Absolute World Position (Excluding Material Offsets) output 0 -> Subtract.B
- [2] Subtract output 0 -> Switch.A
- [6] Subtract output 0 -> Switch.B
- [9] Input Use World Position Offsets (StaticBool) output 0 -> Switch.Value
- [7] Static Bool (True) output 0 -> Input Use World Position Offsets (StaticBool).Preview

## Output Trace Roots

- `Result` is driven by [4] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
