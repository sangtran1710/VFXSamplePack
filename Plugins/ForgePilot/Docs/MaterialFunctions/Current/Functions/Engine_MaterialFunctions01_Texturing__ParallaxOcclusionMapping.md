# ParallaxOcclusionMapping

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ParallaxOcclusionMapping.ParallaxOcclusionMapping`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Parallax Occlusion Mapping uses Ray Tracing to find the correct offsets at each point. Keep in mind that the material cost will not be easily trackable using the instruction count or shader complexity view due to the nature of the loops involved. Use the debug emissive colors to get an idea of expense based on your specified Min and Max Step sizes.

## Inputs

- `Min Steps` [FunctionInput_Scalar] - The Min steps is how many steps will be used when looking downward at the material. Typically fewer steps are required when looking down since there is less resulting offset.
- `Max Steps` [FunctionInput_Scalar] - The Max Steps is how many steps will be performed at glancing angles where more steps are typically necessary.
- `Height Ratio` [FunctionInput_Scalar] - This is how 'deep' the heightmap is in relation to its width. Typical values are between 0.05 and 0.1.
- `Heightmap Texture` [FunctionInput_Texture2D] - The Texture Object containing your Heightmap
- `UVs` [FunctionInput_Vector2] - The coordinates to use for mapping. Can be used with worldposition V2 values as well.
- `-----------------------------` [FunctionInput_StaticBool] - Blank spacer input does nothing
- `Specify Manual Texture Size` [FunctionInput_StaticBool] - Only used for Pixel Depth Offset. This lets you manually specify the World Size of your texture to skip some expensive instructions that are normally used to determine the UV to World Size.Defaults to False.
- `Manual Texture Size` [FunctionInput_Scalar] - Only used for Pixel Depth Offset. This lets you manually specify the World Size of your texture to skip some expensive instructions that are normally used to determine the UV to World Size.
- `------------------------------` [FunctionInput_StaticBool] - Blank spacer input does nothing
- `Render Shadows (Occlusion Mapping)` [FunctionInput_StaticBool] - By default Shadows are disabled for performance. This allows static switching to make the Parallax Only case much cheaper. Occlusion mapping always runs 1 extra step in order to fix an artifact caused by using variable View steps with Fixed shadow steps.
- `Shadow Steps` [FunctionInput_Scalar] - How many steps to use for the shadow occlusion pass.
- `Shadow Penumbra` [FunctionInput_Scalar] - Defines how hard or soft the shadows are. The number is not in degrees since that would cost slightly more to convert to degrees.
- `Light Vector` [FunctionInput_Vector3] - For the occlusion pass, the light vector needs to be specified. It is suggested to use a Material Parameter Collection to define the vector using a simple level or actor blueprint.
- `Heightmap Channel` [FunctionInput_Vector4] - Which Channel your heightmap texture uses. Uses Alpha channel by Default.
- `Use World Coordinates` [FunctionInput_StaticBool] - If using world coordinates as the UVs, the shader needs to skip the tangent->world transform. This saves a few instructions. Default = FALSE
- `Reference Plane` [FunctionInput_Scalar] - The relative point from which the parallax will be applied. When 1 or higher, all parallax will be downwards. When 0, all parallax will be upwards.
- `Transform To VertexNormal` [FunctionInput_StaticBool]

## Outputs

- `Parallax UVs` - This returns the input UVs with Parallax Offset. 
- `Pixel Depth Offset` - Pixel Depth Offset is used to give accurate intersection with other meshes as well as shadows cast by other meshes. Will cause a problem with Dynamic Shadows since the original depths will shadow the new offset depths.
- `Offset Only` - The offsets only from the parallax.
- `Shadow` - This is the shadow from the specified light vector. The color is black; to colorize add a color and clamp to 1 before multiplying by your basecolor. This shadow method will be obsolete once pixel depth offset works in the main dynamic shadow pass.
- `Material Complexity - Steps Debug` - Pixel Depth Offset is used to give accurate intersection with other meshes as well as shadows cast by other meshes. Will cause a problem with Dynamic Shadows since the original depths will shadow the new offset depths.
- `Tangent Light Vector` - The Light Vector Transformed into Tangent Space. Useful for custom effects using the lighting.
- `World Position` - World Position accounting for offset. Can be used to map virtual worldposition textures by subtracting this Z value as the vertex normal (or 0,0,1 for a floor).

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Parallax UVs | Parallax UVs |
| 1 | custom | `MaterialExpressionCustom` | Parallax Only Mapping |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 5 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 8 | expression | `MaterialExpressionAbs` | Abs |  |
| 9 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 10 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 12 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 14 | expression | `MaterialExpressionDivide` | Divide |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionFloor` | Floor |  |
| 17 | expression | `MaterialExpressionDDX` | DDX |  |
| 18 | expression | `MaterialExpressionDDY` | DDY |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Min Steps (Scalar) | Min Steps |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Max Steps (Scalar) | Max Steps |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Height Ratio (Scalar) | Height Ratio |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap Texture (Texture2D) | Heightmap Texture |
| 23 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 26 | expression | `MaterialExpressionDDX` | DDX |  |
| 27 | expression | `MaterialExpressionDDY` | DDY |  |
| 28 | expression | `MaterialExpressionDDX` | DDX |  |
| 29 | expression | `MaterialExpressionDDY` | DDY |  |
| 30 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 31 | expression | `MaterialExpressionDivide` | Divide |  |
| 32 | expression | `MaterialExpressionDivide` | Divide |  |
| 33 | expression | `MaterialExpressionMax` | Max |  |
| 34 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 35 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 36 | expression | `MaterialExpressionAbs` | Abs |  |
| 37 | expression | `MaterialExpressionAbs` | Abs |  |
| 38 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 39 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 40 | expression | `MaterialExpressionDistance` | Distance |  |
| 41 | expression | `MaterialExpressionConstant` | 0 |  |
| 42 | expression | `MaterialExpressionDivide` | Divide |  |
| 43 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Depth Offset | Pixel Depth Offset |
| 44 | expression | `MaterialExpressionAdd` | Add |  |
| 45 | function_output | `MaterialExpressionFunctionOutput` | Output Offset Only | Offset Only |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input ----------------------------- (StaticBool) | ----------------------------- |
| 47 | function_input | `MaterialExpressionFunctionInput` | Input Specify Manual Texture Size (StaticBool) | Specify Manual Texture Size |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input Manual Texture Size (Scalar) | Manual Texture Size |
| 49 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 50 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 51 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 52 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 53 | expression | `MaterialExpressionConstant` | 0 |  |
| 54 | expression | `MaterialExpressionConstant` | 0 |  |
| 55 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 56 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 57 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 58 | expression | `MaterialExpressionConstant` | 0 |  |
| 59 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 60 | expression | `MaterialExpressionConstant` | 0 |  |
| 61 | function_input | `MaterialExpressionFunctionInput` | Input ------------------------------ (StaticBool) | ------------------------------ |
| 62 | function_input | `MaterialExpressionFunctionInput` | Input Render Shadows (Occlusion Mapping) (StaticBool) | Render Shadows (Occlusion Mapping) |
| 63 | function_input | `MaterialExpressionFunctionInput` | Input Shadow Steps (Scalar) | Shadow Steps |
| 64 | function_input | `MaterialExpressionFunctionInput` | Input Shadow Penumbra (Scalar) | Shadow Penumbra |
| 65 | function_input | `MaterialExpressionFunctionInput` | Input Light Vector (Vector3) | Light Vector |
| 66 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 67 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 68 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 69 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 70 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 71 | expression | `MaterialExpressionAbs` | Abs |  |
| 72 | expression | `MaterialExpressionDivide` | Divide |  |
| 73 | custom | `MaterialExpressionCustom` | Parallax Occlusion Mapping |  |
| 74 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 75 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 76 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 77 | expression | `MaterialExpressionAppendVector` | Append |  |
| 78 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 79 | expression | `MaterialExpressionDivide` | Divide |  |
| 80 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 81 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 82 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 83 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 84 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 85 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 86 | expression | `MaterialExpressionConstant` | 1 |  |
| 87 | function_output | `MaterialExpressionFunctionOutput` | Output Shadow | Shadow |
| 88 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 89 | expression | `MaterialExpressionConstant` | 1 |  |
| 90 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 91 | expression | `MaterialExpressionConstant` | 1 |  |
| 92 | function_output | `MaterialExpressionFunctionOutput` | Output Material Complexity - Steps Debug | Material Complexity - Steps Debug |
| 93 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 94 | expression | `MaterialExpressionFloor` | Floor |  |
| 95 | expression | `MaterialExpressionDivide` | Divide |  |
| 96 | custom | `MaterialExpressionCustom` | ShaderComplexity |  |
| 97 | expression | `MaterialExpressionScalarParameter` | Param (32) 'Max Expensive Steps Debug Color' |  |
| 98 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 99 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 100 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 101 | expression | `MaterialExpressionAppendVector` | Append |  |
| 102 | function_output | `MaterialExpressionFunctionOutput` | Output Tangent Light Vector | Tangent Light Vector |
| 103 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap Channel (Vector4) | Heightmap Channel |
| 104 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 105 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 106 | function_output | `MaterialExpressionFunctionOutput` | Output World Position | World Position |
| 107 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 108 | function_input | `MaterialExpressionFunctionInput` | Input Use World Coordinates (StaticBool) | Use World Coordinates |
| 109 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 110 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 111 | function_input | `MaterialExpressionFunctionInput` | Input Reference Plane (Scalar) | Reference Plane |
| 112 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 113 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 114 | expression | `MaterialExpressionAdd` | Add |  |
| 115 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 116 | expression | `MaterialExpressionDivide` | Divide |  |
| 117 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 118 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 119 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 120 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 121 | expression | `MaterialExpressionClamp` | Clamp |  |
| 122 | function_call | `MaterialExpressionMaterialFunctionCall` | TransformToZVector |  |
| 123 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 124 | function_input | `MaterialExpressionFunctionInput` | Input Transform To VertexNormal (StaticBool) | Transform To VertexNormal |
| 125 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 126 | expression | `MaterialExpressionStaticSwitch` | Switch |  |

