# SmoothCeil

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/SmoothCeil.SmoothCeil`
- Category: Engine_MaterialFunctions01/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Smooth ceil is like ceil but gives a smooth ramp before each integer value.

## Inputs

- `In` [FunctionInput_Scalar] - The input value to create a smooth ceil for.
- `Ramp Width` [FunctionInput_Scalar] - The with of the smooth ramp before each integer. Values over 2 will start to have discontinuities.

## Outputs

- `Result` - The smooth ceil result.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 2 | expression | `MaterialExpressionMax` | Max |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 8 | expression | `MaterialExpressionCosine` | Cosine |  |
| 9 | expression | `MaterialExpressionFrac` | Frac |  |
| 10 | expression | `MaterialExpressionCeil` | Ceil |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Ramp Width (Scalar) | Ramp Width |
| 13 | expression | `MaterialExpressionConstant2Vector` | 0,0 |  |
| 14 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 15 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 16 | expression | `MaterialExpressionConstant2Vector` | 0,0 |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [6] Add output 0

### [2] Max

- `A` <= [3] Divide output 0
- `B` <= [1] 0.5 output 0

### [3] Divide

- `A` <= [4] Subtract output 0
- `B` <= [12] Input Ramp Width (Scalar) output 0

### [4] Subtract

- `A` <= [9] Frac output 0
- `B` <= [5] 1-x output 0

### [5] 1-x

- `Input` <= [12] Input Ramp Width (Scalar) output 0

### [6] Add

- `A` <= [10] Ceil output 0
- `B` <= [7] ConstantBiasScale output 0

### [7] ConstantBiasScale

- `Input` <= [8] Cosine output 0

### [8] Cosine

- `Input` <= [2] Max output 0

### [9] Frac

- `Input` <= [11] Input In (Scalar) output 0

### [10] Ceil

- `Input` <= [11] Input In (Scalar) output 0

### [17] Multiply

- `A` <= [14] TexCoord[0] output 0
- `B` <= [13] 0,0 output 0

## Connection List

- [6] Add output 0 -> Output Result.A
- [3] Divide output 0 -> Max.A
- [1] 0.5 output 0 -> Max.B
- [4] Subtract output 0 -> Divide.A
- [12] Input Ramp Width (Scalar) output 0 -> Divide.B
- [9] Frac output 0 -> Subtract.A
- [5] 1-x output 0 -> Subtract.B
- [12] Input Ramp Width (Scalar) output 0 -> 1-x.Input
- [10] Ceil output 0 -> Add.A
- [7] ConstantBiasScale output 0 -> Add.B
- [8] Cosine output 0 -> ConstantBiasScale.Input
- [2] Max output 0 -> Cosine.Input
- [11] Input In (Scalar) output 0 -> Frac.Input
- [11] Input In (Scalar) output 0 -> Ceil.Input
- [14] TexCoord[0] output 0 -> Multiply.A
- [13] 0,0 output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [6] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


