# PlotFunctionOnGraph_Derivative

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/PlotFunctionOnGraph_Derivative.PlotFunctionOnGraph_Derivative`
- Category: Engine_MaterialFunctions01/Debug
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Plots f(x) = y to visualize the output of another function. This version includes the derivative which requires running the function twice, once with  x + Δx as the input.

## Inputs

- `f(x)` [FunctionInput_Scalar]
- `Color` [FunctionInput_Vector3]
- `Thickness` [FunctionInput_Scalar]
- `ShowGrid?` [FunctionInput_StaticBool]
- `X range Y range` [FunctionInput_Vector4] - X range=R-G, Y range =B-A
- `f(x + Δx)` [FunctionInput_Scalar]
- `Gradient Height` [FunctionInput_Scalar]

## Outputs

- `f(x) = y`
- `x`
- `x + Δx`
- `d(x)`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output f(x) = y | f(x) = y |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 2 | expression | `MaterialExpressionDistance` | Distance |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input f(x) (Scalar) | f(x) |
| 4 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionFloor` | Floor |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant3Vector` | 2,0,0 |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Thickness (Scalar) | Thickness |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant3Vector` | 0,0.03,0.25 |  |
| 13 | expression | `MaterialExpressionFrac` | Frac |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(,1.05) |  |
| 15 | expression | `MaterialExpressionFloor` | Floor |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 18 | expression | `MaterialExpressionFrac` | Frac |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply(,1.05) |  |
| 20 | expression | `MaterialExpressionFloor` | Floor |  |
| 21 | expression | `MaterialExpressionAdd` | Add |  |
| 22 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input ShowGrid? (StaticBool) | ShowGrid? |
| 25 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 26 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 27 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 28 | expression | `MaterialExpressionDistance` | Distance |  |
| 29 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 30 | expression | `MaterialExpressionAdd` | Add |  |
| 31 | expression | `MaterialExpressionFloor` | Floor |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply(,0.25) |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionClamp` | Clamp |  |
| 35 | expression | `MaterialExpressionClamp` | Clamp |  |
| 36 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 37 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 38 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 39 | expression | `MaterialExpressionAppendVector` | Append |  |
| 40 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 41 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 42 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 43 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input X range Y range (Vector4) | X range Y range |
| 45 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 46 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 47 | function_output | `MaterialExpressionFunctionOutput` | Output x | x |
| 48 | expression | `MaterialExpressionMultiply` | Multiply(,1.001) |  |
| 49 | function_input | `MaterialExpressionFunctionInput` | Input f(x + Δx) (Scalar) | f(x + Δx) |
| 50 | function_output | `MaterialExpressionFunctionOutput` | Output x + Δx | x + Δx |
| 51 | expression | `MaterialExpressionConstant` | 0.0001 |  |
| 52 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 53 | expression | `MaterialExpressionDivide` | Divide |  |
| 54 | expression | `MaterialExpressionDistance` | Distance |  |
| 55 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 56 | expression | `MaterialExpressionAdd` | Add |  |
| 57 | expression | `MaterialExpressionFloor` | Floor |  |
| 58 | expression | `MaterialExpressionClamp` | Clamp |  |
| 59 | expression | `MaterialExpressionMultiply` | Multiply(,0) |  |
| 60 | expression | `MaterialExpressionConstant3Vector` | 0,0.308,0.5 |  |
| 61 | expression | `MaterialExpressionAdd` | Add |  |
| 62 | custom | `MaterialExpressionCustom` | Custom |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |
| 64 | expression | `MaterialExpressionDivide` | Divide(,57.3) |  |
| 65 | custom | `MaterialExpressionCustom` | Custom |  |
| 66 | expression | `MaterialExpressionSubtract` | Subtract(90,) |  |
| 67 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 68 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 69 | expression | `MaterialExpressionDivide` | Divide |  |
| 70 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 71 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugScalarValues |  |
| 72 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 73 | expression | `MaterialExpressionAdd` | Add |  |
| 74 | custom | `MaterialExpressionCustom` | Custom |  |
| 75 | expression | `MaterialExpressionClamp` | Clamp |  |
| 76 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 77 | expression | `MaterialExpressionDivide` | Divide |  |
| 78 | function_input | `MaterialExpressionFunctionInput` | Input Gradient Height (Scalar) | Gradient Height |
| 79 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 80 | expression | `MaterialExpressionClamp` | Clamp |  |
| 81 | expression | `MaterialExpressionCeil` | Ceil |  |
| 82 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 83 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 84 | expression | `MaterialExpressionDivide` | Divide(,400) |  |
| 85 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 86 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 87 | expression | `MaterialExpressionAdd` | Add |  |
| 88 | expression | `MaterialExpressionAbs` | Abs |  |
| 89 | custom | `MaterialExpressionCustom` | Custom |  |
| 90 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 91 | expression | `MaterialExpressionIf` | If |  |
| 92 | expression | `MaterialExpressionConstant` | 0 |  |
| 93 | expression | `MaterialExpressionIf` | If |  |
| 94 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 95 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 96 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 97 | expression | `MaterialExpressionAdd` | Add |  |
| 98 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 99 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 100 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 101 | expression | `MaterialExpressionCeil` | Ceil |  |
| 102 | expression | `MaterialExpressionCeil` | Ceil |  |
| 103 | expression | `MaterialExpressionClamp` | Clamp |  |
| 104 | expression | `MaterialExpressionAdd` | Add |  |
| 105 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 106 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 107 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 108 | expression | `MaterialExpressionIf` | If |  |
| 109 | expression | `MaterialExpressionConstant` | 0 |  |
| 110 | expression | `MaterialExpressionConstant` | 0 |  |
| 111 | custom | `MaterialExpressionCustom` | Custom |  |
| 112 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |
| 113 | expression | `MaterialExpressionDivide` | Divide(,57.3) |  |
| 114 | custom | `MaterialExpressionCustom` | Custom |  |
| 115 | expression | `MaterialExpressionSubtract` | Subtract(90,) |  |
| 116 | expression | `MaterialExpressionClamp` | Clamp |  |
| 117 | expression | `MaterialExpressionClamp` | Clamp |  |
| 118 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDepthFade |  |
| 119 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,0,) |  |
| 120 | expression | `MaterialExpressionConstant` | 1000 |  |
| 121 | expression | `MaterialExpressionConstant` | 3500 |  |
| 122 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 123 | function_call | `MaterialExpressionMaterialFunctionCall` | AlphaOffset |  |
| 124 | expression | `MaterialExpressionConstant` | 0.005 |  |
| 125 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 126 | expression | `MaterialExpressionDivide` | Divide |  |
| 127 | expression | `MaterialExpressionClamp` | Clamp |  |
| 128 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 129 | expression | `MaterialExpressionConstant` | 0.05 |  |
| 130 | expression | `MaterialExpressionDivide` | Divide(,1) |  |
| 131 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDepthFade |  |
| 132 | expression | `MaterialExpressionConstant` | 6500 |  |
| 133 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 134 | function_output | `MaterialExpressionFunctionOutput` | Output d(x) | d(x) |
| 135 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 136 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 137 | expression | `MaterialExpressionAdd` | Add(0,) |  |
| 138 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 139 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 140 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 141 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |

## Exact Input Wiring

### [0] Output f(x) = y

- `A` <= [122] Multiply output 0

### [1] Mask ( G )

- `Input` <= [39] Append output 0

### [2] Distance

- `A` <= [1] Mask ( G ) output 0
- `B` <= [3] Input f(x) (Scalar) output 0

### [4] 1-x

- `Input` <= [2] Distance output 0

### [5] Add

- `A` <= [4] 1-x output 0
- `B` <= [138] Multiply output 0

### [6] Floor

- `Input` <= [5] Add output 0

### [7] Multiply

- `A` <= [35] Clamp output 0
- `B` <= [9] Input Color (Vector3) output 0

### [9] Input Color (Vector3)

- `Preview` <= [8] 2,0,0 output 0

### [11] Multiply

- `A` <= [22] Texture Sample output 0
- `B` <= [12] 0,0.03,0.25 output 0

### [13] Frac

- `Input` <= [16] Mask ( R ) output 0

### [14] Multiply(,1.05)

- `A` <= [13] Frac output 0

### [15] Floor

- `Input` <= [14] Multiply(,1.05) output 0

### [18] Frac

- `Input` <= [17] Mask ( G ) output 0

### [19] Multiply(,1.05)

- `A` <= [18] Frac output 0

### [20] Floor

- `Input` <= [19] Multiply(,1.05) output 0

### [21] Add

- `A` <= [15] Floor output 0
- `B` <= [20] Floor output 0

### [22] Texture Sample

- `Coordinates` <= [48] Multiply(,1.001) output 0

### [23] Add

- `A` <= [79] Lerp output 0
- `B` <= [33] Add output 0

### [24] Input ShowGrid? (StaticBool)

- `Preview` <= [25] Static Bool (True) output 0

### [26] Switch

- `A` <= [107] Multiply output 0
- `B` <= [33] Add output 0
- `Value` <= [24] Input ShowGrid? (StaticBool) output 0

### [27] Mask ( R )

- `Input` <= [39] Append output 0

### [28] Distance

- `A` <= [27] Mask ( R ) output 0
- `B` <= [1] Mask ( G ) output 0

### [29] 1-x

- `Input` <= [28] Distance output 0

### [30] Add

- `A` <= [29] 1-x output 0
- `B` <= [139] Multiply(,2) output 0

### [31] Floor

- `Input` <= [30] Add output 0

### [32] Multiply(,0.25)

- `A` <= [34] Clamp output 0

### [33] Add

- `A` <= [32] Multiply(,0.25) output 0
- `B` <= [61] Add output 0

### [34] Clamp

- `Input` <= [31] Floor output 0

### [35] Clamp

- `Input` <= [6] Floor output 0

### [37] Lerp

- `A` <= [91] If output 0
- `B` <= [93] If output 0
- `Alpha` <= [38] Mask ( G ) output 0

### [38] Mask ( G )

- `Input` <= [36] TexCoord[0] output 0

### [39] Append

- `A` <= [43] Lerp output 0
- `B` <= [37] Lerp output 0

### [40] Mask ( R )

- `Input` <= [36] TexCoord[0] output 0

### [41] Mask ( B )

- `Input` <= [44] Input X range Y range (Vector4) output 0

### [42] Mask ( A )

- `Input` <= [44] Input X range Y range (Vector4) output 0

### [43] Lerp

- `A` <= [96] Subtract output 0
- `B` <= [97] Add output 0
- `Alpha` <= [40] Mask ( R ) output 0

### [45] Mask ( R )

- `Input` <= [44] Input X range Y range (Vector4) output 0

### [46] Mask ( G )

- `Input` <= [44] Input X range Y range (Vector4) output 0

### [47] Output x

- `A` <= [43] Lerp output 0

### [48] Multiply(,1.001)

- `A` <= [39] Append output 0

### [50] Output x + Δx

- `A` <= [73] Add output 0

### [52] Subtract

- `A` <= [49] Input f(x + Δx) (Scalar) output 0
- `B` <= [3] Input f(x) (Scalar) output 0

### [53] Divide

- `A` <= [52] Subtract output 0
- `B` <= [51] 0.0001 output 0

### [54] Distance

- `A` <= [1] Mask ( G ) output 0
- `B` <= [108] If output 0

### [55] 1-x

- `Input` <= [54] Distance output 0

### [56] Add

- `A` <= [55] 1-x output 0
- `B` <= [77] Divide output 0

### [57] Floor

- `Input` <= [56] Add output 0

### [58] Clamp

- `Input` <= [57] Floor output 0

### [59] Multiply(,0)

- `A` <= [58] Clamp output 0

### [61] Add

- `A` <= [7] Multiply output 0
- `B` <= [59] Multiply(,0) output 0

### [63] Multiply(,57.3)

- `A` <= [62] Custom output 0

### [64] Divide(,57.3)

- `A` <= [66] Subtract(90,) output 0

### [66] Subtract(90,)

- `B` <= [63] Multiply(,57.3) output 0

### [67] Subtract

- `A` <= [46] Mask ( G ) output 0
- `B` <= [45] Mask ( R ) output 0

### [68] Subtract

- `A` <= [42] Mask ( A ) output 0
- `B` <= [41] Mask ( B ) output 0

### [69] Divide

- `A` <= [67] Subtract output 0
- `B` <= [68] Subtract output 0

### [70] Add(,1)

- `A` <= [71] DebugScalarValues output 0

### [72] Divide(1,)

- `B` <= [86] Subtract output 0

### [73] Add

- `A` <= [43] Lerp output 0
- `B` <= [51] 0.0001 output 0

### [75] Clamp

- `Input` <= [135] Divide(1,) output 0

### [76] Multiply

- `A` <= [69] Divide output 0
- `B` <= [108] If output 0

### [77] Divide

- `A` <= [84] Divide(,400) output 0
- `B` <= [72] Divide(1,) output 0

### [79] Lerp

- `A` <= [11] Multiply output 0
- `B` <= [3] Input f(x) (Scalar) output 0
- `Alpha` <= [80] Clamp output 0

### [80] Clamp

- `Input` <= [81] Ceil output 0

### [81] Ceil

- `Input` <= [82] Subtract output 0

### [82] Subtract

- `A` <= [90] Multiply output 0
- `B` <= [83] Multiply(,1) output 0

### [83] Multiply(,1)

- `A` <= [42] Mask ( A ) output 0

### [84] Divide(,400)

- `A` <= [10] Input Thickness (Scalar) output 0

### [85] Multiply

- `A` <= [88] Abs output 0
- `B` <= [68] Subtract output 0

### [86] Subtract

- `A` <= [91] If output 0
- `B` <= [93] If output 0

### [87] Add

- `A` <= [85] Multiply output 0
- `B` <= [42] Mask ( A ) output 0

### [88] Abs

- `Input` <= [78] Input Gradient Height (Scalar) output 0

### [90] Multiply

- `A` <= [89] Custom output 0
- `B` <= [1] Mask ( G ) output 0

### [91] If

- `A` <= [78] Input Gradient Height (Scalar) output 0
- `B` <= [92] 0 output 0
- `AGreaterThanB` <= [87] Add output 0
- `AEqualsB` <= [42] Mask ( A ) output 0
- `ALessThanB` <= [42] Mask ( A ) output 0

### [93] If

- `A` <= [78] Input Gradient Height (Scalar) output 0
- `B` <= [92] 0 output 0
- `AGreaterThanB` <= [41] Mask ( B ) output 0
- `AEqualsB` <= [41] Mask ( B ) output 0
- `ALessThanB` <= [94] Subtract output 0

### [94] Subtract

- `A` <= [41] Mask ( B ) output 0
- `B` <= [85] Multiply output 0

### [95] Add(,1)

- `A` <= [70] Add(,1) output 0

### [96] Subtract

- `A` <= [45] Mask ( R ) output 0
- `B` <= [98] Multiply(,0.5) output 0

### [97] Add

- `A` <= [46] Mask ( G ) output 0
- `B` <= [98] Multiply(,0.5) output 0

### [98] Multiply(,0.5)

- `A` <= [88] Abs output 0

### [99] Subtract

- `A` <= [100] Multiply(,1) output 0
- `B` <= [45] Mask ( R ) output 0

### [100] Multiply(,1)

- `A` <= [43] Lerp output 0

### [101] Ceil

- `Input` <= [99] Subtract output 0

### [102] Ceil

- `Input` <= [104] Add output 0

### [103] Clamp

- `Input` <= [106] Multiply output 0

### [104] Add

- `A` <= [105] Multiply(,-1) output 0
- `B` <= [46] Mask ( G ) output 0

### [105] Multiply(,-1)

- `A` <= [100] Multiply(,1) output 0

### [106] Multiply

- `A` <= [101] Ceil output 0
- `B` <= [102] Ceil output 0

### [107] Multiply

- `A` <= [103] Clamp output 0
- `B` <= [23] Add output 0

### [108] If

- `A` <= [49] Input f(x + Δx) (Scalar) output 0
- `B` <= [109] 0 output 0
- `AGreaterThanB` <= [53] Divide output 0
- `AEqualsB` <= [110] 0 output 0
- `ALessThanB` <= [53] Divide output 0

### [112] Multiply(,57.3)

- `A` <= [111] Custom output 0

### [113] Divide(,57.3)

- `A` <= [115] Subtract(90,) output 0

### [115] Subtract(90,)

- `B` <= [112] Multiply(,57.3) output 0

### [116] Clamp

- `Input` <= [114] Custom output 0

### [117] Clamp

- `Input` <= [26] Switch output 0

### [119] Lerp(1,0,)

- `Alpha` <= [131] CameraDepthFade output 0

### [122] Multiply

- `A` <= [119] Lerp(1,0,) output 0
- `B` <= [117] Clamp output 0

### [125] 1-x

- `Input` <= [127] Clamp output 0

### [126] Divide

- `A` <= [128] Lerp output 0
- `B` <= [72] Divide(1,) output 0

### [127] Clamp

- `Input` <= [130] Divide(,1) output 0

### [128] Lerp

- `A` <= [124] 0.005 output 0
- `B` <= [129] 0.05 output 0
- `Alpha` <= [118] CameraDepthFade output 0

### [130] Divide(,1)

- `A` <= [126] Divide output 0

### [133] 1-x

- `Input` <= [76] Multiply output 0

### [134] Output d(x)

- `A` <= [76] Multiply output 0

### [135] Divide(1,)

- `B` <= [133] 1-x output 0

### [136] Add(,1)

- `A` <= [140] Multiply output 0

### [137] Add(0,)

- `B` <= [76] Multiply output 0

### [138] Multiply

- `A` <= [77] Divide output 0
- `B` <= [141] Sqrt output 0

### [139] Multiply(,2)

- `A` <= [77] Divide output 0

### [140] Multiply

- `A` <= [76] Multiply output 0
- `B` <= [76] Multiply output 0

### [141] Sqrt

- `Input` <= [136] Add(,1) output 0

## Connection List

- [122] Multiply output 0 -> Output f(x) = y.A
- [39] Append output 0 -> Mask ( G ).Input
- [1] Mask ( G ) output 0 -> Distance.A
- [3] Input f(x) (Scalar) output 0 -> Distance.B
- [2] Distance output 0 -> 1-x.Input
- [4] 1-x output 0 -> Add.A
- [138] Multiply output 0 -> Add.B
- [5] Add output 0 -> Floor.Input
- [35] Clamp output 0 -> Multiply.A
- [9] Input Color (Vector3) output 0 -> Multiply.B
- [8] 2,0,0 output 0 -> Input Color (Vector3).Preview
- [22] Texture Sample output 0 -> Multiply.A
- [12] 0,0.03,0.25 output 0 -> Multiply.B
- [16] Mask ( R ) output 0 -> Frac.Input
- [13] Frac output 0 -> Multiply(,1.05).A
- [14] Multiply(,1.05) output 0 -> Floor.Input
- [17] Mask ( G ) output 0 -> Frac.Input
- [18] Frac output 0 -> Multiply(,1.05).A
- [19] Multiply(,1.05) output 0 -> Floor.Input
- [15] Floor output 0 -> Add.A
- [20] Floor output 0 -> Add.B
- [48] Multiply(,1.001) output 0 -> Texture Sample.Coordinates
- [79] Lerp output 0 -> Add.A
- [33] Add output 0 -> Add.B
- [25] Static Bool (True) output 0 -> Input ShowGrid? (StaticBool).Preview
- [107] Multiply output 0 -> Switch.A
- [33] Add output 0 -> Switch.B
- [24] Input ShowGrid? (StaticBool) output 0 -> Switch.Value
- [39] Append output 0 -> Mask ( R ).Input
- [27] Mask ( R ) output 0 -> Distance.A
- [1] Mask ( G ) output 0 -> Distance.B
- [28] Distance output 0 -> 1-x.Input
- [29] 1-x output 0 -> Add.A
- [139] Multiply(,2) output 0 -> Add.B
- [30] Add output 0 -> Floor.Input
- [34] Clamp output 0 -> Multiply(,0.25).A
- [32] Multiply(,0.25) output 0 -> Add.A
- [61] Add output 0 -> Add.B
- [31] Floor output 0 -> Clamp.Input
- [6] Floor output 0 -> Clamp.Input
- [91] If output 0 -> Lerp.A
- [93] If output 0 -> Lerp.B
- [38] Mask ( G ) output 0 -> Lerp.Alpha
- [36] TexCoord[0] output 0 -> Mask ( G ).Input
- [43] Lerp output 0 -> Append.A
- [37] Lerp output 0 -> Append.B
- [36] TexCoord[0] output 0 -> Mask ( R ).Input
- [44] Input X range Y range (Vector4) output 0 -> Mask ( B ).Input
- [44] Input X range Y range (Vector4) output 0 -> Mask ( A ).Input
- [96] Subtract output 0 -> Lerp.A
- [97] Add output 0 -> Lerp.B
- [40] Mask ( R ) output 0 -> Lerp.Alpha
- [44] Input X range Y range (Vector4) output 0 -> Mask ( R ).Input
- [44] Input X range Y range (Vector4) output 0 -> Mask ( G ).Input
- [43] Lerp output 0 -> Output x.A
- [39] Append output 0 -> Multiply(,1.001).A
- [73] Add output 0 -> Output x + Δx.A
- [49] Input f(x + Δx) (Scalar) output 0 -> Subtract.A
- [3] Input f(x) (Scalar) output 0 -> Subtract.B
- [52] Subtract output 0 -> Divide.A
- [51] 0.0001 output 0 -> Divide.B
- [1] Mask ( G ) output 0 -> Distance.A
- [108] If output 0 -> Distance.B
- [54] Distance output 0 -> 1-x.Input
- [55] 1-x output 0 -> Add.A
- [77] Divide output 0 -> Add.B
- [56] Add output 0 -> Floor.Input
- [57] Floor output 0 -> Clamp.Input
- [58] Clamp output 0 -> Multiply(,0).A
- [7] Multiply output 0 -> Add.A
- [59] Multiply(,0) output 0 -> Add.B
- [62] Custom output 0 -> Multiply(,57.3).A
- [66] Subtract(90,) output 0 -> Divide(,57.3).A
- [63] Multiply(,57.3) output 0 -> Subtract(90,).B
- [46] Mask ( G ) output 0 -> Subtract.A
- [45] Mask ( R ) output 0 -> Subtract.B
- [42] Mask ( A ) output 0 -> Subtract.A
- [41] Mask ( B ) output 0 -> Subtract.B
- [67] Subtract output 0 -> Divide.A
- [68] Subtract output 0 -> Divide.B
- [71] DebugScalarValues output 0 -> Add(,1).A
- [86] Subtract output 0 -> Divide(1,).B
- [43] Lerp output 0 -> Add.A
- [51] 0.0001 output 0 -> Add.B
- [135] Divide(1,) output 0 -> Clamp.Input
- [69] Divide output 0 -> Multiply.A
- [108] If output 0 -> Multiply.B
- [84] Divide(,400) output 0 -> Divide.A
- [72] Divide(1,) output 0 -> Divide.B
- [11] Multiply output 0 -> Lerp.A
- [3] Input f(x) (Scalar) output 0 -> Lerp.B
- [80] Clamp output 0 -> Lerp.Alpha
- [81] Ceil output 0 -> Clamp.Input
- [82] Subtract output 0 -> Ceil.Input
- [90] Multiply output 0 -> Subtract.A
- [83] Multiply(,1) output 0 -> Subtract.B
- [42] Mask ( A ) output 0 -> Multiply(,1).A
- [10] Input Thickness (Scalar) output 0 -> Divide(,400).A
- [88] Abs output 0 -> Multiply.A
- [68] Subtract output 0 -> Multiply.B
- [91] If output 0 -> Subtract.A
- [93] If output 0 -> Subtract.B
- [85] Multiply output 0 -> Add.A
- [42] Mask ( A ) output 0 -> Add.B
- [78] Input Gradient Height (Scalar) output 0 -> Abs.Input
- [89] Custom output 0 -> Multiply.A
- [1] Mask ( G ) output 0 -> Multiply.B
- [78] Input Gradient Height (Scalar) output 0 -> If.A
- [92] 0 output 0 -> If.B
- [87] Add output 0 -> If.AGreaterThanB
- [42] Mask ( A ) output 0 -> If.AEqualsB
- [42] Mask ( A ) output 0 -> If.ALessThanB
- [78] Input Gradient Height (Scalar) output 0 -> If.A
- [92] 0 output 0 -> If.B
- [41] Mask ( B ) output 0 -> If.AGreaterThanB
- [41] Mask ( B ) output 0 -> If.AEqualsB
- [94] Subtract output 0 -> If.ALessThanB
- [41] Mask ( B ) output 0 -> Subtract.A
- [85] Multiply output 0 -> Subtract.B
- [70] Add(,1) output 0 -> Add(,1).A
- [45] Mask ( R ) output 0 -> Subtract.A
- [98] Multiply(,0.5) output 0 -> Subtract.B
- [46] Mask ( G ) output 0 -> Add.A
- [98] Multiply(,0.5) output 0 -> Add.B
- [88] Abs output 0 -> Multiply(,0.5).A
- [100] Multiply(,1) output 0 -> Subtract.A
- [45] Mask ( R ) output 0 -> Subtract.B
- [43] Lerp output 0 -> Multiply(,1).A
- [99] Subtract output 0 -> Ceil.Input
- [104] Add output 0 -> Ceil.Input
- [106] Multiply output 0 -> Clamp.Input
- [105] Multiply(,-1) output 0 -> Add.A
- [46] Mask ( G ) output 0 -> Add.B
- [100] Multiply(,1) output 0 -> Multiply(,-1).A
- [101] Ceil output 0 -> Multiply.A
- [102] Ceil output 0 -> Multiply.B
- [103] Clamp output 0 -> Multiply.A
- [23] Add output 0 -> Multiply.B
- [49] Input f(x + Δx) (Scalar) output 0 -> If.A
- [109] 0 output 0 -> If.B
- [53] Divide output 0 -> If.AGreaterThanB
- [110] 0 output 0 -> If.AEqualsB
- [53] Divide output 0 -> If.ALessThanB
- [111] Custom output 0 -> Multiply(,57.3).A
- [115] Subtract(90,) output 0 -> Divide(,57.3).A
- [112] Multiply(,57.3) output 0 -> Subtract(90,).B
- [114] Custom output 0 -> Clamp.Input
- [26] Switch output 0 -> Clamp.Input
- [131] CameraDepthFade output 0 -> Lerp(1,0,).Alpha
- [119] Lerp(1,0,) output 0 -> Multiply.A
- [117] Clamp output 0 -> Multiply.B
- [127] Clamp output 0 -> 1-x.Input
- [128] Lerp output 0 -> Divide.A
- [72] Divide(1,) output 0 -> Divide.B
- [130] Divide(,1) output 0 -> Clamp.Input
- [124] 0.005 output 0 -> Lerp.A
- [129] 0.05 output 0 -> Lerp.B
- [118] CameraDepthFade output 0 -> Lerp.Alpha
- [126] Divide output 0 -> Divide(,1).A
- [76] Multiply output 0 -> 1-x.Input
- [76] Multiply output 0 -> Output d(x).A
- [133] 1-x output 0 -> Divide(1,).B
- [140] Multiply output 0 -> Add(,1).A
- [76] Multiply output 0 -> Add(0,).B
- [77] Divide output 0 -> Multiply.A
- [141] Sqrt output 0 -> Multiply.B
- [77] Divide output 0 -> Multiply(,2).A
- [76] Multiply output 0 -> Multiply.A
- [76] Multiply output 0 -> Multiply.B
- [136] Add(,1) output 0 -> Sqrt.Input

## Output Trace Roots

- `f(x) = y` is driven by [122] Multiply
- `x` is driven by [43] Lerp
- `x + Δx` is driven by [73] Add
- `d(x)` is driven by [76] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
