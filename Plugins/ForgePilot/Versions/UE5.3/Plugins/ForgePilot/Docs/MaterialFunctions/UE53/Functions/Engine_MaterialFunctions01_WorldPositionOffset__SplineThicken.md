# SplineThicken

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/SplineThicken.SplineThicken`
- Category: Engine_MaterialFunctions01/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function  widens strips of polies along the U of their UVs.  These strips shouuld be extremely thin and imported with remove degenerate triangles disabled. Useful for Hair, Grass, Cables etc.

## Inputs

- `WorldPosition` [FunctionInput_Vector3]
- `UVs for Projection` [FunctionInput_Vector2] - The texture coordinate to use in the expansion of the spline. This defaults to texture coordinate 0.
- `WidthBase` [FunctionInput_Scalar]
- `WidthTip` [FunctionInput_Scalar]
- `Expand U or V UV Channel` [FunctionInput_StaticBool] - Defaults to True (expands U)
- `AdditionalNormal` [FunctionInput_Vector3] - Adds a texture normal to the normalmap transform
- `UVs for Thickness` [FunctionInput_Vector2] - Texture coordinate to use for Thickness mapping. If projecting from U, it will use the Y component of the specified UV index for the thickness blending. 
- `DeriveNormalZ` [FunctionInput_StaticBool] - Uses DeriveNormalZ to make a perfectly round normal map. Sets the Z height to 0.62 if False (that's the average of half a sphere). If using CustomUVs, this requires a row of verts to be in the center of the mesh, otherwise it will solve z=0 for the whole surface.
- `AngleCorrectedNormals` [FunctionInput_StaticBool] - Uses DeriveNormalZ to make a perfectly round normal map. Requires a center tesselation vertex otherwise the vale of Z will be 0 for the entire face, causing harsh lighting. When false, Z height is set to 0.62 which approximates the average thickness of a hemisphere.
- `UVs for texturing` [FunctionInput_Vector2] - The UV channel used for texturing. This must be the same UV channel you want to have the 3d-parallax correction.
- `FlattenPixelNormal` [FunctionInput_StaticBool]
- `UVs For NormalShading` [FunctionInput_Vector2]

## Outputs

- `Normal` - Add this result to your other WPO calculations
- `WorldPositionOffset` - Add this result to your other WPO calculations
- `UVs with Parallax`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 1 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 3 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 4 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input UVs for Projection (Vector2) | UVs for Projection |
| 10 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 11 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input WidthBase (Scalar) | WidthBase |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input WidthTip (Scalar) | WidthTip |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output WorldPositionOffset | WorldPositionOffset |
| 19 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Expand U or V UV Channel (StaticBool) | Expand U or V UV Channel |
| 21 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 22 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 24 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 26 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 27 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 28 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 29 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 30 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 31 | expression | `MaterialExpressionAppendVector` | Append |  |
| 32 | expression | `MaterialExpressionConstant` | 0 |  |
| 33 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionConstant` | -1 |  |
| 36 | function_call | `MaterialExpressionMaterialFunctionCall` | TangentBasis |  |
| 37 | expression | `MaterialExpressionConstant` | 0.62 |  |
| 38 | expression | `MaterialExpressionAppendVector` | Append |  |
| 39 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 42 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input AdditionalNormal (Vector3) | AdditionalNormal |
| 45 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 46 | expression | `MaterialExpressionConstant3Vector` | 1,-1,1 |  |
| 47 | expression | `MaterialExpressionConstant` | 1 |  |
| 48 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 49 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 50 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 51 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 54 | function_input | `MaterialExpressionFunctionInput` | Input UVs for Thickness (Vector2) | UVs for Thickness |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input DeriveNormalZ (StaticBool) | DeriveNormalZ |
| 56 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 57 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 58 | function_call | `MaterialExpressionMaterialFunctionCall` | BlendAngleCorrectedNormals |  |
| 59 | expression | `MaterialExpressionIf` | If |  |
| 60 | expression | `MaterialExpressionMultiply` | Multiply(0,1) |  |
| 61 | expression | `MaterialExpressionAdd` | Add |  |
| 62 | function_output | `MaterialExpressionFunctionOutput` | Output UVs with Parallax | UVs with Parallax |
| 63 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 64 | function_input | `MaterialExpressionFunctionInput` | Input AngleCorrectedNormals (StaticBool) | AngleCorrectedNormals |
| 65 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 66 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 67 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 68 | expression | `MaterialExpressionConstant3Vector` | 1,-1,0 |  |
| 69 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 70 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 71 | expression | `MaterialExpressionConstant2Vector` | 1,0 |  |
| 72 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 73 | expression | `MaterialExpressionDDX` | DDX |  |
| 74 | expression | `MaterialExpressionDDY` | DDY |  |
| 75 | expression | `MaterialExpressionDDX` | DDX |  |
| 76 | expression | `MaterialExpressionDDY` | DDY |  |
| 77 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 78 | expression | `MaterialExpressionDivide` | Divide |  |
| 79 | expression | `MaterialExpressionDivide` | Divide |  |
| 80 | expression | `MaterialExpressionMax` | Max |  |
| 81 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 82 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 83 | expression | `MaterialExpressionAbs` | Abs |  |
| 84 | expression | `MaterialExpressionAbs` | Abs |  |
| 85 | function_input | `MaterialExpressionFunctionInput` | Input UVs for texturing (Vector2) | UVs for texturing |
| 86 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 87 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 88 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 89 | expression | `MaterialExpressionAdd` | Add |  |
| 90 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 91 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 92 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 93 | expression | `MaterialExpressionConstant` | 0 |  |
| 94 | function_input | `MaterialExpressionFunctionInput` | Input FlattenPixelNormal (StaticBool) | FlattenPixelNormal |
| 95 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 96 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 97 | expression | `MaterialExpressionFrac` | Frac |  |
| 98 | function_input | `MaterialExpressionFunctionInput` | Input UVs For NormalShading (Vector2) | UVs For NormalShading |
| 99 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 100 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 101 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 102 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 103 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 104 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 105 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 106 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 107 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 108 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [99] Switch output 0

