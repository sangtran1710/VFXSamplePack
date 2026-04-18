# DiffColorKeyer

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DiffColorKeyer.DiffColorKeyer`
- Category: Engine_MaterialFunctions01/Chromakeying
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `GreenScreen` [FunctionInput_Vector3]
- `KeyColor` [FunctionInput_Vector3]
- `AlphaThresOffset` [FunctionInput_Vector2]
- `WeightsRB` [FunctionInput_Vector2]
- `ClipBW` [FunctionInput_Vector2]
- `UnPremult` [FunctionInput_Scalar]

## Outputs

- `RGBA`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGBA | RGBA |
| 1 | custom | `MaterialExpressionCustom` | ColorDiffKeyer |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input GreenScreen (Vector3) | GreenScreen |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input KeyColor (Vector3) | KeyColor |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input AlphaThresOffset (Vector2) | AlphaThresOffset |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input WeightsRB (Vector2) | WeightsRB |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input ClipBW (Vector2) | ClipBW |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input UnPremult (Scalar) | UnPremult |

## Exact Input Wiring

### [0] Output RGBA

- `A` <= [1] ColorDiffKeyer output 0

## Connection List

- [1] ColorDiffKeyer output 0 -> Output RGBA.A

## Output Trace Roots

- `RGBA` is driven by [1] ColorDiffKeyer

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
