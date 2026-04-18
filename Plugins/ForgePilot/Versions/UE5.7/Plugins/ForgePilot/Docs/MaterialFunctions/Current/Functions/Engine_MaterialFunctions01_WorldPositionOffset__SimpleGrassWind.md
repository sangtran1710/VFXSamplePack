# SimpleGrassWind

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/SimpleGrassWind.SimpleGrassWind`
- Category: Engine_MaterialFunctions01/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Simple Waving grass wind, Grass must be textured to full sheet.  This should be the last WPO node applied.

## Inputs

- `AdditionalWPO` [FunctionInput_Vector3]
- `WindIntensity` [FunctionInput_Scalar] - How strong the effect should appear
- `WindWeight` [FunctionInput_Scalar] - Input vertex colors or texturecoods
- `WindSpeed` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input AdditionalWPO (Vector3) | AdditionalWPO |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input WindIntensity (Scalar) | WindIntensity |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input WindWeight (Scalar) | WindWeight |
| 7 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionConstant3Vector` | 0,0,-10 |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionDistance` | Distance |  |
| 13 | expression | `MaterialExpressionConstant` | 0 |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionAbs` | Abs |  |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionConstant` | -1 |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionConstant` | 2 |  |
| 20 | expression | `MaterialExpressionFrac` | Frac |  |
| 21 | expression | `MaterialExpressionAdd` | Add |  |
| 22 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionDivide` | Divide |  |
| 25 | expression | `MaterialExpressionConstant` | 200 |  |
| 26 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 27 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 30 | expression | `MaterialExpressionTime` | Time |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 33 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionConstant` | 2 |  |
| 36 | expression | `MaterialExpressionConstant` | 3 |  |
| 37 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 39 | expression | `MaterialExpressionAbs` | Abs |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | expression | `MaterialExpressionConstant` | -1 |  |
| 42 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 43 | expression | `MaterialExpressionConstant` | 2 |  |
| 44 | expression | `MaterialExpressionFrac` | Frac |  |
| 45 | expression | `MaterialExpressionAdd` | Add |  |
| 46 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | expression | `MaterialExpressionDivide` | Divide |  |
| 49 | expression | `MaterialExpressionConstant` | 1024 |  |
| 50 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 51 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 52 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 53 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 54 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 55 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 56 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 57 | expression | `MaterialExpressionConstant` | 2 |  |
| 58 | expression | `MaterialExpressionConstant` | 3 |  |
| 59 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 60 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 61 | expression | `MaterialExpressionConstant4Vector` | 0,1,0,1 |  |
| 62 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 63 | function_input | `MaterialExpressionFunctionInput` | Input WindSpeed (Scalar) | WindSpeed |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Add output 0

### [1] Add

- `A` <= [3] Multiply output 0
- `B` <= [2] Input AdditionalWPO (Vector3) output 0

### [3] Multiply

- `A` <= [5] Multiply output 0
- `B` <= [4] Input WindIntensity (Scalar) output 0

### [5] Multiply

- `A` <= [7] RotateAboutAxis output 0
- `B` <= [6] Input WindWeight (Scalar) output 0

### [7] RotateAboutAxis

- `NormalizedRotationAxis` <= [59] Cross output 0
- `RotationAngle` <= [11] Add output 0
- `PivotPoint` <= [8] Add output 0
- `Position` <= [2] Input AdditionalWPO (Vector3) output 0

### [8] Add

- `A` <= [2] Input AdditionalWPO (Vector3) output 0
- `B` <= [9] 0,0,-10 output 0

### [10] Append

- `B` <= [11] Add output 0

### [11] Add

- `A` <= [37] Dot output 0
- `B` <= [12] Distance output 0

### [12] Distance

- `A` <= [14] Multiply output 0
- `B` <= [13] 0 output 0

### [14] Multiply

- `A` <= [32] Multiply output 0
- `B` <= [15] Abs output 0

### [15] Abs

- `Input` <= [16] Add output 0

### [16] Add

- `A` <= [18] Multiply output 0
- `B` <= [17] -1 output 0

### [18] Multiply

- `A` <= [20] Frac output 0
- `B` <= [19] 2 output 0

### [20] Frac

- `Input` <= [21] Add output 0

### [21] Add

- `A` <= [23] Add output 0
- `B` <= [22] 0.5 output 0

### [23] Add

- `A` <= [27] Multiply output 0
- `B` <= [24] Divide output 0

### [24] Divide

- `A` <= [26] Absolute World Position output 0
- `B` <= [25] 200 output 0

### [27] Multiply

- `A` <= [31] Mask ( A ) output 0
- `B` <= [28] Multiply output 0

### [28] Multiply

- `A` <= [62] Multiply output 0
- `B` <= [29] -0.5 output 0

### [31] Mask ( A )

- `Input` <= [61] 0,1,0,1 output 0

### [32] Multiply

- `A` <= [33] Subtract output 0
- `B` <= [15] Abs output 0

### [33] Subtract

- `A` <= [36] 3 output 0
- `B` <= [34] Multiply output 0

### [34] Multiply

- `A` <= [15] Abs output 0
- `B` <= [35] 2 output 0

### [37] Dot

- `A` <= [52] Normalize output 0
- `B` <= [38] Multiply output 0

### [38] Multiply

- `A` <= [54] Multiply output 0
- `B` <= [39] Abs output 0

### [39] Abs

