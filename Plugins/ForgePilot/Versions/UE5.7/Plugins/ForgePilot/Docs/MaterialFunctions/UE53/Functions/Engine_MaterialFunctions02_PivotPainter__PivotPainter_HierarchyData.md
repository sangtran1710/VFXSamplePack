# PivotPainter_HierarchyData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_HierarchyData.PivotPainter_HierarchyData`
- Category: Engine_MaterialFunctions02/PivotPainter
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Processes and organizes world position and angle information stored in the model's uvs by the Pivot Painter MAXScript. 

## Inputs

- `Max Dist for Parent Piv` [FunctionInput_Scalar] - This value should match the value that was used in the max script "Max Dist for Parent Piv" spinner while painting your asset. 

## Outputs

- `Child Piv Position` - Return the pivot position information in world space for each child. Parents return a value of (0,0,0).
- `Parent Piv Position` - Returns the pivot point information for each parent in world space. Children return the pivot point information of the parent they're attached to. 
- `Parent X Axis Vector` - Returns a normalized vector pointing down the parent's x-axis. 
- `Child X-Axis Vector` - Returns a normalized vector pointing down the parent's x-axis.
- `Object Scale`
- `Object  Pivot Point`
- `Object Orientation`
- `------------------------` - Empty
- `Black Mask` - Returns a black value for surfaces that are painted black by the pivot painter tool. This mask only works in the vertex shader.
- `------------------------ ` - Empty

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | 0.5,0.5,0 |  |
| 4 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 5 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 10 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 11 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 12 | expression | `MaterialExpressionFrac` | Frac |  |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output Child Piv Position | Child Piv Position |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Parent Piv Position | Parent Piv Position |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Parent X Axis Vector | Parent X Axis Vector |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output Child X-Axis Vector | Child X-Axis Vector |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Object Scale | Object Scale |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output Object  Pivot Point | Object  Pivot Point |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Object Orientation | Object Orientation |
| 21 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 22 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 23 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 24 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 25 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 26 | function_output | `MaterialExpressionFunctionOutput` | Output ------------------------ | ------------------------ |
| 27 | expression | `MaterialExpressionConstant` | 4096 |  |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Max Dist for Parent Piv (Scalar) | Max Dist for Parent Piv |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Black Mask | Black Mask |
| 30 | function_output | `MaterialExpressionFunctionOutput` | Output ------------------------  | ------------------------  |
| 31 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 35 | expression | `MaterialExpressionAdd` | Add |  |
| 36 | expression | `MaterialExpressionAdd` | Add |  |
| 37 | expression | `MaterialExpressionIf` | If |  |
| 38 | expression | `MaterialExpressionConstant` | 0 |  |
| 39 | expression | `MaterialExpressionConstant` | 1 |  |
| 40 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 41 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 42 | expression | `MaterialExpressionTransform` | Local Space to World Space TransformVector |  |
| 43 | expression | `MaterialExpressionAppendVector` | Append |  |
| 44 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 45 | expression | `MaterialExpressionAppendVector` | Append |  |
| 46 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 47 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 48 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 49 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 50 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 51 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 52 | expression | `MaterialExpressionConstant` | 1 |  |
| 53 | expression | `MaterialExpressionIf` | If |  |
| 54 | expression | `MaterialExpressionConstant` | 0 |  |
| 55 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,-1,) |  |
| 56 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,-1,) |  |
| 57 | expression | `MaterialExpressionAdd` | Add |  |
| 58 | expression | `MaterialExpressionConstant` | 1 |  |
| 59 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 60 | expression | `MaterialExpressionIf` | If |  |
| 61 | expression | `MaterialExpressionConstant` | 0.999 |  |
| 62 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 63 | function_call | `MaterialExpressionMaterialFunctionCall` | AddComponents |  |
| 64 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 65 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 66 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 67 | expression | `MaterialExpressionAdd` | Add(,1e-06) |  |
| 68 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 69 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 70 | expression | `MaterialExpressionDivide` | Divide(,255) |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 72 | expression | `MaterialExpressionAdd` | Add |  |
| 73 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 74 | expression | `MaterialExpressionConstant3Vector` | -1,-1,1 |  |
| 75 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 76 | expression | `MaterialExpressionClamp` | Clamp |  |
| 77 | expression | `MaterialExpressionMultiply` | Multiply(,255) |  |
| 78 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 79 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 80 | expression | `MaterialExpressionConstant3Vector` | 2,2,1 |  |
| 81 | expression | `MaterialExpressionAbs` | Abs |  |
| 82 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 83 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 84 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 85 | expression | `MaterialExpressionConstant3Vector` | 0.5,0.5,0 |  |
| 86 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 87 | expression | `MaterialExpressionConstant3Vector` | 2,2,1 |  |
| 88 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 89 | function_call | `MaterialExpressionMaterialFunctionCall` | ProtectFrom0 |  |
| 90 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 91 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |

