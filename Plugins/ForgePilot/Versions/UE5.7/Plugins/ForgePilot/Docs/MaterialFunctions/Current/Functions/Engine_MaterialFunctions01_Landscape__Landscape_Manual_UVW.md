# Landscape_Manual_UVW

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Landscape/Landscape_Manual_UVW.Landscape_Manual_UVW`
- Category: Engine_MaterialFunctions01/Landscape
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Landscape Manual UVW Defined by Min and Max positions

## Inputs

- `Max Pos` [FunctionInput_Vector3]
- `Min Pos` [FunctionInput_Vector3]
- `Position` [FunctionInput_Vector3]

## Outputs

- `UVW`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVW | UVW |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Max Pos (Vector3) | Max Pos |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Min Pos (Vector3) | Min Pos |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionClamp` | Clamp |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Position (Vector3) | Position |

## Exact Input Wiring

### [0] Output UVW

- `A` <= [7] Clamp output 0

### [2] Subtract

- `A` <= [8] Input Position (Vector3) output 0
- `B` <= [4] Input Min Pos (Vector3) output 0

### [5] Divide

- `A` <= [2] Subtract output 0
- `B` <= [6] Subtract output 0

### [6] Subtract

- `A` <= [3] Input Max Pos (Vector3) output 0
- `B` <= [4] Input Min Pos (Vector3) output 0

### [7] Clamp

- `Input` <= [5] Divide output 0

### [8] Input Position (Vector3)

- `Preview` <= [1] Absolute World Position output 0

## Connection List

- [7] Clamp output 0 -> Output UVW.A
- [8] Input Position (Vector3) output 0 -> Subtract.A
- [4] Input Min Pos (Vector3) output 0 -> Subtract.B
- [2] Subtract output 0 -> Divide.A
- [6] Subtract output 0 -> Divide.B
- [3] Input Max Pos (Vector3) output 0 -> Subtract.A
- [4] Input Min Pos (Vector3) output 0 -> Subtract.B
- [5] Divide output 0 -> Clamp.Input
- [1] Absolute World Position output 0 -> Input Position (Vector3).Preview

## Output Trace Roots

- `UVW` is driven by [7] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


