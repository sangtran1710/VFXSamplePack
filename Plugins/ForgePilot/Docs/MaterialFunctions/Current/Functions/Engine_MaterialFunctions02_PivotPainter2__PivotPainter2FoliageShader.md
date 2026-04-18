# PivotPainter2FoliageShader

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/PivotPainter2FoliageShader.PivotPainter2FoliageShader`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This material function contains texture and numeric parameters that should be altered to fit your particular asset. It is suggested that this be done at the material instance level. 

## Inputs

- `Material Attributes (See Note)` [FunctionInput_MaterialAttributes] - Please make sure that you are using tangent space normals within your material and that the use tangent space normals option is unchecked. The normals will be transformed to world space internally.
- `Pivot Painter Texture Coordinate` [FunctionInput_Vector2] - Defaults to UV Coordinate 1.

## Outputs

- `World Position Offset Component`
- `Modified World Space Normal Component`
- `Final Material with World Space Normals`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset Component | World Position Offset Component |
| 1 | expression | `MaterialExpressionScalarParameter` | Param (-0.444) 'Wind 4 Shelter Dot Product Modulation ' |  |
| 2 | expression | `MaterialExpressionScalarParameter` | Param (-1.25) 'Wind 4 Shelter Dot Product Bias ' |  |
| 3 | expression | `MaterialExpressionAppendVector` | Append |  |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionScalarParameter` | Param (1) 'Wind 3's Angle Contribution to Wind 4' |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionConstant` | 0 |  |
| 8 | expression | `MaterialExpressionConstant` | 1 |  |
| 9 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind Speed' |  |
| 10 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind Speed' |  |
| 11 | expression | `MaterialExpressionScalarParameter` | Param (200) 'Wind 4 Random Rotation Change Rate' |  |
| 12 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind 4 Random Rotation Influence' |  |
| 13 | expression | `MaterialExpressionScalarParameter` | Param (200) 'Wind 3 Random Rotation Change Rate' |  |
| 14 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind 3 Random Rotation Influence' |  |
| 15 | expression | `MaterialExpressionScalarParameter` | Param (200) 'Wind 2 Random Rotation Change Rate' |  |
| 16 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind 2 Random Rotation Influence' |  |
| 17 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind 1 Random Rotation Influence' |  |
| 18 | expression | `MaterialExpressionScalarParameter` | Param (200) 'Wind 1 Random Rotation Change Rate' |  |
| 19 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 1 Normals' |  |
| 20 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 2 Normals' |  |
| 21 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 4' |  |
| 22 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 4 Normals' |  |
| 23 | expression | `MaterialExpressionTextureProperty` | Texture Size |  |
| 24 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'Position and Index Texture' |  |
| 25 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 3 Normals' |  |
| 26 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 3' |  |
| 27 | expression | `MaterialExpressionClamp` | Clamp |  |
| 28 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 2' |  |
| 29 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Animate Level 1' |  |
| 30 | expression | `MaterialExpressionScalarParameter` | Param (2) 'Local Wind 4 Speed' |  |
| 31 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind Angle Offset 1' |  |
| 32 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind Angle offset 2' |  |
| 33 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind Angle Offset 3' |  |
| 34 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind Angle Offset 4' |  |
| 35 | expression | `MaterialExpressionScalarParameter` | Param (2) 'Dampening Radius Multiplier 4' |  |
| 36 | expression | `MaterialExpressionScalarParameter` | Param (2) 'Dampening Radius Multiplier 3' |  |
| 37 | expression | `MaterialExpressionScalarParameter` | Param (2) 'Dampening Radius Multiplier 2' |  |
| 38 | expression | `MaterialExpressionScalarParameter` | Param (2) 'Dampening Radius Multiplier 1' |  |
| 39 | expression | `MaterialExpressionScalarParameter` | Param (0.009174) 'Max Rotation 4' |  |
| 40 | expression | `MaterialExpressionScalarParameter` | Param (0.009174) 'Max Rotation 3' |  |
| 41 | expression | `MaterialExpressionScalarParameter` | Param (0.009174) 'Max Rotation 2' |  |
| 42 | expression | `MaterialExpressionScalarParameter` | Param (0.009174) 'Max Rotation 1' |  |
| 43 | expression | `MaterialExpressionAdd` | Add |  |
| 44 | expression | `MaterialExpressionAdd` | Add |  |
| 45 | expression | `MaterialExpressionClamp` | Clamp |  |
| 46 | function_call | `MaterialExpressionMaterialFunctionCall` | PivotPainter2FoliageAnimation |  |
| 47 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 48 | expression | `MaterialExpressionClamp` | Clamp |  |
| 49 | expression | `MaterialExpressionConstant` | 2 |  |
| 50 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 51 | expression | `MaterialExpressionConstant` | 1 |  |
| 52 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 53 | expression | `MaterialExpressionCeil` | Ceil |  |
| 54 | expression | `MaterialExpressionClamp` | Clamp |  |
| 55 | expression | `MaterialExpressionDivide` | Divide(,2) |  |
| 56 | function_call | `MaterialExpressionMaterialFunctionCall` | Lerp_Multiple_Float2 |  |
| 57 | expression | `MaterialExpressionDivide` | Divide(,3) |  |
| 58 | function_call | `MaterialExpressionMaterialFunctionCall` | Lerp_Multiple_Float2 |  |
| 59 | function_call | `MaterialExpressionMaterialFunctionCall` | PivotPainter2FoliageAnimation |  |
| 60 | function_call | `MaterialExpressionMaterialFunctionCall` | PivotPainter2FoliageAnimation |  |
| 61 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_UnpackIntegerAsFloat |  |
| 62 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_UnpackIntegerAsFloat |  |
| 63 | expression | `MaterialExpressionAdd` | Add |  |
| 64 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_ReturnParentTextureInfo |  |
| 65 | expression | `MaterialExpressionTextureSampleParameter2D` | Param2D 'Position and Index Texture' |  |
| 66 | expression | `MaterialExpressionAdd` | Add |  |
| 67 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_ReturnParentTextureInfo |  |
| 68 | expression | `MaterialExpressionTextureSampleParameter2D` | Param2D 'Position and Index Texture' |  |
| 69 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_CalculateMeshElementIndex |  |
| 70 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 71 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_ReturnParentTextureInfo |  |
| 72 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_UnpackIntegerAsFloat |  |
| 73 | expression | `MaterialExpressionTextureProperty` | Texture Size |  |
| 74 | expression | `MaterialExpressionTextureSampleParameter2D` | Param2D 'Position and Index Texture' |  |
| 75 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'Position and Index Texture' |  |
| 76 | expression | `MaterialExpressionAdd` | Add |  |
| 77 | function_call | `MaterialExpressionMaterialFunctionCall` | PivotPainter2FoliageAnimation |  |
| 78 | function_output | `MaterialExpressionFunctionOutput` | Output Modified World Space Normal Component | Modified World Space Normal Component |
| 79 | expression | `MaterialExpressionAppendVector` | Append |  |
| 80 | expression | `MaterialExpressionScalarParameter` | Param (-1.25) 'Wind 3 Shelter Dot Product Bias ' |  |
| 81 | expression | `MaterialExpressionScalarParameter` | Param (-0.444) 'Wind 3 Shelter Dot Product Modulation ' |  |
| 82 | expression | `MaterialExpressionAppendVector` | Append |  |
| 83 | expression | `MaterialExpressionScalarParameter` | Param (-1.25) 'Wind 2 Shelter Dot Product Bias ' |  |
| 84 | expression | `MaterialExpressionScalarParameter` | Param (-0.444) 'Wind 2 Shelter Dot Product Modulation ' |  |
| 85 | expression | `MaterialExpressionAppendVector` | Append |  |
| 86 | expression | `MaterialExpressionScalarParameter` | Param (-1.25) 'Wind 1 Shelter Dot Product Bias ' |  |
| 87 | expression | `MaterialExpressionScalarParameter` | Param (-0.444) 'Wind 1 Shelter Dot Product Modulation ' |  |
| 88 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 89 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |
| 90 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 91 | function_input | `MaterialExpressionFunctionInput` | Input Material Attributes (See Note) (MaterialAttributes) | Material Attributes (See Note) |
| 92 | function_output | `MaterialExpressionFunctionOutput` | Output Final Material with World Space Normals | Final Material with World Space Normals |
| 93 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 94 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |
| 95 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind 1's Angle Contribution to Wind 2' |  |
| 96 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 2's Rotation' |  |
| 97 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 3's Rotation' |  |
| 98 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind 1's Angle Contribution to Wind 3' |  |
| 99 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 100 | expression | `MaterialExpressionConstant` | 0 |  |
| 101 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind 2's Angle Contribution to Wind 3' |  |
| 102 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 103 | expression | `MaterialExpressionAdd` | Add |  |
| 104 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 3's Rotation' |  |
| 105 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 4's Rotation' |  |
| 106 | expression | `MaterialExpressionAdd` | Add |  |
| 107 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind 2's Angle Contribution to Wind 4' |  |
| 108 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 109 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation' |  |
| 110 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Wind 1's Angle Contribution to Wind 4' |  |
| 111 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 112 | expression | `MaterialExpressionConstant` | 0 |  |
| 113 | expression | `MaterialExpressionStaticSwitchParameter` | Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation' |  |
| 114 | expression | `MaterialExpressionAdd` | Add |  |
| 115 | expression | `MaterialExpressionScalarParameter` | Param (1) 'Local Wind Scale 3' |  |
| 116 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 117 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'Position and Index Texture' |  |
| 118 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'X-Vector And X-Extent Texture' |  |
| 119 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 120 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 121 | expression | `MaterialExpressionScalarParameter` | Param (1) 'Local Wind Scale 2' |  |
| 122 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind Speed' |  |
| 123 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 124 | expression | `MaterialExpressionScalarParameter` | Param (1) 'Local Wind Scale 1' |  |
| 125 | expression | `MaterialExpressionScalarParameter` | Param (0.25) 'Wind Speed' |  |
| 126 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 127 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 128 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 129 | function_input | `MaterialExpressionFunctionInput` | Input Pivot Painter Texture Coordinate (Vector2) | Pivot Painter Texture Coordinate |
| 130 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 131 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 132 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 133 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 134 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 135 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 136 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 137 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 138 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 139 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 140 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 141 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 142 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 143 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 144 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 145 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 146 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 147 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 148 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 149 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 150 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 151 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 152 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 153 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 154 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 155 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 156 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 157 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 158 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 159 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 160 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 161 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 162 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 163 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 164 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 165 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 166 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 167 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 168 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 169 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 170 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 171 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 172 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 173 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 174 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output World Position Offset Component

