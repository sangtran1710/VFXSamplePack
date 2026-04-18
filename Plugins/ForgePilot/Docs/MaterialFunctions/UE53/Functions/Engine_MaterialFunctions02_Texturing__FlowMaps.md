# FlowMaps

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/Flowmaps.FlowMaps`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Pushes the Diffuse and Normal textures along the flow map vectors. See Material'/Engine/Functions/Engine_MaterialFunctions02/ExampleContent/FlowMaps.FlowMaps' for setting suggestions or additional techniques.

## Inputs

- `Diffuse` [FunctionInput_Texture2D] - This can be any texture not just a diffuse map. The rgb and alpha values are returned. 
- `Flow Vector Map (see tooltip)` [FunctionInput_Vector2] - Flow direction from -1 to 1. Modulating the flowmap with a macro noise texture may help reduce tiling artifacts. Please note: There is a discrepancy between 3DS Max/Mayas representation of uvs and Unreals. The green channel is inverted. If the provided Photoshop action is used to generate the flow vector maps then this inversion will be corrected. Also note that any textures used as flow maps should have sRGB in the texture properties window disabled. Using low resolution textures with vector displacement texture compression enabled is preferable.
- `UVs` [FunctionInput_Vector2]
- `Time (see tooltip)` [FunctionInput_Scalar] - Adding a large scale noise map to time will help offset any tiling artifacts.
- `UV texture 2 offset` [FunctionInput_Vector2]
- `Normal Map` [FunctionInput_Texture2D]
- `Texture Mip Bias` [FunctionInput_Scalar] - Numbers larger than one will make your diffuse and normal texture maps textures blurry. This is useful for shorelines.
- `Texture Size` [FunctionInput_Vector2] - Input the diffuse or normal maps XY Size. (1024x1024 for example)
- `useMipLevel` [FunctionInput_StaticBool]

## Outputs

- `Diffuse`
- `Diffuse Alpha`
- `Normal`
- `Distortion` - R= amount of distortion for lerp sample A. G= distortion amount for Lerp input B.  B= Lerp Alpha. A= Lerped Distortion amounts. This is useful for modulating normals by the amount of movement that occurs in the texture. I.e. Lerp ((0,0,1), (This nodes normal output), (This nodes distortions alpha channel)) to only display the normals where the waves are currently highest.
- `UVs` - R= amount of distortion for lerp sample A. G= distortion amount for Lerp input B.  B= Lerp Alpha. A= Lerped Distortion amounts. This is useful for modulating normals by the amount of movement that occurs in the texture. I.e. Lerp ((0,0,1), (This nodes normal output), (This nodes distortions alpha channel)) to only display the normals where the waves are currently highest.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Diffuse | Diffuse |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse (Texture2D) | Diffuse |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Flow Vector Map (see tooltip) (Vector2) | Flow Vector Map (see tooltip) |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 6 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 7 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Time (see tooltip) (Scalar) | Time (see tooltip) |
| 10 | expression | `MaterialExpressionFrac` | Frac |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearSine |  |
| 12 | expression | `MaterialExpressionFrac` | Frac |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 16 | expression | `MaterialExpressionFrac` | Frac |  |
| 17 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input UV texture 2 offset (Vector2) | UV texture 2 offset |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | AddComponents |  |
| 22 | expression | `MaterialExpressionAbs` | Abs |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | AddComponents |  |
| 25 | expression | `MaterialExpressionAbs` | Abs |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionAppendVector` | Append |  |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Normal Map (Texture2D) | Normal Map |
| 29 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 30 | expression | `MaterialExpressionAppendVector` | Append |  |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output Diffuse Alpha | Diffuse Alpha |
| 32 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 33 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 34 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 35 | expression | `MaterialExpressionAppendVector` | Append |  |
| 36 | expression | `MaterialExpressionConstant` | 1 |  |
| 37 | function_output | `MaterialExpressionFunctionOutput` | Output Distortion | Distortion |
| 38 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 39 | function_call | `MaterialExpressionMaterialFunctionCall` | BlendAngleCorrectedNormals |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 42 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 43 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 44 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 45 | expression | `MaterialExpressionAdd` | Add |  |
| 46 | expression | `MaterialExpressionAppendVector` | Append |  |
| 47 | expression | `MaterialExpressionConstant` | 0 |  |
| 48 | expression | `MaterialExpressionAdd` | Add |  |
| 49 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 50 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 51 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeVectorsOrthogonal |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 53 | expression | `MaterialExpressionConstant` | 1 |  |
| 54 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,0.5) |  |
| 55 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 56 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 57 | expression | `MaterialExpressionAppendVector` | Append |  |
| 58 | expression | `MaterialExpressionAppendVector` | Append |  |
| 59 | expression | `MaterialExpressionAppendVector` | Append |  |
| 60 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 61 | function_call | `MaterialExpressionMaterialFunctionCall` | ComputeMipLevel |  |
| 62 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 63 | expression | `MaterialExpressionConstant2Vector` | -1,1 |  |
| 64 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 65 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 66 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 67 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 68 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 69 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 70 | expression | `MaterialExpressionAdd` | Add |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply(,0.25) |  |
| 72 | expression | `MaterialExpressionAdd` | Add |  |
| 73 | expression | `MaterialExpressionFrac` | Frac |  |
| 74 | expression | `MaterialExpressionConstant` | 0.02 |  |
| 75 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 76 | expression | `MaterialExpressionAdd` | Add |  |
| 77 | function_input | `MaterialExpressionFunctionInput` | Input Texture Mip Bias (Scalar) | Texture Mip Bias |
| 78 | function_input | `MaterialExpressionFunctionInput` | Input Texture Size (Vector2) | Texture Size |
| 79 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 80 | function_input | `MaterialExpressionFunctionInput` | Input useMipLevel (StaticBool) | useMipLevel |
| 81 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 82 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 83 | expression | `MaterialExpressionAppendVector` | Append |  |
| 84 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 85 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 86 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 87 | expression | `MaterialExpressionAppendVector` | Append |  |
| 88 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 89 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 90 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 91 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 92 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |

