# MatLayerBlend_TopNormal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_TopNormal.MatLayerBlend_TopNormal`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends Material Attributes but only uses the Top Normal Map

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `Top Material` [FunctionInput_MaterialAttributes]
- `Base Material` [FunctionInput_MaterialAttributes]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Top Material (MaterialAttributes) | Top Material |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 4 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 9 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 11 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp(,,0.5) |  |
| 15 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 16 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 17 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |
| 18 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [17] SetMaterialAttributes output 0

### [4] Lerp

- `A` <= [16] GetMaterialAttributes output 4
- `B` <= [18] GetMaterialAttributes output 4
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [5] Lerp

- `A` <= [16] GetMaterialAttributes output 3
- `B` <= [18] GetMaterialAttributes output 3
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [6] Lerp

- `A` <= [16] GetMaterialAttributes output 2
- `B` <= [18] GetMaterialAttributes output 2
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [7] Lerp

- `A` <= [16] GetMaterialAttributes output 1
- `B` <= [18] GetMaterialAttributes output 1
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [8] Lerp

- `A` <= [16] GetMaterialAttributes output 5
- `B` <= [18] GetMaterialAttributes output 5
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [9] Lerp

- `A` <= [16] GetMaterialAttributes output 6
- `B` <= [18] GetMaterialAttributes output 6
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [10] Lerp

- `A` <= [16] GetMaterialAttributes output 7
- `B` <= [18] GetMaterialAttributes output 7
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [11] Lerp

- `A` <= [16] GetMaterialAttributes output 11
- `B` <= [18] GetMaterialAttributes output 11
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [12] Lerp

- `A` <= [16] GetMaterialAttributes output 8
- `B` <= [18] GetMaterialAttributes output 8
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [13] Lerp

- `A` <= [16] GetMaterialAttributes output 12
- `B` <= [18] GetMaterialAttributes output 12
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [14] Lerp(,,0.5)

- `A` <= [16] GetMaterialAttributes output 9
- `B` <= [18] GetMaterialAttributes output 9

### [15] Lerp

- `A` <= [16] GetMaterialAttributes output 10
- `B` <= [18] GetMaterialAttributes output 10
- `Alpha` <= [1] Input Alpha (Scalar) output 0

### [17] SetMaterialAttributes

- `Inputs[0]` <= [16] GetMaterialAttributes output 0
- `Inputs[1]` <= [7] Lerp output 0
- `Inputs[2]` <= [6] Lerp output 0
- `Inputs[3]` <= [5] Lerp output 0
- `Inputs[4]` <= [4] Lerp output 0
- `Inputs[5]` <= [8] Lerp output 0
- `Inputs[6]` <= [9] Lerp output 0
- `Inputs[7]` <= [10] Lerp output 0
- `Inputs[8]` <= [18] GetMaterialAttributes output 13
- `Inputs[9]` <= [12] Lerp output 0
- `Inputs[10]` <= [14] Lerp(,,0.5) output 0
- `Inputs[11]` <= [15] Lerp output 0
- `Inputs[12]` <= [11] Lerp output 0
- `Inputs[13]` <= [13] Lerp output 0

## Connection List

- [17] SetMaterialAttributes output 0 -> Output Blended Material.A
- [16] GetMaterialAttributes output 4 -> Lerp.A
- [18] GetMaterialAttributes output 4 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 3 -> Lerp.A
- [18] GetMaterialAttributes output 3 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 2 -> Lerp.A
- [18] GetMaterialAttributes output 2 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 1 -> Lerp.A
- [18] GetMaterialAttributes output 1 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 5 -> Lerp.A
- [18] GetMaterialAttributes output 5 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 6 -> Lerp.A
- [18] GetMaterialAttributes output 6 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 7 -> Lerp.A
- [18] GetMaterialAttributes output 7 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 11 -> Lerp.A
- [18] GetMaterialAttributes output 11 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 8 -> Lerp.A
- [18] GetMaterialAttributes output 8 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 12 -> Lerp.A
- [18] GetMaterialAttributes output 12 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 9 -> Lerp(,,0.5).A
- [18] GetMaterialAttributes output 9 -> Lerp(,,0.5).B
- [16] GetMaterialAttributes output 10 -> Lerp.A
- [18] GetMaterialAttributes output 10 -> Lerp.B
- [1] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [16] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [7] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [6] Lerp output 0 -> SetMaterialAttributes.Inputs[2]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[3]
- [4] Lerp output 0 -> SetMaterialAttributes.Inputs[4]
- [8] Lerp output 0 -> SetMaterialAttributes.Inputs[5]
- [9] Lerp output 0 -> SetMaterialAttributes.Inputs[6]
- [10] Lerp output 0 -> SetMaterialAttributes.Inputs[7]
- [18] GetMaterialAttributes output 13 -> SetMaterialAttributes.Inputs[8]
- [12] Lerp output 0 -> SetMaterialAttributes.Inputs[9]
- [14] Lerp(,,0.5) output 0 -> SetMaterialAttributes.Inputs[10]
- [15] Lerp output 0 -> SetMaterialAttributes.Inputs[11]
- [11] Lerp output 0 -> SetMaterialAttributes.Inputs[12]
- [13] Lerp output 0 -> SetMaterialAttributes.Inputs[13]

## Output Trace Roots

- `Blended Material` is driven by [17] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


