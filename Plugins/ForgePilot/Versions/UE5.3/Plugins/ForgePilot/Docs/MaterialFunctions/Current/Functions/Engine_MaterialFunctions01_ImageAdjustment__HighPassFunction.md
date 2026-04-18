# HighPassFunction

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/HighPassFunction.HighPassFunction`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs a high pass type operation on the input functions. This filters out low frequency information. Note that you do do not have to hook up all 6 offset function inputs if you do not want full 3d sampling. If you exclude the two Z samples and set the "Number of Samples" to 4, it will be a 2D highpass on the XY plane.

## Inputs

- `Contrast` [FunctionInput_Scalar]
- `Position` [FunctionInput_Vector3]
- `Highpass Offset` [FunctionInput_Vector3]
- `Center Sample` [FunctionInput_Vector3]
- `Offset Sample +X` [FunctionInput_Vector3]
- `Offset Sample -X` [FunctionInput_Vector3]
- `Offset Sample +Y` [FunctionInput_Vector3]
- `Offset Sample -Y` [FunctionInput_Vector3]
- `Offset Sample +Z` [FunctionInput_Vector3]
- `Offset Sample -Z` [FunctionInput_Vector3]
- `Number of Samples` [FunctionInput_Scalar] - Defaults to 6 samples to get 2 samples on each axis. If you want to skip a certain axis you can lower the number of samples to match to perform 2d or 1d highpass. For example, if you leave the two Z offsets disconnected and set this value to 4, it will do a 2D sharpen on the XY plane.

## Outputs

- `Result` - The high pass result.
- `Offset +X`
- `Offset -X`
- `Offset +Y`
- `Offset -Y`
- `Offset +Z`
- `Offset -Z`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Position (Vector3) | Position |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Highpass Offset (Vector3) | Highpass Offset |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 10 | expression | `MaterialExpressionPower` | Power |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Offset +X | Offset +X |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output Offset -X | Offset -X |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Offset +Y | Offset +Y |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Center Sample (Vector3) | Center Sample |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample +X (Vector3) | Offset Sample +X |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample -X (Vector3) | Offset Sample -X |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample +Y (Vector3) | Offset Sample +Y |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | BlurSampleOffsets |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionConstant` | 0 |  |
| 24 | expression | `MaterialExpressionConstant` | 0 |  |
| 25 | expression | `MaterialExpressionAppendVector` | Append |  |
| 26 | expression | `MaterialExpressionConstant` | 0 |  |
| 27 | expression | `MaterialExpressionAppendVector` | Append |  |
| 28 | expression | `MaterialExpressionConstant` | 0 |  |
| 29 | expression | `MaterialExpressionAppendVector` | Append |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample -Y (Vector3) | Offset Sample -Y |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output Offset -Y | Offset -Y |
| 32 | expression | `MaterialExpressionAdd` | Add |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample +Z (Vector3) | Offset Sample +Z |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample -Z (Vector3) | Offset Sample -Z |
| 36 | expression | `MaterialExpressionAdd` | Add |  |
| 37 | expression | `MaterialExpressionAdd` | Add |  |
| 38 | function_output | `MaterialExpressionFunctionOutput` | Output Offset +Z | Offset +Z |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output Offset -Z | Offset -Z |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | expression | `MaterialExpressionAppendVector` | Append |  |
| 42 | expression | `MaterialExpressionConstant` | 0 |  |
| 43 | expression | `MaterialExpressionAppendVector` | Append |  |
| 44 | expression | `MaterialExpressionConstant` | 0 |  |
| 45 | expression | `MaterialExpressionAdd` | Add |  |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Number of Samples (Scalar) | Number of Samples |
| 47 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [11] Multiply(,0.5) output 0

### [2] Input Position (Vector3)

- `Preview` <= [47] Absolute World Position output 0

### [4] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [22] Append output 0

### [5] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [25] Append output 0

### [6] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [29] Append output 0

### [7] Subtract

- `A` <= [15] Input Center Sample (Vector3) output 0
- `B` <= [8] Divide output 0

### [8] Divide

- `A` <= [45] Add output 0
- `B` <= [46] Input Number of Samples (Scalar) output 0

### [9] Add(,1)

- `A` <= [7] Subtract output 0

### [10] Power

- `Base` <= [9] Add(,1) output 0
- `Exponent` <= [1] Input Contrast (Scalar) output 0

### [11] Multiply(,0.5)

- `A` <= [10] Power output 0

### [12] Output Offset +X

- `A` <= [4] Add output 0

### [13] Output Offset -X

- `A` <= [5] Add output 0

### [14] Output Offset +Y

- `A` <= [6] Add output 0

### [19] Add

- `A` <= [16] Input Offset Sample +X (Vector3) output 0
- `B` <= [17] Input Offset Sample -X (Vector3) output 0