## Exact Input Wiring

### [1] Multiply

- `A` <= [28] Input Max Dist for Parent Piv (Scalar) output 0
- `B` <= [79] Multiply output 0

### [2] Subtract

- `A` <= [0] Vertex Color output 0
- `B` <= [3] 0.5,0.5,0 output 0

### [4] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [83] Multiply output 0

### [6] Append

- `A` <= [5] TexCoord[2] output 0
- `B` <= [8] Mask ( R ) output 0

### [7] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [6] Append output 0

### [8] Mask ( R )

- `Input` <= [9] TexCoord[3] output 0

### [10] Instance & Particle Space to World Space TransformVector

- `Input` <= [90] SafeNormalize output 0

### [11] ConstantBiasScale

- `Input` <= [12] Frac output 0

### [12] Frac

- `Input` <= [6] Append output 0

### [13] Output Child Piv Position

- `A` <= [7] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [14] Output Parent Piv Position

- `A` <= [4] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [15] Output Parent X Axis Vector

- `A` <= [69] SafeNormalize output 0

### [16] Output Child X-Axis Vector

- `A` <= [24] Normalize output 0

### [17] Output Object Scale

- `A` <= [18] Mask ( R ) output 0

### [18] Mask ( R )

- `Input` <= [32] ObjectScale output 0

### [19] Output Object  Pivot Point

- `A` <= [21] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [20] Output Object Orientation

- `A` <= [25] Normalize output 0

### [21] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [31] 0,0,0 output 0

### [23] Instance & Particle Space to World Space TransformVector

- `Input` <= [22] 0,0,1 output 0

### [24] Normalize

- `VectorInput` <= [10] Instance & Particle Space to World Space TransformVector output 0

### [25] Normalize

- `VectorInput` <= [23] Instance & Particle Space to World Space TransformVector output 0

### [28] Input Max Dist for Parent Piv (Scalar)

- `Preview` <= [27] 4096 output 0

### [29] Output Black Mask

- `A` <= [37] If output 0

### [33] Add

- `A` <= [34] Vertex Color output 1
- `B` <= [34] Vertex Color output 2

### [35] Add

- `A` <= [33] Add output 0
- `B` <= [34] Vertex Color output 3

### [36] Add

- `A` <= [35] Add output 0
- `B` <= [34] Vertex Color output 4

### [37] If

- `A` <= [36] Add output 0
- `B` <= [38] 0 output 0
- `AGreaterThanB` <= [39] 1 output 0
- `AEqualsB` <= [38] 0 output 0
- `ALessThanB` <= [39] 1 output 0

### [41] ConstantBiasScale

- `Input` <= [40] Vertex Color output 4

### [42] Local Space to World Space TransformVector

- `Input` <= [43] Append output 0

### [43] Append

- `A` <= [44] Mask ( R G ) output 0
- `B` <= [48] Multiply output 0

### [44] Mask ( R G )

- `Input` <= [46] DeriveNormalZ output 0

### [45] Append

- `A` <= [41] ConstantBiasScale output 0
- `B` <= [59] Multiply output 0

### [46] DeriveNormalZ

- `InXY` <= [60] If output 0

### [47] Mask ( B )

- `Input` <= [46] DeriveNormalZ output 0

### [48] Multiply

- `A` <= [47] Mask ( B ) output 0
- `B` <= [55] Lerp(1,-1,) output 0

### [49] ConstantBiasScale

- `Input` <= [57] Add output 0

### [50] Mask ( G )

- `Input` <= [51] TexCoord[3] output 0

### [53] If

- `A` <= [50] Mask ( G ) output 0
- `B` <= [58] 1 output 0
- `AGreaterThanB` <= [52] 1 output 0
- `ALessThanB` <= [54] 0 output 0

### [55] Lerp(1,-1,)

