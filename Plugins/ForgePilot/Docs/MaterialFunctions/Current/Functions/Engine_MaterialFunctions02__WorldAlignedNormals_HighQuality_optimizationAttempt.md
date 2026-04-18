# WorldAlignedNormals_HighQuality_optimizationAttempt

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldAlignedNormals_HighQuality_optimizationAttempt.WorldAlignedNormals_HighQuality_optimizationAttempt`
- Category: Engine_MaterialFunctions02
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Applies normal maps to objects and correctly orients them. See WorldAlignedTexture_Complex for a more thorough implementation.  This version of world aligned normals is ~20 instructions more. The Normals are in worldspace by default. Transform them from world to tangent. 

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Negative World Position Divisor` [FunctionInput_Vector3] - All inputs need to be negative. For example: (-512,-256,-1024)
- `World Position` [FunctionInput_Vector3]
- `X lerp alpha` [FunctionInput_Scalar]
- `Z lerp Alpha` [FunctionInput_Scalar]
- `WorldNormal` [FunctionInput_Vector3] - All inputs need to be negative. For example: (-512,-256,-1024)
- `Optimize (See Comment)` [FunctionInput_StaticBool] - To use this feature the UV outputs from this node must be connected to the custom uv material pins. This will move the most expensive operations out of the pixel shader and into the vertex shader which should be more cost effective in most cases. 

## Outputs

- `World Space Normals XYZ`
- `Z Projection World Space Normals`
- `XY Projection World Space Normals`
- `XY Flat Top Projection World Space Normals`
- `UV3` - If the optimized version is being used this output must be plugged into custom uv 3.
- `UV4` - If the optimized version is being used this output must be plugged into custom uv 4.
- `UV2` - If the optimized version is being used this output must be plugged into custom uv 2.
- `UV6` - If the optimized version is being used this output must be plugged into custom uv 6.
- `UV7` - If the optimized version is being used this output must be plugged into custom uv 7.
- `UV5` - If the optimized version is being used this output must be plugged into custom uv 5.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normals XYZ | World Space Normals XYZ |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp(-1,1,) |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionCeil` | Ceil |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 10 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 12 | expression | `MaterialExpressionConstant` | -1 |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 14 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 15 | expression | `MaterialExpressionAbs` | Abs |  |
| 16 | expression | `MaterialExpressionConstant` | 1 |  |
| 17 | expression | `MaterialExpressionConstant` | 2 |  |
| 18 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 21 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 22 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 23 | expression | `MaterialExpressionDivide` | Divide |  |
| 24 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,-1,) |  |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 26 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 27 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionCeil` | Ceil |  |
| 30 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 31 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 32 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 33 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 34 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 35 | expression | `MaterialExpressionConstant` | -1 |  |
| 36 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 37 | expression | `MaterialExpressionConstant` | 1 |  |
| 38 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 39 | expression | `MaterialExpressionLinearInterpolate` | Lerp(-1,1,) |  |
| 40 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 42 | expression | `MaterialExpressionCeil` | Ceil |  |
| 43 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 44 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 45 | expression | `MaterialExpressionConstant` | 1 |  |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 47 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input Negative World Position Divisor (Vector3) | Negative World Position Divisor |
| 49 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 50 | expression | `MaterialExpressionConstant` | -1 |  |
| 51 | function_input | `MaterialExpressionFunctionInput` | Input X lerp alpha (Scalar) | X lerp alpha |
| 52 | function_input | `MaterialExpressionFunctionInput` | Input Z lerp Alpha (Scalar) | Z lerp Alpha |
| 53 | function_output | `MaterialExpressionFunctionOutput` | Output Z Projection World Space Normals | Z Projection World Space Normals |
| 54 | function_output | `MaterialExpressionFunctionOutput` | Output XY Projection World Space Normals | XY Projection World Space Normals |
| 55 | function_output | `MaterialExpressionFunctionOutput` | Output XY Flat Top Projection World Space Normals | XY Flat Top Projection World Space Normals |
| 56 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 57 | function_input | `MaterialExpressionFunctionInput` | Input WorldNormal (Vector3) | WorldNormal |
| 58 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 59 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 60 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 61 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 62 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 63 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 64 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 65 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 66 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[4] |  |
| 67 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[5] |  |
| 68 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 69 | expression | `MaterialExpressionAppendVector` | Append |  |
| 70 | expression | `MaterialExpressionAppendVector` | Append |  |
| 71 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[6] |  |
| 72 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 73 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[7] |  |
| 74 | expression | `MaterialExpressionAppendVector` | Append |  |
| 75 | expression | `MaterialExpressionAppendVector` | Append |  |
| 76 | function_input | `MaterialExpressionFunctionInput` | Input Optimize (See Comment) (StaticBool) | Optimize (See Comment) |
| 77 | function_output | `MaterialExpressionFunctionOutput` | Output UV3 | UV3 |
| 78 | function_output | `MaterialExpressionFunctionOutput` | Output UV4 | UV4 |
| 79 | function_output | `MaterialExpressionFunctionOutput` | Output UV2 | UV2 |
| 80 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 81 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 82 | expression | `MaterialExpressionAbs` | Abs |  |
| 83 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 84 | expression | `MaterialExpressionAppendVector` | Append |  |
| 85 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 86 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 87 | expression | `MaterialExpressionConstant` | 0 |  |
| 88 | expression | `MaterialExpressionIf` | If |  |
| 89 | expression | `MaterialExpressionConstant` | 1 |  |
| 90 | expression | `MaterialExpressionConstant` | -1 |  |
| 91 | expression | `MaterialExpressionConstant` | 0 |  |
| 92 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 93 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 94 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 95 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 96 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 97 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 98 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 99 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 100 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 101 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 102 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 103 | expression | `MaterialExpressionAppendVector` | Append |  |
| 104 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 105 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 106 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 107 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 108 | function_output | `MaterialExpressionFunctionOutput` | Output UV6 | UV6 |
| 109 | function_output | `MaterialExpressionFunctionOutput` | Output UV7 | UV7 |
| 110 | function_output | `MaterialExpressionFunctionOutput` | Output UV5 | UV5 |
| 111 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 112 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 113 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 114 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 115 | expression | `MaterialExpressionAppendVector` | Append |  |
| 116 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 117 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 118 | expression | `MaterialExpressionStaticSwitch` | Switch |  |