## Exact Input Wiring

### [0] Output Parallax UVs

- `A` <= [50] Feature Level Switch output 0

### [3] Dot

- `A` <= [4] Camera Vector output 0
- `B` <= [5] VertexNormalWS output 0

### [6] Lerp

- `A` <= [20] Input Max Steps (Scalar) output 0
- `B` <= [19] Input Min Steps (Scalar) output 0
- `Alpha` <= [121] Clamp output 0

### [7] Divide(1,)

- `B` <= [16] Floor output 0

### [8] Abs

- `Input` <= [3] Dot output 0

### [10] World Space to Tangent Space TransformVector

- `Input` <= [9] Camera Vector output 0

### [11] Mask ( R G )

- `Input` <= [110] Switch output 0

### [12] Mask ( B )

- `Input` <= [110] Switch output 0

### [13] Multiply(,-1)

- `A` <= [11] Mask ( R G ) output 0

### [14] Divide

- `A` <= [13] Multiply(,-1) output 0
- `B` <= [12] Mask ( B ) output 0

### [15] Multiply

- `A` <= [21] Input Height Ratio (Scalar) output 0
- `B` <= [14] Divide output 0

### [16] Floor

- `Input` <= [6] Lerp output 0

### [17] DDX

- `Value` <= [114] Add output 0

