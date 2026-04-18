# FixRotateAboutAxisNormals2

- Path: `/Engine/Functions/Engine_MaterialFunctions02/FixRotateAboutAxisNormals2.FixRotateAboutAxisNormals2`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Use this material function in conjunction with a rotate about axis based vertex shader to update the vertex normals so they rotate with and match the vertex shaders motion.

## Inputs

- `Rotation Axis` [FunctionInput_Vector3] - Use the same rotation axis used to rotate your models vertices.
- `Rotation Angle` [FunctionInput_Vector3] - Use the same rotation angle used to rotate your models vertices.
- `Movement Mask` [FunctionInput_Scalar] - This value modulates the amount of normal deformation. Which means it can be used as a mask. 
- `World Space Vertex Normals` [FunctionInput_Vector3] - Filled by VertexNormalWS by default

## Outputs

- `New Tangent Space Vertex Normal` - It's recommended that this be calculated in the custom uvs input. The resulting output data should then be used as the "BaseNormal" in the BlendAngleCorrectedNormals material function. Your Normal map should be used as the "AdditionalNormal" input. The result should go into the material normal input. 
- `New Tangent Space Vertex Normal RG` - It's recommended that this be calculated in the custom uvs input. The resulting output data should then be used as the 
- `New Tangent Space Vertex Normal B` - It's recommended that this be calculated in the custom uvs input. The resulting output data should then be used as the 
- `New World Space Vertex Normal` - It's recommended that this be calculated in the custom uvs input. The resulting output data should then be used as the 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output New Tangent Space Vertex Normal | New Tangent Space Vertex Normal |
| 1 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 2 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 3 | expression | `MaterialExpressionConstant` | 0 |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 7 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Axis (Vector3) | Rotation Axis |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Angle (Vector3) | Rotation Angle |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Movement Mask (Scalar) | Movement Mask |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input World Space Vertex Normals (Vector3) | World Space Vertex Normals |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output New Tangent Space Vertex Normal RG | New Tangent Space Vertex Normal RG |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output New Tangent Space Vertex Normal B | New Tangent Space Vertex Normal B |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 15 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output New World Space Vertex Normal | New World Space Vertex Normal |

## Exact Input Wiring

### [0] Output New Tangent Space Vertex Normal

- `A` <= [7] World Space to Tangent Space TransformVector output 0

### [1] RotateAboutAxis

- `NormalizedRotationAxis` <= [8] Input Rotation Axis (Vector3) output 0
- `RotationAngle` <= [9] Input Rotation Angle (Vector3) output 0
- `PivotPoint` <= [3] 0 output 0
- `Position` <= [11] Input World Space Vertex Normals (Vector3) output 0

### [4] Multiply

- `A` <= [1] RotateAboutAxis output 0
- `B` <= [10] Input Movement Mask (Scalar) output 0

### [5] Add

- `A` <= [4] Multiply output 0
- `B` <= [11] Input World Space Vertex Normals (Vector3) output 0

### [6] Normalize

- `VectorInput` <= [5] Add output 0

### [7] World Space to Tangent Space TransformVector

- `Input` <= [6] Normalize output 0

### [11] Input World Space Vertex Normals (Vector3)

- `Preview` <= [2] VertexNormalWS output 0

### [12] Output New Tangent Space Vertex Normal RG

- `A` <= [14] Mask ( R G ) output 0

### [13] Output New Tangent Space Vertex Normal B

- `A` <= [15] Mask ( B ) output 0

### [14] Mask ( R G )

- `Input` <= [7] World Space to Tangent Space TransformVector output 0

### [15] Mask ( B )

- `Input` <= [7] World Space to Tangent Space TransformVector output 0

### [16] Output New World Space Vertex Normal

- `A` <= [6] Normalize output 0

## Connection List

- [7] World Space to Tangent Space TransformVector output 0 -> Output New Tangent Space Vertex Normal.A
- [8] Input Rotation Axis (Vector3) output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [9] Input Rotation Angle (Vector3) output 0 -> RotateAboutAxis.RotationAngle
- [3] 0 output 0 -> RotateAboutAxis.PivotPoint
- [11] Input World Space Vertex Normals (Vector3) output 0 -> RotateAboutAxis.Position
- [1] RotateAboutAxis output 0 -> Multiply.A
- [10] Input Movement Mask (Scalar) output 0 -> Multiply.B
- [4] Multiply output 0 -> Add.A
- [11] Input World Space Vertex Normals (Vector3) output 0 -> Add.B
- [5] Add output 0 -> Normalize.VectorInput
- [6] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [2] VertexNormalWS output 0 -> Input World Space Vertex Normals (Vector3).Preview
- [14] Mask ( R G ) output 0 -> Output New Tangent Space Vertex Normal RG.A
- [15] Mask ( B ) output 0 -> Output New Tangent Space Vertex Normal B.A
- [7] World Space to Tangent Space TransformVector output 0 -> Mask ( R G ).Input
- [7] World Space to Tangent Space TransformVector output 0 -> Mask ( B ).Input
- [6] Normalize output 0 -> Output New World Space Vertex Normal.A

## Output Trace Roots

- `New Tangent Space Vertex Normal` is driven by [7] World Space to Tangent Space TransformVector
- `New Tangent Space Vertex Normal RG` is driven by [14] Mask ( R G )
- `New Tangent Space Vertex Normal B` is driven by [15] Mask ( B )
- `New World Space Vertex Normal` is driven by [6] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
