# Texture_Bombing_POM

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/Texture_Bombing_POM.Texture_Bombing_POM`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Texture bombing uses multiple offset texture samples to break up tiling artifacts.

## Inputs

- `Texture Object` [FunctionInput_Texture2D]
- `UVs` [FunctionInput_Vector2]
- `Tiling` [FunctionInput_Scalar]
- `Offset` [FunctionInput_Scalar] - The strength of the random offsets. 
- `Optional Heightmap` [FunctionInput_Texture2D] - The heightmap input is used only when "Enable Height Lerp" is Enabled. It is used to sharpen the blend between the textures to reduce blurring that occurs with simple blended transitions.
- `Contrast` [FunctionInput_Scalar]
- `Enable Height Lerp` [FunctionInput_StaticBool]
- `Is Normalmap` [FunctionInput_StaticBool] - Normal maps require a few extra instructions to account for the randomly flipping directions. Make sure to set this to TRUE when using a normal map as the input texture object.
- `HeightRatio` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Cosine Blend' |  |
| 2 | custom | `MaterialExpressionCustom` | Cosine Blend |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | expression | `MaterialExpressionFrac` | Frac |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionConstant2Vector` | 0,-1 |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionConstant2Vector` | -1,-1 |  |
| 11 | expression | `MaterialExpressionAdd` | Add(,0) |  |
| 12 | expression | `MaterialExpressionConstant2Vector` | -1,0 |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionCeil` | Ceil |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 17 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Texture Object (Texture2D) | Texture Object |
| 19 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Tiling (Scalar) | Tiling |
| 22 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Scalar) | Offset |
| 25 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 26 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Optional Heightmap (Texture2D) | Optional Heightmap |
| 28 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerp |  |
| 29 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerp |  |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 32 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 33 | expression | `MaterialExpressionMin` | Min(,1) |  |
| 34 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input Enable Height Lerp (StaticBool) | Enable Height Lerp |
| 36 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 38 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 39 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerp |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 42 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 43 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 44 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerp |  |
| 45 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 46 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 47 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 48 | expression | `MaterialExpressionConstant3Vector` | 1,0.2,0.2 |  |
| 49 | expression | `MaterialExpressionConstant3Vector` | 0.2,1,0.2 |  |
| 50 | expression | `MaterialExpressionConstant3Vector` | 0.2,0.2,1 |  |
| 51 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 52 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureBomb_SingleSample_POM |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 54 | expression | `MaterialExpressionMultiply` | Multiply(0,1) |  |
| 55 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 56 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,0.5) |  |
| 57 | expression | `MaterialExpressionLinearInterpolate` | Lerp(,,0.5) |  |
| 58 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 59 | custom | `MaterialExpressionCustom` | Pseudo Random |  |
| 60 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 61 | expression | `MaterialExpressionDivide` | Divide(,-1) |  |
| 62 | expression | `MaterialExpressionAdd` | Add |  |
| 63 | expression | `MaterialExpressionSubtract` | Subtract(1,) |  |
| 64 | expression | `MaterialExpressionDivide` | Divide(1,4) |  |
| 65 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 66 | expression | `MaterialExpressionMultiply` | Multiply(0,4) |  |
| 67 | expression | `MaterialExpressionDDX` | DDX |  |
| 68 | expression | `MaterialExpressionDDY` | DDY |  |
| 69 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureBomb_SingleSample_POM |  |
| 70 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureBomb_SingleSample_POM |  |
| 71 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureBomb_SingleSample_POM |  |
| 72 | expression | `MaterialExpressionDDX` | DDX |  |
| 73 | expression | `MaterialExpressionDDY` | DDY |  |
| 74 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 75 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 76 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 77 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 78 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 79 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,0.5) |  |
| 80 | function_input | `MaterialExpressionFunctionInput` | Input Is Normalmap (StaticBool) | Is Normalmap |
| 81 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 82 | function_input | `MaterialExpressionFunctionInput` | Input HeightRatio (Scalar) | HeightRatio |

## Exact Input Wiring

### [0] Output Result

- `A` <= [17] Lerp output 0