- `A` <= [76] Add output 0

### [3] Append

- `A` <= [2] Param (-1.25) 'Wind 4 Shelter Dot Product Bias ' output 0
- `B` <= [1] Param (-0.444) 'Wind 4 Shelter Dot Product Modulation ' output 0

### [6] Multiply

- `A` <= [137] Reroute Node (reroutes wires) output 0
- `B` <= [5] Param (1) 'Wind 3's Angle Contribution to Wind 4' output 0

### [23] Texture Size

- `TextureObject` <= [24] Param Tex Object 'Position and Index Texture' output 0

### [27] Clamp

- `Input` <= [169] Reroute Node (reroutes wires) output 0

### [43] Add

- `A` <= [142] Reroute Node (reroutes wires) output 0
- `B` <= [59] PivotPainter2FoliageAnimation output 0

### [44] Add

- `A` <= [43] Add output 0
- `B` <= [46] PivotPainter2FoliageAnimation output 0

### [45] Clamp

- `Input` <= [148] Reroute Node (reroutes wires) output 0

### [47] Lerp

- `A` <= [158] Reroute Node (reroutes wires) output 0
- `B` <= [157] Reroute Node (reroutes wires) output 0
- `Alpha` <= [48] Clamp output 0

### [48] Clamp

- `Input` <= [50] Subtract output 0

