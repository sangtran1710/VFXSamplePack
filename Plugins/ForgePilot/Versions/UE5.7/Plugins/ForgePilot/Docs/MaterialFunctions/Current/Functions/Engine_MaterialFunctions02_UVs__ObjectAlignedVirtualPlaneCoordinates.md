# ObjectAlignedVirtualPlaneCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/ObjectAlignedVirtualPlaneCoordinates.ObjectAlignedVirtualPlaneCoordinates`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Draws 2d coordinate at a specified alignment and position.

## Inputs

- `Coordinate Scale (WS)` [FunctionInput_Scalar]
- `World Space Projection Normal` [FunctionInput_Vector3]
- `World Space Projection Tangent` [FunctionInput_Vector3]
- `World Space Projection Binormal` [FunctionInput_Vector3]
- `Projection Pivot Position` [FunctionInput_Vector3]
- `Plane Depth Offset` [FunctionInput_Scalar]

## Outputs

- `Center Aligned UV Plane`
- `Non Biased Coordinates`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Center Aligned UV Plane | Center Aligned UV Plane |
| 1 | expression | `MaterialExpressionDivide` | Divide |  |
| 2 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 3 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 6 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 7 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Coordinate Scale (WS) (Scalar) | Coordinate Scale (WS) |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input World Space Projection Normal (Vector3) | World Space Projection Normal |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input World Space Projection Tangent (Vector3) | World Space Projection Tangent |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input World Space Projection Binormal (Vector3) | World Space Projection Binormal |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Projection Pivot Position (Vector3) | Projection Pivot Position |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Plane Depth Offset (Scalar) | Plane Depth Offset |
| 22 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 23 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 24 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 25 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply(,10) |  |
| 27 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply(,10) |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Non Biased Coordinates | Non Biased Coordinates |
| 30 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 31 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 32 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 33 | expression | `MaterialExpressionConstant3Vector` | 0,0,-1 |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 35 | expression | `MaterialExpressionDivide` | Divide |  |
| 36 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 39 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 42 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 43 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 45 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 46 | expression | `MaterialExpressionDivide` | Divide |  |
| 47 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 48 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 49 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |

## Exact Input Wiring

### [0] Output Center Aligned UV Plane

- `A` <= [7] Add(,0.5) output 0

### [1] Divide

- `A` <= [10] Append output 0
- `B` <= [15] Input Coordinate Scale (WS) (Scalar) output 0

### [3] Instance & Particle Space to World Space TransformVector

- `Input` <= [22] 1,0,0 output 0

### [4] Normalize

- `VectorInput` <= [3] Instance & Particle Space to World Space TransformVector output 0

### [5] Instance & Particle Space to World Space TransformVector

- `Input` <= [23] 0,1,0 output 0

### [6] Normalize

- `VectorInput` <= [5] Instance & Particle Space to World Space TransformVector output 0

### [7] Add(,0.5)

- `A` <= [1] Divide output 0

### [8] Dot

- `A` <= [16] Subtract output 0
- `B` <= [18] Input World Space Projection Tangent (Vector3) output 0

### [9] Dot

- `A` <= [16] Subtract output 0
- `B` <= [19] Input World Space Projection Binormal (Vector3) output 0

### [10] Append

- `A` <= [8] Dot output 0
- `B` <= [9] Dot output 0

### [11] Instance & Particle Space to World Space TransformVector

- `Input` <= [24] 0,0,1 output 0

### [12] Normalize

- `VectorInput` <= [11] Instance & Particle Space to World Space TransformVector output 0

### [13] Dot

- `A` <= [20] Input Projection Pivot Position (Vector3) output 0
- `B` <= [17] Input World Space Projection Normal (Vector3) output 0

### [14] Add

- `A` <= [13] Dot output 0
- `B` <= [21] Input Plane Depth Offset (Scalar) output 0

### [16] Subtract

- `A` <= [40] Add output 0
- `B` <= [20] Input Projection Pivot Position (Vector3) output 0

### [17] Input World Space Projection Normal (Vector3)

- `Preview` <= [4] Normalize output 0

### [18] Input World Space Projection Tangent (Vector3)

- `Preview` <= [6] Normalize output 0

### [19] Input World Space Projection Binormal (Vector3)

- `Preview` <= [12] Normalize output 0

### [20] Input Projection Pivot Position (Vector3)

- `Preview` <= [2] Actor Position (Absolute) output 0

### [26] Multiply(,10)

- `A` <= [25] ObjectScale output 1

### [28] Multiply(,10)

- `A` <= [27] ObjectScale output 1

### [29] Output Non Biased Coordinates

- `A` <= [10] Append output 0

### [30] Dot

- `A` <= [32] View Space to World Space TransformVector output 0
- `B` <= [31] Camera Vector output 0

