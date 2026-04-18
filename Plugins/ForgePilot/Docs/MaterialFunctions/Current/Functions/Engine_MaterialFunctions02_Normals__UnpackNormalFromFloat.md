# UnpackNormalFromFloat

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Normals/UnpackNormalFromFloat.UnpackNormalFromFloat`
- Category: Engine_MaterialFunctions02/Normals
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Unpack Normal/Float3 values from a scalar value

## Inputs

- `Scalar Value` [FunctionInput_Scalar] - To be used with processed normal maps. Example packed and unpacked values stored within the function.

## Outputs

- `XXY.YZZ Vector`
- `XXY.YZZ Normalized Vector`
- `( z sign ) XXX.YYY Normal`
- `( z sign ) XXYY.0`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output XXY.YZZ Vector | XXY.YZZ Vector |
| 1 | expression | `MaterialExpressionConstant` | 221.7 |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | -0.563,-0.67,-0.484 |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 4 | expression | `MaterialExpressionFloor` | Floor |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply(,10) |  |
| 9 | expression | `MaterialExpressionFloor` | Floor |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,0.1) |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply(,0.001) |  |
| 13 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output XXY.YZZ Normalized Vector | XXY.YZZ Normalized Vector |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Scalar Value (Scalar) | Scalar Value |
| 16 | expression | `MaterialExpressionAppendVector` | Append |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat4Values |  |
| 18 | expression | `MaterialExpressionConstant` | -205.2 |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply(,0.001) |  |
| 20 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionIf` | If |  |
| 24 | expression | `MaterialExpressionConstant` | 0 |  |
| 25 | expression | `MaterialExpressionConstant` | 1 |  |
| 26 | expression | `MaterialExpressionConstant` | -1 |  |
| 27 | expression | `MaterialExpressionConstant3Vector` | -0.591,-0.545,-0.594 |  |
| 28 | expression | `MaterialExpressionCeil` | Ceil |  |
| 29 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 31 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 32 | expression | `MaterialExpressionAppendVector` | Append |  |
| 33 | expression | `MaterialExpressionAbs` | Abs |  |
| 34 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 35 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 36 | expression | `MaterialExpressionConstant` | -205.2 |  |
| 37 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 38 | expression | `MaterialExpressionConstant3Vector` | 0.0016,0.0013,0.0028 |  |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output ( z sign ) XXX.YYY Normal | ( z sign ) XXX.YYY Normal |
| 40 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 41 | expression | `MaterialExpressionConstant3Vector` | 0.0024,0.0049,0.004 |  |
| 42 | expression | `MaterialExpressionConstant3Vector` | 0.0016,0.0013,0.0028 |  |
| 43 | expression | `MaterialExpressionConstant3Vector` | 0.0024,0.0049,0.004 |  |
| 44 | expression | `MaterialExpressionConstant3Vector` | 0.0017,0.0452,-0.0401 |  |
| 45 | expression | `MaterialExpressionConstant3Vector` | 0.0064,0.0201,0.484 |  |
| 46 | expression | `MaterialExpressionConstant4Vector` | 1,-1,0.01,0 |  |
| 47 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 48 | expression | `MaterialExpressionAbs` | Abs |  |
| 49 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 50 | expression | `MaterialExpressionIf` | If |  |
| 51 | expression | `MaterialExpressionConstant` | 0 |  |
| 52 | expression | `MaterialExpressionConstant` | 1 |  |
| 53 | expression | `MaterialExpressionConstant` | -1 |  |
| 54 | expression | `MaterialExpressionAbs` | Abs |  |
| 55 | function_call | `MaterialExpressionMaterialFunctionCall` | PassThrough |  |
| 56 | expression | `MaterialExpressionFloor` | Floor |  |
| 57 | expression | `MaterialExpressionDivide` | Divide(,100) |  |
| 58 | expression | `MaterialExpressionDivide` | Divide(,100) |  |
| 59 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 60 | expression | `MaterialExpressionAdd` | Add |  |
| 61 | expression | `MaterialExpressionFloor` | Floor |  |
| 62 | expression | `MaterialExpressionDivide` | Divide(,100) |  |
| 63 | expression | `MaterialExpressionFloor` | Floor |  |
| 64 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 65 | expression | `MaterialExpressionAppendVector` | Append |  |
| 66 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 67 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 68 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 69 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 70 | expression | `MaterialExpressionAppendVector` | Append |  |
| 71 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 72 | function_output | `MaterialExpressionFunctionOutput` | Output ( z sign ) XXYY.0 | ( z sign ) XXYY.0 |
| 73 | expression | `MaterialExpressionConstant` | 6714 |  |
| 74 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat3Values |  |
| 75 | expression | `MaterialExpressionConstant3Vector` | 0.34,-0.72,0.605 |  |
| 76 | expression | `MaterialExpressionConstant3Vector` | 0.326,-0.726,0.606 |  |
| 77 | expression | `MaterialExpressionConstant3Vector` | -0.0139,-0.00587,0.000718 |  |