## Exact Input Wiring

### [0] Output World Space Normals XYZ

- `A` <= [18] Lerp output 0

### [1] Texture Sample

- `Coordinates` <= [20] Mask ( G B ) output 0
- `TextureObject` <= [46] Input Texture (Texture2D) output 0

### [2] Lerp(-1,1,)

- `Alpha` <= [6] Ceil output 0

### [3] Multiply

- `A` <= [1] Texture Sample output 0
- `B` <= [9] MakeFloat3 output 0

### [4] Lerp

- `A` <= [28] Multiply output 0
- `B` <= [3] Multiply output 0
- `Alpha` <= [51] Input X lerp alpha (Scalar) output 0

### [5] Dot

- `A` <= [57] Input WorldNormal (Vector3) output 0
- `B` <= [10] 1,0,0 output 0

### [6] Ceil

- `Input` <= [5] Dot output 0

### [15] Abs

- `Input` <= [57] Input WorldNormal (Vector3) output 0

### [18] Lerp

- `A` <= [59] Switch output 0
- `B` <= [118] Switch output 0
- `Alpha` <= [52] Input Z lerp Alpha (Scalar) output 0

### [20] Mask ( G B )

- `Input` <= [23] Divide output 0

### [21] Texture Sample

- `Coordinates` <= [25] Mask ( R B ) output 0
- `TextureObject` <= [46] Input Texture (Texture2D) output 0

### [23] Divide

- `A` <= [49] Input World Position (Vector3) output 0
- `B` <= [48] Input Negative World Position Divisor (Vector3) output 0

### [24] Lerp(1,-1,)

- `Alpha` <= [29] Ceil output 0

### [25] Mask ( R B )

- `Input` <= [23] Divide output 0

### [26] Dot

- `A` <= [57] Input WorldNormal (Vector3) output 0
- `B` <= [34] 0,1,0 output 0

