# WorldAlignedTextures_Complex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/WorldAlignedTextures_Complex.WorldAlignedTextures_Complex`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Tiles a texture in worldspace

## Inputs

- `World Space Normal` [FunctionInput_Vector3]
- `Diffuse Texture Object` [FunctionInput_Texture2D]
- `TextureSize` [FunctionInput_Vector3] - Size of texture in worldspace units
- `WorldPosition` [FunctionInput_Vector3]
- `Export Float 4 Diffuse` [FunctionInput_StaticBool] - Default = False
- `ProjectionTransitionContrast` [FunctionInput_Scalar]
- `Use Texture Alpha Heightmap to Refine the Transistion` [FunctionInput_StaticBool] - Defaults to false.
- `Texture Size Z projection` [FunctionInput_Vector3] - Size of texture in worldspace units
- `Use seperate settings for Z projection` [FunctionInput_StaticBool]
- `Z Plane Diffuse Texture Object` [FunctionInput_Texture2D]
- `Use seperate texture for Z projection` [FunctionInput_StaticBool]
- `World Space Normals` [FunctionInput_StaticBool] - This switch defaults to true. If left true ensure that tangent space normals is turned off in the shaders properties panel. 
- `NormalTextureObject` [FunctionInput_Texture2D]
- `HeightLerpTransitionContrast` [FunctionInput_Scalar]
- `--------------------------` [FunctionInput_StaticBool] - -------------
- `--------------------------  ` [FunctionInput_StaticBool] - -------------
- `--------------------------       ` [FunctionInput_StaticBool] - -------------
- `--------------------------     ` [FunctionInput_StaticBool] - -------------
- `--------------------------      ` [FunctionInput_StaticBool] - -------------
- `PickTheDiffuseHeightChannel` [FunctionInput_StaticBool] - Default = False
- `TheDiffuseHeightChannel` [FunctionInput_Vector4] - (If pick the diffuse height channel == true)  (1,0,0,0) will use the diffuse red channel as the height map. (0,1,0,0) will use the green channel etc.
- `Spec Texture Object` [FunctionInput_Texture2D] - The spec texture is unpacked as Red channel = specular, Green channel = Roughness, Blue Channel = Metallic when referencing the Material Attribute outputs.
- `Export Float 4 Spec` [FunctionInput_StaticBool] - Default = False
- `Z Plane Spec Texture Object` [FunctionInput_Texture2D] - The spec texture is unpacked as Red channel = specular, Green channel = Roughness, Blue Channel = Metallic when referencing the Material Attribute outputs.

## Outputs

