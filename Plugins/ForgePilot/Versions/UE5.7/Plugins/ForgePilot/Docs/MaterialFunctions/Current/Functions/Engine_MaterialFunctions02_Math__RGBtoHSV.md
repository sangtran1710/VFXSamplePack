# RGBtoHSV

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/RGBtoHSV.RGBtoHSV`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts RGB colors to HSV (hue saturation and value)

## Inputs

- `RGB` [FunctionInput_Vector3]

## Outputs

- `RGB to HSV (0-1)`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionMax` | Max |  |
| 1 | expression | `MaterialExpressionMax` | Max |  |
| 2 | expression | `MaterialExpressionMin` | Min |  |
| 3 | expression | `MaterialExpressionMin` | Min |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output RGB to HSV (0-1) | RGB to HSV (0-1) |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 7 | expression | `MaterialExpressionIf` | If |  |
| 8 | expression | `MaterialExpressionConstant` | 0 |  |
| 9 | expression | `MaterialExpressionIf` | If |  |
| 10 | expression | `MaterialExpressionConstant` | 0 |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionDivide` | Divide(,6) |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionDivide` | Divide(,2) |  |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 18 | expression | `MaterialExpressionIf` | If |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionIf` | If |  |
| 21 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 22 | expression | `MaterialExpressionConstant` | 0.3333 |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionConstant` | 0.6667 |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 27 | expression | `MaterialExpressionIf` | If |  |
| 28 | expression | `MaterialExpressionConstant` | 0 |  |
| 29 | expression | `MaterialExpressionConstant` | 1 |  |
| 30 | expression | `MaterialExpressionAdd` | Add |  |
| 31 | expression | `MaterialExpressionIf` | If |  |
| 32 | expression | `MaterialExpressionConstant` | 1 |  |
| 33 | expression | `MaterialExpressionConstant` | -1 |  |
| 34 | expression | `MaterialExpressionConstant` | 0 |  |
| 35 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 36 | function_input | `MaterialExpressionFunctionInput` | Input RGB (Vector3) | RGB |
| 37 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 38 | expression | `MaterialExpressionIf` | If |  |
| 39 | expression | `MaterialExpressionConstant` | 0 |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 42 | expression | `MaterialExpressionConstant3Vector` | 360,100,100 |  |
| 43 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 44 | expression | `MaterialExpressionIf` | If |  |
| 45 | expression | `MaterialExpressionConstant` | 0 |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply(,6) |  |
| 47 | expression | `MaterialExpressionIf` | If |  |
| 48 | expression | `MaterialExpressionConstant` | 6 |  |
| 49 | expression | `MaterialExpressionConstant` | 0 |  |
| 50 | expression | `MaterialExpressionFloor` | Floor |  |
| 51 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 54 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 55 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 56 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 57 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 58 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 59 | expression | `MaterialExpressionIf` | If |  |
| 60 | expression | `MaterialExpressionConstant` | 0 |  |
| 61 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 62 | expression | `MaterialExpressionIf` | If |  |
| 63 | expression | `MaterialExpressionConstant` | 1 |  |
| 64 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 65 | expression | `MaterialExpressionIf` | If |  |
| 66 | expression | `MaterialExpressionConstant` | 2 |  |
| 67 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 68 | expression | `MaterialExpressionIf` | If |  |
| 69 | expression | `MaterialExpressionConstant` | 3 |  |
| 70 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 71 | expression | `MaterialExpressionIf` | If |  |
| 72 | expression | `MaterialExpressionConstant` | 4 |  |
| 73 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 74 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 75 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 76 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 77 | expression | `MaterialExpressionMultiply` | Multiply(,255) |  |
| 78 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 79 | expression | `MaterialExpressionAdd` | Add |  |
| 80 | expression | `MaterialExpressionConstant2Vector` | 0,0.045 |  |
| 81 | expression | `MaterialExpressionAdd` | Add |  |
| 82 | expression | `MaterialExpressionAdd` | Add |  |
| 83 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 84 | expression | `MaterialExpressionAdd` | Add |  |
| 85 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 86 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |

## Exact Input Wiring

### [0] Max

- `A` <= [37] BreakOutFloat3Components output 0
- `B` <= [37] BreakOutFloat3Components output 1

### [1] Max

- `A` <= [0] Max output 0
- `B` <= [37] BreakOutFloat3Components output 2

### [2] Min

- `A` <= [37] BreakOutFloat3Components output 0
- `B` <= [37] BreakOutFloat3Components output 1

### [3] Min

- `A` <= [2] Min output 0
- `B` <= [37] BreakOutFloat3Components output 2

### [4] Subtract

- `A` <= [1] Max output 0
- `B` <= [3] Min output 0

### [5] Output RGB to HSV (0-1)

- `A` <= [6] MakeFloat3 output 0

