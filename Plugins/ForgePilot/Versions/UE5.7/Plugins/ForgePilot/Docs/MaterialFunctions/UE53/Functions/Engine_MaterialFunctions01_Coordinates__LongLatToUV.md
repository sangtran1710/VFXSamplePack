# LongLatToUV

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/LongLatToUV.LongLatToUV`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This node transforms a directional vector into a UV value for use in Long/Lat coordinates

## Inputs

- `Vector` [FunctionInput_Vector3] - The vector to transform into a UV value.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 3 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Custom output 0

### [3] Normalize

- `VectorInput` <= [2] Input Vector (Vector3) output 0

## Connection List

- [1] Custom output 0 -> Output Result.A
- [2] Input Vector (Vector3) output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Result` is driven by [1] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