- `XYZ Diffuse Texture`
- `XY Diffuse Texture`
- `Z Diffuse Texture`
- `Normal Z Texture`
- `Normal XY Texture`
- `Normal XYZ Texture`
- `Normal XYZ Flat Top`
- `-------------------------- `
- `XYZ Spec`
- `XY Spec`
- `Z Spec`
- ` --------------------------`
- `XYZ Mat Attributes - See Tooltip` - The spec texture is unpacked as Red channel = specular, Green channel = Roughness, Blue Channel = Metallic. Diffuse alpha is in the opacity output if export float 4 is set to true.
- `XY Mat Attributes - See Tooltip` - The spec texture is unpacked as Red channel = specular, Green channel = Roughness, Blue Channel = Metallic. Diffuse alpha is in the opacity output if export float 4 is set to true.
- `Z Mat Attributes - See Tooltip` - The spec texture is unpacked as Red channel = specular, Green channel = Roughness, Blue Channel = Metallic. Diffuse alpha is in the opacity output if export float 4 is set to true.
- `  --------------------------`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Diffuse Texture | XYZ Diffuse Texture |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 3 | expression | `MaterialExpressionAbs` | Abs |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input World Space Normal (Vector3) | World Space Normal |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse Texture Object (Texture2D) | Diffuse Texture Object |
| 7 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (Vector3) | TextureSize |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 12 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 13 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 15 | expression | `MaterialExpressionAbs` | Abs |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 17 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 19 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output XY Diffuse Texture | XY Diffuse Texture |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Z Diffuse Texture | Z Diffuse Texture |
| 23 | expression | `MaterialExpressionAppendVector` | Append |  |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 26 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 27 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Export Float 4 Diffuse (StaticBool) | Export Float 4 Diffuse |
| 29 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 30 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 32 | function_input | `MaterialExpressionFunctionInput` | Input ProjectionTransitionContrast (Scalar) | ProjectionTransitionContrast |
| 33 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 35 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 36 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 37 | function_input | `MaterialExpressionFunctionInput` | Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) | Use Texture Alpha Heightmap to Refine the Transistion |
| 38 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 39 | expression | `MaterialExpressionDivide` | Divide |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input Texture Size Z projection (Vector3) | Texture Size Z projection |
| 42 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 43 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input Use seperate settings for Z projection (StaticBool) | Use seperate settings for Z projection |
| 45 | function_input | `MaterialExpressionFunctionInput` | Input Z Plane Diffuse Texture Object (Texture2D) | Z Plane Diffuse Texture Object |
| 46 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 47 | function_input | `MaterialExpressionFunctionInput` | Input Use seperate texture for Z projection (StaticBool) | Use seperate texture for Z projection |
| 48 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 49 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 50 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 51 | expression | `MaterialExpressionAppendVector` | Append |  |
| 52 | expression | `MaterialExpressionAppendVector` | Append |  |
| 53 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 54 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input World Space Normals (StaticBool) | World Space Normals |
| 56 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 57 | function_input | `MaterialExpressionFunctionInput` | Input NormalTextureObject (Texture2D) | NormalTextureObject |
| 58 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 59 | function_output | `MaterialExpressionFunctionOutput` | Output Normal Z Texture | Normal Z Texture |
| 60 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XY Texture | Normal XY Texture |
| 61 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XYZ Texture | Normal XYZ Texture |
| 62 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XYZ Flat Top | Normal XYZ Flat Top |
| 63 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 64 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 65 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldAlignedNormals_HighQuality |  |
| 66 | expression | `MaterialExpressionAbs` | Abs |  |
| 67 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 68 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 69 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 70 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 71 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 72 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 73 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 74 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 75 | function_input | `MaterialExpressionFunctionInput` | Input HeightLerpTransitionContrast (Scalar) | HeightLerpTransitionContrast |
| 76 | function_input | `MaterialExpressionFunctionInput` | Input -------------------------- (StaticBool) | -------------------------- |
| 77 | function_input | `MaterialExpressionFunctionInput` | Input --------------------------   (StaticBool) | --------------------------   |
| 78 | function_input | `MaterialExpressionFunctionInput` | Input --------------------------        (StaticBool) | --------------------------        |
| 79 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerpWithTwoHeightMaps |  |
| 80 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerpWithTwoHeightMaps |  |
| 81 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 82 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 83 | function_output | `MaterialExpressionFunctionOutput` | Output --------------------------  | --------------------------  |
| 84 | function_input | `MaterialExpressionFunctionInput` | Input --------------------------      (StaticBool) | --------------------------      |
| 85 | function_input | `MaterialExpressionFunctionInput` | Input --------------------------       (StaticBool) | --------------------------       |
| 86 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 87 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 88 | function_input | `MaterialExpressionFunctionInput` | Input PickTheDiffuseHeightChannel (StaticBool) | PickTheDiffuseHeightChannel |
| 89 | function_input | `MaterialExpressionFunctionInput` | Input TheDiffuseHeightChannel (Vector4) | TheDiffuseHeightChannel |
| 90 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 91 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 92 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 93 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 94 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 95 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Spec | XYZ Spec |
| 96 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 97 | function_input | `MaterialExpressionFunctionInput` | Input Spec Texture Object (Texture2D) | Spec Texture Object |
| 98 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 99 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 100 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 101 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 102 | function_output | `MaterialExpressionFunctionOutput` | Output XY Spec | XY Spec |
| 103 | function_output | `MaterialExpressionFunctionOutput` | Output Z Spec | Z Spec |
| 104 | expression | `MaterialExpressionAppendVector` | Append |  |
| 105 | expression | `MaterialExpressionAppendVector` | Append |  |
| 106 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 107 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 108 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 109 | function_input | `MaterialExpressionFunctionInput` | Input Export Float 4 Spec (StaticBool) | Export Float 4 Spec |
| 110 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 111 | function_input | `MaterialExpressionFunctionInput` | Input Z Plane Spec Texture Object (Texture2D) | Z Plane Spec Texture Object |
| 112 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 113 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 114 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 115 | expression | `MaterialExpressionAppendVector` | Append |  |
| 116 | expression | `MaterialExpressionAppendVector` | Append |  |
| 117 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 118 | function_output | `MaterialExpressionFunctionOutput` | Output  -------------------------- |  -------------------------- |
| 119 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Mat Attributes - See Tooltip | XYZ Mat Attributes - See Tooltip |
| 120 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 121 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 122 | function_output | `MaterialExpressionFunctionOutput` | Output XY Mat Attributes - See Tooltip | XY Mat Attributes - See Tooltip |
| 123 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 124 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 125 | function_output | `MaterialExpressionFunctionOutput` | Output Z Mat Attributes - See Tooltip | Z Mat Attributes - See Tooltip |
| 126 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 127 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 128 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 129 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 130 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 131 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 132 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 133 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 134 | expression | `MaterialExpressionConstant` | 0 |  |
| 135 | function_output | `MaterialExpressionFunctionOutput` | Output   -------------------------- |   -------------------------- |

