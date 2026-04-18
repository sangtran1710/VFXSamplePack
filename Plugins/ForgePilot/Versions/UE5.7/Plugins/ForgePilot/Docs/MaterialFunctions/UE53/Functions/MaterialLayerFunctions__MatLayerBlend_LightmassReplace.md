# MatLayerBlend_LightmassReplace

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_LightmassReplace.MatLayerBlend_LightmassReplace`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Replace the Base Color in Lightmass

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `LightmassReplaceColor` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | expression | `MaterialExpressionLightmassReplace` | LightmassReplace |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input LightmassReplaceColor (Vector3) | LightmassReplaceColor |
| 4 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 5 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [5] SetMaterialAttributes output 0

### [2] LightmassReplace

- `Realtime` <= [4] GetMaterialAttributes output 1
- `Lightmass` <= [3] Input LightmassReplaceColor (Vector3) output 0

### [5] SetMaterialAttributes

- `Inputs[0]` <= [4] GetMaterialAttributes output 0
- `Inputs[1]` <= [2] LightmassReplace output 0

## Connection List

- [5] SetMaterialAttributes output 0 -> Output Blended Material.A
- [4] GetMaterialAttributes output 1 -> LightmassReplace.Realtime
- [3] Input LightmassReplaceColor (Vector3) output 0 -> LightmassReplace.Lightmass
- [4] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [2] LightmassReplace output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [5] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
