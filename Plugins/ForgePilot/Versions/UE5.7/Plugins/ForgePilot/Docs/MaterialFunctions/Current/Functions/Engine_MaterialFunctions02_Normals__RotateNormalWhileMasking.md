# RotateNormalWhileMasking

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Normals/RotateNormalWhileMasking.RotateNormalWhileMasking`
- Category: Engine_MaterialFunctions02/Normals
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This will rotate a normal toward it's slope as the greyscale input increases. This is useful for thresholding a normal map and generating a lip around the newly masked areas.

## Inputs

- `Hp Normal` [FunctionInput_Vector3] - Enter a normal texture transformed into world space. 
- `LowPolyNormal` [FunctionInput_Vector3]
- `Grey Scale Mask Amount` [FunctionInput_Scalar] - Enter a mask value (this will control the number of rotations the normal makes.

## Outputs

- `Normal`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Hp Normal (Vector3) | Hp Normal |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input LowPolyNormal (Vector3) | LowPolyNormal |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Grey Scale Mask Amount (Scalar) | Grey Scale Mask Amount |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 6 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 7 | expression | `MaterialExpressionConstant` | 0 |  |
| 8 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 11 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 12 | expression | `MaterialExpressionPixelNormalWS` | PixelNormalWS |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [10] Normalize output 0

### [1] Input Hp Normal (Vector3)

- `Preview` <= [12] PixelNormalWS output 0

### [2] Input LowPolyNormal (Vector3)

- `Preview` <= [11] VertexNormalWS output 0

### [4] Normalize

- `VectorInput` <= [1] Input Hp Normal (Vector3) output 0

### [5] Cross

- `A` <= [2] Input LowPolyNormal (Vector3) output 0
- `B` <= [4] Normalize output 0

### [6] Normalize

- `VectorInput` <= [5] Cross output 0

### [8] RotateAboutAxis

- `NormalizedRotationAxis` <= [6] Normalize output 0
- `RotationAngle` <= [3] Input Grey Scale Mask Amount (Scalar) output 0
- `PivotPoint` <= [7] 0 output 0
- `Position` <= [4] Normalize output 0

### [9] Add

- `A` <= [4] Normalize output 0
- `B` <= [13] Multiply(,-1) output 0

### [10] Normalize

- `VectorInput` <= [9] Add output 0

### [13] Multiply(,-1)

- `A` <= [8] RotateAboutAxis output 0

## Connection List

- [10] Normalize output 0 -> Output Normal.A
- [12] PixelNormalWS output 0 -> Input Hp Normal (Vector3).Preview
- [11] VertexNormalWS output 0 -> Input LowPolyNormal (Vector3).Preview
- [1] Input Hp Normal (Vector3) output 0 -> Normalize.VectorInput
- [2] Input LowPolyNormal (Vector3) output 0 -> Cross.A
- [4] Normalize output 0 -> Cross.B
- [5] Cross output 0 -> Normalize.VectorInput
- [6] Normalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [3] Input Grey Scale Mask Amount (Scalar) output 0 -> RotateAboutAxis.RotationAngle
- [7] 0 output 0 -> RotateAboutAxis.PivotPoint
- [4] Normalize output 0 -> RotateAboutAxis.Position
- [4] Normalize output 0 -> Add.A
- [13] Multiply(,-1) output 0 -> Add.B
- [9] Add output 0 -> Normalize.VectorInput
- [8] RotateAboutAxis output 0 -> Multiply(,-1).A

## Output Trace Roots

- `Normal` is driven by [10] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