## Exact Input Wiring

### [0] Output XYZ Diffuse Texture

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [13] Lerp output 0
- `B` <= [27] Switch output 0
- `Alpha` <= [38] Switch output 0

### [3] Abs

- `Input` <= [4] Mask ( B ) output 0

### [4] Mask ( B )

- `Input` <= [5] Input World Space Normal (Vector3) output 0

### [5] Input World Space Normal (Vector3)

- `Preview` <= [33] VertexNormalWS output 0

### [6] Input Diffuse Texture Object (Texture2D)

- `Preview` <= [7] Texture Object output 0

### [8] Mask ( R G )

- `Input` <= [43] Switch output 0

### [9] Divide

- `A` <= [11] Input WorldPosition (Vector3) output 0
- `B` <= [34] Multiply(,-1) output 0

### [11] Input WorldPosition (Vector3)

- `Preview` <= [12] Absolute World Position output 0

### [13] Lerp

- `A` <= [26] Switch output 0
- `B` <= [25] Switch output 0
- `Alpha` <= [35] Switch output 0

### [15] Abs

- `Input` <= [16] Mask ( R ) output 0

### [16] Mask ( R )

- `Input` <= [5] Input World Space Normal (Vector3) output 0

### [17] Texture Sample

- `Coordinates` <= [18] Mask ( G B ) output 0
- `TextureObject` <= [6] Input Diffuse Texture Object (Texture2D) output 0

### [18] Mask ( G B )

- `Input` <= [9] Divide output 0

### [19] Texture Sample

- `Coordinates` <= [20] Mask ( R B ) output 0
- `TextureObject` <= [6] Input Diffuse Texture Object (Texture2D) output 0

### [20] Mask ( R B )

- `Input` <= [9] Divide output 0

### [21] Output XY Diffuse Texture

- `A` <= [13] Lerp output 0

### [22] Output Z Diffuse Texture

- `A` <= [27] Switch output 0

### [23] Append

- `A` <= [17] Texture Sample output 0
- `B` <= [17] Texture Sample output 4

### [24] Append

- `A` <= [19] Texture Sample output 0
- `B` <= [19] Texture Sample output 4

### [25] Switch

- `A` <= [23] Append output 0
- `B` <= [17] Texture Sample output 0
- `Value` <= [28] Input Export Float 4 Diffuse (StaticBool) output 0

### [26] Switch

- `A` <= [24] Append output 0
- `B` <= [19] Texture Sample output 0
- `Value` <= [28] Input Export Float 4 Diffuse (StaticBool) output 0

### [27] Switch

- `A` <= [46] Switch output 0
- `B` <= [53] Mask ( R G B ) output 0
- `Value` <= [28] Input Export Float 4 Diffuse (StaticBool) output 0

### [28] Input Export Float 4 Diffuse (StaticBool)

- `Preview` <= [29] Static Bool (False) output 0

### [30] Mask ( R )

- `Input` <= [14] CheapContrast output 0

### [31] Mask ( R )

- `Input` <= [2] CheapContrast output 0

### [34] Multiply(,-1)

- `A` <= [66] Abs output 0

### [35] Switch

- `A` <= [81] 1-x output 0
- `B` <= [30] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool)

- `Preview` <= [36] Static Bool (False) output 0

### [38] Switch

- `A` <= [82] 1-x output 0
- `B` <= [31] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [39] Divide

- `A` <= [11] Input WorldPosition (Vector3) output 0
- `B` <= [40] Multiply(,-1) output 0

### [40] Multiply(,-1)

- `A` <= [41] Input Texture Size Z projection (Vector3) output 0

### [43] Switch

- `A` <= [39] Divide output 0
- `B` <= [9] Divide output 0
- `Value` <= [44] Input Use seperate settings for Z projection (StaticBool) output 0

### [44] Input Use seperate settings for Z projection (StaticBool)

- `Preview` <= [42] Static Bool (False) output 0

### [45] Input Z Plane Diffuse Texture Object (Texture2D)

- `Preview` <= [7] Texture Object output 0

### [46] Switch

- `A` <= [52] Append output 0
- `B` <= [51] Append output 0
- `Value` <= [47] Input Use seperate texture for Z projection (StaticBool) output 0