### [32] View Space to World Space TransformVector

- `Input` <= [33] 0,0,-1 output 0

### [34] Multiply(,1)

- `A` <= [48] Multiply output 0

### [35] Divide

- `A` <= [34] Multiply(,1) output 0
- `B` <= [36] PixelDepth output 0

### [37] Multiply

- `A` <= [35] Divide output 0
- `B` <= [39] Subtract output 0

### [39] Subtract

- `A` <= [38] Absolute World Position output 0
- `B` <= [41] Camera Position output 0

### [40] Add

- `A` <= [37] Multiply output 0
- `B` <= [41] Camera Position output 0

### [43] Subtract

- `A` <= [42] Camera Position output 0
- `B` <= [44] Multiply output 0

### [44] Multiply

- `A` <= [14] Add output 0
- `B` <= [17] Input World Space Projection Normal (Vector3) output 0

### [45] Dot

- `A` <= [17] Input World Space Projection Normal (Vector3) output 0
- `B` <= [43] Subtract output 0

### [46] Divide

- `A` <= [45] Dot output 0
- `B` <= [47] Dot output 0

### [47] Dot

- `A` <= [17] Input World Space Projection Normal (Vector3) output 0
- `B` <= [49] Camera Vector output 0

### [48] Multiply

- `A` <= [46] Divide output 0
- `B` <= [30] Dot output 0

## Connection List

- [7] Add(,0.5) output 0 -> Output Center Aligned UV Plane.A
- [10] Append output 0 -> Divide.A
- [15] Input Coordinate Scale (WS) (Scalar) output 0 -> Divide.B
- [22] 1,0,0 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [3] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [23] 0,1,0 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [5] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [1] Divide output 0 -> Add(,0.5).A
- [16] Subtract output 0 -> Dot.A
- [18] Input World Space Projection Tangent (Vector3) output 0 -> Dot.B
- [16] Subtract output 0 -> Dot.A
- [19] Input World Space Projection Binormal (Vector3) output 0 -> Dot.B
- [8] Dot output 0 -> Append.A
- [9] Dot output 0 -> Append.B
- [24] 0,0,1 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [11] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [20] Input Projection Pivot Position (Vector3) output 0 -> Dot.A
- [17] Input World Space Projection Normal (Vector3) output 0 -> Dot.B
- [13] Dot output 0 -> Add.A
- [21] Input Plane Depth Offset (Scalar) output 0 -> Add.B
- [40] Add output 0 -> Subtract.A
- [20] Input Projection Pivot Position (Vector3) output 0 -> Subtract.B
- [4] Normalize output 0 -> Input World Space Projection Normal (Vector3).Preview
- [6] Normalize output 0 -> Input World Space Projection Tangent (Vector3).Preview
- [12] Normalize output 0 -> Input World Space Projection Binormal (Vector3).Preview
- [2] Actor Position (Absolute) output 0 -> Input Projection Pivot Position (Vector3).Preview
- [25] ObjectScale output 1 -> Multiply(,10).A
- [27] ObjectScale output 1 -> Multiply(,10).A
- [10] Append output 0 -> Output Non Biased Coordinates.A
- [32] View Space to World Space TransformVector output 0 -> Dot.A
- [31] Camera Vector output 0 -> Dot.B
- [33] 0,0,-1 output 0 -> View Space to World Space TransformVector.Input
- [48] Multiply output 0 -> Multiply(,1).A
- [34] Multiply(,1) output 0 -> Divide.A
- [36] PixelDepth output 0 -> Divide.B
- [35] Divide output 0 -> Multiply.A
- [39] Subtract output 0 -> Multiply.B
- [38] Absolute World Position output 0 -> Subtract.A
- [41] Camera Position output 0 -> Subtract.B
- [37] Multiply output 0 -> Add.A
- [41] Camera Position output 0 -> Add.B
- [42] Camera Position output 0 -> Subtract.A
- [44] Multiply output 0 -> Subtract.B
- [14] Add output 0 -> Multiply.A
- [17] Input World Space Projection Normal (Vector3) output 0 -> Multiply.B
- [17] Input World Space Projection Normal (Vector3) output 0 -> Dot.A
- [43] Subtract output 0 -> Dot.B
- [45] Dot output 0 -> Divide.A
- [47] Dot output 0 -> Divide.B
- [17] Input World Space Projection Normal (Vector3) output 0 -> Dot.A
- [49] Camera Vector output 0 -> Dot.B
- [46] Divide output 0 -> Multiply.A
- [30] Dot output 0 -> Multiply.B

## Output Trace Roots

- `Center Aligned UV Plane` is driven by [7] Add(,0.5)
- `Non Biased Coordinates` is driven by [10] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


