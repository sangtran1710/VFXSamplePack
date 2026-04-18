# AlphaOffset

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/AlphaOffset.AlphaOffset`
- Category: Engine_MaterialFunctions01/Opacity
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Applies an offset to alpha while keeping the range 0 to 1. Useful for blurred alphas that need to shrink.

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `Offset` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Scalar) | Offset |
| 4 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [6] Clamp output 0

### [2] Subtract

- `A` <= [1] Input Alpha (Scalar) output 0
- `B` <= [3] Input Offset (Scalar) output 0

### [4] 1-x

- `Input` <= [3] Input Offset (Scalar) output 0

### [5] Divide

- `A` <= [2] Subtract output 0
- `B` <= [4] 1-x output 0

### [6] Clamp

- `Input` <= [5] Divide output 0

## Connection List

- [6] Clamp output 0 -> Output Result.A
- [1] Input Alpha (Scalar) output 0 -> Subtract.A
- [3] Input Offset (Scalar) output 0 -> Subtract.B
- [3] Input Offset (Scalar) output 0 -> 1-x.Input
- [2] Subtract output 0 -> Divide.A
- [4] 1-x output 0 -> Divide.B
- [5] Divide output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [6] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
