# CameraDirectionVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/CameraDirectionVector.CameraDirectionVector`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

World space vector of the camera direction. 

## Inputs

- None

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] View Space to World Space TransformVector output 0

### [1] View Space to World Space TransformVector

- `Input` <= [2] 0,0,1 output 0

## Connection List

- [1] View Space to World Space TransformVector output 0 -> Output Result.A
- [2] 0,0,1 output 0 -> View Space to World Space TransformVector.Input

## Output Trace Roots

- `Result` is driven by [1] View Space to World Space TransformVector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
