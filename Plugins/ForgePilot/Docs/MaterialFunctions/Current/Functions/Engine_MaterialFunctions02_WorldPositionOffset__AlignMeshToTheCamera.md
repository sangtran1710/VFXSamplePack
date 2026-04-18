# AlignMeshToTheCamera

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/AlignMeshToTheCamera.AlignMeshToTheCamera`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Aligns a static mesh to the camera and allows the user to rotate the mesh by altering the objects basis vectors.

## Inputs

- `Custom Object Basis 1` [FunctionInput_Vector3]
- `Custom Object Basis 2` [FunctionInput_Vector3]
- `Custom Object Basis 3` [FunctionInput_Vector3]
- `WS Normals` [FunctionInput_Vector3] - Enter the models world space normals. 
- `WorldPosition` [FunctionInput_Vector3]
- `PivotLocation` [FunctionInput_Vector3]

## Outputs

- `World Position Offset`
- `Rotated Normals`
- `Undeformed Pixel Shader World Position`
- `test WPO`
- `CameraXVector`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Custom Object Basis 1 (Vector3) | Custom Object Basis 1 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Custom Object Basis 2 (Vector3) | Custom Object Basis 2 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Custom Object Basis 3 (Vector3) | Custom Object Basis 3 |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | ComponentPivotLocation |  |
| 5 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 8 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeVectorsOrthogonal |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 16 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 17 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Rotated Normals | Rotated Normals |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input WS Normals (Vector3) | WS Normals |
| 20 | expression | `MaterialExpressionTransform` | World Space to Instance & Particle Space TransformVector |  |
| 21 | expression | `MaterialExpressionTransform` | World Space to Instance & Particle Space TransformVector |  |
| 22 | expression | `MaterialExpressionTransform` | World Space to Instance & Particle Space TransformVector |  |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output Undeformed Pixel Shader World Position | Undeformed Pixel Shader World Position |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraVectorWithWPOOptions |  |
| 26 | function_call | `MaterialExpressionMaterialFunctionCall` | InverseTransformMatrix |  |
| 27 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 28 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 29 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeVectorsOrthogonal |  |
| 31 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 32 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | function_output | `MaterialExpressionFunctionOutput` | Output test WPO | test WPO |
| 35 | function_output | `MaterialExpressionFunctionOutput` | Output CameraXVector | CameraXVector |
| 36 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 37 | function_input | `MaterialExpressionFunctionInput` | Input PivotLocation (Vector3) | PivotLocation |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [14] Add output 0

### [5] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [36] Input WorldPosition (Vector3) output 0

### [8] View Space to World Space TransformVector

- `Input` <= [7] Transform3x3Matrix output 0

### [9] Subtract

- `A` <= [8] View Space to World Space TransformVector output 0
- `B` <= [10] Absolute World Position output 0

### [12] Multiply

- `A` <= [11] ObjectScale output 0
- `B` <= [5] Absolute World Space to Instance & Particle Space TransformPosition output 0

### [14] Add

- `A` <= [9] Subtract output 0
- `B` <= [37] Input PivotLocation (Vector3) output 0

### [16] View Space to World Space TransformVector

- `Input` <= [15] Transform3x3Matrix output 0

### [18] Output Rotated Normals

- `A` <= [16] View Space to World Space TransformVector output 0

### [19] Input WS Normals (Vector3)

- `Preview` <= [17] VertexNormalWS output 0

### [20] World Space to Instance & Particle Space TransformVector

- `Input` <= [1] Input Custom Object Basis 1 (Vector3) output 0

### [21] World Space to Instance & Particle Space TransformVector

- `Input` <= [2] Input Custom Object Basis 2 (Vector3) output 0

### [22] World Space to Instance & Particle Space TransformVector

- `Input` <= [3] Input Custom Object Basis 3 (Vector3) output 0

### [23] Output Undeformed Pixel Shader World Position

- `A` <= [24] Add output 0

### [24] Add

- `A` <= [8] View Space to World Space TransformVector output 0
- `B` <= [37] Input PivotLocation (Vector3) output 0

### [27] Cross

- `A` <= [25] CameraVectorWithWPOOptions output 0
- `B` <= [28] View Space to World Space TransformVector output 0

### [28] View Space to World Space TransformVector

- `Input` <= [29] 0,1,0 output 0

### [31] Cross

- `A` <= [27] Cross output 0
- `B` <= [25] CameraVectorWithWPOOptions output 0

### [32] Subtract

- `A` <= [10] Absolute World Position output 0
- `B` <= [26] InverseTransformMatrix output 0

### [33] Add

- `A` <= [32] Subtract output 0
- `B` <= [37] Input PivotLocation (Vector3) output 0

### [34] Output test WPO

- `A` <= [33] Add output 0

### [35] Output CameraXVector

- `A` <= [13] MakeVectorsOrthogonal output 0

### [36] Input WorldPosition (Vector3)

- `Preview` <= [6] Absolute World Position (Excluding Material Offsets) output 0

### [37] Input PivotLocation (Vector3)

- `Preview` <= [4] ComponentPivotLocation output 0

## Connection List

- [14] Add output 0 -> Output World Position Offset.A
- [36] Input WorldPosition (Vector3) output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [7] Transform3x3Matrix output 0 -> View Space to World Space TransformVector.Input
- [8] View Space to World Space TransformVector output 0 -> Subtract.A
- [10] Absolute World Position output 0 -> Subtract.B
- [11] ObjectScale output 0 -> Multiply.A
- [5] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Multiply.B
- [9] Subtract output 0 -> Add.A
- [37] Input PivotLocation (Vector3) output 0 -> Add.B
- [15] Transform3x3Matrix output 0 -> View Space to World Space TransformVector.Input
- [16] View Space to World Space TransformVector output 0 -> Output Rotated Normals.A
- [17] VertexNormalWS output 0 -> Input WS Normals (Vector3).Preview
- [1] Input Custom Object Basis 1 (Vector3) output 0 -> World Space to Instance & Particle Space TransformVector.Input
- [2] Input Custom Object Basis 2 (Vector3) output 0 -> World Space to Instance & Particle Space TransformVector.Input
- [3] Input Custom Object Basis 3 (Vector3) output 0 -> World Space to Instance & Particle Space TransformVector.Input
- [24] Add output 0 -> Output Undeformed Pixel Shader World Position.A
- [8] View Space to World Space TransformVector output 0 -> Add.A
- [37] Input PivotLocation (Vector3) output 0 -> Add.B
- [25] CameraVectorWithWPOOptions output 0 -> Cross.A
- [28] View Space to World Space TransformVector output 0 -> Cross.B
- [29] 0,1,0 output 0 -> View Space to World Space TransformVector.Input
- [27] Cross output 0 -> Cross.A
- [25] CameraVectorWithWPOOptions output 0 -> Cross.B
- [10] Absolute World Position output 0 -> Subtract.A
- [26] InverseTransformMatrix output 0 -> Subtract.B
- [32] Subtract output 0 -> Add.A
- [37] Input PivotLocation (Vector3) output 0 -> Add.B
- [33] Add output 0 -> Output test WPO.A
- [13] MakeVectorsOrthogonal output 0 -> Output CameraXVector.A
- [6] Absolute World Position (Excluding Material Offsets) output 0 -> Input WorldPosition (Vector3).Preview
- [4] ComponentPivotLocation output 0 -> Input PivotLocation (Vector3).Preview

## Output Trace Roots

- `World Position Offset` is driven by [14] Add
- `Rotated Normals` is driven by [16] View Space to World Space TransformVector
- `Undeformed Pixel Shader World Position` is driven by [24] Add
- `test WPO` is driven by [33] Add
- `CameraXVector` is driven by [13] MakeVectorsOrthogonal

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


