# MatLayerBlend_BakedNormal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BakedNormal.MatLayerBlend_BakedNormal`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends a Processed Normal Map with layered normals

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BlendAngleCorrectedNormals |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionConstant3Vector` | 1,1,0 |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 10 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [10] SetMaterialAttributes output 0

### [4] Normalize

- `VectorInput` <= [2] BlendAngleCorrectedNormals output 0

### [5] Quality Switch

- `Default` <= [4] Normalize output 0
- `Inputs` <= [8] Add output 0

### [6] Multiply

- `A` <= [9] GetMaterialAttributes output 1
- `B` <= [7] 1,1,0 output 0

### [8] Add

- `A` <= [3] Input Normal (Vector3) output 0
- `B` <= [6] Multiply output 0

### [10] SetMaterialAttributes

- `Inputs[0]` <= [9] GetMaterialAttributes output 0
- `Inputs[1]` <= [5] Quality Switch output 0

## Connection List

- [10] SetMaterialAttributes output 0 -> Output Blended Material.A
- [2] BlendAngleCorrectedNormals output 0 -> Normalize.VectorInput
- [4] Normalize output 0 -> Quality Switch.Default
- [8] Add output 0 -> Quality Switch.Inputs
- [9] GetMaterialAttributes output 1 -> Multiply.A
- [7] 1,1,0 output 0 -> Multiply.B
- [3] Input Normal (Vector3) output 0 -> Add.A
- [6] Multiply output 0 -> Add.B
- [9] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [5] Quality Switch output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [10] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