### [18] DDY

- `Value` <= [114] Add output 0

### [22] Input Heightmap Texture (Texture2D)

- `Preview` <= [23] Texture Object output 0

### [24] Input UVs (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [25] Mask ( R G )

- `Input` <= [82] Switch output 0

### [26] DDX

- `Value` <= [114] Add output 0

### [27] DDY

- `Value` <= [114] Add output 0

### [28] DDX

- `Value` <= [30] Absolute World Position output 0

### [29] DDY

- `Value` <= [30] Absolute World Position output 0

### [31] Divide

- `A` <= [38] VectorLength output 1
- `B` <= [34] VectorLength output 0

### [32] Divide

- `A` <= [39] VectorLength output 1
- `B` <= [35] VectorLength output 0

### [33] Max

- `A` <= [31] Divide output 0
- `B` <= [32] Divide output 0

### [36] Abs

- `Input` <= [26] DDX output 0

### [37] Abs

- `Input` <= [27] DDY output 0

### [40] Distance

- `A` <= [101] Append output 0
- `B` <= [41] 0 output 0

### [42] Divide

- `A` <= [40] Distance output 0
- `B` <= [72] Divide output 0

### [43] Output Pixel Depth Offset

- `A` <= [52] Feature Level Switch output 0

### [44] Add

