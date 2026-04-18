# IsFloatValid

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/IsFloatValid.IsFloatValid`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns 1 if the float is a real number and 0 if it is either infinite or a NaN.

## Inputs

- `In` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Custom output 0

## Connection List

- [1] Custom output 0 -> Output Result.A

## Output Trace Roots

- `Result` is driven by [1] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