### [1] Subtract

- `A` <= [4] Camera Position output 0
- `B` <= [2] Input WorldPosition (Vector3) output 0

### [2] Input WorldPosition (Vector3)

- `Preview` <= [3] Absolute World Position output 0

### [5] Multiply

- `A` <= [16] Normalize output 0
- `B` <= [6] Multiply output 0

### [6] Multiply

- `A` <= [12] Lerp output 0
- `B` <= [24] Switch output 0

### [7] Mask ( R )

- `Input` <= [8] Add output 0

### [8] Add

- `A` <= [11] -0.5 output 0
- `B` <= [9] Input UVs for Projection (Vector2) output 0

### [9] Input UVs for Projection (Vector2)

- `Preview` <= [10] TexCoord[0] output 0

### [12] Lerp

- `A` <= [15] Input WidthTip (Scalar) output 0
- `B` <= [14] Input WidthBase (Scalar) output 0
- `Alpha` <= [22] Switch output 0

### [13] Mask ( G )

- `Input` <= [54] Input UVs for Thickness (Vector2) output 0

### [16] Normalize

- `VectorInput` <= [17] Cross output 0

### [17] Cross

- `A` <= [1] Subtract output 0
- `B` <= [19] Switch output 0

### [18] Output WorldPositionOffset

- `A` <= [5] Multiply output 0

### [19] Switch

- `A` <= [36] TangentBasis output 1
- `B` <= [36] TangentBasis output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [20] Input Expand U or V UV Channel (StaticBool)

- `Preview` <= [21] Static Bool (True) output 0

### [22] Switch

- `A` <= [13] Mask ( G ) output 0
- `B` <= [23] Mask ( R ) output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [23] Mask ( R )

- `Input` <= [54] Input UVs for Thickness (Vector2) output 0

### [24] Switch

- `A` <= [7] Mask ( R ) output 0
- `B` <= [25] Mask ( G ) output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [25] Mask ( G )

- `Input` <= [8] Add output 0

### [29] Mask ( R )

- `Input` <= [98] Input UVs For NormalShading (Vector2) output 0

### [30] ConstantBiasScale

- `Input` <= [97] Frac output 0

### [31] Append

- `A` <= [95] Switch output 0
- `B` <= [32] 0 output 0

### [33] DeriveNormalZ

- `InXY` <= [31] Append output 0

### [34] Multiply

- `A` <= [19] Switch output 0
- `B` <= [35] -1 output 0

### [38] Append

- `A` <= [31] Append output 0
- `B` <= [37] 0.62 output 0