- `A` <= [114] Add output 0
- `B` <= [25] Mask ( R G ) output 0

### [45] Output Offset Only

- `A` <= [51] Feature Level Switch output 0

### [47] Input Specify Manual Texture Size (StaticBool)

- `Preview` <= [66] Static Bool (False) output 0

### [49] Switch

- `A` <= [107] Multiply output 0
- `B` <= [42] Divide output 0
- `Value` <= [47] Input Specify Manual Texture Size (StaticBool) output 0

### [50] Feature Level Switch

- `Default` <= [56] Quality Switch output 0
- `Inputs` <= [55] Multiply(,1) output 0

### [51] Feature Level Switch

- `Default` <= [57] Quality Switch output 0
- `Inputs` <= [54] 0 output 0

### [52] Feature Level Switch

- `Default` <= [59] Quality Switch output 0
- `Inputs` <= [53] 0 output 0

### [55] Multiply(,1)

- `A` <= [114] Add output 0

### [56] Quality Switch

- `Default` <= [44] Add output 0
- `Inputs` <= [55] Multiply(,1) output 0

### [57] Quality Switch

- `Default` <= [25] Mask ( R G ) output 0
- `Inputs` <= [58] 0 output 0

### [59] Quality Switch

- `Default` <= [49] Switch output 0
- `Inputs` <= [60] 0 output 0

### [62] Input Render Shadows (Occlusion Mapping) (StaticBool)

- `Preview` <= [67] Static Bool (False) output 0

### [68] Dot

- `A` <= [70] CameraDirectionVector output 0
- `B` <= [69] Camera Vector output 0

### [71] Abs

- `Input` <= [68] Dot output 0

### [72] Divide

- `A` <= [33] Max output 0
- `B` <= [71] Abs output 0

### [74] World Space to Tangent Space TransformVector

- `Input` <= [80] Multiply(,-1) output 0

### [75] Mask ( R G )

- `Input` <= [74] World Space to Tangent Space TransformVector output 0

### [76] Mask ( B )

- `Input` <= [74] World Space to Tangent Space TransformVector output 0

### [77] Append

- `A` <= [75] Mask ( R G ) output 0
- `B` <= [79] Divide output 0

### [78] Normalize

- `VectorInput` <= [77] Append output 0

### [79] Divide

- `A` <= [76] Mask ( B ) output 0
- `B` <= [21] Input Height Ratio (Scalar) output 0

### [80] Multiply(,-1)

- `A` <= [65] Input Light Vector (Vector3) output 0

### [81] Divide(1,)

- `B` <= [6] Lerp output 0

### [82] Switch

- `A` <= [73] Parallax Occlusion Mapping output 0
- `B` <= [1] Parallax Only Mapping output 0
- `Value` <= [62] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0

### [83] Mask ( R G )

- `Input` <= [82] Switch output 0

