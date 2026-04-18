# Parallax_For_Bomb

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/Parallax_For_Bomb.Parallax_For_Bomb`
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
- `Reference Plane` [FunctionInput_Scalar] - The relative point from which the parallax will be applied. When 1 or higher, all parallax will be downwards. When 0, all parallax will be upwards.
- `Override Camera Vector` [FunctionInput_Vector3] - When this pin is used, the "Use World Coordinates" option will be bypassed. This allows you to perform your own transform into an arbitrary basis for the inital camera trace. This was added mostly to add support for triplanar mapping and texture bombing.

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
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 12 | expression | `MaterialExpressionDivide` | Divide |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionFloor` | Floor |  |
| 15 | expression | `MaterialExpressionDDX` | DDX |  |
| 16 | expression | `MaterialExpressionDDY` | DDY |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Min Steps (Scalar) | Min Steps |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Max Steps (Scalar) | Max Steps |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Height Ratio (Scalar) | Height Ratio |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap Texture (Texture2D) | Heightmap Texture |
| 21 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 24 | expression | `MaterialExpressionDDX` | DDX |  |
| 25 | expression | `MaterialExpressionDDY` | DDY |  |
| 26 | expression | `MaterialExpressionDDX` | DDX |  |
| 27 | expression | `MaterialExpressionDDY` | DDY |  |
| 28 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 29 | expression | `MaterialExpressionDivide` | Divide |  |
| 30 | expression | `MaterialExpressionDivide` | Divide |  |
| 31 | expression | `MaterialExpressionMax` | Max |  |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 33 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 34 | expression | `MaterialExpressionAbs` | Abs |  |
| 35 | expression | `MaterialExpressionAbs` | Abs |  |
| 36 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 37 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 38 | expression | `MaterialExpressionDistance` | Distance |  |
| 39 | expression | `MaterialExpressionConstant` | 0 |  |
| 40 | expression | `MaterialExpressionDivide` | Divide |  |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Depth Offset | Pixel Depth Offset |
| 42 | expression | `MaterialExpressionAdd` | Add |  |
| 43 | function_output | `MaterialExpressionFunctionOutput` | Output Offset Only | Offset Only |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input ----------------------------- (StaticBool) | ----------------------------- |
| 45 | function_input | `MaterialExpressionFunctionInput` | Input Specify Manual Texture Size (StaticBool) | Specify Manual Texture Size |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Manual Texture Size (Scalar) | Manual Texture Size |
| 47 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 48 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 49 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 50 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 51 | expression | `MaterialExpressionConstant` | 0 |  |
| 52 | expression | `MaterialExpressionConstant` | 0 |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 54 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 55 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 56 | expression | `MaterialExpressionConstant` | 0 |  |
| 57 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 58 | expression | `MaterialExpressionConstant` | 0 |  |
| 59 | function_input | `MaterialExpressionFunctionInput` | Input ------------------------------ (StaticBool) | ------------------------------ |
| 60 | function_input | `MaterialExpressionFunctionInput` | Input Render Shadows (Occlusion Mapping) (StaticBool) | Render Shadows (Occlusion Mapping) |
| 61 | function_input | `MaterialExpressionFunctionInput` | Input Shadow Steps (Scalar) | Shadow Steps |
| 62 | function_input | `MaterialExpressionFunctionInput` | Input Shadow Penumbra (Scalar) | Shadow Penumbra |
| 63 | function_input | `MaterialExpressionFunctionInput` | Input Light Vector (Vector3) | Light Vector |
| 64 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 65 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 66 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 67 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 68 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 69 | expression | `MaterialExpressionAbs` | Abs |  |
| 70 | expression | `MaterialExpressionDivide` | Divide |  |
| 71 | custom | `MaterialExpressionCustom` | Parallax Occlusion Mapping |  |
| 72 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 73 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 74 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 75 | expression | `MaterialExpressionAppendVector` | Append |  |
| 76 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 77 | expression | `MaterialExpressionDivide` | Divide |  |
| 78 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 79 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 80 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 81 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 82 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 83 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 84 | expression | `MaterialExpressionConstant` | 1 |  |
| 85 | function_output | `MaterialExpressionFunctionOutput` | Output Shadow | Shadow |
| 86 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 87 | expression | `MaterialExpressionConstant` | 1 |  |
| 88 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 89 | expression | `MaterialExpressionConstant` | 1 |  |
| 90 | function_output | `MaterialExpressionFunctionOutput` | Output Material Complexity - Steps Debug | Material Complexity - Steps Debug |
| 91 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 92 | expression | `MaterialExpressionFloor` | Floor |  |
| 93 | expression | `MaterialExpressionDivide` | Divide |  |
| 94 | custom | `MaterialExpressionCustom` | ShaderComplexity |  |
| 95 | expression | `MaterialExpressionScalarParameter` | Param (32) 'Max Expensive Steps Debug Color' |  |
| 96 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 97 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 98 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 99 | expression | `MaterialExpressionAppendVector` | Append |  |
| 100 | function_output | `MaterialExpressionFunctionOutput` | Output Tangent Light Vector | Tangent Light Vector |
| 101 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap Channel (Vector4) | Heightmap Channel |
| 102 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 103 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 104 | function_output | `MaterialExpressionFunctionOutput` | Output World Position | World Position |
| 105 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 106 | function_input | `MaterialExpressionFunctionInput` | Input Reference Plane (Scalar) | Reference Plane |
| 107 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 108 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 109 | expression | `MaterialExpressionAdd` | Add |  |
| 110 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 111 | expression | `MaterialExpressionDivide` | Divide |  |
| 112 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 113 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 114 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 115 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 116 | function_input | `MaterialExpressionFunctionInput` | Input Override Camera Vector (Vector3) | Override Camera Vector |
| 117 | function_call | `MaterialExpressionMaterialFunctionCall` | ParallaxOcclusionMapping |  |
| 118 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 119 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 120 | expression | `MaterialExpressionSaturate` | Saturate |  |

