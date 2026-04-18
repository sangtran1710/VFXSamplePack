# LineIntervalIntersection

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/LineIntervalIntersection.LineIntervalIntersection`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the point of intersection for two lines. The X value for the endpoints of both lines must match. A Y value will be calculated from the interval.

## Inputs

- `A.x` [FunctionInput_Scalar] - The x value of both A points to sample from. In most cases leave at 0.
- `B.x` [FunctionInput_Scalar] - The x value of both B points to sample from.  In most cases leave at 1.
- `Line1 - A.y` [FunctionInput_Scalar] - The Y value of point A on the first line
- `Line2 - A.y` [FunctionInput_Scalar] - The Y value of point A on the second line
- `Line1 - B.y` [FunctionInput_Scalar] - The Y value of point B on the first line
- `Line2 - B.y` [FunctionInput_Scalar] - The Y value of point B on the second line

## Outputs

- `xy`
- `x`
- `y`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output xy | xy |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input A.x (Scalar) | A.x |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input B.x (Scalar) | B.x |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Line1 - A.y (Scalar) | Line1 - A.y |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Line2 - A.y (Scalar) | Line2 - A.y |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Line1 - B.y (Scalar) | Line1 - B.y |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Line2 - B.y (Scalar) | Line2 - B.y |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output x | x |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output y | y |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | custom | `MaterialExpressionCustom` | Custom |  |

## Exact Input Wiring

### [0] Output xy

- `A` <= [8] Append output 0

### [1] Multiply

- `A` <= [14] Input Line1 - B.y (Scalar) output 0
- `B` <= [6] Divide output 0

### [2] Multiply

- `A` <= [12] Input Line1 - A.y (Scalar) output 0
- `B` <= [7] 1-x output 0

### [3] Add

- `A` <= [1] Multiply output 0
- `B` <= [2] Multiply output 0

### [4] Subtract

- `A` <= [13] Input Line2 - A.y (Scalar) output 0
- `B` <= [12] Input Line1 - A.y (Scalar) output 0

### [5] Subtract

- `A` <= [14] Input Line1 - B.y (Scalar) output 0
- `B` <= [15] Input Line2 - B.y (Scalar) output 0

### [6] Divide

- `A` <= [4] Subtract output 0
- `B` <= [18] Add output 0

### [7] 1-x

- `Input` <= [6] Divide output 0

### [8] Append

- `A` <= [16] Output x output 0
- `B` <= [17] Output y output 0

### [9] Lerp

- `A` <= [10] Input A.x (Scalar) output 0
- `B` <= [11] Input B.x (Scalar) output 0
- `Alpha` <= [6] Divide output 0

### [16] Output x

- `A` <= [6] Divide output 0

### [17] Output y

- `A` <= [3] Add output 0

### [18] Add

- `A` <= [4] Subtract output 0
- `B` <= [5] Subtract output 0

## Connection List

- [8] Append output 0 -> Output xy.A
- [14] Input Line1 - B.y (Scalar) output 0 -> Multiply.A
- [6] Divide output 0 -> Multiply.B
- [12] Input Line1 - A.y (Scalar) output 0 -> Multiply.A
- [7] 1-x output 0 -> Multiply.B
- [1] Multiply output 0 -> Add.A
- [2] Multiply output 0 -> Add.B
- [13] Input Line2 - A.y (Scalar) output 0 -> Subtract.A
- [12] Input Line1 - A.y (Scalar) output 0 -> Subtract.B
- [14] Input Line1 - B.y (Scalar) output 0 -> Subtract.A
- [15] Input Line2 - B.y (Scalar) output 0 -> Subtract.B
- [4] Subtract output 0 -> Divide.A
- [18] Add output 0 -> Divide.B
- [6] Divide output 0 -> 1-x.Input
- [16] Output x output 0 -> Append.A
- [17] Output y output 0 -> Append.B
- [10] Input A.x (Scalar) output 0 -> Lerp.A
- [11] Input B.x (Scalar) output 0 -> Lerp.B
- [6] Divide output 0 -> Lerp.Alpha
- [6] Divide output 0 -> Output x.A
- [3] Add output 0 -> Output y.A
- [4] Subtract output 0 -> Add.A
- [5] Subtract output 0 -> Add.B

## Output Trace Roots

- `xy` is driven by [8] Append
- `x` is driven by [6] Divide
- `y` is driven by [3] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


