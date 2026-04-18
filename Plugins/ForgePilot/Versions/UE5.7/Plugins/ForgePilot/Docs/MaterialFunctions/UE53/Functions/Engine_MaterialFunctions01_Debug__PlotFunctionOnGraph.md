# PlotFunctionOnGraph

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/PlotFunctionOnGraph.PlotFunctionOnGraph`
- Category: Engine_MaterialFunctions01/Debug
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Plots f(x) = y to visualize the output of another function.

## Inputs

- `f(x)` [FunctionInput_Scalar]
- `Color` [FunctionInput_Vector3]
- `Thickness` [FunctionInput_Scalar]
- `ShowGrid?` [FunctionInput_StaticBool]
- `X range Y range` [FunctionInput_Vector4] - X range=R-G, Y range =B-A
- `Gradient Height` [FunctionInput_Scalar]

## Outputs

- `f(x) = y`
- `x`

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
| 12 | expression | `MaterialExpressionConstant3Vector` | 0,0.02,0.15 |  |
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
| 23 | function_input | `MaterialExpressionFunctionInput` | Input ShowGrid? (StaticBool) | ShowGrid? |
| 24 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 25 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 26 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 27 | expression | `MaterialExpressionDistance` | Distance |  |
| 28 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 29 | expression | `MaterialExpressionAdd` | Add |  |
| 30 | expression | `MaterialExpressionFloor` | Floor |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply(,0.1) |  |
| 32 | expression | `MaterialExpressionAdd` | Add |  |
| 33 | expression | `MaterialExpressionClamp` | Clamp |  |
| 34 | expression | `MaterialExpressionClamp` | Clamp |  |
| 35 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 36 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 37 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 38 | expression | `MaterialExpressionAppendVector` | Append |  |
| 39 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 40 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 41 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 42 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input X range Y range (Vector4) | X range Y range |
| 44 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 45 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 46 | function_output | `MaterialExpressionFunctionOutput` | Output x | x |
| 47 | expression | `MaterialExpressionMultiply` | Multiply(,1.001) |  |
| 48 | expression | `MaterialExpressionConstant` | 0.0001 |  |
| 49 | expression | `MaterialExpressionSubtract` | Subtract(1,) |  |
| 50 | expression | `MaterialExpressionDivide` | Divide |  |
| 51 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 52 | custom | `MaterialExpressionCustom` | Custom |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |
| 54 | expression | `MaterialExpressionDivide` | Divide(,57.3) |  |
| 55 | custom | `MaterialExpressionCustom` | Custom |  |
| 56 | expression | `MaterialExpressionSubtract` | Subtract(90,) |  |
| 57 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 58 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 59 | expression | `MaterialExpressionDivide` | Divide |  |
| 60 | expression | `MaterialExpressionDivide` | Divide |  |
| 61 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 62 | expression | `MaterialExpressionAdd` | Add |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply(0,1) |  |
| 64 | custom | `MaterialExpressionCustom` | Custom |  |
| 65 | expression | `MaterialExpressionClamp` | Clamp |  |
| 66 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 67 | expression | `MaterialExpressionDivide` | Divide |  |
| 68 | expression | `MaterialExpressionDivide` | Divide(,1.41) |  |
| 69 | function_input | `MaterialExpressionFunctionInput` | Input Gradient Height (Scalar) | Gradient Height |
| 70 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 71 | expression | `MaterialExpressionClamp` | Clamp |  |
| 72 | expression | `MaterialExpressionCeil` | Ceil |  |
| 73 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 74 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 75 | expression | `MaterialExpressionDivide` | Divide(,200) |  |
| 76 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 77 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 78 | expression | `MaterialExpressionAdd` | Add |  |
| 79 | expression | `MaterialExpressionAbs` | Abs |  |
| 80 | custom | `MaterialExpressionCustom` | Custom |  |
| 81 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 82 | expression | `MaterialExpressionIf` | If |  |
| 83 | expression | `MaterialExpressionConstant` | 0 |  |
| 84 | expression | `MaterialExpressionIf` | If |  |
| 85 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 86 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 87 | expression | `MaterialExpressionAdd` | Add |  |
| 88 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 89 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 90 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 91 | expression | `MaterialExpressionCeil` | Ceil |  |
| 92 | expression | `MaterialExpressionCeil` | Ceil |  |
| 93 | expression | `MaterialExpressionClamp` | Clamp |  |
| 94 | expression | `MaterialExpressionAdd` | Add |  |
| 95 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 96 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 97 | expression | `MaterialExpressionConstant` | 1 |  |
| 98 | expression | `MaterialExpressionAdd` | Add |  |
| 99 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output f(x) = y

- `A` <= [25] Switch output 0

### [1] Mask ( G )

- `Input` <= [38] Append output 0

### [2] Distance

- `A` <= [1] Mask ( G ) output 0
- `B` <= [3] Input f(x) (Scalar) output 0

### [4] 1-x

- `Input` <= [2] Distance output 0

### [5] Add

- `A` <= [4] 1-x output 0
- `B` <= [67] Divide output 0

### [6] Floor

- `Input` <= [5] Add output 0

### [7] Multiply

- `A` <= [34] Clamp output 0
- `B` <= [9] Input Color (Vector3) output 0

### [9] Input Color (Vector3)

- `Preview` <= [8] 2,0,0 output 0

### [11] Multiply

- `A` <= [22] Texture Sample output 0
- `B` <= [12] 0,0.02,0.15 output 0

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

- `Coordinates` <= [47] Multiply(,1.001) output 0

### [23] Input ShowGrid? (StaticBool)

- `Preview` <= [24] Static Bool (True) output 0

### [25] Switch

- `A` <= [99] Multiply output 0
- `B` <= [32] Add output 0
- `Value` <= [23] Input ShowGrid? (StaticBool) output 0

### [26] Mask ( R )

- `Input` <= [38] Append output 0

### [27] Distance

- `A` <= [26] Mask ( R ) output 0
- `B` <= [1] Mask ( G ) output 0

### [28] 1-x

- `Input` <= [68] Divide(,1.41) output 0

### [29] Add

- `A` <= [28] 1-x output 0
- `B` <= [67] Divide output 0

### [30] Floor

- `Input` <= [29] Add output 0

### [31] Multiply(,0.1)

- `A` <= [33] Clamp output 0

### [32] Add

- `A` <= [31] Multiply(,0.1) output 0
- `B` <= [7] Multiply output 0

### [33] Clamp

- `Input` <= [30] Floor output 0

### [34] Clamp

- `Input` <= [6] Floor output 0

### [36] Lerp

- `A` <= [82] If output 0
- `B` <= [84] If output 0
- `Alpha` <= [37] Mask ( G ) output 0

### [37] Mask ( G )

- `Input` <= [35] TexCoord[0] output 0

### [38] Append

- `A` <= [42] Lerp output 0
- `B` <= [36] Lerp output 0

### [39] Mask ( R )

- `Input` <= [35] TexCoord[0] output 0

### [40] Mask ( B )

- `Input` <= [43] Input X range Y range (Vector4) output 0

### [41] Mask ( A )

- `Input` <= [43] Input X range Y range (Vector4) output 0

### [42] Lerp

- `A` <= [86] Subtract output 0
- `B` <= [87] Add output 0
- `Alpha` <= [39] Mask ( R ) output 0

### [44] Mask ( R )

- `Input` <= [43] Input X range Y range (Vector4) output 0

### [45] Mask ( G )

- `Input` <= [43] Input X range Y range (Vector4) output 0

### [46] Output x

- `A` <= [42] Lerp output 0

### [47] Multiply(,1.001)

- `A` <= [38] Append output 0

### [49] Subtract(1,)

- `B` <= [3] Input f(x) (Scalar) output 0

### [50] Divide

- `A` <= [97] 1 output 0
- `B` <= [48] 0.0001 output 0

### [51] Add(,1)

- `A` <= [7] Multiply output 0

### [53] Multiply(,57.3)

- `A` <= [52] Custom output 0

### [54] Divide(,57.3)

- `A` <= [56] Subtract(90,) output 0

### [56] Subtract(90,)

- `B` <= [53] Multiply(,57.3) output 0

### [57] Subtract

- `A` <= [45] Mask ( G ) output 0
- `B` <= [44] Mask ( R ) output 0

### [58] Subtract

- `A` <= [41] Mask ( A ) output 0
- `B` <= [40] Mask ( B ) output 0

### [59] Divide

- `A` <= [57] Subtract output 0
- `B` <= [58] Subtract output 0

### [60] Divide

- `A` <= [67] Divide output 0
- `B` <= [65] Clamp output 0

### [61] Divide(1,)

- `B` <= [77] Subtract output 0

### [62] Add

- `A` <= [42] Lerp output 0
- `B` <= [48] 0.0001 output 0

### [65] Clamp

- `Input` <= [55] Custom output 0

### [66] Multiply

- `A` <= [59] Divide output 0
- `B` <= [50] Divide output 0

### [67] Divide

- `A` <= [75] Divide(,200) output 0
- `B` <= [61] Divide(1,) output 0

### [68] Divide(,1.41)

- `A` <= [27] Distance output 0

### [70] Lerp

- `A` <= [11] Multiply output 0
- `B` <= [3] Input f(x) (Scalar) output 0
- `Alpha` <= [71] Clamp output 0

### [71] Clamp

- `Input` <= [72] Ceil output 0

### [72] Ceil

- `Input` <= [73] Subtract output 0

### [73] Subtract

- `A` <= [81] Multiply output 0
- `B` <= [74] Multiply(,1) output 0

### [74] Multiply(,1)

- `A` <= [41] Mask ( A ) output 0

### [75] Divide(,200)

- `A` <= [10] Input Thickness (Scalar) output 0

### [76] Multiply

- `A` <= [79] Abs output 0
- `B` <= [58] Subtract output 0

### [77] Subtract

- `A` <= [82] If output 0
- `B` <= [84] If output 0

### [78] Add

- `A` <= [76] Multiply output 0
- `B` <= [41] Mask ( A ) output 0

### [79] Abs

- `Input` <= [69] Input Gradient Height (Scalar) output 0

### [81] Multiply

- `A` <= [80] Custom output 0
- `B` <= [1] Mask ( G ) output 0

### [82] If

- `A` <= [69] Input Gradient Height (Scalar) output 0
- `B` <= [83] 0 output 0
- `AGreaterThanB` <= [78] Add output 0
- `AEqualsB` <= [41] Mask ( A ) output 0
- `ALessThanB` <= [41] Mask ( A ) output 0

### [84] If

- `A` <= [69] Input Gradient Height (Scalar) output 0
- `B` <= [83] 0 output 0
- `AGreaterThanB` <= [40] Mask ( B ) output 0
- `AEqualsB` <= [40] Mask ( B ) output 0
- `ALessThanB` <= [85] Subtract output 0

### [85] Subtract

- `A` <= [40] Mask ( B ) output 0
- `B` <= [76] Multiply output 0

### [86] Subtract

- `A` <= [44] Mask ( R ) output 0
- `B` <= [88] Multiply(,0.5) output 0

### [87] Add

- `A` <= [45] Mask ( G ) output 0
- `B` <= [88] Multiply(,0.5) output 0

### [88] Multiply(,0.5)

- `A` <= [76] Multiply output 0

### [89] Subtract

- `A` <= [90] Multiply(,1) output 0
- `B` <= [44] Mask ( R ) output 0

### [90] Multiply(,1)

- `A` <= [42] Lerp output 0

### [91] Ceil

- `Input` <= [89] Subtract output 0

### [92] Ceil

- `Input` <= [94] Add output 0

### [93] Clamp

- `Input` <= [96] Multiply output 0

### [94] Add

- `A` <= [95] Multiply(,-1) output 0
- `B` <= [45] Mask ( G ) output 0

### [95] Multiply(,-1)

- `A` <= [90] Multiply(,1) output 0

### [96] Multiply

- `A` <= [91] Ceil output 0
- `B` <= [92] Ceil output 0

### [98] Add

- `A` <= [70] Lerp output 0
- `B` <= [32] Add output 0

### [99] Multiply

- `A` <= [93] Clamp output 0
- `B` <= [98] Add output 0

## Connection List

- [25] Switch output 0 -> Output f(x) = y.A
- [38] Append output 0 -> Mask ( G ).Input
- [1] Mask ( G ) output 0 -> Distance.A
- [3] Input f(x) (Scalar) output 0 -> Distance.B
- [2] Distance output 0 -> 1-x.Input
- [4] 1-x output 0 -> Add.A
- [67] Divide output 0 -> Add.B
- [5] Add output 0 -> Floor.Input
- [34] Clamp output 0 -> Multiply.A
- [9] Input Color (Vector3) output 0 -> Multiply.B
- [8] 2,0,0 output 0 -> Input Color (Vector3).Preview
- [22] Texture Sample output 0 -> Multiply.A
- [12] 0,0.02,0.15 output 0 -> Multiply.B
- [16] Mask ( R ) output 0 -> Frac.Input
- [13] Frac output 0 -> Multiply(,1.05).A
- [14] Multiply(,1.05) output 0 -> Floor.Input
- [17] Mask ( G ) output 0 -> Frac.Input
- [18] Frac output 0 -> Multiply(,1.05).A
- [19] Multiply(,1.05) output 0 -> Floor.Input
- [15] Floor output 0 -> Add.A
- [20] Floor output 0 -> Add.B
- [47] Multiply(,1.001) output 0 -> Texture Sample.Coordinates
- [24] Static Bool (True) output 0 -> Input ShowGrid? (StaticBool).Preview
- [99] Multiply output 0 -> Switch.A
- [32] Add output 0 -> Switch.B
- [23] Input ShowGrid? (StaticBool) output 0 -> Switch.Value
- [38] Append output 0 -> Mask ( R ).Input
- [26] Mask ( R ) output 0 -> Distance.A
- [1] Mask ( G ) output 0 -> Distance.B
- [68] Divide(,1.41) output 0 -> 1-x.Input
- [28] 1-x output 0 -> Add.A
- [67] Divide output 0 -> Add.B
- [29] Add output 0 -> Floor.Input
- [33] Clamp output 0 -> Multiply(,0.1).A
- [31] Multiply(,0.1) output 0 -> Add.A
- [7] Multiply output 0 -> Add.B
- [30] Floor output 0 -> Clamp.Input
- [6] Floor output 0 -> Clamp.Input
- [82] If output 0 -> Lerp.A
- [84] If output 0 -> Lerp.B
- [37] Mask ( G ) output 0 -> Lerp.Alpha
- [35] TexCoord[0] output 0 -> Mask ( G ).Input
- [42] Lerp output 0 -> Append.A
- [36] Lerp output 0 -> Append.B
- [35] TexCoord[0] output 0 -> Mask ( R ).Input
- [43] Input X range Y range (Vector4) output 0 -> Mask ( B ).Input
- [43] Input X range Y range (Vector4) output 0 -> Mask ( A ).Input
- [86] Subtract output 0 -> Lerp.A
- [87] Add output 0 -> Lerp.B
- [39] Mask ( R ) output 0 -> Lerp.Alpha
- [43] Input X range Y range (Vector4) output 0 -> Mask ( R ).Input
- [43] Input X range Y range (Vector4) output 0 -> Mask ( G ).Input
- [42] Lerp output 0 -> Output x.A
- [38] Append output 0 -> Multiply(,1.001).A
- [3] Input f(x) (Scalar) output 0 -> Subtract(1,).B
- [97] 1 output 0 -> Divide.A
- [48] 0.0001 output 0 -> Divide.B
- [7] Multiply output 0 -> Add(,1).A
- [52] Custom output 0 -> Multiply(,57.3).A
- [56] Subtract(90,) output 0 -> Divide(,57.3).A
- [53] Multiply(,57.3) output 0 -> Subtract(90,).B
- [45] Mask ( G ) output 0 -> Subtract.A
- [44] Mask ( R ) output 0 -> Subtract.B
- [41] Mask ( A ) output 0 -> Subtract.A
- [40] Mask ( B ) output 0 -> Subtract.B
- [57] Subtract output 0 -> Divide.A
- [58] Subtract output 0 -> Divide.B
- [67] Divide output 0 -> Divide.A
- [65] Clamp output 0 -> Divide.B
- [77] Subtract output 0 -> Divide(1,).B
- [42] Lerp output 0 -> Add.A
- [48] 0.0001 output 0 -> Add.B
- [55] Custom output 0 -> Clamp.Input
- [59] Divide output 0 -> Multiply.A
- [50] Divide output 0 -> Multiply.B
- [75] Divide(,200) output 0 -> Divide.A
- [61] Divide(1,) output 0 -> Divide.B
- [27] Distance output 0 -> Divide(,1.41).A
- [11] Multiply output 0 -> Lerp.A
- [3] Input f(x) (Scalar) output 0 -> Lerp.B
- [71] Clamp output 0 -> Lerp.Alpha
- [72] Ceil output 0 -> Clamp.Input
- [73] Subtract output 0 -> Ceil.Input
- [81] Multiply output 0 -> Subtract.A
- [74] Multiply(,1) output 0 -> Subtract.B
- [41] Mask ( A ) output 0 -> Multiply(,1).A
- [10] Input Thickness (Scalar) output 0 -> Divide(,200).A
- [79] Abs output 0 -> Multiply.A
- [58] Subtract output 0 -> Multiply.B
- [82] If output 0 -> Subtract.A
- [84] If output 0 -> Subtract.B
- [76] Multiply output 0 -> Add.A
- [41] Mask ( A ) output 0 -> Add.B
- [69] Input Gradient Height (Scalar) output 0 -> Abs.Input
- [80] Custom output 0 -> Multiply.A
- [1] Mask ( G ) output 0 -> Multiply.B
- [69] Input Gradient Height (Scalar) output 0 -> If.A
- [83] 0 output 0 -> If.B
- [78] Add output 0 -> If.AGreaterThanB
- [41] Mask ( A ) output 0 -> If.AEqualsB
- [41] Mask ( A ) output 0 -> If.ALessThanB
- [69] Input Gradient Height (Scalar) output 0 -> If.A
- [83] 0 output 0 -> If.B
- [40] Mask ( B ) output 0 -> If.AGreaterThanB
- [40] Mask ( B ) output 0 -> If.AEqualsB
- [85] Subtract output 0 -> If.ALessThanB
- [40] Mask ( B ) output 0 -> Subtract.A
- [76] Multiply output 0 -> Subtract.B
- [44] Mask ( R ) output 0 -> Subtract.A
- [88] Multiply(,0.5) output 0 -> Subtract.B
- [45] Mask ( G ) output 0 -> Add.A
- [88] Multiply(,0.5) output 0 -> Add.B
- [76] Multiply output 0 -> Multiply(,0.5).A
- [90] Multiply(,1) output 0 -> Subtract.A
- [44] Mask ( R ) output 0 -> Subtract.B
- [42] Lerp output 0 -> Multiply(,1).A
- [89] Subtract output 0 -> Ceil.Input
- [94] Add output 0 -> Ceil.Input
- [96] Multiply output 0 -> Clamp.Input
- [95] Multiply(,-1) output 0 -> Add.A
- [45] Mask ( G ) output 0 -> Add.B
- [90] Multiply(,1) output 0 -> Multiply(,-1).A
- [91] Ceil output 0 -> Multiply.A
- [92] Ceil output 0 -> Multiply.B
- [70] Lerp output 0 -> Add.A
- [32] Add output 0 -> Add.B
- [93] Clamp output 0 -> Multiply.A
- [98] Add output 0 -> Multiply.B

## Output Trace Roots

- `f(x) = y` is driven by [25] Switch
- `x` is driven by [42] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
