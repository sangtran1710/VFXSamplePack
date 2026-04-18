# MatLayerBlend_BreakOpacity

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MatLayerBlend_BreakOpacity.MatLayerBlend_BreakOpacity`
- Category: Engine_MaterialFunctions02
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Inputs

- `Mat attributes` [FunctionInput_MaterialAttributes]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Mat attributes (MaterialAttributes) | Mat attributes |
| 2 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] BreakMaterialAttributes output 6

## Connection List

- [2] BreakMaterialAttributes output 6 -> Output Result.A

## Output Trace Roots

- `Result` is driven by [2] BreakMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
