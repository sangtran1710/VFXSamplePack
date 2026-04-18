# PivotPainter_TreeData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_TreeData.PivotPainter_TreeData`
- Category: Engine_MaterialFunctions02/PivotPainter
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Processes and organizes world position and angle information stored in the model's uvs by the painter script. The outputs starting with (tree) process the models uv information as it would be stored by the  Pivot Painter MAXScript. The outputs starting with (Leaf) process the uv information as it would be stored by the per object pivot painting section of the script.

## Inputs

- `Max Dist for Parent Piv` [FunctionInput_Scalar] - This value should match the value that was used in the max script "Max Dist for Parent Piv" spinner while painting your asset. 
- `WindVector` [FunctionInput_Vector3]

## Outputs

- `Leaf Rot Axis` - Returns a rotation axis for leaves to be used with the "RotateAboutAxis" node. Branches return (0,0,0). Note: The angle is found by transforming a vector along the leaf's x-axis from local to world space. Then the cross product is found between the wind direction and the transformed vector.
- `Leaf Piv Position` - Return the pivot position information in world space for each leaf. Branches return a value of (0,0,0).
- `Branch Piv Position` - Returns the pivot point information for each branch in world space. Leaves return the pivot point information of the branch they're attached to. 
- `Branch Wind Rot Axis` - Returns a rotation axis for branches to be used with the "RotateAboutAxis" node. Leaves return the same information. Note: The angle is found by transforming a vector along the branch's x-axis from local to world space. Then the cross product is found between the wind direction and the transformed vector.
- `Branch-X Axis Vector` - Returns a normalized vector pointing down the branches x-axis. This is not typically needed for foliage animations unless it is needed for custom vector calculations.
- `WindSpeed` - Wind speed multiplied by time.
- `WindStrength` - Returns wind strength. The magnitude of the wind vector found by calculating distance from the wind vector to 0. 
- `Leaf X-Axis Vector` - Returns a normalized vector pointing down the branches x-axis. This is not typically needed for foliage animations unless it is needed for custom vector calculations.
- `Normalized Wind Vector`
- `Object Scale`
- `Object  Pivot Point`
- `Object Orientation`
- `Object Rotation Axis`
- `Branch flow Grad` - Returns a gradient value in the direction of the blowing wind.
- `Branch flow Grad 90 deg` - Returns a gradient value across world space at a 90 degree angle to the wind.
- `Leaf flow Grad` - Returns a gradient value in the direction of the blowing wind.
- `Leaf flow Grad 90 deg` - Returns a gradient value across world space at a 90 degree angle to the wind.
- `------------------------` - Empty
- `------------------------ ` - Empty
- `------------------------  ` - Empty
- `Object Flow Grad` - Gradient aligned with the wind vector at object level in world space.
- `Object Flow Grad 90 deg` - Gradient aligned to the wind vector at a 90 degree angle at object level in world space.
- `------------------------   ` - Empty
- `Black Mask` - Returns a black value for surfaces that are painted black by the pivot painter tool. This mask only works in the vertex shader.
- `Leaf Mask` - Returns a white mask for leaves. All branches are black.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | 0.5,0.5,0 |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output Leaf Rot Axis | Leaf Rot Axis |
| 5 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 10 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 11 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 14 | expression | `MaterialExpressionFrac` | Frac |  |
| 15 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output Leaf Piv Position | Leaf Piv Position |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Branch Piv Position | Branch Piv Position |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Branch Wind Rot Axis | Branch Wind Rot Axis |
| 19 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 20 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Branch-X Axis Vector | Branch-X Axis Vector |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output WindSpeed | WindSpeed |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output WindStrength | WindStrength |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output Leaf X-Axis Vector | Leaf X-Axis Vector |
| 25 | function_call | `MaterialExpressionMaterialFunctionCall` | Wind |  |
| 26 | function_output | `MaterialExpressionFunctionOutput` | Output Normalized Wind Vector | Normalized Wind Vector |
| 27 | function_output | `MaterialExpressionFunctionOutput` | Output Object Scale | Object Scale |
| 28 | function_output | `MaterialExpressionFunctionOutput` | Output Object  Pivot Point | Object  Pivot Point |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Object Orientation | Object Orientation |
| 30 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 32 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 33 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 34 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 35 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 36 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 37 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 38 | function_output | `MaterialExpressionFunctionOutput` | Output Object Rotation Axis | Object Rotation Axis |
| 39 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 40 | function_output | `MaterialExpressionFunctionOutput` | Output Branch flow Grad | Branch flow Grad |
| 41 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 42 | function_output | `MaterialExpressionFunctionOutput` | Output Branch flow Grad 90 deg | Branch flow Grad 90 deg |
| 43 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 44 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 45 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 46 | function_output | `MaterialExpressionFunctionOutput` | Output Leaf flow Grad | Leaf flow Grad |
| 47 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 48 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 49 | function_output | `MaterialExpressionFunctionOutput` | Output Leaf flow Grad 90 deg | Leaf flow Grad 90 deg |
| 50 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 51 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 52 | function_output | `MaterialExpressionFunctionOutput` | Output ------------------------ | ------------------------ |
| 53 | function_output | `MaterialExpressionFunctionOutput` | Output ------------------------  | ------------------------  |
| 54 | function_output | `MaterialExpressionFunctionOutput` | Output ------------------------   | ------------------------   |
| 55 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 56 | function_output | `MaterialExpressionFunctionOutput` | Output Object Flow Grad | Object Flow Grad |
| 57 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 58 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 59 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 60 | function_output | `MaterialExpressionFunctionOutput` | Output Object Flow Grad 90 deg | Object Flow Grad 90 deg |
| 61 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 62 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 63 | expression | `MaterialExpressionConstant` | 4096 |  |
| 64 | function_input | `MaterialExpressionFunctionInput` | Input Max Dist for Parent Piv (Scalar) | Max Dist for Parent Piv |
| 65 | expression | `MaterialExpressionAdd` | Add |  |
| 66 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 67 | expression | `MaterialExpressionAdd` | Add |  |
| 68 | expression | `MaterialExpressionAdd` | Add |  |
| 69 | function_output | `MaterialExpressionFunctionOutput` | Output ------------------------    | ------------------------    |
| 70 | function_output | `MaterialExpressionFunctionOutput` | Output Black Mask | Black Mask |
| 71 | expression | `MaterialExpressionIf` | If |  |
| 72 | expression | `MaterialExpressionConstant` | 0 |  |
| 73 | expression | `MaterialExpressionConstant` | 1 |  |
| 74 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 75 | function_input | `MaterialExpressionFunctionInput` | Input WindVector (Vector3) | WindVector |
| 76 | expression | `MaterialExpressionIf` | If |  |
| 77 | expression | `MaterialExpressionConstant` | 0 |  |
| 78 | expression | `MaterialExpressionConstant` | 1 |  |
| 79 | function_output | `MaterialExpressionFunctionOutput` | Output Leaf Mask | Leaf Mask |
| 80 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 81 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 82 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 83 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 84 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 85 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 86 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 87 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 88 | expression | `MaterialExpressionAppendVector` | Append |  |
| 89 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 90 | expression | `MaterialExpressionAppendVector` | Append |  |
| 91 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 92 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 93 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 94 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 95 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 96 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 97 | expression | `MaterialExpressionConstant` | 1 |  |
| 98 | expression | `MaterialExpressionIf` | If |  |
| 99 | expression | `MaterialExpressionConstant` | 0 |  |
| 100 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,-1,) |  |
| 101 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,-1,) |  |
| 102 | expression | `MaterialExpressionAdd` | Add |  |
| 103 | expression | `MaterialExpressionConstant` | 1 |  |
| 104 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 105 | expression | `MaterialExpressionIf` | If |  |
| 106 | expression | `MaterialExpressionConstant` | 0.999 |  |
| 107 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 108 | function_call | `MaterialExpressionMaterialFunctionCall` | AddComponents |  |
| 109 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 110 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 111 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 112 | expression | `MaterialExpressionAdd` | Add(,1e-06) |  |
| 113 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 114 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 115 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 116 | expression | `MaterialExpressionConstant3Vector` | 2,2,1 |  |

