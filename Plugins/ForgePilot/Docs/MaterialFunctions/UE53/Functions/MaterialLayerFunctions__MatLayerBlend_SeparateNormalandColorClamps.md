# MatLayerBlend_SeparateNormalandColorClamps

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_SeparateNormalandColorClamps.MatLayerBlend_SeparateNormalandColorClamps`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends all attributes of 2 Materials

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `Top Material` [FunctionInput_MaterialAttributes]
- `Base Material` [FunctionInput_MaterialAttributes]
- `Color Blend Min` [FunctionInput_Scalar]
- `Color Blend Max` [FunctionInput_Scalar]
- `Normal Blend Min` [FunctionInput_Scalar]
- `Normal Blend Max` [FunctionInput_Scalar]

## Outputs

- `Blended Material`
- `Displacement`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Top Material (MaterialAttributes) | Top Material |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 9 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 11 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 15 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 16 | expression | `MaterialExpressionClamp` | Clamp |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Color Blend Min (Scalar) | Color Blend Min |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Color Blend Max (Scalar) | Color Blend Max |
| 19 | expression | `MaterialExpressionClamp` | Clamp |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Normal Blend Min (Scalar) | Normal Blend Min |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Normal Blend Max (Scalar) | Normal Blend Max |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Displacement | Displacement |
| 23 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 24 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 25 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [25] SetMaterialAttributes output 0

### [1] Lerp

- `A` <= [23] GetMaterialAttributes output 8
- `B` <= [24] GetMaterialAttributes output 8
- `Alpha` <= [19] Clamp output 0

### [5] Lerp

- `A` <= [23] GetMaterialAttributes output 4
- `B` <= [24] GetMaterialAttributes output 4
- `Alpha` <= [16] Clamp output 0

### [6] Lerp

- `A` <= [23] GetMaterialAttributes output 3
- `B` <= [24] GetMaterialAttributes output 3
- `Alpha` <= [16] Clamp output 0

### [7] Lerp

- `A` <= [23] GetMaterialAttributes output 2
- `B` <= [24] GetMaterialAttributes output 2
- `Alpha` <= [16] Clamp output 0

### [8] Lerp

- `A` <= [23] GetMaterialAttributes output 1
- `B` <= [24] GetMaterialAttributes output 1
- `Alpha` <= [16] Clamp output 0

### [9] Lerp

- `A` <= [23] GetMaterialAttributes output 5
- `B` <= [24] GetMaterialAttributes output 5
- `Alpha` <= [16] Clamp output 0

### [10] Lerp

- `A` <= [23] GetMaterialAttributes output 11
- `B` <= [24] GetMaterialAttributes output 11
- `Alpha` <= [16] Clamp output 0

### [11] Lerp

- `A` <= [23] GetMaterialAttributes output 10
- `B` <= [24] GetMaterialAttributes output 10
- `Alpha` <= [16] Clamp output 0

### [12] Lerp

- `A` <= [23] GetMaterialAttributes output 12
- `B` <= [24] GetMaterialAttributes output 12
- `Alpha` <= [16] Clamp output 0

### [13] Lerp

- `A` <= [23] GetMaterialAttributes output 6
- `B` <= [24] GetMaterialAttributes output 6
- `Alpha` <= [16] Clamp output 0

### [14] Lerp

- `A` <= [23] GetMaterialAttributes output 7
- `B` <= [24] GetMaterialAttributes output 7
- `Alpha` <= [16] Clamp output 0

### [15] Lerp

- `A` <= [23] GetMaterialAttributes output 13
- `B` <= [24] GetMaterialAttributes output 13
- `Alpha` <= [16] Clamp output 0

### [16] Clamp

- `Input` <= [2] Input Alpha (Scalar) output 0
- `Min` <= [17] Input Color Blend Min (Scalar) output 0
- `Max` <= [18] Input Color Blend Max (Scalar) output 0

### [19] Clamp

- `Input` <= [2] Input Alpha (Scalar) output 0
- `Min` <= [20] Input Normal Blend Min (Scalar) output 0
- `Max` <= [21] Input Normal Blend Max (Scalar) output 0

### [25] SetMaterialAttributes

- `Inputs[0]` <= [24] GetMaterialAttributes output 0
- `Inputs[1]` <= [8] Lerp output 0
- `Inputs[2]` <= [7] Lerp output 0
- `Inputs[3]` <= [6] Lerp output 0
- `Inputs[4]` <= [5] Lerp output 0
- `Inputs[5]` <= [9] Lerp output 0
- `Inputs[6]` <= [13] Lerp output 0
- `Inputs[7]` <= [14] Lerp output 0
- `Inputs[8]` <= [1] Lerp output 0
- `Inputs[9]` <= [23] GetMaterialAttributes output 9
- `Inputs[10]` <= [11] Lerp output 0
- `Inputs[11]` <= [10] Lerp output 0
- `Inputs[12]` <= [12] Lerp output 0
- `Inputs[13]` <= [15] Lerp output 0

## Connection List

- [25] SetMaterialAttributes output 0 -> Output Blended Material.A
- [23] GetMaterialAttributes output 8 -> Lerp.A
- [24] GetMaterialAttributes output 8 -> Lerp.B
- [19] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 4 -> Lerp.A
- [24] GetMaterialAttributes output 4 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 3 -> Lerp.A
- [24] GetMaterialAttributes output 3 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 2 -> Lerp.A
- [24] GetMaterialAttributes output 2 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 1 -> Lerp.A
- [24] GetMaterialAttributes output 1 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 5 -> Lerp.A
- [24] GetMaterialAttributes output 5 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 11 -> Lerp.A
- [24] GetMaterialAttributes output 11 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 10 -> Lerp.A
- [24] GetMaterialAttributes output 10 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 12 -> Lerp.A
- [24] GetMaterialAttributes output 12 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 6 -> Lerp.A
- [24] GetMaterialAttributes output 6 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 7 -> Lerp.A
- [24] GetMaterialAttributes output 7 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [23] GetMaterialAttributes output 13 -> Lerp.A
- [24] GetMaterialAttributes output 13 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [2] Input Alpha (Scalar) output 0 -> Clamp.Input
- [17] Input Color Blend Min (Scalar) output 0 -> Clamp.Min
- [18] Input Color Blend Max (Scalar) output 0 -> Clamp.Max
- [2] Input Alpha (Scalar) output 0 -> Clamp.Input
- [20] Input Normal Blend Min (Scalar) output 0 -> Clamp.Min
- [21] Input Normal Blend Max (Scalar) output 0 -> Clamp.Max
- [24] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [8] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [7] Lerp output 0 -> SetMaterialAttributes.Inputs[2]
- [6] Lerp output 0 -> SetMaterialAttributes.Inputs[3]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[4]
- [9] Lerp output 0 -> SetMaterialAttributes.Inputs[5]
- [13] Lerp output 0 -> SetMaterialAttributes.Inputs[6]
- [14] Lerp output 0 -> SetMaterialAttributes.Inputs[7]
- [1] Lerp output 0 -> SetMaterialAttributes.Inputs[8]
- [23] GetMaterialAttributes output 9 -> SetMaterialAttributes.Inputs[9]
- [11] Lerp output 0 -> SetMaterialAttributes.Inputs[10]
- [10] Lerp output 0 -> SetMaterialAttributes.Inputs[11]
- [12] Lerp output 0 -> SetMaterialAttributes.Inputs[12]
- [15] Lerp output 0 -> SetMaterialAttributes.Inputs[13]

## Output Trace Roots

- `Blended Material` is driven by [25] SetMaterialAttributes
- `Displacement` has no captured source.

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