### [28] Multiply

- `A` <= [21] Texture Sample output 0
- `B` <= [33] MakeFloat3 output 0

### [29] Ceil

- `Input` <= [26] Dot output 0

### [30] Mask ( R G )

- `Input` <= [23] Divide output 0

### [38] Texture Sample

- `Coordinates` <= [30] Mask ( R G ) output 0
- `TextureObject` <= [46] Input Texture (Texture2D) output 0

### [39] Lerp(-1,1,)

- `Alpha` <= [42] Ceil output 0

### [40] Dot

- `A` <= [57] Input WorldNormal (Vector3) output 0
- `B` <= [44] 0,0,1 output 0

### [41] Multiply

- `A` <= [38] Texture Sample output 0
- `B` <= [43] MakeFloat3 output 0

### [42] Ceil

- `Input` <= [40] Dot output 0

### [46] Input Texture (Texture2D)

- `Preview` <= [47] Texture Object output 0

### [49] Input World Position (Vector3)

- `Preview` <= [22] Absolute World Position (Excluding Material Offsets) output 0

### [51] Input X lerp alpha (Scalar)

- `Preview` <= [11] CheapContrast output 0

### [52] Input Z lerp Alpha (Scalar)

- `Preview` <= [19] CheapContrast output 0

### [53] Output Z Projection World Space Normals

- `A` <= [118] Switch output 0

### [54] Output XY Projection World Space Normals

- `A` <= [59] Switch output 0

### [55] Output XY Flat Top Projection World Space Normals

- `A` <= [56] Lerp output 0

### [56] Lerp

- `A` <= [59] Switch output 0
- `B` <= [57] Input WorldNormal (Vector3) output 0
- `Alpha` <= [52] Input Z lerp Alpha (Scalar) output 0

### [57] Input WorldNormal (Vector3)

- `Preview` <= [32] VertexNormalWS output 0

### [59] Switch

- `A` <= [97] Transform3x3Matrix output 0
- `B` <= [7] Transform3x3Matrix output 0
- `Value` <= [76] Input Optimize (See Comment) (StaticBool) output 0

### [61] Cross

- `A` <= [116] VertexNormalWS output 0
- `B` <= [63] 0,0,1 output 0

### [62] Cross

- `A` <= [61] Cross output 0
- `B` <= [116] VertexNormalWS output 0

### [69] Append

- `A` <= [64] TexCoord[2] output 0
- `B` <= [68] BreakOutFloat2Components output 0

### [70] Append

- `A` <= [68] BreakOutFloat2Components output 1
- `B` <= [66] TexCoord[4] output 0

### [74] Append

- `A` <= [67] TexCoord[5] output 0
- `B` <= [72] BreakOutFloat2Components output 0

### [75] Append

- `A` <= [72] BreakOutFloat2Components output 1
- `B` <= [73] TexCoord[7] output 0

### [76] Input Optimize (See Comment) (StaticBool)

- `Preview` <= [60] Static Bool (False) output 0

### [77] Output UV3

- `A` <= [103] Append output 0

### [78] Output UV4

- `A` <= [102] Mask ( G B ) output 0

### [79] Output UV2

- `A` <= [99] Mask ( R G ) output 0

### [82] Abs

- `Input` <= [81] BreakOutFloat2Components output 0

### [83] ConstantBiasScale

- `Input` <= [82] Abs output 0

### [84] Append

- `A` <= [83] ConstantBiasScale output 0
- `B` <= [81] BreakOutFloat2Components output 1

### [85] DeriveNormalZ

- `InXY` <= [84] Append output 0

### [88] If

- `A` <= [81] BreakOutFloat2Components output 0
- `B` <= [91] 0 output 0
- `AGreaterThanB` <= [89] 1 output 0
- `AEqualsB` <= [89] 1 output 0
- `ALessThanB` <= [90] -1 output 0

### [92] Multiply

- `A` <= [86] MakeFloat3 output 0
- `B` <= [85] DeriveNormalZ output 0

### [99] Mask ( R G )

- `Input` <= [61] Cross output 0

### [100] Mask ( B )

- `Input` <= [61] Cross output 0

### [101] Mask ( R )

