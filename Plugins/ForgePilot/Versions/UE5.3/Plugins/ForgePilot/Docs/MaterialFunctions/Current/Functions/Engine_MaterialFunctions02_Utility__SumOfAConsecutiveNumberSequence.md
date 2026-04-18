# SumOfAConsecutiveNumberSequence

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/SumOfAConsecutiveNumberSequence.SumOfAConsecutiveNumberSequence`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This will find the sum of numbers up to the integer inserted. I.e. [ 1+2+3...+ n ].

## Inputs

- `Number of integers` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Number of integers (Scalar) | Number of integers |
| 2 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionDivide` | Divide(,2) |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Divide(,2) output 0

### [2] Add(,1)

- `A` <= [5] Abs output 0

### [3] Multiply

- `A` <= [2] Add(,1) output 0
- `B` <= [5] Abs output 0

### [4] Divide(,2)

- `A` <= [3] Multiply output 0

### [5] Abs

- `Input` <= [1] Input Number of integers (Scalar) output 0

## Connection List

- [4] Divide(,2) output 0 -> Output Result.A
- [5] Abs output 0 -> Add(,1).A
- [2] Add(,1) output 0 -> Multiply.A
- [5] Abs output 0 -> Multiply.B
- [3] Multiply output 0 -> Divide(,2).A
- [1] Input Number of integers (Scalar) output 0 -> Abs.Input

## Output Trace Roots

- `Result` is driven by [4] Divide(,2)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


