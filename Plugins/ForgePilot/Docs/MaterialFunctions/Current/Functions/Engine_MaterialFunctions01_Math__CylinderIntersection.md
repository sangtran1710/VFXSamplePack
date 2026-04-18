# CylinderIntersection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/CylinderIntersection.CylinderIntersection`
- Category: Engine_MaterialFunctions01/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns an analytical intersection for a cylinder of the specified coordinates.

## Inputs

- `Origin` [FunctionInput_Vector3] - The center of the cylinder. Defaults to Actor Position.
- `Z Axis` [FunctionInput_Vector3] - The Z axis is the axis along the length of the cylinder. Defaults to the local Z axis of the mesh the material is applied to.
- `X Axis` [FunctionInput_Vector3] - The X axis is the axis used to apply non uniform scale along.  Defaults to the local X axis of the mesh the material is applied to.
- `Radius` [FunctionInput_Scalar] - The X axis is the axis used to apply non uniform scale along.  Defaults to the local X axis of the mesh the material is applied to.
- `XY Scale` [FunctionInput_Vector2] - The X axis is the axis used to apply non uniform scale along.  Defaults to the local X axis of the mesh the material is applied to.
- `Capped?` [FunctionInput_StaticBool] - By default the cylinder will have end caps defined by the Z min and Z max but instructions can be saved when calculating infinite cylinders by setting this to true.
- `Z Min-Max` [FunctionInput_Vector2] - The X axis is the axis used to apply non uniform scale along.  Defaults to the local X axis of the mesh the material is applied to.

## Outputs

