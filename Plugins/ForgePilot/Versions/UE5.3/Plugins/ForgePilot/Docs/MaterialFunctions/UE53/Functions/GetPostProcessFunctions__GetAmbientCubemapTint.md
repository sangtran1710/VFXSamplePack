# GetAmbientCubemapTint

- Path: `/Engine/Functions/GetPostProcessFunctions/GetAmbientCubemapTint.GetAmbientCubemapTint`
- Category: GetPostProcessFunctions
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Get Ambient Cubemap Tint

## Inputs

- None

## Outputs

- `AmbientCubemapTint`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output AmbientCubemapTint | AmbientCubemapTint |
| 1 | custom | `MaterialExpressionCustom` | Ambient Cubemap Tint |  |

## Exact Input Wiring

### [0] Output AmbientCubemapTint

- `A` <= [1] Ambient Cubemap Tint output 0

## Connection List

- [1] Ambient Cubemap Tint output 0 -> Output AmbientCubemapTint.A

## Output Trace Roots

- `AmbientCubemapTint` is driven by [1] Ambient Cubemap Tint

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
