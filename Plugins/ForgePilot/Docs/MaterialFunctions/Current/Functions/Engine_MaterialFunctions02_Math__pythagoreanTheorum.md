# pythagoreanTheorum

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/pythagoreanTheorum.pythagoreanTheorum`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Side 1 Length` [FunctionInput_Scalar]

## Outputs

- `Side 2 Length`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Side 2 Length | Side 2 Length |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Side 1 Length (Scalar) | Side 1 Length |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |

## Exact Input Wiring

### [0] Output Side 2 Length

- `A` <= [4] Sqrt output 0

### [2] 1-x

- `Input` <= [3] Multiply output 0

### [3] Multiply

- `A` <= [1] Input Side 1 Length (Scalar) output 0
- `B` <= [1] Input Side 1 Length (Scalar) output 0

### [4] Sqrt

- `Input` <= [2] 1-x output 0

## Connection List

- [4] Sqrt output 0 -> Output Side 2 Length.A
- [3] Multiply output 0 -> 1-x.Input
- [1] Input Side 1 Length (Scalar) output 0 -> Multiply.A
- [1] Input Side 1 Length (Scalar) output 0 -> Multiply.B
- [2] 1-x output 0 -> Sqrt.Input

## Output Trace Roots

- `Side 2 Length` is driven by [4] Sqrt

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