## Exact Input Wiring

### [1] Multiply

- `A` <= [115] Multiply output 0
- `B` <= [2] Subtract output 0

### [2] Subtract

- `A` <= [0] Vertex Color output 0
- `B` <= [3] 0.5,0.5,0 output 0

### [4] Output Leaf Rot Axis

- `A` <= [34] Normalize output 0

### [6] Append

- `A` <= [5] TexCoord[2] output 0
- `B` <= [8] Mask ( R ) output 0

### [7] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [6] Append output 0

### [8] Mask ( R )

- `Input` <= [10] TexCoord[3] output 0

### [9] Mask ( R G B )

- `Input` <= [7] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [11] Instance & Particle Space to World Space TransformVector

- `Input` <= [12] Normalize output 0

### [12] Normalize

- `VectorInput` <= [13] ConstantBiasScale output 0

### [13] ConstantBiasScale

- `Input` <= [14] Frac output 0

### [14] Frac

- `Input` <= [6] Append output 0

### [15] Cross

- `A` <= [11] Instance & Particle Space to World Space TransformVector output 0
- `B` <= [83] Normalize output 0

### [16] Output Leaf Piv Position

- `A` <= [9] Mask ( R G B ) output 0

### [17] Output Branch Piv Position

- `A` <= [74] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [18] Output Branch Wind Rot Axis

