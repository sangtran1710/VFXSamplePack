# WithinRangeFloat3

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRangeFloat3.WithinRangeFloat3`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns 1 if a value is within the provided range limits. 0 otherwise.

## Inputs

- `Value` [FunctionInput_Vector3]
- `RangeMax` [FunctionInput_Vector3]
- `RangeMin` [FunctionInput_Vector3]

## Outputs

- `AnyComponentsWithinRange` - Returns a vector with each components set to 1 if the component's value is within the provided range limits. 0 otherwise.
- `AllComponentsWithinRange` - Returns 1 if all components of the value vector are within the provided range. 0 otherwise.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output AnyComponentsWithinRange | AnyComponentsWithinRange |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Value (Vector3) | Value |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input RangeMax (Vector3) | RangeMax |
| 3 | expression | `MaterialExpressionStep` | Step |  |
| 4 | expression | `MaterialExpressionStep` | Step |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input RangeMin (Vector3) | RangeMin |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output AllComponentsWithinRange | AllComponentsWithinRange |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | SplitComponents |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionSaturate` | Saturate |  |

## Exact Input Wiring

### [0] Output AnyComponentsWithinRange

- `A` <= [13] Saturate output 0

### [3] Step

- `Y` <= [6] Input RangeMin (Vector3) output 0
- `X` <= [1] Input Value (Vector3) output 0

### [4] Step

- `Y` <= [1] Input Value (Vector3) output 0
- `X` <= [2] Input RangeMax (Vector3) output 0

### [5] Multiply

- `A` <= [3] Step output 0
- `B` <= [4] Step output 0

### [7] Multiply

- `A` <= [9] SplitComponents output 1
- `B` <= [9] SplitComponents output 2

### [8] Output AllComponentsWithinRange

- `A` <= [10] Multiply output 0

### [10] Multiply

- `A` <= [7] Multiply output 0
- `B` <= [9] SplitComponents output 3

### [11] Add

- `A` <= [9] SplitComponents output 1
- `B` <= [9] SplitComponents output 2

### [12] Add

- `A` <= [9] SplitComponents output 3
- `B` <= [11] Add output 0

### [13] Saturate

- `Input` <= [12] Add output 0

## Connection List

- [13] Saturate output 0 -> Output AnyComponentsWithinRange.A
- [6] Input RangeMin (Vector3) output 0 -> Step.Y
- [1] Input Value (Vector3) output 0 -> Step.X
- [1] Input Value (Vector3) output 0 -> Step.Y
- [2] Input RangeMax (Vector3) output 0 -> Step.X
- [3] Step output 0 -> Multiply.A
- [4] Step output 0 -> Multiply.B
- [9] SplitComponents output 1 -> Multiply.A
- [9] SplitComponents output 2 -> Multiply.B
- [10] Multiply output 0 -> Output AllComponentsWithinRange.A
- [7] Multiply output 0 -> Multiply.A
- [9] SplitComponents output 3 -> Multiply.B
- [9] SplitComponents output 1 -> Add.A
- [9] SplitComponents output 2 -> Add.B
- [9] SplitComponents output 3 -> Add.A
- [11] Add output 0 -> Add.B
- [12] Add output 0 -> Saturate.Input

## Output Trace Roots

- `AnyComponentsWithinRange` is driven by [13] Saturate
- `AllComponentsWithinRange` is driven by [10] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
