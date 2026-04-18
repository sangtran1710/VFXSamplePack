# PivotPainter2FoliageAnimation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/PivotPainter2FoliageAnimation.PivotPainter2FoliageAnimation`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Texture Dimensions To Gather Parent UVs` [FunctionInput_Vector2] - Use the "Texture Property" node with the texture size option selected from the drop down.
- `Wind Gust Offset` [FunctionInput_Scalar]
- `Wind Gust Angle Rotation` [FunctionInput_Scalar]
- `Motion Dampening Falloff Radius` [FunctionInput_Scalar]
- `Pivot Painter Texture Coordinate` [FunctionInput_Vector2] - Defaults To UV 1.
- `Wind Speed Down Wind Vector` [FunctionInput_Scalar]
- `Optional - world space pixel normal` [FunctionInput_Vector3]
- `Layer Mask` [FunctionInput_Scalar] - Enter a black and white value. Grayscale values will produce incorrect results.
- `Enable` [FunctionInput_StaticBool] - Defaults to true and outputs the correct World Position Offset and Rotated Normal. 
- `Enable Normal Rotation` [FunctionInput_StaticBool] - Defaults to true. 
- `Wind Shelter Settings` [FunctionInput_Vector2] - Bias and scale to the results of a dot product between the wind vector and the element axis vector. The defaults make sure that the wind fully affects branches facing the wind while leaving the other branches unaffected. 
- `Random Rotation Influence` [FunctionInput_Scalar]
- `Random Rotation Texture Sample Scale` [FunctionInput_Scalar]
- `Advanced - Parent Rotation Angle Influence` [FunctionInput_Scalar]
- `Optional - Wind Turbulence and Gust Mag` [FunctionInput_Texture2D]
- `Wind Direction` [FunctionInput_Vector3]
- `Wind Direction Y Axis` [FunctionInput_Vector3]
- `Position And Index Texture` [FunctionInput_Texture2D]
- `X-axis and X-Extent Texture` [FunctionInput_Texture2D]
- `Wind Speed Horizontal Speed` [FunctionInput_Scalar]
- `Wind Gust World Scale` [FunctionInput_Scalar]
- `Use Wind Shelter Settings` [FunctionInput_StaticBool]

## Outputs

