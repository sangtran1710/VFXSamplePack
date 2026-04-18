# DynamicBranch

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DynamicBranch.DynamicBranch`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The code connected to the "Color Input 1" will be executed if the alpha is greater than 0 and the "Color Input 0" input code will be evaluated  if the alpha is 0. Dynamic branches are best used when masking 20 or more instructions or a texture sample.

## Inputs

- `Color Input 0` [FunctionInput_Vector3]
- `Alpha` [FunctionInput_Scalar]
- `Color Input 1` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | expression | `MaterialExpressionConstant` | 0 |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Color Input 0 (Vector3) | Color Input 0 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Color Input 1 (Vector3) | Color Input 1 |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] If output 0

### [1] If

- `A` <= [4] Input Alpha (Scalar) output 0
- `B` <= [2] 0 output 0
- `AGreaterThanB` <= [5] Input Color Input 1 (Vector3) output 0
- `AEqualsB` <= [3] Input Color Input 0 (Vector3) output 0
- `ALessThanB` <= [3] Input Color Input 0 (Vector3) output 0

## Connection List

- [1] If output 0 -> Output Result.A
- [4] Input Alpha (Scalar) output 0 -> If.A
- [2] 0 output 0 -> If.B
- [5] Input Color Input 1 (Vector3) output 0 -> If.AGreaterThanB
- [3] Input Color Input 0 (Vector3) output 0 -> If.AEqualsB
- [3] Input Color Input 0 (Vector3) output 0 -> If.ALessThanB

## Output Trace Roots

- `Result` is driven by [1] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