- `Ray Entry Position`
- `Normal`
- `Opacity Mask`
- `Cylinder Thickness`
- `T0`
- `T1`
- `Ray Exit Position`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Ray Entry Position | Ray Entry Position |
| 1 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 2 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | expression | `MaterialExpressionConstant3Vector` | 1,1,0 |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionVectorParameter` | Param (0,0,1,0) 'Z' |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionVectorParameter` | Param (1,0,0,0) 'X' |  |
| 14 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 16 | function_call | `MaterialExpressionMaterialFunctionCall` | InverseTransformMatrix |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 18 | expression | `MaterialExpressionVectorParameter` | Param (1,0,0,0) 'X' |  |
| 19 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 20 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 21 | expression | `MaterialExpressionVectorParameter` | Param (0,0,1,0) 'Z' |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | InverseTransformMatrix |  |
| 23 | expression | `MaterialExpressionConstant` | 1 |  |
| 24 | expression | `MaterialExpressionConstant` | 1 |  |
| 25 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 26 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 27 | expression | `MaterialExpressionDivide` | Divide |  |
| 28 | expression | `MaterialExpressionScalarParameter` | Param (-100) 'Z Min' |  |
| 29 | expression | `MaterialExpressionConstant3Vector` | 0,0,-1 |  |
| 30 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 31 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 32 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 33 | expression | `MaterialExpressionDivide` | Divide |  |
| 34 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 35 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 36 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 37 | expression | `MaterialExpressionScalarParameter` | Param (100) 'Z Max' |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 39 | expression | `MaterialExpressionAdd` | Add |  |
| 40 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 41 | expression | `MaterialExpressionIf` | If |  |
| 42 | expression | `MaterialExpressionScalarParameter` | Param (-100) 'Z Min' |  |
| 43 | expression | `MaterialExpressionConstant` | 0 |  |
| 44 | expression | `MaterialExpressionConstant` | 1 |  |
| 45 | expression | `MaterialExpressionClamp` | Clamp |  |
| 46 | expression | `MaterialExpressionAdd` | Add |  |
| 47 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 48 | expression | `MaterialExpressionIf` | If |  |
| 49 | expression | `MaterialExpressionScalarParameter` | Param (100) 'Z Max' |  |
| 50 | expression | `MaterialExpressionConstant` | 0 |  |
| 51 | expression | `MaterialExpressionConstant` | 1 |  |
| 52 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 53 | expression | `MaterialExpressionIf` | If |  |
| 54 | expression | `MaterialExpressionScalarParameter` | Param (-100) 'Z Min' |  |
| 55 | expression | `MaterialExpressionConstant` | 0 |  |
| 56 | expression | `MaterialExpressionConstant` | 1 |  |
| 57 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 58 | expression | `MaterialExpressionAdd` | Add |  |
| 59 | expression | `MaterialExpressionConstant` | 1 |  |
| 60 | expression | `MaterialExpressionConstant` | 0 |  |
| 61 | expression | `MaterialExpressionScalarParameter` | Param (100) 'Z Max' |  |
| 62 | expression | `MaterialExpressionIf` | If |  |
| 63 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 64 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 65 | expression | `MaterialExpressionAdd` | Add |  |
| 66 | expression | `MaterialExpressionConstant` | 1 |  |
| 67 | expression | `MaterialExpressionScalarParameter` | Param (1) 'NU' |  |
| 68 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 69 | function_call | `MaterialExpressionMaterialFunctionCall` | QuadraticFormula |  |
| 70 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 71 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 72 | expression | `MaterialExpressionConstant` | 1 |  |
| 73 | expression | `MaterialExpressionScalarParameter` | Param (1) 'NU' |  |
| 74 | expression | `MaterialExpressionConstant` | 0 |  |
| 75 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 76 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 77 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 78 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 79 | expression | `MaterialExpressionAdd` | Add |  |
| 80 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 81 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 82 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 83 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 84 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 85 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 86 | expression | `MaterialExpressionAdd` | Add |  |
| 87 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 88 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 89 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 90 | expression | `MaterialExpressionAdd` | Add |  |
| 91 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 92 | expression | `MaterialExpressionScalarParameter` | Param (50) 'Radius' |  |
| 93 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 94 | expression | `MaterialExpressionAdd` | Add |  |
| 95 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 96 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 97 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 98 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 99 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 100 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 101 | expression | `MaterialExpressionConstant` | 0 |  |
| 102 | expression | `MaterialExpressionConstant` | 1 |  |
| 103 | expression | `MaterialExpressionIf` | If |  |
| 104 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 105 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 106 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 107 | function_input | `MaterialExpressionFunctionInput` | Input Origin (Vector3) | Origin |
| 108 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 109 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 110 | function_input | `MaterialExpressionFunctionInput` | Input Z Axis (Vector3) | Z Axis |
| 111 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 112 | expression | `MaterialExpressionTransform` | Local Space to World Space TransformVector |  |
| 113 | expression | `MaterialExpressionTransform` | Local Space to World Space TransformVector |  |
| 114 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 115 | function_input | `MaterialExpressionFunctionInput` | Input X Axis (Vector3) | X Axis |
| 116 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 117 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 118 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 119 | function_output | `MaterialExpressionFunctionOutput` | Output Opacity Mask | Opacity Mask |
| 120 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |
| 121 | function_input | `MaterialExpressionFunctionInput` | Input XY Scale (Vector2) | XY Scale |
| 122 | expression | `MaterialExpressionAppendVector` | Append |  |
| 123 | function_input | `MaterialExpressionFunctionInput` | Input Capped? (StaticBool) | Capped? |
| 124 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 125 | function_output | `MaterialExpressionFunctionOutput` | Output Cylinder Thickness | Cylinder Thickness |
| 126 | function_output | `MaterialExpressionFunctionOutput` | Output T0 | T0 |
| 127 | function_output | `MaterialExpressionFunctionOutput` | Output T1 | T1 |
| 128 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 129 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 130 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 131 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 132 | function_input | `MaterialExpressionFunctionInput` | Input Z Min-Max (Vector2) | Z Min-Max |
| 133 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 134 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 135 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 136 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 137 | expression | `MaterialExpressionAdd` | Add |  |
| 138 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 139 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 140 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 141 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 142 | function_output | `MaterialExpressionFunctionOutput` | Output Ray Exit Position | Ray Exit Position |
| 143 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 144 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |

## Exact Input Wiring

### [0] Output Ray Entry Position

- `A` <= [3] Add output 0