- `A` <= [20] Normalize output 0

### [19] Cross

- `A` <= [84] SafeNormalize output 0
- `B` <= [83] Normalize output 0

### [20] Normalize

- `VectorInput` <= [19] Cross output 0

### [21] Output Branch-X Axis Vector

- `A` <= [84] SafeNormalize output 0

### [22] Output WindSpeed

- `A` <= [25] Wind output 2

### [23] Output WindStrength

- `A` <= [25] Wind output 1

### [24] Output Leaf X-Axis Vector

- `A` <= [35] Normalize output 0

### [26] Output Normalized Wind Vector

- `A` <= [25] Wind output 0

### [27] Output Object Scale

- `A` <= [82] ObjectScale output 3

### [28] Output Object  Pivot Point

- `A` <= [31] Mask ( R G B ) output 0

### [29] Output Object Orientation

- `A` <= [37] Normalize output 0

### [30] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [114] 0,0,0 output 0

### [31] Mask ( R G B )

- `Input` <= [30] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [33] Instance & Particle Space to World Space TransformVector

- `Input` <= [32] 0,0,1 output 0

### [34] Normalize

- `VectorInput` <= [15] Cross output 0

### [35] Normalize

- `VectorInput` <= [11] Instance & Particle Space to World Space TransformVector output 0

### [36] Cross

- `A` <= [37] Normalize output 0
- `B` <= [25] Wind output 0

### [37] Normalize

- `VectorInput` <= [33] Instance & Particle Space to World Space TransformVector output 0

### [38] Output Object Rotation Axis

- `A` <= [39] Normalize output 0

### [39] Normalize

- `VectorInput` <= [36] Cross output 0

### [40] Output Branch flow Grad

- `A` <= [41] Dot output 0

### [41] Dot

- `A` <= [74] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `B` <= [83] Normalize output 0

### [42] Output Branch flow Grad 90 deg

- `A` <= [45] Dot output 0

### [44] Cross

- `A` <= [83] Normalize output 0
- `B` <= [43] 0,0,1 output 0

### [45] Dot

- `A` <= [74] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `B` <= [44] Cross output 0

### [46] Output Leaf flow Grad

- `A` <= [61] Mask ( R ) output 0

### [47] Dot

- `A` <= [9] Mask ( R G B ) output 0
- `B` <= [25] Wind output 0

### [48] Dot

- `A` <= [9] Mask ( R G B ) output 0
- `B` <= [50] Cross output 0

### [49] Output Leaf flow Grad 90 deg

- `A` <= [62] Mask ( R ) output 0

### [50] Cross

- `A` <= [25] Wind output 0
- `B` <= [51] 0,0,1 output 0

### [55] Dot

- `A` <= [31] Mask ( R G B ) output 0
- `B` <= [25] Wind output 0

### [56] Output Object Flow Grad

- `A` <= [55] Dot output 0

### [57] Dot

- `A` <= [31] Mask ( R G B ) output 0
- `B` <= [58] Cross output 0

### [58] Cross

- `A` <= [25] Wind output 0
- `B` <= [59] 0,0,1 output 0

### [60] Output Object Flow Grad 90 deg

