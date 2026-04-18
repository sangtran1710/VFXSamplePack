# ms_PivotPainter2_DecodePostion

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_DecodePostion.ms_PivotPainter2_DecodePostion`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Transforms Pivot Painter 2.0's local space position information into world position info.

## Inputs

- `Position RGB` [FunctionInput_Vector3] - Insert the RGB values of a texture that contain Pivot Painter 2 “Pivot Position (16-bit)” data.

## Outputs

- `Result` - The output value is the world space location of each model’s pivot point position as it was captured by Pivot Painter.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Position RGB (Vector3) | Position RGB |
| 2 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [2] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [1] Input Position RGB (Vector3) output 0

## Connection List

- [2] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Result.A
- [1] Input Position RGB (Vector3) output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input

## Output Trace Roots

- `Result` is driven by [2] Instance & Particle Space to Absolute World Space TransformPosition

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
