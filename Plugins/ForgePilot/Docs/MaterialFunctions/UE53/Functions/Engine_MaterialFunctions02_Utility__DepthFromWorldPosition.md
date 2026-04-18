# DepthFromWorldPosition

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DepthFromWorldPosition.DepthFromWorldPosition`
- Category: Engine_MaterialFunctions02/Utility
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the same value that Pixel Depth or Scene Depth Return but using an arbitrary world position.

## Inputs

- `World Position` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Dot output 0

### [1] Dot

- `A` <= [4] CameraDirectionVector output 0
- `B` <= [3] Subtract output 0

### [3] Subtract

- `A` <= [5] Input World Position (Vector3) output 0
- `B` <= [2] Camera Position output 0

### [5] Input World Position (Vector3)

- `Preview` <= [6] Absolute World Position (Excluding Material Offsets) output 0

## Connection List

- [1] Dot output 0 -> Output Result.A
- [4] CameraDirectionVector output 0 -> Dot.A
- [3] Subtract output 0 -> Dot.B
- [5] Input World Position (Vector3) output 0 -> Subtract.A
- [2] Camera Position output 0 -> Subtract.B
- [6] Absolute World Position (Excluding Material Offsets) output 0 -> Input World Position (Vector3).Preview

## Output Trace Roots

- `Result` is driven by [1] Dot

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
