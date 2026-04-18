# SmoothThreshold

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SmoothThreshold.SmoothThreshold`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Smooth contrast applied after a threshold value is passed

## Inputs

- `Gradient` [FunctionInput_Scalar] - Input a value to use as the base for the contrast operation. 
- `Cutoff Value` [FunctionInput_Scalar] - Values from the gradient input below this value will be effected by the operation.
- `Lerp Value` [FunctionInput_Scalar] - Add a negative or positive number to smoothly ramp to from the original values. Ramping occurs in values lower than the Cutoff Value.

## Outputs

- `Result` - Be sure to clamp the values if needed. There is no cap on the outputed range.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Gradient (Scalar) | Gradient |
| 5 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 7 | expression | `MaterialExpressionConstant` | 1 |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 10 | expression | `MaterialExpressionConstant` | 1 |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Cutoff Value (Scalar) | Cutoff Value |
| 12 | expression | `MaterialExpressionConstant` | 0 |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Lerp Value (Scalar) | Lerp Value |
| 14 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] If output 0

### [1] If

- `A` <= [4] Input Gradient (Scalar) output 0
- `B` <= [14] Add output 0
- `AGreaterThanB` <= [4] Input Gradient (Scalar) output 0
- `ALessThanB` <= [3] Multiply output 0

### [2] Multiply

- `A` <= [4] Input Gradient (Scalar) output 0
- `B` <= [8] Divide output 0

### [3] Multiply

- `A` <= [9] Lerp output 0
- `B` <= [14] Add output 0

### [4] Input Gradient (Scalar)

- `Preview` <= [6] Mask ( G ) output 0

### [6] Mask ( G )

- `Input` <= [5] TexCoord[0] output 0

### [8] Divide

- `A` <= [7] 1 output 0
- `B` <= [14] Add output 0

### [9] Lerp

- `A` <= [13] Input Lerp Value (Scalar) output 0
- `B` <= [10] 1 output 0
- `Alpha` <= [2] Multiply output 0

### [11] Input Cutoff Value (Scalar)

- `Preview` <= [12] 0 output 0

### [14] Add

- `A` <= [11] Input Cutoff Value (Scalar) output 0
- `B` <= [11] Input Cutoff Value (Scalar) output 0

## Connection List

- [1] If output 0 -> Output Result.A
- [4] Input Gradient (Scalar) output 0 -> If.A
- [14] Add output 0 -> If.B
- [4] Input Gradient (Scalar) output 0 -> If.AGreaterThanB
- [3] Multiply output 0 -> If.ALessThanB
- [4] Input Gradient (Scalar) output 0 -> Multiply.A
- [8] Divide output 0 -> Multiply.B
- [9] Lerp output 0 -> Multiply.A
- [14] Add output 0 -> Multiply.B
- [6] Mask ( G ) output 0 -> Input Gradient (Scalar).Preview
- [5] TexCoord[0] output 0 -> Mask ( G ).Input
- [7] 1 output 0 -> Divide.A
- [14] Add output 0 -> Divide.B
- [13] Input Lerp Value (Scalar) output 0 -> Lerp.A
- [10] 1 output 0 -> Lerp.B
- [2] Multiply output 0 -> Lerp.Alpha
- [12] 0 output 0 -> Input Cutoff Value (Scalar).Preview
- [11] Input Cutoff Value (Scalar) output 0 -> Add.A
- [11] Input Cutoff Value (Scalar) output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [1] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


