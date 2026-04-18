# MatLayerBlend_ReplaceNormals

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_ReplaceNormals.MatLayerBlend_ReplaceNormals`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Normal` [FunctionInput_Vector3] - Defaults to a flat tangent space normal. (0,0,1)

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 3 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |
| 4 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] SetMaterialAttributes output 0

### [3] SetMaterialAttributes

- `Inputs[0]` <= [4] GetMaterialAttributes output 0
- `Inputs[1]` <= [2] Input Normal (Vector3) output 0

## Connection List

- [3] SetMaterialAttributes output 0 -> Output Result.A
- [4] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [2] Input Normal (Vector3) output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Result` is driven by [3] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


