# ProtectFrom0

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/ProtectFrom0.ProtectFrom0`
- Category: Engine_MaterialFunctions02/PivotPainter
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `In` [FunctionInput_Scalar]

## Outputs

- `RGB`
- `R`
- `G`
- `B`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 4 | expression | `MaterialExpressionConstant` | 1e-05 |  |
| 5 | expression | `MaterialExpressionIf` | If |  |
| 6 | expression | `MaterialExpressionConstant` | 0 |  |
| 7 | expression | `MaterialExpressionIf` | If |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionAppendVector` | Append |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output R | R |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output G | G |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output B | B |

## Exact Input Wiring

### [0] Output RGB

- `A` <= [9] Append output 0

### [1] If

- `A` <= [3] BreakOutFloat3Components output 0
- `B` <= [6] 0 output 0
- `AGreaterThanB` <= [3] BreakOutFloat3Components output 0
- `AEqualsB` <= [4] 1e-05 output 0
- `ALessThanB` <= [3] BreakOutFloat3Components output 0

### [5] If

- `A` <= [3] BreakOutFloat3Components output 1
- `B` <= [6] 0 output 0
- `AGreaterThanB` <= [3] BreakOutFloat3Components output 1
- `AEqualsB` <= [4] 1e-05 output 0
- `ALessThanB` <= [3] BreakOutFloat3Components output 1

### [7] If

- `A` <= [3] BreakOutFloat3Components output 2
- `B` <= [6] 0 output 0
- `AGreaterThanB` <= [3] BreakOutFloat3Components output 2
- `AEqualsB` <= [4] 1e-05 output 0
- `ALessThanB` <= [3] BreakOutFloat3Components output 2

### [8] Append

- `A` <= [1] If output 0
- `B` <= [5] If output 0

### [9] Append

- `A` <= [8] Append output 0
- `B` <= [7] If output 0

### [10] Output R

- `A` <= [1] If output 0

### [11] Output G

- `A` <= [5] If output 0

### [12] Output B

- `A` <= [7] If output 0

## Connection List

- [9] Append output 0 -> Output RGB.A
- [3] BreakOutFloat3Components output 0 -> If.A
- [6] 0 output 0 -> If.B
- [3] BreakOutFloat3Components output 0 -> If.AGreaterThanB
- [4] 1e-05 output 0 -> If.AEqualsB
- [3] BreakOutFloat3Components output 0 -> If.ALessThanB
- [3] BreakOutFloat3Components output 1 -> If.A
- [6] 0 output 0 -> If.B
- [3] BreakOutFloat3Components output 1 -> If.AGreaterThanB
- [4] 1e-05 output 0 -> If.AEqualsB
- [3] BreakOutFloat3Components output 1 -> If.ALessThanB
- [3] BreakOutFloat3Components output 2 -> If.A
- [6] 0 output 0 -> If.B
- [3] BreakOutFloat3Components output 2 -> If.AGreaterThanB
- [4] 1e-05 output 0 -> If.AEqualsB
- [3] BreakOutFloat3Components output 2 -> If.ALessThanB
- [1] If output 0 -> Append.A
- [5] If output 0 -> Append.B
- [8] Append output 0 -> Append.A
- [7] If output 0 -> Append.B
- [1] If output 0 -> Output R.A
- [5] If output 0 -> Output G.A
- [7] If output 0 -> Output B.A

## Output Trace Roots

- `RGB` is driven by [9] Append
- `R` is driven by [1] If
- `G` is driven by [5] If
- `B` is driven by [7] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