- `Parent Index Integer`
- `World Position Offset`
- `Parent UVs`
- `Rotated World Space Pixel Normal`
- `Final World Position`
- `Advanced - Rotation Angle Animation`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Parent Index Integer | Parent Index Integer |
| 1 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_ReturnParentTextureInfo |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Texture Dimensions To Gather Parent UVs (Vector2) | Texture Dimensions To Gather Parent UVs |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_DecodePostion |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_DecodeAxisVector |  |
| 7 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Wind Gust Offset (Scalar) | Wind Gust Offset |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Wind Gust Angle Rotation (Scalar) | Wind Gust Angle Rotation |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 16 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Motion Dampening Falloff Radius (Scalar) | Motion Dampening Falloff Radius |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionTime` | Time (Period of 99999997952.00) |  |
| 20 | expression | `MaterialExpressionFrac` | Frac |  |
| 21 | expression | `MaterialExpressionAdd` | Add |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_Decode8BitAlphaAxisExtent |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Pivot Painter Texture Coordinate (Vector2) | Pivot Painter Texture Coordinate |
| 28 | function_output | `MaterialExpressionFunctionOutput` | Output Parent UVs | Parent UVs |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input Wind Speed Down Wind Vector (Scalar) | Wind Speed Down Wind Vector |
| 30 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 32 | function_input | `MaterialExpressionFunctionInput` | Input Optional - world space pixel normal (Vector3) | Optional - world space pixel normal |
| 33 | function_output | `MaterialExpressionFunctionOutput` | Output Rotated World Space Pixel Normal | Rotated World Space Pixel Normal |
| 34 | function_call | `MaterialExpressionMaterialFunctionCall` | FixRotateAboutAxisNormals |  |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input Layer Mask (Scalar) | Layer Mask |
| 36 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 37 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 38 | function_input | `MaterialExpressionFunctionInput` | Input Enable (StaticBool) | Enable |
| 39 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 40 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 41 | expression | `MaterialExpressionConstant` | 0 |  |
| 42 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input Enable Normal Rotation (StaticBool) | Enable Normal Rotation |
| 44 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 45 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Wind Shelter Settings (Vector2) | Wind Shelter Settings |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 49 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 50 | expression | `MaterialExpressionAdd` | Add |  |
| 51 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 52 | function_output | `MaterialExpressionFunctionOutput` | Output Final World Position | Final World Position |
| 53 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 54 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 55 | expression | `MaterialExpressionDivide` | Divide |  |
| 56 | expression | `MaterialExpressionClamp` | Clamp |  |
| 57 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 58 | expression | `MaterialExpressionAdd` | Add |  |
| 59 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 60 | function_input | `MaterialExpressionFunctionInput` | Input Random Rotation Influence (Scalar) | Random Rotation Influence |
| 61 | function_input | `MaterialExpressionFunctionInput` | Input Random Rotation Texture Sample Scale (Scalar) | Random Rotation Texture Sample Scale |
| 62 | expression | `MaterialExpressionFrac` | Frac |  |
| 63 | expression | `MaterialExpressionDivide` | Divide |  |
| 64 | function_input | `MaterialExpressionFunctionInput` | Input Advanced - Parent Rotation Angle Influence (Scalar) | Advanced - Parent Rotation Angle Influence |
| 65 | expression | `MaterialExpressionAdd` | Add |  |
| 66 | function_output | `MaterialExpressionFunctionOutput` | Output Advanced - Rotation Angle Animation | Advanced - Rotation Angle Animation |
| 67 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 68 | function_input | `MaterialExpressionFunctionInput` | Input Optional - Wind Turbulence and Gust Mag (Texture2D) | Optional - Wind Turbulence and Gust Mag |
| 69 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 70 | function_input | `MaterialExpressionFunctionInput` | Input Wind Direction (Vector3) | Wind Direction |
| 71 | function_input | `MaterialExpressionFunctionInput` | Input Wind Direction Y Axis (Vector3) | Wind Direction Y Axis |
| 72 | function_input | `MaterialExpressionFunctionInput` | Input Position And Index Texture (Texture2D) | Position And Index Texture |
| 73 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 74 | function_input | `MaterialExpressionFunctionInput` | Input X-axis and X-Extent Texture (Texture2D) | X-axis and X-Extent Texture |
| 75 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 76 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'WindTurbulenceVectorAndGustMagnitude' |  |
| 77 | function_input | `MaterialExpressionFunctionInput` | Input Wind Speed Horizontal Speed (Scalar) | Wind Speed Horizontal Speed |
| 78 | expression | `MaterialExpressionAppendVector` | Append |  |
| 79 | expression | `MaterialExpressionDivide` | Divide |  |
| 80 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 81 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'X-Vector And X-Extent Texture' |  |
| 82 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'Position and Index Texture' |  |
| 83 | expression | `MaterialExpressionVectorParameter` | Param (1,0,0,1) 'Normalized Wind Direction X Axis' |  |
| 84 | expression | `MaterialExpressionVectorParameter` | Param (0,1,0,1) 'Normalized Wind Direction Y Axis' |  |
| 85 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind Speed' |  |
| 86 | expression | `MaterialExpressionScalarParameter` | Param (1) 'Wind Horizontal Speed ' |  |
| 87 | expression | `MaterialExpressionScalarParameter` | Param (1) 'Wind Gust World Scale' |  |
| 88 | function_input | `MaterialExpressionFunctionInput` | Input Wind Gust World Scale (Scalar) | Wind Gust World Scale |
| 89 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 90 | expression | `MaterialExpressionPower` | Power(X, 5) |  |
| 91 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 92 | expression | `MaterialExpressionConstant3Vector` | 0,0,-0.2 |  |
| 93 | expression | `MaterialExpressionAdd` | Add |  |
| 94 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 95 | function_input | `MaterialExpressionFunctionInput` | Input Use Wind Shelter Settings (StaticBool) | Use Wind Shelter Settings |
| 96 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 97 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_UnpackIntegerAsFloat |  |
| 98 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 99 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 100 | expression | `MaterialExpressionAdd` | Add |  |
| 101 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Manually Control Pivot Painter 2 Foliage Animation Time' |  |
| 102 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Expose Previous Frame Time' |  |
| 103 | expression | `MaterialExpressionScalarParameter` | Param (0) 'PreviousTime' |  |
| 104 | expression | `MaterialExpressionScalarParameter` | Param (0) 'CurrentTime' |  |
| 105 | expression | `MaterialExpressionPreviousFrameSwitch` | PreviousFrameSwitch |  |
| 106 | expression | `MaterialExpressionScalarParameter` | Param (0) 'CurrentTime' |  |
| 107 | expression | `MaterialExpressionDistance` | Distance |  |
| 108 | expression | `MaterialExpressionConstant` | 0 |  |
| 109 | expression | `MaterialExpressionDivide` | Divide |  |
| 110 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 111 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 112 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 113 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 114 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 115 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 116 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 117 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 118 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 119 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 120 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 121 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 122 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 123 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 124 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 125 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 126 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 127 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 128 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 129 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 130 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 131 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 132 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 133 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 134 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 135 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 136 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 137 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 138 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output Parent Index Integer

- `A` <= [112] Reroute Node (reroutes wires) output 0

### [1] RotateAboutAxis

- `NormalizedRotationAxis` <= [128] Reroute Node (reroutes wires) output 0
- `RotationAngle` <= [129] Reroute Node (reroutes wires) output 0
- `PivotPoint` <= [119] Reroute Node (reroutes wires) output 0
- `Position` <= [2] Absolute World Position (Excluding Material Offsets) output 0

### [7] Cross

- `A` <= [123] Reroute Node (reroutes wires) output 0
- `B` <= [93] Add output 0

### [8] Dot

- `A` <= [126] Reroute Node (reroutes wires) output 0
- `B` <= [100] Add output 0

### [9] Dot

- `A` <= [71] Input Wind Direction Y Axis (Vector3) output 0
- `B` <= [125] Reroute Node (reroutes wires) output 0

### [10] Divide

- `A` <= [135] Reroute Node (reroutes wires) output 0
- `B` <= [88] Input Wind Gust World Scale (Scalar) output 0

### [11] Append

- `A` <= [8] Dot output 0
- `B` <= [9] Dot output 0

### [12] Add

- `A` <= [13] Input Wind Gust Offset (Scalar) output 0
- `B` <= [134] Reroute Node (reroutes wires) output 0

### [15] Output World Position Offset

- `A` <= [40] Switch output 0

### [18] Multiply

- `A` <= [65] Add output 0
- `B` <= [114] Reroute Node (reroutes wires) output 0

### [20] Frac

- `Input` <= [21] Add output 0

### [21] Add

- `A` <= [22] Multiply output 0
- `B` <= [10] Divide output 0

### [22] Multiply

- `A` <= [133] Reroute Node (reroutes wires) output 0
- `B` <= [78] Append output 0

### [23] Multiply

- `A` <= [14] Input Wind Gust Angle Rotation (Scalar) output 0
- `B` <= [12] Add output 0

### [25] Multiply

- `A` <= [17] Input Motion Dampening Falloff Radius (Scalar) output 0
- `B` <= [24] ms_PivotPainter2_Decode8BitAlphaAxisExtent output 0

### [27] Input Pivot Painter Texture Coordinate (Vector2)

- `Preview` <= [26] TexCoord[1] output 0

### [28] Output Parent UVs

- `A` <= [3] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [29] Input Wind Speed Down Wind Vector (Scalar)

- `Preview` <= [85] Param (0.25) 'Wind Speed' output 0

### [30] Dot

- `A` <= [70] Input Wind Direction (Vector3) output 0
- `B` <= [121] Reroute Node (reroutes wires) output 0

### [31] Multiply

- `A` <= [66] Output Advanced - Rotation Angle Animation output 0
- `B` <= [56] Clamp output 0

### [32] Input Optional - world space pixel normal (Vector3)

- `Preview` <= [75] VertexNormalWS output 0

### [33] Output Rotated World Space Pixel Normal

- `A` <= [45] Switch output 0

### [36] Multiply

- `A` <= [1] RotateAboutAxis output 0
- `B` <= [35] Input Layer Mask (Scalar) output 0

### [37] Lerp

- `A` <= [130] Reroute Node (reroutes wires) output 0
- `B` <= [34] FixRotateAboutAxisNormals output 3
- `Alpha` <= [35] Input Layer Mask (Scalar) output 0

### [38] Input Enable (StaticBool)

- `Preview` <= [39] Static Bool (True) output 0

### [40] Switch

- `A` <= [36] Multiply output 0
- `B` <= [41] 0 output 0
- `Value` <= [132] Reroute Node (reroutes wires) output 0

### [42] Switch

- `A` <= [37] Lerp output 0
- `B` <= [131] Reroute Node (reroutes wires) output 0
- `Value` <= [43] Input Enable Normal Rotation (StaticBool) output 0

### [43] Input Enable Normal Rotation (StaticBool)

- `Preview` <= [44] Static Bool (True) output 0

### [45] Switch

- `A` <= [42] Switch output 0
- `B` <= [131] Reroute Node (reroutes wires) output 0
- `Value` <= [132] Reroute Node (reroutes wires) output 0

### [47] Add

- `A` <= [30] Dot output 0
- `B` <= [49] BreakOutFloat2Components output 0

### [48] Multiply

- `A` <= [47] Add output 0
- `B` <= [49] BreakOutFloat2Components output 1

### [50] Add

- `A` <= [15] Output World Position Offset output 0
- `B` <= [51] Absolute World Position (Excluding Material Offsets) output 0

### [52] Output Final World Position

- `A` <= [50] Add output 0

### [53] Dot

- `A` <= [79] Divide output 0
- `B` <= [113] Reroute Node (reroutes wires) output 0

### [54] Subtract

- `A` <= [16] Absolute World Position (Excluding Material Offsets) output 0
- `B` <= [110] Reroute Node (reroutes wires) output 0

### [55] Divide

- `A` <= [53] Dot output 0
- `B` <= [137] Reroute Node (reroutes wires) output 0

### [56] Clamp

- `Input` <= [55] Divide output 0

### [57] Texture Sample

- `Coordinates` <= [62] Frac output 0
- `TextureObject` <= [68] Input Optional - Wind Turbulence and Gust Mag (Texture2D) output 0

### [58] Add

- `A` <= [59] Multiply output 0
- `B` <= [7] Cross output 0

### [59] Multiply

- `A` <= [60] Input Random Rotation Influence (Scalar) output 0
- `B` <= [98] Subtract(,0.5) output 0

### [62] Frac

- `Input` <= [63] Divide output 0

### [63] Divide

- `A` <= [136] Reroute Node (reroutes wires) output 0
- `B` <= [61] Input Random Rotation Texture Sample Scale (Scalar) output 0

### [65] Add

- `A` <= [64] Input Advanced - Parent Rotation Angle Influence (Scalar) output 0
- `B` <= [23] Multiply output 0

### [66] Output Advanced - Rotation Angle Animation

- `A` <= [94] Switch output 0

### [67] Texture Sample

- `Coordinates` <= [20] Frac output 0
- `TextureObject` <= [68] Input Optional - Wind Turbulence and Gust Mag (Texture2D) output 0

### [68] Input Optional - Wind Turbulence and Gust Mag (Texture2D)

- `Preview` <= [76] Param Tex Object 'WindTurbulenceVectorAndGustMagnitude' output 0

### [69] Texture Sample

- `Coordinates` <= [27] Input Pivot Painter Texture Coordinate (Vector2) output 0
- `TextureObject` <= [74] Input X-axis and X-Extent Texture (Texture2D) output 0

### [70] Input Wind Direction (Vector3)

- `Preview` <= [83] Param (1,0,0,1) 'Normalized Wind Direction X Axis' output 0

### [71] Input Wind Direction Y Axis (Vector3)

- `Preview` <= [84] Param (0,1,0,1) 'Normalized Wind Direction Y Axis' output 0

### [72] Input Position And Index Texture (Texture2D)

- `Preview` <= [82] Param Tex Object 'Position and Index Texture' output 0

### [73] Texture Sample

- `Coordinates` <= [27] Input Pivot Painter Texture Coordinate (Vector2) output 0
- `TextureObject` <= [72] Input Position And Index Texture (Texture2D) output 0

### [74] Input X-axis and X-Extent Texture (Texture2D)

- `Preview` <= [81] Param Tex Object 'X-Vector And X-Extent Texture' output 0

### [77] Input Wind Speed Horizontal Speed (Scalar)

- `Preview` <= [86] Param (1) 'Wind Horizontal Speed ' output 0

### [78] Append

- `A` <= [29] Input Wind Speed Down Wind Vector (Scalar) output 0
- `B` <= [77] Input Wind Speed Horizontal Speed (Scalar) output 0

### [79] Divide

- `A` <= [54] Subtract output 0
- `B` <= [80] ObjectScale output 0

### [88] Input Wind Gust World Scale (Scalar)

- `Preview` <= [87] Param (1) 'Wind Gust World Scale' output 0

### [89] Dot

- `A` <= [124] Reroute Node (reroutes wires) output 0
- `B` <= [122] Reroute Node (reroutes wires) output 0

### [90] Power(X, 5)

- `Base` <= [89] Dot output 0

### [91] Multiply

- `A` <= [90] Power(X, 5) output 0
- `B` <= [92] 0,0,-0.2 output 0

### [93] Add

- `A` <= [91] Multiply output 0
- `B` <= [124] Reroute Node (reroutes wires) output 0

### [94] Switch

- `A` <= [18] Multiply output 0
- `B` <= [65] Add output 0
- `Value` <= [95] Input Use Wind Shelter Settings (StaticBool) output 0

### [95] Input Use Wind Shelter Settings (StaticBool)

- `Preview` <= [96] Static Bool (True) output 0

### [98] Subtract(,0.5)

- `A` <= [57] Texture Sample output 0

### [99] Multiply

- `A` <= [6] ms_PivotPainter2_DecodeAxisVector output 0
- `B` <= [24] ms_PivotPainter2_Decode8BitAlphaAxisExtent output 0

### [100] Add

- `A` <= [5] ms_PivotPainter2_DecodePostion output 0
- `B` <= [99] Multiply output 0

### [101] Switch Param (False) 'Manually Control Pivot Painter 2 Foliage Animation Time'

- `A` <= [102] Switch Param (False) 'Expose Previous Frame Time' output 0
- `B` <= [19] Time (Period of 99999997952.00) output 0

### [102] Switch Param (False) 'Expose Previous Frame Time'

- `A` <= [105] PreviousFrameSwitch output 0
- `B` <= [106] Param (0) 'CurrentTime' output 0

### [105] PreviousFrameSwitch

- `CurrentFrame` <= [104] Param (0) 'CurrentTime' output 0
- `PreviousFrame` <= [103] Param (0) 'PreviousTime' output 0

### [107] Distance

- `A` <= [58] Add output 0
- `B` <= [108] 0 output 0

### [109] Divide

- `A` <= [58] Add output 0
- `B` <= [107] Distance output 0

### [110] Reroute Node (reroutes wires)

- `Input` <= [5] ms_PivotPainter2_DecodePostion output 0

### [111] Reroute Node (reroutes wires)

- `Input` <= [110] Reroute Node (reroutes wires) output 0

### [112] Reroute Node (reroutes wires)

- `Input` <= [73] Texture Sample output 4

### [113] Reroute Node (reroutes wires)

- `Input` <= [117] Normalize output 0

### [114] Reroute Node (reroutes wires)

- `Input` <= [48] Multiply output 0

### [115] Reroute Node (reroutes wires)

- `Input` <= [116] Reroute Node (reroutes wires) output 0

### [116] Reroute Node (reroutes wires)

- `Input` <= [109] Divide output 0

### [117] Normalize

- `VectorInput` <= [118] Reroute Node (reroutes wires) output 0

### [118] Reroute Node (reroutes wires)

- `Input` <= [6] ms_PivotPainter2_DecodeAxisVector output 0

### [119] Reroute Node (reroutes wires)

- `Input` <= [120] Reroute Node (reroutes wires) output 0

### [120] Reroute Node (reroutes wires)

- `Input` <= [111] Reroute Node (reroutes wires) output 0

### [121] Reroute Node (reroutes wires)

- `Input` <= [6] ms_PivotPainter2_DecodeAxisVector output 0

### [122] Reroute Node (reroutes wires)

- `Input` <= [121] Reroute Node (reroutes wires) output 0

### [123] Reroute Node (reroutes wires)

- `Input` <= [122] Reroute Node (reroutes wires) output 0

### [124] Reroute Node (reroutes wires)

- `Input` <= [126] Reroute Node (reroutes wires) output 0

### [125] Reroute Node (reroutes wires)

- `Input` <= [100] Add output 0

### [126] Reroute Node (reroutes wires)

- `Input` <= [70] Input Wind Direction (Vector3) output 0

### [127] Reroute Node (reroutes wires)

- `Input` <= [115] Reroute Node (reroutes wires) output 0

### [128] Reroute Node (reroutes wires)

- `Input` <= [127] Reroute Node (reroutes wires) output 0

### [129] Reroute Node (reroutes wires)

- `Input` <= [138] Reroute Node (reroutes wires) output 0

### [130] Reroute Node (reroutes wires)

- `Input` <= [32] Input Optional - world space pixel normal (Vector3) output 0

### [131] Reroute Node (reroutes wires)

- `Input` <= [130] Reroute Node (reroutes wires) output 0

### [132] Reroute Node (reroutes wires)

- `Input` <= [38] Input Enable (StaticBool) output 0

### [133] Reroute Node (reroutes wires)

- `Input` <= [101] Switch Param (False) 'Manually Control Pivot Painter 2 Foliage Animation Time' output 0

### [134] Reroute Node (reroutes wires)

- `Input` <= [67] Texture Sample output 4

### [135] Reroute Node (reroutes wires)

- `Input` <= [11] Append output 0

### [136] Reroute Node (reroutes wires)

- `Input` <= [21] Add output 0

### [137] Reroute Node (reroutes wires)

- `Input` <= [25] Multiply output 0

### [138] Reroute Node (reroutes wires)

- `Input` <= [31] Multiply output 0

## Connection List

- [112] Reroute Node (reroutes wires) output 0 -> Output Parent Index Integer.A
- [128] Reroute Node (reroutes wires) output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [129] Reroute Node (reroutes wires) output 0 -> RotateAboutAxis.RotationAngle
- [119] Reroute Node (reroutes wires) output 0 -> RotateAboutAxis.PivotPoint
- [2] Absolute World Position (Excluding Material Offsets) output 0 -> RotateAboutAxis.Position
- [123] Reroute Node (reroutes wires) output 0 -> Cross.A
- [93] Add output 0 -> Cross.B
- [126] Reroute Node (reroutes wires) output 0 -> Dot.A
- [100] Add output 0 -> Dot.B
- [71] Input Wind Direction Y Axis (Vector3) output 0 -> Dot.A
- [125] Reroute Node (reroutes wires) output 0 -> Dot.B
- [135] Reroute Node (reroutes wires) output 0 -> Divide.A
- [88] Input Wind Gust World Scale (Scalar) output 0 -> Divide.B
- [8] Dot output 0 -> Append.A
- [9] Dot output 0 -> Append.B
- [13] Input Wind Gust Offset (Scalar) output 0 -> Add.A
- [134] Reroute Node (reroutes wires) output 0 -> Add.B
- [40] Switch output 0 -> Output World Position Offset.A
- [65] Add output 0 -> Multiply.A
- [114] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [21] Add output 0 -> Frac.Input
- [22] Multiply output 0 -> Add.A
- [10] Divide output 0 -> Add.B
- [133] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [78] Append output 0 -> Multiply.B
- [14] Input Wind Gust Angle Rotation (Scalar) output 0 -> Multiply.A
- [12] Add output 0 -> Multiply.B
- [17] Input Motion Dampening Falloff Radius (Scalar) output 0 -> Multiply.A
- [24] ms_PivotPainter2_Decode8BitAlphaAxisExtent output 0 -> Multiply.B
- [26] TexCoord[1] output 0 -> Input Pivot Painter Texture Coordinate (Vector2).Preview
- [3] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Output Parent UVs.A
- [85] Param (0.25) 'Wind Speed' output 0 -> Input Wind Speed Down Wind Vector (Scalar).Preview
- [70] Input Wind Direction (Vector3) output 0 -> Dot.A
- [121] Reroute Node (reroutes wires) output 0 -> Dot.B
- [66] Output Advanced - Rotation Angle Animation output 0 -> Multiply.A
- [56] Clamp output 0 -> Multiply.B
- [75] VertexNormalWS output 0 -> Input Optional - world space pixel normal (Vector3).Preview
- [45] Switch output 0 -> Output Rotated World Space Pixel Normal.A
- [1] RotateAboutAxis output 0 -> Multiply.A
- [35] Input Layer Mask (Scalar) output 0 -> Multiply.B
- [130] Reroute Node (reroutes wires) output 0 -> Lerp.A
- [34] FixRotateAboutAxisNormals output 3 -> Lerp.B
- [35] Input Layer Mask (Scalar) output 0 -> Lerp.Alpha
- [39] Static Bool (True) output 0 -> Input Enable (StaticBool).Preview
- [36] Multiply output 0 -> Switch.A
- [41] 0 output 0 -> Switch.B
- [132] Reroute Node (reroutes wires) output 0 -> Switch.Value
- [37] Lerp output 0 -> Switch.A
- [131] Reroute Node (reroutes wires) output 0 -> Switch.B
- [43] Input Enable Normal Rotation (StaticBool) output 0 -> Switch.Value
- [44] Static Bool (True) output 0 -> Input Enable Normal Rotation (StaticBool).Preview
- [42] Switch output 0 -> Switch.A
- [131] Reroute Node (reroutes wires) output 0 -> Switch.B
- [132] Reroute Node (reroutes wires) output 0 -> Switch.Value
- [30] Dot output 0 -> Add.A
- [49] BreakOutFloat2Components output 0 -> Add.B
- [47] Add output 0 -> Multiply.A
- [49] BreakOutFloat2Components output 1 -> Multiply.B
- [15] Output World Position Offset output 0 -> Add.A
- [51] Absolute World Position (Excluding Material Offsets) output 0 -> Add.B
- [50] Add output 0 -> Output Final World Position.A
- [79] Divide output 0 -> Dot.A
- [113] Reroute Node (reroutes wires) output 0 -> Dot.B
- [16] Absolute World Position (Excluding Material Offsets) output 0 -> Subtract.A
- [110] Reroute Node (reroutes wires) output 0 -> Subtract.B
- [53] Dot output 0 -> Divide.A
- [137] Reroute Node (reroutes wires) output 0 -> Divide.B
- [55] Divide output 0 -> Clamp.Input
- [62] Frac output 0 -> Texture Sample.Coordinates
- [68] Input Optional - Wind Turbulence and Gust Mag (Texture2D) output 0 -> Texture Sample.TextureObject
- [59] Multiply output 0 -> Add.A
- [7] Cross output 0 -> Add.B
- [60] Input Random Rotation Influence (Scalar) output 0 -> Multiply.A
- [98] Subtract(,0.5) output 0 -> Multiply.B
- [63] Divide output 0 -> Frac.Input
- [136] Reroute Node (reroutes wires) output 0 -> Divide.A
- [61] Input Random Rotation Texture Sample Scale (Scalar) output 0 -> Divide.B
- [64] Input Advanced - Parent Rotation Angle Influence (Scalar) output 0 -> Add.A
- [23] Multiply output 0 -> Add.B
- [94] Switch output 0 -> Output Advanced - Rotation Angle Animation.A
- [20] Frac output 0 -> Texture Sample.Coordinates
- [68] Input Optional - Wind Turbulence and Gust Mag (Texture2D) output 0 -> Texture Sample.TextureObject
- [76] Param Tex Object 'WindTurbulenceVectorAndGustMagnitude' output 0 -> Input Optional - Wind Turbulence and Gust Mag (Texture2D).Preview
- [27] Input Pivot Painter Texture Coordinate (Vector2) output 0 -> Texture Sample.Coordinates
- [74] Input X-axis and X-Extent Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [83] Param (1,0,0,1) 'Normalized Wind Direction X Axis' output 0 -> Input Wind Direction (Vector3).Preview
- [84] Param (0,1,0,1) 'Normalized Wind Direction Y Axis' output 0 -> Input Wind Direction Y Axis (Vector3).Preview
- [82] Param Tex Object 'Position and Index Texture' output 0 -> Input Position And Index Texture (Texture2D).Preview
- [27] Input Pivot Painter Texture Coordinate (Vector2) output 0 -> Texture Sample.Coordinates
- [72] Input Position And Index Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [81] Param Tex Object 'X-Vector And X-Extent Texture' output 0 -> Input X-axis and X-Extent Texture (Texture2D).Preview
- [86] Param (1) 'Wind Horizontal Speed ' output 0 -> Input Wind Speed Horizontal Speed (Scalar).Preview
- [29] Input Wind Speed Down Wind Vector (Scalar) output 0 -> Append.A
- [77] Input Wind Speed Horizontal Speed (Scalar) output 0 -> Append.B
- [54] Subtract output 0 -> Divide.A
- [80] ObjectScale output 0 -> Divide.B
- [87] Param (1) 'Wind Gust World Scale' output 0 -> Input Wind Gust World Scale (Scalar).Preview
- [124] Reroute Node (reroutes wires) output 0 -> Dot.A
- [122] Reroute Node (reroutes wires) output 0 -> Dot.B
- [89] Dot output 0 -> Power(X, 5).Base
- [90] Power(X, 5) output 0 -> Multiply.A
- [92] 0,0,-0.2 output 0 -> Multiply.B
- [91] Multiply output 0 -> Add.A
- [124] Reroute Node (reroutes wires) output 0 -> Add.B
- [18] Multiply output 0 -> Switch.A
- [65] Add output 0 -> Switch.B
- [95] Input Use Wind Shelter Settings (StaticBool) output 0 -> Switch.Value
- [96] Static Bool (True) output 0 -> Input Use Wind Shelter Settings (StaticBool).Preview
- [57] Texture Sample output 0 -> Subtract(,0.5).A
- [6] ms_PivotPainter2_DecodeAxisVector output 0 -> Multiply.A
- [24] ms_PivotPainter2_Decode8BitAlphaAxisExtent output 0 -> Multiply.B
- [5] ms_PivotPainter2_DecodePostion output 0 -> Add.A
- [99] Multiply output 0 -> Add.B
- [102] Switch Param (False) 'Expose Previous Frame Time' output 0 -> Switch Param (False) 'Manually Control Pivot Painter 2 Foliage Animation Time'.A
- [19] Time (Period of 99999997952.00) output 0 -> Switch Param (False) 'Manually Control Pivot Painter 2 Foliage Animation Time'.B
- [105] PreviousFrameSwitch output 0 -> Switch Param (False) 'Expose Previous Frame Time'.A
- [106] Param (0) 'CurrentTime' output 0 -> Switch Param (False) 'Expose Previous Frame Time'.B
- [104] Param (0) 'CurrentTime' output 0 -> PreviousFrameSwitch.CurrentFrame
- [103] Param (0) 'PreviousTime' output 0 -> PreviousFrameSwitch.PreviousFrame
- [58] Add output 0 -> Distance.A
- [108] 0 output 0 -> Distance.B
- [58] Add output 0 -> Divide.A
- [107] Distance output 0 -> Divide.B
- [5] ms_PivotPainter2_DecodePostion output 0 -> Reroute Node (reroutes wires).Input
- [110] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [73] Texture Sample output 4 -> Reroute Node (reroutes wires).Input
- [117] Normalize output 0 -> Reroute Node (reroutes wires).Input
- [48] Multiply output 0 -> Reroute Node (reroutes wires).Input
- [116] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [109] Divide output 0 -> Reroute Node (reroutes wires).Input
- [118] Reroute Node (reroutes wires) output 0 -> Normalize.VectorInput
- [6] ms_PivotPainter2_DecodeAxisVector output 0 -> Reroute Node (reroutes wires).Input
- [120] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [111] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [6] ms_PivotPainter2_DecodeAxisVector output 0 -> Reroute Node (reroutes wires).Input
- [121] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [122] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [126] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [100] Add output 0 -> Reroute Node (reroutes wires).Input
- [70] Input Wind Direction (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [115] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [127] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [138] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [32] Input Optional - world space pixel normal (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [130] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [38] Input Enable (StaticBool) output 0 -> Reroute Node (reroutes wires).Input
- [101] Switch Param (False) 'Manually Control Pivot Painter 2 Foliage Animation Time' output 0 -> Reroute Node (reroutes wires).Input
- [67] Texture Sample output 4 -> Reroute Node (reroutes wires).Input
- [11] Append output 0 -> Reroute Node (reroutes wires).Input
- [21] Add output 0 -> Reroute Node (reroutes wires).Input
- [25] Multiply output 0 -> Reroute Node (reroutes wires).Input
- [31] Multiply output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `Parent Index Integer` is driven by [112] Reroute Node (reroutes wires)
- `World Position Offset` is driven by [40] Switch
- `Parent UVs` is driven by [3] ms_PivotPainter2_ReturnParentTextureInfo
- `Rotated World Space Pixel Normal` is driven by [45] Switch
- `Final World Position` is driven by [50] Add
- `Advanced - Rotation Angle Animation` is driven by [94] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


