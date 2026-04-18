# DebugScalarValues

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugScalarValues.DebugScalarValues`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Enter a value into the material function and read its return value as a number in uv space.

## Inputs

- `Number` [FunctionInput_Scalar]
- `MaximumNumberOfDigits` [FunctionInput_Scalar] - Max number of displayed integers.
- `UVs` [FunctionInput_Vector2]
- `DebugTextLocation RG_UpperRight BA_LowerLeft` [FunctionInput_Vector4]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Number (Scalar) | Number |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 8 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionConstant` | 10 |  |
| 11 | expression | `MaterialExpressionPower` | Power |  |
| 12 | expression | `MaterialExpressionFrac` | Frac |  |
| 13 | expression | `MaterialExpressionConstant` | 128.5 |  |
| 14 | expression | `MaterialExpressionFrac` | Frac |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,100) |  |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | expression | `MaterialExpressionFloor` | Floor |  |
| 18 | expression | `MaterialExpressionDivide` | Divide(,10) |  |
| 19 | expression | `MaterialExpressionIf` | If |  |
| 20 | expression | `MaterialExpressionConstant` | 0 |  |
| 21 | expression | `MaterialExpressionConstant` | 1 |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input MaximumNumberOfDigits (Scalar) | MaximumNumberOfDigits |
| 24 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 25 | expression | `MaterialExpressionCeil` | Ceil |  |
| 26 | expression | `MaterialExpressionClamp` | Clamp |  |
| 27 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 28 | expression | `MaterialExpressionAdd` | Add |  |
| 29 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 30 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 31 | expression | `MaterialExpressionAppendVector` | Append |  |
| 32 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionClamp` | Clamp |  |
| 36 | expression | `MaterialExpressionAbs` | Abs |  |
| 37 | expression | `MaterialExpressionCeil` | Ceil |  |
| 38 | expression | `MaterialExpressionPower` | Power |  |
| 39 | expression | `MaterialExpressionFloor` | Floor |  |
| 40 | expression | `MaterialExpressionClamp` | Clamp |  |
| 41 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 42 | expression | `MaterialExpressionAdd` | Add |  |
| 43 | expression | `MaterialExpressionFrac` | Frac |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply(,10) |  |
| 45 | expression | `MaterialExpressionDivide` | Divide(,10) |  |
| 46 | expression | `MaterialExpressionFloor` | Floor |  |
| 47 | expression | `MaterialExpressionAdd` | Add(0.5,) |  |
| 48 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 49 | expression | `MaterialExpressionClamp` | Clamp |  |
| 50 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 51 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 52 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 53 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 54 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureCropping |  |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) | DebugTextLocation RG_UpperRight BA_LowerLeft |
| 56 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 57 | expression | `MaterialExpressionComponentMask` | Mask ( B A ) |  |
| 58 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 59 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 60 | expression | `MaterialExpressionConstant2Vector` | 2,0.1 |  |
| 61 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 62 | expression | `MaterialExpressionAdd` | Add |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 64 | expression | `MaterialExpressionClamp` | Clamp |  |
| 65 | expression | `MaterialExpressionAdd` | Add |  |
| 66 | expression | `MaterialExpressionDivide` | Divide(,1e+06) |  |
| 67 | expression | `MaterialExpressionDivide` | Divide |  |
| 68 | expression | `MaterialExpressionAdd` | Add(,3) |  |
| 69 | expression | `MaterialExpressionPower` | Power |  |
| 70 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 71 | expression | `MaterialExpressionAbs` | Abs |  |
| 72 | expression | `MaterialExpressionIf` | If |  |
| 73 | expression | `MaterialExpressionConstant` | 0 |  |
| 74 | expression | `MaterialExpressionIf` | If |  |
| 75 | expression | `MaterialExpressionPower` | Power |  |
| 76 | expression | `MaterialExpressionAdd` | Add(,2) |  |
| 77 | expression | `MaterialExpressionDivide` | Divide |  |
| 78 | expression | `MaterialExpressionIf` | If |  |
| 79 | expression | `MaterialExpressionConstant` | 0 |  |
| 80 | expression | `MaterialExpressionConstant` | 1 |  |
| 81 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 82 | expression | `MaterialExpressionAdd` | Add |  |
| 83 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 84 | expression | `MaterialExpressionAppendVector` | Append |  |
| 85 | expression | `MaterialExpressionFrac` | Frac |  |
| 86 | function_call | `MaterialExpressionMaterialFunctionCall` | ComputeMipLevel |  |
| 87 | expression | `MaterialExpressionConstant2Vector` | 128,128 |  |
| 88 | expression | `MaterialExpressionAppendVector` | Append |  |
| 89 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 90 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 91 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 92 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 93 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 94 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 95 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 96 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 97 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 98 | expression | `MaterialExpressionConstant` | 10 |  |
| 99 | expression | `MaterialExpressionConstant` | 10 |  |
| 100 | expression | `MaterialExpressionConstant` | 10 |  |
| 101 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 102 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 103 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 104 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 105 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 106 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 107 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 108 | expression | `MaterialExpressionConstant` | 0 |  |
| 109 | expression | `MaterialExpressionConstant` | 1 |  |
| 110 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 111 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 112 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 113 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 114 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 115 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 116 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 117 | expression | `MaterialExpressionIf` | If |  |
| 118 | expression | `MaterialExpressionConstant` | 1 |  |
| 119 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 120 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 121 | expression | `MaterialExpressionCeil` | Ceil |  |
| 122 | expression | `MaterialExpressionCeil` | Ceil |  |
| 123 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 124 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 125 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 126 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [70] Mask ( R ) output 0

### [1] Texture Sample

- `Coordinates` <= [5] Append output 0

### [2] Add

- `A` <= [90] Reroute Node (reroutes wires) output 0
- `B` <= [18] Divide(,10) output 0

### [3] Input Number (Scalar)

- `Preview` <= [13] 128.5 output 0

### [4] Mask ( G )

- `Input` <= [59] Multiply output 0

### [5] Append

- `A` <= [12] Frac output 0
- `B` <= [41] Lerp output 0

### [6] Mask ( R )

- `Input` <= [59] Multiply output 0

### [7] Texture Sample

- `Coordinates` <= [31] Append output 0

### [8] 1-x

- `Input` <= [6] Mask ( R ) output 0

### [9] Multiply

- `A` <= [8] 1-x output 0
- `B` <= [23] Input MaximumNumberOfDigits (Scalar) output 0

### [11] Power

- `Base` <= [98] 10 output 0
- `Exponent` <= [24] Add(,1) output 0

### [12] Frac

- `Input` <= [89] Reroute Node (reroutes wires) output 0

### [14] Frac

- `Input` <= [16] Divide output 0

### [15] Multiply(,100)

- `A` <= [14] Frac output 0

### [16] Divide

- `A` <= [71] Abs output 0
- `B` <= [11] Power output 0

### [17] Floor

- `Input` <= [15] Multiply(,100) output 0

### [18] Divide(,10)

- `A` <= [17] Floor output 0

### [19] If

- `A` <= [67] Divide output 0
- `B` <= [20] 0 output 0
- `AGreaterThanB` <= [21] 1 output 0
- `AEqualsB` <= [20] 0 output 0
- `ALessThanB` <= [20] 0 output 0

### [22] Multiply

- `A` <= [1] Texture Sample output 1
- `B` <= [63] Multiply output 0

### [24] Add(,1)

- `A` <= [25] Ceil output 0

### [25] Ceil

- `Input` <= [26] Clamp output 0

### [26] Clamp

- `Input` <= [101] Reroute Node (reroutes wires) output 0

### [27] Mask ( R )

- `Input` <= [32] Subtract(,0.5) output 0

### [28] Add

- `A` <= [23] Input MaximumNumberOfDigits (Scalar) output 0
- `B` <= [23] Input MaximumNumberOfDigits (Scalar) output 0

### [29] Multiply

- `A` <= [28] Add output 0
- `B` <= [27] Mask ( R ) output 0

### [30] Mask ( G )

- `Input` <= [102] Reroute Node (reroutes wires) output 0

### [31] Append

- `A` <= [47] Add(0.5,) output 0
- `B` <= [30] Mask ( G ) output 0

### [32] Subtract(,0.5)

- `A` <= [102] Reroute Node (reroutes wires) output 0

### [33] Add

- `A` <= [22] Multiply output 0
- `B` <= [7] Texture Sample output 1

### [34] Multiply

- `A` <= [65] Add output 0
- `B` <= [38] Power output 0

### [35] Clamp

- `Input` <= [101] Reroute Node (reroutes wires) output 0

### [36] Abs

- `Input` <= [35] Clamp output 0

### [37] Ceil

- `Input` <= [49] Clamp output 0

### [38] Power

- `Base` <= [10] 10 output 0
- `Exponent` <= [46] Floor output 0

### [39] Floor

- `Input` <= [44] Multiply(,10) output 0

### [40] Clamp

- `Input` <= [37] Ceil output 0

### [41] Lerp

- `A` <= [2] Add output 0
- `B` <= [42] Add output 0
- `Alpha` <= [95] Reroute Node (reroutes wires) output 0

### [42] Add

- `A` <= [90] Reroute Node (reroutes wires) output 0
- `B` <= [45] Divide(,10) output 0

### [43] Frac

- `Input` <= [34] Multiply output 0

### [44] Multiply(,10)

- `A` <= [43] Frac output 0

### [45] Divide(,10)

- `A` <= [39] Floor output 0

### [46] Floor

- `Input` <= [49] Clamp output 0

### [47] Add(0.5,)

- `B` <= [29] Multiply output 0

### [48] Subtract(,1)

- `A` <= [36] Abs output 0

### [49] Clamp

- `Input` <= [48] Subtract(,1) output 0

### [50] SphereMask

- `A` <= [89] Reroute Node (reroutes wires) output 0
- `B` <= [51] -0.5 output 0

### [52] 1-x

- `Input` <= [50] SphereMask output 0

### [53] Input UVs (Vector2)

- `Preview` <= [58] TexCoord[0] output 0

### [56] Mask ( R G )

- `Input` <= [55] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [57] Mask ( B A )

- `Input` <= [55] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [59] Multiply

- `A` <= [102] Reroute Node (reroutes wires) output 0
- `B` <= [60] 2,0.1 output 0

### [61] Multiply

- `A` <= [82] Add output 0
- `B` <= [96] Reroute Node (reroutes wires) output 0

### [62] Add

- `A` <= [126] Reroute Node (reroutes wires) output 0
- `B` <= [95] Reroute Node (reroutes wires) output 0

### [63] Multiply

- `A` <= [52] 1-x output 0
- `B` <= [62] Add output 0

### [64] Clamp

- `Input` <= [61] Multiply output 0

### [65] Add

- `A` <= [93] Reroute Node (reroutes wires) output 0
- `B` <= [66] Divide(,1e+06) output 0

### [66] Divide(,1e+06)

- `A` <= [93] Reroute Node (reroutes wires) output 0

### [67] Divide

- `A` <= [91] Reroute Node (reroutes wires) output 0
- `B` <= [69] Power output 0

### [68] Add(,3)

- `A` <= [92] Reroute Node (reroutes wires) output 0

### [69] Power

- `Base` <= [99] 10 output 0
- `Exponent` <= [68] Add(,3) output 0

### [70] Mask ( R )

- `Input` <= [64] Clamp output 0

### [71] Abs

- `Input` <= [3] Input Number (Scalar) output 0

### [72] If

- `A` <= [94] Reroute Node (reroutes wires) output 0
- `B` <= [73] 0 output 0
- `AGreaterThanB` <= [73] 0 output 0
- `AEqualsB` <= [73] 0 output 0
- `ALessThanB` <= [80] 1 output 0

### [74] If

- `A` <= [77] Divide output 0
- `B` <= [108] 0 output 0
- `AGreaterThanB` <= [109] 1 output 0
- `AEqualsB` <= [108] 0 output 0
- `ALessThanB` <= [108] 0 output 0

### [75] Power

- `Base` <= [100] 10 output 0
- `Exponent` <= [76] Add(,2) output 0

### [76] Add(,2)

- `A` <= [92] Reroute Node (reroutes wires) output 0

### [77] Divide

- `A` <= [91] Reroute Node (reroutes wires) output 0
- `B` <= [75] Power output 0

### [78] If

- `A` <= [74] If output 0
- `B` <= [19] If output 0
- `AGreaterThanB` <= [110] Reroute Node (reroutes wires) output 0
- `AEqualsB` <= [79] 0 output 0
- `ALessThanB` <= [79] 0 output 0

### [81] Multiply

- `A` <= [78] If output 0
- `B` <= [72] If output 0

### [82] Add

- `A` <= [33] Add output 0
- `B` <= [112] Reroute Node (reroutes wires) output 0

### [83] Texture Sample

- `Coordinates` <= [84] Append output 0
- `MipValue` <= [86] ComputeMipLevel output 0

### [84] Append

- `A` <= [85] Frac output 0
- `B` <= [97] Reroute Node (reroutes wires) output 0

### [85] Frac

- `Input` <= [101] Reroute Node (reroutes wires) output 0

### [88] Append

- `A` <= [101] Reroute Node (reroutes wires) output 0
- `B` <= [97] Reroute Node (reroutes wires) output 0

### [89] Reroute Node (reroutes wires)

- `Input` <= [9] Multiply output 0

### [90] Reroute Node (reroutes wires)

- `Input` <= [4] Mask ( G ) output 0

### [91] Reroute Node (reroutes wires)

- `Input` <= [71] Abs output 0

### [92] Reroute Node (reroutes wires)

- `Input` <= [106] Reroute Node (reroutes wires) output 0

### [93] Reroute Node (reroutes wires)

- `Input` <= [71] Abs output 0

### [94] Reroute Node (reroutes wires)

- `Input` <= [3] Input Number (Scalar) output 0

### [95] Reroute Node (reroutes wires)

- `Input` <= [40] Clamp output 0

### [96] Reroute Node (reroutes wires)

- `Input` <= [103] Reroute Node (reroutes wires) output 0

### [97] Reroute Node (reroutes wires)

- `Input` <= [30] Mask ( G ) output 0

### [101] Reroute Node (reroutes wires)

- `Input` <= [9] Multiply output 0

### [102] Reroute Node (reroutes wires)

- `Input` <= [54] TextureCropping output 2

### [103] Reroute Node (reroutes wires)

- `Input` <= [104] Reroute Node (reroutes wires) output 0

### [104] Reroute Node (reroutes wires)

- `Input` <= [105] Reroute Node (reroutes wires) output 0

### [105] Reroute Node (reroutes wires)

- `Input` <= [54] TextureCropping output 3

### [106] Reroute Node (reroutes wires)

- `Input` <= [24] Add(,1) output 0

### [107] Reroute Node (reroutes wires)

- `Input` <= [117] If output 0

### [110] Reroute Node (reroutes wires)

- `Input` <= [111] Reroute Node (reroutes wires) output 0

### [111] Reroute Node (reroutes wires)

- `Input` <= [83] Texture Sample output 2

### [112] Reroute Node (reroutes wires)

- `Input` <= [107] Reroute Node (reroutes wires) output 0

### [113] SphereMask

- `A` <= [115] Reroute Node (reroutes wires) output 0
- `B` <= [114] 0.5 output 0

### [115] Reroute Node (reroutes wires)

- `Input` <= [101] Reroute Node (reroutes wires) output 0

### [116] Multiply

- `A` <= [125] Saturate output 0
- `B` <= [113] SphereMask output 0

### [117] If

- `A` <= [118] 1 output 0
- `B` <= [125] Saturate output 0
- `AGreaterThanB` <= [81] Multiply output 0
- `AEqualsB` <= [119] Multiply output 0
- `ALessThanB` <= [81] Multiply output 0

### [119] Multiply

- `A` <= [110] Reroute Node (reroutes wires) output 0
- `B` <= [116] Multiply output 0

### [120] Add(,1)

- `A` <= [3] Input Number (Scalar) output 0

### [121] Ceil

- `Input` <= [120] Add(,1) output 0

### [122] Ceil

- `Input` <= [123] Multiply(,-1) output 0

### [123] Multiply(,-1)

- `A` <= [3] Input Number (Scalar) output 0

### [124] Multiply

- `A` <= [121] Ceil output 0
- `B` <= [122] Ceil output 0

### [125] Saturate

- `Input` <= [124] Multiply output 0

### [126] Reroute Node (reroutes wires)

- `Input` <= [19] If output 0

## Connection List

- [70] Mask ( R ) output 0 -> Output Result.A
- [5] Append output 0 -> Texture Sample.Coordinates
- [90] Reroute Node (reroutes wires) output 0 -> Add.A
- [18] Divide(,10) output 0 -> Add.B
- [13] 128.5 output 0 -> Input Number (Scalar).Preview
- [59] Multiply output 0 -> Mask ( G ).Input
- [12] Frac output 0 -> Append.A
- [41] Lerp output 0 -> Append.B
- [59] Multiply output 0 -> Mask ( R ).Input
- [31] Append output 0 -> Texture Sample.Coordinates
- [6] Mask ( R ) output 0 -> 1-x.Input
- [8] 1-x output 0 -> Multiply.A
- [23] Input MaximumNumberOfDigits (Scalar) output 0 -> Multiply.B
- [98] 10 output 0 -> Power.Base
- [24] Add(,1) output 0 -> Power.Exponent
- [89] Reroute Node (reroutes wires) output 0 -> Frac.Input
- [16] Divide output 0 -> Frac.Input
- [14] Frac output 0 -> Multiply(,100).A
- [71] Abs output 0 -> Divide.A
- [11] Power output 0 -> Divide.B
- [15] Multiply(,100) output 0 -> Floor.Input
- [17] Floor output 0 -> Divide(,10).A
- [67] Divide output 0 -> If.A
- [20] 0 output 0 -> If.B
- [21] 1 output 0 -> If.AGreaterThanB
- [20] 0 output 0 -> If.AEqualsB
- [20] 0 output 0 -> If.ALessThanB
- [1] Texture Sample output 1 -> Multiply.A
- [63] Multiply output 0 -> Multiply.B
- [25] Ceil output 0 -> Add(,1).A
- [26] Clamp output 0 -> Ceil.Input
- [101] Reroute Node (reroutes wires) output 0 -> Clamp.Input
- [32] Subtract(,0.5) output 0 -> Mask ( R ).Input
- [23] Input MaximumNumberOfDigits (Scalar) output 0 -> Add.A
- [23] Input MaximumNumberOfDigits (Scalar) output 0 -> Add.B
- [28] Add output 0 -> Multiply.A
- [27] Mask ( R ) output 0 -> Multiply.B
- [102] Reroute Node (reroutes wires) output 0 -> Mask ( G ).Input
- [47] Add(0.5,) output 0 -> Append.A
- [30] Mask ( G ) output 0 -> Append.B
- [102] Reroute Node (reroutes wires) output 0 -> Subtract(,0.5).A
- [22] Multiply output 0 -> Add.A
- [7] Texture Sample output 1 -> Add.B
- [65] Add output 0 -> Multiply.A
- [38] Power output 0 -> Multiply.B
- [101] Reroute Node (reroutes wires) output 0 -> Clamp.Input
- [35] Clamp output 0 -> Abs.Input
- [49] Clamp output 0 -> Ceil.Input
- [10] 10 output 0 -> Power.Base
- [46] Floor output 0 -> Power.Exponent
- [44] Multiply(,10) output 0 -> Floor.Input
- [37] Ceil output 0 -> Clamp.Input
- [2] Add output 0 -> Lerp.A
- [42] Add output 0 -> Lerp.B
- [95] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [90] Reroute Node (reroutes wires) output 0 -> Add.A
- [45] Divide(,10) output 0 -> Add.B
- [34] Multiply output 0 -> Frac.Input
- [43] Frac output 0 -> Multiply(,10).A
- [39] Floor output 0 -> Divide(,10).A
- [49] Clamp output 0 -> Floor.Input
- [29] Multiply output 0 -> Add(0.5,).B
- [36] Abs output 0 -> Subtract(,1).A
- [48] Subtract(,1) output 0 -> Clamp.Input
- [89] Reroute Node (reroutes wires) output 0 -> SphereMask.A
- [51] -0.5 output 0 -> SphereMask.B
- [50] SphereMask output 0 -> 1-x.Input
- [58] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [55] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( R G ).Input
- [55] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( B A ).Input
- [102] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [60] 2,0.1 output 0 -> Multiply.B
- [82] Add output 0 -> Multiply.A
- [96] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [126] Reroute Node (reroutes wires) output 0 -> Add.A
- [95] Reroute Node (reroutes wires) output 0 -> Add.B
- [52] 1-x output 0 -> Multiply.A
- [62] Add output 0 -> Multiply.B
- [61] Multiply output 0 -> Clamp.Input
- [93] Reroute Node (reroutes wires) output 0 -> Add.A
- [66] Divide(,1e+06) output 0 -> Add.B
- [93] Reroute Node (reroutes wires) output 0 -> Divide(,1e+06).A
- [91] Reroute Node (reroutes wires) output 0 -> Divide.A
- [69] Power output 0 -> Divide.B
- [92] Reroute Node (reroutes wires) output 0 -> Add(,3).A
- [99] 10 output 0 -> Power.Base
- [68] Add(,3) output 0 -> Power.Exponent
- [64] Clamp output 0 -> Mask ( R ).Input
- [3] Input Number (Scalar) output 0 -> Abs.Input
- [94] Reroute Node (reroutes wires) output 0 -> If.A
- [73] 0 output 0 -> If.B
- [73] 0 output 0 -> If.AGreaterThanB
- [73] 0 output 0 -> If.AEqualsB
- [80] 1 output 0 -> If.ALessThanB
- [77] Divide output 0 -> If.A
- [108] 0 output 0 -> If.B
- [109] 1 output 0 -> If.AGreaterThanB
- [108] 0 output 0 -> If.AEqualsB
- [108] 0 output 0 -> If.ALessThanB
- [100] 10 output 0 -> Power.Base
- [76] Add(,2) output 0 -> Power.Exponent
- [92] Reroute Node (reroutes wires) output 0 -> Add(,2).A
- [91] Reroute Node (reroutes wires) output 0 -> Divide.A
- [75] Power output 0 -> Divide.B
- [74] If output 0 -> If.A
- [19] If output 0 -> If.B
- [110] Reroute Node (reroutes wires) output 0 -> If.AGreaterThanB
- [79] 0 output 0 -> If.AEqualsB
- [79] 0 output 0 -> If.ALessThanB
- [78] If output 0 -> Multiply.A
- [72] If output 0 -> Multiply.B
- [33] Add output 0 -> Add.A
- [112] Reroute Node (reroutes wires) output 0 -> Add.B
- [84] Append output 0 -> Texture Sample.Coordinates
- [86] ComputeMipLevel output 0 -> Texture Sample.MipValue
- [85] Frac output 0 -> Append.A
- [97] Reroute Node (reroutes wires) output 0 -> Append.B
- [101] Reroute Node (reroutes wires) output 0 -> Frac.Input
- [101] Reroute Node (reroutes wires) output 0 -> Append.A
- [97] Reroute Node (reroutes wires) output 0 -> Append.B
- [9] Multiply output 0 -> Reroute Node (reroutes wires).Input
- [4] Mask ( G ) output 0 -> Reroute Node (reroutes wires).Input
- [71] Abs output 0 -> Reroute Node (reroutes wires).Input
- [106] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [71] Abs output 0 -> Reroute Node (reroutes wires).Input
- [3] Input Number (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [40] Clamp output 0 -> Reroute Node (reroutes wires).Input
- [103] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [30] Mask ( G ) output 0 -> Reroute Node (reroutes wires).Input
- [9] Multiply output 0 -> Reroute Node (reroutes wires).Input
- [54] TextureCropping output 2 -> Reroute Node (reroutes wires).Input
- [104] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [105] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [54] TextureCropping output 3 -> Reroute Node (reroutes wires).Input
- [24] Add(,1) output 0 -> Reroute Node (reroutes wires).Input
- [117] If output 0 -> Reroute Node (reroutes wires).Input
- [111] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [83] Texture Sample output 2 -> Reroute Node (reroutes wires).Input
- [107] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [115] Reroute Node (reroutes wires) output 0 -> SphereMask.A
- [114] 0.5 output 0 -> SphereMask.B
- [101] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [125] Saturate output 0 -> Multiply.A
- [113] SphereMask output 0 -> Multiply.B
- [118] 1 output 0 -> If.A
- [125] Saturate output 0 -> If.B
- [81] Multiply output 0 -> If.AGreaterThanB
- [119] Multiply output 0 -> If.AEqualsB
- [81] Multiply output 0 -> If.ALessThanB
- [110] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [116] Multiply output 0 -> Multiply.B
- [3] Input Number (Scalar) output 0 -> Add(,1).A
- [120] Add(,1) output 0 -> Ceil.Input
- [123] Multiply(,-1) output 0 -> Ceil.Input
- [3] Input Number (Scalar) output 0 -> Multiply(,-1).A
- [121] Ceil output 0 -> Multiply.A
- [122] Ceil output 0 -> Multiply.B
- [124] Multiply output 0 -> Saturate.Input
- [19] If output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `Result` is driven by [70] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


