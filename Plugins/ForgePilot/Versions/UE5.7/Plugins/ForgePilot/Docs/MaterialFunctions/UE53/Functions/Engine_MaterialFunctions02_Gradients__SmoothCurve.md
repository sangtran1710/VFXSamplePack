# SmoothCurve

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/SmoothCurve.SmoothCurve`
- Category: Engine_MaterialFunctions02/Gradients
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Adjust the tangents at x0 and x1 to adjust x's curve.

## Inputs

- `Tangent 0` [FunctionInput_Scalar]
- `Tangent 1` [FunctionInput_Scalar]
- `X` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionConstant` | 2 |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionConstant` | 3 |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Tangent 0 (Scalar) | Tangent 0 |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Tangent 1 (Scalar) | Tangent 1 |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input X (Scalar) | X |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 20 | expression | `MaterialExpressionConstant` | 2 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Add output 0

### [1] Multiply

- `A` <= [18] Input X (Scalar) output 0
- `B` <= [16] Input Tangent 0 (Scalar) output 0

### [2] Add

- `A` <= [1] Multiply output 0
- `B` <= [8] Add output 0

### [3] Multiply

- `A` <= [18] Input X (Scalar) output 0
- `B` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [18] Input X (Scalar) output 0
- `B` <= [6] Multiply output 0

### [5] Add

- `A` <= [16] Input Tangent 0 (Scalar) output 0
- `B` <= [17] Input Tangent 1 (Scalar) output 0

### [6] Multiply

- `A` <= [18] Input X (Scalar) output 0
- `B` <= [7] Subtract output 0

### [7] Subtract

- `A` <= [5] Add output 0
- `B` <= [9] 2 output 0

### [8] Add

- `A` <= [3] Multiply output 0
- `B` <= [10] Multiply output 0

### [10] Multiply

- `A` <= [18] Input X (Scalar) output 0
- `B` <= [11] Multiply output 0

### [11] Multiply

- `A` <= [18] Input X (Scalar) output 0
- `B` <= [15] Multiply output 0

### [12] Subtract

- `A` <= [13] 3 output 0
- `B` <= [17] Input Tangent 1 (Scalar) output 0

### [14] Subtract

- `A` <= [12] Subtract output 0
- `B` <= [9] 2 output 0

### [15] Multiply

- `A` <= [14] Subtract output 0
- `B` <= [16] Input Tangent 0 (Scalar) output 0

### [16] Input Tangent 0 (Scalar)

- `Preview` <= [20] 2 output 0

### [17] Input Tangent 1 (Scalar)

- `Preview` <= [20] 2 output 0

### [18] Input X (Scalar)

- `Preview` <= [19] LinearGradient output 0

## Connection List

- [2] Add output 0 -> Output Result.A
- [18] Input X (Scalar) output 0 -> Multiply.A
- [16] Input Tangent 0 (Scalar) output 0 -> Multiply.B
- [1] Multiply output 0 -> Add.A
- [8] Add output 0 -> Add.B
- [18] Input X (Scalar) output 0 -> Multiply.A
- [4] Multiply output 0 -> Multiply.B
- [18] Input X (Scalar) output 0 -> Multiply.A
- [6] Multiply output 0 -> Multiply.B
- [16] Input Tangent 0 (Scalar) output 0 -> Add.A
- [17] Input Tangent 1 (Scalar) output 0 -> Add.B
- [18] Input X (Scalar) output 0 -> Multiply.A
- [7] Subtract output 0 -> Multiply.B
- [5] Add output 0 -> Subtract.A
- [9] 2 output 0 -> Subtract.B
- [3] Multiply output 0 -> Add.A
- [10] Multiply output 0 -> Add.B
- [18] Input X (Scalar) output 0 -> Multiply.A
- [11] Multiply output 0 -> Multiply.B
- [18] Input X (Scalar) output 0 -> Multiply.A
- [15] Multiply output 0 -> Multiply.B
- [13] 3 output 0 -> Subtract.A
- [17] Input Tangent 1 (Scalar) output 0 -> Subtract.B
- [12] Subtract output 0 -> Subtract.A
- [9] 2 output 0 -> Subtract.B
- [14] Subtract output 0 -> Multiply.A
- [16] Input Tangent 0 (Scalar) output 0 -> Multiply.B
- [20] 2 output 0 -> Input Tangent 0 (Scalar).Preview
- [20] 2 output 0 -> Input Tangent 1 (Scalar).Preview
- [19] LinearGradient output 0 -> Input X (Scalar).Preview

## Output Trace Roots

- `Result` is driven by [2] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
