# WorldAlignedTextures_Complex_OptimizationAttempt

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/WorldAlignedTextures_Complex_OptimizationAttempt.WorldAlignedTextures_Complex_OptimizationAttempt`
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
- `Export Float 4` [FunctionInput_StaticBool] - Default = False
- `ProjectionTransitionContrast` [FunctionInput_Scalar]
- `Use Texture Alpha Heightmap to Refine the Transistion` [FunctionInput_StaticBool] - Defaults to false.
- `Texture Size Z projection` [FunctionInput_Vector3] - Size of texture in worldspace units
- `Use seperate settings for Z projection` [FunctionInput_StaticBool]
- `Z Plane Diffuse Texture Object` [FunctionInput_Texture2D]
- `Use seperate texture for Z projection` [FunctionInput_StaticBool]
- `World Space Normals` [FunctionInput_StaticBool] - This switch defaults to true. If left true ensure that tangent space normals is turned off in the shaders properties panel.   . 
- `NormalTextureObject` [FunctionInput_Texture2D]
- `UseHighQualityNormals` [FunctionInput_StaticBool] - This switch defaults to true
- `HeightLerpTransitionContrast` [FunctionInput_Scalar]
- `-------------` [FunctionInput_StaticBool] - -------------
- `  -------------` [FunctionInput_StaticBool] - -------------
- `    -------------` [FunctionInput_StaticBool] - -------------
- `     -------------` [FunctionInput_StaticBool] - -------------
- `      -------------` [FunctionInput_StaticBool] - -------------
- `PickTheDiffuseHeightChannel` [FunctionInput_StaticBool] - Default = False
- `TheDiffuseHeightChannel` [FunctionInput_Vector4] - (If pick the diffuse height channel == true)  (1,0,0,0) will use the diffuse red channel as the height map. (0,1,0,0) will use the green channel etc.
- `OptimizeHighQualityNormals (see comment)` [FunctionInput_StaticBool] - This switch defaults to false. This switch moves several operations out of the pixel shader and into the vertex shader. Which should be faster to run in most cases.  Enable high quality normals and connect all of the UV outputs. from this function to the corresponding custom uv inputs on the base shader. Multiple world aligned normal functions can use this technique at the same time. Only one node needs to connect to the uv inputs.       

## Outputs

- `XYZ Diffuse Texture`
- `XY Diffuse Texture`
- `Z Diffuse Texture`
- `Normal Z Texture`
- `Normal XY Texture`
- `Normal XYZ Texture`
- `Normal XYZ Flat Top`
- `-------------`
- `Opti Normal - Custom UV 6`
- `Opti Normal - Custom UV 5`
- `Opti Normal - Custom UV 4`
- `-------`
- `Opti Normal - Custom UV 7`
- `Opti Normal - Custom UV 3`
- `Opti Normal - Custom UV 2`

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
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Export Float 4 (StaticBool) | Export Float 4 |
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
| 55 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 56 | function_input | `MaterialExpressionFunctionInput` | Input World Space Normals (StaticBool) | World Space Normals |
| 57 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 58 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 59 | expression | `MaterialExpressionAppendVector` | Append |  |
| 60 | expression | `MaterialExpressionAppendVector` | Append |  |
| 61 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 62 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 64 | expression | `MaterialExpressionAppendVector` | Append |  |
| 65 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 66 | expression | `MaterialExpressionAppendVector` | Append |  |
| 67 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 68 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 69 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 70 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 72 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 73 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 74 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 75 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 76 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 77 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 78 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 79 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 80 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 81 | function_input | `MaterialExpressionFunctionInput` | Input NormalTextureObject (Texture2D) | NormalTextureObject |
| 82 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 83 | expression | `MaterialExpressionAppendVector` | Append |  |
| 84 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 85 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 86 | function_output | `MaterialExpressionFunctionOutput` | Output Normal Z Texture | Normal Z Texture |
| 87 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 88 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 89 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 90 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 91 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 92 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XY Texture | Normal XY Texture |
| 93 | expression | `MaterialExpressionConstant3Vector` | -1,-1,1 |  |
| 94 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XYZ Texture | Normal XYZ Texture |
| 95 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XYZ Flat Top | Normal XYZ Flat Top |
| 96 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 97 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 98 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 99 | expression | `MaterialExpressionConstant` | 1 |  |
| 100 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 101 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 102 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 103 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldAlignedNormals_HighQuality |  |
| 104 | expression | `MaterialExpressionAbs` | Abs |  |
| 105 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 106 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 107 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 108 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 109 | function_input | `MaterialExpressionFunctionInput` | Input UseHighQualityNormals (StaticBool) | UseHighQualityNormals |
| 110 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 111 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 112 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 113 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 114 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 115 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 116 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 117 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 118 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 119 | function_input | `MaterialExpressionFunctionInput` | Input HeightLerpTransitionContrast (Scalar) | HeightLerpTransitionContrast |
| 120 | function_input | `MaterialExpressionFunctionInput` | Input ------------- (StaticBool) | ------------- |
| 121 | function_input | `MaterialExpressionFunctionInput` | Input   ------------- (StaticBool) |   ------------- |
| 122 | function_input | `MaterialExpressionFunctionInput` | Input     ------------- (StaticBool) |     ------------- |
| 123 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerpWithTwoHeightMaps |  |
| 124 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerpWithTwoHeightMaps |  |
| 125 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 126 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 127 | function_output | `MaterialExpressionFunctionOutput` | Output ------------- | ------------- |
| 128 | function_input | `MaterialExpressionFunctionInput` | Input      ------------- (StaticBool) |      ------------- |
| 129 | function_input | `MaterialExpressionFunctionInput` | Input       ------------- (StaticBool) |       ------------- |
| 130 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 131 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 132 | function_input | `MaterialExpressionFunctionInput` | Input PickTheDiffuseHeightChannel (StaticBool) | PickTheDiffuseHeightChannel |
| 133 | function_input | `MaterialExpressionFunctionInput` | Input TheDiffuseHeightChannel (Vector4) | TheDiffuseHeightChannel |
| 134 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 135 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 136 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 137 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 138 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 139 | function_output | `MaterialExpressionFunctionOutput` | Output Opti Normal - Custom UV 6 | Opti Normal - Custom UV 6 |
| 140 | function_output | `MaterialExpressionFunctionOutput` | Output Opti Normal - Custom UV 5 | Opti Normal - Custom UV 5 |
| 141 | function_output | `MaterialExpressionFunctionOutput` | Output Opti Normal - Custom UV 4 | Opti Normal - Custom UV 4 |
| 142 | function_output | `MaterialExpressionFunctionOutput` | Output ------- | ------- |
| 143 | function_input | `MaterialExpressionFunctionInput` | Input OptimizeHighQualityNormals (see comment) (StaticBool) | OptimizeHighQualityNormals (see comment) |
| 144 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 145 | function_output | `MaterialExpressionFunctionOutput` | Output Opti Normal - Custom UV 7 | Opti Normal - Custom UV 7 |
| 146 | function_output | `MaterialExpressionFunctionOutput` | Output Opti Normal - Custom UV 3 | Opti Normal - Custom UV 3 |
| 147 | function_output | `MaterialExpressionFunctionOutput` | Output Opti Normal - Custom UV 2 | Opti Normal - Custom UV 2 |

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
- `Value` <= [28] Input Export Float 4 (StaticBool) output 0

### [26] Switch

- `A` <= [24] Append output 0
- `B` <= [19] Texture Sample output 0
- `Value` <= [28] Input Export Float 4 (StaticBool) output 0

### [27] Switch

- `A` <= [46] Switch output 0
- `B` <= [53] Mask ( R G B ) output 0
- `Value` <= [28] Input Export Float 4 (StaticBool) output 0

### [28] Input Export Float 4 (StaticBool)

- `Preview` <= [29] Static Bool (False) output 0

### [30] Mask ( R )

- `Input` <= [14] CheapContrast output 0

### [31] Mask ( R )

- `Input` <= [2] CheapContrast output 0

### [34] Multiply(,-1)

- `A` <= [104] Abs output 0

### [35] Switch

- `A` <= [125] 1-x output 0
- `B` <= [30] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool)

- `Preview` <= [36] Static Bool (False) output 0

### [38] Switch

- `A` <= [126] 1-x output 0
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

### [55] Multiply(,-1)

- `A` <= [61] Texture Sample output 1

### [56] Input World Space Normals (StaticBool)

- `Preview` <= [58] Static Bool (True) output 0

### [57] Multiply

- `A` <= [61] Texture Sample output 3
- `B` <= [100] Mask ( G ) output 0

### [59] Append

- `A` <= [55] Multiply(,-1) output 0
- `B` <= [57] Multiply output 0

### [60] Append

- `A` <= [59] Append output 0
- `B` <= [62] Multiply(,-1) output 0

### [61] Texture Sample

- `Coordinates` <= [20] Mask ( R B ) output 0
- `TextureObject` <= [81] Input NormalTextureObject (Texture2D) output 0

### [62] Multiply(,-1)

- `A` <= [61] Texture Sample output 2

### [63] Multiply

- `A` <= [96] Mask ( R ) output 0
- `B` <= [72] Texture Sample output 3

### [64] Append

- `A` <= [63] Multiply output 0
- `B` <= [71] Multiply(,-1) output 0

### [65] Lerp

- `A` <= [69] Lerp output 0
- `B` <= [83] Append output 0
- `Alpha` <= [101] Switch output 0

### [66] Append

- `A` <= [64] Append output 0
- `B` <= [76] Multiply(,-1) output 0

### [67] Switch

- `A` <= [68] Normalize output 0
- `B` <= [70] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [68] Normalize

- `VectorInput` <= [65] Lerp output 0

### [69] Lerp

- `A` <= [60] Append output 0
- `B` <= [66] Append output 0
- `Alpha` <= [102] Switch output 0

### [70] World Space to Tangent Space TransformVector

- `Input` <= [68] Normalize output 0

### [71] Multiply(,-1)

- `A` <= [72] Texture Sample output 1

### [72] Texture Sample

- `Coordinates` <= [18] Mask ( G B ) output 0
- `TextureObject` <= [81] Input NormalTextureObject (Texture2D) output 0

### [73] Lerp

- `A` <= [69] Lerp output 0
- `B` <= [5] Input World Space Normal (Vector3) output 0
- `Alpha` <= [101] Switch output 0

### [74] Switch

- `A` <= [75] Normalize output 0
- `B` <= [77] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [75] Normalize

- `VectorInput` <= [73] Lerp output 0

### [76] Multiply(,-1)

- `A` <= [72] Texture Sample output 2

### [77] World Space to Tangent Space TransformVector

- `Input` <= [75] Normalize output 0

### [78] Mask ( R G )

- `Input` <= [91] Multiply output 0

### [79] Switch

- `A` <= [80] Normalize output 0
- `B` <= [85] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [80] Normalize

- `VectorInput` <= [69] Lerp output 0

### [81] Input NormalTextureObject (Texture2D)

- `Preview` <= [82] Texture Object output 0

### [83] Append

- `A` <= [78] Mask ( R G ) output 0
- `B` <= [89] Multiply output 0

### [84] Texture Sample

- `Coordinates` <= [8] Mask ( R G ) output 0
- `TextureObject` <= [81] Input NormalTextureObject (Texture2D) output 0

### [85] World Space to Tangent Space TransformVector

- `Input` <= [80] Normalize output 0

### [86] Output Normal Z Texture

- `A` <= [108] Switch output 0

### [87] Switch

- `A` <= [88] Normalize output 0
- `B` <= [90] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [88] Normalize

- `VectorInput` <= [83] Append output 0

### [89] Multiply

- `A` <= [84] Texture Sample output 3
- `B` <= [98] Mask ( B ) output 0

### [90] World Space to Tangent Space TransformVector

- `Input` <= [88] Normalize output 0

### [91] Multiply

- `A` <= [84] Texture Sample output 0
- `B` <= [93] -1,-1,1 output 0

### [92] Output Normal XY Texture

- `A` <= [107] Switch output 0

### [94] Output Normal XYZ Texture

- `A` <= [105] Switch output 0

### [95] Output Normal XYZ Flat Top

- `A` <= [106] Switch output 0

### [96] Mask ( R )

- `Input` <= [97] Multiply output 0

### [97] Multiply

- `A` <= [5] Input World Space Normal (Vector3) output 0
- `B` <= [99] 1 output 0

### [98] Mask ( B )

- `Input` <= [97] Multiply output 0

### [100] Mask ( G )

- `Input` <= [97] Multiply output 0

### [101] Switch

- `A` <= [126] 1-x output 0
- `B` <= [31] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [102] Switch

- `A` <= [125] 1-x output 0
- `B` <= [30] Mask ( R ) output 0
- `Value` <= [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0

### [104] Abs

- `Input` <= [10] Input TextureSize (Vector3) output 0

### [105] Switch

- `A` <= [117] Switch output 0
- `B` <= [67] Switch output 0
- `Value` <= [109] Input UseHighQualityNormals (StaticBool) output 0

### [106] Switch

- `A` <= [115] Switch output 0
- `B` <= [74] Switch output 0
- `Value` <= [109] Input UseHighQualityNormals (StaticBool) output 0

### [107] Switch

- `A` <= [113] Switch output 0
- `B` <= [79] Switch output 0
- `Value` <= [109] Input UseHighQualityNormals (StaticBool) output 0

### [108] Switch

- `A` <= [111] Switch output 0
- `B` <= [87] Switch output 0
- `Value` <= [109] Input UseHighQualityNormals (StaticBool) output 0

### [109] Input UseHighQualityNormals (StaticBool)

- `Preview` <= [110] Static Bool (True) output 0

### [111] Switch

- `A` <= [103] WorldAlignedNormals_HighQuality output 3
- `B` <= [112] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [112] World Space to Tangent Space TransformVector

- `Input` <= [103] WorldAlignedNormals_HighQuality output 3

### [113] Switch

- `A` <= [103] WorldAlignedNormals_HighQuality output 2
- `B` <= [114] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [114] World Space to Tangent Space TransformVector

- `Input` <= [103] WorldAlignedNormals_HighQuality output 2

### [115] Switch

- `A` <= [103] WorldAlignedNormals_HighQuality output 1
- `B` <= [116] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [116] World Space to Tangent Space TransformVector

- `Input` <= [103] WorldAlignedNormals_HighQuality output 1

### [117] Switch

- `A` <= [103] WorldAlignedNormals_HighQuality output 0
- `B` <= [118] World Space to Tangent Space TransformVector output 0
- `Value` <= [56] Input World Space Normals (StaticBool) output 0

### [118] World Space to Tangent Space TransformVector

- `Input` <= [103] WorldAlignedNormals_HighQuality output 0

### [125] 1-x

- `Input` <= [123] HeightLerpWithTwoHeightMaps output 1

### [126] 1-x

- `Input` <= [124] HeightLerpWithTwoHeightMaps output 1

### [130] Dot

- `A` <= [24] Append output 0
- `B` <= [133] Input TheDiffuseHeightChannel (Vector4) output 0

### [131] Dot

- `A` <= [23] Append output 0
- `B` <= [133] Input TheDiffuseHeightChannel (Vector4) output 0

### [132] Input PickTheDiffuseHeightChannel (StaticBool)

- `Preview` <= [138] Static Bool (False) output 0

### [134] Switch

- `A` <= [130] Dot output 0
- `B` <= [19] Texture Sample output 4
- `Value` <= [132] Input PickTheDiffuseHeightChannel (StaticBool) output 0

### [135] Switch

- `A` <= [131] Dot output 0
- `B` <= [17] Texture Sample output 4
- `Value` <= [132] Input PickTheDiffuseHeightChannel (StaticBool) output 0

### [136] Dot

- `A` <= [46] Switch output 0
- `B` <= [133] Input TheDiffuseHeightChannel (Vector4) output 0

### [137] Switch

- `A` <= [136] Dot output 0
- `B` <= [54] Mask ( A ) output 0
- `Value` <= [132] Input PickTheDiffuseHeightChannel (StaticBool) output 0

### [143] Input OptimizeHighQualityNormals (see comment) (StaticBool)

- `Preview` <= [144] Static Bool (False) output 0

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
- [28] Input Export Float 4 (StaticBool) output 0 -> Switch.Value
- [24] Append output 0 -> Switch.A
- [19] Texture Sample output 0 -> Switch.B
- [28] Input Export Float 4 (StaticBool) output 0 -> Switch.Value
- [46] Switch output 0 -> Switch.A
- [53] Mask ( R G B ) output 0 -> Switch.B
- [28] Input Export Float 4 (StaticBool) output 0 -> Switch.Value
- [29] Static Bool (False) output 0 -> Input Export Float 4 (StaticBool).Preview
- [14] CheapContrast output 0 -> Mask ( R ).Input
- [2] CheapContrast output 0 -> Mask ( R ).Input
- [104] Abs output 0 -> Multiply(,-1).A
- [125] 1-x output 0 -> Switch.A
- [30] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [36] Static Bool (False) output 0 -> Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool).Preview
- [126] 1-x output 0 -> Switch.A
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
- [61] Texture Sample output 1 -> Multiply(,-1).A
- [58] Static Bool (True) output 0 -> Input World Space Normals (StaticBool).Preview
- [61] Texture Sample output 3 -> Multiply.A
- [100] Mask ( G ) output 0 -> Multiply.B
- [55] Multiply(,-1) output 0 -> Append.A
- [57] Multiply output 0 -> Append.B
- [59] Append output 0 -> Append.A
- [62] Multiply(,-1) output 0 -> Append.B
- [20] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [81] Input NormalTextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [61] Texture Sample output 2 -> Multiply(,-1).A
- [96] Mask ( R ) output 0 -> Multiply.A
- [72] Texture Sample output 3 -> Multiply.B
- [63] Multiply output 0 -> Append.A
- [71] Multiply(,-1) output 0 -> Append.B
- [69] Lerp output 0 -> Lerp.A
- [83] Append output 0 -> Lerp.B
- [101] Switch output 0 -> Lerp.Alpha
- [64] Append output 0 -> Append.A
- [76] Multiply(,-1) output 0 -> Append.B
- [68] Normalize output 0 -> Switch.A
- [70] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [65] Lerp output 0 -> Normalize.VectorInput
- [60] Append output 0 -> Lerp.A
- [66] Append output 0 -> Lerp.B
- [102] Switch output 0 -> Lerp.Alpha
- [68] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [72] Texture Sample output 1 -> Multiply(,-1).A
- [18] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [81] Input NormalTextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [69] Lerp output 0 -> Lerp.A
- [5] Input World Space Normal (Vector3) output 0 -> Lerp.B
- [101] Switch output 0 -> Lerp.Alpha
- [75] Normalize output 0 -> Switch.A
- [77] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [73] Lerp output 0 -> Normalize.VectorInput
- [72] Texture Sample output 2 -> Multiply(,-1).A
- [75] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [91] Multiply output 0 -> Mask ( R G ).Input
- [80] Normalize output 0 -> Switch.A
- [85] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [69] Lerp output 0 -> Normalize.VectorInput
- [82] Texture Object output 0 -> Input NormalTextureObject (Texture2D).Preview
- [78] Mask ( R G ) output 0 -> Append.A
- [89] Multiply output 0 -> Append.B
- [8] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [81] Input NormalTextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [80] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [108] Switch output 0 -> Output Normal Z Texture.A
- [88] Normalize output 0 -> Switch.A
- [90] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [83] Append output 0 -> Normalize.VectorInput
- [84] Texture Sample output 3 -> Multiply.A
- [98] Mask ( B ) output 0 -> Multiply.B
- [88] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [84] Texture Sample output 0 -> Multiply.A
- [93] -1,-1,1 output 0 -> Multiply.B
- [107] Switch output 0 -> Output Normal XY Texture.A
- [105] Switch output 0 -> Output Normal XYZ Texture.A
- [106] Switch output 0 -> Output Normal XYZ Flat Top.A
- [97] Multiply output 0 -> Mask ( R ).Input
- [5] Input World Space Normal (Vector3) output 0 -> Multiply.A
- [99] 1 output 0 -> Multiply.B
- [97] Multiply output 0 -> Mask ( B ).Input
- [97] Multiply output 0 -> Mask ( G ).Input
- [126] 1-x output 0 -> Switch.A
- [31] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [125] 1-x output 0 -> Switch.A
- [30] Mask ( R ) output 0 -> Switch.B
- [37] Input Use Texture Alpha Heightmap to Refine the Transistion (StaticBool) output 0 -> Switch.Value
- [10] Input TextureSize (Vector3) output 0 -> Abs.Input
- [117] Switch output 0 -> Switch.A
- [67] Switch output 0 -> Switch.B
- [109] Input UseHighQualityNormals (StaticBool) output 0 -> Switch.Value
- [115] Switch output 0 -> Switch.A
- [74] Switch output 0 -> Switch.B
- [109] Input UseHighQualityNormals (StaticBool) output 0 -> Switch.Value
- [113] Switch output 0 -> Switch.A
- [79] Switch output 0 -> Switch.B
- [109] Input UseHighQualityNormals (StaticBool) output 0 -> Switch.Value
- [111] Switch output 0 -> Switch.A
- [87] Switch output 0 -> Switch.B
- [109] Input UseHighQualityNormals (StaticBool) output 0 -> Switch.Value
- [110] Static Bool (True) output 0 -> Input UseHighQualityNormals (StaticBool).Preview
- [103] WorldAlignedNormals_HighQuality output 3 -> Switch.A
- [112] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [103] WorldAlignedNormals_HighQuality output 3 -> World Space to Tangent Space TransformVector.Input
- [103] WorldAlignedNormals_HighQuality output 2 -> Switch.A
- [114] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [103] WorldAlignedNormals_HighQuality output 2 -> World Space to Tangent Space TransformVector.Input
- [103] WorldAlignedNormals_HighQuality output 1 -> Switch.A
- [116] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [103] WorldAlignedNormals_HighQuality output 1 -> World Space to Tangent Space TransformVector.Input
- [103] WorldAlignedNormals_HighQuality output 0 -> Switch.A
- [118] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [56] Input World Space Normals (StaticBool) output 0 -> Switch.Value
- [103] WorldAlignedNormals_HighQuality output 0 -> World Space to Tangent Space TransformVector.Input
- [123] HeightLerpWithTwoHeightMaps output 1 -> 1-x.Input
- [124] HeightLerpWithTwoHeightMaps output 1 -> 1-x.Input
- [24] Append output 0 -> Dot.A
- [133] Input TheDiffuseHeightChannel (Vector4) output 0 -> Dot.B
- [23] Append output 0 -> Dot.A
- [133] Input TheDiffuseHeightChannel (Vector4) output 0 -> Dot.B
- [138] Static Bool (False) output 0 -> Input PickTheDiffuseHeightChannel (StaticBool).Preview
- [130] Dot output 0 -> Switch.A
- [19] Texture Sample output 4 -> Switch.B
- [132] Input PickTheDiffuseHeightChannel (StaticBool) output 0 -> Switch.Value
- [131] Dot output 0 -> Switch.A
- [17] Texture Sample output 4 -> Switch.B
- [132] Input PickTheDiffuseHeightChannel (StaticBool) output 0 -> Switch.Value
- [46] Switch output 0 -> Dot.A
- [133] Input TheDiffuseHeightChannel (Vector4) output 0 -> Dot.B
- [136] Dot output 0 -> Switch.A
- [54] Mask ( A ) output 0 -> Switch.B
- [132] Input PickTheDiffuseHeightChannel (StaticBool) output 0 -> Switch.Value
- [144] Static Bool (False) output 0 -> Input OptimizeHighQualityNormals (see comment) (StaticBool).Preview

## Output Trace Roots

- `XYZ Diffuse Texture` is driven by [1] Lerp
- `XY Diffuse Texture` is driven by [13] Lerp
- `Z Diffuse Texture` is driven by [27] Switch
- `Normal Z Texture` is driven by [108] Switch
- `Normal XY Texture` is driven by [107] Switch
- `Normal XYZ Texture` is driven by [105] Switch
- `Normal XYZ Flat Top` is driven by [106] Switch
- `-------------` has no captured source.
- `Opti Normal - Custom UV 6` has no captured source.
- `Opti Normal - Custom UV 5` has no captured source.
- `Opti Normal - Custom UV 4` has no captured source.
- `-------` has no captured source.
- `Opti Normal - Custom UV 7` has no captured source.
- `Opti Normal - Custom UV 3` has no captured source.
- `Opti Normal - Custom UV 2` has no captured source.

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


