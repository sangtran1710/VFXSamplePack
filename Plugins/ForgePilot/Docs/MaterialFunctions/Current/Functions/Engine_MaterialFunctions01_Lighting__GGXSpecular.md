# GGXSpecular

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Lighting/GGXSpecular.GGXSpecular`
- Category: Engine_MaterialFunctions01/Lighting
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns a specular lobe using GGX.

## Inputs

- `Normal` [FunctionInput_Vector3] - The surface normal.
- `Light Vector` [FunctionInput_Vector3] - The light vector to calculate specular for.
- `Roughness` [FunctionInput_Scalar] - The surface roughness

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Light Vector (Vector3) | Light Vector |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Roughness (Scalar) | Roughness |

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


