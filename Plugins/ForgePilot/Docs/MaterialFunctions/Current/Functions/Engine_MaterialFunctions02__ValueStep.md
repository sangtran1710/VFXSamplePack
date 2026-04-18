# ValueStep

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ValueStep.ValueStep`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Takes a gradient with values over one and masks out a portion of the gradient based on input variables.

## Inputs

- `Gradient` [FunctionInput_Scalar] - Enter a gradient with values above one.
- `Mask Offset Value` [FunctionInput_Scalar] - Offsets where the black spots are placed.
- `Number Before White Result` [FunctionInput_Scalar] - If you enter a gradient that goes to 10 and this number is 9. 0-9 will be black and 10 will be white. 

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionCeil` | Ceil |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionFrac` | Frac |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionIf` | If |  |
| 8 | expression | `MaterialExpressionConstant` | 1 |  |
| 9 | expression | `MaterialExpressionConstant` | 0 |  |
| 10 | expression | `MaterialExpressionConstant` | 1 |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Gradient (Scalar) | Gradient |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Mask Offset Value (Scalar) | Mask Offset Value |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Number Before White Result (Scalar) | Number Before White Result |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionConstant` | 3 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] If output 0

### [1] Add

- `A` <= [11] Input Gradient (Scalar) output 0
- `B` <= [12] Input Mask Offset Value (Scalar) output 0

### [2] Ceil

- `Input` <= [4] Multiply output 0

### [3] Divide

- `A` <= [1] Add output 0
- `B` <= [6] Add output 0

### [4] Multiply

- `A` <= [5] Frac output 0
- `B` <= [6] Add output 0

### [5] Frac

- `Input` <= [3] Divide output 0

### [6] Add

- `A` <= [13] Input Number Before White Result (Scalar) output 0
- `B` <= [8] 1 output 0

### [7] If

- `A` <= [2] Ceil output 0
- `B` <= [13] Input Number Before White Result (Scalar) output 0
- `AGreaterThanB` <= [9] 0 output 0
- `AEqualsB` <= [10] 1 output 0
- `ALessThanB` <= [9] 0 output 0

### [11] Input Gradient (Scalar)

- `Preview` <= [15] Multiply output 0

### [15] Multiply

- `A` <= [14] LinearGradient output 1
- `B` <= [16] 3 output 0

## Connection List

- [7] If output 0 -> Output Result.A
- [11] Input Gradient (Scalar) output 0 -> Add.A
- [12] Input Mask Offset Value (Scalar) output 0 -> Add.B
- [4] Multiply output 0 -> Ceil.Input
- [1] Add output 0 -> Divide.A
- [6] Add output 0 -> Divide.B
- [5] Frac output 0 -> Multiply.A
- [6] Add output 0 -> Multiply.B
- [3] Divide output 0 -> Frac.Input
- [13] Input Number Before White Result (Scalar) output 0 -> Add.A
- [8] 1 output 0 -> Add.B
- [2] Ceil output 0 -> If.A
- [13] Input Number Before White Result (Scalar) output 0 -> If.B
- [9] 0 output 0 -> If.AGreaterThanB
- [10] 1 output 0 -> If.AEqualsB
- [9] 0 output 0 -> If.ALessThanB
- [15] Multiply output 0 -> Input Gradient (Scalar).Preview
- [14] LinearGradient output 1 -> Multiply.A
- [16] 3 output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [7] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


