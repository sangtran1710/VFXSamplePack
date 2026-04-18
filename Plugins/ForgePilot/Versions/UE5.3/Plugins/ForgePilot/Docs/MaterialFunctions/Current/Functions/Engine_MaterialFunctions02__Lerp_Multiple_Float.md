# Lerp_Multiple_Float

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_Multiple_Float.Lerp_Multiple_Float`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Lerps multiple inputs based on a single value.

## Inputs

- `1` [FunctionInput_Scalar]
- `2` [FunctionInput_Scalar]
- `3` [FunctionInput_Scalar]
- `4` [FunctionInput_Scalar]
- `A` [FunctionInput_Scalar] - 0-1 lerp alpha

## Outputs

- `Lerp 3 Inputs`
- `Lerp 4 Inputs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Lerp 3 Inputs | Lerp 3 Inputs |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input 1 (Scalar) | 1 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input 2 (Scalar) | 2 |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input 3 (Scalar) | 3 |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input 4 (Scalar) | 4 |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input A (Scalar) | A |
| 8 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 9 | expression | `MaterialExpressionClamp` | Clamp |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 16 | expression | `MaterialExpressionClamp` | Clamp |  |
| 17 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 18 | expression | `MaterialExpressionClamp` | Clamp |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 20 | expression | `MaterialExpressionClamp` | Clamp |  |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Lerp 4 Inputs | Lerp 4 Inputs |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |

## Exact Input Wiring

### [0] Output Lerp 3 Inputs

- `A` <= [2] Lerp output 0

### [1] Lerp

- `A` <= [3] Input 1 (Scalar) output 0
- `B` <= [4] Input 2 (Scalar) output 0
- `Alpha` <= [9] Clamp output 0

### [2] Lerp

- `A` <= [1] Lerp output 0
- `B` <= [5] Input 3 (Scalar) output 0
- `Alpha` <= [11] Clamp output 0

### [7] Input A (Scalar)

- `Preview` <= [22] TimeWithSpeedVariable output 0

### [8] Multiply(,2)

- `A` <= [7] Input A (Scalar) output 0

### [9] Clamp

- `Input` <= [8] Multiply(,2) output 0

### [10] Subtract(,1)

- `A` <= [8] Multiply(,2) output 0

### [11] Clamp

- `Input` <= [10] Subtract(,1) output 0

### [12] Lerp

- `A` <= [3] Input 1 (Scalar) output 0
- `B` <= [4] Input 2 (Scalar) output 0
- `Alpha` <= [16] Clamp output 0

### [13] Lerp

- `A` <= [12] Lerp output 0
- `B` <= [5] Input 3 (Scalar) output 0
- `Alpha` <= [18] Clamp output 0

### [14] Lerp

- `A` <= [13] Lerp output 0
- `B` <= [6] Input 4 (Scalar) output 0
- `Alpha` <= [20] Clamp output 0

### [15] Multiply(,3)

- `A` <= [7] Input A (Scalar) output 0

### [16] Clamp

- `Input` <= [15] Multiply(,3) output 0

### [17] Subtract(,1)

- `A` <= [15] Multiply(,3) output 0

### [18] Clamp

- `Input` <= [17] Subtract(,1) output 0

### [19] Subtract(,1)

- `A` <= [17] Subtract(,1) output 0

### [20] Clamp

- `Input` <= [19] Subtract(,1) output 0

### [21] Output Lerp 4 Inputs

- `A` <= [14] Lerp output 0

## Connection List

- [2] Lerp output 0 -> Output Lerp 3 Inputs.A
- [3] Input 1 (Scalar) output 0 -> Lerp.A
- [4] Input 2 (Scalar) output 0 -> Lerp.B
- [9] Clamp output 0 -> Lerp.Alpha
- [1] Lerp output 0 -> Lerp.A
- [5] Input 3 (Scalar) output 0 -> Lerp.B
- [11] Clamp output 0 -> Lerp.Alpha
- [22] TimeWithSpeedVariable output 0 -> Input A (Scalar).Preview
- [7] Input A (Scalar) output 0 -> Multiply(,2).A
- [8] Multiply(,2) output 0 -> Clamp.Input
- [8] Multiply(,2) output 0 -> Subtract(,1).A
- [10] Subtract(,1) output 0 -> Clamp.Input
- [3] Input 1 (Scalar) output 0 -> Lerp.A
- [4] Input 2 (Scalar) output 0 -> Lerp.B
- [16] Clamp output 0 -> Lerp.Alpha
- [12] Lerp output 0 -> Lerp.A
- [5] Input 3 (Scalar) output 0 -> Lerp.B
- [18] Clamp output 0 -> Lerp.Alpha
- [13] Lerp output 0 -> Lerp.A
- [6] Input 4 (Scalar) output 0 -> Lerp.B
- [20] Clamp output 0 -> Lerp.Alpha
- [7] Input A (Scalar) output 0 -> Multiply(,3).A
- [15] Multiply(,3) output 0 -> Clamp.Input
- [15] Multiply(,3) output 0 -> Subtract(,1).A
- [17] Subtract(,1) output 0 -> Clamp.Input
- [17] Subtract(,1) output 0 -> Subtract(,1).A
- [19] Subtract(,1) output 0 -> Clamp.Input
- [14] Lerp output 0 -> Output Lerp 4 Inputs.A

## Output Trace Roots

- `Lerp 3 Inputs` is driven by [2] Lerp
- `Lerp 4 Inputs` is driven by [14] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


