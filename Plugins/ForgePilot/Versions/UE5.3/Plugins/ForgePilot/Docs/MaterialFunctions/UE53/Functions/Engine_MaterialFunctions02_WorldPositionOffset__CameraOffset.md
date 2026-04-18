# CameraOffset

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/CameraOffset.CameraOffset`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Offset objects in the direction of the camera to either make them appear in front or behind other objects. 

## Inputs

- `Offset Amount` [FunctionInput_Scalar] - Enter a negative or positive number to offset your model in the direction of the camera. Note that positive values will bring the model closer to the camera and cause rendering errors if the mesh greatly exceeds the models bounding box. 
- `World Position` [FunctionInput_Vector3] - Enter the model's vertex world position. Default Value = World Position. (Note: this used to default to actor position but world position is more useful and mimics the particle effects camera offset.)
- `Clamp Padding` [FunctionInput_Scalar] - Amount of padding to prevent the clamped version of the offset value from clipping into the camera. 

## Outputs

- `Camera Offset` - Either add this value to other world position offset code or enter it directily intro the world position offset master material nub to offset your mesh.
- `Clamped Camera Offset` - Camera offset clamped to avoid early camera intersections. Adjust the padding to change the amount of space reserved to avoid camera intersections. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Camera Offset | Camera Offset |
| 1 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Offset Amount (Scalar) | Offset Amount |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 6 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 7 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Clamped Camera Offset | Clamped Camera Offset |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionDistance` | Distance |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | expression | `MaterialExpressionClamp` | Clamp |  |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Clamp Padding (Scalar) | Clamp Padding |

## Exact Input Wiring

### [0] Output Camera Offset

- `A` <= [3] Multiply output 0

### [1] Normalize

- `VectorInput` <= [2] Subtract output 0

### [2] Subtract

- `A` <= [6] Camera Position output 0
- `B` <= [5] Input World Position (Vector3) output 0

### [3] Multiply

- `A` <= [1] Normalize output 0
- `B` <= [4] Input Offset Amount (Scalar) output 0

### [5] Input World Position (Vector3)

- `Preview` <= [7] Absolute World Position output 0

### [8] Output Clamped Camera Offset

- `A` <= [9] Multiply output 0

### [9] Multiply

- `A` <= [1] Normalize output 0
- `B` <= [12] Clamp output 0

### [10] Distance

- `A` <= [2] Subtract output 0
- `B` <= [11] 0 output 0

### [12] Clamp

- `Input` <= [4] Input Offset Amount (Scalar) output 0
- `Max` <= [13] Subtract output 0

### [13] Subtract

- `A` <= [10] Distance output 0
- `B` <= [14] Input Clamp Padding (Scalar) output 0

## Connection List

- [3] Multiply output 0 -> Output Camera Offset.A
- [2] Subtract output 0 -> Normalize.VectorInput
- [6] Camera Position output 0 -> Subtract.A
- [5] Input World Position (Vector3) output 0 -> Subtract.B
- [1] Normalize output 0 -> Multiply.A
- [4] Input Offset Amount (Scalar) output 0 -> Multiply.B
- [7] Absolute World Position output 0 -> Input World Position (Vector3).Preview
- [9] Multiply output 0 -> Output Clamped Camera Offset.A
- [1] Normalize output 0 -> Multiply.A
- [12] Clamp output 0 -> Multiply.B
- [2] Subtract output 0 -> Distance.A
- [11] 0 output 0 -> Distance.B
- [4] Input Offset Amount (Scalar) output 0 -> Clamp.Input
- [13] Subtract output 0 -> Clamp.Max
- [10] Distance output 0 -> Subtract.A
- [14] Input Clamp Padding (Scalar) output 0 -> Subtract.B

## Output Trace Roots

- `Camera Offset` is driven by [3] Multiply
- `Clamped Camera Offset` is driven by [9] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