- `A` <= [57] Dot output 0

### [61] Mask ( R )

- `Input` <= [47] Dot output 0

### [62] Mask ( R )

- `Input` <= [48] Dot output 0

### [64] Input Max Dist for Parent Piv (Scalar)

- `Preview` <= [63] 4096 output 0

### [65] Add

- `A` <= [66] Vertex Color output 1
- `B` <= [66] Vertex Color output 2

### [67] Add

- `A` <= [65] Add output 0
- `B` <= [66] Vertex Color output 3

### [68] Add

- `A` <= [67] Add output 0
- `B` <= [66] Vertex Color output 4

### [70] Output Black Mask

- `A` <= [71] If output 0

### [71] If

- `A` <= [68] Add output 0
- `B` <= [72] 0 output 0
- `AGreaterThanB` <= [73] 1 output 0
- `AEqualsB` <= [72] 0 output 0
- `ALessThanB` <= [73] 1 output 0

### [74] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [1] Multiply output 0

### [76] If

- `A` <= [80] Mask ( R ) output 0
- `B` <= [77] 0 output 0
- `AGreaterThanB` <= [78] 1 output 0
- `AEqualsB` <= [77] 0 output 0
- `ALessThanB` <= [78] 1 output 0

### [79] Output Leaf Mask

- `A` <= [76] If output 0

### [80] Mask ( R )

- `Input` <= [81] TexCoord[3] output 0

### [83] Normalize

- `VectorInput` <= [75] Input WindVector (Vector3) output 0

### [86] ConstantBiasScale

- `Input` <= [85] Vertex Color output 4

### [87] Instance & Particle Space to World Space TransformVector

- `Input` <= [88] Append output 0

### [88] Append

- `A` <= [89] Mask ( R G ) output 0
- `B` <= [93] Multiply output 0

### [89] Mask ( R G )

- `Input` <= [91] DeriveNormalZ output 0

### [90] Append

- `A` <= [86] ConstantBiasScale output 0
- `B` <= [104] Multiply output 0

### [91] DeriveNormalZ

- `InXY` <= [105] If output 0

### [92] Mask ( B )

- `Input` <= [91] DeriveNormalZ output 0

### [93] Multiply

- `A` <= [92] Mask ( B ) output 0
- `B` <= [100] Lerp(1,-1,) output 0

### [94] ConstantBiasScale

- `Input` <= [102] Add output 0

### [95] Mask ( G )

- `Input` <= [96] TexCoord[3] output 0

### [98] If

- `A` <= [95] Mask ( G ) output 0
- `B` <= [103] 1 output 0
- `AGreaterThanB` <= [97] 1 output 0
- `ALessThanB` <= [99] 0 output 0

### [100] Lerp(1,-1,)

- `Alpha` <= [98] If output 0

### [101] Lerp(0,-1,)

- `Alpha` <= [98] If output 0

### [102] Add

- `A` <= [95] Mask ( G ) output 0
- `B` <= [101] Lerp(0,-1,) output 0

### [104] Multiply

- `A` <= [94] ConstantBiasScale output 0
- `B` <= [113] Multiply(,-1) output 0

### [105] If

- `A` <= [108] AddComponents output 0
- `B` <= [106] 0.999 output 0
- `AGreaterThanB` <= [111] Multiply output 0
- `ALessThanB` <= [90] Append output 0

### [107] Multiply

- `A` <= [90] Append output 0
- `B` <= [90] Append output 0

### [109] Divide(1,)

- `B` <= [112] Add(,1e-06) output 0

### [110] Sqrt

- `Input` <= [108] AddComponents output 0

### [111] Multiply

- `A` <= [90] Append output 0
- `B` <= [109] Divide(1,) output 0

### [112] Add(,1e-06)

- `A` <= [110] Sqrt output 0

### [113] Multiply(,-1)

- `A` <= [100] Lerp(1,-1,) output 0

### [115] Multiply

- `A` <= [64] Input Max Dist for Parent Piv (Scalar) output 0
- `B` <= [116] 2,2,1 output 0

## Connection List

