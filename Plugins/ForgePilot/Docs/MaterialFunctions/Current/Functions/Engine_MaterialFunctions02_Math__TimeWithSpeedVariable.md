# TimeWithSpeedVariable

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TimeWithSpeedVariable.TimeWithSpeedVariable`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Time multiplied by speed with an option to use the remainder or time itself.

## Inputs

- `Speed` [FunctionInput_Scalar]
- `Time` [FunctionInput_Vector3]

## Outputs

- `Time`
- `Frac Time`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Time | Time |
| 1 | expression | `MaterialExpressionTime` | Time |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Speed (Scalar) | Speed |
| 4 | expression | `MaterialExpressionFrac` | Frac |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Frac Time | Frac Time |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Time (Vector3) | Time |

## Exact Input Wiring

### [0] Output Time

- `A` <= [2] Multiply output 0

### [2] Multiply

- `A` <= [6] Input Time (Vector3) output 0
- `B` <= [3] Input Speed (Scalar) output 0

### [4] Frac

- `Input` <= [2] Multiply output 0

### [5] Output Frac Time

- `A` <= [4] Frac output 0

### [6] Input Time (Vector3)

- `Preview` <= [1] Time output 0

## Connection List

- [2] Multiply output 0 -> Output Time.A
- [6] Input Time (Vector3) output 0 -> Multiply.A
- [3] Input Speed (Scalar) output 0 -> Multiply.B
- [2] Multiply output 0 -> Frac.Input
- [4] Frac output 0 -> Output Frac Time.A
- [1] Time output 0 -> Input Time (Vector3).Preview

## Output Trace Roots

- `Time` is driven by [2] Multiply
- `Frac Time` is driven by [4] Frac

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


