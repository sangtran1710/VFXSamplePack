# AttachMeshToTheCamera

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/AttachMeshToTheCamera.AttachMeshToTheCamera`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Attaches a static mesh to the camera and allows the user to rotate the mesh by altering the object bases and also provides a camera space offset variable.

## Inputs

- `Camera Space Position Offset` [FunctionInput_Vector3]
- `Object Basis 1` [FunctionInput_Vector3]
- `Object Basis 2` [FunctionInput_Vector3]
- `Object Basis 3` [FunctionInput_Vector3]

## Outputs

- `World Position Offset`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 4 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Camera Space Position Offset (Vector3) | Camera Space Position Offset |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Object Basis 1 (Vector3) | Object Basis 1 |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Object Basis 2 (Vector3) | Object Basis 2 |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Object Basis 3 (Vector3) | Object Basis 3 |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeVectorsOrthogonal |  |
| 16 | expression | `MaterialExpressionConstant3Vector` | 0,0,200 |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [7] Subtract output 0

### [1] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [2] Absolute World Position output 0

### [4] View Space to World Space TransformVector

- `Input` <= [3] Transform3x3Matrix output 0

### [5] Add

- `A` <= [4] View Space to World Space TransformVector output 0
- `B` <= [6] Camera Position output 0

### [7] Subtract

- `A` <= [5] Add output 0
- `B` <= [8] Absolute World Position output 0

### [9] Input Camera Space Position Offset (Vector3)

- `Preview` <= [16] 0,0,200 output 0

### [11] Multiply

- `A` <= [10] ObjectScale output 0
- `B` <= [1] Absolute World Space to Instance & Particle Space TransformPosition output 0

## Connection List

- [7] Subtract output 0 -> Output World Position Offset.A
- [2] Absolute World Position output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [3] Transform3x3Matrix output 0 -> View Space to World Space TransformVector.Input
- [4] View Space to World Space TransformVector output 0 -> Add.A
- [6] Camera Position output 0 -> Add.B
- [5] Add output 0 -> Subtract.A
- [8] Absolute World Position output 0 -> Subtract.B
- [16] 0,0,200 output 0 -> Input Camera Space Position Offset (Vector3).Preview
- [10] ObjectScale output 0 -> Multiply.A
- [1] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Multiply.B

## Output Trace Roots

- `World Position Offset` is driven by [7] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