### [7] If

- `A` <= [4] Subtract output 0
- `B` <= [8] 0 output 0
- `AGreaterThanB` <= [40] Add output 0
- `AEqualsB` <= [8] 0 output 0
- `ALessThanB` <= [40] Add output 0

### [9] If

- `A` <= [4] Subtract output 0
- `B` <= [10] 0 output 0
- `AGreaterThanB` <= [11] Divide output 0
- `AEqualsB` <= [10] 0 output 0
- `ALessThanB` <= [11] Divide output 0

### [11] Divide

- `A` <= [4] Subtract output 0
- `B` <= [1] Max output 0

### [12] Subtract

- `A` <= [1] Max output 0
- `B` <= [36] Input RGB (Vector3) output 0

### [13] Divide(,6)

- `A` <= [12] Subtract output 0

### [14] Add

- `A` <= [13] Divide(,6) output 0
- `B` <= [15] Divide(,2) output 0

### [15] Divide(,2)

- `A` <= [4] Subtract output 0

### [16] Divide

- `A` <= [14] Add output 0
- `B` <= [4] Subtract output 0

### [18] If

- `A` <= [1] Max output 0
- `B` <= [37] BreakOutFloat3Components output 0
- `AGreaterThanB` <= [20] If output 0
- `AEqualsB` <= [19] Subtract output 0
- `ALessThanB` <= [20] If output 0

### [19] Subtract

- `A` <= [17] BreakOutFloat3Components output 2
- `B` <= [17] BreakOutFloat3Components output 1

### [20] If

- `A` <= [37] BreakOutFloat3Components output 1
- `B` <= [1] Max output 0
- `AGreaterThanB` <= [38] If output 0
- `AEqualsB` <= [21] Subtract output 0
- `ALessThanB` <= [38] If output 0

### [21] Subtract

- `A` <= [23] Add output 0
- `B` <= [17] BreakOutFloat3Components output 2

### [23] Add

- `A` <= [22] 0.3333 output 0
- `B` <= [17] BreakOutFloat3Components output 0

### [25] Add

- `A` <= [24] 0.6667 output 0
- `B` <= [17] BreakOutFloat3Components output 1

### [26] Subtract

- `A` <= [25] Add output 0
- `B` <= [17] BreakOutFloat3Components output 0

### [27] If

- `A` <= [18] If output 0
- `B` <= [28] 0 output 0
- `AGreaterThanB` <= [28] 0 output 0
- `AEqualsB` <= [28] 0 output 0
- `ALessThanB` <= [29] 1 output 0

### [30] Add

- `A` <= [18] If output 0
- `B` <= [27] If output 0

### [31] If

- `A` <= [30] Add output 0
- `B` <= [32] 1 output 0
- `AGreaterThanB` <= [33] -1 output 0
- `AEqualsB` <= [34] 0 output 0
- `ALessThanB` <= [34] 0 output 0

### [38] If

- `A` <= [37] BreakOutFloat3Components output 2
- `B` <= [1] Max output 0
- `AGreaterThanB` <= [39] 0 output 0
- `AEqualsB` <= [26] Subtract output 0
- `ALessThanB` <= [39] 0 output 0

### [40] Add

- `A` <= [31] If output 0
- `B` <= [30] Add output 0

### [41] Multiply

- `A` <= [5] Output RGB to HSV (0-1) output 0
- `B` <= [42] 360,100,100 output 0

### [44] If

- `A` <= [43] BreakOutFloat3Components output 1
- `B` <= [45] 0 output 0
- `AGreaterThanB` <= [59] If output 0
- `AEqualsB` <= [86] 0,0,0 output 0
- `ALessThanB` <= [59] If output 0

### [46] Multiply(,6)

- `A` <= [43] BreakOutFloat3Components output 0

### [47] If

- `A` <= [46] Multiply(,6) output 0
- `B` <= [48] 6 output 0
- `AGreaterThanB` <= [46] Multiply(,6) output 0
- `AEqualsB` <= [49] 0 output 0
- `ALessThanB` <= [46] Multiply(,6) output 0

### [50] Floor

- `Input` <= [47] If output 0

### [51] 1-x

- `Input` <= [43] BreakOutFloat3Components output 1

### [52] Multiply

- `A` <= [43] BreakOutFloat3Components output 2
- `B` <= [51] 1-x output 0

### [53] Multiply

- `A` <= [54] Subtract output 0
- `B` <= [51] 1-x output 0

### [54] Subtract

- `A` <= [47] If output 0
- `B` <= [50] Floor output 0

### [55] Multiply

- `A` <= [51] 1-x output 0
- `B` <= [58] 1-x output 0

### [56] Multiply

- `A` <= [53] Multiply output 0
- `B` <= [43] BreakOutFloat3Components output 2

### [57] Multiply

