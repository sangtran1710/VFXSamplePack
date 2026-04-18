# MatLayerBlend_ModulateSpecular

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_ModulateSpecular.MatLayerBlend_ModulateSpecular`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Modulates the Specular value

## Inputs

- `Base Material` [FunctionInput_MaterialAttributes]
- `SpecularMultiplier` [FunctionInput_Scalar]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input SpecularMultiplier (Scalar) | SpecularMultiplier |
| 4 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 5 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 6 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [6] SetMaterialAttributes output 0

### [1] Input Base Material (MaterialAttributes)

- `Preview` <= [4] MakeMaterialAttributes output 0

### [2] Multiply

- `A` <= [5] GetMaterialAttributes output 1
- `B` <= [3] Input SpecularMultiplier (Scalar) output 0

### [6] SetMaterialAttributes

- `Inputs[0]` <= [5] GetMaterialAttributes output 0
- `Inputs[1]` <= [2] Multiply output 0

## Connection List

- [6] SetMaterialAttributes output 0 -> Output Blended Material.A
- [4] MakeMaterialAttributes output 0 -> Input Base Material (MaterialAttributes).Preview
- [5] GetMaterialAttributes output 1 -> Multiply.A
- [3] Input SpecularMultiplier (Scalar) output 0 -> Multiply.B
- [5] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [2] Multiply output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [6] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