- `Alpha` <= [53] If output 0

### [56] Lerp(0,-1,)

- `Alpha` <= [53] If output 0

### [57] Add

- `A` <= [50] Mask ( G ) output 0
- `B` <= [56] Lerp(0,-1,) output 0

### [59] Multiply

- `A` <= [49] ConstantBiasScale output 0
- `B` <= [68] Multiply(,-1) output 0

### [60] If

- `A` <= [65] Sqrt output 0
- `B` <= [61] 0.999 output 0
- `AGreaterThanB` <= [66] Multiply output 0
- `ALessThanB` <= [89] ProtectFrom0 output 0

### [62] Multiply

- `A` <= [89] ProtectFrom0 output 0
- `B` <= [89] ProtectFrom0 output 0

### [64] Divide(1,)

- `B` <= [67] Add(,1e-06) output 0

### [65] Sqrt

- `Input` <= [63] AddComponents output 0

### [66] Multiply

- `A` <= [89] ProtectFrom0 output 0
- `B` <= [64] Divide(1,) output 0

### [67] Add(,1e-06)

- `A` <= [65] Sqrt output 0

### [68] Multiply(,-1)

- `A` <= [55] Lerp(1,-1,) output 0

### [70] Divide(,255)

- `A` <= [28] Input Max Dist for Parent Piv (Scalar) output 0

### [71] Multiply

- `A` <= [70] Divide(,255) output 0
- `B` <= [73] Multiply output 0

### [72] Add

- `A` <= [71] Multiply output 0
- `B` <= [1] Multiply output 0

### [73] Multiply

- `A` <= [74] -1,-1,1 output 0
- `B` <= [75] 1-x output 0

### [75] 1-x

- `Input` <= [2] Subtract output 0

### [76] Clamp

- `Input` <= [81] Abs output 0

### [77] Multiply(,255)

- `A` <= [79] Multiply output 0

### [78] Multiply

- `A` <= [72] Add output 0
- `B` <= [76] Clamp output 0

### [79] Multiply

- `A` <= [2] Subtract output 0
- `B` <= [80] 2,2,1 output 0

### [81] Abs

- `Input` <= [77] Multiply(,255) output 0

### [83] Multiply

- `A` <= [86] Multiply output 0
- `B` <= [84] Subtract output 0

### [84] Subtract

- `A` <= [82] Vertex Color output 0
- `B` <= [85] 0.5,0.5,0 output 0

### [86] Multiply

- `A` <= [28] Input Max Dist for Parent Piv (Scalar) output 0
- `B` <= [87] 2,2,1 output 0

## Connection List

