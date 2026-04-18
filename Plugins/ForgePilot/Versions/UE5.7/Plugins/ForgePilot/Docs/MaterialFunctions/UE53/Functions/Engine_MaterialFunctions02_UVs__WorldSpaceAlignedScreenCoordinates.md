# WorldSpaceAlignedScreenCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/WorldSpaceAlignedScreenCoordinates.WorldSpaceAlignedScreenCoordinates`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Moves the center of screen aligned coordinates to the center of a world position. A second output scales the results based on the users fov and view size to maintain a consistent look across multiple resolutions.

## Inputs

- `Center Point World Position` [FunctionInput_Vector3] - Defaults to the component's pivot point.

## Outputs

- `Offset UVs`
- `Offset and Scaled UVS` - This output is scaled based on the camera's fov and the view size. Which will produce more predictable results. For most use cases, this value should be multiplied by a large number.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Offset UVs | Offset UVs |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | TransformToClipSpace |  |
| 2 | expression | `MaterialExpressionScreenPosition` | ScreenPosition |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionDistance` | Distance |  |
| 5 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | ScreenResolution |  |
| 12 | expression | `MaterialExpressionConstant` | 1 |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Center Point World Position (Vector3) | Center Point World Position |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Offset and Scaled UVS | Offset and Scaled UVS |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 17 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | FOV |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | ComponentPivotLocation |  |
| 20 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output Offset UVs

- `A` <= [7] Multiply output 0

### [3] Subtract

- `A` <= [2] ScreenPosition output 0
- `B` <= [1] TransformToClipSpace output 0

### [4] Distance

- `A` <= [5] Camera Position output 0
- `B` <= [13] Input Center Point World Position (Vector3) output 0

### [6] Multiply

- `A` <= [4] Distance output 0
- `B` <= [3] Subtract output 0

### [7] Multiply

- `A` <= [6] Multiply output 0
- `B` <= [8] Append output 0

### [8] Append

- `A` <= [9] Divide output 0
- `B` <= [12] 1 output 0

### [9] Divide

- `A` <= [10] BreakOutFloat2Components output 0
- `B` <= [10] BreakOutFloat2Components output 1

### [13] Input Center Point World Position (Vector3)

- `Preview` <= [19] ComponentPivotLocation output 0

### [14] Output Offset and Scaled UVS

- `A` <= [15] Multiply output 0

### [15] Multiply

- `A` <= [0] Output Offset UVs output 0
- `B` <= [20] Divide output 0

### [16] Mask ( R )

- `Input` <= [17] ViewSize output 0

### [20] Divide

- `A` <= [18] FOV output 0
- `B` <= [16] Mask ( R ) output 0

## Connection List

- [7] Multiply output 0 -> Output Offset UVs.A
- [2] ScreenPosition output 0 -> Subtract.A
- [1] TransformToClipSpace output 0 -> Subtract.B
- [5] Camera Position output 0 -> Distance.A
- [13] Input Center Point World Position (Vector3) output 0 -> Distance.B
- [4] Distance output 0 -> Multiply.A
- [3] Subtract output 0 -> Multiply.B
- [6] Multiply output 0 -> Multiply.A
- [8] Append output 0 -> Multiply.B
- [9] Divide output 0 -> Append.A
- [12] 1 output 0 -> Append.B
- [10] BreakOutFloat2Components output 0 -> Divide.A
- [10] BreakOutFloat2Components output 1 -> Divide.B
- [19] ComponentPivotLocation output 0 -> Input Center Point World Position (Vector3).Preview
- [15] Multiply output 0 -> Output Offset and Scaled UVS.A
- [0] Output Offset UVs output 0 -> Multiply.A
- [20] Divide output 0 -> Multiply.B
- [17] ViewSize output 0 -> Mask ( R ).Input
- [18] FOV output 0 -> Divide.A
- [16] Mask ( R ) output 0 -> Divide.B

## Output Trace Roots

- `Offset UVs` is driven by [7] Multiply
- `Offset and Scaled UVS` is driven by [15] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
