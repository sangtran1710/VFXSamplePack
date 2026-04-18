# CenterPivotAroundVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/CenterPivotAroundVector.CenterPivotAroundVector`
- Category: Engine_MaterialFunctions02
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns world position for every vertex as it would be projected against a vector defined by the vector user input. This can be used as the pivot point in a rotate about axis node.

## Inputs

- `Vector` [FunctionInput_Vector3]

## Outputs

- `WorldPosition`
- `LocalPosition`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output WorldPosition | WorldPosition |
| 1 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 2 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 5 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output LocalPosition | LocalPosition |

## Exact Input Wiring

### [0] Output WorldPosition

- `A` <= [7] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [2] Normalize

- `VectorInput` <= [1] 0,0,1 output 0

### [3] Dot

- `A` <= [9] Normalize output 0
- `B` <= [4] Absolute World Space to Instance & Particle Space TransformPosition output 0

### [4] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [5] Absolute World Position output 0

### [6] Multiply

- `A` <= [9] Normalize output 0
- `B` <= [3] Dot output 0

### [7] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [6] Multiply output 0

### [8] Input Vector (Vector3)

- `Preview` <= [2] Normalize output 0

### [9] Normalize

- `VectorInput` <= [8] Input Vector (Vector3) output 0

### [10] Output LocalPosition

- `A` <= [6] Multiply output 0

## Connection List

- [7] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output WorldPosition.A
- [1] 0,0,1 output 0 -> Normalize.VectorInput
- [9] Normalize output 0 -> Dot.A
- [4] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Dot.B
- [5] Absolute World Position output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [9] Normalize output 0 -> Multiply.A
- [3] Dot output 0 -> Multiply.B
- [6] Multiply output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [2] Normalize output 0 -> Input Vector (Vector3).Preview
- [8] Input Vector (Vector3) output 0 -> Normalize.VectorInput
- [6] Multiply output 0 -> Output LocalPosition.A

## Output Trace Roots

- `WorldPosition` is driven by [7] Instance & Particle Space to Absolute World Space TransformPosition
- `LocalPosition` is driven by [6] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
