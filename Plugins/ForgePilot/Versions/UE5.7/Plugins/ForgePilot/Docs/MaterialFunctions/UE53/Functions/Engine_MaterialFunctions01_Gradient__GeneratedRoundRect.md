# GeneratedRoundRect

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/GeneratedRoundRect.GeneratedRoundRect`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Draws a box in 2D Space. Controls for size and falloff.

## Inputs

- `UV Coords` [FunctionInput_Vector2] - The Position Coordinates to measure the box. Default is World Position and can be left default in most cases.
- `Box Dimensions` [FunctionInput_Vector2] - Width and Height of the box
- `Center` [FunctionInput_Vector2] - The Center Point location of the box
- `CornerRadius` [FunctionInput_Scalar] - Falloff of the edge gradient in units.
- `Sharpness` [FunctionInput_Scalar] - The Center Point location of the box

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 2 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input UV Coords (Vector2) | UV Coords |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Box Dimensions (Vector2) | Box Dimensions |
| 10 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Center (Vector2) | Center |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input CornerRadius (Scalar) | CornerRadius |
| 14 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 15 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Sharpness (Scalar) | Sharpness |
| 19 | expression | `MaterialExpressionAdd` | Add(,1) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [17] Saturate output 0

### [1] Subtract

- `A` <= [5] Abs output 0
- `B` <= [10] Multiply(,0.5) output 0

### [2] Max(,0)

- `A` <= [1] Subtract output 0

### [4] Clamp

- `Input` <= [6] Divide output 0

### [5] Abs

- `Input` <= [12] Subtract output 0

### [6] Divide

- `A` <= [3] VectorLength output 1
- `B` <= [13] Input CornerRadius (Scalar) output 0

### [7] 1-x

- `Input` <= [4] Clamp output 0

### [8] Input UV Coords (Vector2)

- `Preview` <= [14] TexCoord[0] output 0

### [10] Multiply(,0.5)

- `A` <= [15] Subtract output 0

### [12] Subtract

- `A` <= [8] Input UV Coords (Vector2) output 0
- `B` <= [11] Input Center (Vector2) output 0

### [15] Subtract

- `A` <= [9] Input Box Dimensions (Vector2) output 0
- `B` <= [13] Input CornerRadius (Scalar) output 0

### [16] Multiply

- `A` <= [7] 1-x output 0
- `B` <= [19] Add(,1) output 0

### [17] Saturate

- `Input` <= [16] Multiply output 0

### [19] Add(,1)

- `A` <= [18] Input Sharpness (Scalar) output 0

## Connection List

- [17] Saturate output 0 -> Output Result.A
- [5] Abs output 0 -> Subtract.A
- [10] Multiply(,0.5) output 0 -> Subtract.B
- [1] Subtract output 0 -> Max(,0).A
- [6] Divide output 0 -> Clamp.Input
- [12] Subtract output 0 -> Abs.Input
- [3] VectorLength output 1 -> Divide.A
- [13] Input CornerRadius (Scalar) output 0 -> Divide.B
- [4] Clamp output 0 -> 1-x.Input
- [14] TexCoord[0] output 0 -> Input UV Coords (Vector2).Preview
- [15] Subtract output 0 -> Multiply(,0.5).A
- [8] Input UV Coords (Vector2) output 0 -> Subtract.A
- [11] Input Center (Vector2) output 0 -> Subtract.B
- [9] Input Box Dimensions (Vector2) output 0 -> Subtract.A
- [13] Input CornerRadius (Scalar) output 0 -> Subtract.B
- [7] 1-x output 0 -> Multiply.A
- [19] Add(,1) output 0 -> Multiply.B
- [16] Multiply output 0 -> Saturate.Input
- [18] Input Sharpness (Scalar) output 0 -> Add(,1).A

## Output Trace Roots

- `Result` is driven by [17] Saturate

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
