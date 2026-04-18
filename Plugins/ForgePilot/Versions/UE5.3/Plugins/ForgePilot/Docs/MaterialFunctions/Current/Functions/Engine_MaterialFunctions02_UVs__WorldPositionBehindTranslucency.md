# WorldPositionBehindTranslucency

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/WorldPositionBehindTranslucency.WorldPositionBehindTranslucency`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This will recreate the scenes world position behind a translucent surface. 

## Inputs

- `Exclude WPO Offsets` [FunctionInput_StaticBool] - False by default.
- `SceneDepth` [FunctionInput_Scalar] - Allows overriding scene depth with custom sampling.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionSceneDepth` | Scene Depth |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 9 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Exclude WPO Offsets (StaticBool) | Exclude WPO Offsets |
| 11 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 12 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 13 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 14 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldPositionWithScale |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 18 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input SceneDepth (Scalar) | SceneDepth |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Add output 0

### [2] Divide

- `A` <= [19] Input SceneDepth (Scalar) output 0
- `B` <= [3] PixelDepth output 0

### [4] Multiply

- `A` <= [2] Divide output 0
- `B` <= [6] Subtract output 0

### [6] Subtract

- `A` <= [12] Switch output 0
- `B` <= [8] Camera Position output 0

### [7] Add

- `A` <= [4] Multiply output 0
- `B` <= [8] Camera Position output 0

### [10] Input Exclude WPO Offsets (StaticBool)

- `Preview` <= [9] Static Bool (False) output 0

### [12] Switch

- `A` <= [11] Absolute World Position (Excluding Material Offsets) output 0
- `B` <= [5] Absolute World Position output 0
- `Value` <= [10] Input Exclude WPO Offsets (StaticBool) output 0

### [13] Dot

- `A` <= [17] CameraDirectionVector output 0
- `B` <= [16] Subtract output 0

### [16] Subtract

- `A` <= [15] WorldPositionWithScale output 0
- `B` <= [14] Camera Position output 0

### [19] Input SceneDepth (Scalar)

- `Preview` <= [1] Scene Depth output 0

## Connection List

- [7] Add output 0 -> Output Result.A
- [19] Input SceneDepth (Scalar) output 0 -> Divide.A
- [3] PixelDepth output 0 -> Divide.B
- [2] Divide output 0 -> Multiply.A
- [6] Subtract output 0 -> Multiply.B
- [12] Switch output 0 -> Subtract.A
- [8] Camera Position output 0 -> Subtract.B
- [4] Multiply output 0 -> Add.A
- [8] Camera Position output 0 -> Add.B
- [9] Static Bool (False) output 0 -> Input Exclude WPO Offsets (StaticBool).Preview
- [11] Absolute World Position (Excluding Material Offsets) output 0 -> Switch.A
- [5] Absolute World Position output 0 -> Switch.B
- [10] Input Exclude WPO Offsets (StaticBool) output 0 -> Switch.Value
- [17] CameraDirectionVector output 0 -> Dot.A
- [16] Subtract output 0 -> Dot.B
- [15] WorldPositionWithScale output 0 -> Subtract.A
- [14] Camera Position output 0 -> Subtract.B
- [1] Scene Depth output 0 -> Input SceneDepth (Scalar).Preview

## Output Trace Roots

- `Result` is driven by [7] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


