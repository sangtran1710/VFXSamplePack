# DebugOnOff

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugOnOff.DebugOnOff`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Useful for doing AB comparrison in material

## Inputs

- None

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTime` | Time |  |
| 2 | expression | `MaterialExpressionSine` | Sine |  |
| 3 | expression | `MaterialExpressionCeil` | Ceil |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Clamp output 0

### [2] Sine

- `Input` <= [1] Time output 0

### [3] Ceil

- `Input` <= [2] Sine output 0

### [4] Clamp

- `Input` <= [3] Ceil output 0

## Connection List

- [4] Clamp output 0 -> Output Result.A
- [1] Time output 0 -> Sine.Input
- [2] Sine output 0 -> Ceil.Input
- [3] Ceil output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [4] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


