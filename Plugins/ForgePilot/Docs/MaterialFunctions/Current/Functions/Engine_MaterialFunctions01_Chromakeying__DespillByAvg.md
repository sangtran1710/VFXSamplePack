# DespillByAvg

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DespillByAvg.DespillByAvg`
- Category: Engine_MaterialFunctions01/Chromakeying
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Despill using average color of input

## Inputs

- `Image Color` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionDivide` | Divide(,2) |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Image Color (Vector3) | Image Color |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | SplitComponents |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [6] MakeFloat3 output 0

### [1] If

- `A` <= [5] SplitComponents output 2
- `B` <= [3] Divide(,2) output 0
- `AGreaterThanB` <= [3] Divide(,2) output 0
- `AEqualsB` <= [5] SplitComponents output 2
- `ALessThanB` <= [5] SplitComponents output 2

### [2] Add

- `A` <= [5] SplitComponents output 1
- `B` <= [5] SplitComponents output 3

### [3] Divide(,2)

- `A` <= [2] Add output 0

## Connection List

- [6] MakeFloat3 output 0 -> Output Result.A
- [5] SplitComponents output 2 -> If.A
- [3] Divide(,2) output 0 -> If.B
- [3] Divide(,2) output 0 -> If.AGreaterThanB
- [5] SplitComponents output 2 -> If.AEqualsB
- [5] SplitComponents output 2 -> If.ALessThanB
- [5] SplitComponents output 1 -> Add.A
- [5] SplitComponents output 3 -> Add.B
- [2] Add output 0 -> Divide(,2).A

## Output Trace Roots

- `Result` is driven by [6] MakeFloat3

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


