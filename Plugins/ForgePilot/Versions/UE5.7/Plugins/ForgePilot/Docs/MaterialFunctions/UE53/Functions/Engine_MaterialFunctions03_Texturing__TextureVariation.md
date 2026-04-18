# TextureVariation

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation.TextureVariation`
- Category: Engine_MaterialFunctions03/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Gives Texture UV variation that can be used to break up tiling of repetitive textures. It is designed to use displacement maps to create an intersection based on height, and then does a temporal dither. This avoids needing to pass in every texture to be randomized as a texture object and instead the "Shifted UVs" output can be used for regular texture samples. Make sure to set texture samples to MipValueMode: Derivative. Additional Parameters such as Dithering and Scale Variance can be adjusted by creating a material instance of this material.

## Inputs

- `Heightmap` [FunctionInput_Texture2D] - This should contain a Texture Object with a Displacement map. Make sure to specify the Height Channel to match your texture.
- `UVs` [FunctionInput_Vector2] - The base input UVs
- `Variation Scale` [FunctionInput_Scalar] - This is how large the variation pattern should be relative to the base texture.
- `Variation Levels` [FunctionInput_Scalar] - This inputs allows you to change how many random variation bands there are.
- `Heightmap Influence` [FunctionInput_Scalar] - How much the displacement affects the blend edge location.
- `Random Rotation and Scale` [FunctionInput_StaticBool] - Whether to randomly rotate the texture samples. When enabled, this can cause issues with anisotropic Mip Maps. If this causes issues, consider using the function "Compute Mip Level" as an alternative to DDX,DDY.
- `Use Dither` [FunctionInput_StaticBool] - Enabling Dither uses a pattern to alternate between layers in a way that Temporal Anti Aliasing can smooth out.
- `HQ Edge Comparison` [FunctionInput_StaticBool] - Reads a second displacement sample so that at cell boundaries, both heights will be used when making a height comparison for a more accurate intersection. When false, only one displacement value is used.
- `Mask Channel` [FunctionInput_Vector4] - Which channel to use

## Outputs

- `Shifted UVs` - The randomly shifted UVs. Plug this pin into your texture UVs. 
- `Raw UVs` - To fix mip map artifacts, set your texture samples to MipValueMode: Derivative and use DDX, DDY from this Raw UVs pin. This will ensure the mip maps are calculated from clean unmodified UVs.
- `DDX`
- `Random Offset` - This is the random UV offset given to each random sample. The B channel contains the raondom Rotation used to fix up Normals.
- `DDY`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Shifted UVs | Shifted UVs |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap (Texture2D) | Heightmap |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Variation Scale (Scalar) | Variation Scale |
| 5 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Variation Levels (Scalar) | Variation Levels |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap Influence (Scalar) | Heightmap Influence |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Raw UVs | Raw UVs |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output DDX | DDX |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Random Offset | Random Offset |
| 11 | expression | `MaterialExpressionTextureSampleParameter2D` | Param2D 'RandomVariationPattern' |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldPosition-XY |  |
| 13 | expression | `MaterialExpressionConstant` | 1000 |  |
| 14 | expression | `MaterialExpressionDivide` | Divide |  |
| 15 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionScalarParameter` | Param (8) 'Random Offset Variation' |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 20 | expression | `MaterialExpressionFloor` | Floor |  |
| 21 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 22 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 23 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 28 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 29 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 30 | expression | `MaterialExpressionFrac` | Frac |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 32 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 33 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 34 | expression | `MaterialExpressionScalarParameter` | Param (8) 'Random Offset Variation' |  |
| 35 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureVariation_RotateUV |  |
| 36 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureVariation_RotateUV |  |
| 37 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 38 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 39 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 40 | expression | `MaterialExpressionDDX` | DDX |  |
| 41 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 42 | expression | `MaterialExpressionDDY` | DDY |  |
| 43 | expression | `MaterialExpressionDDX` | DDX |  |
| 44 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 45 | expression | `MaterialExpressionDDY` | DDY |  |
| 46 | expression | `MaterialExpressionAdd` | Add |  |
| 47 | function_input | `MaterialExpressionFunctionInput` | Input Random Rotation and Scale (StaticBool) | Random Rotation and Scale |
| 48 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 49 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 50 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 51 | expression | `MaterialExpressionConstant` | 0 |  |
| 52 | function_call | `MaterialExpressionMaterialFunctionCall` | DitherTemporalAA |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 54 | expression | `MaterialExpressionScalarParameter` | Param (0.2) 'Texture Variation Dithering' |  |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input Use Dither (StaticBool) | Use Dither |
| 56 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 57 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 58 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 59 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 60 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Texture Variation Height VS Mip' |  |
| 61 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 62 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 63 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 64 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 65 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 66 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 67 | expression | `MaterialExpressionShaderStageSwitch` | Shader Stage Switch |  |
| 68 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 69 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 70 | expression | `MaterialExpressionConstant` | 0 |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 72 | function_output | `MaterialExpressionFunctionOutput` | Output DDY | DDY |
| 73 | expression | `MaterialExpressionDivide` | Divide(,32) |  |
| 74 | expression | `MaterialExpressionAdd` | Add |  |
| 75 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 76 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 77 | expression | `MaterialExpressionComponentMask` | Mask ( B A ) |  |
| 78 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 79 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 80 | function_input | `MaterialExpressionFunctionInput` | Input HQ Edge Comparison (StaticBool) | HQ Edge Comparison |
| 81 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 82 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 83 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 84 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 85 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 86 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 87 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 88 | expression | `MaterialExpressionRound` | Round |  |
| 89 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 90 | expression | `MaterialExpressionAdd` | Add |  |
| 91 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 92 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 93 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 94 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 95 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 96 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 97 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 98 | function_input | `MaterialExpressionFunctionInput` | Input Mask Channel (Vector4) | Mask Channel |
| 99 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 100 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 101 | expression | `MaterialExpressionDivide` | Divide(,32) |  |
| 102 | expression | `MaterialExpressionAppendVector` | Append |  |
| 103 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 104 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 105 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 106 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 107 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 108 | expression | `MaterialExpressionScalarParameter` | Param (-0.5) 'Random Scale Variation' |  |
| 109 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 110 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 111 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 112 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 113 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 114 | expression | `MaterialExpressionScalarParameter` | Param (-0.3) 'Random Scale Variation' |  |
| 115 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 116 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 117 | expression | `MaterialExpressionAdd` | Add |  |
| 118 | expression | `MaterialExpressionScalarParameter` | Param (9) 'Random Stripe Offset' |  |