### [47] Input Use seperate texture for Z projection (StaticBool)

- `Preview` <= [48] Static Bool (False) output 0

### [49] Texture Sample

- `Coordinates` <= [8] Mask ( R G ) output 0
- `TextureObject` <= [6] Input Diffuse Texture Object (Texture2D) output 0

### [50] Texture Sample

- `Coordinates` <= [8] Mask ( R G ) output 0
- `TextureObject` <= [45] Input Z Plane Diffuse Texture Object (Texture2D) output 0

### [51] Append

- `A` <= [49] Texture Sample output 0
- `B` <= [49] Texture Sample output 4

### [52] Append

- `A` <= [50] Texture Sample output 0
- `B` <= [50] Texture Sample output 4

### [53] Mask ( R G B )

- `Input` <= [46] Switch output 0

### [54] Mask ( A )

- `Input` <= [46] Switch output 0

### [55] Input World Space Normals (StaticBool)

- `Preview` <= [56] Static Bool (True) output 0

### [57] Input NormalTextureObject (Texture2D)

- `Preview` <= [58] Texture Object output 0

### [59] Output Normal Z Texture

- `A` <= [67] Switch output 0

### [60] Output Normal XY Texture

- `A` <= [69] Switch output 0

### [61] Output Normal XYZ Texture

- `A` <= [73] Switch output 0

### [62] Output Normal XYZ Flat Top

- `A` <= [71] Switch output 0

### [63] Switch

- `A` <= [82] 1-x output 0
- `B` <= [31] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [64] Switch

- `A` <= [81] 1-x output 0
- `B` <= [30] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [66] Abs

- `Input` <= [10] Input TextureSize (Vector3) output 0

### [67] Switch

- `A` <= [65] WorldAlignedNormals_HighQuality output 3
- `B` <= [68] World Space to Tangent Space TransformVector output 0
- `Value` <= [55] Input World Space Normals (StaticBool) output 0

### [68] World Space to Tangent Space TransformVector

- `Input` <= [65] WorldAlignedNormals_HighQuality output 3

### [69] Switch

- `A` <= [65] WorldAlignedNormals_HighQuality output 2
- `B` <= [70] World Space to Tangent Space TransformVector output 0
- `Value` <= [55] Input World Space Normals (StaticBool) output 0

### [70] World Space to Tangent Space TransformVector

- `Input` <= [65] WorldAlignedNormals_HighQuality output 2

### [71] Switch

- `A` <= [65] WorldAlignedNormals_HighQuality output 1
- `B` <= [72] World Space to Tangent Space TransformVector output 0
- `Value` <= [55] Input World Space Normals (StaticBool) output 0

### [72] World Space to Tangent Space TransformVector

- `Input` <= [65] WorldAlignedNormals_HighQuality output 1

### [73] Switch

- `A` <= [65] WorldAlignedNormals_HighQuality output 0
- `B` <= [74] World Space to Tangent Space TransformVector output 0
- `Value` <= [55] Input World Space Normals (StaticBool) output 0

### [74] World Space to Tangent Space TransformVector

- `Input` <= [65] WorldAlignedNormals_HighQuality output 0

### [81] 1-x

- `Input` <= [79] HeightLerpWithTwoHeightMaps output 1

### [82] 1-x

- `Input` <= [80] HeightLerpWithTwoHeightMaps output 1

### [86] Dot

- `A` <= [24] Append output 0
- `B` <= [89] Input TheDiffuseHeightChannel (Vector4) output 0

### [87] Dot

- `A` <= [23] Append output 0
- `B` <= [89] Input TheDiffuseHeightChannel (Vector4) output 0

### [88] Input PickTheDiffuseHeightChannel (StaticBool)

- `Preview` <= [94] Static Bool (False) output 0

### [90] Switch

- `A` <= [86] Dot output 0
- `B` <= [19] Texture Sample output 4
- `Value` <= [88] Input PickTheDiffuseHeightChannel (StaticBool) output 0

### [91] Switch

- `A` <= [87] Dot output 0
- `B` <= [17] Texture Sample output 4
- `Value` <= [88] Input PickTheDiffuseHeightChannel (StaticBool) output 0

### [92] Dot

- `A` <= [46] Switch output 0
- `B` <= [89] Input TheDiffuseHeightChannel (Vector4) output 0

### [93] Switch

- `A` <= [92] Dot output 0
- `B` <= [54] Mask ( A ) output 0
- `Value` <= [88] Input PickTheDiffuseHeightChannel (StaticBool) output 0

### [95] Output XYZ Spec

