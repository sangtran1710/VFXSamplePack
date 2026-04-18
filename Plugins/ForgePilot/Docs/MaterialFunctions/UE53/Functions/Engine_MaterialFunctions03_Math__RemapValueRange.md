# RemapValueRange

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRange.RemapValueRange`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Generalized remapping function to explicitly remap an input range to an output range.

## Inputs

- `Input` [FunctionInput_Scalar]
- `Input Low` [FunctionInput_Scalar]
- `Input High` [FunctionInput_Scalar]
- `Target Low` [FunctionInput_Scalar]
- `Target High` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Input (Scalar) | Input |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Input Low (Scalar) | Input Low |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Input High (Scalar) | Input High |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Target Low (Scalar) | Target Low |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Target High (Scalar) | Target High |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [11] Add output 0

### [6] Subtract

- `A` <= [1] Input Input (Scalar) output 0
- `B` <= [2] Input Input Low (Scalar) output 0

### [7] Divide

- `A` <= [6] Subtract output 0
- `B` <= [8] Subtract output 0

### [8] Subtract

- `A` <= [3] Input Input High (Scalar) output 0
- `B` <= [2] Input Input Low (Scalar) output 0

### [9] Multiply

- `A` <= [7] Divide output 0
- `B` <= [10] Subtract output 0

### [10] Subtract

- `A` <= [5] Input Target High (Scalar) output 0
- `B` <= [4] Input Target Low (Scalar) output 0

### [11] Add

- `A` <= [9] Multiply output 0
- `B` <= [4] Input Target Low (Scalar) output 0

## Connection List

- [11] Add output 0 -> Output Result.A
- [1] Input Input (Scalar) output 0 -> Subtract.A
- [2] Input Input Low (Scalar) output 0 -> Subtract.B
- [6] Subtract output 0 -> Divide.A
- [8] Subtract output 0 -> Divide.B
- [3] Input Input High (Scalar) output 0 -> Subtract.A
- [2] Input Input Low (Scalar) output 0 -> Subtract.B
- [7] Divide output 0 -> Multiply.A
- [10] Subtract output 0 -> Multiply.B
- [5] Input Target High (Scalar) output 0 -> Subtract.A
- [4] Input Target Low (Scalar) output 0 -> Subtract.B
- [9] Multiply output 0 -> Add.A
- [4] Input Target Low (Scalar) output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [11] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