## Exact Input Wiring

### [0] Output Shifted UVs

- `A` <= [31] Mask ( R G ) output 0

### [1] Multiply

- `A` <= [11] Param2D 'RandomVariationPattern' output 1
- `B` <= [6] Input Variation Levels (Scalar) output 0

### [2] Input Heightmap (Texture2D)

- `Preview` <= [5] Texture Object output 0

### [3] Input UVs (Vector2)

- `Preview` <= [12] WorldPosition-XY output 0

### [8] Output Raw UVs

- `A` <= [3] Input UVs (Vector2) output 0

### [9] Output DDX

- `A` <= [40] DDX output 0

### [10] Output Random Offset

- `A` <= [84] Lerp output 0

### [11] Param2D 'RandomVariationPattern'

- `Coordinates` <= [14] Divide output 0

### [14] Divide

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [4] Input Variation Scale (Scalar) output 0

### [15] Mask ( R G )

- `Input` <= [76] Mask ( R G ) output 0

### [16] Multiply

- `A` <= [15] Mask ( R G ) output 0
- `B` <= [17] Param (8) 'Random Offset Variation' output 0

### [18] Mask ( R G )

- `Input` <= [77] Mask ( B A ) output 0

### [19] Multiply

- `A` <= [18] Mask ( R G ) output 0
- `B` <= [34] Param (8) 'Random Offset Variation' output 0

### [20] Floor

- `Input` <= [21] Reroute Node (reroutes wires) output 0

### [21] Reroute Node (reroutes wires)

- `Input` <= [1] Multiply output 0

### [22] Texture Sample

- `Coordinates` <= [32] Reroute Node (reroutes wires) output 0
- `TextureObject` <= [57] Reroute Node (reroutes wires) output 0
- `CoordinatesDX` <= [9] Output DDX output 0
- `CoordinatesDY` <= [72] Output DDY output 0

### [23] Texture Sample

- `Coordinates` <= [33] Reroute Node (reroutes wires) output 0
- `TextureObject` <= [61] Reroute Node (reroutes wires) output 0
- `CoordinatesDX` <= [43] DDX output 0
- `CoordinatesDY` <= [45] DDY output 0

### [24] Add

- `A` <= [25] Reroute Node (reroutes wires) output 0
- `B` <= [16] Multiply output 0

### [25] Reroute Node (reroutes wires)

