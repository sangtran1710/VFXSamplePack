# MatLayerBlend_BreakNormal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BreakNormal.MatLayerBlend_BreakNormal`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Break out the Normal

## Inputs

- `Material` [FunctionInput_MaterialAttributes]

## Outputs

- `Normal`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [2] GetMaterialAttributes output 1

## Connection List

- [2] GetMaterialAttributes output 1 -> Output Normal.A

## Output Trace Roots

- `Normal` is driven by [2] GetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