- `Input` <= [62] Cross output 0

### [102] Mask ( G B )

- `Input` <= [62] Cross output 0

### [103] Append

- `A` <= [100] Mask ( B ) output 0
- `B` <= [101] Mask ( R ) output 0

### [104] Normalize

- `VectorInput` <= [116] VertexNormalWS output 0

### [105] Cross

- `A` <= [104] Normalize output 0
- `B` <= [107] 0,1,0 output 0

### [106] Cross

- `A` <= [105] Cross output 0
- `B` <= [104] Normalize output 0

### [108] Output UV6

- `A` <= [115] Append output 0

### [109] Output UV7

- `A` <= [114] Mask ( G B ) output 0

### [110] Output UV5

- `A` <= [111] Mask ( R G ) output 0

### [111] Mask ( R G )

- `Input` <= [105] Cross output 0

### [112] Mask ( B )

- `Input` <= [105] Cross output 0

### [113] Mask ( R )

- `Input` <= [106] Cross output 0

### [114] Mask ( G B )

- `Input` <= [106] Cross output 0

### [115] Append

- `A` <= [112] Mask ( B ) output 0
- `B` <= [113] Mask ( R ) output 0

### [118] Switch

- `A` <= [98] Transform3x3Matrix output 0
- `B` <= [27] Transform3x3Matrix output 0
- `Value` <= [76] Input Optimize (See Comment) (StaticBool) output 0

## Connection List