- `A` <= [96] Lerp output 0

### [96] Lerp

- `A` <= [99] Lerp output 0
- `B` <= [108] Switch output 0
- `Alpha` <= [38] Switch output 0

### [97] Input Spec Texture Object (Texture2D)

- `Preview` <= [98] Texture Object output 0

### [99] Lerp

- `A` <= [107] Switch output 0
- `B` <= [106] Switch output 0
- `Alpha` <= [35] Switch output 0

### [100] Texture Sample

- `Coordinates` <= [18] Mask ( G B ) output 0
- `TextureObject` <= [97] Input Spec Texture Object (Texture2D) output 0

### [101] Texture Sample

- `Coordinates` <= [20] Mask ( R B ) output 0
- `TextureObject` <= [97] Input Spec Texture Object (Texture2D) output 0

### [102] Output XY Spec

- `A` <= [99] Lerp output 0

### [103] Output Z Spec

- `A` <= [108] Switch output 0

### [104] Append

- `A` <= [100] Texture Sample output 0
- `B` <= [100] Texture Sample output 4

### [105] Append

- `A` <= [101] Texture Sample output 0
- `B` <= [101] Texture Sample output 4

### [106] Switch

- `A` <= [104] Append output 0
- `B` <= [100] Texture Sample output 0
- `Value` <= [109] Input Export Float 4 Spec (StaticBool) output 0

### [107] Switch

- `A` <= [105] Append output 0
- `B` <= [101] Texture Sample output 0
- `Value` <= [109] Input Export Float 4 Spec (StaticBool) output 0

### [108] Switch

- `A` <= [112] Switch output 0
- `B` <= [117] Mask ( R G B ) output 0
- `Value` <= [109] Input Export Float 4 Spec (StaticBool) output 0

### [109] Input Export Float 4 Spec (StaticBool)

- `Preview` <= [110] Static Bool (False) output 0

### [111] Input Z Plane Spec Texture Object (Texture2D)

- `Preview` <= [98] Texture Object output 0

### [112] Switch

- `A` <= [116] Append output 0
- `B` <= [115] Append output 0
- `Value` <= [44] Input Use seperate settings for Z projection (StaticBool) output 0

### [113] Texture Sample

- `Coordinates` <= [8] Mask ( R G ) output 0
- `TextureObject` <= [97] Input Spec Texture Object (Texture2D) output 0

### [114] Texture Sample

- `Coordinates` <= [8] Mask ( R G ) output 0
- `TextureObject` <= [111] Input Z Plane Spec Texture Object (Texture2D) output 0

### [115] Append

- `A` <= [113] Texture Sample output 0
- `B` <= [113] Texture Sample output 4

### [116] Append

- `A` <= [114] Texture Sample output 0
- `B` <= [114] Texture Sample output 4

### [117] Mask ( R G B )

- `Input` <= [112] Switch output 0

### [119] Output XYZ Mat Attributes - See Tooltip

- `A` <= [120] MakeMaterialAttributes output 0

### [120] MakeMaterialAttributes

- `BaseColor` <= [0] Output XYZ Diffuse Texture output 0
- `Metallic` <= [121] BreakOutFloat3Components output 2
- `Specular` <= [121] BreakOutFloat3Components output 0
- `Roughness` <= [121] BreakOutFloat3Components output 1
- `Opacity` <= [131] Switch output 0
- `Normal` <= [61] Output Normal XYZ Texture output 0

### [122] Output XY Mat Attributes - See Tooltip

- `A` <= [123] MakeMaterialAttributes output 0

### [123] MakeMaterialAttributes

- `BaseColor` <= [21] Output XY Diffuse Texture output 0
- `Metallic` <= [124] BreakOutFloat3Components output 2
- `Specular` <= [124] BreakOutFloat3Components output 0
- `Roughness` <= [124] BreakOutFloat3Components output 1
- `Opacity` <= [132] Switch output 0
- `Normal` <= [60] Output Normal XY Texture output 0

### [125] Output Z Mat Attributes - See Tooltip

- `A` <= [126] MakeMaterialAttributes output 0

### [126] MakeMaterialAttributes

- `BaseColor` <= [22] Output Z Diffuse Texture output 0
- `Metallic` <= [127] BreakOutFloat3Components output 2
- `Specular` <= [127] BreakOutFloat3Components output 0
- `Roughness` <= [127] BreakOutFloat3Components output 1
- `Opacity` <= [133] Switch output 0
- `Normal` <= [59] Output Normal Z Texture output 0

### [128] Mask ( A )

- `Input` <= [0] Output XYZ Diffuse Texture output 0

### [129] Mask ( A )