### [39] Mask ( B )

- `Input` <= [33] DeriveNormalZ output 0

### [40] Multiply

- `A` <= [39] Mask ( B ) output 0
- `B` <= [12] Lerp output 0

### [41] Dot

- `A` <= [42] Normalize output 0
- `B` <= [26] CreateThirdOrthogonalVector output 1

### [42] Normalize

- `VectorInput` <= [1] Subtract output 0

### [43] Multiply

- `A` <= [19] Switch output 0
- `B` <= [53] Multiply output 0

### [45] Multiply

- `A` <= [44] Input AdditionalNormal (Vector3) output 0
- `B` <= [46] 1,-1,1 output 0

### [48] Subtract

- `A` <= [47] 1 output 0
- `B` <= [49] Multiply output 0

### [49] Multiply

- `A` <= [41] Dot output 0
- `B` <= [41] Dot output 0

### [50] Sqrt

- `Input` <= [48] Subtract output 0

### [51] Divide(1,)

- `B` <= [50] Sqrt output 0

### [52] Multiply

- `A` <= [51] Divide(1,) output 0
- `B` <= [40] Multiply output 0

### [53] Multiply

- `A` <= [41] Dot output 0
- `B` <= [52] Multiply output 0

### [54] Input UVs for Thickness (Vector2)

- `Preview` <= [10] TexCoord[0] output 0

### [55] Input DeriveNormalZ (StaticBool)

- `Preview` <= [57] Static Bool (False) output 0

### [56] Switch

- `A` <= [33] DeriveNormalZ output 0
- `B` <= [38] Append output 0
- `Value` <= [55] Input DeriveNormalZ (StaticBool) output 0

### [59] If

- `A` <= [44] Input AdditionalNormal (Vector3) output 0

### [61] Add

- `A` <= [67] Multiply output 0
- `B` <= [56] Switch output 0

### [62] Output UVs with Parallax

- `A` <= [89] Add output 0

### [63] Multiply(,-1)

- `A` <= [53] Multiply output 0

### [64] Input AngleCorrectedNormals (StaticBool)

- `Preview` <= [65] Static Bool (False) output 0

### [66] Switch

- `A` <= [58] BlendAngleCorrectedNormals output 0
- `B` <= [61] Add output 0
- `Value` <= [64] Input AngleCorrectedNormals (StaticBool) output 0

### [67] Multiply

- `A` <= [44] Input AdditionalNormal (Vector3) output 0
- `B` <= [68] 1,-1,0 output 0

### [69] Switch

- `A` <= [70] 0,1 output 0
- `B` <= [71] 1,0 output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [72] Multiply

- `A` <= [63] Multiply(,-1) output 0
- `B` <= [69] Switch output 0

### [73] DDX

- `Value` <= [86] Switch output 0

### [74] DDY

- `Value` <= [86] Switch output 0

### [75] DDX

- `Value` <= [77] Absolute World Position output 0

### [76] DDY

- `Value` <= [77] Absolute World Position output 0

### [78] Divide

- `A` <= [83] Abs output 0
- `B` <= [81] VectorLength output 0

### [79] Divide

- `A` <= [84] Abs output 0
- `B` <= [82] VectorLength output 0

### [80] Max

- `A` <= [78] Divide output 0
- `B` <= [79] Divide output 0

### [83] Abs

- `Input` <= [73] DDX output 0

### [84] Abs

- `Input` <= [74] DDY output 0

### [85] Input UVs for texturing (Vector2)

- `Preview` <= [90] TexCoord[0] output 0

### [86] Switch

- `A` <= [87] Mask ( G ) output 0
- `B` <= [88] Mask ( R ) output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [87] Mask ( G )

- `Input` <= [85] Input UVs for texturing (Vector2) output 0

### [88] Mask ( R )

- `Input` <= [85] Input UVs for texturing (Vector2) output 0

### [89] Add

- `A` <= [85] Input UVs for texturing (Vector2) output 0
- `B` <= [92] Multiply(,0.5) output 0

### [91] Multiply

- `A` <= [80] Max output 0
- `B` <= [72] Multiply output 0

### [92] Multiply(,0.5)

- `A` <= [91] Multiply output 0

### [94] Input FlattenPixelNormal (StaticBool)

- `Preview` <= [96] Static Bool (False) output 0

### [95] Switch