- `A` <= [43] BreakOutFloat3Components output 2
- `B` <= [55] Multiply output 0

### [58] 1-x

- `Input` <= [54] Subtract output 0

### [59] If

- `A` <= [50] Floor output 0
- `B` <= [60] 0 output 0
- `AGreaterThanB` <= [62] If output 0
- `AEqualsB` <= [61] MakeFloat3 output 0
- `ALessThanB` <= [62] If output 0

### [62] If

- `A` <= [50] Floor output 0
- `B` <= [63] 1 output 0
- `AGreaterThanB` <= [65] If output 0
- `AEqualsB` <= [64] MakeFloat3 output 0
- `ALessThanB` <= [65] If output 0

### [65] If

- `A` <= [50] Floor output 0
- `B` <= [66] 2 output 0
- `AGreaterThanB` <= [68] If output 0
- `AEqualsB` <= [67] MakeFloat3 output 0
- `ALessThanB` <= [68] If output 0

### [68] If

- `A` <= [50] Floor output 0
- `B` <= [69] 3 output 0
- `AGreaterThanB` <= [71] If output 0
- `AEqualsB` <= [70] MakeFloat3 output 0
- `ALessThanB` <= [71] If output 0

### [71] If

- `A` <= [50] Floor output 0
- `B` <= [72] 4 output 0
- `AGreaterThanB` <= [74] MakeFloat3 output 0
- `AEqualsB` <= [73] MakeFloat3 output 0
- `ALessThanB` <= [74] MakeFloat3 output 0

### [77] Multiply(,255)

- `A` <= [36] Input RGB (Vector3) output 0

### [79] Add

- `A` <= [78] TexCoord[0] output 0
- `B` <= [80] 0,0.045 output 0

### [81] Add

- `A` <= [35] DebugFloat3Values output 0
- `B` <= [76] DebugFloat3Values output 0

### [82] Add

- `A` <= [81] Add output 0
- `B` <= [83] DebugFloat3Values output 0

### [84] Add

- `A` <= [78] TexCoord[0] output 0
- `B` <= [85] Multiply(,2) output 0

### [85] Multiply(,2)

- `A` <= [80] 0,0.045 output 0

## Connection List