- `Input` <= [21] Output XY Diffuse Texture output 0

### [130] Mask ( A )

- `Input` <= [22] Output Z Diffuse Texture output 0

### [131] Switch

- `A` <= [128] Mask ( A ) output 0
- `B` <= [134] 0 output 0
- `Value` <= [28] Input Export Float 4 Diffuse (StaticBool) output 0

### [132] Switch

- `A` <= [129] Mask ( A ) output 0
- `B` <= [134] 0 output 0
- `Value` <= [28] Input Export Float 4 Diffuse (StaticBool) output 0

### [133] Switch

- `A` <= [130] Mask ( A ) output 0
- `B` <= [134] 0 output 0
- `Value` <= [28] Input Export Float 4 Diffuse (StaticBool) output 0

## Connection List

- [1] Lerp output 0 -> Output XYZ Diffuse Texture.A
- [13] Lerp output 0 -> Lerp.A
- [27] Switch output 0 -> Lerp.B
- [38] Switch output 0 -> Lerp.Alpha
- [4] Mask ( B ) output 0 -> Abs.Input
- [5] Input World Space Normal (Vector3) output 0 -> Mask ( B ).Input
- [33] VertexNormalWS output 0 -> Input World Space Normal (Vector3).Preview
- [7] Texture Object output 0 -> Input Diffuse Texture Object (Texture2D).Preview
- [43] Switch output 0 -> Mask ( R G ).Input
- [11] Input WorldPosition (Vector3) output 0 -> Divide.A
- [34] Multiply(,-1) output 0 -> Divide.B
- [12] Absolute World Position output 0 -> Input WorldPosition (Vector3).Preview
- [26] Switch output 0 -> Lerp.A
- [25] Switch output 0 -> Lerp.B
- [35] Switch output 0 -> Lerp.Alpha
- [16] Mask ( R ) output 0 -> Abs.Input
- [5] Input World Space Normal (Vector3) output 0 -> Mask ( R ).Input
- [18] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [6] Input Diffuse Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [9] Divide output 0 -> Mask ( G B ).Input
- [20] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [6] Input Diffuse Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [9] Divide output 0 -> Mask ( R B ).Input
- [13] Lerp output 0 -> Output XY Diffuse Texture.A
- [27] Switch output 0 -> Output Z Diffuse Texture.A
- [17] Texture Sample output 0 -> Append.A
- [17] Texture Sample output 4 -> Append.B
- [19] Texture Sample output 0 -> Append.A
- [19] Texture Sample output 4 -> Append.B
- [23] Append output 0 -> Switch.A
- [17] Texture Sample output 0 -> Switch.B
- [28] Input Export Float 4 Diffuse (StaticBool) output 0 -> Switch.Value
- [24] Append output 0 -> Switch.A
- [19] Texture Sample output 0 -> Switch.B
- [28] Input Export Float 4 Diffuse (StaticBool) output 0 -> Switch.Value
- [46] Switch output 0 -> Switch.A
- [53] Mask ( R G B ) output 0 -> Switch.B
- [28] Input Export Float 4 Diffuse (StaticBool) output 0 -> Switch.Value
- [29] Static Bool (False) output 0 -> Input Export Float 4 Diffuse (StaticBool).Preview
- [14] CheapContrast output 0 -> Mask ( R ).Input
- [2] CheapContrast output 0 -> Mask ( R ).Input
- [66] Abs output 0 -> Multiply(,-1).A
- [81] 1-x output 0 -> Switch.A
- [30] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [36] Static Bool (False) output 0 -> Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool).Preview
- [82] 1-x output 0 -> Switch.A
- [31] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [11] Input WorldPosition (Vector3) output 0 -> Divide.A
- [40] Multiply(,-1) output 0 -> Divide.B
- [41] Input Texture Size Z projection (Vector3) output 0 -> Multiply(,-1).A
- [39] Divide output 0 -> Switch.A
- [9] Divide output 0 -> Switch.B
- [44] Input Use seperate settings for Z projection (StaticBool) output 0 -> Switch.Value
- [42] Static Bool (False) output 0 -> Input Use seperate settings for Z projection (StaticBool).Preview
- [7] Texture Object output 0 -> Input Z Plane Diffuse Texture Object (Texture2D).Preview
- [52] Append output 0 -> Switch.A
- [51] Append output 0 -> Switch.B
- [47] Input Use seperate texture for Z projection (StaticBool) output 0 -> Switch.Value
- [48] Static Bool (False) output 0 -> Input Use seperate texture for Z projection (StaticBool).Preview
- [8] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [6] Input Diffuse Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [45] Input Z Plane Diffuse Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [49] Texture Sample output 0 -> Append.A
- [49] Texture Sample output 4 -> Append.B
- [50] Texture Sample output 0 -> Append.A
- [50] Texture Sample output 4 -> Append.B
- [46] Switch output 0 -> Mask ( R G B ).Input
- [46] Switch output 0 -> Mask ( A ).Input
- [56] Static Bool (True) output 0 -> Input World Space Normals (StaticBool).Preview
- [58] Texture Object output 0 -> Input NormalTextureObject (Texture2D).Preview
- [67] Switch output 0 -> Output Normal Z Texture.A
- [69] Switch output 0 -> Output Normal XY Texture.A
- [73] Switch output 0 -> Output Normal XYZ Texture.A
- [71] Switch output 0 -> Output Normal XYZ Flat Top.A
- [82] 1-x output 0 -> Switch.A
- [31] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [81] 1-x output 0 -> Switch.A
- [30] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [10] Input TextureSize (Vector3) output 0 -> Abs.Input
- [65] WorldAlignedNormals_HighQuality output 3 -> Switch.A
- [68] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [55] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [65] WorldAlignedNormals_HighQuality output 3 -> World Space to Tangent Space TransformVector.Input
- [65] WorldAlignedNormals_HighQuality output 2 -> Switch.A
- [70] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [55] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [65] WorldAlignedNormals_HighQuality output 2 -> World Space to Tangent Space TransformVector.Input
- [65] WorldAlignedNormals_HighQuality output 1 -> Switch.A
- [72] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [55] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [65] WorldAlignedNormals_HighQuality output 1 -> World Space to Tangent Space TransformVector.Input
- [65] WorldAlignedNormals_HighQuality output 0 -> Switch.A
- [74] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [55] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [65] WorldAlignedNormals_HighQuality output 0 -> World Space to Tangent Space TransformVector.Input
- [79] HeightLerpWithTwoHeightMaps output 1 -> 1-x.Input
- [80] HeightLerpWithTwoHeightMaps output 1 -> 1-x.Input
- [24] Append output 0 -> Dot.A
- [89] Input TheDiffuseHeightChannel (Vector4) output 0 -> Dot.B
- [23] Append output 0 -> Dot.A
- [89] Input TheDiffuseHeightChannel (Vector4) output 0 -> Dot.B
- [94] Static Bool (False) output 0 -> Input PickTheDiffuseHeightChannel (StaticBool).Preview
- [86] Dot output 0 -> Switch.A
- [19] Texture Sample output 4 -> Switch.B
- [88] Input PickTheDiffuseHeightChannel (StaticBool) output 0 -> Switch.Value
- [87] Dot output 0 -> Switch.A
- [17] Texture Sample output 4 -> Switch.B
- [88] Input PickTheDiffuseHeightChannel (StaticBool) output 0 -> Switch.Value
- [46] Switch output 0 -> Dot.A
- [89] Input TheDiffuseHeightChannel (Vector4) output 0 -> Dot.B
- [92] Dot output 0 -> Switch.A
- [54] Mask ( A ) output 0 -> Switch.B
- [88] Input PickTheDiffuseHeightChannel (StaticBool) output 0 -> Switch.Value
- [96] Lerp output 0 -> Output XYZ Spec.A
- [99] Lerp output 0 -> Lerp.A
- [108] Switch output 0 -> Lerp.B
- [38] Switch output 0 -> Lerp.Alpha
- [98] Texture Object output 0 -> Input Spec Texture Object (Texture2D).Preview
- [107] Switch output 0 -> Lerp.A
- [106] Switch output 0 -> Lerp.B
- [35] Switch output 0 -> Lerp.Alpha
- [18] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [97] Input Spec Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [20] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [97] Input Spec Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [99] Lerp output 0 -> Output XY Spec.A
- [108] Switch output 0 -> Output Z Spec.A
- [100] Texture Sample output 0 -> Append.A
- [100] Texture Sample output 4 -> Append.B
- [101] Texture Sample output 0 -> Append.A
- [101] Texture Sample output 4 -> Append.B
- [104] Append output 0 -> Switch.A
- [100] Texture Sample output 0 -> Switch.B
- [109] Input Export Float 4 Spec (StaticBool) output 0 -> Switch.Value
- [105] Append output 0 -> Switch.A
- [101] Texture Sample output 0 -> Switch.B
- [109] Input Export Float 4 Spec (StaticBool) output 0 -> Switch.Value
- [112] Switch output 0 -> Switch.A
- [117] Mask ( R G B ) output 0 -> Switch.B
- [109] Input Export Float 4 Spec (StaticBool) output 0 -> Switch.Value
- [110] Static Bool (False) output 0 -> Input Export Float 4 Spec (StaticBool).Preview
- [98] Texture Object output 0 -> Input Z Plane Spec Texture Object (Texture2D).Preview
- [116] Append output 0 -> Switch.A
- [115] Append output 0 -> Switch.B
- [44] Input Use seperate settings for Z projection (StaticBool) output 0 -> Switch.Value
- [8] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [97] Input Spec Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [111] Input Z Plane Spec Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [113] Texture Sample output 0 -> Append.A
- [113] Texture Sample output 4 -> Append.B
- [114] Texture Sample output 0 -> Append.A
- [114] Texture Sample output 4 -> Append.B
- [112] Switch output 0 -> Mask ( R G B ).Input
- [120] MakeMaterialAttributes output 0 -> Output XYZ Mat Attributes - See Tooltip.A
- [0] Output XYZ Diffuse Texture output 0 -> MakeMaterialAttributes.BaseColor
- [121] BreakOutFloat3Components output 2 -> MakeMaterialAttributes.Metallic
- [121] BreakOutFloat3Components output 0 -> MakeMaterialAttributes.Specular
- [121] BreakOutFloat3Components output 1 -> MakeMaterialAttributes.Roughness
- [131] Switch output 0 -> MakeMaterialAttributes.Opacity
- [61] Output Normal XYZ Texture output 0 -> MakeMaterialAttributes.Normal
- [123] MakeMaterialAttributes output 0 -> Output XY Mat Attributes - See Tooltip.A
- [21] Output XY Diffuse Texture output 0 -> MakeMaterialAttributes.BaseColor
- [124] BreakOutFloat3Components output 2 -> MakeMaterialAttributes.Metallic
- [124] BreakOutFloat3Components output 0 -> MakeMaterialAttributes.Specular
- [124] BreakOutFloat3Components output 1 -> MakeMaterialAttributes.Roughness
- [132] Switch output 0 -> MakeMaterialAttributes.Opacity
- [60] Output Normal XY Texture output 0 -> MakeMaterialAttributes.Normal
- [126] MakeMaterialAttributes output 0 -> Output Z Mat Attributes - See Tooltip.A
- [22] Output Z Diffuse Texture output 0 -> MakeMaterialAttributes.BaseColor
- [127] BreakOutFloat3Components output 2 -> MakeMaterialAttributes.Metallic
- [127] BreakOutFloat3Components output 0 -> MakeMaterialAttributes.Specular
- [127] BreakOutFloat3Components output 1 -> MakeMaterialAttributes.Roughness
- [133] Switch output 0 -> MakeMaterialAttributes.Opacity
- [59] Output Normal Z Texture output 0 -> MakeMaterialAttributes.Normal
- [0] Output XYZ Diffuse Texture output 0 -> Mask ( A ).Input
- [21] Output XY Diffuse Texture output 0 -> Mask ( A ).Input
- [22] Output Z Diffuse Texture output 0 -> Mask ( A ).Input
- [128] Mask ( A ) output 0 -> Switch.A
- [134] 0 output 0 -> Switch.B
- [28] Input Export Float 4 Diffuse (StaticBool) output 0 -> Switch.Value
- [129] Mask ( A ) output 0 -> Switch.A
- [134] 0 output 0 -> Switch.B
- [28] Input Export Float 4 Diffuse (StaticBool) output 0 -> Switch.Value
- [130] Mask ( A ) output 0 -> Switch.A
- [134] 0 output 0 -> Switch.B
- [28] Input Export Float 4 Diffuse (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `XYZ Diffuse Texture` is driven by [1] Lerp
- `XY Diffuse Texture` is driven by [13] Lerp
- `Z Diffuse Texture` is driven by [27] Switch
- `Normal Z Texture` is driven by [67] Switch
- `Normal XY Texture` is driven by [69] Switch
- `Normal XYZ Texture` is driven by [73] Switch
- `Normal XYZ Flat Top` is driven by [71] Switch
- `-------------------------- ` has no captured source.
- `XYZ Spec` is driven by [96] Lerp
- `XY Spec` is driven by [99] Lerp
- `Z Spec` is driven by [108] Switch
- ` --------------------------` has no captured source.
- `XYZ Mat Attributes - See Tooltip` is driven by [120] MakeMaterialAttributes
- `XY Mat Attributes - See Tooltip` is driven by [123] MakeMaterialAttributes
- `Z Mat Attributes - See Tooltip` is driven by [126] MakeMaterialAttributes
- `  --------------------------` has no captured source.

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


