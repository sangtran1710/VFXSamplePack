# CanopyCreator_Branches

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/CanopyCreator_Branches.CanopyCreator_Branches`
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
| 32 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 33 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 34 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 35 | expression | `MaterialExpressionConstant` | 0 |  |
| 36 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionConstant` | -1 |  |
| 39 | function_call | `MaterialExpressionMaterialFunctionCall` | TangentBasis |  |
| 40 | expression | `MaterialExpressionConstant` | 0.62 |  |
| 41 | expression | `MaterialExpressionAppendVector` | Append |  |
| 42 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 45 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 47 | function_input | `MaterialExpressionFunctionInput` | Input AdditionalNormal (Vector3) | AdditionalNormal |
| 48 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 49 | expression | `MaterialExpressionConstant3Vector` | 1,-1,1 |  |
| 50 | expression | `MaterialExpressionConstant` | 1 |  |
| 51 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 53 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 54 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 55 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 56 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 57 | function_input | `MaterialExpressionFunctionInput` | Input UVs for Thickness (Vector2) | UVs for Thickness |
| 58 | function_input | `MaterialExpressionFunctionInput` | Input DeriveNormalZ (StaticBool) | DeriveNormalZ |
| 59 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 60 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 61 | function_call | `MaterialExpressionMaterialFunctionCall` | BlendAngleCorrectedNormals |  |
| 62 | expression | `MaterialExpressionIf` | If |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply(0,1) |  |
| 64 | expression | `MaterialExpressionAdd` | Add |  |
| 65 | function_output | `MaterialExpressionFunctionOutput` | Output UVs with Parallax | UVs with Parallax |
| 66 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 67 | function_input | `MaterialExpressionFunctionInput` | Input AngleCorrectedNormals (StaticBool) | AngleCorrectedNormals |
| 68 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 69 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 70 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 71 | expression | `MaterialExpressionConstant3Vector` | 1,-1,0 |  |
| 72 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 73 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 74 | expression | `MaterialExpressionConstant2Vector` | 1,0 |  |
| 75 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 76 | expression | `MaterialExpressionDDX` | DDX |  |
| 77 | expression | `MaterialExpressionDDY` | DDY |  |
| 78 | expression | `MaterialExpressionDDX` | DDX |  |
| 79 | expression | `MaterialExpressionDDY` | DDY |  |
| 80 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 81 | expression | `MaterialExpressionDivide` | Divide |  |
| 82 | expression | `MaterialExpressionDivide` | Divide |  |
| 83 | expression | `MaterialExpressionMax` | Max |  |
| 84 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 85 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 86 | expression | `MaterialExpressionAbs` | Abs |  |
| 87 | expression | `MaterialExpressionAbs` | Abs |  |
| 88 | function_input | `MaterialExpressionFunctionInput` | Input UVs for texturing (Vector2) | UVs for texturing |
| 89 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 90 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 91 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 92 | expression | `MaterialExpressionAdd` | Add |  |
| 93 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 94 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 95 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 96 | expression | `MaterialExpressionConstant` | 0 |  |
| 97 | function_input | `MaterialExpressionFunctionInput` | Input FlattenPixelNormal (StaticBool) | FlattenPixelNormal |
| 98 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 99 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 100 | expression | `MaterialExpressionFrac` | Frac |  |
| 101 | function_input | `MaterialExpressionFunctionInput` | Input UVs For NormalShading (Vector2) | UVs For NormalShading |
| 102 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 103 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 104 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 105 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 106 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 107 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 108 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [102] Switch output 0

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

- `Input` <= [57] Input UVs for Thickness (Vector2) output 0

### [16] Normalize

- `VectorInput` <= [17] Cross output 0

### [17] Cross

- `A` <= [1] Subtract output 0
- `B` <= [19] Switch output 0

### [18] Output WorldPositionOffset

- `A` <= [5] Multiply output 0

### [19] Switch

- `A` <= [39] TangentBasis output 1
- `B` <= [39] TangentBasis output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [20] Input Expand U or V UV Channel (StaticBool)

- `Preview` <= [21] Static Bool (True) output 0

### [22] Switch

- `A` <= [13] Mask ( G ) output 0
- `B` <= [23] Mask ( R ) output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [23] Mask ( R )

- `Input` <= [57] Input UVs for Thickness (Vector2) output 0

