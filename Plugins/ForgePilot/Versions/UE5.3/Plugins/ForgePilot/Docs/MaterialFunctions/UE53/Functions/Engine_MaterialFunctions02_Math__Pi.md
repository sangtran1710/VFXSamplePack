# Pi

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/Pi.Pi`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Pi multiplied by an input value.

## Inputs

- `Multiplier` [FunctionInput_Scalar] - Multiply pi

## Outputs

- `Result` - Pi multiplied by the multiplier value.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant` | 3.142 |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Multiplier (Scalar) | Multiplier |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Multiply output 0

### [2] Multiply

- `A` <= [1] 3.142 output 0
- `B` <= [3] Input Multiplier (Scalar) output 0

## Connection List

- [2] Multiply output 0 -> Output Result.A
- [1] 3.142 output 0 -> Multiply.A
- [3] Input Multiplier (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [2] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
