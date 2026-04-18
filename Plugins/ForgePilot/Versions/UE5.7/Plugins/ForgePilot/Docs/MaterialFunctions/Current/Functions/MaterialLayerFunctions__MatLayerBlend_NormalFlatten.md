# MatLayerBlend_NormalFlatten

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_NormalFlatten.MatLayerBlend_NormalFlatten`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Diminishes the effect of a normalmap.

## Inputs

- `NormalFlatness` [FunctionInput_Scalar]
- `Material` [FunctionInput_MaterialAttributes]
- `Normal` [FunctionInput_Vector3] - Defaults to a flat tangent space normal. (0,0,1)

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input NormalFlatness (Scalar) | NormalFlatness |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 5 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 6 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [6] SetMaterialAttributes output 0

### [1] Lerp

- `A` <= [5] GetMaterialAttributes output 1
- `B` <= [4] Input Normal (Vector3) output 0
- `Alpha` <= [2] Input NormalFlatness (Scalar) output 0

### [6] SetMaterialAttributes

- `Inputs[0]` <= [5] GetMaterialAttributes output 0
- `Inputs[1]` <= [1] Lerp output 0

## Connection List

- [6] SetMaterialAttributes output 0 -> Output Blended Material.A
- [5] GetMaterialAttributes output 1 -> Lerp.A
- [4] Input Normal (Vector3) output 0 -> Lerp.B
- [2] Input NormalFlatness (Scalar) output 0 -> Lerp.Alpha
- [5] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [1] Lerp output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [6] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