- [115] Multiply output 0 -> Multiply.A
- [2] Subtract output 0 -> Multiply.B
- [0] Vertex Color output 0 -> Subtract.A
- [3] 0.5,0.5,0 output 0 -> Subtract.B
- [34] Normalize output 0 -> Output Leaf Rot Axis.A
- [5] TexCoord[2] output 0 -> Append.A
- [8] Mask ( R ) output 0 -> Append.B
- [6] Append output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [10] TexCoord[3] output 0 -> Mask ( R ).Input
- [7] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Mask ( R G B ).Input
- [12] Normalize output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [13] ConstantBiasScale output 0 -> Normalize.VectorInput
- [14] Frac output 0 -> ConstantBiasScale.Input
- [6] Append output 0 -> Frac.Input
- [11] Instance & Particle Space to World Space TransformVector output 0 -> Cross.A
- [83] Normalize output 0 -> Cross.B
- [9] Mask ( R G B ) output 0 -> Output Leaf Piv Position.A
- [74] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Branch Piv Position.A
- [20] Normalize output 0 -> Output Branch Wind Rot Axis.A
- [84] SafeNormalize output 0 -> Cross.A
- [83] Normalize output 0 -> Cross.B
- [19] Cross output 0 -> Normalize.VectorInput
- [84] SafeNormalize output 0 -> Output Branch-X Axis Vector.A
- [25] Wind output 2 -> Output WindSpeed.A
- [25] Wind output 1 -> Output WindStrength.A
- [35] Normalize output 0 -> Output Leaf X-Axis Vector.A
- [25] Wind output 0 -> Output Normalized Wind Vector.A
- [82] ObjectScale output 3 -> Output Object Scale.A
- [31] Mask ( R G B ) output 0 -> Output Object  Pivot Point.A
- [37] Normalize output 0 -> Output Object Orientation.A
- [114] 0,0,0 output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [30] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Mask ( R G B ).Input
- [32] 0,0,1 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [15] Cross output 0 -> Normalize.VectorInput
- [11] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [37] Normalize output 0 -> Cross.A
- [25] Wind output 0 -> Cross.B
- [33] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [39] Normalize output 0 -> Output Object Rotation Axis.A
- [36] Cross output 0 -> Normalize.VectorInput
- [41] Dot output 0 -> Output Branch flow Grad.A
- [74] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Dot.A
- [83] Normalize output 0 -> Dot.B
- [45] Dot output 0 -> Output Branch flow Grad 90 deg.A
- [83] Normalize output 0 -> Cross.A
- [43] 0,0,1 output 0 -> Cross.B
- [74] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Dot.A
- [44] Cross output 0 -> Dot.B
- [61] Mask ( R ) output 0 -> Output Leaf flow Grad.A
- [9] Mask ( R G B ) output 0 -> Dot.A
- [25] Wind output 0 -> Dot.B
- [9] Mask ( R G B ) output 0 -> Dot.A
- [50] Cross output 0 -> Dot.B
- [62] Mask ( R ) output 0 -> Output Leaf flow Grad 90 deg.A
- [25] Wind output 0 -> Cross.A
- [51] 0,0,1 output 0 -> Cross.B
- [31] Mask ( R G B ) output 0 -> Dot.A
- [25] Wind output 0 -> Dot.B
- [55] Dot output 0 -> Output Object Flow Grad.A
- [31] Mask ( R G B ) output 0 -> Dot.A
- [58] Cross output 0 -> Dot.B
- [25] Wind output 0 -> Cross.A
- [59] 0,0,1 output 0 -> Cross.B
- [57] Dot output 0 -> Output Object Flow Grad 90 deg.A
- [47] Dot output 0 -> Mask ( R ).Input
- [48] Dot output 0 -> Mask ( R ).Input
- [63] 4096 output 0 -> Input Max Dist for Parent Piv (Scalar).Preview
- [66] Vertex Color output 1 -> Add.A
- [66] Vertex Color output 2 -> Add.B
- [65] Add output 0 -> Add.A
- [66] Vertex Color output 3 -> Add.B
- [67] Add output 0 -> Add.A
- [66] Vertex Color output 4 -> Add.B
- [71] If output 0 -> Output Black Mask.A
- [68] Add output 0 -> If.A
- [72] 0 output 0 -> If.B
- [73] 1 output 0 -> If.AGreaterThanB
- [72] 0 output 0 -> If.AEqualsB
- [73] 1 output 0 -> If.ALessThanB
- [1] Multiply output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [80] Mask ( R ) output 0 -> If.A
- [77] 0 output 0 -> If.B
- [78] 1 output 0 -> If.AGreaterThanB
- [77] 0 output 0 -> If.AEqualsB
- [78] 1 output 0 -> If.ALessThanB
- [76] If output 0 -> Output Leaf Mask.A
- [81] TexCoord[3] output 0 -> Mask ( R ).Input
- [75] Input WindVector (Vector3) output 0 -> Normalize.VectorInput
- [85] Vertex Color output 4 -> ConstantBiasScale.Input
- [88] Append output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [89] Mask ( R G ) output 0 -> Append.A
- [93] Multiply output 0 -> Append.B
- [91] DeriveNormalZ output 0 -> Mask ( R G ).Input
- [86] ConstantBiasScale output 0 -> Append.A
- [104] Multiply output 0 -> Append.B
- [105] If output 0 -> DeriveNormalZ.InXY
- [91] DeriveNormalZ output 0 -> Mask ( B ).Input
- [92] Mask ( B ) output 0 -> Multiply.A
- [100] Lerp(1,-1,) output 0 -> Multiply.B
- [102] Add output 0 -> ConstantBiasScale.Input
- [96] TexCoord[3] output 0 -> Mask ( G ).Input
- [95] Mask ( G ) output 0 -> If.A
- [103] 1 output 0 -> If.B
- [97] 1 output 0 -> If.AGreaterThanB
- [99] 0 output 0 -> If.ALessThanB
- [98] If output 0 -> Lerp(1,-1,).Alpha
- [98] If output 0 -> Lerp(0,-1,).Alpha
- [95] Mask ( G ) output 0 -> Add.A
- [101] Lerp(0,-1,) output 0 -> Add.B
- [94] ConstantBiasScale output 0 -> Multiply.A
- [113] Multiply(,-1) output 0 -> Multiply.B
- [108] AddComponents output 0 -> If.A
- [106] 0.999 output 0 -> If.B
- [111] Multiply output 0 -> If.AGreaterThanB
- [90] Append output 0 -> If.ALessThanB
- [90] Append output 0 -> Multiply.A
- [90] Append output 0 -> Multiply.B
- [112] Add(,1e-06) output 0 -> Divide(1,).B
- [108] AddComponents output 0 -> Sqrt.Input
- [90] Append output 0 -> Multiply.A
- [109] Divide(1,) output 0 -> Multiply.B
- [110] Sqrt output 0 -> Add(,1e-06).A
- [100] Lerp(1,-1,) output 0 -> Multiply(,-1).A
- [64] Input Max Dist for Parent Piv (Scalar) output 0 -> Multiply.A
- [116] 2,2,1 output 0 -> Multiply.B