### [2] Normalize

- `VectorInput` <= [106] Transform3x3Matrix output 0

### [3] Add

- `A` <= [1] Camera Position output 0
- `B` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [141] Normalize output 0
- `B` <= [126] Output T0 output 0

### [5] Lerp

- `A` <= [33] Divide output 0
- `B` <= [144] Multiply(,1) output 0
- `Alpha` <= [62] If output 0

### [6] Lerp

- `A` <= [27] Divide output 0
- `B` <= [5] Lerp output 0
- `Alpha` <= [41] If output 0

### [7] Multiply

- `A` <= [120] Input Radius (Scalar) output 0
- `B` <= [120] Input Radius (Scalar) output 0

### [8] Subtract

- `A` <= [126] Output T0 output 0
- `B` <= [127] Output T1 output 0

### [10] Multiply

- `A` <= [58] Add output 0
- `B` <= [9] 1,1,0 output 0

### [12] Normalize

- `VectorInput` <= [11] Param (0,0,1,0) 'Z' output 0

### [14] Normalize

- `VectorInput` <= [13] Param (1,0,0,0) 'X' output 0

### [19] Normalize

- `VectorInput` <= [115] Input X Axis (Vector3) output 0

### [20] Normalize

- `VectorInput` <= [110] Input Z Axis (Vector3) output 0

### [25] Lerp

- `A` <= [27] Divide output 0
- `B` <= [32] Lerp output 0
- `Alpha` <= [53] If output 0

### [26] Subtract

- `A` <= [133] BreakOutFloat2Components output 0
- `B` <= [36] Mask ( B ) output 0

### [27] Divide

- `A` <= [26] Subtract output 0
- `B` <= [34] Mask ( B ) output 0

### [31] Lerp

- `A` <= [30] 0,0,1 output 0
- `B` <= [10] Multiply output 0
- `Alpha` <= [48] If output 0

### [32] Lerp

- `A` <= [33] Divide output 0
- `B` <= [143] Multiply(,1) output 0
- `Alpha` <= [48] If output 0

### [33] Divide

- `A` <= [35] Subtract output 0
- `B` <= [34] Mask ( B ) output 0

### [34] Mask ( B )

- `Input` <= [96] Multiply output 0

### [35] Subtract

- `A` <= [133] BreakOutFloat2Components output 1
- `B` <= [36] Mask ( B ) output 0

### [36] Mask ( B )

- `Input` <= [95] Multiply output 0

### [38] Multiply

- `A` <= [46] Add output 0
- `B` <= [39] Add output 0

### [39] Add

- `A` <= [53] If output 0
- `B` <= [41] If output 0

### [40] Mask ( B )

- `Input` <= [65] Add output 0

### [41] If

- `A` <= [40] Mask ( B ) output 0
- `B` <= [134] BreakOutFloat2Components output 0
- `AGreaterThanB` <= [44] 1 output 0
- `ALessThanB` <= [43] 0 output 0

### [45] Clamp

- `Input` <= [38] Multiply output 0

### [46] Add

- `A` <= [48] If output 0
- `B` <= [62] If output 0

### [47] Mask ( B )

- `Input` <= [94] Add output 0

### [48] If

- `A` <= [47] Mask ( B ) output 0
- `B` <= [134] BreakOutFloat2Components output 1
- `AGreaterThanB` <= [50] 0 output 0
- `ALessThanB` <= [51] 1 output 0

### [52] Mask ( B )

- `Input` <= [94] Add output 0

### [53] If

- `A` <= [52] Mask ( B ) output 0
- `B` <= [134] BreakOutFloat2Components output 0
- `AGreaterThanB` <= [56] 1 output 0
- `ALessThanB` <= [55] 0 output 0

### [57] Multiply

- `A` <= [96] Multiply output 0
- `B` <= [126] Output T0 output 0

### [58] Add

- `A` <= [95] Multiply output 0
- `B` <= [57] Multiply output 0

### [62] If

