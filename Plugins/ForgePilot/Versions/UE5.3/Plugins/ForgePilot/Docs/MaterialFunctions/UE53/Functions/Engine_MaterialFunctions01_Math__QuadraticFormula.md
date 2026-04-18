# QuadraticFormula

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/QuadraticFormula.QuadraticFormula`
- Category: Engine_MaterialFunctions01/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Solved a quadratic equation of the form ax^2 + bx + c = 0.

## Inputs

- `A` [FunctionInput_Scalar]
- `B` [FunctionInput_Scalar]
- `C` [FunctionInput_Scalar]

## Outputs

- `T0`
- `T1`
- `Discriminant`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output T0 | T0 |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output T1 | T1 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input A (Scalar) | A |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input B (Scalar) | B |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input C (Scalar) | C |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,4) |  |
| 12 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 15 | expression | `MaterialExpressionDivide` | Divide |  |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output Discriminant | Discriminant |

## Exact Input Wiring

### [0] Output T0

- `A` <= [5] Divide output 0

### [1] Output T1

- `A` <= [15] Divide output 0

### [5] Divide

- `A` <= [14] Subtract output 0
- `B` <= [13] Multiply(,2) output 0

### [6] Multiply(,-1)

- `A` <= [3] Input B (Scalar) output 0

### [7] Add

- `A` <= [6] Multiply(,-1) output 0
- `B` <= [12] Sqrt output 0

### [8] Multiply

- `A` <= [3] Input B (Scalar) output 0
- `B` <= [3] Input B (Scalar) output 0

### [9] Subtract

- `A` <= [8] Multiply output 0
- `B` <= [11] Multiply(,4) output 0

### [10] Multiply

- `A` <= [4] Input C (Scalar) output 0
- `B` <= [2] Input A (Scalar) output 0

### [11] Multiply(,4)

- `A` <= [10] Multiply output 0

### [12] Sqrt

- `Input` <= [9] Subtract output 0

### [13] Multiply(,2)

- `A` <= [2] Input A (Scalar) output 0

### [14] Subtract

- `A` <= [6] Multiply(,-1) output 0
- `B` <= [12] Sqrt output 0

### [15] Divide

- `A` <= [7] Add output 0
- `B` <= [13] Multiply(,2) output 0

### [16] Output Discriminant

- `A` <= [9] Subtract output 0

## Connection List

- [5] Divide output 0 -> Output T0.A
- [15] Divide output 0 -> Output T1.A
- [14] Subtract output 0 -> Divide.A
- [13] Multiply(,2) output 0 -> Divide.B
- [3] Input B (Scalar) output 0 -> Multiply(,-1).A
- [6] Multiply(,-1) output 0 -> Add.A
- [12] Sqrt output 0 -> Add.B
- [3] Input B (Scalar) output 0 -> Multiply.A
- [3] Input B (Scalar) output 0 -> Multiply.B
- [8] Multiply output 0 -> Subtract.A
- [11] Multiply(,4) output 0 -> Subtract.B
- [4] Input C (Scalar) output 0 -> Multiply.A
- [2] Input A (Scalar) output 0 -> Multiply.B
- [10] Multiply output 0 -> Multiply(,4).A
- [9] Subtract output 0 -> Sqrt.Input
- [2] Input A (Scalar) output 0 -> Multiply(,2).A
- [6] Multiply(,-1) output 0 -> Subtract.A
- [12] Sqrt output 0 -> Subtract.B
- [7] Add output 0 -> Divide.A
- [13] Multiply(,2) output 0 -> Divide.B
- [9] Subtract output 0 -> Output Discriminant.A

## Output Trace Roots

- `T0` is driven by [5] Divide
- `T1` is driven by [15] Divide
- `Discriminant` is driven by [9] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
