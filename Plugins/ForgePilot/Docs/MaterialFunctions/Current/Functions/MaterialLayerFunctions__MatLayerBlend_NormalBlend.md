# MatLayerBlend_NormalBlend

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_NormalBlend.MatLayerBlend_NormalBlend`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends a Normalmap on top of a material using a mask.

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `NormalMask` [FunctionInput_Scalar]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BlendAngleCorrectedNormals |  |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input NormalMask (Scalar) | NormalMask |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 6 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 7 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 8 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 9 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [9] SetMaterialAttributes output 0

### [3] Lerp

- `A` <= [6] 0,0,1 output 0
- `B` <= [5] Input Normal (Vector3) output 0
- `Alpha` <= [4] Input NormalMask (Scalar) output 0

### [7] Normalize

- `VectorInput` <= [2] BlendAngleCorrectedNormals output 0

### [9] SetMaterialAttributes

- `Inputs[0]` <= [8] GetMaterialAttributes output 0
- `Inputs[1]` <= [7] Normalize output 0

## Connection List

- [9] SetMaterialAttributes output 0 -> Output Blended Material.A
- [6] 0,0,1 output 0 -> Lerp.A
- [5] Input Normal (Vector3) output 0 -> Lerp.B
- [4] Input NormalMask (Scalar) output 0 -> Lerp.Alpha
- [2] BlendAngleCorrectedNormals output 0 -> Normalize.VectorInput
- [8] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [7] Normalize output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [9] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


