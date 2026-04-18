# RGBtoHSV

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/RGBtoHSV.RGBtoHSV`
- Category: Engine_MaterialFunctions01/Chromakeying
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `In` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In (Vector3) | In |
| 2 | custom | `MaterialExpressionCustom` | Custom |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Custom output 0

## Connection List

- [2] Custom output 0 -> Output Result.A

## Output Trace Roots

- `Result` is driven by [2] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
