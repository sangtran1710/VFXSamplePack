# MatLayerBlend_Simple

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Simple.MatLayerBlend_Simple`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends all attributes of 2 Materials except Normal.

## Inputs

- `Base Material` [FunctionInput_MaterialAttributes]
- `Alpha` [FunctionInput_Scalar]
- `Top Material` [FunctionInput_MaterialAttributes]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Top Material (MaterialAttributes) | Top Material |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 9 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |
| 10 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [9] SetMaterialAttributes output 0

### [2] Lerp

- `A` <= [8] GetMaterialAttributes output 3
- `B` <= [10] GetMaterialAttributes output 3
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [5] Lerp

- `A` <= [8] GetMaterialAttributes output 2
- `B` <= [10] GetMaterialAttributes output 2
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [6] Lerp

- `A` <= [8] GetMaterialAttributes output 1
- `B` <= [10] GetMaterialAttributes output 1
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [7] Lerp

- `A` <= [8] GetMaterialAttributes output 4
- `B` <= [10] GetMaterialAttributes output 4
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [9] SetMaterialAttributes

- `Inputs[0]` <= [8] GetMaterialAttributes output 0
- `Inputs[1]` <= [6] Lerp output 0
- `Inputs[2]` <= [5] Lerp output 0
- `Inputs[3]` <= [2] Lerp output 0
- `Inputs[4]` <= [7] Lerp output 0

## Connection List

- [9] SetMaterialAttributes output 0 -> Output Blended Material.A
- [8] GetMaterialAttributes output 3 -> Lerp.A
- [10] GetMaterialAttributes output 3 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [8] GetMaterialAttributes output 2 -> Lerp.A
- [10] GetMaterialAttributes output 2 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [8] GetMaterialAttributes output 1 -> Lerp.A
- [10] GetMaterialAttributes output 1 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [8] GetMaterialAttributes output 4 -> Lerp.A
- [10] GetMaterialAttributes output 4 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [8] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [6] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[2]
- [2] Lerp output 0 -> SetMaterialAttributes.Inputs[3]
- [7] Lerp output 0 -> SetMaterialAttributes.Inputs[4]

## Output Trace Roots

- `Blended Material` is driven by [9] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