### [1] Switch Param (False) 'Cosine Blend'

- `A` <= [2] Cosine Blend output 0
- `B` <= [4] Frac output 0

### [4] Frac

- `Input` <= [22] Multiply(,1) output 0

### [5] Lerp

- `A` <= [41] Multiply(,1) output 0
- `B` <= [78] Multiply(,1) output 0
- `Alpha` <= [75] Switch output 0

### [6] Lerp

- `A` <= [40] Multiply(,1) output 0
- `B` <= [37] Multiply(,1) output 0
- `Alpha` <= [74] Switch output 0

### [8] Add

- `A` <= [14] Ceil output 0
- `B` <= [7] 0,-1 output 0

### [9] Add

- `A` <= [14] Ceil output 0
- `B` <= [10] -1,-1 output 0

### [11] Add(,0)

- `A` <= [14] Ceil output 0

### [13] Add

- `A` <= [14] Ceil output 0
- `B` <= [12] -1,0 output 0

### [14] Ceil

- `Input` <= [15] Multiply output 0

### [15] Multiply

- `A` <= [20] Input UVs (Vector2) output 0
- `B` <= [21] Input Tiling (Scalar) output 0

### [17] Lerp

- `A` <= [5] Lerp output 0
- `B` <= [6] Lerp output 0
- `Alpha` <= [77] Switch output 0

### [18] Input Texture Object (Texture2D)

- `Preview` <= [19] Texture Object output 0

### [20] Input UVs (Vector2)

- `Preview` <= [16] TexCoord[0] output 0

### [22] Multiply(,1)

- `A` <= [15] Multiply output 0

### [23] Multiply(,1)

- `A` <= [15] Multiply output 0

### [25] Multiply(,-1)

- `A` <= [24] Input Offset (Scalar) output 0

### [26] Texture Sample

- `Coordinates` <= [52] TextureBomb_SingleSample_POM output 0
- `TextureObject` <= [27] Input Optional Heightmap (Texture2D) output 0
- `CoordinatesDX` <= [72] DDX output 0
- `CoordinatesDY` <= [73] DDY output 0

### [27] Input Optional Heightmap (Texture2D)

- `Preview` <= [29] Texture Object output 0

### [32] Lerp

- `A` <= [3] BreakOutFloat2Components output 1
- `B` <= [28] HeightLerp output 1
- `Alpha` <= [33] Min(,1) output 0

### [33] Min(,1)

- `A` <= [31] Input Contrast (Scalar) output 0

### [34] Lerp

- `A` <= [3] BreakOutFloat2Components output 0
- `B` <= [30] HeightLerp output 1
- `Alpha` <= [33] Min(,1) output 0

### [35] Input Enable Height Lerp (StaticBool)

- `Preview` <= [36] Static Bool (False) output 0

### [37] Multiply(,1)

- `A` <= [52] TextureBomb_SingleSample_POM output 1

### [38] Texture Sample

- `Coordinates` <= [71] TextureBomb_SingleSample_POM output 0
- `TextureObject` <= [27] Input Optional Heightmap (Texture2D) output 0
- `CoordinatesDX` <= [72] DDX output 0
- `CoordinatesDY` <= [73] DDY output 0

### [40] Multiply(,1)

- `A` <= [69] TextureBomb_SingleSample_POM output 1

### [41] Multiply(,1)

- `A` <= [70] TextureBomb_SingleSample_POM output 1

### [42] Texture Sample

- `Coordinates` <= [69] TextureBomb_SingleSample_POM output 0
- `TextureObject` <= [27] Input Optional Heightmap (Texture2D) output 0
- `CoordinatesDX` <= [72] DDX output 0
- `CoordinatesDY` <= [73] DDY output 0

### [43] Lerp

- `A` <= [44] HeightLerp output 1
- `B` <= [28] HeightLerp output 1
- `Alpha` <= [30] HeightLerp output 1

### [45] Lerp

- `A` <= [46] BreakOutFloat2Components output 1
- `B` <= [44] HeightLerp output 1
- `Alpha` <= [33] Min(,1) output 0

### [51] Multiply(,1)

- `A` <= [15] Multiply output 0