### [84] Mask ( A )

- `Input` <= [82] Switch output 0

### [85] Switch

- `A` <= [84] Mask ( A ) output 0
- `B` <= [86] 1 output 0
- `Value` <= [62] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0

### [87] Output Shadow

- `A` <= [88] Feature Level Switch output 0

### [88] Feature Level Switch

- `Default` <= [90] Quality Switch output 0
- `Inputs` <= [89] 1 output 0

### [90] Quality Switch

- `Default` <= [85] Switch output 0
- `Inputs` <= [91] 1 output 0

### [92] Output Material Complexity - Steps Debug

- `A` <= [96] ShaderComplexity output 0

### [93] Multiply

- `A` <= [16] Floor output 0
- `B` <= [95] Divide output 0

### [94] Floor

- `Input` <= [93] Multiply output 0

### [95] Divide

- `A` <= [100] Multiply output 0
- `B` <= [21] Input Height Ratio (Scalar) output 0

### [98] Mask ( B )

- `Input` <= [82] Switch output 0

### [99] 1-x

- `Input` <= [98] Mask ( B ) output 0

### [100] Multiply

- `A` <= [99] 1-x output 0
- `B` <= [21] Input Height Ratio (Scalar) output 0

### [101] Append

- `A` <= [83] Mask ( R G ) output 0
- `B` <= [100] Multiply output 0

### [102] Output Tangent Light Vector

- `A` <= [74] World Space to Tangent Space TransformVector output 0

### [104] Multiply

- `A` <= [7] Divide(1,) output 0
- `B` <= [15] Multiply output 0

### [105] Multiply

- `A` <= [15] Multiply output 0
- `B` <= [81] Divide(1,) output 0

### [106] Output World Position

- `A` <= [118] Subtract output 0

### [107] Multiply

- `A` <= [40] Distance output 0
- `B` <= [48] Input Manual Texture Size (Scalar) output 0

### [108] Input Use World Coordinates (StaticBool)

- `Preview` <= [109] Static Bool (False) output 0

### [110] Switch

- `A` <= [126] Switch output 0
- `B` <= [10] World Space to Tangent Space TransformVector output 0
- `Value` <= [108] Input Use World Coordinates (StaticBool) output 0

### [112] Multiply

- `A` <= [15] Multiply output 0
- `B` <= [113] Multiply(,-1) output 0

### [113] Multiply(,-1)

- `A` <= [115] 1-x output 0

### [114] Add

- `A` <= [24] Input UVs (Vector2) output 0
- `B` <= [112] Multiply output 0

### [115] 1-x

- `Input` <= [111] Input Reference Plane (Scalar) output 0

### [116] Divide

- `A` <= [40] Distance output 0
- `B` <= [33] Max output 0

### [118] Subtract

- `A` <= [120] Absolute World Position output 0
- `B` <= [119] Multiply output 0

### [119] Multiply

- `A` <= [116] Divide output 0
- `B` <= [117] Camera Vector output 0

### [121] Clamp

- `Input` <= [8] Abs output 0

### [124] Input Transform To VertexNormal (StaticBool)

- `Preview` <= [125] Static Bool (True) output 0

### [126] Switch

- `A` <= [122] TransformToZVector output 0
- `B` <= [9] Camera Vector output 0
- `Value` <= [124] Input Transform To VertexNormal (StaticBool) output 0

## Connection List

