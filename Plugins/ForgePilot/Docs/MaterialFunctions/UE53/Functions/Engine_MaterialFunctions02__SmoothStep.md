# SmoothStep

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SmoothStep.SmoothStep`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

HLSL smoothstep function. a custom node)

## Inputs

- `Min` [FunctionInput_Scalar]
- `Max` [FunctionInput_Scalar]
- `Alpha` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Min (Scalar) | Min |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Max (Scalar) | Max |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |

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