### [53] Multiply(,1)

- `A` <= [59] Pseudo Random output 0

### [55] Multiply

- `A` <= [56] Lerp(0,1,0.5) output 0
- `B` <= [66] Multiply(0,4) output 0

### [57] Lerp(,,0.5)

- `A` <= [58] Swizzle output 1
- `B` <= [55] Multiply output 0

### [60] Texture Sample

- `Coordinates` <= [62] Add output 0
- `CoordinatesDX` <= [67] DDX output 0
- `CoordinatesDY` <= [68] DDY output 0

### [61] Divide(,-1)

- `A` <= [59] Pseudo Random output 0

### [62] Add

- `A` <= [53] Multiply(,1) output 0
- `B` <= [65] Subtract output 0

### [63] Subtract(1,)

- `B` <= [64] Divide(1,4) output 0

### [65] Subtract

- `A` <= [57] Lerp(,,0.5) output 0
- `B` <= [63] Subtract(1,) output 0

### [67] DDX

- `Value` <= [57] Lerp(,,0.5) output 0

### [68] DDY

- `Value` <= [57] Lerp(,,0.5) output 0

### [72] DDX

- `Value` <= [22] Multiply(,1) output 0

### [73] DDY

- `Value` <= [22] Multiply(,1) output 0

### [74] Switch

- `A` <= [30] HeightLerp output 1
- `B` <= [76] BreakOutFloat2Components output 0
- `Value` <= [35] Input Enable Height Lerp (StaticBool) output 0

### [75] Switch

- `A` <= [39] HeightLerp output 1
- `B` <= [76] BreakOutFloat2Components output 0
- `Value` <= [35] Input Enable Height Lerp (StaticBool) output 0

### [77] Switch

- `A` <= [43] Lerp output 0
- `B` <= [76] BreakOutFloat2Components output 1
- `Value` <= [35] Input Enable Height Lerp (StaticBool) output 0

### [78] Multiply(,1)

- `A` <= [71] TextureBomb_SingleSample_POM output 1

### [80] Input Is Normalmap (StaticBool)

- `Preview` <= [81] Static Bool (False) output 0

## Connection List