- `Input` <= [40] Add output 0

### [40] Add

- `A` <= [42] Multiply output 0
- `B` <= [41] -1 output 0

### [42] Multiply

- `A` <= [44] Frac output 0
- `B` <= [43] 2 output 0

### [44] Frac

- `Input` <= [45] Add output 0

### [45] Add

- `A` <= [47] Add output 0
- `B` <= [46] 0.5 output 0

### [47] Add

- `A` <= [51] Multiply output 0
- `B` <= [48] Divide output 0

### [48] Divide

- `A` <= [50] Absolute World Position output 0
- `B` <= [49] 1024 output 0

### [51] Multiply

- `A` <= [52] Normalize output 0
- `B` <= [27] Multiply output 0

### [52] Normalize

- `VectorInput` <= [53] Mask ( R G B ) output 0

### [53] Mask ( R G B )

- `Input` <= [61] 0,1,0,1 output 0

### [54] Multiply

- `A` <= [55] Subtract output 0
- `B` <= [39] Abs output 0

### [55] Subtract

- `A` <= [58] 3 output 0
- `B` <= [56] Multiply output 0

### [56] Multiply

- `A` <= [39] Abs output 0
- `B` <= [57] 2 output 0

### [59] Cross

- `A` <= [52] Normalize output 0
- `B` <= [60] 0,0,1 output 0

### [62] Multiply

- `A` <= [30] Time output 0
- `B` <= [63] Input WindSpeed (Scalar) output 0

## Connection List

- [1] Add output 0 -> Output Result.A
- [3] Multiply output 0 -> Add.A
- [2] Input AdditionalWPO (Vector3) output 0 -> Add.B
- [5] Multiply output 0 -> Multiply.A
- [4] Input WindIntensity (Scalar) output 0 -> Multiply.B
- [7] RotateAboutAxis output 0 -> Multiply.A
- [6] Input WindWeight (Scalar) output 0 -> Multiply.B
- [59] Cross output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [11] Add output 0 -> RotateAboutAxis.RotationAngle
- [8] Add output 0 -> RotateAboutAxis.PivotPoint
- [2] Input AdditionalWPO (Vector3) output 0 -> RotateAboutAxis.Position
- [2] Input AdditionalWPO (Vector3) output 0 -> Add.A
- [9] 0,0,-10 output 0 -> Add.B
- [11] Add output 0 -> Append.B
- [37] Dot output 0 -> Add.A
- [12] Distance output 0 -> Add.B
- [14] Multiply output 0 -> Distance.A
- [13] 0 output 0 -> Distance.B
- [32] Multiply output 0 -> Multiply.A
- [15] Abs output 0 -> Multiply.B
- [16] Add output 0 -> Abs.Input
- [18] Multiply output 0 -> Add.A
- [17] -1 output 0 -> Add.B
- [20] Frac output 0 -> Multiply.A
- [19] 2 output 0 -> Multiply.B
- [21] Add output 0 -> Frac.Input
- [23] Add output 0 -> Add.A
- [22] 0.5 output 0 -> Add.B
- [27] Multiply output 0 -> Add.A
- [24] Divide output 0 -> Add.B
- [26] Absolute World Position output 0 -> Divide.A
- [25] 200 output 0 -> Divide.B
- [31] Mask ( A ) output 0 -> Multiply.A
- [28] Multiply output 0 -> Multiply.B
- [62] Multiply output 0 -> Multiply.A
- [29] -0.5 output 0 -> Multiply.B
- [61] 0,1,0,1 output 0 -> Mask ( A ).Input
- [33] Subtract output 0 -> Multiply.A
- [15] Abs output 0 -> Multiply.B
- [36] 3 output 0 -> Subtract.A
- [34] Multiply output 0 -> Subtract.B
- [15] Abs output 0 -> Multiply.A
- [35] 2 output 0 -> Multiply.B
- [52] Normalize output 0 -> Dot.A
- [38] Multiply output 0 -> Dot.B
- [54] Multiply output 0 -> Multiply.A
- [39] Abs output 0 -> Multiply.B
- [40] Add output 0 -> Abs.Input
- [42] Multiply output 0 -> Add.A
- [41] -1 output 0 -> Add.B
- [44] Frac output 0 -> Multiply.A
- [43] 2 output 0 -> Multiply.B
- [45] Add output 0 -> Frac.Input
- [47] Add output 0 -> Add.A
- [46] 0.5 output 0 -> Add.B
- [51] Multiply output 0 -> Add.A
- [48] Divide output 0 -> Add.B
- [50] Absolute World Position output 0 -> Divide.A
- [49] 1024 output 0 -> Divide.B
- [52] Normalize output 0 -> Multiply.A
- [27] Multiply output 0 -> Multiply.B
- [53] Mask ( R G B ) output 0 -> Normalize.VectorInput
- [61] 0,1,0,1 output 0 -> Mask ( R G B ).Input
- [55] Subtract output 0 -> Multiply.A
- [39] Abs output 0 -> Multiply.B
- [58] 3 output 0 -> Subtract.A
- [56] Multiply output 0 -> Subtract.B
- [39] Abs output 0 -> Multiply.A
- [57] 2 output 0 -> Multiply.B
- [52] Normalize output 0 -> Cross.A
- [60] 0,0,1 output 0 -> Cross.B
- [30] Time output 0 -> Multiply.A
- [63] Input WindSpeed (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [1] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