- [37] BreakOutFloat3Components output 0 -> Max.A
- [37] BreakOutFloat3Components output 1 -> Max.B
- [0] Max output 0 -> Max.A
- [37] BreakOutFloat3Components output 2 -> Max.B
- [37] BreakOutFloat3Components output 0 -> Min.A
- [37] BreakOutFloat3Components output 1 -> Min.B
- [2] Min output 0 -> Min.A
- [37] BreakOutFloat3Components output 2 -> Min.B
- [1] Max output 0 -> Subtract.A
- [3] Min output 0 -> Subtract.B
- [6] MakeFloat3 output 0 -> Output RGB to HSV (0-1).A
- [4] Subtract output 0 -> If.A
- [8] 0 output 0 -> If.B
- [40] Add output 0 -> If.AGreaterThanB
- [8] 0 output 0 -> If.AEqualsB
- [40] Add output 0 -> If.ALessThanB
- [4] Subtract output 0 -> If.A
- [10] 0 output 0 -> If.B
- [11] Divide output 0 -> If.AGreaterThanB
- [10] 0 output 0 -> If.AEqualsB
- [11] Divide output 0 -> If.ALessThanB
- [4] Subtract output 0 -> Divide.A
- [1] Max output 0 -> Divide.B
- [1] Max output 0 -> Subtract.A
- [36] Input RGB (Vector3) output 0 -> Subtract.B
- [12] Subtract output 0 -> Divide(,6).A
- [13] Divide(,6) output 0 -> Add.A
- [15] Divide(,2) output 0 -> Add.B
- [4] Subtract output 0 -> Divide(,2).A
- [14] Add output 0 -> Divide.A
- [4] Subtract output 0 -> Divide.B
- [1] Max output 0 -> If.A
- [37] BreakOutFloat3Components output 0 -> If.B
- [20] If output 0 -> If.AGreaterThanB
- [19] Subtract output 0 -> If.AEqualsB
- [20] If output 0 -> If.ALessThanB
- [17] BreakOutFloat3Components output 2 -> Subtract.A
- [17] BreakOutFloat3Components output 1 -> Subtract.B
- [37] BreakOutFloat3Components output 1 -> If.A
- [1] Max output 0 -> If.B
- [38] If output 0 -> If.AGreaterThanB
- [21] Subtract output 0 -> If.AEqualsB
- [38] If output 0 -> If.ALessThanB
- [23] Add output 0 -> Subtract.A
- [17] BreakOutFloat3Components output 2 -> Subtract.B
- [22] 0.3333 output 0 -> Add.A
- [17] BreakOutFloat3Components output 0 -> Add.B
- [24] 0.6667 output 0 -> Add.A
- [17] BreakOutFloat3Components output 1 -> Add.B
- [25] Add output 0 -> Subtract.A
- [17] BreakOutFloat3Components output 0 -> Subtract.B
- [18] If output 0 -> If.A
- [28] 0 output 0 -> If.B
- [28] 0 output 0 -> If.AGreaterThanB
- [28] 0 output 0 -> If.AEqualsB
- [29] 1 output 0 -> If.ALessThanB
- [18] If output 0 -> Add.A
- [27] If output 0 -> Add.B
- [30] Add output 0 -> If.A
- [32] 1 output 0 -> If.B
- [33] -1 output 0 -> If.AGreaterThanB
- [34] 0 output 0 -> If.AEqualsB
- [34] 0 output 0 -> If.ALessThanB
- [37] BreakOutFloat3Components output 2 -> If.A
- [1] Max output 0 -> If.B
- [39] 0 output 0 -> If.AGreaterThanB
- [26] Subtract output 0 -> If.AEqualsB
- [39] 0 output 0 -> If.ALessThanB
- [31] If output 0 -> Add.A
- [30] Add output 0 -> Add.B
- [5] Output RGB to HSV (0-1) output 0 -> Multiply.A
- [42] 360,100,100 output 0 -> Multiply.B
- [43] BreakOutFloat3Components output 1 -> If.A
- [45] 0 output 0 -> If.B
- [59] If output 0 -> If.AGreaterThanB
- [86] 0,0,0 output 0 -> If.AEqualsB
- [59] If output 0 -> If.ALessThanB
- [43] BreakOutFloat3Components output 0 -> Multiply(,6).A
- [46] Multiply(,6) output 0 -> If.A
- [48] 6 output 0 -> If.B
- [46] Multiply(,6) output 0 -> If.AGreaterThanB
- [49] 0 output 0 -> If.AEqualsB
- [46] Multiply(,6) output 0 -> If.ALessThanB
- [47] If output 0 -> Floor.Input
- [43] BreakOutFloat3Components output 1 -> 1-x.Input
- [43] BreakOutFloat3Components output 2 -> Multiply.A
- [51] 1-x output 0 -> Multiply.B
- [54] Subtract output 0 -> Multiply.A
- [51] 1-x output 0 -> Multiply.B
- [47] If output 0 -> Subtract.A
- [50] Floor output 0 -> Subtract.B
- [51] 1-x output 0 -> Multiply.A
- [58] 1-x output 0 -> Multiply.B
- [53] Multiply output 0 -> Multiply.A
- [43] BreakOutFloat3Components output 2 -> Multiply.B
- [43] BreakOutFloat3Components output 2 -> Multiply.A
- [55] Multiply output 0 -> Multiply.B
- [54] Subtract output 0 -> 1-x.Input
- [50] Floor output 0 -> If.A
- [60] 0 output 0 -> If.B
- [62] If output 0 -> If.AGreaterThanB
- [61] MakeFloat3 output 0 -> If.AEqualsB
- [62] If output 0 -> If.ALessThanB
- [50] Floor output 0 -> If.A
- [63] 1 output 0 -> If.B
- [65] If output 0 -> If.AGreaterThanB
- [64] MakeFloat3 output 0 -> If.AEqualsB
- [65] If output 0 -> If.ALessThanB
- [50] Floor output 0 -> If.A
- [66] 2 output 0 -> If.B
- [68] If output 0 -> If.AGreaterThanB
- [67] MakeFloat3 output 0 -> If.AEqualsB
- [68] If output 0 -> If.ALessThanB
- [50] Floor output 0 -> If.A
- [69] 3 output 0 -> If.B
- [71] If output 0 -> If.AGreaterThanB
- [70] MakeFloat3 output 0 -> If.AEqualsB
- [71] If output 0 -> If.ALessThanB
- [50] Floor output 0 -> If.A
- [72] 4 output 0 -> If.B
- [74] MakeFloat3 output 0 -> If.AGreaterThanB
- [73] MakeFloat3 output 0 -> If.AEqualsB
- [74] MakeFloat3 output 0 -> If.ALessThanB
- [36] Input RGB (Vector3) output 0 -> Multiply(,255).A
- [78] TexCoord[0] output 0 -> Add.A
- [80] 0,0.045 output 0 -> Add.B
- [35] DebugFloat3Values output 0 -> Add.A
- [76] DebugFloat3Values output 0 -> Add.B
- [81] Add output 0 -> Add.A
- [83] DebugFloat3Values output 0 -> Add.B
- [78] TexCoord[0] output 0 -> Add.A
- [85] Multiply(,2) output 0 -> Add.B
- [80] 0,0.045 output 0 -> Multiply(,2).A

## Output Trace Roots

- `RGB to HSV (0-1)` is driven by [6] MakeFloat3

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


