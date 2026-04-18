# DistanceField

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/DistanceField.DistanceField`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Reads a Distance field texture

## Inputs

- `EdgeSoftness` [FunctionInput_Scalar]
- `DistanceField` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 3 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input EdgeSoftness (Scalar) | EdgeSoftness |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input DistanceField (Scalar) | DistanceField |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Custom output 0

### [5] Subtract

- `A` <= [2] 0.5 output 0
- `B` <= [4] Input EdgeSoftness (Scalar) output 0

### [6] Add

- `A` <= [4] Input EdgeSoftness (Scalar) output 0
- `B` <= [3] 0.5 output 0

## Connection List

- [1] Custom output 0 -> Output Result.A
- [2] 0.5 output 0 -> Subtract.A
- [4] Input EdgeSoftness (Scalar) output 0 -> Subtract.B
- [4] Input EdgeSoftness (Scalar) output 0 -> Add.A
- [3] 0.5 output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [1] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
