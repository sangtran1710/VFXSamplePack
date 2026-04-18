# GetGradientMapRow

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/GetGradientMapRow.GetGradientMapRow`
- Category: Engine_MaterialFunctions02/Gradients
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Gets the row needed for a given gradient index in a gradient atlas map of a given size

## Inputs

- `Index (0 Based)` [FunctionInput_Scalar] - The first index is 0 and the last index is (number of gradients in the gradient map) -1.
- `Atlas Height` [FunctionInput_Scalar]
- `CurveTime` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionAdd` | Add |  |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Index (0 Based) (Scalar) | Index (0 Based) |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Atlas Height (Scalar) | Atlas Height |
| 3 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 4 | expression | `MaterialExpressionDivide` | Divide |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input CurveTime (Scalar) | CurveTime |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Add

- `A` <= [5] Divide output 0
- `B` <= [4] Divide output 0

### [4] Divide

- `A` <= [3] 0.5 output 0
- `B` <= [2] Input Atlas Height (Scalar) output 0

### [5] Divide

- `A` <= [1] Input Index (0 Based) (Scalar) output 0
- `B` <= [2] Input Atlas Height (Scalar) output 0

### [6] Output Result

- `A` <= [8] Append output 0

### [8] Append

- `A` <= [7] Input CurveTime (Scalar) output 0
- `B` <= [0] Add output 0

## Connection List

- [5] Divide output 0 -> Add.A
- [4] Divide output 0 -> Add.B
- [3] 0.5 output 0 -> Divide.A
- [2] Input Atlas Height (Scalar) output 0 -> Divide.B
- [1] Input Index (0 Based) (Scalar) output 0 -> Divide.A
- [2] Input Atlas Height (Scalar) output 0 -> Divide.B
- [8] Append output 0 -> Output Result.A
- [7] Input CurveTime (Scalar) output 0 -> Append.A
- [0] Add output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [8] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


