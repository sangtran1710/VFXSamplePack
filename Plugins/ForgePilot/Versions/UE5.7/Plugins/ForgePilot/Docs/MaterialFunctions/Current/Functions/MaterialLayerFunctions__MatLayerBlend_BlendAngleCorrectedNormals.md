# MatLayerBlend_BlendAngleCorrectedNormals

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BlendAngleCorrectedNormals.MatLayerBlend_BlendAngleCorrectedNormals`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Replaces WorldPositionDisplacement

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Additional Normal` [FunctionInput_Vector3]
- `Mask` [FunctionInput_Scalar]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Additional Normal (Vector3) | Additional Normal |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Mask (Scalar) | Mask |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | BlendAngleCorrectedNormals |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 7 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [7] SetMaterialAttributes output 0

### [5] Lerp

- `A` <= [6] GetMaterialAttributes output 1
- `B` <= [4] BlendAngleCorrectedNormals output 0
- `Alpha` <= [3] Input Mask (Scalar) output 0

### [7] SetMaterialAttributes

- `Inputs[0]` <= [6] GetMaterialAttributes output 0
- `Inputs[1]` <= [5] Lerp output 0

## Connection List

- [7] SetMaterialAttributes output 0 -> Output Blended Material.A
- [6] GetMaterialAttributes output 1 -> Lerp.A
- [4] BlendAngleCorrectedNormals output 0 -> Lerp.B
- [3] Input Mask (Scalar) output 0 -> Lerp.Alpha
- [6] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [7] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