## Exact Input Wiring

### [0] Output Parallax UVs

- `A` <= [42] Add output 0

### [3] Dot

- `A` <= [4] Camera Vector output 0
- `B` <= [5] VertexNormalWS output 0

### [6] Lerp

- `A` <= [18] Input Max Steps (Scalar) output 0
- `B` <= [17] Input Min Steps (Scalar) output 0
- `Alpha` <= [120] Saturate output 0

### [7] Divide(1,)

- `B` <= [14] Floor output 0

### [8] Abs

- `Input` <= [3] Dot output 0

### [9] Mask ( R G )

- `Input` <= [116] Input Override Camera Vector (Vector3) output 0

### [10] Mask ( B )

- `Input` <= [116] Input Override Camera Vector (Vector3) output 0

### [11] Multiply(,-1)

- `A` <= [9] Mask ( R G ) output 0

### [12] Divide

- `A` <= [11] Multiply(,-1) output 0
- `B` <= [10] Mask ( B ) output 0

### [13] Multiply

- `A` <= [19] Input Height Ratio (Scalar) output 0
- `B` <= [12] Divide output 0

### [14] Floor

- `Input` <= [6] Lerp output 0

### [15] DDX

- `Value` <= [109] Add output 0

### [16] DDY

- `Value` <= [109] Add output 0

### [20] Input Heightmap Texture (Texture2D)

- `Preview` <= [21] Texture Object output 0

### [22] Input UVs (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [23] Mask ( R G )

- `Input` <= [80] Switch output 0

### [24] DDX

- `Value` <= [109] Add output 0

### [25] DDY

- `Value` <= [109] Add output 0

### [26] DDX

- `Value` <= [28] Absolute World Position output 0

### [27] DDY

- `Value` <= [28] Absolute World Position output 0

### [29] Divide

- `A` <= [36] VectorLength output 1
- `B` <= [32] VectorLength output 0

### [30] Divide

- `A` <= [37] VectorLength output 1
- `B` <= [33] VectorLength output 0

### [31] Max

- `A` <= [29] Divide output 0
- `B` <= [30] Divide output 0

### [34] Abs

- `Input` <= [24] DDX output 0

### [35] Abs

- `Input` <= [25] DDY output 0

### [38] Distance

- `A` <= [99] Append output 0
- `B` <= [39] 0 output 0

### [40] Divide

- `A` <= [38] Distance output 0
- `B` <= [70] Divide output 0

### [41] Output Pixel Depth Offset

- `A` <= [50] Feature Level Switch output 0

### [42] Add

- `A` <= [109] Add output 0
- `B` <= [23] Mask ( R G ) output 0

### [43] Output Offset Only

- `A` <= [49] Feature Level Switch output 0

### [45] Input Specify Manual Texture Size (StaticBool)

- `Preview` <= [64] Static Bool (False) output 0

### [47] Switch