## Exact Input Wiring

### [0] Output Diffuse

- `A` <= [55] Mask ( R G B ) output 0

### [1] Texture Sample

- `Coordinates` <= [18] Add output 0
- `TextureObject` <= [2] Input Diffuse (Texture2D) output 0
- `MipValue` <= [76] Add output 0

### [2] Input Diffuse (Texture2D)

- `Preview` <= [65] Texture Object output 0

### [3] Add

- `A` <= [5] Input UVs (Vector2) output 0
- `B` <= [13] Multiply output 0

### [5] Input UVs (Vector2)

- `Preview` <= [6] TexCoord[0] output 0

### [7] Texture Sample

- `Coordinates` <= [8] Add output 0
- `TextureObject` <= [2] Input Diffuse (Texture2D) output 0
- `MipValue` <= [76] Add output 0

### [8] Add

- `A` <= [14] Multiply output 0
- `B` <= [5] Input UVs (Vector2) output 0

### [9] Input Time (see tooltip) (Scalar)

- `Preview` <= [70] Add output 0

### [10] Frac

- `Input` <= [3] Add output 0

### [12] Frac

- `Input` <= [9] Input Time (see tooltip) (Scalar) output 0

### [13] Multiply

- `A` <= [16] Frac output 0
- `B` <= [62] Multiply output 0

### [14] Multiply

- `A` <= [62] Multiply output 0
- `B` <= [12] Frac output 0

### [15] Subtract(,0.5)

- `A` <= [9] Input Time (see tooltip) (Scalar) output 0

### [16] Frac

- `Input` <= [15] Subtract(,0.5) output 0

### [18] Add

- `A` <= [10] Frac output 0
- `B` <= [20] Input UV texture 2 offset (Vector2) output 0

### [19] Lerp

- `A` <= [86] Switch output 0
- `B` <= [81] Switch output 0
- `Alpha` <= [11] LinearSine output 0

### [22] Abs

- `Input` <= [14] Multiply output 0

### [23] Multiply

- `A` <= [21] AddComponents output 0
- `B` <= [21] AddComponents output 0

### [25] Abs

- `Input` <= [13] Multiply output 0

