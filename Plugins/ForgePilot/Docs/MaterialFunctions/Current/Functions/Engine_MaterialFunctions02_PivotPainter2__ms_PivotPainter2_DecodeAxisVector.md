# ms_PivotPainter2_DecodeAxisVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_DecodeAxisVector.ms_PivotPainter2_DecodeAxisVector`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Transforms Pivot Painter 2.0's local space vector info into world space vectors.

## Inputs

- `Axis Vector RGB` [FunctionInput_Vector3] - Input RGB vector information from a Pivot Painter 2 texture that output those values.

## Outputs

- `Result` - The input axis vector information has now been transformed to world space.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Axis Vector RGB (Vector3) | Axis Vector RGB |
| 2 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 3 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Normalize output 0

### [2] ConstantBiasScale

- `Input` <= [1] Input Axis Vector RGB (Vector3) output 0

### [3] Instance & Particle Space to World Space TransformVector

- `Input` <= [2] ConstantBiasScale output 0

### [4] Normalize

- `VectorInput` <= [3] Instance & Particle Space to World Space TransformVector output 0

## Connection List

- [4] Normalize output 0 -> Output Result.A
- [1] Input Axis Vector RGB (Vector3) output 0 -> ConstantBiasScale.Input
- [2] ConstantBiasScale output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [3] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Result` is driven by [4] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