### [50] Subtract

- `A` <= [169] Reroute Node (reroutes wires) output 0
- `B` <= [49] 2 output 0

### [52] Subtract

- `A` <= [172] Reroute Node (reroutes wires) output 0
- `B` <= [51] 1 output 0

### [53] Ceil

- `Input` <= [63] Add output 0

### [54] Clamp

- `Input` <= [55] Divide(,2) output 0

### [55] Divide(,2)

- `A` <= [52] Subtract output 0

### [57] Divide(,3)

- `A` <= [171] Reroute Node (reroutes wires) output 0

### [63] Add

- `A` <= [162] Reroute Node (reroutes wires) output 0
- `B` <= [64] ms_PivotPainter2_ReturnParentTextureInfo output 1

### [65] Param2D 'Position and Index Texture'

- `Coordinates` <= [67] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [66] Add

- `A` <= [67] ms_PivotPainter2_ReturnParentTextureInfo output 1
- `B` <= [165] Reroute Node (reroutes wires) output 0

### [68] Param2D 'Position and Index Texture'

- `Coordinates` <= [71] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [73] Texture Size

- `TextureObject` <= [75] Param Tex Object 'Position and Index Texture' output 0

### [74] Param2D 'Position and Index Texture'

- `Coordinates` <= [130] Reroute Node (reroutes wires) output 0