### [24] Switch

- `A` <= [7] Mask ( R ) output 0
- `B` <= [25] Mask ( G ) output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [25] Mask ( G )

- `Input` <= [8] Add output 0

### [29] Mask ( R )

- `Input` <= [101] Input UVs For NormalShading (Vector2) output 0

### [30] ConstantBiasScale

- `Input` <= [100] Frac output 0

### [31] Append

- `A` <= [98] Switch output 0
- `B` <= [35] 0 output 0

### [32] Normalize

- `VectorInput` <= [26] CreateThirdOrthogonalVector output 0

### [33] Normalize

- `VectorInput` <= [26] CreateThirdOrthogonalVector output 1

### [34] Normalize

- `VectorInput` <= [26] CreateThirdOrthogonalVector output 2

### [36] DeriveNormalZ

- `InXY` <= [31] Append output 0

### [37] Multiply

- `A` <= [19] Switch output 0
- `B` <= [38] -1 output 0

### [41] Append

- `A` <= [31] Append output 0
- `B` <= [40] 0.62 output 0

### [42] Mask ( B )

- `Input` <= [36] DeriveNormalZ output 0

### [43] Multiply

- `A` <= [42] Mask ( B ) output 0
- `B` <= [12] Lerp output 0

### [44] Dot

- `A` <= [45] Normalize output 0
- `B` <= [33] Normalize output 0

### [45] Normalize

- `VectorInput` <= [1] Subtract output 0

### [46] Multiply

- `A` <= [19] Switch output 0
- `B` <= [56] Multiply output 0

### [48] Multiply

- `A` <= [47] Input AdditionalNormal (Vector3) output 0
- `B` <= [49] 1,-1,1 output 0

### [51] Subtract

- `A` <= [50] 1 output 0
- `B` <= [52] Multiply output 0

### [52] Multiply

- `A` <= [44] Dot output 0
- `B` <= [44] Dot output 0

### [53] Sqrt

- `Input` <= [51] Subtract output 0

### [54] Divide(1,)

- `B` <= [53] Sqrt output 0

### [55] Multiply

- `A` <= [54] Divide(1,) output 0
- `B` <= [43] Multiply output 0

### [56] Multiply

- `A` <= [44] Dot output 0
- `B` <= [55] Multiply output 0

### [57] Input UVs for Thickness (Vector2)

- `Preview` <= [10] TexCoord[0] output 0

### [58] Input DeriveNormalZ (StaticBool)

- `Preview` <= [60] Static Bool (False) output 0

### [59] Switch

- `A` <= [36] DeriveNormalZ output 0
- `B` <= [41] Append output 0
- `Value` <= [58] Input DeriveNormalZ (StaticBool) output 0

### [62] If

- `A` <= [47] Input AdditionalNormal (Vector3) output 0

### [64] Add

- `A` <= [70] Multiply output 0
- `B` <= [59] Switch output 0

### [65] Output UVs with Parallax

- `A` <= [92] Add output 0

### [66] Multiply(,-1)

- `A` <= [56] Multiply output 0

### [67] Input AngleCorrectedNormals (StaticBool)

- `Preview` <= [68] Static Bool (False) output 0

### [69] Switch

- `A` <= [61] BlendAngleCorrectedNormals output 0
- `B` <= [64] Add output 0
- `Value` <= [67] Input AngleCorrectedNormals (StaticBool) output 0

### [70] Multiply

- `A` <= [47] Input AdditionalNormal (Vector3) output 0
- `B` <= [71] 1,-1,0 output 0

### [72] Switch

- `A` <= [73] 0,1 output 0
- `B` <= [74] 1,0 output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [75] Multiply

- `A` <= [66] Multiply(,-1) output 0
- `B` <= [72] Switch output 0

### [76] DDX

- `Value` <= [89] Switch output 0

### [77] DDY

- `Value` <= [89] Switch output 0

### [78] DDX

- `Value` <= [80] Absolute World Position output 0

### [79] DDY

- `Value` <= [80] Absolute World Position output 0

### [81] Divide

- `A` <= [86] Abs output 0
- `B` <= [84] VectorLength output 0

### [82] Divide

- `A` <= [87] Abs output 0
- `B` <= [85] VectorLength output 0

### [83] Max

- `A` <= [81] Divide output 0
- `B` <= [82] Divide output 0

