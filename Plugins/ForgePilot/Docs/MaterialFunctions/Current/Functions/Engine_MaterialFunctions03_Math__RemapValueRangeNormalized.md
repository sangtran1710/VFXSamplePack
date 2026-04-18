# RemapValueRangeNormalized

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRangeNormalized.RemapValueRangeNormalized`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Generalized remapping function to explicitly remap an input range to 0 - 1.

## Inputs

- `Input` [FunctionInput_Scalar]
- `Input Low` [FunctionInput_Scalar]
- `Input High` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Input (Scalar) | Input |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Input Low (Scalar) | Input Low |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Input High (Scalar) | Input High |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionSaturate` | Saturate |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Saturate output 0

### [4] Subtract

- `A` <= [1] Input Input (Scalar) output 0
- `B` <= [2] Input Input Low (Scalar) output 0

### [5] Divide

- `A` <= [4] Subtract output 0
- `B` <= [6] Subtract output 0

### [6] Subtract

- `A` <= [3] Input Input High (Scalar) output 0
- `B` <= [2] Input Input Low (Scalar) output 0

### [7] Saturate

- `Input` <= [5] Divide output 0

## Connection List

- [7] Saturate output 0 -> Output Result.A
- [1] Input Input (Scalar) output 0 -> Subtract.A
- [2] Input Input Low (Scalar) output 0 -> Subtract.B
- [4] Subtract output 0 -> Divide.A
- [6] Subtract output 0 -> Divide.B
- [3] Input Input High (Scalar) output 0 -> Subtract.A
- [2] Input Input Low (Scalar) output 0 -> Subtract.B
- [5] Divide output 0 -> Saturate.Input

## Output Trace Roots

- `Result` is driven by [7] Saturate

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