- [18] Lerp output 0 -> Output World Space Normals XYZ.A
- [20] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [46] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [6] Ceil output 0 -> Lerp(-1,1,).Alpha
- [1] Texture Sample output 0 -> Multiply.A
- [9] MakeFloat3 output 0 -> Multiply.B
- [28] Multiply output 0 -> Lerp.A
- [3] Multiply output 0 -> Lerp.B
- [51] Input X lerp alpha (Scalar) output 0 -> Lerp.Alpha
- [57] Input WorldNormal (Vector3) output 0 -> Dot.A
- [10] 1,0,0 output 0 -> Dot.B
- [5] Dot output 0 -> Ceil.Input
- [57] Input WorldNormal (Vector3) output 0 -> Abs.Input
- [59] Switch output 0 -> Lerp.A
- [118] Switch output 0 -> Lerp.B
- [52] Input Z lerp Alpha (Scalar) output 0 -> Lerp.Alpha
- [23] Divide output 0 -> Mask ( G B ).Input
- [25] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [46] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [49] Input World Position (Vector3) output 0 -> Divide.A
- [48] Input Negative World Position Divisor (Vector3) output 0 -> Divide.B
- [29] Ceil output 0 -> Lerp(1,-1,).Alpha
- [23] Divide output 0 -> Mask ( R B ).Input
- [57] Input WorldNormal (Vector3) output 0 -> Dot.A
- [34] 0,1,0 output 0 -> Dot.B
- [21] Texture Sample output 0 -> Multiply.A
- [33] MakeFloat3 output 0 -> Multiply.B
- [26] Dot output 0 -> Ceil.Input
- [23] Divide output 0 -> Mask ( R G ).Input
- [30] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [46] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [42] Ceil output 0 -> Lerp(-1,1,).Alpha
- [57] Input WorldNormal (Vector3) output 0 -> Dot.A
- [44] 0,0,1 output 0 -> Dot.B
- [38] Texture Sample output 0 -> Multiply.A
- [43] MakeFloat3 output 0 -> Multiply.B
- [40] Dot output 0 -> Ceil.Input
- [47] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [22] Absolute World Position (Excluding Material Offsets) output 0 -> Input World Position (Vector3).Preview
- [11] CheapContrast output 0 -> Input X lerp alpha (Scalar).Preview
- [19] CheapContrast output 0 -> Input Z lerp Alpha (Scalar).Preview
- [118] Switch output 0 -> Output Z Projection World Space Normals.A
- [59] Switch output 0 -> Output XY Projection World Space Normals.A
- [56] Lerp output 0 -> Output XY Flat Top Projection World Space Normals.A
- [59] Switch output 0 -> Lerp.A
- [57] Input WorldNormal (Vector3) output 0 -> Lerp.B
- [52] Input Z lerp Alpha (Scalar) output 0 -> Lerp.Alpha
- [32] VertexNormalWS output 0 -> Input WorldNormal (Vector3).Preview
- [97] Transform3x3Matrix output 0 -> Switch.A
- [7] Transform3x3Matrix output 0 -> Switch.B
- [76] Input Optimize (See Comment) (StaticBool) output 0 -> Switch.Value
- [116] VertexNormalWS output 0 -> Cross.A
- [63] 0,0,1 output 0 -> Cross.B
- [61] Cross output 0 -> Cross.A
- [116] VertexNormalWS output 0 -> Cross.B
- [64] TexCoord[2] output 0 -> Append.A
- [68] BreakOutFloat2Components output 0 -> Append.B
- [68] BreakOutFloat2Components output 1 -> Append.A
- [66] TexCoord[4] output 0 -> Append.B
- [67] TexCoord[5] output 0 -> Append.A
- [72] BreakOutFloat2Components output 0 -> Append.B
- [72] BreakOutFloat2Components output 1 -> Append.A
- [73] TexCoord[7] output 0 -> Append.B
- [60] Static Bool (False) output 0 -> Input Optimize (See Comment) (StaticBool).Preview
- [103] Append output 0 -> Output UV3.A
- [102] Mask ( G B ) output 0 -> Output UV4.A
- [99] Mask ( R G ) output 0 -> Output UV2.A
- [81] BreakOutFloat2Components output 0 -> Abs.Input
- [82] Abs output 0 -> ConstantBiasScale.Input
- [83] ConstantBiasScale output 0 -> Append.A
- [81] BreakOutFloat2Components output 1 -> Append.B
- [84] Append output 0 -> DeriveNormalZ.InXY
- [81] BreakOutFloat2Components output 0 -> If.A
- [91] 0 output 0 -> If.B
- [89] 1 output 0 -> If.AGreaterThanB
- [89] 1 output 0 -> If.AEqualsB
- [90] -1 output 0 -> If.ALessThanB
- [86] MakeFloat3 output 0 -> Multiply.A
- [85] DeriveNormalZ output 0 -> Multiply.B
- [61] Cross output 0 -> Mask ( R G ).Input
- [61] Cross output 0 -> Mask ( B ).Input
- [62] Cross output 0 -> Mask ( R ).Input
- [62] Cross output 0 -> Mask ( G B ).Input
- [100] Mask ( B ) output 0 -> Append.A
- [101] Mask ( R ) output 0 -> Append.B
- [116] VertexNormalWS output 0 -> Normalize.VectorInput
- [104] Normalize output 0 -> Cross.A
- [107] 0,1,0 output 0 -> Cross.B
- [105] Cross output 0 -> Cross.A
- [104] Normalize output 0 -> Cross.B
- [115] Append output 0 -> Output UV6.A
- [114] Mask ( G B ) output 0 -> Output UV7.A
- [111] Mask ( R G ) output 0 -> Output UV5.A
- [105] Cross output 0 -> Mask ( R G ).Input
- [105] Cross output 0 -> Mask ( B ).Input
- [106] Cross output 0 -> Mask ( R ).Input
- [106] Cross output 0 -> Mask ( G B ).Input
- [112] Mask ( B ) output 0 -> Append.A
- [113] Mask ( R ) output 0 -> Append.B
- [98] Transform3x3Matrix output 0 -> Switch.A
- [27] Transform3x3Matrix output 0 -> Switch.B
- [76] Input Optimize (See Comment) (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `World Space Normals XYZ` is driven by [18] Lerp
- `Z Projection World Space Normals` is driven by [118] Switch
- `XY Projection World Space Normals` is driven by [59] Switch
- `XY Flat Top Projection World Space Normals` is driven by [56] Lerp
- `UV3` is driven by [103] Append
- `UV4` is driven by [102] Mask ( G B )
- `UV2` is driven by [99] Mask ( R G )
- `UV6` is driven by [115] Append
- `UV7` is driven by [114] Mask ( G B )
- `UV5` is driven by [111] Mask ( R G )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


