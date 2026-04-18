# ComponentPivotLocation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ComponentPivotLocation.ComponentPivotLocation`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the world position of the components origin.

## Inputs

- None

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 2 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [2] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [1] 0,0,0 output 0

## Connection List

- [2] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Result.A
- [1] 0,0,0 output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input

## Output Trace Roots

- `Result` is driven by [2] Instance & Particle Space to Absolute World Space TransformPosition

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
