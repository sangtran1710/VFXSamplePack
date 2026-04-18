# MatLayerBlend_StandardWithMaskEdgeTint

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_StandardWithMaskEdgeTint.MatLayerBlend_StandardWithMaskEdgeTint`
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
- `Tint Mask` [FunctionInput_Scalar]

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
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,) |  |
| 13 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 15 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 16 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Displacement | Displacement |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Tint Mask (Scalar) | Tint Mask |
| 20 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 21 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 22 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [22] SetMaterialAttributes output 0

### [1] Lerp

- `A` <= [20] GetMaterialAttributes output 8
- `B` <= [21] GetMaterialAttributes output 8
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [5] Lerp

- `A` <= [20] GetMaterialAttributes output 4
- `B` <= [21] GetMaterialAttributes output 4
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [6] Lerp

- `A` <= [20] GetMaterialAttributes output 3
- `B` <= [21] GetMaterialAttributes output 3
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [7] Lerp

- `A` <= [20] GetMaterialAttributes output 2
- `B` <= [21] GetMaterialAttributes output 2
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [8] Lerp

- `A` <= [20] GetMaterialAttributes output 1
- `B` <= [18] Multiply output 0
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [9] Lerp

- `A` <= [20] GetMaterialAttributes output 5
- `B` <= [21] GetMaterialAttributes output 5
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [10] Lerp

- `A` <= [20] GetMaterialAttributes output 10
- `B` <= [21] GetMaterialAttributes output 10
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [11] Lerp

- `A` <= [20] GetMaterialAttributes output 9
- `B` <= [21] GetMaterialAttributes output 9
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [12] Lerp(0,1,)

- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [13] Lerp

- `A` <= [20] GetMaterialAttributes output 11
- `B` <= [21] GetMaterialAttributes output 11
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [14] Lerp

- `A` <= [20] GetMaterialAttributes output 6
- `B` <= [21] GetMaterialAttributes output 6
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [15] Lerp

- `A` <= [20] GetMaterialAttributes output 7
- `B` <= [21] GetMaterialAttributes output 7
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [16] Lerp

- `A` <= [20] GetMaterialAttributes output 12
- `B` <= [21] GetMaterialAttributes output 12
- `Alpha` <= [2] Input Alpha (Scalar) output 0

### [17] Output Displacement

- `A` <= [12] Lerp(0,1,) output 0

### [18] Multiply

- `A` <= [21] GetMaterialAttributes output 1
- `B` <= [19] Input Tint Mask (Scalar) output 0

### [22] SetMaterialAttributes

- `Inputs[0]` <= [20] GetMaterialAttributes output 0
- `Inputs[1]` <= [8] Lerp output 0
- `Inputs[2]` <= [7] Lerp output 0
- `Inputs[3]` <= [6] Lerp output 0
- `Inputs[4]` <= [5] Lerp output 0
- `Inputs[5]` <= [9] Lerp output 0
- `Inputs[6]` <= [14] Lerp output 0
- `Inputs[7]` <= [15] Lerp output 0
- `Inputs[8]` <= [1] Lerp output 0
- `Inputs[9]` <= [11] Lerp output 0
- `Inputs[10]` <= [10] Lerp output 0
- `Inputs[11]` <= [13] Lerp output 0
- `Inputs[12]` <= [16] Lerp output 0

## Connection List

- [22] SetMaterialAttributes output 0 -> Output Blended Material.A
- [20] GetMaterialAttributes output 8 -> Lerp.A
- [21] GetMaterialAttributes output 8 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 4 -> Lerp.A
- [21] GetMaterialAttributes output 4 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 3 -> Lerp.A
- [21] GetMaterialAttributes output 3 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 2 -> Lerp.A
- [21] GetMaterialAttributes output 2 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 1 -> Lerp.A
- [18] Multiply output 0 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 5 -> Lerp.A
- [21] GetMaterialAttributes output 5 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 10 -> Lerp.A
- [21] GetMaterialAttributes output 10 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 9 -> Lerp.A
- [21] GetMaterialAttributes output 9 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [2] Input Alpha (Scalar) output 0 -> Lerp(0,1,).Alpha
- [20] GetMaterialAttributes output 11 -> Lerp.A
- [21] GetMaterialAttributes output 11 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 6 -> Lerp.A
- [21] GetMaterialAttributes output 6 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 7 -> Lerp.A
- [21] GetMaterialAttributes output 7 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [20] GetMaterialAttributes output 12 -> Lerp.A
- [21] GetMaterialAttributes output 12 -> Lerp.B
- [2] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [12] Lerp(0,1,) output 0 -> Output Displacement.A
- [21] GetMaterialAttributes output 1 -> Multiply.A
- [19] Input Tint Mask (Scalar) output 0 -> Multiply.B
- [20] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [8] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [7] Lerp output 0 -> SetMaterialAttributes.Inputs[2]
- [6] Lerp output 0 -> SetMaterialAttributes.Inputs[3]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[4]
- [9] Lerp output 0 -> SetMaterialAttributes.Inputs[5]
- [14] Lerp output 0 -> SetMaterialAttributes.Inputs[6]
- [15] Lerp output 0 -> SetMaterialAttributes.Inputs[7]
- [1] Lerp output 0 -> SetMaterialAttributes.Inputs[8]
- [11] Lerp output 0 -> SetMaterialAttributes.Inputs[9]
- [10] Lerp output 0 -> SetMaterialAttributes.Inputs[10]
- [13] Lerp output 0 -> SetMaterialAttributes.Inputs[11]
- [16] Lerp output 0 -> SetMaterialAttributes.Inputs[12]

## Output Trace Roots

- `Blended Material` is driven by [22] SetMaterialAttributes
- `Displacement` is driven by [12] Lerp(0,1,)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