- [28] Input Max Dist for Parent Piv (Scalar) output 0 -> Multiply.A
- [79] Multiply output 0 -> Multiply.B
- [0] Vertex Color output 0 -> Subtract.A
- [3] 0.5,0.5,0 output 0 -> Subtract.B
- [83] Multiply output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [5] TexCoord[2] output 0 -> Append.A
- [8] Mask ( R ) output 0 -> Append.B
- [6] Append output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [9] TexCoord[3] output 0 -> Mask ( R ).Input
- [90] SafeNormalize output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [12] Frac output 0 -> ConstantBiasScale.Input
- [6] Append output 0 -> Frac.Input
- [7] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Child Piv Position.A
- [4] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Parent Piv Position.A
- [69] SafeNormalize output 0 -> Output Parent X Axis Vector.A
- [24] Normalize output 0 -> Output Child X-Axis Vector.A
- [18] Mask ( R ) output 0 -> Output Object Scale.A
- [32] ObjectScale output 0 -> Mask ( R ).Input
- [21] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Object  Pivot Point.A
- [25] Normalize output 0 -> Output Object Orientation.A
- [31] 0,0,0 output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [22] 0,0,1 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [10] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [23] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [27] 4096 output 0 -> Input Max Dist for Parent Piv (Scalar).Preview
- [37] If output 0 -> Output Black Mask.A
- [34] Vertex Color output 1 -> Add.A
- [34] Vertex Color output 2 -> Add.B
- [33] Add output 0 -> Add.A
- [34] Vertex Color output 3 -> Add.B
- [35] Add output 0 -> Add.A
- [34] Vertex Color output 4 -> Add.B
- [36] Add output 0 -> If.A
- [38] 0 output 0 -> If.B
- [39] 1 output 0 -> If.AGreaterThanB
- [38] 0 output 0 -> If.AEqualsB
- [39] 1 output 0 -> If.ALessThanB
- [40] Vertex Color output 4 -> ConstantBiasScale.Input
- [43] Append output 0 -> Local Space to World Space TransformVector.Input
- [44] Mask ( R G ) output 0 -> Append.A
- [48] Multiply output 0 -> Append.B
- [46] DeriveNormalZ output 0 -> Mask ( R G ).Input
- [41] ConstantBiasScale output 0 -> Append.A
- [59] Multiply output 0 -> Append.B
- [60] If output 0 -> DeriveNormalZ.InXY
- [46] DeriveNormalZ output 0 -> Mask ( B ).Input
- [47] Mask ( B ) output 0 -> Multiply.A
- [55] Lerp(1,-1,) output 0 -> Multiply.B
- [57] Add output 0 -> ConstantBiasScale.Input
- [51] TexCoord[3] output 0 -> Mask ( G ).Input
- [50] Mask ( G ) output 0 -> If.A
- [58] 1 output 0 -> If.B
- [52] 1 output 0 -> If.AGreaterThanB
- [54] 0 output 0 -> If.ALessThanB
- [53] If output 0 -> Lerp(1,-1,).Alpha
- [53] If output 0 -> Lerp(0,-1,).Alpha
- [50] Mask ( G ) output 0 -> Add.A
- [56] Lerp(0,-1,) output 0 -> Add.B
- [49] ConstantBiasScale output 0 -> Multiply.A
- [68] Multiply(,-1) output 0 -> Multiply.B
- [65] Sqrt output 0 -> If.A
- [61] 0.999 output 0 -> If.B
- [66] Multiply output 0 -> If.AGreaterThanB
- [89] ProtectFrom0 output 0 -> If.ALessThanB
- [89] ProtectFrom0 output 0 -> Multiply.A
- [89] ProtectFrom0 output 0 -> Multiply.B
- [67] Add(,1e-06) output 0 -> Divide(1,).B
- [63] AddComponents output 0 -> Sqrt.Input
- [89] ProtectFrom0 output 0 -> Multiply.A
- [64] Divide(1,) output 0 -> Multiply.B
- [65] Sqrt output 0 -> Add(,1e-06).A
- [55] Lerp(1,-1,) output 0 -> Multiply(,-1).A
- [28] Input Max Dist for Parent Piv (Scalar) output 0 -> Divide(,255).A
- [70] Divide(,255) output 0 -> Multiply.A
- [73] Multiply output 0 -> Multiply.B
- [71] Multiply output 0 -> Add.A
- [1] Multiply output 0 -> Add.B
- [74] -1,-1,1 output 0 -> Multiply.A
- [75] 1-x output 0 -> Multiply.B
- [2] Subtract output 0 -> 1-x.Input
- [81] Abs output 0 -> Clamp.Input
- [79] Multiply output 0 -> Multiply(,255).A
- [72] Add output 0 -> Multiply.A
- [76] Clamp output 0 -> Multiply.B
- [2] Subtract output 0 -> Multiply.A
- [80] 2,2,1 output 0 -> Multiply.B
- [77] Multiply(,255) output 0 -> Abs.Input
- [86] Multiply output 0 -> Multiply.A
- [84] Subtract output 0 -> Multiply.B
- [82] Vertex Color output 0 -> Subtract.A
- [85] 0.5,0.5,0 output 0 -> Subtract.B
- [28] Input Max Dist for Parent Piv (Scalar) output 0 -> Multiply.A
- [87] 2,2,1 output 0 -> Multiply.B

## Output Trace Roots

- `Child Piv Position` is driven by [7] Instance & Particle Space to Absolute World Space TransformPosition
- `Parent Piv Position` is driven by [4] Instance & Particle Space to Absolute World Space TransformPosition
- `Parent X Axis Vector` is driven by [69] SafeNormalize
- `Child X-Axis Vector` is driven by [24] Normalize
- `Object Scale` is driven by [18] Mask ( R )
- `Object  Pivot Point` is driven by [21] Instance & Particle Space to Absolute World Space TransformPosition
- `Object Orientation` is driven by [25] Normalize
- `------------------------` has no captured source.
- `Black Mask` is driven by [37] If
- `------------------------ ` has no captured source.

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