## Exact Input Wiring

### [0] Output XXY.YZZ Vector

- `A` <= [7] Subtract(,1) output 0

### [4] Floor

- `Input` <= [11] Multiply(,0.1) output 0

### [5] Subtract

- `A` <= [11] Multiply(,0.1) output 0
- `B` <= [4] Floor output 0

### [6] Multiply(,2)

- `A` <= [3] MakeFloat3 output 0

### [7] Subtract(,1)

- `A` <= [6] Multiply(,2) output 0

### [8] Multiply(,10)

- `A` <= [15] Input Scalar Value (Scalar) output 0

### [9] Floor

- `Input` <= [8] Multiply(,10) output 0

### [10] Subtract

- `A` <= [8] Multiply(,10) output 0
- `B` <= [9] Floor output 0

### [11] Multiply(,0.1)

- `A` <= [15] Input Scalar Value (Scalar) output 0

### [12] Multiply(,0.001)

- `A` <= [15] Input Scalar Value (Scalar) output 0

### [13] Normalize

- `VectorInput` <= [7] Subtract(,1) output 0

### [14] Output XXY.YZZ Normalized Vector

- `A` <= [13] Normalize output 0

### [16] Append

- `A` <= [40] Subtract output 0
- `B` <= [15] Input Scalar Value (Scalar) output 0

### [19] Multiply(,0.001)

- `A` <= [48] Abs output 0

### [20] DeriveNormalZ

- `InXY` <= [31] Subtract(,1) output 0

### [21] Multiply

- `A` <= [20] DeriveNormalZ output 0
- `B` <= [22] Append output 0

### [22] Append

- `A` <= [35] 1,1 output 0
- `B` <= [23] If output 0

### [23] If

- `A` <= [15] Input Scalar Value (Scalar) output 0
- `B` <= [24] 0 output 0
- `AGreaterThanB` <= [25] 1 output 0
- `AEqualsB` <= [25] 1 output 0
- `ALessThanB` <= [26] -1 output 0

### [28] Ceil

- `Input` <= [48] Abs output 0

### [29] Subtract

- `A` <= [28] Ceil output 0
- `B` <= [48] Abs output 0

### [30] Multiply(,2)

- `A` <= [33] Abs output 0

### [31] Subtract(,1)

- `A` <= [30] Multiply(,2) output 0

### [32] Append

- `A` <= [19] Multiply(,0.001) output 0
- `B` <= [29] Subtract output 0

### [33] Abs

- `Input` <= [32] Append output 0

### [37] Subtract

- `A` <= [21] Multiply output 0
- `B` <= [27] -0.591,-0.545,-0.594 output 0

### [39] Output ( z sign ) XXX.YYY Normal

- `A` <= [49] SafeNormalize output 0

### [40] Subtract

- `A` <= [7] Subtract(,1) output 0
- `B` <= [2] -0.563,-0.67,-0.484 output 0

### [48] Abs

- `Input` <= [15] Input Scalar Value (Scalar) output 0

### [50] If

- `A` <= [55] PassThrough output 0
- `B` <= [51] 0 output 0
- `AGreaterThanB` <= [52] 1 output 0
- `ALessThanB` <= [53] -1 output 0

### [54] Abs

- `Input` <= [55] PassThrough output 0

### [56] Floor

- `Input` <= [57] Divide(,100) output 0

### [57] Divide(,100)

- `A` <= [54] Abs output 0

### [58] Divide(,100)

- `A` <= [56] Floor output 0

### [59] Subtract(,0.5)

- `A` <= [65] Append output 0

### [60] Add

- `A` <= [59] Subtract(,0.5) output 0
- `B` <= [59] Subtract(,0.5) output 0

### [61] Floor

- `Input` <= [54] Abs output 0

### [62] Divide(,100)

- `A` <= [61] Floor output 0

### [63] Floor

- `Input` <= [62] Divide(,100) output 0

### [64] Subtract

- `A` <= [62] Divide(,100) output 0
- `B` <= [63] Floor output 0

### [65] Append

- `A` <= [58] Divide(,100) output 0
- `B` <= [64] Subtract output 0

### [66] DeriveNormalZ

- `InXY` <= [60] Add output 0

### [69] Multiply

- `A` <= [66] DeriveNormalZ output 0
- `B` <= [50] If output 0

### [70] Append

