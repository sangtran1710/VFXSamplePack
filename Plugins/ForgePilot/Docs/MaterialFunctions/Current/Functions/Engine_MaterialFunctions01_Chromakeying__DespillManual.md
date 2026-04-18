# DespillManual

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DespillManual.DespillManual`
- Category: Engine_MaterialFunctions01/Chromakeying
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Manual despill function

## Inputs

- `Image Color` [FunctionInput_Vector3] - The input image to apply despill on
- `Raw Comparison` [FunctionInput_Vector3] - Image comparison with key color
- `KeyedColor` [FunctionInput_Vector3]
- `Despill Minimum` [FunctionInput_Scalar]
- `Despill Divisor` [FunctionInput_Scalar] - Soften despill effect
- `Despill Amount` [FunctionInput_Scalar]
- `Sky Color` [FunctionInput_Vector3]
- `Sky Intensity` [FunctionInput_Scalar] - Fake sky bounce fill color

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 6 | expression | `MaterialExpressionConstant` | 1 |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 9 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Image Color (Vector3) | Image Color |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Raw Comparison (Vector3) | Raw Comparison |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input KeyedColor (Vector3) | KeyedColor |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Despill Minimum (Scalar) | Despill Minimum |
| 21 | expression | `MaterialExpressionConstant` | 0.3 |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Despill Divisor (Scalar) | Despill Divisor |
| 23 | expression | `MaterialExpressionConstant` | 0.3 |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Despill Amount (Scalar) | Despill Amount |
| 25 | expression | `MaterialExpressionConstant` | 0.3 |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Sky Color (Vector3) | Sky Color |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Sky Intensity (Scalar) | Sky Intensity |
| 28 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Add output 0

### [1] Add

- `A` <= [3] Multiply output 0
- `B` <= [9] Saturate output 0

### [2] Multiply

- `A` <= [5] Desaturation output 0
- `B` <= [26] Input Sky Color (Vector3) output 0

### [3] Multiply

- `A` <= [2] Multiply output 0
- `B` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [27] Input Sky Intensity (Scalar) output 0
- `B` <= [15] 1-x output 0

### [5] Desaturation

- `Input` <= [17] Input Image Color (Vector3) output 0
- `Fraction` <= [6] 1 output 0

### [7] Divide

- `A` <= [16] Subtract output 0
- `B` <= [22] Input Despill Divisor (Scalar) output 0

### [8] Saturate

- `Input` <= [7] Divide output 0

### [9] Saturate

- `Input` <= [11] Subtract output 0

### [10] Multiply

- `A` <= [14] Multiply output 0
- `B` <= [15] 1-x output 0

### [11] Subtract

- `A` <= [17] Input Image Color (Vector3) output 0
- `B` <= [13] Multiply output 0

### [12] Normalize

- `VectorInput` <= [19] Input KeyedColor (Vector3) output 0

### [13] Multiply

- `A` <= [10] Multiply output 0
- `B` <= [24] Input Despill Amount (Scalar) output 0

### [14] Multiply

- `A` <= [17] Input Image Color (Vector3) output 0
- `B` <= [12] Normalize output 0

### [15] 1-x

- `Input` <= [8] Saturate output 0

### [16] Subtract

- `A` <= [18] Input Raw Comparison (Vector3) output 0
- `B` <= [20] Input Despill Minimum (Scalar) output 0

### [20] Input Despill Minimum (Scalar)

- `Preview` <= [21] 0.3 output 0

### [22] Input Despill Divisor (Scalar)

- `Preview` <= [23] 0.3 output 0

### [24] Input Despill Amount (Scalar)

- `Preview` <= [25] 0.3 output 0

### [27] Input Sky Intensity (Scalar)

- `Preview` <= [28] 1 output 0

## Connection List

- [1] Add output 0 -> Output Result.A
- [3] Multiply output 0 -> Add.A
- [9] Saturate output 0 -> Add.B
- [5] Desaturation output 0 -> Multiply.A
- [26] Input Sky Color (Vector3) output 0 -> Multiply.B
- [2] Multiply output 0 -> Multiply.A
- [4] Multiply output 0 -> Multiply.B
- [27] Input Sky Intensity (Scalar) output 0 -> Multiply.A
- [15] 1-x output 0 -> Multiply.B
- [17] Input Image Color (Vector3) output 0 -> Desaturation.Input
- [6] 1 output 0 -> Desaturation.Fraction
- [16] Subtract output 0 -> Divide.A
- [22] Input Despill Divisor (Scalar) output 0 -> Divide.B
- [7] Divide output 0 -> Saturate.Input
- [11] Subtract output 0 -> Saturate.Input
- [14] Multiply output 0 -> Multiply.A
- [15] 1-x output 0 -> Multiply.B
- [17] Input Image Color (Vector3) output 0 -> Subtract.A
- [13] Multiply output 0 -> Subtract.B
- [19] Input KeyedColor (Vector3) output 0 -> Normalize.VectorInput
- [10] Multiply output 0 -> Multiply.A
- [24] Input Despill Amount (Scalar) output 0 -> Multiply.B
- [17] Input Image Color (Vector3) output 0 -> Multiply.A
- [12] Normalize output 0 -> Multiply.B
- [8] Saturate output 0 -> 1-x.Input
- [18] Input Raw Comparison (Vector3) output 0 -> Subtract.A
- [20] Input Despill Minimum (Scalar) output 0 -> Subtract.B
- [21] 0.3 output 0 -> Input Despill Minimum (Scalar).Preview
- [23] 0.3 output 0 -> Input Despill Divisor (Scalar).Preview
- [25] 0.3 output 0 -> Input Despill Amount (Scalar).Preview
- [28] 1 output 0 -> Input Sky Intensity (Scalar).Preview

## Output Trace Roots

- `Result` is driven by [1] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


