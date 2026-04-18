# PivotAxis

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/PivotAxis.PivotAxis`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Creates a common pivot location on arbitrary axes. This is helpful for creating flag motion. Instead of using a single pivot point near the top of the flag, use a shared z point and unique xy location data to create a more realistic connection along the object's width. Note: this shader node only supports uniform scaling. Also, the rotate about axis axis and the pivot axis/pos should not coincide.

## Inputs

- `Pivot Axis/Pos` [FunctionInput_Vector3] - Enter a number to use as your both your local axis line axis and position. If you would like to lock off your model enter on it's local z axis near its pivot enter (0,0,1). If you would like to lock of the top of the model enter your model's height in as (0,0,mod6el height).

## Outputs

- `Pivot` - This can be used as the pivot point in the rotate about axis node.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 1 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 2 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Pivot Axis/Pos (Vector3) | Pivot Axis/Pos |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Pivot | Pivot |

## Exact Input Wiring

### [1] Subtract

- `A` <= [0] Absolute World Position output 0
- `B` <= [5] Multiply output 0

### [2] Normalize

- `VectorInput` <= [11] Input Pivot Axis/Pos (Vector3) output 0

### [3] Divide

- `A` <= [4] Instance & Particle Space to World Space TransformVector output 0
- `B` <= [6] ObjectScale output 0

### [4] Instance & Particle Space to World Space TransformVector

- `Input` <= [2] Normalize output 0

### [5] Multiply

- `A` <= [3] Divide output 0
- `B` <= [7] Dot output 0

### [7] Dot

- `A` <= [3] Divide output 0
- `B` <= [9] Subtract output 0

### [9] Subtract

- `A` <= [8] Absolute World Position output 0
- `B` <= [10] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [10] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [11] Input Pivot Axis/Pos (Vector3) output 0

### [12] Output Pivot

- `A` <= [1] Subtract output 0

## Connection List

- [0] Absolute World Position output 0 -> Subtract.A
- [5] Multiply output 0 -> Subtract.B
- [11] Input Pivot Axis/Pos (Vector3) output 0 -> Normalize.VectorInput
- [4] Instance & Particle Space to World Space TransformVector output 0 -> Divide.A
- [6] ObjectScale output 0 -> Divide.B
- [2] Normalize output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [3] Divide output 0 -> Multiply.A
- [7] Dot output 0 -> Multiply.B
- [3] Divide output 0 -> Dot.A
- [9] Subtract output 0 -> Dot.B
- [8] Absolute World Position output 0 -> Subtract.A
- [10] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Subtract.B
- [11] Input Pivot Axis/Pos (Vector3) output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [1] Subtract output 0 -> Output Pivot.A

## Output Trace Roots

- `Pivot` is driven by [1] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