### [76] Add

- `A` <= [44] Add output 0
- `B` <= [146] Reroute Node (reroutes wires) output 0

### [78] Output Modified World Space Normal Component

- `A` <= [145] Reroute Node (reroutes wires) output 0

### [79] Append

- `A` <= [80] Param (-1.25) 'Wind 3 Shelter Dot Product Bias ' output 0
- `B` <= [81] Param (-0.444) 'Wind 3 Shelter Dot Product Modulation ' output 0

### [82] Append

- `A` <= [83] Param (-1.25) 'Wind 2 Shelter Dot Product Bias ' output 0
- `B` <= [84] Param (-0.444) 'Wind 2 Shelter Dot Product Modulation ' output 0

### [85] Append

- `A` <= [86] Param (-1.25) 'Wind 1 Shelter Dot Product Bias ' output 0
- `B` <= [87] Param (-0.444) 'Wind 1 Shelter Dot Product Modulation ' output 0

### [88] Multiply

- `A` <= [60] PivotPainter2FoliageAnimation output 5
- `B` <= [95] Param (0) 'Wind 1's Angle Contribution to Wind 2' output 0

### [91] Input Material Attributes (See Note) (MaterialAttributes)

- `Preview` <= [90] MakeMaterialAttributes output 0

### [92] Output Final Material with World Space Normals

- `A` <= [127] Feature Level Switch output 0

### [93] MakeMaterialAttributes

- `BaseColor` <= [94] BreakMaterialAttributes output 0
- `Metallic` <= [94] BreakMaterialAttributes output 1
- `Specular` <= [94] BreakMaterialAttributes output 2
- `Roughness` <= [94] BreakMaterialAttributes output 3
- `EmissiveColor` <= [94] BreakMaterialAttributes output 5
- `Opacity` <= [94] BreakMaterialAttributes output 6
- `OpacityMask` <= [94] BreakMaterialAttributes output 7
- `Normal` <= [78] Output Modified World Space Normal Component output 0
- `WorldPositionOffset` <= [0] Output World Position Offset Component output 0
- `SubsurfaceColor` <= [94] BreakMaterialAttributes output 11
- `ClearCoat` <= [94] BreakMaterialAttributes output 12
- `ClearCoatRoughness` <= [94] BreakMaterialAttributes output 13
- `AmbientOcclusion` <= [94] BreakMaterialAttributes output 14
- `Refraction` <= [94] BreakMaterialAttributes output 15
- `CustomizedUVs` <= [94] BreakMaterialAttributes output 16
- `PixelDepthOffset` <= [94] BreakMaterialAttributes output 24

### [96] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 2's Rotation'

- `A` <= [88] Multiply output 0
- `B` <= [4] 0 output 0

### [97] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 3's Rotation'

- `A` <= [102] Multiply output 0
- `B` <= [100] 0 output 0

### [99] Multiply

- `A` <= [144] Reroute Node (reroutes wires) output 0
- `B` <= [98] Param (0) 'Wind 1's Angle Contribution to Wind 3' output 0

### [102] Multiply

- `A` <= [139] Reroute Node (reroutes wires) output 0
- `B` <= [101] Param (0) 'Wind 2's Angle Contribution to Wind 3' output 0

### [103] Add

- `A` <= [104] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 3's Rotation' output 0
- `B` <= [97] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 3's Rotation' output 0

### [104] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 3's Rotation'

- `A` <= [99] Multiply output 0
- `B` <= [100] 0 output 0

### [105] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 4's Rotation'

- `A` <= [111] Multiply output 0
- `B` <= [112] 0 output 0

### [106] Add

- `A` <= [105] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 4's Rotation' output 0
- `B` <= [109] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation' output 0

### [108] Multiply