## Output Trace Roots

- `Leaf Rot Axis` is driven by [34] Normalize
- `Leaf Piv Position` is driven by [9] Mask ( R G B )
- `Branch Piv Position` is driven by [74] Instance & Particle Space to Absolute World Space TransformPosition
- `Branch Wind Rot Axis` is driven by [20] Normalize
- `Branch-X Axis Vector` is driven by [84] SafeNormalize
- `WindSpeed` is driven by [25] Wind
- `WindStrength` is driven by [25] Wind
- `Leaf X-Axis Vector` is driven by [35] Normalize
- `Normalized Wind Vector` is driven by [25] Wind
- `Object Scale` is driven by [82] ObjectScale
- `Object  Pivot Point` is driven by [31] Mask ( R G B )
- `Object Orientation` is driven by [37] Normalize
- `Object Rotation Axis` is driven by [39] Normalize
- `Branch flow Grad` is driven by [41] Dot
- `Branch flow Grad 90 deg` is driven by [45] Dot
- `Leaf flow Grad` is driven by [61] Mask ( R )
- `Leaf flow Grad 90 deg` is driven by [62] Mask ( R )
- `------------------------` has no captured source.
- `------------------------ ` has no captured source.
- `------------------------  ` has no captured source.
- `Object Flow Grad` is driven by [55] Dot
- `Object Flow Grad 90 deg` is driven by [57] Dot
- `------------------------   ` has no captured source.
- `Black Mask` is driven by [71] If
- `Leaf Mask` is driven by [76] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