- `A` <= [105] Multiply output 0
- `B` <= [40] Divide output 0
- `Value` <= [45] Input Specify Manual Texture Size (StaticBool) output 0

### [48] Feature Level Switch

- `Default` <= [54] Quality Switch output 0
- `Inputs` <= [53] Multiply(,1) output 0

### [49] Feature Level Switch

- `Default` <= [55] Quality Switch output 0
- `Inputs` <= [52] 0 output 0

### [50] Feature Level Switch

- `Default` <= [57] Quality Switch output 0
- `Inputs` <= [51] 0 output 0

### [53] Multiply(,1)

- `A` <= [109] Add output 0

### [54] Quality Switch

- `Default` <= [42] Add output 0
- `Inputs` <= [53] Multiply(,1) output 0

### [55] Quality Switch

- `Default` <= [23] Mask ( R G ) output 0
- `Inputs` <= [56] 0 output 0

### [57] Quality Switch

- `Default` <= [47] Switch output 0
- `Inputs` <= [58] 0 output 0

### [60] Input Render Shadows (Occlusion Mapping) (StaticBool)

- `Preview` <= [65] Static Bool (False) output 0

### [66] Dot

- `A` <= [68] CameraDirectionVector output 0
- `B` <= [67] Camera Vector output 0

### [69] Abs

- `Input` <= [66] Dot output 0

### [70] Divide

- `A` <= [31] Max output 0
- `B` <= [69] Abs output 0

### [72] World Space to Tangent Space TransformVector

- `Input` <= [78] Multiply(,-1) output 0

### [73] Mask ( R G )

- `Input` <= [72] World Space to Tangent Space TransformVector output 0

### [74] Mask ( B )

- `Input` <= [72] World Space to Tangent Space TransformVector output 0

### [75] Append

- `A` <= [73] Mask ( R G ) output 0
- `B` <= [77] Divide output 0

### [76] Normalize

- `VectorInput` <= [75] Append output 0

### [77] Divide

- `A` <= [74] Mask ( B ) output 0
- `B` <= [19] Input Height Ratio (Scalar) output 0

### [78] Multiply(,-1)

- `A` <= [63] Input Light Vector (Vector3) output 0

### [79] Divide(1,)

- `B` <= [6] Lerp output 0

### [80] Switch

- `A` <= [71] Parallax Occlusion Mapping output 0
- `B` <= [1] Parallax Only Mapping output 0
- `Value` <= [60] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0

### [81] Mask ( R G )

- `Input` <= [80] Switch output 0

### [82] Mask ( A )

- `Input` <= [71] Parallax Occlusion Mapping output 0

### [83] Switch

- `A` <= [82] Mask ( A ) output 0
- `B` <= [84] 1 output 0
- `Value` <= [60] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0

### [85] Output Shadow

- `A` <= [86] Feature Level Switch output 0

### [86] Feature Level Switch

- `Default` <= [88] Quality Switch output 0
- `Inputs` <= [87] 1 output 0

### [88] Quality Switch

- `Default` <= [83] Switch output 0
- `Inputs` <= [89] 1 output 0

### [90] Output Material Complexity - Steps Debug

- `A` <= [94] ShaderComplexity output 0

### [91] Multiply

- `A` <= [14] Floor output 0
- `B` <= [93] Divide output 0

### [92] Floor

- `Input` <= [91] Multiply output 0

### [93] Divide

- `A` <= [98] Multiply output 0
- `B` <= [19] Input Height Ratio (Scalar) output 0

### [96] Mask ( B )

- `Input` <= [80] Switch output 0

### [97] 1-x

- `Input` <= [96] Mask ( B ) output 0

### [98] Multiply

- `A` <= [97] 1-x output 0
- `B` <= [19] Input Height Ratio (Scalar) output 0

### [99] Append

- `A` <= [81] Mask ( R G ) output 0
- `B` <= [98] Multiply output 0

### [100] Output Tangent Light Vector

- `A` <= [72] World Space to Tangent Space TransformVector output 0

### [102] Multiply

- `A` <= [7] Divide(1,) output 0
- `B` <= [13] Multiply output 0

### [103] Multiply

- `A` <= [13] Multiply output 0
- `B` <= [79] Divide(1,) output 0

### [104] Output World Position

- `A` <= [113] Subtract output 0

### [105] Multiply

- `A` <= [38] Distance output 0
- `B` <= [46] Input Manual Texture Size (Scalar) output 0

### [107] Multiply

- `A` <= [13] Multiply output 0
- `B` <= [108] Multiply(,-1) output 0