- `A` <= [140] Reroute Node (reroutes wires) output 0
- `B` <= [107] Param (0) 'Wind 2's Angle Contribution to Wind 4' output 0

### [109] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation'

- `A` <= [108] Multiply output 0
- `B` <= [112] 0 output 0

### [111] Multiply

- `A` <= [143] Reroute Node (reroutes wires) output 0
- `B` <= [110] Param (0) 'Wind 1's Angle Contribution to Wind 4' output 0

### [113] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation'

- `A` <= [6] Multiply output 0
- `B` <= [112] 0 output 0

### [114] Add

- `A` <= [106] Add output 0
- `B` <= [113] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation' output 0

### [116] Multiply

- `A` <= [115] Param (1) 'Local Wind Scale 3' output 0
- `B` <= [10] Param (0.25) 'Wind Speed' output 0

### [119] Multiply

- `A` <= [9] Param (0.25) 'Wind Speed' output 0
- `B` <= [30] Param (2) 'Local Wind 4 Speed' output 0

### [120] Multiply

- `A` <= [121] Param (1) 'Local Wind Scale 2' output 0
- `B` <= [122] Param (0.25) 'Wind Speed' output 0

### [123] Multiply

- `A` <= [124] Param (1) 'Local Wind Scale 1' output 0
- `B` <= [125] Param (0.25) 'Wind Speed' output 0

### [126] Tangent Space to World Space TransformVector

- `Input` <= [89] BreakMaterialAttributes output 8

### [127] Feature Level Switch

- `Default` <= [128] MakeMaterialAttributes output 0
- `Inputs` <= [128] MakeMaterialAttributes output 0

### [129] Input Pivot Painter Texture Coordinate (Vector2)

- `Preview` <= [70] TexCoord[1] output 0

### [130] Reroute Node (reroutes wires)

- `Input` <= [129] Input Pivot Painter Texture Coordinate (Vector2) output 0

### [131] Reroute Node (reroutes wires)

- `Input` <= [130] Reroute Node (reroutes wires) output 0

### [132] Reroute Node (reroutes wires)

- `Input` <= [131] Reroute Node (reroutes wires) output 0

### [133] Reroute Node (reroutes wires)

- `Input` <= [132] Reroute Node (reroutes wires) output 0

### [134] Reroute Node (reroutes wires)

- `Input` <= [132] Reroute Node (reroutes wires) output 0

### [135] Reroute Node (reroutes wires)

- `Input` <= [133] Reroute Node (reroutes wires) output 0

### [136] Reroute Node (reroutes wires)

- `Input` <= [46] PivotPainter2FoliageAnimation output 4

### [137] Reroute Node (reroutes wires)

- `Input` <= [46] PivotPainter2FoliageAnimation output 5

### [138] Reroute Node (reroutes wires)

- `Input` <= [48] Clamp output 0

### [139] Reroute Node (reroutes wires)

- `Input` <= [59] PivotPainter2FoliageAnimation output 5

### [140] Reroute Node (reroutes wires)

- `Input` <= [139] Reroute Node (reroutes wires) output 0

### [141] Reroute Node (reroutes wires)

- `Input` <= [59] PivotPainter2FoliageAnimation output 4

### [142] Reroute Node (reroutes wires)

- `Input` <= [60] PivotPainter2FoliageAnimation output 0

### [143] Reroute Node (reroutes wires)

- `Input` <= [144] Reroute Node (reroutes wires) output 0

### [144] Reroute Node (reroutes wires)

- `Input` <= [60] PivotPainter2FoliageAnimation output 5

### [145] Reroute Node (reroutes wires)

- `Input` <= [77] PivotPainter2FoliageAnimation output 4

### [146] Reroute Node (reroutes wires)

- `Input` <= [77] PivotPainter2FoliageAnimation output 0

### [147] Reroute Node (reroutes wires)

- `Input` <= [23] Texture Size output 0

### [148] Reroute Node (reroutes wires)

- `Input` <= [52] Subtract output 0

### [149] Reroute Node (reroutes wires)

- `Input` <= [23] Texture Size output 0

### [150] Reroute Node (reroutes wires)

- `Input` <= [47] Lerp output 0

### [151] Reroute Node (reroutes wires)

- `Input` <= [164] Reroute Node (reroutes wires) output 0

### [152] Reroute Node (reroutes wires)

- `Input` <= [73] Texture Size output 0

