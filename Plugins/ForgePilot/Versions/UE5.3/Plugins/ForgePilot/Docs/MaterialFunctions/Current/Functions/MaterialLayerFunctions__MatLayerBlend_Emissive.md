# MatLayerBlend_Emissive

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Emissive.MatLayerBlend_Emissive`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Adds Emissive

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Emissive` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Emissive (Vector3) | Emissive |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |
| 5 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 6 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [6] SetMaterialAttributes output 0

### [3] Add

- `A` <= [5] GetMaterialAttributes output 1
- `B` <= [2] Input Emissive (Vector3) output 0

### [4] Clamp

- `Input` <= [3] Add output 0
- `Max` <= [3] Add output 0

### [6] SetMaterialAttributes

- `Inputs[0]` <= [5] GetMaterialAttributes output 0
- `Inputs[1]` <= [4] Clamp output 0

## Connection List

- [6] SetMaterialAttributes output 0 -> Output Blended Material.A
- [5] GetMaterialAttributes output 1 -> Add.A
- [2] Input Emissive (Vector3) output 0 -> Add.B
- [3] Add output 0 -> Clamp.Input
- [3] Add output 0 -> Clamp.Max
- [5] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [4] Clamp output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [6] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


