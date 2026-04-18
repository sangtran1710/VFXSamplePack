# WithinRangeFloat4

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRangeFloat4.WithinRangeFloat4`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns 1 if a value is within the provided range limits. 0 otherwise.

## Inputs

- `Value` [FunctionInput_Vector4]
- `RangeMax` [FunctionInput_Vector4]
- `RangeMin` [FunctionInput_Vector4]

## Outputs

- `AnyComponentsWithinRange` - Returns a vector with each components set to 1 if the component's value is within the provided range limits. 0 otherwise.
- `AllComponentsWithinRange` - Returns 1 if all components of the value vector are within the provided range. 0 otherwise.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output AnyComponentsWithinRange | AnyComponentsWithinRange |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Value (Vector4) | Value |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input RangeMax (Vector4) | RangeMax |
| 3 | expression | `MaterialExpressionStep` | Step |  |
| 4 | expression | `MaterialExpressionStep` | Step |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input RangeMin (Vector4) | RangeMin |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output AllComponentsWithinRange | AllComponentsWithinRange |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 12 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionAdd` | Add |  |
| 18 | expression | `MaterialExpressionSaturate` | Saturate |  |

## Exact Input Wiring

### [0] Output AnyComponentsWithinRange

- `A` <= [18] Saturate output 0

### [3] Step

- `Y` <= [6] Input RangeMin (Vector4) output 0
- `X` <= [1] Input Value (Vector4) output 0

### [4] Step

- `Y` <= [1] Input Value (Vector4) output 0
- `X` <= [2] Input RangeMax (Vector4) output 0

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

- `A` <= [14] Multiply output 0

### [11] Mask ( B )

- `Input` <= [5] Multiply output 0

### [12] Mask ( A )

- `Input` <= [5] Multiply output 0

### [13] Multiply

- `A` <= [11] Mask ( B ) output 0
- `B` <= [12] Mask ( A ) output 0

### [14] Multiply

- `A` <= [9] Multiply output 0
- `B` <= [13] Multiply output 0

### [15] Add

- `A` <= [7] Mask ( R ) output 0
- `B` <= [8] Mask ( G ) output 0

### [16] Add

- `A` <= [11] Mask ( B ) output 0
- `B` <= [12] Mask ( A ) output 0

### [17] Add

- `A` <= [15] Add output 0
- `B` <= [16] Add output 0

### [18] Saturate

- `Input` <= [17] Add output 0

## Connection List

- [18] Saturate output 0 -> Output AnyComponentsWithinRange.A
- [6] Input RangeMin (Vector4) output 0 -> Step.Y
- [1] Input Value (Vector4) output 0 -> Step.X
- [1] Input Value (Vector4) output 0 -> Step.Y
- [2] Input RangeMax (Vector4) output 0 -> Step.X
- [3] Step output 0 -> Multiply.A
- [4] Step output 0 -> Multiply.B
- [5] Multiply output 0 -> Mask ( R ).Input
- [5] Multiply output 0 -> Mask ( G ).Input
- [7] Mask ( R ) output 0 -> Multiply.A
- [8] Mask ( G ) output 0 -> Multiply.B
- [14] Multiply output 0 -> Output AllComponentsWithinRange.A
- [5] Multiply output 0 -> Mask ( B ).Input
- [5] Multiply output 0 -> Mask ( A ).Input
- [11] Mask ( B ) output 0 -> Multiply.A
- [12] Mask ( A ) output 0 -> Multiply.B
- [9] Multiply output 0 -> Multiply.A
- [13] Multiply output 0 -> Multiply.B
- [7] Mask ( R ) output 0 -> Add.A
- [8] Mask ( G ) output 0 -> Add.B
- [11] Mask ( B ) output 0 -> Add.A
- [12] Mask ( A ) output 0 -> Add.B
- [15] Add output 0 -> Add.A
- [16] Add output 0 -> Add.B
- [17] Add output 0 -> Saturate.Input

## Output Trace Roots

- `AnyComponentsWithinRange` is driven by [18] Saturate
- `AllComponentsWithinRange` is driven by [14] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