### [26] Multiply

- `A` <= [24] AddComponents output 0
- `B` <= [24] AddComponents output 0

### [27] Append

- `A` <= [7] Texture Sample output 0
- `B` <= [7] Texture Sample output 4

### [28] Input Normal Map (Texture2D)

- `Preview` <= [64] Texture Object output 0

### [29] Texture Sample

- `Coordinates` <= [18] Add output 0
- `TextureObject` <= [28] Input Normal Map (Texture2D) output 0
- `MipValue` <= [76] Add output 0

### [30] Append

- `A` <= [1] Texture Sample output 0
- `B` <= [1] Texture Sample output 4

### [31] Output Diffuse Alpha

- `A` <= [56] Mask ( A ) output 0

### [32] Output Normal

- `A` <= [34] Lerp output 0

### [33] Texture Sample

- `Coordinates` <= [8] Add output 0
- `TextureObject` <= [28] Input Normal Map (Texture2D) output 0
- `MipValue` <= [76] Add output 0

### [34] Lerp

- `A` <= [88] Switch output 0
- `B` <= [84] Switch output 0
- `Alpha` <= [11] LinearSine output 0

### [35] Append

- `A` <= [62] Multiply output 0
- `B` <= [36] 1 output 0

### [37] Output Distortion

- `A` <= [59] Append output 0

### [38] Normalize

- `VectorInput` <= [35] Append output 0

### [40] Add

- `A` <= [39] BlendAngleCorrectedNormals output 0
- `B` <= [41] 0,0,1 output 0

### [42] Normalize

- `VectorInput` <= [40] Add output 0

### [45] Add

- `A` <= [52] Multiply output 0
- `B` <= [43] 1,0,0 output 0

### [46] Append

- `A` <= [54] Lerp(0,1,0.5) output 0
- `B` <= [47] 0 output 0

### [48] Add

- `A` <= [52] Multiply output 0
- `B` <= [44] 0,1,0 output 0

### [52] Multiply

- `A` <= [46] Append output 0
- `B` <= [53] 1 output 0

### [55] Mask ( R G B )

- `Input` <= [19] Lerp output 0

### [56] Mask ( A )

- `Input` <= [19] Lerp output 0

### [57] Append

- `A` <= [26] Multiply output 0
- `B` <= [23] Multiply output 0

### [58] Append

- `A` <= [57] Append output 0
- `B` <= [11] LinearSine output 0

### [59] Append

- `A` <= [58] Append output 0
- `B` <= [60] Lerp output 0

### [60] Lerp

- `A` <= [26] Multiply output 0
- `B` <= [23] Multiply output 0
- `Alpha` <= [11] LinearSine output 0

### [62] Multiply

- `A` <= [4] Input Flow Vector Map (see tooltip) (Vector2) output 0
- `B` <= [63] -1,1 output 0

### [69] Texture Sample

- `Coordinates` <= [73] Frac output 0

### [70] Add

- `A` <= [71] Multiply(,0.25) output 0
- `B` <= [66] TimeWithSpeedVariable output 1

### [71] Multiply(,0.25)

- `A` <= [69] Texture Sample output 1

### [72] Add

- `A` <= [68] TexCoord[0] output 0
- `B` <= [75] Multiply output 0

### [73] Frac

- `Input` <= [72] Add output 0

### [75] Multiply

- `A` <= [66] TimeWithSpeedVariable output 1
- `B` <= [74] 0.02 output 0

### [76] Add

- `A` <= [61] ComputeMipLevel output 0
- `B` <= [77] Input Texture Mip Bias (Scalar) output 0

### [80] Input useMipLevel (StaticBool)

- `Preview` <= [79] Static Bool (True) output 0

### [81] Switch

- `A` <= [27] Append output 0
- `B` <= [83] Append output 0
- `Value` <= [80] Input useMipLevel (StaticBool) output 0

### [82] Texture Sample

- `Coordinates` <= [8] Add output 0
- `TextureObject` <= [2] Input Diffuse (Texture2D) output 0

### [83] Append

- `A` <= [82] Texture Sample output 0
- `B` <= [82] Texture Sample output 4