- [50] Feature Level Switch output 0 -> Output Parallax UVs.A
- [4] Camera Vector output 0 -> Dot.A
- [5] VertexNormalWS output 0 -> Dot.B
- [20] Input Max Steps (Scalar) output 0 -> Lerp.A
- [19] Input Min Steps (Scalar) output 0 -> Lerp.B
- [121] Clamp output 0 -> Lerp.Alpha
- [16] Floor output 0 -> Divide(1,).B
- [3] Dot output 0 -> Abs.Input
- [9] Camera Vector output 0 -> World Space to Tangent Space TransformVector.Input
- [110] Switch output 0 -> Mask ( R G ).Input
- [110] Switch output 0 -> Mask ( B ).Input
- [11] Mask ( R G ) output 0 -> Multiply(,-1).A
- [13] Multiply(,-1) output 0 -> Divide.A
- [12] Mask ( B ) output 0 -> Divide.B
- [21] Input Height Ratio (Scalar) output 0 -> Multiply.A
- [14] Divide output 0 -> Multiply.B
- [6] Lerp output 0 -> Floor.Input
- [114] Add output 0 -> DDX.Value
- [114] Add output 0 -> DDY.Value
- [23] Texture Object output 0 -> Input Heightmap Texture (Texture2D).Preview
- [2] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [82] Switch output 0 -> Mask ( R G ).Input
- [114] Add output 0 -> DDX.Value
- [114] Add output 0 -> DDY.Value
- [30] Absolute World Position output 0 -> DDX.Value
- [30] Absolute World Position output 0 -> DDY.Value
- [38] VectorLength output 1 -> Divide.A
- [34] VectorLength output 0 -> Divide.B
- [39] VectorLength output 1 -> Divide.A
- [35] VectorLength output 0 -> Divide.B
- [31] Divide output 0 -> Max.A
- [32] Divide output 0 -> Max.B
- [26] DDX output 0 -> Abs.Input
- [27] DDY output 0 -> Abs.Input
- [101] Append output 0 -> Distance.A
- [41] 0 output 0 -> Distance.B
- [40] Distance output 0 -> Divide.A
- [72] Divide output 0 -> Divide.B
- [52] Feature Level Switch output 0 -> Output Pixel Depth Offset.A
- [114] Add output 0 -> Add.A
- [25] Mask ( R G ) output 0 -> Add.B
- [51] Feature Level Switch output 0 -> Output Offset Only.A
- [66] Static Bool (False) output 0 -> Input Specify Manual Texture Size (StaticBool).Preview
- [107] Multiply output 0 -> Switch.A
- [42] Divide output 0 -> Switch.B
- [47] Input Specify Manual Texture Size (StaticBool) output 0 -> Switch.Value
- [56] Quality Switch output 0 -> Feature Level Switch.Default
- [55] Multiply(,1) output 0 -> Feature Level Switch.Inputs
- [57] Quality Switch output 0 -> Feature Level Switch.Default
- [54] 0 output 0 -> Feature Level Switch.Inputs
- [59] Quality Switch output 0 -> Feature Level Switch.Default
- [53] 0 output 0 -> Feature Level Switch.Inputs
- [114] Add output 0 -> Multiply(,1).A
- [44] Add output 0 -> Quality Switch.Default
- [55] Multiply(,1) output 0 -> Quality Switch.Inputs
- [25] Mask ( R G ) output 0 -> Quality Switch.Default
- [58] 0 output 0 -> Quality Switch.Inputs
- [49] Switch output 0 -> Quality Switch.Default
- [60] 0 output 0 -> Quality Switch.Inputs
- [67] Static Bool (False) output 0 -> Input Render Shadows (Occlusion Mapping) (StaticBool).Preview
- [70] CameraDirectionVector output 0 -> Dot.A
- [69] Camera Vector output 0 -> Dot.B
- [68] Dot output 0 -> Abs.Input
- [33] Max output 0 -> Divide.A
- [71] Abs output 0 -> Divide.B
- [80] Multiply(,-1) output 0 -> World Space to Tangent Space TransformVector.Input
- [74] World Space to Tangent Space TransformVector output 0 -> Mask ( R G ).Input
- [74] World Space to Tangent Space TransformVector output 0 -> Mask ( B ).Input
- [75] Mask ( R G ) output 0 -> Append.A
- [79] Divide output 0 -> Append.B
- [77] Append output 0 -> Normalize.VectorInput
- [76] Mask ( B ) output 0 -> Divide.A
- [21] Input Height Ratio (Scalar) output 0 -> Divide.B
- [65] Input Light Vector (Vector3) output 0 -> Multiply(,-1).A
- [6] Lerp output 0 -> Divide(1,).B
- [73] Parallax Occlusion Mapping output 0 -> Switch.A
- [1] Parallax Only Mapping output 0 -> Switch.B
- [62] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0 -> Switch.Value
- [82] Switch output 0 -> Mask ( R G ).Input
- [82] Switch output 0 -> Mask ( A ).Input
- [84] Mask ( A ) output 0 -> Switch.A
- [86] 1 output 0 -> Switch.B
- [62] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0 -> Switch.Value
- [88] Feature Level Switch output 0 -> Output Shadow.A
- [90] Quality Switch output 0 -> Feature Level Switch.Default
- [89] 1 output 0 -> Feature Level Switch.Inputs
- [85] Switch output 0 -> Quality Switch.Default
- [91] 1 output 0 -> Quality Switch.Inputs
- [96] ShaderComplexity output 0 -> Output Material Complexity - Steps Debug.A
- [16] Floor output 0 -> Multiply.A
- [95] Divide output 0 -> Multiply.B
- [93] Multiply output 0 -> Floor.Input
- [100] Multiply output 0 -> Divide.A
- [21] Input Height Ratio (Scalar) output 0 -> Divide.B
- [82] Switch output 0 -> Mask ( B ).Input
- [98] Mask ( B ) output 0 -> 1-x.Input
- [99] 1-x output 0 -> Multiply.A
- [21] Input Height Ratio (Scalar) output 0 -> Multiply.B
- [83] Mask ( R G ) output 0 -> Append.A
- [100] Multiply output 0 -> Append.B
- [74] World Space to Tangent Space TransformVector output 0 -> Output Tangent Light Vector.A
- [7] Divide(1,) output 0 -> Multiply.A
- [15] Multiply output 0 -> Multiply.B
- [15] Multiply output 0 -> Multiply.A
- [81] Divide(1,) output 0 -> Multiply.B
- [118] Subtract output 0 -> Output World Position.A
- [40] Distance output 0 -> Multiply.A
- [48] Input Manual Texture Size (Scalar) output 0 -> Multiply.B
- [109] Static Bool (False) output 0 -> Input Use World Coordinates (StaticBool).Preview
- [126] Switch output 0 -> Switch.A
- [10] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [108] Input Use World Coordinates (StaticBool) output 0 -> Switch.Value
- [15] Multiply output 0 -> Multiply.A
- [113] Multiply(,-1) output 0 -> Multiply.B
- [115] 1-x output 0 -> Multiply(,-1).A
- [24] Input UVs (Vector2) output 0 -> Add.A
- [112] Multiply output 0 -> Add.B
- [111] Input Reference Plane (Scalar) output 0 -> 1-x.Input
- [40] Distance output 0 -> Divide.A
- [33] Max output 0 -> Divide.B
- [120] Absolute World Position output 0 -> Subtract.A
- [119] Multiply output 0 -> Subtract.B
- [116] Divide output 0 -> Multiply.A
- [117] Camera Vector output 0 -> Multiply.B
- [8] Abs output 0 -> Clamp.Input
- [125] Static Bool (True) output 0 -> Input Transform To VertexNormal (StaticBool).Preview
- [122] TransformToZVector output 0 -> Switch.A
- [9] Camera Vector output 0 -> Switch.B
- [124] Input Transform To VertexNormal (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `Parallax UVs` is driven by [50] Feature Level Switch
- `Pixel Depth Offset` is driven by [52] Feature Level Switch
- `Offset Only` is driven by [51] Feature Level Switch
- `Shadow` is driven by [88] Feature Level Switch
- `Material Complexity - Steps Debug` is driven by [96] ShaderComplexity
- `Tangent Light Vector` is driven by [74] World Space to Tangent Space TransformVector
- `World Position` is driven by [118] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