- `Input` <= [8] Output Raw UVs output 0

### [26] Add

- `A` <= [25] Reroute Node (reroutes wires) output 0
- `B` <= [19] Multiply output 0

### [27] Subtract

- `A` <= [23] Texture Sample output 5
- `B` <= [22] Texture Sample output 5

### [28] Lerp

- `A` <= [83] Reroute Node (reroutes wires) output 0
- `B` <= [94] Reroute Node (reroutes wires) output 0
- `Alpha` <= [89] Saturate output 0

### [29] Reroute Node (reroutes wires)

- `Input` <= [21] Reroute Node (reroutes wires) output 0

### [30] Frac

- `Input` <= [29] Reroute Node (reroutes wires) output 0

### [31] Mask ( R G )

- `Input` <= [28] Lerp output 0

### [32] Reroute Node (reroutes wires)

- `Input` <= [49] Switch output 0

### [33] Reroute Node (reroutes wires)

- `Input` <= [50] Switch output 0

### [37] Reroute Node (reroutes wires)

- `Input` <= [50] Switch output 0

### [38] Reroute Node (reroutes wires)

- `Input` <= [49] Switch output 0

### [39] Reroute Node (reroutes wires)

- `Input` <= [8] Output Raw UVs output 0

### [40] DDX

- `Value` <= [41] Reroute Node (reroutes wires) output 0

### [41] Reroute Node (reroutes wires)

- `Input` <= [39] Reroute Node (reroutes wires) output 0

### [42] DDY

- `Value` <= [41] Reroute Node (reroutes wires) output 0

### [43] DDX

- `Value` <= [44] Reroute Node (reroutes wires) output 0

### [44] Reroute Node (reroutes wires)

- `Input` <= [41] Reroute Node (reroutes wires) output 0

### [45] DDY

- `Value` <= [44] Reroute Node (reroutes wires) output 0

### [46] Add

- `A` <= [30] Frac output 0
- `B` <= [56] Switch output 0

### [47] Input Random Rotation and Scale (StaticBool)

- `Preview` <= [48] Static Bool (False) output 0

### [49] Switch

- `A` <= [35] TextureVariation_RotateUV output 0
- `B` <= [65] Reroute Node (reroutes wires) output 0
- `Value` <= [47] Input Random Rotation and Scale (StaticBool) output 0

### [50] Switch

- `A` <= [36] TextureVariation_RotateUV output 0
- `B` <= [66] Reroute Node (reroutes wires) output 0
- `Value` <= [47] Input Random Rotation and Scale (StaticBool) output 0

### [53] Multiply

- `A` <= [52] DitherTemporalAA output 0
- `B` <= [54] Param (0.2) 'Texture Variation Dithering' output 0

### [55] Input Use Dither (StaticBool)

- `Preview` <= [69] Static Bool (False) output 0

### [56] Switch

- `A` <= [71] Multiply(,0.5) output 0
- `B` <= [51] 0 output 0
- `Value` <= [55] Input Use Dither (StaticBool) output 0

### [57] Reroute Node (reroutes wires)

- `Input` <= [2] Input Heightmap (Texture2D) output 0

### [58] Texture Sample

- `Coordinates` <= [63] Reroute Node (reroutes wires) output 0
- `TextureObject` <= [62] Reroute Node (reroutes wires) output 0
- `MipValue` <= [60] Param (0) 'Texture Variation Height VS Mip' output 0

### [59] Texture Sample

- `Coordinates` <= [64] Reroute Node (reroutes wires) output 0
- `TextureObject` <= [62] Reroute Node (reroutes wires) output 0
- `MipValue` <= [60] Param (0) 'Texture Variation Height VS Mip' output 0

### [61] Reroute Node (reroutes wires)

- `Input` <= [57] Reroute Node (reroutes wires) output 0

### [62] Reroute Node (reroutes wires)

- `Input` <= [61] Reroute Node (reroutes wires) output 0

### [63] Reroute Node (reroutes wires)

- `Input` <= [32] Reroute Node (reroutes wires) output 0

### [64] Reroute Node (reroutes wires)

- `Input` <= [33] Reroute Node (reroutes wires) output 0

### [65] Reroute Node (reroutes wires)

- `Input` <= [24] Add output 0

### [66] Reroute Node (reroutes wires)

- `Input` <= [26] Add output 0

### [67] Shader Stage Switch

- `PixelShader` <= [82] Switch output 0
- `VertexShader` <= [68] Subtract output 0

### [68] Subtract