### [108] Multiply(,-1)

- `A` <= [110] 1-x output 0

### [109] Add

- `A` <= [22] Input UVs (Vector2) output 0
- `B` <= [107] Multiply output 0

### [110] 1-x

- `Input` <= [106] Input Reference Plane (Scalar) output 0

### [111] Divide

- `A` <= [38] Distance output 0
- `B` <= [31] Max output 0

### [113] Subtract

- `A` <= [115] Absolute World Position output 0
- `B` <= [114] Multiply output 0

### [114] Multiply

- `A` <= [111] Divide output 0
- `B` <= [112] Camera Vector output 0

### [118] World Space to Tangent Space TransformVector

- `Input` <= [119] Camera Vector output 0

### [120] Saturate

- `Input` <= [8] Abs output 0

## Connection List

- [42] Add output 0 -> Output Parallax UVs.A
- [4] Camera Vector output 0 -> Dot.A
- [5] VertexNormalWS output 0 -> Dot.B
- [18] Input Max Steps (Scalar) output 0 -> Lerp.A
- [17] Input Min Steps (Scalar) output 0 -> Lerp.B
- [120] Saturate output 0 -> Lerp.Alpha
- [14] Floor output 0 -> Divide(1,).B
- [3] Dot output 0 -> Abs.Input
- [116] Input Override Camera Vector (Vector3) output 0 -> Mask ( R G ).Input
- [116] Input Override Camera Vector (Vector3) output 0 -> Mask ( B ).Input
- [9] Mask ( R G ) output 0 -> Multiply(,-1).A
- [11] Multiply(,-1) output 0 -> Divide.A
- [10] Mask ( B ) output 0 -> Divide.B
- [19] Input Height Ratio (Scalar) output 0 -> Multiply.A
- [12] Divide output 0 -> Multiply.B
- [6] Lerp output 0 -> Floor.Input
- [109] Add output 0 -> DDX.Value
- [109] Add output 0 -> DDY.Value
- [21] Texture Object output 0 -> Input Heightmap Texture (Texture2D).Preview
- [2] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [80] Switch output 0 -> Mask ( R G ).Input
- [109] Add output 0 -> DDX.Value
- [109] Add output 0 -> DDY.Value
- [28] Absolute World Position output 0 -> DDX.Value
- [28] Absolute World Position output 0 -> DDY.Value
- [36] VectorLength output 1 -> Divide.A
- [32] VectorLength output 0 -> Divide.B
- [37] VectorLength output 1 -> Divide.A
- [33] VectorLength output 0 -> Divide.B
- [29] Divide output 0 -> Max.A
- [30] Divide output 0 -> Max.B
- [24] DDX output 0 -> Abs.Input
- [25] DDY output 0 -> Abs.Input
- [99] Append output 0 -> Distance.A
- [39] 0 output 0 -> Distance.B
- [38] Distance output 0 -> Divide.A
- [70] Divide output 0 -> Divide.B
- [50] Feature Level Switch output 0 -> Output Pixel Depth Offset.A
- [109] Add output 0 -> Add.A
- [23] Mask ( R G ) output 0 -> Add.B
- [49] Feature Level Switch output 0 -> Output Offset Only.A
- [64] Static Bool (False) output 0 -> Input Specify Manual Texture Size (StaticBool).Preview
- [105] Multiply output 0 -> Switch.A
- [40] Divide output 0 -> Switch.B
- [45] Input Specify Manual Texture Size (StaticBool) output 0 -> Switch.Value
- [54] Quality Switch output 0 -> Feature Level Switch.Default
- [53] Multiply(,1) output 0 -> Feature Level Switch.Inputs
- [55] Quality Switch output 0 -> Feature Level Switch.Default
- [52] 0 output 0 -> Feature Level Switch.Inputs
- [57] Quality Switch output 0 -> Feature Level Switch.Default
- [51] 0 output 0 -> Feature Level Switch.Inputs
- [109] Add output 0 -> Multiply(,1).A
- [42] Add output 0 -> Quality Switch.Default
- [53] Multiply(,1) output 0 -> Quality Switch.Inputs
- [23] Mask ( R G ) output 0 -> Quality Switch.Default
- [56] 0 output 0 -> Quality Switch.Inputs
- [47] Switch output 0 -> Quality Switch.Default
- [58] 0 output 0 -> Quality Switch.Inputs
- [65] Static Bool (False) output 0 -> Input Render Shadows (Occlusion Mapping) (StaticBool).Preview
- [68] CameraDirectionVector output 0 -> Dot.A
- [67] Camera Vector output 0 -> Dot.B
- [66] Dot output 0 -> Abs.Input
- [31] Max output 0 -> Divide.A
- [69] Abs output 0 -> Divide.B
- [78] Multiply(,-1) output 0 -> World Space to Tangent Space TransformVector.Input
- [72] World Space to Tangent Space TransformVector output 0 -> Mask ( R G ).Input
- [72] World Space to Tangent Space TransformVector output 0 -> Mask ( B ).Input
- [73] Mask ( R G ) output 0 -> Append.A
- [77] Divide output 0 -> Append.B
- [75] Append output 0 -> Normalize.VectorInput
- [74] Mask ( B ) output 0 -> Divide.A
- [19] Input Height Ratio (Scalar) output 0 -> Divide.B
- [63] Input Light Vector (Vector3) output 0 -> Multiply(,-1).A
- [6] Lerp output 0 -> Divide(1,).B
- [71] Parallax Occlusion Mapping output 0 -> Switch.A
- [1] Parallax Only Mapping output 0 -> Switch.B
- [60] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0 -> Switch.Value
- [80] Switch output 0 -> Mask ( R G ).Input
- [71] Parallax Occlusion Mapping output 0 -> Mask ( A ).Input
- [82] Mask ( A ) output 0 -> Switch.A
- [84] 1 output 0 -> Switch.B
- [60] Input Render Shadows (Occlusion Mapping) (StaticBool) output 0 -> Switch.Value
- [86] Feature Level Switch output 0 -> Output Shadow.A
- [88] Quality Switch output 0 -> Feature Level Switch.Default
- [87] 1 output 0 -> Feature Level Switch.Inputs
- [83] Switch output 0 -> Quality Switch.Default
- [89] 1 output 0 -> Quality Switch.Inputs
- [94] ShaderComplexity output 0 -> Output Material Complexity - Steps Debug.A
- [14] Floor output 0 -> Multiply.A
- [93] Divide output 0 -> Multiply.B
- [91] Multiply output 0 -> Floor.Input
- [98] Multiply output 0 -> Divide.A
- [19] Input Height Ratio (Scalar) output 0 -> Divide.B
- [80] Switch output 0 -> Mask ( B ).Input
- [96] Mask ( B ) output 0 -> 1-x.Input
- [97] 1-x output 0 -> Multiply.A
- [19] Input Height Ratio (Scalar) output 0 -> Multiply.B
- [81] Mask ( R G ) output 0 -> Append.A
- [98] Multiply output 0 -> Append.B
- [72] World Space to Tangent Space TransformVector output 0 -> Output Tangent Light Vector.A
- [7] Divide(1,) output 0 -> Multiply.A
- [13] Multiply output 0 -> Multiply.B
- [13] Multiply output 0 -> Multiply.A
- [79] Divide(1,) output 0 -> Multiply.B
- [113] Subtract output 0 -> Output World Position.A
- [38] Distance output 0 -> Multiply.A
- [46] Input Manual Texture Size (Scalar) output 0 -> Multiply.B
- [13] Multiply output 0 -> Multiply.A
- [108] Multiply(,-1) output 0 -> Multiply.B
- [110] 1-x output 0 -> Multiply(,-1).A
- [22] Input UVs (Vector2) output 0 -> Add.A
- [107] Multiply output 0 -> Add.B
- [106] Input Reference Plane (Scalar) output 0 -> 1-x.Input
- [38] Distance output 0 -> Divide.A
- [31] Max output 0 -> Divide.B
- [115] Absolute World Position output 0 -> Subtract.A
- [114] Multiply output 0 -> Subtract.B
- [111] Divide output 0 -> Multiply.A
- [112] Camera Vector output 0 -> Multiply.B
- [119] Camera Vector output 0 -> World Space to Tangent Space TransformVector.Input
- [8] Abs output 0 -> Saturate.Input

## Output Trace Roots

- `Parallax UVs` is driven by [42] Add
- `Pixel Depth Offset` is driven by [50] Feature Level Switch
- `Offset Only` is driven by [49] Feature Level Switch
- `Shadow` is driven by [86] Feature Level Switch
- `Material Complexity - Steps Debug` is driven by [94] ShaderComplexity
- `Tangent Light Vector` is driven by [72] World Space to Tangent Space TransformVector
- `World Position` is driven by [113] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
