# Sine_Remapped

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Sine_Remapped.Sine_Remapped`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

remaps the output of a sine wave to two colors/vectors specified by the user.

## Inputs

- `Value 2` [FunctionInput_Vector3]
- `Sine Phase` [FunctionInput_Scalar]
- `Value 1` [FunctionInput_Vector3]

## Outputs

- `Remapped Sine` - Returns 0-1 linear sine with a period of 1

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Remapped Sine | Remapped Sine |
| 1 | expression | `MaterialExpressionSine` | Sine |  |
| 2 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Value 2 (Vector3) | Value 2 |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Sine Phase (Scalar) | Sine Phase |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Value 1 (Vector3) | Value 1 |

## Exact Input Wiring

### [0] Output Remapped Sine

- `A` <= [3] Lerp output 0

### [1] Sine

- `Input` <= [5] Input Sine Phase (Scalar) output 0

### [2] ConstantBiasScale

- `Input` <= [1] Sine output 0

### [3] Lerp

- `A` <= [6] Input Value 1 (Vector3) output 0
- `B` <= [4] Input Value 2 (Vector3) output 0
- `Alpha` <= [2] ConstantBiasScale output 0

## Connection List

- [3] Lerp output 0 -> Output Remapped Sine.A
- [5] Input Sine Phase (Scalar) output 0 -> Sine.Input
- [1] Sine output 0 -> ConstantBiasScale.Input
- [6] Input Value 1 (Vector3) output 0 -> Lerp.A
- [4] Input Value 2 (Vector3) output 0 -> Lerp.B
- [2] ConstantBiasScale output 0 -> Lerp.Alpha

## Output Trace Roots

- `Remapped Sine` is driven by [3] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


