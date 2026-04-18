# FlowMaps_Simple

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlowMaps_Simple.FlowMaps_Simple`
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

## Outputs

- `Diffuse`
- `Diffuse Alpha`
- `Normal`
- `Distortion` - R= amount of distortion for lerp sample A. G= distortion amount for Lerp input B.  B= Lerp Alpha. A= Lerped Distortion amounts. This is useful for modulating normals by the amount of movement that occurs in the texture. I.e. Lerp ((0,0,1), (This nodes normal output), (This nodes distortions alpha channel)) to only display the normals where the waves are currently highest.

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
| 61 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 62 | expression | `MaterialExpressionConstant2Vector` | -1,1 |  |
| 63 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 64 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 65 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 66 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 67 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 68 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 69 | expression | `MaterialExpressionAdd` | Add |  |
| 70 | expression | `MaterialExpressionMultiply` | Multiply(,0.25) |  |
| 71 | expression | `MaterialExpressionAdd` | Add |  |
| 72 | expression | `MaterialExpressionFrac` | Frac |  |
| 73 | expression | `MaterialExpressionConstant` | 0.02 |  |
| 74 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Diffuse

- `A` <= [55] Mask ( R G B ) output 0

### [1] Texture Sample

- `Coordinates` <= [18] Add output 0
- `TextureObject` <= [2] Input Diffuse (Texture2D) output 0

### [2] Input Diffuse (Texture2D)

- `Preview` <= [64] Texture Object output 0

### [3] Add

- `A` <= [5] Input UVs (Vector2) output 0
- `B` <= [13] Multiply output 0

### [5] Input UVs (Vector2)

- `Preview` <= [6] TexCoord[0] output 0

### [7] Texture Sample

- `Coordinates` <= [8] Add output 0
- `TextureObject` <= [2] Input Diffuse (Texture2D) output 0

### [8] Add

- `A` <= [14] Multiply output 0
- `B` <= [5] Input UVs (Vector2) output 0

### [9] Input Time (see tooltip) (Scalar)

- `Preview` <= [69] Add output 0

### [10] Frac

- `Input` <= [3] Add output 0

### [12] Frac

- `Input` <= [9] Input Time (see tooltip) (Scalar) output 0

### [13] Multiply

- `A` <= [16] Frac output 0
- `B` <= [61] Multiply output 0

### [14] Multiply

- `A` <= [61] Multiply output 0
- `B` <= [12] Frac output 0

### [15] Subtract(,0.5)

- `A` <= [9] Input Time (see tooltip) (Scalar) output 0

### [16] Frac

- `Input` <= [15] Subtract(,0.5) output 0

### [18] Add

- `A` <= [3] Add output 0
- `B` <= [20] Input UV texture 2 offset (Vector2) output 0

### [19] Lerp

- `A` <= [30] Append output 0
- `B` <= [27] Append output 0
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

- `Preview` <= [63] Texture Object output 0

### [29] Texture Sample

- `Coordinates` <= [18] Add output 0
- `TextureObject` <= [28] Input Normal Map (Texture2D) output 0

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

### [34] Lerp

- `A` <= [29] Texture Sample output 0
- `B` <= [33] Texture Sample output 0
- `Alpha` <= [11] LinearSine output 0

### [35] Append

- `A` <= [61] Multiply output 0
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

### [61] Multiply

- `A` <= [4] Input Flow Vector Map (see tooltip) (Vector2) output 0
- `B` <= [62] -1,1 output 0

### [68] Texture Sample

- `Coordinates` <= [72] Frac output 0

### [69] Add

- `A` <= [70] Multiply(,0.25) output 0
- `B` <= [65] TimeWithSpeedVariable output 1

### [70] Multiply(,0.25)

- `A` <= [68] Texture Sample output 1

### [71] Add

- `A` <= [67] TexCoord[0] output 0
- `B` <= [74] Multiply output 0

### [72] Frac

- `Input` <= [71] Add output 0

### [74] Multiply

- `A` <= [65] TimeWithSpeedVariable output 1
- `B` <= [73] 0.02 output 0

## Connection List

- [55] Mask ( R G B ) output 0 -> Output Diffuse.A
- [18] Add output 0 -> Texture Sample.Coordinates
- [2] Input Diffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [64] Texture Object output 0 -> Input Diffuse (Texture2D).Preview
- [5] Input UVs (Vector2) output 0 -> Add.A
- [13] Multiply output 0 -> Add.B
- [6] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [8] Add output 0 -> Texture Sample.Coordinates
- [2] Input Diffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [14] Multiply output 0 -> Add.A
- [5] Input UVs (Vector2) output 0 -> Add.B
- [69] Add output 0 -> Input Time (see tooltip) (Scalar).Preview
- [3] Add output 0 -> Frac.Input
- [9] Input Time (see tooltip) (Scalar) output 0 -> Frac.Input
- [16] Frac output 0 -> Multiply.A
- [61] Multiply output 0 -> Multiply.B
- [61] Multiply output 0 -> Multiply.A
- [12] Frac output 0 -> Multiply.B
- [9] Input Time (see tooltip) (Scalar) output 0 -> Subtract(,0.5).A
- [15] Subtract(,0.5) output 0 -> Frac.Input
- [3] Add output 0 -> Add.A
- [20] Input UV texture 2 offset (Vector2) output 0 -> Add.B
- [30] Append output 0 -> Lerp.A
- [27] Append output 0 -> Lerp.B
- [11] LinearSine output 0 -> Lerp.Alpha
- [14] Multiply output 0 -> Abs.Input
- [21] AddComponents output 0 -> Multiply.A
- [21] AddComponents output 0 -> Multiply.B
- [13] Multiply output 0 -> Abs.Input
- [24] AddComponents output 0 -> Multiply.A
- [24] AddComponents output 0 -> Multiply.B
- [7] Texture Sample output 0 -> Append.A
- [7] Texture Sample output 4 -> Append.B
- [63] Texture Object output 0 -> Input Normal Map (Texture2D).Preview
- [18] Add output 0 -> Texture Sample.Coordinates
- [28] Input Normal Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [1] Texture Sample output 0 -> Append.A
- [1] Texture Sample output 4 -> Append.B
- [56] Mask ( A ) output 0 -> Output Diffuse Alpha.A
- [34] Lerp output 0 -> Output Normal.A
- [8] Add output 0 -> Texture Sample.Coordinates
- [28] Input Normal Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [29] Texture Sample output 0 -> Lerp.A
- [33] Texture Sample output 0 -> Lerp.B
- [11] LinearSine output 0 -> Lerp.Alpha
- [61] Multiply output 0 -> Append.A
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
- [62] -1,1 output 0 -> Multiply.B
- [72] Frac output 0 -> Texture Sample.Coordinates
- [70] Multiply(,0.25) output 0 -> Add.A
- [65] TimeWithSpeedVariable output 1 -> Add.B
- [68] Texture Sample output 1 -> Multiply(,0.25).A
- [67] TexCoord[0] output 0 -> Add.A
- [74] Multiply output 0 -> Add.B
- [71] Add output 0 -> Frac.Input
- [65] TimeWithSpeedVariable output 1 -> Multiply.A
- [73] 0.02 output 0 -> Multiply.B

## Output Trace Roots

- `Diffuse` is driven by [55] Mask ( R G B )
- `Diffuse Alpha` is driven by [56] Mask ( A )
- `Normal` is driven by [34] Lerp
- `Distortion` is driven by [59] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