### [153] Reroute Node (reroutes wires)

- `Input` <= [152] Reroute Node (reroutes wires) output 0

### [154] Reroute Node (reroutes wires)

- `Input` <= [153] Reroute Node (reroutes wires) output 0

### [155] Reroute Node (reroutes wires)

- `Input` <= [64] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [156] Reroute Node (reroutes wires)

- `Input` <= [155] Reroute Node (reroutes wires) output 0

### [157] Reroute Node (reroutes wires)

- `Input` <= [151] Reroute Node (reroutes wires) output 0

### [158] Reroute Node (reroutes wires)

- `Input` <= [133] Reroute Node (reroutes wires) output 0

### [159] Reroute Node (reroutes wires)

- `Input` <= [58] Lerp_Multiple_Float2 output 1

### [160] Reroute Node (reroutes wires)

- `Input` <= [53] Ceil output 0

### [161] Reroute Node (reroutes wires)

- `Input` <= [67] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [162] Reroute Node (reroutes wires)

- `Input` <= [66] Add output 0

### [163] Reroute Node (reroutes wires)

- `Input` <= [67] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [164] Reroute Node (reroutes wires)

- `Input` <= [71] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [165] Reroute Node (reroutes wires)

- `Input` <= [71] ms_PivotPainter2_ReturnParentTextureInfo output 1

### [166] Reroute Node (reroutes wires)

- `Input` <= [167] Reroute Node (reroutes wires) output 0

### [167] Reroute Node (reroutes wires)

- `Input` <= [72] ms_PivotPainter2_UnpackIntegerAsFloat output 0

### [168] Reroute Node (reroutes wires)

- `Input` <= [152] Reroute Node (reroutes wires) output 0

### [169] Reroute Node (reroutes wires)

- `Input` <= [160] Reroute Node (reroutes wires) output 0

### [170] Reroute Node (reroutes wires)

- `Input` <= [164] Reroute Node (reroutes wires) output 0

### [171] Reroute Node (reroutes wires)

- `Input` <= [53] Ceil output 0

### [172] Reroute Node (reroutes wires)

- `Input` <= [53] Ceil output 0

### [173] Reroute Node (reroutes wires)

- `Input` <= [23] Texture Size output 0

### [174] Reroute Node (reroutes wires)

- `Input` <= [79] Append output 0

## Connection List