- `A` <= [63] Mask ( B ) output 0
- `B` <= [134] BreakOutFloat2Components output 1
- `AGreaterThanB` <= [60] 0 output 0
- `ALessThanB` <= [59] 1 output 0

### [63] Mask ( B )

- `Input` <= [65] Add output 0

### [64] Multiply

- `A` <= [144] Multiply(,1) output 0
- `B` <= [70] Multiply output 0

### [65] Add

- `A` <= [95] Multiply output 0
- `B` <= [64] Multiply output 0

### [70] Multiply

- `A` <= [122] Append output 0
- `B` <= [96] Multiply output 0

### [75] Normalize

- `VectorInput` <= [100] Subtract output 0

### [77] Multiply

- `A` <= [76] BreakOutFloat2Components output 0
- `B` <= [76] BreakOutFloat2Components output 0

### [78] Multiply

- `A` <= [76] BreakOutFloat2Components output 1
- `B` <= [76] BreakOutFloat2Components output 1

### [79] Add

- `A` <= [77] Multiply output 0
- `B` <= [78] Multiply output 0

### [81] Multiply

- `A` <= [80] BreakOutFloat2Components output 0
- `B` <= [82] BreakOutFloat2Components output 0

### [83] Multiply(,2)

- `A` <= [81] Multiply output 0

### [84] Multiply(,2)

- `A` <= [85] Multiply output 0

### [85] Multiply

- `A` <= [80] BreakOutFloat2Components output 1
- `B` <= [82] BreakOutFloat2Components output 1

### [86] Add

- `A` <= [83] Multiply(,2) output 0
- `B` <= [84] Multiply(,2) output 0

### [88] Multiply

- `A` <= [87] BreakOutFloat2Components output 1
- `B` <= [87] BreakOutFloat2Components output 1

### [89] Multiply

- `A` <= [87] BreakOutFloat2Components output 0
- `B` <= [87] BreakOutFloat2Components output 0

### [90] Add

- `A` <= [89] Multiply output 0
- `B` <= [88] Multiply output 0

### [91] Subtract

- `A` <= [90] Add output 0
- `B` <= [7] Multiply output 0

### [93] Multiply

- `A` <= [143] Multiply(,1) output 0
- `B` <= [70] Multiply output 0

### [94] Add

- `A` <= [95] Multiply output 0
- `B` <= [93] Multiply output 0

### [95] Multiply

- `A` <= [16] InverseTransformMatrix output 0
- `B` <= [122] Append output 0

### [96] Multiply

- `A` <= [122] Append output 0
- `B` <= [22] InverseTransformMatrix output 0

### [100] Subtract

- `A` <= [99] Camera Position output 0
- `B` <= [98] Absolute World Position output 0

### [103] If

- `A` <= [69] QuadraticFormula output 2
- `B` <= [74] 0 output 0
- `AGreaterThanB` <= [102] 1 output 0
- `ALessThanB` <= [101] 0 output 0

### [104] Lerp

- `A` <= [29] 0,0,-1 output 0
- `B` <= [31] Lerp output 0
- `Alpha` <= [53] If output 0

### [105] Multiply

- `A` <= [45] Clamp output 0
- `B` <= [103] If output 0

### [107] Input Origin (Vector3)

- `Preview` <= [109] Actor Position (Absolute) output 0

### [108] Subtract

- `A` <= [97] Camera Position output 0
- `B` <= [107] Input Origin (Vector3) output 0

### [110] Input Z Axis (Vector3)

- `Preview` <= [112] Local Space to World Space TransformVector output 0

### [112] Local Space to World Space TransformVector

- `Input` <= [111] 0,0,1 output 0

### [113] Local Space to World Space TransformVector

- `Input` <= [114] 1,0,0 output 0

### [115] Input X Axis (Vector3)

- `Preview` <= [113] Local Space to World Space TransformVector output 0

### [116] Subtract

- `A` <= [143] Multiply(,1) output 0
- `B` <= [144] Multiply(,1) output 0

### [117] Multiply(,1)

- `A` <= [103] If output 0

### [118] Output Normal

- `A` <= [2] Normalize output 0

### [119] Output Opacity Mask