### [86] Abs

- `Input` <= [76] DDX output 0

### [87] Abs

- `Input` <= [77] DDY output 0

### [88] Input UVs for texturing (Vector2)

- `Preview` <= [93] TexCoord[0] output 0

### [89] Switch

- `A` <= [90] Mask ( G ) output 0
- `B` <= [91] Mask ( R ) output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [90] Mask ( G )

- `Input` <= [88] Input UVs for texturing (Vector2) output 0

### [91] Mask ( R )

- `Input` <= [88] Input UVs for texturing (Vector2) output 0

### [92] Add

- `A` <= [88] Input UVs for texturing (Vector2) output 0
- `B` <= [95] Multiply(,0.5) output 0

### [94] Multiply

- `A` <= [83] Max output 0
- `B` <= [75] Multiply output 0

### [95] Multiply(,0.5)

- `A` <= [94] Multiply output 0

### [97] Input FlattenPixelNormal (StaticBool)

- `Preview` <= [99] Static Bool (False) output 0

### [98] Switch

- `A` <= [96] 0 output 0
- `B` <= [30] ConstantBiasScale output 0
- `Value` <= [97] Input FlattenPixelNormal (StaticBool) output 0

### [100] Frac

- `Input` <= [29] Mask ( R ) output 0

### [101] Input UVs For NormalShading (Vector2)

- `Preview` <= [28] TexCoord[0] output 0

### [102] Switch

- `A` <= [27] Transform3x3Matrix output 0
- `B` <= [103] Transform3x3Matrix output 0
- `Value` <= [20] Input Expand U or V UV Channel (StaticBool) output 0

### [105] Normalize

- `VectorInput` <= [104] CreateThirdOrthogonalVector output 0

### [106] Normalize

- `VectorInput` <= [104] CreateThirdOrthogonalVector output 1

### [107] Normalize

- `VectorInput` <= [104] CreateThirdOrthogonalVector output 2

### [108] Multiply(,-1)

- `A` <= [17] Cross output 0

## Connection List