- `A` <= [60] Add output 0
- `B` <= [71] Mask ( B ) output 0

### [71] Mask ( B )

- `Input` <= [69] Multiply output 0

### [72] Output ( z sign ) XXYY.0

- `A` <= [67] SafeNormalize output 0

## Connection List

- [7] Subtract(,1) output 0 -> Output XXY.YZZ Vector.A
- [11] Multiply(,0.1) output 0 -> Floor.Input
- [11] Multiply(,0.1) output 0 -> Subtract.A
- [4] Floor output 0 -> Subtract.B
- [3] MakeFloat3 output 0 -> Multiply(,2).A
- [6] Multiply(,2) output 0 -> Subtract(,1).A
- [15] Input Scalar Value (Scalar) output 0 -> Multiply(,10).A
- [8] Multiply(,10) output 0 -> Floor.Input
- [8] Multiply(,10) output 0 -> Subtract.A
- [9] Floor output 0 -> Subtract.B
- [15] Input Scalar Value (Scalar) output 0 -> Multiply(,0.1).A
- [15] Input Scalar Value (Scalar) output 0 -> Multiply(,0.001).A
- [7] Subtract(,1) output 0 -> Normalize.VectorInput
- [13] Normalize output 0 -> Output XXY.YZZ Normalized Vector.A
- [40] Subtract output 0 -> Append.A
- [15] Input Scalar Value (Scalar) output 0 -> Append.B
- [48] Abs output 0 -> Multiply(,0.001).A
- [31] Subtract(,1) output 0 -> DeriveNormalZ.InXY
- [20] DeriveNormalZ output 0 -> Multiply.A
- [22] Append output 0 -> Multiply.B
- [35] 1,1 output 0 -> Append.A
- [23] If output 0 -> Append.B
- [15] Input Scalar Value (Scalar) output 0 -> If.A
- [24] 0 output 0 -> If.B
- [25] 1 output 0 -> If.AGreaterThanB
- [25] 1 output 0 -> If.AEqualsB
- [26] -1 output 0 -> If.ALessThanB
- [48] Abs output 0 -> Ceil.Input
- [28] Ceil output 0 -> Subtract.A
- [48] Abs output 0 -> Subtract.B
- [33] Abs output 0 -> Multiply(,2).A
- [30] Multiply(,2) output 0 -> Subtract(,1).A
- [19] Multiply(,0.001) output 0 -> Append.A
- [29] Subtract output 0 -> Append.B
- [32] Append output 0 -> Abs.Input
- [21] Multiply output 0 -> Subtract.A
- [27] -0.591,-0.545,-0.594 output 0 -> Subtract.B
- [49] SafeNormalize output 0 -> Output ( z sign ) XXX.YYY Normal.A
- [7] Subtract(,1) output 0 -> Subtract.A
- [2] -0.563,-0.67,-0.484 output 0 -> Subtract.B
- [15] Input Scalar Value (Scalar) output 0 -> Abs.Input
- [55] PassThrough output 0 -> If.A
- [51] 0 output 0 -> If.B
- [52] 1 output 0 -> If.AGreaterThanB
- [53] -1 output 0 -> If.ALessThanB
- [55] PassThrough output 0 -> Abs.Input
- [57] Divide(,100) output 0 -> Floor.Input
- [54] Abs output 0 -> Divide(,100).A
- [56] Floor output 0 -> Divide(,100).A
- [65] Append output 0 -> Subtract(,0.5).A
- [59] Subtract(,0.5) output 0 -> Add.A
- [59] Subtract(,0.5) output 0 -> Add.B
- [54] Abs output 0 -> Floor.Input
- [61] Floor output 0 -> Divide(,100).A
- [62] Divide(,100) output 0 -> Floor.Input
- [62] Divide(,100) output 0 -> Subtract.A
- [63] Floor output 0 -> Subtract.B
- [58] Divide(,100) output 0 -> Append.A
- [64] Subtract output 0 -> Append.B
- [60] Add output 0 -> DeriveNormalZ.InXY
- [66] DeriveNormalZ output 0 -> Multiply.A
- [50] If output 0 -> Multiply.B
- [60] Add output 0 -> Append.A
- [71] Mask ( B ) output 0 -> Append.B
- [69] Multiply output 0 -> Mask ( B ).Input
- [67] SafeNormalize output 0 -> Output ( z sign ) XXYY.0.A

## Output Trace Roots

- `XXY.YZZ Vector` is driven by [7] Subtract(,1)
- `XXY.YZZ Normalized Vector` is driven by [13] Normalize
- `( z sign ) XXX.YYY Normal` is driven by [49] SafeNormalize
- `( z sign ) XXYY.0` is driven by [67] SafeNormalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