- `A` <= [131] Switch output 0

### [122] Append

- `A` <= [121] Input XY Scale (Vector2) output 0
- `B` <= [66] 1 output 0

### [123] Input Capped? (StaticBool)

- `Preview` <= [130] Static Bool (True) output 0

### [124] Switch

- `A` <= [8] Subtract output 0
- `B` <= [116] Subtract output 0
- `Value` <= [123] Input Capped? (StaticBool) output 0

### [125] Output Cylinder Thickness

- `A` <= [124] Switch output 0

### [126] Output T0

- `A` <= [129] Switch output 0

### [127] Output T1

- `A` <= [128] Switch output 0

### [128] Switch

- `A` <= [6] Lerp output 0
- `B` <= [144] Multiply(,1) output 0
- `Value` <= [123] Input Capped? (StaticBool) output 0

### [129] Switch

- `A` <= [25] Lerp output 0
- `B` <= [143] Multiply(,1) output 0
- `Value` <= [123] Input Capped? (StaticBool) output 0

### [131] Switch

- `A` <= [105] Multiply output 0
- `B` <= [117] Multiply(,1) output 0
- `Value` <= [123] Input Capped? (StaticBool) output 0

### [135] Switch

- `A` <= [104] Lerp output 0
- `B` <= [10] Multiply output 0
- `Value` <= [123] Input Capped? (StaticBool) output 0

### [136] Multiply

- `A` <= [141] Normalize output 0
- `B` <= [127] Output T1 output 0

### [137] Add

- `A` <= [1] Camera Position output 0
- `B` <= [136] Multiply output 0

### [138] Subtract

- `A` <= [139] Camera Position output 0
- `B` <= [140] Absolute World Position output 0

### [141] Normalize

- `VectorInput` <= [138] Subtract output 0

### [142] Output Ray Exit Position

- `A` <= [137] Add output 0

### [143] Multiply(,1)

- `A` <= [69] QuadraticFormula output 1

### [144] Multiply(,1)

- `A` <= [69] QuadraticFormula output 0

## Connection List