- [102] Switch output 0 -> Output Normal.A
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
- [57] Input UVs for Thickness (Vector2) output 0 -> Mask ( G ).Input
- [17] Cross output 0 -> Normalize.VectorInput
- [1] Subtract output 0 -> Cross.A
- [19] Switch output 0 -> Cross.B
- [5] Multiply output 0 -> Output WorldPositionOffset.A
- [39] TangentBasis output 1 -> Switch.A
- [39] TangentBasis output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [21] Static Bool (True) output 0 -> Input Expand U or V UV Channel (StaticBool).Preview
- [13] Mask ( G ) output 0 -> Switch.A
- [23] Mask ( R ) output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [57] Input UVs for Thickness (Vector2) output 0 -> Mask ( R ).Input
- [7] Mask ( R ) output 0 -> Switch.A
- [25] Mask ( G ) output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [8] Add output 0 -> Mask ( G ).Input
- [101] Input UVs For NormalShading (Vector2) output 0 -> Mask ( R ).Input
- [100] Frac output 0 -> ConstantBiasScale.Input
- [98] Switch output 0 -> Append.A
- [35] 0 output 0 -> Append.B
- [26] CreateThirdOrthogonalVector output 0 -> Normalize.VectorInput
- [26] CreateThirdOrthogonalVector output 1 -> Normalize.VectorInput
- [26] CreateThirdOrthogonalVector output 2 -> Normalize.VectorInput
- [31] Append output 0 -> DeriveNormalZ.InXY
- [19] Switch output 0 -> Multiply.A
- [38] -1 output 0 -> Multiply.B
- [31] Append output 0 -> Append.A
- [40] 0.62 output 0 -> Append.B
- [36] DeriveNormalZ output 0 -> Mask ( B ).Input
- [42] Mask ( B ) output 0 -> Multiply.A
- [12] Lerp output 0 -> Multiply.B
- [45] Normalize output 0 -> Dot.A
- [33] Normalize output 0 -> Dot.B
- [1] Subtract output 0 -> Normalize.VectorInput
- [19] Switch output 0 -> Multiply.A
- [56] Multiply output 0 -> Multiply.B
- [47] Input AdditionalNormal (Vector3) output 0 -> Multiply.A
- [49] 1,-1,1 output 0 -> Multiply.B
- [50] 1 output 0 -> Subtract.A
- [52] Multiply output 0 -> Subtract.B
- [44] Dot output 0 -> Multiply.A
- [44] Dot output 0 -> Multiply.B
- [51] Subtract output 0 -> Sqrt.Input
- [53] Sqrt output 0 -> Divide(1,).B
- [54] Divide(1,) output 0 -> Multiply.A
- [43] Multiply output 0 -> Multiply.B
- [44] Dot output 0 -> Multiply.A
- [55] Multiply output 0 -> Multiply.B
- [10] TexCoord[0] output 0 -> Input UVs for Thickness (Vector2).Preview
- [60] Static Bool (False) output 0 -> Input DeriveNormalZ (StaticBool).Preview
- [36] DeriveNormalZ output 0 -> Switch.A
- [41] Append output 0 -> Switch.B
- [58] Input DeriveNormalZ (StaticBool) output 0 -> Switch.Value
- [47] Input AdditionalNormal (Vector3) output 0 -> If.A
- [70] Multiply output 0 -> Add.A
- [59] Switch output 0 -> Add.B
- [92] Add output 0 -> Output UVs with Parallax.A
- [56] Multiply output 0 -> Multiply(,-1).A
- [68] Static Bool (False) output 0 -> Input AngleCorrectedNormals (StaticBool).Preview
- [61] BlendAngleCorrectedNormals output 0 -> Switch.A
- [64] Add output 0 -> Switch.B
- [67] Input AngleCorrectedNormals (StaticBool) output 0 -> Switch.Value
- [47] Input AdditionalNormal (Vector3) output 0 -> Multiply.A
- [71] 1,-1,0 output 0 -> Multiply.B
- [73] 0,1 output 0 -> Switch.A
- [74] 1,0 output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [66] Multiply(,-1) output 0 -> Multiply.A
- [72] Switch output 0 -> Multiply.B
- [89] Switch output 0 -> DDX.Value
- [89] Switch output 0 -> DDY.Value
- [80] Absolute World Position output 0 -> DDX.Value
- [80] Absolute World Position output 0 -> DDY.Value
- [86] Abs output 0 -> Divide.A
- [84] VectorLength output 0 -> Divide.B
- [87] Abs output 0 -> Divide.A
- [85] VectorLength output 0 -> Divide.B
- [81] Divide output 0 -> Max.A
- [82] Divide output 0 -> Max.B
- [76] DDX output 0 -> Abs.Input
- [77] DDY output 0 -> Abs.Input
- [93] TexCoord[0] output 0 -> Input UVs for texturing (Vector2).Preview
- [90] Mask ( G ) output 0 -> Switch.A
- [91] Mask ( R ) output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [88] Input UVs for texturing (Vector2) output 0 -> Mask ( G ).Input
- [88] Input UVs for texturing (Vector2) output 0 -> Mask ( R ).Input
- [88] Input UVs for texturing (Vector2) output 0 -> Add.A
- [95] Multiply(,0.5) output 0 -> Add.B
- [83] Max output 0 -> Multiply.A
- [75] Multiply output 0 -> Multiply.B
- [94] Multiply output 0 -> Multiply(,0.5).A
- [99] Static Bool (False) output 0 -> Input FlattenPixelNormal (StaticBool).Preview
- [96] 0 output 0 -> Switch.A
- [30] ConstantBiasScale output 0 -> Switch.B
- [97] Input FlattenPixelNormal (StaticBool) output 0 -> Switch.Value
- [29] Mask ( R ) output 0 -> Frac.Input
- [28] TexCoord[0] output 0 -> Input UVs For NormalShading (Vector2).Preview
- [27] Transform3x3Matrix output 0 -> Switch.A
- [103] Transform3x3Matrix output 0 -> Switch.B
- [20] Input Expand U or V UV Channel (StaticBool) output 0 -> Switch.Value
- [104] CreateThirdOrthogonalVector output 0 -> Normalize.VectorInput
- [104] CreateThirdOrthogonalVector output 1 -> Normalize.VectorInput
- [104] CreateThirdOrthogonalVector output 2 -> Normalize.VectorInput
- [17] Cross output 0 -> Multiply(,-1).A

## Output Trace Roots

- `Normal` is driven by [102] Switch
- `WorldPositionOffset` is driven by [5] Multiply
- `UVs with Parallax` is driven by [92] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


