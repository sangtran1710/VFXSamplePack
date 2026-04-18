# UnSharpMaskFunction

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/UnSharpMaskFunction.UnSharpMaskFunction`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs a high pass type operation on the input functions. This filters out low frequency information.

## Inputs

- `Amount` [FunctionInput_Scalar]
- `Position` [FunctionInput_Vector3]
- `Sample Offset` [FunctionInput_Vector3]
- `Center Sample` [FunctionInput_Vector3]
- `Offset Sample +X` [FunctionInput_Vector3]
- `Offset Sample -X` [FunctionInput_Vector3]
- `Offset Sample +Y` [FunctionInput_Vector3]
- `Offset Sample -Y` [FunctionInput_Vector3]
- `Offset Sample +Z` [FunctionInput_Vector3]
- `Offset Sample -Z` [FunctionInput_Vector3]
- `Number of Samples` [FunctionInput_Scalar] - Defaults to 6 samples to get 2 samples on each axis. If you want to skip a certain axis you can lower the number of samples to match to perform 2d or 1d highpass. For example, if you leave the two Z offsets disconnected and set this value to 4, it will do a 2D highpass on the XY plane.

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
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Amount (Scalar) | Amount |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Position (Vector3) | Position |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Sample Offset (Vector3) | Sample Offset |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output Offset +X | Offset +X |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Offset -X | Offset -X |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output Offset +Y | Offset +Y |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Center Sample (Vector3) | Center Sample |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample +X (Vector3) | Offset Sample +X |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample -X (Vector3) | Offset Sample -X |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample +Y (Vector3) | Offset Sample +Y |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionAdd` | Add |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | BlurSampleOffsets |  |
| 19 | expression | `MaterialExpressionAppendVector` | Append |  |
| 20 | expression | `MaterialExpressionConstant` | 0 |  |
| 21 | expression | `MaterialExpressionConstant` | 0 |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionConstant` | 0 |  |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | expression | `MaterialExpressionConstant` | 0 |  |
| 26 | expression | `MaterialExpressionAppendVector` | Append |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample -Y (Vector3) | Offset Sample -Y |
| 28 | function_output | `MaterialExpressionFunctionOutput` | Output Offset -Y | Offset -Y |
| 29 | expression | `MaterialExpressionAdd` | Add |  |
| 30 | expression | `MaterialExpressionAdd` | Add |  |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample +Z (Vector3) | Offset Sample +Z |
| 32 | function_input | `MaterialExpressionFunctionInput` | Input Offset Sample -Z (Vector3) | Offset Sample -Z |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionAdd` | Add |  |
| 35 | function_output | `MaterialExpressionFunctionOutput` | Output Offset +Z | Offset +Z |
| 36 | function_output | `MaterialExpressionFunctionOutput` | Output Offset -Z | Offset -Z |
| 37 | expression | `MaterialExpressionAdd` | Add |  |
| 38 | expression | `MaterialExpressionAppendVector` | Append |  |
| 39 | expression | `MaterialExpressionConstant` | 0 |  |
| 40 | expression | `MaterialExpressionAppendVector` | Append |  |
| 41 | expression | `MaterialExpressionConstant` | 0 |  |
| 42 | expression | `MaterialExpressionAdd` | Add |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input Number of Samples (Scalar) | Number of Samples |
| 44 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 45 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 46 | expression | `MaterialExpressionAdd` | Add(1,) |  |
| 47 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Subtract output 0

### [2] Input Position (Vector3)

- `Preview` <= [44] Absolute World Position output 0

### [4] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [19] Append output 0

### [5] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [22] Append output 0

### [6] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [26] Append output 0

### [7] Subtract

- `A` <= [45] Multiply output 0
- `B` <= [47] Multiply output 0

### [8] Divide

- `A` <= [42] Add output 0
- `B` <= [43] Input Number of Samples (Scalar) output 0

### [9] Output Offset +X

- `A` <= [4] Add output 0

### [10] Output Offset -X

- `A` <= [5] Add output 0

### [11] Output Offset +Y

- `A` <= [6] Add output 0

### [16] Add

- `A` <= [13] Input Offset Sample +X (Vector3) output 0
- `B` <= [14] Input Offset Sample -X (Vector3) output 0

### [17] Add

- `A` <= [16] Add output 0
- `B` <= [30] Add output 0

### [19] Append

- `A` <= [18] BlurSampleOffsets output 1
- `B` <= [20] 0 output 0

### [22] Append

- `A` <= [18] BlurSampleOffsets output 3
- `B` <= [21] 0 output 0

### [24] Append

- `A` <= [18] BlurSampleOffsets output 2
- `B` <= [23] 0 output 0

### [26] Append

