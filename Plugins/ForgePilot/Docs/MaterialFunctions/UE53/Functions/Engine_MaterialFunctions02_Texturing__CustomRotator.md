# CustomRotator

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/CustomRotator.CustomRotator`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Rotator with rotation center exposed and a rotation angle on 0-1 scale. A rotation angle value of 1 is equal to one full rotation.

## Inputs

- `Rotation Center` [FunctionInput_Vector2] - Enter a number to be used as the center point of rotation.
- `Rotation Angle (0-1)` [FunctionInput_Scalar] - Enter 0-1 values for rotation. A value of 1 is equal to a 360 degree turn.
- `UVs` [FunctionInput_Vector2] - Insert UVs, world coordinates or other values to rotate.

## Outputs

- `Rotated Values`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Rotated Values | Rotated Values |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Center (Vector2) | Rotation Center |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionConstant` | -1 |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionCosine` | Cosine |  |
| 9 | expression | `MaterialExpressionSine` | Sine |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant` | -1 |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Angle (0-1) (Scalar) | Rotation Angle (0-1) |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |

## Exact Input Wiring

### [0] Output Rotated Values

- `A` <= [15] Add output 0

### [2] Multiply

- `A` <= [1] Input Rotation Center (Vector2) output 0
- `B` <= [3] -1 output 0

### [4] Add

- `A` <= [2] Multiply output 0
- `B` <= [16] Input UVs (Vector2) output 0

### [5] Dot

- `A` <= [4] Add output 0
- `B` <= [10] Append output 0

### [6] Dot

- `A` <= [4] Add output 0
- `B` <= [7] Append output 0

### [7] Append

- `A` <= [8] Cosine output 0
- `B` <= [11] Multiply output 0

### [8] Cosine

- `Input` <= [13] Input Rotation Angle (0-1) (Scalar) output 0

### [9] Sine

- `Input` <= [13] Input Rotation Angle (0-1) (Scalar) output 0

### [10] Append

- `A` <= [9] Sine output 0
- `B` <= [8] Cosine output 0

### [11] Multiply

- `A` <= [9] Sine output 0
- `B` <= [12] -1 output 0

### [14] Append

- `A` <= [6] Dot output 0
- `B` <= [5] Dot output 0

### [15] Add

- `A` <= [1] Input Rotation Center (Vector2) output 0
- `B` <= [14] Append output 0

## Connection List

- [15] Add output 0 -> Output Rotated Values.A
- [1] Input Rotation Center (Vector2) output 0 -> Multiply.A
- [3] -1 output 0 -> Multiply.B
- [2] Multiply output 0 -> Add.A
- [16] Input UVs (Vector2) output 0 -> Add.B
- [4] Add output 0 -> Dot.A
- [10] Append output 0 -> Dot.B
- [4] Add output 0 -> Dot.A
- [7] Append output 0 -> Dot.B
- [8] Cosine output 0 -> Append.A
- [11] Multiply output 0 -> Append.B
- [13] Input Rotation Angle (0-1) (Scalar) output 0 -> Cosine.Input
- [13] Input Rotation Angle (0-1) (Scalar) output 0 -> Sine.Input
- [9] Sine output 0 -> Append.A
- [8] Cosine output 0 -> Append.B
- [9] Sine output 0 -> Multiply.A
- [12] -1 output 0 -> Multiply.B
- [6] Dot output 0 -> Append.A
- [5] Dot output 0 -> Append.B
- [1] Input Rotation Center (Vector2) output 0 -> Add.A
- [14] Append output 0 -> Add.B

## Output Trace Roots

- `Rotated Values` is driven by [15] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
