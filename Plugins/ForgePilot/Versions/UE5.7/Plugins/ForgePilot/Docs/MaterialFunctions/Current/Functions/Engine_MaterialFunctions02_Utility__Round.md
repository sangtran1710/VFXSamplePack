# Round

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Round.Round`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Rounds a number to the next closest whole number

## Inputs

- `In` [FunctionInput_Scalar] - Rounds to the nearest whole number.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 2 | expression | `MaterialExpressionFrac` | Frac |  |
| 3 | expression | `MaterialExpressionFloor` | Floor |  |
| 4 | expression | `MaterialExpressionCeil` | Ceil |  |
| 5 | expression | `MaterialExpressionIf` | If |  |
| 6 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | Sign |  |
| 8 | expression | `MaterialExpressionAbs` | Abs |  |
| 9 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 10 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [5] If output 0

### [2] Frac

- `Input` <= [1] Input In (Scalar) output 0

### [3] Floor

- `Input` <= [1] Input In (Scalar) output 0

### [4] Ceil

- `Input` <= [1] Input In (Scalar) output 0

### [5] If

- `A` <= [8] Abs output 0
- `B` <= [6] 0.5 output 0
- `AGreaterThanB` <= [4] Ceil output 0
- `AEqualsB` <= [9] Lerp output 0
- `ALessThanB` <= [3] Floor output 0

### [8] Abs

- `Input` <= [2] Frac output 0

### [9] Lerp

- `A` <= [4] Ceil output 0
- `B` <= [3] Floor output 0
- `Alpha` <= [10] ConstantBiasScale output 0

### [10] ConstantBiasScale

- `Input` <= [7] Sign output 0

## Connection List

- [5] If output 0 -> Output Result.A
- [1] Input In (Scalar) output 0 -> Frac.Input
- [1] Input In (Scalar) output 0 -> Floor.Input
- [1] Input In (Scalar) output 0 -> Ceil.Input
- [8] Abs output 0 -> If.A
- [6] 0.5 output 0 -> If.B
- [4] Ceil output 0 -> If.AGreaterThanB
- [9] Lerp output 0 -> If.AEqualsB
- [3] Floor output 0 -> If.ALessThanB
- [2] Frac output 0 -> Abs.Input
- [4] Ceil output 0 -> Lerp.A
- [3] Floor output 0 -> Lerp.B
- [10] ConstantBiasScale output 0 -> Lerp.Alpha
- [7] Sign output 0 -> ConstantBiasScale.Input

## Output Trace Roots

- `Result` is driven by [5] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