- [3] Add output 0 -> Output Ray Entry Position.A
- [106] Transform3x3Matrix output 0 -> Normalize.VectorInput
- [1] Camera Position output 0 -> Add.A
- [4] Multiply output 0 -> Add.B
- [141] Normalize output 0 -> Multiply.A
- [126] Output T0 output 0 -> Multiply.B
- [33] Divide output 0 -> Lerp.A
- [144] Multiply(,1) output 0 -> Lerp.B
- [62] If output 0 -> Lerp.Alpha
- [27] Divide output 0 -> Lerp.A
- [5] Lerp output 0 -> Lerp.B
- [41] If output 0 -> Lerp.Alpha
- [120] Input Radius (Scalar) output 0 -> Multiply.A
- [120] Input Radius (Scalar) output 0 -> Multiply.B
- [126] Output T0 output 0 -> Subtract.A
- [127] Output T1 output 0 -> Subtract.B
- [58] Add output 0 -> Multiply.A
- [9] 1,1,0 output 0 -> Multiply.B
- [11] Param (0,0,1,0) 'Z' output 0 -> Normalize.VectorInput
- [13] Param (1,0,0,0) 'X' output 0 -> Normalize.VectorInput
- [115] Input X Axis (Vector3) output 0 -> Normalize.VectorInput
- [110] Input Z Axis (Vector3) output 0 -> Normalize.VectorInput
- [27] Divide output 0 -> Lerp.A
- [32] Lerp output 0 -> Lerp.B
- [53] If output 0 -> Lerp.Alpha
- [133] BreakOutFloat2Components output 0 -> Subtract.A
- [36] Mask ( B ) output 0 -> Subtract.B
- [26] Subtract output 0 -> Divide.A
- [34] Mask ( B ) output 0 -> Divide.B
- [30] 0,0,1 output 0 -> Lerp.A
- [10] Multiply output 0 -> Lerp.B
- [48] If output 0 -> Lerp.Alpha
- [33] Divide output 0 -> Lerp.A
- [143] Multiply(,1) output 0 -> Lerp.B
- [48] If output 0 -> Lerp.Alpha
- [35] Subtract output 0 -> Divide.A
- [34] Mask ( B ) output 0 -> Divide.B
- [96] Multiply output 0 -> Mask ( B ).Input
- [133] BreakOutFloat2Components output 1 -> Subtract.A
- [36] Mask ( B ) output 0 -> Subtract.B
- [95] Multiply output 0 -> Mask ( B ).Input
- [46] Add output 0 -> Multiply.A
- [39] Add output 0 -> Multiply.B
- [53] If output 0 -> Add.A
- [41] If output 0 -> Add.B
- [65] Add output 0 -> Mask ( B ).Input
- [40] Mask ( B ) output 0 -> If.A
- [134] BreakOutFloat2Components output 0 -> If.B
- [44] 1 output 0 -> If.AGreaterThanB
- [43] 0 output 0 -> If.ALessThanB
- [38] Multiply output 0 -> Clamp.Input
- [48] If output 0 -> Add.A
- [62] If output 0 -> Add.B
- [94] Add output 0 -> Mask ( B ).Input
- [47] Mask ( B ) output 0 -> If.A
- [134] BreakOutFloat2Components output 1 -> If.B
- [50] 0 output 0 -> If.AGreaterThanB
- [51] 1 output 0 -> If.ALessThanB
- [94] Add output 0 -> Mask ( B ).Input
- [52] Mask ( B ) output 0 -> If.A
- [134] BreakOutFloat2Components output 0 -> If.B
- [56] 1 output 0 -> If.AGreaterThanB
- [55] 0 output 0 -> If.ALessThanB
- [96] Multiply output 0 -> Multiply.A
- [126] Output T0 output 0 -> Multiply.B
- [95] Multiply output 0 -> Add.A
- [57] Multiply output 0 -> Add.B
- [63] Mask ( B ) output 0 -> If.A
- [134] BreakOutFloat2Components output 1 -> If.B
- [60] 0 output 0 -> If.AGreaterThanB
- [59] 1 output 0 -> If.ALessThanB
- [65] Add output 0 -> Mask ( B ).Input
- [144] Multiply(,1) output 0 -> Multiply.A
- [70] Multiply output 0 -> Multiply.B
- [95] Multiply output 0 -> Add.A
- [64] Multiply output 0 -> Add.B
- [122] Append output 0 -> Multiply.A
- [96] Multiply output 0 -> Multiply.B
- [100] Subtract output 0 -> Normalize.VectorInput
- [76] BreakOutFloat2Components output 0 -> Multiply.A
- [76] BreakOutFloat2Components output 0 -> Multiply.B
- [76] BreakOutFloat2Components output 1 -> Multiply.A
- [76] BreakOutFloat2Components output 1 -> Multiply.B
- [77] Multiply output 0 -> Add.A
- [78] Multiply output 0 -> Add.B
- [80] BreakOutFloat2Components output 0 -> Multiply.A
- [82] BreakOutFloat2Components output 0 -> Multiply.B
- [81] Multiply output 0 -> Multiply(,2).A
- [85] Multiply output 0 -> Multiply(,2).A
- [80] BreakOutFloat2Components output 1 -> Multiply.A
- [82] BreakOutFloat2Components output 1 -> Multiply.B
- [83] Multiply(,2) output 0 -> Add.A
- [84] Multiply(,2) output 0 -> Add.B
- [87] BreakOutFloat2Components output 1 -> Multiply.A
- [87] BreakOutFloat2Components output 1 -> Multiply.B
- [87] BreakOutFloat2Components output 0 -> Multiply.A
- [87] BreakOutFloat2Components output 0 -> Multiply.B
- [89] Multiply output 0 -> Add.A
- [88] Multiply output 0 -> Add.B
- [90] Add output 0 -> Subtract.A
- [7] Multiply output 0 -> Subtract.B
- [143] Multiply(,1) output 0 -> Multiply.A
- [70] Multiply output 0 -> Multiply.B
- [95] Multiply output 0 -> Add.A
- [93] Multiply output 0 -> Add.B
- [16] InverseTransformMatrix output 0 -> Multiply.A
- [122] Append output 0 -> Multiply.B
- [122] Append output 0 -> Multiply.A
- [22] InverseTransformMatrix output 0 -> Multiply.B
- [99] Camera Position output 0 -> Subtract.A
- [98] Absolute World Position output 0 -> Subtract.B
- [69] QuadraticFormula output 2 -> If.A
- [74] 0 output 0 -> If.B
- [102] 1 output 0 -> If.AGreaterThanB
- [101] 0 output 0 -> If.ALessThanB
- [29] 0,0,-1 output 0 -> Lerp.A
- [31] Lerp output 0 -> Lerp.B
- [53] If output 0 -> Lerp.Alpha
- [45] Clamp output 0 -> Multiply.A
- [103] If output 0 -> Multiply.B
- [109] Actor Position (Absolute) output 0 -> Input Origin (Vector3).Preview
- [97] Camera Position output 0 -> Subtract.A
- [107] Input Origin (Vector3) output 0 -> Subtract.B
- [112] Local Space to World Space TransformVector output 0 -> Input Z Axis (Vector3).Preview
- [111] 0,0,1 output 0 -> Local Space to World Space TransformVector.Input
- [114] 1,0,0 output 0 -> Local Space to World Space TransformVector.Input
- [113] Local Space to World Space TransformVector output 0 -> Input X Axis (Vector3).Preview
- [143] Multiply(,1) output 0 -> Subtract.A
- [144] Multiply(,1) output 0 -> Subtract.B
- [103] If output 0 -> Multiply(,1).A
- [2] Normalize output 0 -> Output Normal.A
- [131] Switch output 0 -> Output Opacity Mask.A
- [121] Input XY Scale (Vector2) output 0 -> Append.A
- [66] 1 output 0 -> Append.B
- [130] Static Bool (True) output 0 -> Input Capped? (StaticBool).Preview
- [8] Subtract output 0 -> Switch.A
- [116] Subtract output 0 -> Switch.B
- [123] Input Capped? (StaticBool) output 0 -> Switch.Value
- [124] Switch output 0 -> Output Cylinder Thickness.A
- [129] Switch output 0 -> Output T0.A
- [128] Switch output 0 -> Output T1.A
- [6] Lerp output 0 -> Switch.A
- [144] Multiply(,1) output 0 -> Switch.B
- [123] Input Capped? (StaticBool) output 0 -> Switch.Value
- [25] Lerp output 0 -> Switch.A
- [143] Multiply(,1) output 0 -> Switch.B
- [123] Input Capped? (StaticBool) output 0 -> Switch.Value
- [105] Multiply output 0 -> Switch.A
- [117] Multiply(,1) output 0 -> Switch.B
- [123] Input Capped? (StaticBool) output 0 -> Switch.Value
- [104] Lerp output 0 -> Switch.A
- [10] Multiply output 0 -> Switch.B
- [123] Input Capped? (StaticBool) output 0 -> Switch.Value
- [141] Normalize output 0 -> Multiply.A
- [127] Output T1 output 0 -> Multiply.B
- [1] Camera Position output 0 -> Add.A
- [136] Multiply output 0 -> Add.B
- [139] Camera Position output 0 -> Subtract.A
- [140] Absolute World Position output 0 -> Subtract.B
- [138] Subtract output 0 -> Normalize.VectorInput
- [137] Add output 0 -> Output Ray Exit Position.A
- [69] QuadraticFormula output 1 -> Multiply(,1).A
- [69] QuadraticFormula output 0 -> Multiply(,1).A

## Output Trace Roots

- `Ray Entry Position` is driven by [3] Add
- `Normal` is driven by [2] Normalize
- `Opacity Mask` is driven by [131] Switch
- `Cylinder Thickness` is driven by [124] Switch
- `T0` is driven by [129] Switch
- `T1` is driven by [128] Switch
- `Ray Exit Position` is driven by [137] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


