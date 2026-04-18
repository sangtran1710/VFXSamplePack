# PlotFunctionOnGraph_Setup_Input

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/PlotFunctionOnGraph_Setup_Input.PlotFunctionOnGraph_Setup_Input`
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
- `UVs` [FunctionInput_Vector2]
- `Corrected Y-size` [FunctionInput_Scalar]

## Outputs

- `f(x) = y`

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
| 46 | expression | `MaterialExpressionMultiply` | Multiply(,1.001) |  |
| 47 | expression | `MaterialExpressionSubtract` | Subtract(1,) |  |
| 48 | expression | `MaterialExpressionDivide` | Divide(,1) |  |
| 49 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 50 | custom | `MaterialExpressionCustom` | Custom |  |
| 51 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |
| 52 | expression | `MaterialExpressionDivide` | Divide(,57.3) |  |
| 53 | custom | `MaterialExpressionCustom` | Custom |  |
| 54 | expression | `MaterialExpressionSubtract` | Subtract(90,) |  |
| 55 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 56 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 57 | expression | `MaterialExpressionDivide` | Divide |  |
| 58 | expression | `MaterialExpressionDivide` | Divide |  |
| 59 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 60 | expression | `MaterialExpressionMultiply` | Multiply(0,1) |  |
| 61 | custom | `MaterialExpressionCustom` | Custom |  |
| 62 | expression | `MaterialExpressionClamp` | Clamp |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 64 | expression | `MaterialExpressionDivide` | Divide |  |
| 65 | expression | `MaterialExpressionDivide` | Divide(,1.41) |  |
| 66 | function_input | `MaterialExpressionFunctionInput` | Input Gradient Height (Scalar) | Gradient Height |
| 67 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 68 | expression | `MaterialExpressionClamp` | Clamp |  |
| 69 | expression | `MaterialExpressionCeil` | Ceil |  |
| 70 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 72 | expression | `MaterialExpressionDivide` | Divide(,200) |  |
| 73 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 74 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 75 | expression | `MaterialExpressionAdd` | Add |  |
| 76 | expression | `MaterialExpressionAbs` | Abs |  |
| 77 | custom | `MaterialExpressionCustom` | Custom |  |
| 78 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 79 | expression | `MaterialExpressionIf` | If |  |
| 80 | expression | `MaterialExpressionConstant` | 0 |  |
| 81 | expression | `MaterialExpressionIf` | If |  |
| 82 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 83 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 84 | expression | `MaterialExpressionAdd` | Add |  |
| 85 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 86 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 87 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 88 | expression | `MaterialExpressionCeil` | Ceil |  |
| 89 | expression | `MaterialExpressionCeil` | Ceil |  |
| 90 | expression | `MaterialExpressionClamp` | Clamp |  |
| 91 | expression | `MaterialExpressionAdd` | Add |  |
| 92 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 93 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 94 | expression | `MaterialExpressionConstant` | 1 |  |
| 95 | expression | `MaterialExpressionAdd` | Add |  |
| 96 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 97 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 98 | function_input | `MaterialExpressionFunctionInput` | Input Corrected Y-size (Scalar) | Corrected Y-size |

## Exact Input Wiring

### [0] Output f(x) = y

- `A` <= [25] Switch output 0

### [1] Mask ( G )

- `Input` <= [97] Input UVs (Vector2) output 0

### [2] Distance

- `A` <= [1] Mask ( G ) output 0
- `B` <= [3] Input f(x) (Scalar) output 0

### [4] 1-x

- `Input` <= [2] Distance output 0

### [5] Add

- `A` <= [4] 1-x output 0
- `B` <= [64] Divide output 0

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

- `Coordinates` <= [46] Multiply(,1.001) output 0

### [23] Input ShowGrid? (StaticBool)

- `Preview` <= [24] Static Bool (True) output 0

### [25] Switch

- `A` <= [96] Multiply output 0
- `B` <= [32] Add output 0
- `Value` <= [23] Input ShowGrid? (StaticBool) output 0

### [26] Mask ( R )

- `Input` <= [97] Input UVs (Vector2) output 0

### [27] Distance

- `A` <= [26] Mask ( R ) output 0
- `B` <= [1] Mask ( G ) output 0

### [28] 1-x

- `Input` <= [65] Divide(,1.41) output 0

### [29] Add

- `A` <= [28] 1-x output 0
- `B` <= [64] Divide output 0

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

- `A` <= [79] If output 0
- `B` <= [81] If output 0
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

- `A` <= [83] Subtract output 0
- `B` <= [84] Add output 0
- `Alpha` <= [39] Mask ( R ) output 0

### [44] Mask ( R )

- `Input` <= [43] Input X range Y range (Vector4) output 0

### [45] Mask ( G )

- `Input` <= [43] Input X range Y range (Vector4) output 0

### [46] Multiply(,1.001)

- `A` <= [97] Input UVs (Vector2) output 0

### [47] Subtract(1,)

- `B` <= [3] Input f(x) (Scalar) output 0

### [48] Divide(,1)

- `A` <= [94] 1 output 0

### [49] Add(,1)

- `A` <= [7] Multiply output 0

### [51] Multiply(,57.3)

- `A` <= [50] Custom output 0

### [52] Divide(,57.3)

- `A` <= [54] Subtract(90,) output 0

### [54] Subtract(90,)

- `B` <= [51] Multiply(,57.3) output 0

### [55] Subtract

- `A` <= [45] Mask ( G ) output 0
- `B` <= [44] Mask ( R ) output 0

### [56] Subtract

- `A` <= [41] Mask ( A ) output 0
- `B` <= [40] Mask ( B ) output 0

### [57] Divide

- `A` <= [55] Subtract output 0
- `B` <= [56] Subtract output 0

### [58] Divide

- `A` <= [64] Divide output 0
- `B` <= [62] Clamp output 0

### [59] Divide(1,)

- `B` <= [98] Input Corrected Y-size (Scalar) output 0

### [62] Clamp

- `Input` <= [53] Custom output 0

### [63] Multiply

- `A` <= [57] Divide output 0
- `B` <= [48] Divide(,1) output 0

### [64] Divide

- `A` <= [72] Divide(,200) output 0
- `B` <= [59] Divide(1,) output 0

### [65] Divide(,1.41)

- `A` <= [27] Distance output 0

### [67] Lerp

- `A` <= [11] Multiply output 0
- `B` <= [3] Input f(x) (Scalar) output 0
- `Alpha` <= [68] Clamp output 0

### [68] Clamp

- `Input` <= [69] Ceil output 0

### [69] Ceil

- `Input` <= [70] Subtract output 0

### [70] Subtract

- `A` <= [78] Multiply output 0
- `B` <= [71] Multiply(,1) output 0

### [71] Multiply(,1)

- `A` <= [41] Mask ( A ) output 0

### [72] Divide(,200)

- `A` <= [10] Input Thickness (Scalar) output 0

### [73] Multiply

- `A` <= [76] Abs output 0
- `B` <= [56] Subtract output 0

### [74] Subtract

- `A` <= [79] If output 0
- `B` <= [81] If output 0

### [75] Add

- `A` <= [73] Multiply output 0
- `B` <= [41] Mask ( A ) output 0

### [76] Abs

- `Input` <= [66] Input Gradient Height (Scalar) output 0

### [78] Multiply

- `A` <= [77] Custom output 0
- `B` <= [1] Mask ( G ) output 0

### [79] If

- `A` <= [66] Input Gradient Height (Scalar) output 0
- `B` <= [80] 0 output 0
- `AGreaterThanB` <= [75] Add output 0
- `AEqualsB` <= [41] Mask ( A ) output 0
- `ALessThanB` <= [41] Mask ( A ) output 0

### [81] If

- `A` <= [66] Input Gradient Height (Scalar) output 0
- `B` <= [80] 0 output 0
- `AGreaterThanB` <= [40] Mask ( B ) output 0
- `AEqualsB` <= [40] Mask ( B ) output 0
- `ALessThanB` <= [82] Subtract output 0

### [82] Subtract

- `A` <= [40] Mask ( B ) output 0
- `B` <= [73] Multiply output 0

### [83] Subtract

- `A` <= [44] Mask ( R ) output 0
- `B` <= [85] Multiply(,0.5) output 0

### [84] Add

- `A` <= [45] Mask ( G ) output 0
- `B` <= [85] Multiply(,0.5) output 0

### [85] Multiply(,0.5)

- `A` <= [73] Multiply output 0

### [86] Subtract

- `A` <= [87] Multiply(,1) output 0
- `B` <= [44] Mask ( R ) output 0

### [87] Multiply(,1)

- `A` <= [26] Mask ( R ) output 0

### [88] Ceil

- `Input` <= [86] Subtract output 0

### [89] Ceil

- `Input` <= [91] Add output 0

### [90] Clamp

- `Input` <= [93] Multiply output 0

### [91] Add

- `A` <= [92] Multiply(,-1) output 0
- `B` <= [45] Mask ( G ) output 0

### [92] Multiply(,-1)

- `A` <= [87] Multiply(,1) output 0

### [93] Multiply

- `A` <= [88] Ceil output 0
- `B` <= [89] Ceil output 0

### [95] Add

- `A` <= [67] Lerp output 0
- `B` <= [32] Add output 0

### [96] Multiply

- `A` <= [90] Clamp output 0
- `B` <= [95] Add output 0

### [97] Input UVs (Vector2)

- `Preview` <= [38] Append output 0

### [98] Input Corrected Y-size (Scalar)

- `Preview` <= [74] Subtract output 0

## Connection List

- [25] Switch output 0 -> Output f(x) = y.A
- [97] Input UVs (Vector2) output 0 -> Mask ( G ).Input
- [1] Mask ( G ) output 0 -> Distance.A
- [3] Input f(x) (Scalar) output 0 -> Distance.B
- [2] Distance output 0 -> 1-x.Input
- [4] 1-x output 0 -> Add.A
- [64] Divide output 0 -> Add.B
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
- [46] Multiply(,1.001) output 0 -> Texture Sample.Coordinates
- [24] Static Bool (True) output 0 -> Input ShowGrid? (StaticBool).Preview
- [96] Multiply output 0 -> Switch.A
- [32] Add output 0 -> Switch.B
- [23] Input ShowGrid? (StaticBool) output 0 -> Switch.Value
- [97] Input UVs (Vector2) output 0 -> Mask ( R ).Input
- [26] Mask ( R ) output 0 -> Distance.A
- [1] Mask ( G ) output 0 -> Distance.B
- [65] Divide(,1.41) output 0 -> 1-x.Input
- [28] 1-x output 0 -> Add.A
- [64] Divide output 0 -> Add.B
- [29] Add output 0 -> Floor.Input
- [33] Clamp output 0 -> Multiply(,0.1).A
- [31] Multiply(,0.1) output 0 -> Add.A
- [7] Multiply output 0 -> Add.B
- [30] Floor output 0 -> Clamp.Input
- [6] Floor output 0 -> Clamp.Input
- [79] If output 0 -> Lerp.A
- [81] If output 0 -> Lerp.B
- [37] Mask ( G ) output 0 -> Lerp.Alpha
- [35] TexCoord[0] output 0 -> Mask ( G ).Input
- [42] Lerp output 0 -> Append.A
- [36] Lerp output 0 -> Append.B
- [35] TexCoord[0] output 0 -> Mask ( R ).Input
- [43] Input X range Y range (Vector4) output 0 -> Mask ( B ).Input
- [43] Input X range Y range (Vector4) output 0 -> Mask ( A ).Input
- [83] Subtract output 0 -> Lerp.A
- [84] Add output 0 -> Lerp.B
- [39] Mask ( R ) output 0 -> Lerp.Alpha
- [43] Input X range Y range (Vector4) output 0 -> Mask ( R ).Input
- [43] Input X range Y range (Vector4) output 0 -> Mask ( G ).Input
- [97] Input UVs (Vector2) output 0 -> Multiply(,1.001).A
- [3] Input f(x) (Scalar) output 0 -> Subtract(1,).B
- [94] 1 output 0 -> Divide(,1).A
- [7] Multiply output 0 -> Add(,1).A
- [50] Custom output 0 -> Multiply(,57.3).A
- [54] Subtract(90,) output 0 -> Divide(,57.3).A
- [51] Multiply(,57.3) output 0 -> Subtract(90,).B
- [45] Mask ( G ) output 0 -> Subtract.A
- [44] Mask ( R ) output 0 -> Subtract.B
- [41] Mask ( A ) output 0 -> Subtract.A
- [40] Mask ( B ) output 0 -> Subtract.B
- [55] Subtract output 0 -> Divide.A
- [56] Subtract output 0 -> Divide.B
- [64] Divide output 0 -> Divide.A
- [62] Clamp output 0 -> Divide.B
- [98] Input Corrected Y-size (Scalar) output 0 -> Divide(1,).B
- [53] Custom output 0 -> Clamp.Input
- [57] Divide output 0 -> Multiply.A
- [48] Divide(,1) output 0 -> Multiply.B
- [72] Divide(,200) output 0 -> Divide.A
- [59] Divide(1,) output 0 -> Divide.B
- [27] Distance output 0 -> Divide(,1.41).A
- [11] Multiply output 0 -> Lerp.A
- [3] Input f(x) (Scalar) output 0 -> Lerp.B
- [68] Clamp output 0 -> Lerp.Alpha
- [69] Ceil output 0 -> Clamp.Input
- [70] Subtract output 0 -> Ceil.Input
- [78] Multiply output 0 -> Subtract.A
- [71] Multiply(,1) output 0 -> Subtract.B
- [41] Mask ( A ) output 0 -> Multiply(,1).A
- [10] Input Thickness (Scalar) output 0 -> Divide(,200).A
- [76] Abs output 0 -> Multiply.A
- [56] Subtract output 0 -> Multiply.B
- [79] If output 0 -> Subtract.A
- [81] If output 0 -> Subtract.B
- [73] Multiply output 0 -> Add.A
- [41] Mask ( A ) output 0 -> Add.B
- [66] Input Gradient Height (Scalar) output 0 -> Abs.Input
- [77] Custom output 0 -> Multiply.A
- [1] Mask ( G ) output 0 -> Multiply.B
- [66] Input Gradient Height (Scalar) output 0 -> If.A
- [80] 0 output 0 -> If.B
- [75] Add output 0 -> If.AGreaterThanB
- [41] Mask ( A ) output 0 -> If.AEqualsB
- [41] Mask ( A ) output 0 -> If.ALessThanB
- [66] Input Gradient Height (Scalar) output 0 -> If.A
- [80] 0 output 0 -> If.B
- [40] Mask ( B ) output 0 -> If.AGreaterThanB
- [40] Mask ( B ) output 0 -> If.AEqualsB
- [82] Subtract output 0 -> If.ALessThanB
- [40] Mask ( B ) output 0 -> Subtract.A
- [73] Multiply output 0 -> Subtract.B
- [44] Mask ( R ) output 0 -> Subtract.A
- [85] Multiply(,0.5) output 0 -> Subtract.B
- [45] Mask ( G ) output 0 -> Add.A
- [85] Multiply(,0.5) output 0 -> Add.B
- [73] Multiply output 0 -> Multiply(,0.5).A
- [87] Multiply(,1) output 0 -> Subtract.A
- [44] Mask ( R ) output 0 -> Subtract.B
- [26] Mask ( R ) output 0 -> Multiply(,1).A
- [86] Subtract output 0 -> Ceil.Input
- [91] Add output 0 -> Ceil.Input
- [93] Multiply output 0 -> Clamp.Input
- [92] Multiply(,-1) output 0 -> Add.A
- [45] Mask ( G ) output 0 -> Add.B
- [87] Multiply(,1) output 0 -> Multiply(,-1).A
- [88] Ceil output 0 -> Multiply.A
- [89] Ceil output 0 -> Multiply.B
- [67] Lerp output 0 -> Add.A
- [32] Add output 0 -> Add.B
- [90] Clamp output 0 -> Multiply.A
- [95] Add output 0 -> Multiply.B
- [38] Append output 0 -> Input UVs (Vector2).Preview
- [74] Subtract output 0 -> Input Corrected Y-size (Scalar).Preview

## Output Trace Roots

- `f(x) = y` is driven by [25] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