### [84] Switch

- `A` <= [33] Texture Sample output 0
- `B` <= [85] Texture Sample output 0
- `Value` <= [80] Input useMipLevel (StaticBool) output 0

### [85] Texture Sample

- `Coordinates` <= [8] Add output 0
- `TextureObject` <= [28] Input Normal Map (Texture2D) output 0

### [86] Switch

- `A` <= [30] Append output 0
- `B` <= [87] Append output 0
- `Value` <= [80] Input useMipLevel (StaticBool) output 0

### [87] Append

- `A` <= [89] Texture Sample output 0
- `B` <= [89] Texture Sample output 4

### [88] Switch

- `A` <= [29] Texture Sample output 0
- `B` <= [90] Texture Sample output 0
- `Value` <= [80] Input useMipLevel (StaticBool) output 0

### [89] Texture Sample

- `Coordinates` <= [18] Add output 0
- `TextureObject` <= [2] Input Diffuse (Texture2D) output 0

### [90] Texture Sample

- `Coordinates` <= [18] Add output 0
- `TextureObject` <= [28] Input Normal Map (Texture2D) output 0

### [91] Lerp

- `A` <= [3] Add output 0
- `B` <= [8] Add output 0
- `Alpha` <= [11] LinearSine output 0

### [92] Output UVs

- `A` <= [91] Lerp output 0

## Connection List

