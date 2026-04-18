# DebugTimeSine

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugTimeSine.DebugTimeSine`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

useful for seeing what a scalar value does between 0 and 1

## Inputs

- `Speed` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTime` | Time |  |
| 2 | expression | `MaterialExpressionSine` | Sine |  |
| 3 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Speed (Scalar) | Speed |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] ConstantBiasScale output 0

### [2] Sine

- `Input` <= [4] Multiply output 0

### [3] ConstantBiasScale

- `Input` <= [2] Sine output 0

### [4] Multiply

- `A` <= [1] Time output 0
- `B` <= [5] Input Speed (Scalar) output 0

## Connection List

- [3] ConstantBiasScale output 0 -> Output Result.A
- [4] Multiply output 0 -> Sine.Input
- [2] Sine output 0 -> ConstantBiasScale.Input
- [1] Time output 0 -> Multiply.A
- [5] Input Speed (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [3] ConstantBiasScale

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