- `A` <= [93] 0 output 0
- `B` <= [30] ConstantBiasScale output 0
- `Value` <= [94] Input FlattenPixelNormal (StaticBool) output 0

### [97] Frac

- `Input` <= [29] Mask ( R ) output 0

### [98] Input UVs For NormalShading (Vector2)

- `Preview` <= [28] TexCoord[0] output 0

### [99] Switch

- `A` <= [27] Transform3x3Matrix output 0
- `B` <= [100] Transform3x3Matrix output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [102] Normalize

- `VectorInput` <= [101] CreateThirdOrthogonalVector output 0

### [103] Normalize

- `VectorInput` <= [101] CreateThirdOrthogonalVector output 1

### [104] Normalize

- `VectorInput` <= [101] CreateThirdOrthogonalVector output 2

### [105] Multiply(,-1)

- `A` <= [17] Cross output 0

### [106] Normalize

- `VectorInput` <= [26] CreateThirdOrthogonalVector output 0

### [107] Normalize

- `VectorInput` <= [26] CreateThirdOrthogonalVector output 1

### [108] Normalize

- `VectorInput` <= [26] CreateThirdOrthogonalVector output 2

## Connection List

- [99] Switch output 0 -> Output Normal.A
- [4] Camera Position output 0 -> Subtract.A
- [2] Input WorldPosition (Vector3) output 0 -> Subtract.B
- [3] Absolute World Position output 0 -> Input WorldPosition (Vector3).Preview
- [16] Normalize output 0 -> Multiply.A
- [6] Multiply output 0 -> Multiply.B
- [12] Lerp output 0 -> Multiply.A
- [24] Switch output 0 -> Multiply.B
- [8] Add output 0 -> Mask ( R ).Input
- [11] -0.5 output 0 -> Add.A
- [9] Input UVs for Projection (Vector2) output 0 -> Add.B
- [10] TexCoord[0] output 0 -> Input UVs for Projection (Vector2).Preview
- [15] Input WidthTip (Scalar) output 0 -> Lerp.A
- [14] Input WidthBase (Scalar) output 0 -> Lerp.B
- [22] Switch output 0 -> Lerp.Alpha
- [54] Input UVs for Thickness (Vector2) output 0 -> Mask ( G ).Input
- [17] Cross output 0 -> Normalize.VectorInput
- [1] Subtract output 0 -> Cross.A
- [19] Switch output 0 -> Cross.B
- [5] Multiply output 0 -> Output WorldPositionOffset.A
- [36] TangentBasis output 1 -> Switch.A
- [36] TangentBasis output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [21] Static Bool (True) output 0 -> Input Expand U or V UV Channel (StaticBool).Preview
- [13] Mask ( G ) output 0 -> Switch.A
- [23] Mask ( R ) output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [54] Input UVs for Thickness (Vector2) output 0 -> Mask ( R ).Input
- [7] Mask ( R ) output 0 -> Switch.A
- [25] Mask ( G ) output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [8] Add output 0 -> Mask ( G ).Input
- [98] Input UVs For NormalShading (Vector2) output 0 -> Mask ( R ).Input
- [97] Frac output 0 -> ConstantBiasScale.Input
- [95] Switch output 0 -> Append.A
- [32] 0 output 0 -> Append.B
- [31] Append output 0 -> DeriveNormalZ.InXY
- [19] Switch output 0 -> Multiply.A
- [35] -1 output 0 -> Multiply.B
- [31] Append output 0 -> Append.A
- [37] 0.62 output 0 -> Append.B
- [33] DeriveNormalZ output 0 -> Mask ( B ).Input
- [39] Mask ( B ) output 0 -> Multiply.A
- [12] Lerp output 0 -> Multiply.B
- [42] Normalize output 0 -> Dot.A
- [26] CreateThirdOrthogonalVector output 1 -> Dot.B
- [1] Subtract output 0 -> Normalize.VectorInput
- [19] Switch output 0 -> Multiply.A
- [53] Multiply output 0 -> Multiply.B
- [44] Input AdditionalNormal (Vector3) output 0 -> Multiply.A
- [46] 1,-1,1 output 0 -> Multiply.B
- [47] 1 output 0 -> Subtract.A
- [49] Multiply output 0 -> Subtract.B
- [41] Dot output 0 -> Multiply.A
- [41] Dot output 0 -> Multiply.B
- [48] Subtract output 0 -> Sqrt.Input
- [50] Sqrt output 0 -> Divide(1,).B
- [51] Divide(1,) output 0 -> Multiply.A
- [40] Multiply output 0 -> Multiply.B
- [41] Dot output 0 -> Multiply.A
- [52] Multiply output 0 -> Multiply.B
- [10] TexCoord[0] output 0 -> Input UVs for Thickness (Vector2).Preview
- [57] Static Bool (False) output 0 -> Input DeriveNormalZ (StaticBool).Preview
- [33] DeriveNormalZ output 0 -> Switch.A
- [38] Append output 0 -> Switch.B
- [55] Input DeriveNormalZ (StaticBool) output 0 -> Switch.Value
- [44] Input AdditionalNormal (Vector3) output 0 -> If.A
- [67] Multiply output 0 -> Add.A
- [56] Switch output 0 -> Add.B
- [89] Add output 0 -> Output UVs with Parallax.A
- [53] Multiply output 0 -> Multiply(,-1).A
- [65] Static Bool (False) output 0 -> Input AngleCorrectedNormals (StaticBool).Preview
- [58] BlendAngleCorrectedNormals output 0 -> Switch.A
- [61] Add output 0 -> Switch.B
- [64] Input AngleCorrectedNormals (StaticBool) output 0 -> Switch.Value
- [44] Input AdditionalNormal (Vector3) output 0 -> Multiply.A
- [68] 1,-1,0 output 0 -> Multiply.B
- [70] 0,1 output 0 -> Switch.A
- [71] 1,0 output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [63] Multiply(,-1) output 0 -> Multiply.A
- [69] Switch output 0 -> Multiply.B
- [86] Switch output 0 -> DDX.Value
- [86] Switch output 0 -> DDY.Value
- [77] Absolute World Position output 0 -> DDX.Value
- [77] Absolute World Position output 0 -> DDY.Value
- [83] Abs output 0 -> Divide.A
- [81] VectorLength output 0 -> Divide.B
- [84] Abs output 0 -> Divide.A
- [82] VectorLength output 0 -> Divide.B
- [78] Divide output 0 -> Max.A
- [79] Divide output 0 -> Max.B
- [73] DDX output 0 -> Abs.Input
- [74] DDY output 0 -> Abs.Input
- [90] TexCoord[0] output 0 -> Input UVs for texturing (Vector2).Preview
- [87] Mask ( G ) output 0 -> Switch.A
- [88] Mask ( R ) output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [85] Input UVs for texturing (Vector2) output 0 -> Mask ( G ).Input
- [85] Input UVs for texturing (Vector2) output 0 -> Mask ( R ).Input
- [85] Input UVs for texturing (Vector2) output 0 -> Add.A
- [92] Multiply(,0.5) output 0 -> Add.B
- [80] Max output 0 -> Multiply.A
- [72] Multiply output 0 -> Multiply.B
- [91] Multiply output 0 -> Multiply(,0.5).A
- [96] Static Bool (False) output 0 -> Input FlattenPixelNormal (StaticBool).Preview
- [93] 0 output 0 -> Switch.A
- [30] ConstantBiasScale output 0 -> Switch.B
- [94] Input FlattenPixelNormal (StaticBool) output 0 -> Switch.Value
- [29] Mask ( R ) output 0 -> Frac.Input
- [28] TexCoord[0] output 0 -> Input UVs For NormalShading (Vector2).Preview
- [27] Transform3x3Matrix output 0 -> Switch.A
- [100] Transform3x3Matrix output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [101] CreateThirdOrthogonalVector output 0 -> Normalize.VectorInput
- [101] CreateThirdOrthogonalVector output 1 -> Normalize.VectorInput
- [101] CreateThirdOrthogonalVector output 2 -> Normalize.VectorInput
- [17] Cross output 0 -> Multiply(,-1).A
- [26] CreateThirdOrthogonalVector output 0 -> Normalize.VectorInput
- [26] CreateThirdOrthogonalVector output 1 -> Normalize.VectorInput
- [26] CreateThirdOrthogonalVector output 2 -> Normalize.VectorInput

## Output Trace Roots

- `Normal` is driven by [99] Switch
- `WorldPositionOffset` is driven by [5] Multiply
- `UVs with Parallax` is driven by [89] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
