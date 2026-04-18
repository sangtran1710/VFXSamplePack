# ObjectPivotPoint

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ObjectPivotPoint.ObjectPivotPoint`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the object's pivot point in world space. This is not compatible with the pixel shader.

## Inputs

- None

## Outputs

- `Object Pivot Location` - Returns the object's pivot point in world space. This only works with the vertex shader.
- `Mesh Particle Pivot Location` - Returns the object's pivot point in world space. This only works with the vertex shader.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Object Pivot Location | Object Pivot Location |
| 1 | expression | `MaterialExpressionTransformPosition` | Local Space to Absolute World Space TransformPosition |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 3 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output Mesh Particle Pivot Location | Mesh Particle Pivot Location |

## Exact Input Wiring

### [0] Output Object Pivot Location

- `A` <= [1] Local Space to Absolute World Space TransformPosition output 0

### [1] Local Space to Absolute World Space TransformPosition

- `Input` <= [2] 0,0,0 output 0

### [3] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [2] 0,0,0 output 0

### [4] Output Mesh Particle Pivot Location

- `A` <= [3] Instance & Particle Space to Absolute World Space TransformPosition output 0

## Connection List

- [1] Local Space to Absolute World Space TransformPosition output 0 -> Output Object Pivot Location.A
- [2] 0,0,0 output 0 -> Local Space to Absolute World Space TransformPosition.Input
- [2] 0,0,0 output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [3] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Mesh Particle Pivot Location.A

## Output Trace Roots

- `Object Pivot Location` is driven by [1] Local Space to Absolute World Space TransformPosition
- `Mesh Particle Pivot Location` is driven by [3] Instance & Particle Space to Absolute World Space TransformPosition

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


