# WithinRange

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRange.WithinRange`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns 1 if a value is within the provided range limits. 0 otherwise.

## Inputs

- `Value` [FunctionInput_Scalar]
- `RangeMax` [FunctionInput_Scalar]
- `RangeMin` [FunctionInput_Scalar]

## Outputs

- `AllComponentsWithinRange` - Returns 1 if a value is within the provided range limits. 0 otherwise.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_input | `MaterialExpressionFunctionInput` | Input Value (Scalar) | Value |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input RangeMax (Scalar) | RangeMax |
| 2 | expression | `MaterialExpressionStep` | Step |  |
| 3 | expression | `MaterialExpressionStep` | Step |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input RangeMin (Scalar) | RangeMin |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output AllComponentsWithinRange | AllComponentsWithinRange |

## Exact Input Wiring

### [2] Step

- `Y` <= [5] Input RangeMin (Scalar) output 0
- `X` <= [0] Input Value (Scalar) output 0

### [3] Step

- `Y` <= [0] Input Value (Scalar) output 0
- `X` <= [1] Input RangeMax (Scalar) output 0

### [4] Multiply

- `A` <= [2] Step output 0
- `B` <= [3] Step output 0

### [6] Mask ( R )

- `Input` <= [4] Multiply output 0

### [7] Mask ( G )

- `Input` <= [4] Multiply output 0

### [8] Multiply

- `A` <= [6] Mask ( R ) output 0
- `B` <= [7] Mask ( G ) output 0

### [9] Output AllComponentsWithinRange

- `A` <= [8] Multiply output 0

## Connection List

- [5] Input RangeMin (Scalar) output 0 -> Step.Y
- [0] Input Value (Scalar) output 0 -> Step.X
- [0] Input Value (Scalar) output 0 -> Step.Y
- [1] Input RangeMax (Scalar) output 0 -> Step.X
- [2] Step output 0 -> Multiply.A
- [3] Step output 0 -> Multiply.B
- [4] Multiply output 0 -> Mask ( R ).Input
- [4] Multiply output 0 -> Mask ( G ).Input
- [6] Mask ( R ) output 0 -> Multiply.A
- [7] Mask ( G ) output 0 -> Multiply.B
- [8] Multiply output 0 -> Output AllComponentsWithinRange.A

## Output Trace Roots

- `AllComponentsWithinRange` is driven by [8] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