- [17] Lerp output 0 -> Output Result.A
- [2] Cosine Blend output 0 -> Switch Param (False) 'Cosine Blend'.A
- [4] Frac output 0 -> Switch Param (False) 'Cosine Blend'.B
- [22] Multiply(,1) output 0 -> Frac.Input
- [41] Multiply(,1) output 0 -> Lerp.A
- [78] Multiply(,1) output 0 -> Lerp.B
- [75] Switch output 0 -> Lerp.Alpha
- [40] Multiply(,1) output 0 -> Lerp.A
- [37] Multiply(,1) output 0 -> Lerp.B
- [74] Switch output 0 -> Lerp.Alpha
- [14] Ceil output 0 -> Add.A
- [7] 0,-1 output 0 -> Add.B
- [14] Ceil output 0 -> Add.A
- [10] -1,-1 output 0 -> Add.B
- [14] Ceil output 0 -> Add(,0).A
- [14] Ceil output 0 -> Add.A
- [12] -1,0 output 0 -> Add.B
- [15] Multiply output 0 -> Ceil.Input
- [20] Input UVs (Vector2) output 0 -> Multiply.A
- [21] Input Tiling (Scalar) output 0 -> Multiply.B
- [5] Lerp output 0 -> Lerp.A
- [6] Lerp output 0 -> Lerp.B
- [77] Switch output 0 -> Lerp.Alpha
- [19] Texture Object output 0 -> Input Texture Object (Texture2D).Preview
- [16] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [15] Multiply output 0 -> Multiply(,1).A
- [15] Multiply output 0 -> Multiply(,1).A
- [24] Input Offset (Scalar) output 0 -> Multiply(,-1).A
- [52] TextureBomb_SingleSample_POM output 0 -> Texture Sample.Coordinates
- [27] Input Optional Heightmap (Texture2D) output 0 -> Texture Sample.TextureObject
- [72] DDX output 0 -> Texture Sample.CoordinatesDX
- [73] DDY output 0 -> Texture Sample.CoordinatesDY
- [29] Texture Object output 0 -> Input Optional Heightmap (Texture2D).Preview
- [3] BreakOutFloat2Components output 1 -> Lerp.A
- [28] HeightLerp output 1 -> Lerp.B
- [33] Min(,1) output 0 -> Lerp.Alpha
- [31] Input Contrast (Scalar) output 0 -> Min(,1).A
- [3] BreakOutFloat2Components output 0 -> Lerp.A
- [30] HeightLerp output 1 -> Lerp.B
- [33] Min(,1) output 0 -> Lerp.Alpha
- [36] Static Bool (False) output 0 -> Input Enable Height Lerp (StaticBool).Preview
- [52] TextureBomb_SingleSample_POM output 1 -> Multiply(,1).A
- [71] TextureBomb_SingleSample_POM output 0 -> Texture Sample.Coordinates
- [27] Input Optional Heightmap (Texture2D) output 0 -> Texture Sample.TextureObject
- [72] DDX output 0 -> Texture Sample.CoordinatesDX
- [73] DDY output 0 -> Texture Sample.CoordinatesDY
- [69] TextureBomb_SingleSample_POM output 1 -> Multiply(,1).A
- [70] TextureBomb_SingleSample_POM output 1 -> Multiply(,1).A
- [69] TextureBomb_SingleSample_POM output 0 -> Texture Sample.Coordinates
- [27] Input Optional Heightmap (Texture2D) output 0 -> Texture Sample.TextureObject
- [72] DDX output 0 -> Texture Sample.CoordinatesDX
- [73] DDY output 0 -> Texture Sample.CoordinatesDY
- [44] HeightLerp output 1 -> Lerp.A
- [28] HeightLerp output 1 -> Lerp.B
- [30] HeightLerp output 1 -> Lerp.Alpha
- [46] BreakOutFloat2Components output 1 -> Lerp.A
- [44] HeightLerp output 1 -> Lerp.B
- [33] Min(,1) output 0 -> Lerp.Alpha
- [15] Multiply output 0 -> Multiply(,1).A
- [59] Pseudo Random output 0 -> Multiply(,1).A
- [56] Lerp(0,1,0.5) output 0 -> Multiply.A
- [66] Multiply(0,4) output 0 -> Multiply.B
- [58] Swizzle output 1 -> Lerp(,,0.5).A
- [55] Multiply output 0 -> Lerp(,,0.5).B
- [62] Add output 0 -> Texture Sample.Coordinates
- [67] DDX output 0 -> Texture Sample.CoordinatesDX
- [68] DDY output 0 -> Texture Sample.CoordinatesDY
- [59] Pseudo Random output 0 -> Divide(,-1).A
- [53] Multiply(,1) output 0 -> Add.A
- [65] Subtract output 0 -> Add.B
- [64] Divide(1,4) output 0 -> Subtract(1,).B
- [57] Lerp(,,0.5) output 0 -> Subtract.A
- [63] Subtract(1,) output 0 -> Subtract.B
- [57] Lerp(,,0.5) output 0 -> DDX.Value
- [57] Lerp(,,0.5) output 0 -> DDY.Value
- [22] Multiply(,1) output 0 -> DDX.Value
- [22] Multiply(,1) output 0 -> DDY.Value
- [30] HeightLerp output 1 -> Switch.A
- [76] BreakOutFloat2Components output 0 -> Switch.B
- [35] Input Enable Height Lerp (StaticBool) output 0 -> Switch.Value
- [39] HeightLerp output 1 -> Switch.A
- [76] BreakOutFloat2Components output 0 -> Switch.B
- [35] Input Enable Height Lerp (StaticBool) output 0 -> Switch.Value
- [43] Lerp output 0 -> Switch.A
- [76] BreakOutFloat2Components output 1 -> Switch.B
- [35] Input Enable Height Lerp (StaticBool) output 0 -> Switch.Value
- [71] TextureBomb_SingleSample_POM output 1 -> Multiply(,1).A
- [81] Static Bool (False) output 0 -> Input Is Normalmap (StaticBool).Preview

## Output Trace Roots

- `Result` is driven by [17] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
