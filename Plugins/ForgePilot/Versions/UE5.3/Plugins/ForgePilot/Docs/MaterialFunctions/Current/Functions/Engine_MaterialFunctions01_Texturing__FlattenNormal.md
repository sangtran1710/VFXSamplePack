# FlattenNormal

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/FlattenNormal.FlattenNormal`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Lerps a NormalMap with 0,0,1

## Inputs

- `Normal` [FunctionInput_Vector3]
- `Flatness` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Flatness (Scalar) | Flatness |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Lerp output 0

### [2] Lerp

- `A` <= [1] Input Normal (Vector3) output 0
- `B` <= [3] 0,0,1 output 0
- `Alpha` <= [4] Input Flatness (Scalar) output 0

## Connection List

- [2] Lerp output 0 -> Output Result.A
- [1] Input Normal (Vector3) output 0 -> Lerp.A
- [3] 0,0,1 output 0 -> Lerp.B
- [4] Input Flatness (Scalar) output 0 -> Lerp.Alpha

## Output Trace Roots

- `Result` is driven by [2] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


