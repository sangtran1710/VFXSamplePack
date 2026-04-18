# MakeFloat2

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat2.MakeFloat2`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Creates a float 2 vector from a series of scalar inputs.

## Inputs

- `X` [FunctionInput_Scalar]
- `Y` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input X (Scalar) | X |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Y (Scalar) | Y |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Append output 0

### [1] Append

- `A` <= [2] Input X (Scalar) output 0
- `B` <= [3] Input Y (Scalar) output 0

## Connection List

- [1] Append output 0 -> Output Result.A
- [2] Input X (Scalar) output 0 -> Append.A
- [3] Input Y (Scalar) output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [1] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
