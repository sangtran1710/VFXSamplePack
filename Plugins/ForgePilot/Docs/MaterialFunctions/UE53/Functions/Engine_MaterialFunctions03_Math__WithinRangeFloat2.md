# WithinRangeFloat2

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRangeFloat2.WithinRangeFloat2`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns 1 if a value is within the provided range limits. 0 otherwise.

## Inputs

- `Value` [FunctionInput_Vector2]
- `RangeMax` [FunctionInput_Vector2]
- `RangeMin` [FunctionInput_Vector2]

## Outputs

- `AnyComponentsWithinRange` - Returns a vector with each components set to 1 if the component's value is within the provided range limits. 0 otherwise.
- `AllComponentsWithinRange` - Returns 1 if all components of the value vector are within the provided range. 0 otherwise.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output AnyComponentsWithinRange | AnyComponentsWithinRange |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Value (Vector2) | Value |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input RangeMax (Vector2) | RangeMax |
| 3 | expression | `MaterialExpressionStep` | Step |  |
| 4 | expression | `MaterialExpressionStep` | Step |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input RangeMin (Vector2) | RangeMin |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output AllComponentsWithinRange | AllComponentsWithinRange |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionSaturate` | Saturate |  |

## Exact Input Wiring

### [0] Output AnyComponentsWithinRange

- `A` <= [12] Saturate output 0

### [3] Step

- `Y` <= [6] Input RangeMin (Vector2) output 0
- `X` <= [1] Input Value (Vector2) output 0

### [4] Step

- `Y` <= [1] Input Value (Vector2) output 0
- `X` <= [2] Input RangeMax (Vector2) output 0

### [5] Multiply

- `A` <= [3] Step output 0
- `B` <= [4] Step output 0

### [7] Mask ( R )

- `Input` <= [5] Multiply output 0

### [8] Mask ( G )

- `Input` <= [5] Multiply output 0

### [9] Multiply

- `A` <= [7] Mask ( R ) output 0
- `B` <= [8] Mask ( G ) output 0

### [10] Output AllComponentsWithinRange

- `A` <= [9] Multiply output 0

### [11] Add

- `A` <= [7] Mask ( R ) output 0
- `B` <= [8] Mask ( G ) output 0

### [12] Saturate

- `Input` <= [11] Add output 0

## Connection List

- [12] Saturate output 0 -> Output AnyComponentsWithinRange.A
- [6] Input RangeMin (Vector2) output 0 -> Step.Y
- [1] Input Value (Vector2) output 0 -> Step.X
- [1] Input Value (Vector2) output 0 -> Step.Y
- [2] Input RangeMax (Vector2) output 0 -> Step.X
- [3] Step output 0 -> Multiply.A
- [4] Step output 0 -> Multiply.B
- [5] Multiply output 0 -> Mask ( R ).Input
- [5] Multiply output 0 -> Mask ( G ).Input
- [7] Mask ( R ) output 0 -> Multiply.A
- [8] Mask ( G ) output 0 -> Multiply.B
- [9] Multiply output 0 -> Output AllComponentsWithinRange.A
- [7] Mask ( R ) output 0 -> Add.A
- [8] Mask ( G ) output 0 -> Add.B
- [11] Add output 0 -> Saturate.Input

## Output Trace Roots

- `AnyComponentsWithinRange` is driven by [12] Saturate
- `AllComponentsWithinRange` is driven by [9] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
