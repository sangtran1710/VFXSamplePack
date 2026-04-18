# LocalPosition

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/LocalPosition.LocalPosition`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- None

## Outputs

- `Local Position`
- `Local Position (Excluding Offsets)`
- `Instance Local Position (Excluding Offsets)`
- `Instance Local Position`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Local Position | Local Position |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output Local Position (Excluding Offsets) | Local Position (Excluding Offsets) |
| 4 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Local Space TransformPosition |  |
| 5 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Local Space TransformPosition |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Instance Local Position (Excluding Offsets) | Instance Local Position (Excluding Offsets) |
| 7 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Instance Local Position | Instance Local Position |
| 9 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |

## Exact Input Wiring

### [0] Output Local Position

- `A` <= [4] Absolute World Space to Local Space TransformPosition output 0

### [3] Output Local Position (Excluding Offsets)

- `A` <= [5] Absolute World Space to Local Space TransformPosition output 0

### [4] Absolute World Space to Local Space TransformPosition

- `Input` <= [1] Absolute World Position output 0

### [5] Absolute World Space to Local Space TransformPosition

- `Input` <= [2] Absolute World Position (Excluding Material Offsets) output 0

### [6] Output Instance Local Position (Excluding Offsets)

- `A` <= [7] Absolute World Space to Instance & Particle Space TransformPosition output 0

### [7] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [2] Absolute World Position (Excluding Material Offsets) output 0

### [8] Output Instance Local Position

- `A` <= [9] Absolute World Space to Instance & Particle Space TransformPosition output 0

### [9] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [1] Absolute World Position output 0

## Connection List

- [4] Absolute World Space to Local Space TransformPosition output 0 -> Output Local Position.A
- [5] Absolute World Space to Local Space TransformPosition output 0 -> Output Local Position (Excluding Offsets).A
- [1] Absolute World Position output 0 -> Absolute World Space to Local Space TransformPosition.Input
- [2] Absolute World Position (Excluding Material Offsets) output 0 -> Absolute World Space to Local Space TransformPosition.Input
- [7] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Output Instance Local Position (Excluding Offsets).A
- [2] Absolute World Position (Excluding Material Offsets) output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [9] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Output Instance Local Position.A
- [1] Absolute World Position output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input

## Output Trace Roots

- `Local Position` is driven by [4] Absolute World Space to Local Space TransformPosition
- `Local Position (Excluding Offsets)` is driven by [5] Absolute World Space to Local Space TransformPosition
- `Instance Local Position (Excluding Offsets)` is driven by [7] Absolute World Space to Instance & Particle Space TransformPosition
- `Instance Local Position` is driven by [9] Absolute World Space to Instance & Particle Space TransformPosition

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