- `A` <= [59] Texture Sample output 1
- `B` <= [58] Texture Sample output 1

### [71] Multiply(,0.5)

- `A` <= [53] Multiply output 0

### [72] Output DDY

- `A` <= [42] DDY output 0

### [73] Divide(,32)

- `A` <= [117] Add output 0

### [74] Add

- `A` <= [73] Divide(,32) output 0
- `B` <= [101] Divide(,32) output 0

### [75] Texture Sample

- `Coordinates` <= [102] Append output 0

### [76] Mask ( R G )

- `Input` <= [75] Texture Sample output 0

### [77] Mask ( B A )

- `Input` <= [75] Texture Sample output 5

### [78] Reroute Node (reroutes wires)

- `Input` <= [100] Reroute Node (reroutes wires) output 0

### [79] Reroute Node (reroutes wires)

- `Input` <= [99] Reroute Node (reroutes wires) output 0

### [80] Input HQ Edge Comparison (StaticBool)

- `Preview` <= [81] Static Bool (True) output 0

### [82] Switch

- `A` <= [27] Subtract output 0
- `B` <= [96] Subtract output 0
- `Value` <= [80] Input HQ Edge Comparison (StaticBool) output 0

### [83] Reroute Node (reroutes wires)

- `Input` <= [38] Reroute Node (reroutes wires) output 0

### [84] Lerp

- `A` <= [86] Reroute Node (reroutes wires) output 0
- `B` <= [93] Reroute Node (reroutes wires) output 0
- `Alpha` <= [89] Saturate output 0

### [85] Reroute Node (reroutes wires)

- `Input` <= [99] Reroute Node (reroutes wires) output 0

### [86] Reroute Node (reroutes wires)

- `Input` <= [87] Reroute Node (reroutes wires) output 0

### [87] Reroute Node (reroutes wires)

- `Input` <= [100] Reroute Node (reroutes wires) output 0

### [88] Round

- `Input` <= [90] Add output 0

### [89] Saturate

- `Input` <= [88] Round output 0

### [90] Add

- `A` <= [92] Reroute Node (reroutes wires) output 0
- `B` <= [91] Multiply output 0

### [91] Multiply

- `A` <= [7] Input Heightmap Influence (Scalar) output 0
- `B` <= [97] Dot output 0

### [92] Reroute Node (reroutes wires)

- `Input` <= [46] Add output 0

### [93] Reroute Node (reroutes wires)

- `Input` <= [85] Reroute Node (reroutes wires) output 0

### [94] Reroute Node (reroutes wires)

- `Input` <= [37] Reroute Node (reroutes wires) output 0

### [96] Subtract

- `A` <= [95] 0.5 output 0
- `B` <= [22] Texture Sample output 5

### [97] Dot

- `A` <= [67] Shader Stage Switch output 0
- `B` <= [98] Input Mask Channel (Vector4) output 0

### [99] Reroute Node (reroutes wires)

- `Input` <= [18] Mask ( R G ) output 0

### [100] Reroute Node (reroutes wires)

- `Input` <= [15] Mask ( R G ) output 0

### [101] Divide(,32)

- `A` <= [104] 0.5 output 0

### [102] Append

- `A` <= [74] Add output 0
- `B` <= [103] 0.5 output 0

### [105] Mask ( R )

- `Input` <= [15] Mask ( R G ) output 0

### [106] Subtract(,0.5)

- `A` <= [105] Mask ( R ) output 0

### [107] Multiply

- `A` <= [106] Subtract(,0.5) output 0
- `B` <= [108] Param (-0.5) 'Random Scale Variation' output 0

### [109] Multiply

- `A` <= [24] Add output 0
- `B` <= [115] Add(,1) output 0

### [110] Multiply

- `A` <= [26] Add output 0
- `B` <= [116] Add(,1) output 0

### [111] Subtract(,0.5)

- `A` <= [112] Mask ( R ) output 0

### [112] Mask ( R )

- `Input` <= [18] Mask ( R G ) output 0

### [113] Multiply

- `A` <= [111] Subtract(,0.5) output 0
- `B` <= [114] Param (-0.3) 'Random Scale Variation' output 0

### [115] Add(,1)

- `A` <= [107] Multiply output 0

### [116] Add(,1)

- `A` <= [113] Multiply output 0

### [117] Add

- `A` <= [20] Floor output 0
- `B` <= [118] Param (9) 'Random Stripe Offset' output 0

## Connection List

