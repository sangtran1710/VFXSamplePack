# VectorLength

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/VectorLength.VectorLength`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns a vectors length.

## Inputs

- `Vector 3` [FunctionInput_Vector3]
- `Vector 2` [FunctionInput_Vector2]

## Outputs

- `V3 Length`
- `V2 Length`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output V3 Length | V3 Length |
| 1 | expression | `MaterialExpressionDistance` | Distance |  |
| 2 | expression | `MaterialExpressionConstant` | 0 |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Vector 3 (Vector3) | Vector 3 |
| 4 | expression | `MaterialExpressionDistance` | Distance |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Vector 2 (Vector2) | Vector 2 |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output V2 Length | V2 Length |

## Exact Input Wiring

### [0] Output V3 Length

- `A` <= [1] Distance output 0

### [1] Distance

- `A` <= [2] 0 output 0
- `B` <= [3] Input Vector 3 (Vector3) output 0

### [4] Distance

- `A` <= [5] Input Vector 2 (Vector2) output 0
- `B` <= [2] 0 output 0

### [6] Output V2 Length

- `A` <= [4] Distance output 0

## Connection List

- [1] Distance output 0 -> Output V3 Length.A
- [2] 0 output 0 -> Distance.A
- [3] Input Vector 3 (Vector3) output 0 -> Distance.B
- [5] Input Vector 2 (Vector2) output 0 -> Distance.A
- [2] 0 output 0 -> Distance.B
- [4] Distance output 0 -> Output V2 Length.A

## Output Trace Roots

- `V3 Length` is driven by [1] Distance
- `V2 Length` is driven by [4] Distance

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