### [20] Add

- `A` <= [19] Add output 0
- `B` <= [33] Add output 0

### [22] Append

- `A` <= [21] BlurSampleOffsets output 1
- `B` <= [23] 0 output 0

### [25] Append

- `A` <= [21] BlurSampleOffsets output 3
- `B` <= [24] 0 output 0

### [27] Append

- `A` <= [21] BlurSampleOffsets output 2
- `B` <= [26] 0 output 0

### [29] Append

- `A` <= [21] BlurSampleOffsets output 0
- `B` <= [28] 0 output 0

### [31] Output Offset -Y

- `A` <= [32] Add output 0

### [32] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [27] Append output 0

### [33] Add

- `A` <= [18] Input Offset Sample +Y (Vector3) output 0
- `B` <= [30] Input Offset Sample -Y (Vector3) output 0

### [36] Add

- `A` <= [34] Input Offset Sample +Z (Vector3) output 0
- `B` <= [35] Input Offset Sample -Z (Vector3) output 0

### [37] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [41] Append output 0

### [38] Output Offset +Z

- `A` <= [37] Add output 0

### [39] Output Offset -Z

- `A` <= [40] Add output 0

### [40] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [43] Append output 0

### [41] Append

- `A` <= [42] 0 output 0
- `B` <= [21] BlurSampleOffsets output 0

### [43] Append

- `A` <= [44] 0 output 0
- `B` <= [21] BlurSampleOffsets output 2

### [45] Add

- `A` <= [20] Add output 0
- `B` <= [36] Add output 0

## Connection List

- [11] Multiply(,0.5) output 0 -> Output Result.A
- [47] Absolute World Position output 0 -> Input Position (Vector3).Preview
- [2] Input Position (Vector3) output 0 -> Add.A
- [22] Append output 0 -> Add.B
- [2] Input Position (Vector3) output 0 -> Add.A
- [25] Append output 0 -> Add.B
- [2] Input Position (Vector3) output 0 -> Add.A
- [29] Append output 0 -> Add.B
- [15] Input Center Sample (Vector3) output 0 -> Subtract.A
- [8] Divide output 0 -> Subtract.B
- [45] Add output 0 -> Divide.A
- [46] Input Number of Samples (Scalar) output 0 -> Divide.B
- [7] Subtract output 0 -> Add(,1).A
- [9] Add(,1) output 0 -> Power.Base
- [1] Input Contrast (Scalar) output 0 -> Power.Exponent
- [10] Power output 0 -> Multiply(,0.5).A
- [4] Add output 0 -> Output Offset +X.A
- [5] Add output 0 -> Output Offset -X.A
- [6] Add output 0 -> Output Offset +Y.A
- [16] Input Offset Sample +X (Vector3) output 0 -> Add.A
- [17] Input Offset Sample -X (Vector3) output 0 -> Add.B
- [19] Add output 0 -> Add.A
- [33] Add output 0 -> Add.B
- [21] BlurSampleOffsets output 1 -> Append.A
- [23] 0 output 0 -> Append.B
- [21] BlurSampleOffsets output 3 -> Append.A
- [24] 0 output 0 -> Append.B
- [21] BlurSampleOffsets output 2 -> Append.A
- [26] 0 output 0 -> Append.B
- [21] BlurSampleOffsets output 0 -> Append.A
- [28] 0 output 0 -> Append.B
- [32] Add output 0 -> Output Offset -Y.A
- [2] Input Position (Vector3) output 0 -> Add.A
- [27] Append output 0 -> Add.B
- [18] Input Offset Sample +Y (Vector3) output 0 -> Add.A
- [30] Input Offset Sample -Y (Vector3) output 0 -> Add.B
- [34] Input Offset Sample +Z (Vector3) output 0 -> Add.A
- [35] Input Offset Sample -Z (Vector3) output 0 -> Add.B
- [2] Input Position (Vector3) output 0 -> Add.A
- [41] Append output 0 -> Add.B
- [37] Add output 0 -> Output Offset +Z.A
- [40] Add output 0 -> Output Offset -Z.A
- [2] Input Position (Vector3) output 0 -> Add.A
- [43] Append output 0 -> Add.B
- [42] 0 output 0 -> Append.A
- [21] BlurSampleOffsets output 0 -> Append.B
- [44] 0 output 0 -> Append.A
- [21] BlurSampleOffsets output 2 -> Append.B
- [20] Add output 0 -> Add.A
- [36] Add output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [11] Multiply(,0.5)
- `Offset +X` is driven by [4] Add
- `Offset -X` is driven by [5] Add
- `Offset +Y` is driven by [6] Add
- `Offset -Y` is driven by [32] Add
- `Offset +Z` is driven by [37] Add
- `Offset -Z` is driven by [40] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