- [76] Add output 0 -> Output World Position Offset Component.A
- [2] Param (-1.25) 'Wind 4 Shelter Dot Product Bias ' output 0 -> Append.A
- [1] Param (-0.444) 'Wind 4 Shelter Dot Product Modulation ' output 0 -> Append.B
- [137] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [5] Param (1) 'Wind 3's Angle Contribution to Wind 4' output 0 -> Multiply.B
- [24] Param Tex Object 'Position and Index Texture' output 0 -> Texture Size.TextureObject
- [169] Reroute Node (reroutes wires) output 0 -> Clamp.Input
- [142] Reroute Node (reroutes wires) output 0 -> Add.A
- [59] PivotPainter2FoliageAnimation output 0 -> Add.B
- [43] Add output 0 -> Add.A
- [46] PivotPainter2FoliageAnimation output 0 -> Add.B
- [148] Reroute Node (reroutes wires) output 0 -> Clamp.Input
- [158] Reroute Node (reroutes wires) output 0 -> Lerp.A
- [157] Reroute Node (reroutes wires) output 0 -> Lerp.B
- [48] Clamp output 0 -> Lerp.Alpha
- [50] Subtract output 0 -> Clamp.Input
- [169] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [49] 2 output 0 -> Subtract.B
- [172] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [51] 1 output 0 -> Subtract.B
- [63] Add output 0 -> Ceil.Input
- [55] Divide(,2) output 0 -> Clamp.Input
- [52] Subtract output 0 -> Divide(,2).A
- [171] Reroute Node (reroutes wires) output 0 -> Divide(,3).A
- [162] Reroute Node (reroutes wires) output 0 -> Add.A
- [64] ms_PivotPainter2_ReturnParentTextureInfo output 1 -> Add.B
- [67] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Param2D 'Position and Index Texture'.Coordinates
- [67] ms_PivotPainter2_ReturnParentTextureInfo output 1 -> Add.A
- [165] Reroute Node (reroutes wires) output 0 -> Add.B
- [71] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Param2D 'Position and Index Texture'.Coordinates
- [75] Param Tex Object 'Position and Index Texture' output 0 -> Texture Size.TextureObject
- [130] Reroute Node (reroutes wires) output 0 -> Param2D 'Position and Index Texture'.Coordinates
- [44] Add output 0 -> Add.A
- [146] Reroute Node (reroutes wires) output 0 -> Add.B
- [145] Reroute Node (reroutes wires) output 0 -> Output Modified World Space Normal Component.A
- [80] Param (-1.25) 'Wind 3 Shelter Dot Product Bias ' output 0 -> Append.A
- [81] Param (-0.444) 'Wind 3 Shelter Dot Product Modulation ' output 0 -> Append.B
- [83] Param (-1.25) 'Wind 2 Shelter Dot Product Bias ' output 0 -> Append.A
- [84] Param (-0.444) 'Wind 2 Shelter Dot Product Modulation ' output 0 -> Append.B
- [86] Param (-1.25) 'Wind 1 Shelter Dot Product Bias ' output 0 -> Append.A
- [87] Param (-0.444) 'Wind 1 Shelter Dot Product Modulation ' output 0 -> Append.B
- [60] PivotPainter2FoliageAnimation output 5 -> Multiply.A
- [95] Param (0) 'Wind 1's Angle Contribution to Wind 2' output 0 -> Multiply.B
- [90] MakeMaterialAttributes output 0 -> Input Material Attributes (See Note) (MaterialAttributes).Preview
- [127] Feature Level Switch output 0 -> Output Final Material with World Space Normals.A
- [94] BreakMaterialAttributes output 0 -> MakeMaterialAttributes.BaseColor
- [94] BreakMaterialAttributes output 1 -> MakeMaterialAttributes.Metallic
- [94] BreakMaterialAttributes output 2 -> MakeMaterialAttributes.Specular
- [94] BreakMaterialAttributes output 3 -> MakeMaterialAttributes.Roughness
- [94] BreakMaterialAttributes output 5 -> MakeMaterialAttributes.EmissiveColor
- [94] BreakMaterialAttributes output 6 -> MakeMaterialAttributes.Opacity
- [94] BreakMaterialAttributes output 7 -> MakeMaterialAttributes.OpacityMask
- [78] Output Modified World Space Normal Component output 0 -> MakeMaterialAttributes.Normal
- [0] Output World Position Offset Component output 0 -> MakeMaterialAttributes.WorldPositionOffset
- [94] BreakMaterialAttributes output 11 -> MakeMaterialAttributes.SubsurfaceColor
- [94] BreakMaterialAttributes output 12 -> MakeMaterialAttributes.ClearCoat
- [94] BreakMaterialAttributes output 13 -> MakeMaterialAttributes.ClearCoatRoughness
- [94] BreakMaterialAttributes output 14 -> MakeMaterialAttributes.AmbientOcclusion
- [94] BreakMaterialAttributes output 15 -> MakeMaterialAttributes.Refraction
- [94] BreakMaterialAttributes output 16 -> MakeMaterialAttributes.CustomizedUVs
- [94] BreakMaterialAttributes output 24 -> MakeMaterialAttributes.PixelDepthOffset
- [88] Multiply output 0 -> Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 2's Rotation'.A
- [4] 0 output 0 -> Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 2's Rotation'.B
- [102] Multiply output 0 -> Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 3's Rotation'.A
- [100] 0 output 0 -> Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 3's Rotation'.B
- [144] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [98] Param (0) 'Wind 1's Angle Contribution to Wind 3' output 0 -> Multiply.B
- [139] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [101] Param (0) 'Wind 2's Angle Contribution to Wind 3' output 0 -> Multiply.B
- [104] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 3's Rotation' output 0 -> Add.A
- [97] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 3's Rotation' output 0 -> Add.B
- [99] Multiply output 0 -> Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 3's Rotation'.A
- [100] 0 output 0 -> Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 3's Rotation'.B
- [111] Multiply output 0 -> Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 4's Rotation'.A
- [112] 0 output 0 -> Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 4's Rotation'.B
- [105] Switch Param (False) 'Allow Wind 1 Calculations To Affect Wind 4's Rotation' output 0 -> Add.A
- [109] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation' output 0 -> Add.B
- [140] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [107] Param (0) 'Wind 2's Angle Contribution to Wind 4' output 0 -> Multiply.B
- [108] Multiply output 0 -> Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation'.A
- [112] 0 output 0 -> Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation'.B
- [143] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [110] Param (0) 'Wind 1's Angle Contribution to Wind 4' output 0 -> Multiply.B
- [6] Multiply output 0 -> Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation'.A
- [112] 0 output 0 -> Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation'.B
- [106] Add output 0 -> Add.A
- [113] Switch Param (False) 'Allow Wind 2 Calculations To Affect Wind 4's Rotation' output 0 -> Add.B
- [115] Param (1) 'Local Wind Scale 3' output 0 -> Multiply.A
- [10] Param (0.25) 'Wind Speed' output 0 -> Multiply.B
- [9] Param (0.25) 'Wind Speed' output 0 -> Multiply.A
- [30] Param (2) 'Local Wind 4 Speed' output 0 -> Multiply.B
- [121] Param (1) 'Local Wind Scale 2' output 0 -> Multiply.A
- [122] Param (0.25) 'Wind Speed' output 0 -> Multiply.B
- [124] Param (1) 'Local Wind Scale 1' output 0 -> Multiply.A
- [125] Param (0.25) 'Wind Speed' output 0 -> Multiply.B
- [89] BreakMaterialAttributes output 8 -> Tangent Space to World Space TransformVector.Input
- [128] MakeMaterialAttributes output 0 -> Feature Level Switch.Default
- [128] MakeMaterialAttributes output 0 -> Feature Level Switch.Inputs
- [70] TexCoord[1] output 0 -> Input Pivot Painter Texture Coordinate (Vector2).Preview
- [129] Input Pivot Painter Texture Coordinate (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [130] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [131] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [132] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [132] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [133] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [46] PivotPainter2FoliageAnimation output 4 -> Reroute Node (reroutes wires).Input
- [46] PivotPainter2FoliageAnimation output 5 -> Reroute Node (reroutes wires).Input
- [48] Clamp output 0 -> Reroute Node (reroutes wires).Input
- [59] PivotPainter2FoliageAnimation output 5 -> Reroute Node (reroutes wires).Input
- [139] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [59] PivotPainter2FoliageAnimation output 4 -> Reroute Node (reroutes wires).Input
- [60] PivotPainter2FoliageAnimation output 0 -> Reroute Node (reroutes wires).Input
- [144] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [60] PivotPainter2FoliageAnimation output 5 -> Reroute Node (reroutes wires).Input
- [77] PivotPainter2FoliageAnimation output 4 -> Reroute Node (reroutes wires).Input
- [77] PivotPainter2FoliageAnimation output 0 -> Reroute Node (reroutes wires).Input
- [23] Texture Size output 0 -> Reroute Node (reroutes wires).Input
- [52] Subtract output 0 -> Reroute Node (reroutes wires).Input
- [23] Texture Size output 0 -> Reroute Node (reroutes wires).Input
- [47] Lerp output 0 -> Reroute Node (reroutes wires).Input
- [164] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [73] Texture Size output 0 -> Reroute Node (reroutes wires).Input
- [152] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [153] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [64] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Reroute Node (reroutes wires).Input
- [155] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [151] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [133] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [58] Lerp_Multiple_Float2 output 1 -> Reroute Node (reroutes wires).Input
- [53] Ceil output 0 -> Reroute Node (reroutes wires).Input
- [67] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Reroute Node (reroutes wires).Input
- [66] Add output 0 -> Reroute Node (reroutes wires).Input
- [67] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Reroute Node (reroutes wires).Input
- [71] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Reroute Node (reroutes wires).Input
- [71] ms_PivotPainter2_ReturnParentTextureInfo output 1 -> Reroute Node (reroutes wires).Input
- [167] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [72] ms_PivotPainter2_UnpackIntegerAsFloat output 0 -> Reroute Node (reroutes wires).Input
- [152] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [160] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [164] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [53] Ceil output 0 -> Reroute Node (reroutes wires).Input
- [53] Ceil output 0 -> Reroute Node (reroutes wires).Input
- [23] Texture Size output 0 -> Reroute Node (reroutes wires).Input
- [79] Append output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `World Position Offset Component` is driven by [76] Add
- `Modified World Space Normal Component` is driven by [145] Reroute Node (reroutes wires)
- `Final Material with World Space Normals` is driven by [127] Feature Level Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


