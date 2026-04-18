# SineWithNormalSupport

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/SineWithNormalSupport.SineWithNormalSupport`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Very specific material function that projects into world space and then maps a normal map in that direction and bends its normals to face in the new direction.  For test purposes only. (May remove eventually)  Outputs worls space normals

## Inputs

- `X world dot` [FunctionInput_Scalar]
- `Y world dot` [FunctionInput_Scalar]
- `World Position` [FunctionInput_Vector3]
- `time` [FunctionInput_Scalar]
- `Rotation fudge factor` [FunctionInput_Scalar]

## Outputs

- `Sine Wave World Normal`
- `SineWave`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Sine Wave World Normal | Sine Wave World Normal |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 6 | expression | `MaterialExpressionConstant` | 0.1 |  |
| 7 | expression | `MaterialExpressionSine` | Sine |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionAppendVector` | Append |  |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorToRadialValue |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 14 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionFrac` | Frac |  |
| 17 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 18 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 19 | expression | `MaterialExpressionTwoSidedSign` | TwoSidedSign |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 22 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input X world dot (Scalar) | X world dot |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Y world dot (Scalar) | Y world dot |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output SineWave | SineWave |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input time (Scalar) | time |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Rotation fudge factor (Scalar) | Rotation fudge factor |

## Exact Input Wiring

### [0] Output Sine Wave World Normal

- `A` <= [17] RotateAboutAxis output 0

### [2] Dot

- `A` <= [28] Input World Position (Vector3) output 0
- `B` <= [3] Multiply(,-1) output 0

### [3] Multiply(,-1)

- `A` <= [9] Append output 0

### [4] Add

- `A` <= [2] Dot output 0
- `B` <= [30] Input time (Scalar) output 0

### [7] Sine

- `Input` <= [4] Add output 0

### [8] Append

- `A` <= [26] Input X world dot (Scalar) output 0
- `B` <= [27] Input Y world dot (Scalar) output 0

### [9] Append

- `A` <= [8] Append output 0
- `B` <= [11] 0 output 0

### [12] Normalize

- `VectorInput` <= [9] Append output 0

### [13] Mask ( R G )

- `Input` <= [12] Normalize output 0

### [15] Add

- `A` <= [10] VectorToRadialValue output 1
- `B` <= [31] Input Rotation fudge factor (Scalar) output 0

### [16] Frac

- `Input` <= [15] Add output 0

### [17] RotateAboutAxis

- `NormalizedRotationAxis` <= [14] 0,0,1 output 0
- `RotationAngle` <= [16] Frac output 0
- `PivotPoint` <= [18] 0,0,0 output 0
- `Position` <= [22] Normalize output 0

### [20] Multiply

- `A` <= [19] TwoSidedSign output 0
- `B` <= [25] Texture Sample output 0

### [21] Multiply(,-1)

- `A` <= [20] Multiply output 0

### [22] Normalize

- `VectorInput` <= [24] Append output 0

### [23] Mask ( R G )

- `Input` <= [21] Multiply(,-1) output 0

### [24] Append

- `A` <= [23] Mask ( R G ) output 0
- `B` <= [25] Texture Sample output 3

### [25] Texture Sample

- `Coordinates` <= [4] Add output 0

### [28] Input World Position (Vector3)

- `Preview` <= [1] Absolute World Position output 0

### [29] Output SineWave

- `A` <= [7] Sine output 0

### [30] Input time (Scalar)

- `Preview` <= [5] TimeWithSpeedVariable output 0

## Connection List

- [17] RotateAboutAxis output 0 -> Output Sine Wave World Normal.A
- [28] Input World Position (Vector3) output 0 -> Dot.A
- [3] Multiply(,-1) output 0 -> Dot.B
- [9] Append output 0 -> Multiply(,-1).A
- [2] Dot output 0 -> Add.A
- [30] Input time (Scalar) output 0 -> Add.B
- [4] Add output 0 -> Sine.Input
- [26] Input X world dot (Scalar) output 0 -> Append.A
- [27] Input Y world dot (Scalar) output 0 -> Append.B
- [8] Append output 0 -> Append.A
- [11] 0 output 0 -> Append.B
- [9] Append output 0 -> Normalize.VectorInput
- [12] Normalize output 0 -> Mask ( R G ).Input
- [10] VectorToRadialValue output 1 -> Add.A
- [31] Input Rotation fudge factor (Scalar) output 0 -> Add.B
- [15] Add output 0 -> Frac.Input
- [14] 0,0,1 output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [16] Frac output 0 -> RotateAboutAxis.RotationAngle
- [18] 0,0,0 output 0 -> RotateAboutAxis.PivotPoint
- [22] Normalize output 0 -> RotateAboutAxis.Position
- [19] TwoSidedSign output 0 -> Multiply.A
- [25] Texture Sample output 0 -> Multiply.B
- [20] Multiply output 0 -> Multiply(,-1).A
- [24] Append output 0 -> Normalize.VectorInput
- [21] Multiply(,-1) output 0 -> Mask ( R G ).Input
- [23] Mask ( R G ) output 0 -> Append.A
- [25] Texture Sample output 3 -> Append.B
- [4] Add output 0 -> Texture Sample.Coordinates
- [1] Absolute World Position output 0 -> Input World Position (Vector3).Preview
- [7] Sine output 0 -> Output SineWave.A
- [5] TimeWithSpeedVariable output 0 -> Input time (Scalar).Preview

## Output Trace Roots

- `Sine Wave World Normal` is driven by [17] RotateAboutAxis
- `SineWave` is driven by [7] Sine

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