- [55] Mask ( R G B ) output 0 -> Output Diffuse.A
- [18] Add output 0 -> Texture Sample.Coordinates
- [2] Input Diffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [76] Add output 0 -> Texture Sample.MipValue
- [65] Texture Object output 0 -> Input Diffuse (Texture2D).Preview
- [5] Input UVs (Vector2) output 0 -> Add.A
- [13] Multiply output 0 -> Add.B
- [6] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [8] Add output 0 -> Texture Sample.Coordinates
- [2] Input Diffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [76] Add output 0 -> Texture Sample.MipValue
- [14] Multiply output 0 -> Add.A
- [5] Input UVs (Vector2) output 0 -> Add.B
- [70] Add output 0 -> Input Time (see tooltip) (Scalar).Preview
- [3] Add output 0 -> Frac.Input
- [9] Input Time (see tooltip) (Scalar) output 0 -> Frac.Input
- [16] Frac output 0 -> Multiply.A
- [62] Multiply output 0 -> Multiply.B
- [62] Multiply output 0 -> Multiply.A
- [12] Frac output 0 -> Multiply.B
- [9] Input Time (see tooltip) (Scalar) output 0 -> Subtract(,0.5).A
- [15] Subtract(,0.5) output 0 -> Frac.Input
- [10] Frac output 0 -> Add.A
- [20] Input UV texture 2 offset (Vector2) output 0 -> Add.B
- [86] Switch output 0 -> Lerp.A
- [81] Switch output 0 -> Lerp.B
- [11] LinearSine output 0 -> Lerp.Alpha
- [14] Multiply output 0 -> Abs.Input
- [21] AddComponents output 0 -> Multiply.A
- [21] AddComponents output 0 -> Multiply.B
- [13] Multiply output 0 -> Abs.Input
- [24] AddComponents output 0 -> Multiply.A
- [24] AddComponents output 0 -> Multiply.B
- [7] Texture Sample output 0 -> Append.A
- [7] Texture Sample output 4 -> Append.B
- [64] Texture Object output 0 -> Input Normal Map (Texture2D).Preview
- [18] Add output 0 -> Texture Sample.Coordinates
- [28] Input Normal Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [76] Add output 0 -> Texture Sample.MipValue
- [1] Texture Sample output 0 -> Append.A
- [1] Texture Sample output 4 -> Append.B
- [56] Mask ( A ) output 0 -> Output Diffuse Alpha.A
- [34] Lerp output 0 -> Output Normal.A
- [8] Add output 0 -> Texture Sample.Coordinates
- [28] Input Normal Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [76] Add output 0 -> Texture Sample.MipValue
- [88] Switch output 0 -> Lerp.A
- [84] Switch output 0 -> Lerp.B
- [11] LinearSine output 0 -> Lerp.Alpha
- [62] Multiply output 0 -> Append.A
- [36] 1 output 0 -> Append.B
- [59] Append output 0 -> Output Distortion.A
- [35] Append output 0 -> Normalize.VectorInput
- [39] BlendAngleCorrectedNormals output 0 -> Add.A
- [41] 0,0,1 output 0 -> Add.B
- [40] Add output 0 -> Normalize.VectorInput
- [52] Multiply output 0 -> Add.A
- [43] 1,0,0 output 0 -> Add.B
- [54] Lerp(0,1,0.5) output 0 -> Append.A
- [47] 0 output 0 -> Append.B
- [52] Multiply output 0 -> Add.A
- [44] 0,1,0 output 0 -> Add.B
- [46] Append output 0 -> Multiply.A
- [53] 1 output 0 -> Multiply.B
- [19] Lerp output 0 -> Mask ( R G B ).Input
- [19] Lerp output 0 -> Mask ( A ).Input
- [26] Multiply output 0 -> Append.A
- [23] Multiply output 0 -> Append.B
- [57] Append output 0 -> Append.A
- [11] LinearSine output 0 -> Append.B
- [58] Append output 0 -> Append.A
- [60] Lerp output 0 -> Append.B
- [26] Multiply output 0 -> Lerp.A
- [23] Multiply output 0 -> Lerp.B
- [11] LinearSine output 0 -> Lerp.Alpha
- [4] Input Flow Vector Map (see tooltip) (Vector2) output 0 -> Multiply.A
- [63] -1,1 output 0 -> Multiply.B
- [73] Frac output 0 -> Texture Sample.Coordinates
- [71] Multiply(,0.25) output 0 -> Add.A
- [66] TimeWithSpeedVariable output 1 -> Add.B
- [69] Texture Sample output 1 -> Multiply(,0.25).A
- [68] TexCoord[0] output 0 -> Add.A
- [75] Multiply output 0 -> Add.B
- [72] Add output 0 -> Frac.Input
- [66] TimeWithSpeedVariable output 1 -> Multiply.A
- [74] 0.02 output 0 -> Multiply.B
- [61] ComputeMipLevel output 0 -> Add.A
- [77] Input Texture Mip Bias (Scalar) output 0 -> Add.B
- [79] Static Bool (True) output 0 -> Input useMipLevel (StaticBool).Preview
- [27] Append output 0 -> Switch.A
- [83] Append output 0 -> Switch.B
- [80] Input useMipLevel (StaticBool) output 0 -> Switch.Value
- [8] Add output 0 -> Texture Sample.Coordinates
- [2] Input Diffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [82] Texture Sample output 0 -> Append.A
- [82] Texture Sample output 4 -> Append.B
- [33] Texture Sample output 0 -> Switch.A
- [85] Texture Sample output 0 -> Switch.B
- [80] Input useMipLevel (StaticBool) output 0 -> Switch.Value
- [8] Add output 0 -> Texture Sample.Coordinates
- [28] Input Normal Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [30] Append output 0 -> Switch.A
- [87] Append output 0 -> Switch.B
- [80] Input useMipLevel (StaticBool) output 0 -> Switch.Value
- [89] Texture Sample output 0 -> Append.A
- [89] Texture Sample output 4 -> Append.B
- [29] Texture Sample output 0 -> Switch.A
- [90] Texture Sample output 0 -> Switch.B
- [80] Input useMipLevel (StaticBool) output 0 -> Switch.Value
- [18] Add output 0 -> Texture Sample.Coordinates
- [2] Input Diffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [18] Add output 0 -> Texture Sample.Coordinates
- [28] Input Normal Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [3] Add output 0 -> Lerp.A
- [8] Add output 0 -> Lerp.B
- [11] LinearSine output 0 -> Lerp.Alpha
- [91] Lerp output 0 -> Output UVs.A

## Output Trace Roots

- `Diffuse` is driven by [55] Mask ( R G B )
- `Diffuse Alpha` is driven by [56] Mask ( A )
- `Normal` is driven by [34] Lerp
- `Distortion` is driven by [59] Append
- `UVs` is driven by [91] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
