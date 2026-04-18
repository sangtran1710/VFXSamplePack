# FindSaturation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/FindSaturation.FindSaturation`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Finds the saturation of a color.

## Inputs

- `Color` [FunctionInput_Vector3]

## Outputs

- `Saturation`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Saturation | Saturation |
| 1 | expression | `MaterialExpressionMin` | Min |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 3 | expression | `MaterialExpressionMin` | Min |  |
| 4 | expression | `MaterialExpressionMax` | Max |  |
| 5 | expression | `MaterialExpressionMax` | Max |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionIf` | If |  |
| 10 | expression | `MaterialExpressionConstant` | 0 |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |

## Exact Input Wiring

### [0] Output Saturation

- `A` <= [9] If output 0

### [1] Min

- `A` <= [2] BreakOutFloat3Components output 0
- `B` <= [2] BreakOutFloat3Components output 1

### [3] Min

- `A` <= [1] Min output 0
- `B` <= [2] BreakOutFloat3Components output 2

### [4] Max

- `A` <= [2] BreakOutFloat3Components output 0
- `B` <= [2] BreakOutFloat3Components output 1

### [5] Max

- `A` <= [4] Max output 0
- `B` <= [2] BreakOutFloat3Components output 2

### [6] Subtract

- `A` <= [5] Max output 0
- `B` <= [3] Min output 0

### [7] Divide

- `A` <= [6] Subtract output 0
- `B` <= [8] Add output 0

### [8] Add

- `A` <= [3] Min output 0
- `B` <= [5] Max output 0

### [9] If

- `A` <= [8] Add output 0
- `B` <= [10] 0 output 0
- `AGreaterThanB` <= [7] Divide output 0
- `AEqualsB` <= [11] 0 output 0
- `ALessThanB` <= [7] Divide output 0

## Connection List

- [9] If output 0 -> Output Saturation.A
- [2] BreakOutFloat3Components output 0 -> Min.A
- [2] BreakOutFloat3Components output 1 -> Min.B
- [1] Min output 0 -> Min.A
- [2] BreakOutFloat3Components output 2 -> Min.B
- [2] BreakOutFloat3Components output 0 -> Max.A
- [2] BreakOutFloat3Components output 1 -> Max.B
- [4] Max output 0 -> Max.A
- [2] BreakOutFloat3Components output 2 -> Max.B
- [5] Max output 0 -> Subtract.A
- [3] Min output 0 -> Subtract.B
- [6] Subtract output 0 -> Divide.A
- [8] Add output 0 -> Divide.B
- [3] Min output 0 -> Add.A
- [5] Max output 0 -> Add.B
- [8] Add output 0 -> If.A
- [10] 0 output 0 -> If.B
- [7] Divide output 0 -> If.AGreaterThanB
- [11] 0 output 0 -> If.AEqualsB
- [7] Divide output 0 -> If.ALessThanB

## Output Trace Roots

- `Saturation` is driven by [9] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