- `A` <= [18] BlurSampleOffsets output 0
- `B` <= [25] 0 output 0

### [28] Output Offset -Y

- `A` <= [29] Add output 0

### [29] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [24] Append output 0

### [30] Add

- `A` <= [15] Input Offset Sample +Y (Vector3) output 0
- `B` <= [27] Input Offset Sample -Y (Vector3) output 0

### [33] Add

- `A` <= [31] Input Offset Sample +Z (Vector3) output 0
- `B` <= [32] Input Offset Sample -Z (Vector3) output 0

### [34] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [38] Append output 0

### [35] Output Offset +Z

- `A` <= [34] Add output 0

### [36] Output Offset -Z

- `A` <= [37] Add output 0

### [37] Add

- `A` <= [2] Input Position (Vector3) output 0
- `B` <= [40] Append output 0

### [38] Append

- `A` <= [39] 0 output 0
- `B` <= [18] BlurSampleOffsets output 0

### [40] Append

- `A` <= [41] 0 output 0
- `B` <= [18] BlurSampleOffsets output 2

### [42] Add

- `A` <= [17] Add output 0
- `B` <= [33] Add output 0

### [45] Multiply

- `A` <= [12] Input Center Sample (Vector3) output 0
- `B` <= [46] Add(1,) output 0

### [46] Add(1,)

- `B` <= [1] Input Amount (Scalar) output 0

### [47] Multiply

- `A` <= [1] Input Amount (Scalar) output 0
- `B` <= [8] Divide output 0

## Connection List

- [7] Subtract output 0 -> Output Result.A
- [44] Absolute World Position output 0 -> Input Position (Vector3).Preview
- [2] Input Position (Vector3) output 0 -> Add.A
- [19] Append output 0 -> Add.B
- [2] Input Position (Vector3) output 0 -> Add.A
- [22] Append output 0 -> Add.B
- [2] Input Position (Vector3) output 0 -> Add.A
- [26] Append output 0 -> Add.B
- [45] Multiply output 0 -> Subtract.A
- [47] Multiply output 0 -> Subtract.B
- [42] Add output 0 -> Divide.A
- [43] Input Number of Samples (Scalar) output 0 -> Divide.B
- [4] Add output 0 -> Output Offset +X.A
- [5] Add output 0 -> Output Offset -X.A
- [6] Add output 0 -> Output Offset +Y.A
- [13] Input Offset Sample +X (Vector3) output 0 -> Add.A
- [14] Input Offset Sample -X (Vector3) output 0 -> Add.B
- [16] Add output 0 -> Add.A
- [30] Add output 0 -> Add.B
- [18] BlurSampleOffsets output 1 -> Append.A
- [20] 0 output 0 -> Append.B
- [18] BlurSampleOffsets output 3 -> Append.A
- [21] 0 output 0 -> Append.B
- [18] BlurSampleOffsets output 2 -> Append.A
- [23] 0 output 0 -> Append.B
- [18] BlurSampleOffsets output 0 -> Append.A
- [25] 0 output 0 -> Append.B
- [29] Add output 0 -> Output Offset -Y.A
- [2] Input Position (Vector3) output 0 -> Add.A
- [24] Append output 0 -> Add.B
- [15] Input Offset Sample +Y (Vector3) output 0 -> Add.A
- [27] Input Offset Sample -Y (Vector3) output 0 -> Add.B
- [31] Input Offset Sample +Z (Vector3) output 0 -> Add.A
- [32] Input Offset Sample -Z (Vector3) output 0 -> Add.B
- [2] Input Position (Vector3) output 0 -> Add.A
- [38] Append output 0 -> Add.B
- [34] Add output 0 -> Output Offset +Z.A
- [37] Add output 0 -> Output Offset -Z.A
- [2] Input Position (Vector3) output 0 -> Add.A
- [40] Append output 0 -> Add.B
- [39] 0 output 0 -> Append.A
- [18] BlurSampleOffsets output 0 -> Append.B
- [41] 0 output 0 -> Append.A
- [18] BlurSampleOffsets output 2 -> Append.B
- [17] Add output 0 -> Add.A
- [33] Add output 0 -> Add.B
- [12] Input Center Sample (Vector3) output 0 -> Multiply.A
- [46] Add(1,) output 0 -> Multiply.B
- [1] Input Amount (Scalar) output 0 -> Add(1,).B
- [1] Input Amount (Scalar) output 0 -> Multiply.A
- [8] Divide output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [7] Subtract
- `Offset +X` is driven by [4] Add
- `Offset -X` is driven by [5] Add
- `Offset +Y` is driven by [6] Add
- `Offset -Y` is driven by [29] Add
- `Offset +Z` is driven by [34] Add
- `Offset -Z` is driven by [37] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


