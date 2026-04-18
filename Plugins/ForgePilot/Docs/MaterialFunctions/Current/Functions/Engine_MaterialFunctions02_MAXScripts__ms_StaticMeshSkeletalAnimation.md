# ms_StaticMeshSkeletalAnimation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/ms_StaticMeshSkeletalAnimation.ms_StaticMeshSkeletalAnimation`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This material function decodes the skeletal mesh animation textures and bone index data packed by the 3DS MAXScript located here: ...Engine\Extras\3dsMaxScripts\StaticMeshSkeletalAnimation.ms. For more information please refer to the guide button with the script and online documentation.

## Inputs

- `Vertex Shader Bone Index` [FunctionInput_Scalar]
- `Pixel Shader Bone Index` [FunctionInput_Scalar] - Input the vertex alpha or a channel from the UV that was used to store the bone index in the script.
- `Animation Phase` [FunctionInput_Scalar] - Enter a 0-1 value as a lookup into the animation texture.
- `Rotation Animation` [FunctionInput_Texture2D]
- `World Space Normals` [FunctionInput_Vector3]
- `Position Animation` [FunctionInput_Texture2D]
- `Rest Pose Texture` [FunctionInput_Texture2D]
- `World Position` [FunctionInput_Vector3]

## Outputs

- `World Position Offset`
- `World Space Normals`
- `Advanced: Mesh Rotation Axis And Angle`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | FixRotateAboutAxisNormals |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 6 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 7 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 8 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 10 | expression | `MaterialExpressionTime` | Time |  |
| 11 | expression | `MaterialExpressionFrac` | Frac |  |
| 12 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 13 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 14 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 15 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 16 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Vertex Shader Bone Index (Scalar) | Vertex Shader Bone Index |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Pixel Shader Bone Index (Scalar) | Pixel Shader Bone Index |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Animation Phase (Scalar) | Animation Phase |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Animation (Texture2D) | Rotation Animation |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input World Space Normals (Vector3) | World Space Normals |
| 23 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normals | World Space Normals |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Position Animation (Texture2D) | Position Animation |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Rest Pose Texture (Texture2D) | Rest Pose Texture |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 28 | function_output | `MaterialExpressionFunctionOutput` | Output Advanced: Mesh Rotation Axis And Angle | Advanced: Mesh Rotation Axis And Angle |
| 29 | expression | `MaterialExpressionAppendVector` | Append |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 31 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 33 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [2] Add output 0

### [2] Add

- `A` <= [6] Instance & Particle Space to World Space TransformVector output 0
- `B` <= [12] RotateAboutAxis output 0

### [3] Texture Sample

- `Coordinates` <= [9] MakeFloat2 output 0
- `TextureObject` <= [25] Input Position Animation (Texture2D) output 0

### [4] Texture Sample

- `Coordinates` <= [9] MakeFloat2 output 0
- `TextureObject` <= [21] Input Rotation Animation (Texture2D) output 0

### [5] Texture Sample

- `Coordinates` <= [18] Input Vertex Shader Bone Index (Scalar) output 0
- `TextureObject` <= [26] Input Rest Pose Texture (Texture2D) output 0

### [6] Instance & Particle Space to World Space TransformVector

- `Input` <= [3] Texture Sample output 0

### [7] Instance & Particle Space to World Space TransformVector

- `Input` <= [4] Texture Sample output 0

### [11] Frac

- `Input` <= [10] Time output 0

### [12] RotateAboutAxis

- `NormalizedRotationAxis` <= [30] SafeNormalize output 0
- `RotationAngle` <= [4] Texture Sample output 4
- `PivotPoint` <= [14] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `Position` <= [27] Input World Position (Vector3) output 0

### [14] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [5] Texture Sample output 0

### [15] Texture Sample

- `Coordinates` <= [17] MakeFloat2 output 0
- `TextureObject` <= [21] Input Rotation Animation (Texture2D) output 0

### [16] Instance & Particle Space to World Space TransformVector

- `Input` <= [15] Texture Sample output 0

### [18] Input Vertex Shader Bone Index (Scalar)

- `Preview` <= [8] Vertex Color output 4

### [19] Input Pixel Shader Bone Index (Scalar)

- `Preview` <= [18] Input Vertex Shader Bone Index (Scalar) output 0

### [20] Input Animation Phase (Scalar)

- `Preview` <= [11] Frac output 0

### [22] Input World Space Normals (Vector3)

- `Preview` <= [23] VertexNormalWS output 0

### [24] Output World Space Normals

- `A` <= [1] FixRotateAboutAxisNormals output 3

### [27] Input World Position (Vector3)

- `Preview` <= [13] Absolute World Position output 0

### [28] Output Advanced: Mesh Rotation Axis And Angle

- `A` <= [29] Append output 0

### [29] Append

- `A` <= [30] SafeNormalize output 0
- `B` <= [4] Texture Sample output 4

## Connection List

- [2] Add output 0 -> Output World Position Offset.A
- [6] Instance & Particle Space to World Space TransformVector output 0 -> Add.A
- [12] RotateAboutAxis output 0 -> Add.B
- [9] MakeFloat2 output 0 -> Texture Sample.Coordinates
- [25] Input Position Animation (Texture2D) output 0 -> Texture Sample.TextureObject
- [9] MakeFloat2 output 0 -> Texture Sample.Coordinates
- [21] Input Rotation Animation (Texture2D) output 0 -> Texture Sample.TextureObject
- [18] Input Vertex Shader Bone Index (Scalar) output 0 -> Texture Sample.Coordinates
- [26] Input Rest Pose Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [3] Texture Sample output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [4] Texture Sample output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [10] Time output 0 -> Frac.Input
- [30] SafeNormalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [4] Texture Sample output 4 -> RotateAboutAxis.RotationAngle
- [14] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> RotateAboutAxis.PivotPoint
- [27] Input World Position (Vector3) output 0 -> RotateAboutAxis.Position
- [5] Texture Sample output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [17] MakeFloat2 output 0 -> Texture Sample.Coordinates
- [21] Input Rotation Animation (Texture2D) output 0 -> Texture Sample.TextureObject
- [15] Texture Sample output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [8] Vertex Color output 4 -> Input Vertex Shader Bone Index (Scalar).Preview
- [18] Input Vertex Shader Bone Index (Scalar) output 0 -> Input Pixel Shader Bone Index (Scalar).Preview
- [11] Frac output 0 -> Input Animation Phase (Scalar).Preview
- [23] VertexNormalWS output 0 -> Input World Space Normals (Vector3).Preview
- [1] FixRotateAboutAxisNormals output 3 -> Output World Space Normals.A
- [13] Absolute World Position output 0 -> Input World Position (Vector3).Preview
- [29] Append output 0 -> Output Advanced: Mesh Rotation Axis And Angle.A
- [30] SafeNormalize output 0 -> Append.A
- [4] Texture Sample output 4 -> Append.B

## Output Trace Roots

- `World Position Offset` is driven by [2] Add
- `World Space Normals` is driven by [1] FixRotateAboutAxisNormals
- `Advanced: Mesh Rotation Axis And Angle` is driven by [29] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


