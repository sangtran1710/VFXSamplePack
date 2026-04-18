# SteppingPannerTime

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SteppingPannerTime.SteppingPannerTime`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Add to UV coordinates to make them jump by the "Jump Amount Per Step" value for the number of times, per scaled second, set by the "Number of Steps Per Unit of Time". This will make noise maps seem to be more random than they are. 

## Inputs

- `Number of Steps Per Unit of Time` [FunctionInput_Vector2]
- `TimeSpeed` [FunctionInput_Scalar]
- `Jump Amount Per Step` [FunctionInput_Vector2]
- `Time Offset` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionCeil` | Ceil |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Number of Steps Per Unit of Time (Vector2) | Number of Steps Per Unit of Time |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input TimeSpeed (Scalar) | TimeSpeed |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Jump Amount Per Step (Vector2) | Jump Amount Per Step |
| 7 | expression | `MaterialExpressionTime` | Time |  |
| 8 | expression | `MaterialExpressionFrac` | Frac |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Time Offset (Scalar) | Time Offset |
| 12 | expression | `MaterialExpressionMultiply` | Multiply(,30) |  |
| 13 | expression | `MaterialExpressionCeil` | Ceil |  |
| 14 | expression | `MaterialExpressionDivide` | Divide(,30) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Divide output 0

### [1] Multiply

- `A` <= [8] Frac output 0
- `B` <= [4] Input Number of Steps Per Unit of Time (Vector2) output 0

### [2] Ceil

- `Input` <= [1] Multiply output 0

### [3] Divide

- `A` <= [2] Ceil output 0
- `B` <= [6] Input Jump Amount Per Step (Vector2) output 0

### [8] Frac

- `Input` <= [9] Add output 0

### [9] Add

- `A` <= [10] Multiply output 0
- `B` <= [11] Input Time Offset (Scalar) output 0

### [10] Multiply

- `A` <= [7] Time output 0
- `B` <= [5] Input TimeSpeed (Scalar) output 0

### [12] Multiply(,30)

- `A` <= [9] Add output 0

### [13] Ceil

- `Input` <= [12] Multiply(,30) output 0

### [14] Divide(,30)

- `A` <= [13] Ceil output 0

## Connection List

- [3] Divide output 0 -> Output Result.A
- [8] Frac output 0 -> Multiply.A
- [4] Input Number of Steps Per Unit of Time (Vector2) output 0 -> Multiply.B
- [1] Multiply output 0 -> Ceil.Input
- [2] Ceil output 0 -> Divide.A
- [6] Input Jump Amount Per Step (Vector2) output 0 -> Divide.B
- [9] Add output 0 -> Frac.Input
- [10] Multiply output 0 -> Add.A
- [11] Input Time Offset (Scalar) output 0 -> Add.B
- [7] Time output 0 -> Multiply.A
- [5] Input TimeSpeed (Scalar) output 0 -> Multiply.B
- [9] Add output 0 -> Multiply(,30).A
- [12] Multiply(,30) output 0 -> Ceil.Input
- [13] Ceil output 0 -> Divide(,30).A

## Output Trace Roots

- `Result` is driven by [3] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
