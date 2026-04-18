# Lerp_3Color

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_3Color.Lerp_3Color`
- Category: Engine_MaterialFunctions02
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Lerps 3 inputs based on a single value.

## Inputs

- `A` [FunctionInput_Vector3]
- `B` [FunctionInput_Vector3]
- `C` [FunctionInput_Vector3]
- `Alpha` [FunctionInput_Scalar] - 0-1 Lerp Alpha

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input A (Vector3) | A |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input B (Vector3) | B |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input C (Vector3) | C |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 9 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 10 | expression | `MaterialExpressionSaturate` | Saturate |  |

## Exact Input Wiring

### [0] Lerp

- `A` <= [2] Input A (Vector3) output 0
- `B` <= [3] Input B (Vector3) output 0
- `Alpha` <= [9] Saturate output 0

### [1] Lerp

- `A` <= [0] Lerp output 0
- `B` <= [4] Input C (Vector3) output 0
- `Alpha` <= [10] Saturate output 0

### [6] Multiply(,2)

- `A` <= [5] Input Alpha (Scalar) output 0

### [7] Subtract(,1)

- `A` <= [6] Multiply(,2) output 0

### [8] Output Result

- `A` <= [1] Lerp output 0

### [9] Saturate

- `Input` <= [6] Multiply(,2) output 0

### [10] Saturate

- `Input` <= [7] Subtract(,1) output 0

## Connection List

- [2] Input A (Vector3) output 0 -> Lerp.A
- [3] Input B (Vector3) output 0 -> Lerp.B
- [9] Saturate output 0 -> Lerp.Alpha
- [0] Lerp output 0 -> Lerp.A
- [4] Input C (Vector3) output 0 -> Lerp.B
- [10] Saturate output 0 -> Lerp.Alpha
- [5] Input Alpha (Scalar) output 0 -> Multiply(,2).A
- [6] Multiply(,2) output 0 -> Subtract(,1).A
- [1] Lerp output 0 -> Output Result.A
- [6] Multiply(,2) output 0 -> Saturate.Input
- [7] Subtract(,1) output 0 -> Saturate.Input

## Output Trace Roots

- `Result` is driven by [1] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
