# Sign

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Sign.Sign`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns a -1 for negative numbers, a 1 for positive numbers and 0 for 0.

## Inputs

- `In` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | sign |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 3 | expression | `MaterialExpressionIf` | If |  |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionConstant` | -1 |  |
| 6 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] sign output 0

### [3] If

- `A` <= [2] Input In (Scalar) output 0
- `B` <= [4] 0 output 0
- `AGreaterThanB` <= [6] 1 output 0
- `AEqualsB` <= [4] 0 output 0
- `ALessThanB` <= [5] -1 output 0

## Connection List

- [1] sign output 0 -> Output Result.A
- [2] Input In (Scalar) output 0 -> If.A
- [4] 0 output 0 -> If.B
- [6] 1 output 0 -> If.AGreaterThanB
- [4] 0 output 0 -> If.AEqualsB
- [5] -1 output 0 -> If.ALessThanB

## Output Trace Roots

- `Result` is driven by [1] sign

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