- [31] Mask ( R G ) output 0 -> Output Shifted UVs.A
- [11] Param2D 'RandomVariationPattern' output 1 -> Multiply.A
- [6] Input Variation Levels (Scalar) output 0 -> Multiply.B
- [5] Texture Object output 0 -> Input Heightmap (Texture2D).Preview
- [12] WorldPosition-XY output 0 -> Input UVs (Vector2).Preview
- [3] Input UVs (Vector2) output 0 -> Output Raw UVs.A
- [40] DDX output 0 -> Output DDX.A
- [84] Lerp output 0 -> Output Random Offset.A
- [14] Divide output 0 -> Param2D 'RandomVariationPattern'.Coordinates
- [3] Input UVs (Vector2) output 0 -> Divide.A
- [4] Input Variation Scale (Scalar) output 0 -> Divide.B
- [76] Mask ( R G ) output 0 -> Mask ( R G ).Input
- [15] Mask ( R G ) output 0 -> Multiply.A
- [17] Param (8) 'Random Offset Variation' output 0 -> Multiply.B
- [77] Mask ( B A ) output 0 -> Mask ( R G ).Input
- [18] Mask ( R G ) output 0 -> Multiply.A
- [34] Param (8) 'Random Offset Variation' output 0 -> Multiply.B
- [21] Reroute Node (reroutes wires) output 0 -> Floor.Input
- [1] Multiply output 0 -> Reroute Node (reroutes wires).Input
- [32] Reroute Node (reroutes wires) output 0 -> Texture Sample.Coordinates
- [57] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [9] Output DDX output 0 -> Texture Sample.CoordinatesDX
- [72] Output DDY output 0 -> Texture Sample.CoordinatesDY
- [33] Reroute Node (reroutes wires) output 0 -> Texture Sample.Coordinates
- [61] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [43] DDX output 0 -> Texture Sample.CoordinatesDX
- [45] DDY output 0 -> Texture Sample.CoordinatesDY
- [25] Reroute Node (reroutes wires) output 0 -> Add.A
- [16] Multiply output 0 -> Add.B
- [8] Output Raw UVs output 0 -> Reroute Node (reroutes wires).Input
- [25] Reroute Node (reroutes wires) output 0 -> Add.A
- [19] Multiply output 0 -> Add.B
- [23] Texture Sample output 5 -> Subtract.A
- [22] Texture Sample output 5 -> Subtract.B
- [83] Reroute Node (reroutes wires) output 0 -> Lerp.A
- [94] Reroute Node (reroutes wires) output 0 -> Lerp.B
- [89] Saturate output 0 -> Lerp.Alpha
- [21] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [29] Reroute Node (reroutes wires) output 0 -> Frac.Input
- [28] Lerp output 0 -> Mask ( R G ).Input
- [49] Switch output 0 -> Reroute Node (reroutes wires).Input
- [50] Switch output 0 -> Reroute Node (reroutes wires).Input
- [50] Switch output 0 -> Reroute Node (reroutes wires).Input
- [49] Switch output 0 -> Reroute Node (reroutes wires).Input
- [8] Output Raw UVs output 0 -> Reroute Node (reroutes wires).Input
- [41] Reroute Node (reroutes wires) output 0 -> DDX.Value
- [39] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [41] Reroute Node (reroutes wires) output 0 -> DDY.Value
- [44] Reroute Node (reroutes wires) output 0 -> DDX.Value
- [41] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [44] Reroute Node (reroutes wires) output 0 -> DDY.Value
- [30] Frac output 0 -> Add.A
- [56] Switch output 0 -> Add.B
- [48] Static Bool (False) output 0 -> Input Random Rotation and Scale (StaticBool).Preview
- [35] TextureVariation_RotateUV output 0 -> Switch.A
- [65] Reroute Node (reroutes wires) output 0 -> Switch.B
- [47] Input Random Rotation and Scale (StaticBool) output 0 -> Switch.Value
- [36] TextureVariation_RotateUV output 0 -> Switch.A
- [66] Reroute Node (reroutes wires) output 0 -> Switch.B
- [47] Input Random Rotation and Scale (StaticBool) output 0 -> Switch.Value
- [52] DitherTemporalAA output 0 -> Multiply.A
- [54] Param (0.2) 'Texture Variation Dithering' output 0 -> Multiply.B
- [69] Static Bool (False) output 0 -> Input Use Dither (StaticBool).Preview
- [71] Multiply(,0.5) output 0 -> Switch.A
- [51] 0 output 0 -> Switch.B
- [55] Input Use Dither (StaticBool) output 0 -> Switch.Value
- [2] Input Heightmap (Texture2D) output 0 -> Reroute Node (reroutes wires).Input
- [63] Reroute Node (reroutes wires) output 0 -> Texture Sample.Coordinates
- [62] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [60] Param (0) 'Texture Variation Height VS Mip' output 0 -> Texture Sample.MipValue
- [64] Reroute Node (reroutes wires) output 0 -> Texture Sample.Coordinates
- [62] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [60] Param (0) 'Texture Variation Height VS Mip' output 0 -> Texture Sample.MipValue
- [57] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [61] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [32] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [33] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [24] Add output 0 -> Reroute Node (reroutes wires).Input
- [26] Add output 0 -> Reroute Node (reroutes wires).Input
- [82] Switch output 0 -> Shader Stage Switch.PixelShader
- [68] Subtract output 0 -> Shader Stage Switch.VertexShader
- [59] Texture Sample output 1 -> Subtract.A
- [58] Texture Sample output 1 -> Subtract.B
- [53] Multiply output 0 -> Multiply(,0.5).A
- [42] DDY output 0 -> Output DDY.A
- [117] Add output 0 -> Divide(,32).A
- [73] Divide(,32) output 0 -> Add.A
- [101] Divide(,32) output 0 -> Add.B
- [102] Append output 0 -> Texture Sample.Coordinates
- [75] Texture Sample output 0 -> Mask ( R G ).Input
- [75] Texture Sample output 5 -> Mask ( B A ).Input
- [100] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [99] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [81] Static Bool (True) output 0 -> Input HQ Edge Comparison (StaticBool).Preview
- [27] Subtract output 0 -> Switch.A
- [96] Subtract output 0 -> Switch.B
- [80] Input HQ Edge Comparison (StaticBool) output 0 -> Switch.Value
- [38] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [86] Reroute Node (reroutes wires) output 0 -> Lerp.A
- [93] Reroute Node (reroutes wires) output 0 -> Lerp.B
- [89] Saturate output 0 -> Lerp.Alpha
- [99] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [87] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [100] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [90] Add output 0 -> Round.Input
- [88] Round output 0 -> Saturate.Input
- [92] Reroute Node (reroutes wires) output 0 -> Add.A
- [91] Multiply output 0 -> Add.B
- [7] Input Heightmap Influence (Scalar) output 0 -> Multiply.A
- [97] Dot output 0 -> Multiply.B
- [46] Add output 0 -> Reroute Node (reroutes wires).Input
- [85] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [37] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [95] 0.5 output 0 -> Subtract.A
- [22] Texture Sample output 5 -> Subtract.B
- [67] Shader Stage Switch output 0 -> Dot.A
- [98] Input Mask Channel (Vector4) output 0 -> Dot.B
- [18] Mask ( R G ) output 0 -> Reroute Node (reroutes wires).Input
- [15] Mask ( R G ) output 0 -> Reroute Node (reroutes wires).Input
- [104] 0.5 output 0 -> Divide(,32).A
- [74] Add output 0 -> Append.A
- [103] 0.5 output 0 -> Append.B
- [15] Mask ( R G ) output 0 -> Mask ( R ).Input
- [105] Mask ( R ) output 0 -> Subtract(,0.5).A
- [106] Subtract(,0.5) output 0 -> Multiply.A
- [108] Param (-0.5) 'Random Scale Variation' output 0 -> Multiply.B
- [24] Add output 0 -> Multiply.A
- [115] Add(,1) output 0 -> Multiply.B
- [26] Add output 0 -> Multiply.A
- [116] Add(,1) output 0 -> Multiply.B
- [112] Mask ( R ) output 0 -> Subtract(,0.5).A
- [18] Mask ( R G ) output 0 -> Mask ( R ).Input
- [111] Subtract(,0.5) output 0 -> Multiply.A
- [114] Param (-0.3) 'Random Scale Variation' output 0 -> Multiply.B
- [107] Multiply output 0 -> Add(,1).A
- [113] Multiply output 0 -> Add(,1).A
- [20] Floor output 0 -> Add.A
- [118] Param (9) 'Random Stripe Offset' output 0 -> Add.B

## Output Trace Roots

- `Shifted UVs` is driven by [31] Mask ( R G )
- `Raw UVs` is driven by [3] Input UVs (Vector2)
- `DDX` is driven by [40] DDX
- `Random Offset` is driven by [84] Lerp
- `DDY` is driven by [42] DDY

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
