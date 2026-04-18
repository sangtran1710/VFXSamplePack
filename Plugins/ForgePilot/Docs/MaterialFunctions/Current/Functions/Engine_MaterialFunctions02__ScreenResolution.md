# ScreenResolution

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ScreenResolution.ScreenResolution`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Retrieves the visible screen resolution and the render target size.

## Inputs

- None

## Outputs

- `Visible Resolution`
- `Buffer Resolution`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Visible Resolution | Visible Resolution |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Buffer Resolution | Buffer Resolution |
| 2 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 3 | expression | `MaterialExpressionViewProperty` | Render Target Size |  |

## Exact Input Wiring

### [0] Output Visible Resolution

- `A` <= [2] ViewSize output 0

### [1] Output Buffer Resolution

- `A` <= [3] Render Target Size output 0

## Connection List

- [2] ViewSize output 0 -> Output Visible Resolution.A
- [3] Render Target Size output 0 -> Output Buffer Resolution.A

## Output Trace Roots

- `Visible Resolution` is driven by [2] ViewSize
- `Buffer Resolution` is driven by [3] Render Target Size

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


