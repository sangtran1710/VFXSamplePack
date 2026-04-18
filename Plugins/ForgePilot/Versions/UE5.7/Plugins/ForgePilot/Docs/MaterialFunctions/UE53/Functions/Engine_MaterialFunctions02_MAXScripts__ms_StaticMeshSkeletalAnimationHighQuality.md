# ms_StaticMeshSkeletalAnimationHighQuality

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/ms_StaticMeshSkeletalAnimationHighQuality.ms_StaticMeshSkeletalAnimationHighQuality`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This material function decodes the skeletal mesh animation textures and bone index data packed by the 3DS MAXScript located here: ...Engine\Extras\3dsMaxScripts\StaticMeshSkeletalAnimation.ms. When using this material function in a chain, feed the output vectors to the subsequent material function. You'll only need to pipe the normal texture input into the last material function. The last instance of the function should be connected to the base materials custom uv inputs.  For more information please refer to the guide button with the script and online documentation. 

## Inputs

- `Vertex Shader Bone Index` [FunctionInput_Scalar] - Input the vertex alpha or a channel from the UV that was used to store the bone index in the script.
- `Animation Phase` [FunctionInput_Scalar] - Enter a 0-1 value as a lookup into the animation texture.
- `Rotation Animation` [FunctionInput_Texture2D]
- `Tangent Space Normal Map` [FunctionInput_Vector3]
- `Position Animation` [FunctionInput_Texture2D]
- `Rest Pose Texture` [FunctionInput_Texture2D]
- `Tangent Basis X In WS` [FunctionInput_Vector3]
- `Tangent Basis Y In WS` [FunctionInput_Vector3]
- `Tangent Basis Z In WS` [FunctionInput_Vector3]
- `World Position` [FunctionInput_Vector3]

## Outputs

- `World Position Offset`
- `World Space Normals`
- `Updated Vertex Shader X Vector`
- `Updated Vertex Shader Y Vector`
- `Updated Vertex Shader Z Vector`
- `Custom UV 2`
- `Custom UV 3`
- `Custom UV 4`
- `Custom UV 5`
- `Custom UV 6`
- `Updated Pixel Shader X Vector`
- `Updated Pixel Shader Y Vector`
- `Updated Pixel Shader Z Vector`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 6 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 7 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 9 | expression | `MaterialExpressionTime` | Time |  |
| 10 | expression | `MaterialExpressionFrac` | Frac |  |
| 11 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 12 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 13 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Vertex Shader Bone Index (Scalar) | Vertex Shader Bone Index |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Animation Phase (Scalar) | Animation Phase |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Animation (Texture2D) | Rotation Animation |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Tangent Space Normal Map (Vector3) | Tangent Space Normal Map |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normals | World Space Normals |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Position Animation (Texture2D) | Position Animation |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Rest Pose Texture (Texture2D) | Rest Pose Texture |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Tangent Basis X In WS (Vector3) | Tangent Basis X In WS |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Tangent Basis Y In WS (Vector3) | Tangent Basis Y In WS |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Tangent Basis Z In WS (Vector3) | Tangent Basis Z In WS |
| 24 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 25 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 26 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 27 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 28 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 29 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 30 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 31 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 32 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 33 | expression | `MaterialExpressionConstant` | 0 |  |
| 34 | expression | `MaterialExpressionConstant` | 0 |  |
| 35 | expression | `MaterialExpressionConstant` | 0 |  |
| 36 | function_output | `MaterialExpressionFunctionOutput` | Output Updated Vertex Shader X Vector | Updated Vertex Shader X Vector |
| 37 | function_output | `MaterialExpressionFunctionOutput` | Output Updated Vertex Shader Y Vector | Updated Vertex Shader Y Vector |
| 38 | function_output | `MaterialExpressionFunctionOutput` | Output Updated Vertex Shader Z Vector | Updated Vertex Shader Z Vector |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 2 | Custom UV 2 |
| 40 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 3 | Custom UV 3 |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 4 | Custom UV 4 |
| 42 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 43 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 44 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[4] |  |
| 45 | expression | `MaterialExpressionAdd` | Add |  |
| 46 | expression | `MaterialExpressionAdd` | Add |  |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 49 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 50 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 5 | Custom UV 5 |
| 51 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 52 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 53 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 54 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 55 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[5] |  |
| 56 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 57 | expression | `MaterialExpressionAppendVector` | Append |  |
| 58 | expression | `MaterialExpressionAppendVector` | Append |  |
| 59 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 60 | expression | `MaterialExpressionAppendVector` | Append |  |
| 61 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 62 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 6 | Custom UV 6 |
| 63 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[6] |  |
| 64 | expression | `MaterialExpressionAppendVector` | Append |  |
| 65 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 66 | function_output | `MaterialExpressionFunctionOutput` | Output Updated Pixel Shader X Vector | Updated Pixel Shader X Vector |
| 67 | function_output | `MaterialExpressionFunctionOutput` | Output Updated Pixel Shader Y Vector | Updated Pixel Shader Y Vector |
| 68 | function_output | `MaterialExpressionFunctionOutput` | Output Updated Pixel Shader Z Vector | Updated Pixel Shader Z Vector |
| 69 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [1] Add output 0

### [1] Add

- `A` <= [5] Instance & Particle Space to World Space TransformVector output 0
- `B` <= [11] RotateAboutAxis output 0

### [2] Texture Sample

- `Coordinates` <= [8] MakeFloat2 output 0
- `TextureObject` <= [19] Input Position Animation (Texture2D) output 0

### [3] Texture Sample

- `Coordinates` <= [8] MakeFloat2 output 0
- `TextureObject` <= [16] Input Rotation Animation (Texture2D) output 0

### [4] Texture Sample

- `Coordinates` <= [14] Input Vertex Shader Bone Index (Scalar) output 0
- `TextureObject` <= [20] Input Rest Pose Texture (Texture2D) output 0

### [5] Instance & Particle Space to World Space TransformVector

- `Input` <= [2] Texture Sample output 0

### [6] Instance & Particle Space to World Space TransformVector

- `Input` <= [3] Texture Sample output 0

### [10] Frac

- `Input` <= [9] Time output 0

### [11] RotateAboutAxis

- `NormalizedRotationAxis` <= [69] Normalize output 0
- `RotationAngle` <= [3] Texture Sample output 4
- `PivotPoint` <= [13] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `Position` <= [65] Input World Position (Vector3) output 0

### [13] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [4] Texture Sample output 0

### [14] Input Vertex Shader Bone Index (Scalar)

- `Preview` <= [7] Vertex Color output 4

### [15] Input Animation Phase (Scalar)

- `Preview` <= [10] Frac output 0

### [18] Output World Space Normals

- `A` <= [48] Transform3x3Matrix output 0

### [21] Input Tangent Basis X In WS (Vector3)

- `Preview` <= [25] Tangent Space to World Space TransformVector output 0

### [22] Input Tangent Basis Y In WS (Vector3)

- `Preview` <= [27] Tangent Space to World Space TransformVector output 0

### [23] Input Tangent Basis Z In WS (Vector3)

- `Preview` <= [29] Tangent Space to World Space TransformVector output 0

### [25] Tangent Space to World Space TransformVector

- `Input` <= [24] 1,0,0 output 0

### [27] Tangent Space to World Space TransformVector

- `Input` <= [26] 0,1,0 output 0

### [29] Tangent Space to World Space TransformVector

- `Input` <= [28] 0,0,1 output 0

### [30] RotateAboutAxis

- `NormalizedRotationAxis` <= [69] Normalize output 0
- `RotationAngle` <= [3] Texture Sample output 4
- `PivotPoint` <= [35] 0 output 0
- `Position` <= [21] Input Tangent Basis X In WS (Vector3) output 0

### [31] RotateAboutAxis

- `NormalizedRotationAxis` <= [69] Normalize output 0
- `RotationAngle` <= [3] Texture Sample output 4
- `PivotPoint` <= [34] 0 output 0
- `Position` <= [22] Input Tangent Basis Y In WS (Vector3) output 0

### [32] RotateAboutAxis

- `NormalizedRotationAxis` <= [69] Normalize output 0
- `RotationAngle` <= [3] Texture Sample output 4
- `PivotPoint` <= [33] 0 output 0
- `Position` <= [23] Input Tangent Basis Z In WS (Vector3) output 0

### [36] Output Updated Vertex Shader X Vector

- `A` <= [45] Add output 0

### [37] Output Updated Vertex Shader Y Vector

- `A` <= [46] Add output 0

### [38] Output Updated Vertex Shader Z Vector

- `A` <= [47] Add output 0

### [39] Output Custom UV 2

- `A` <= [60] Append output 0

### [40] Output Custom UV 3

- `A` <= [52] MakeFloat2 output 0

### [41] Output Custom UV 4

- `A` <= [53] MakeFloat2 output 0

### [45] Add

- `A` <= [30] RotateAboutAxis output 0
- `B` <= [21] Input Tangent Basis X In WS (Vector3) output 0

### [46] Add

- `A` <= [31] RotateAboutAxis output 0
- `B` <= [22] Input Tangent Basis Y In WS (Vector3) output 0

### [47] Add

- `A` <= [32] RotateAboutAxis output 0
- `B` <= [23] Input Tangent Basis Z In WS (Vector3) output 0

### [50] Output Custom UV 5

- `A` <= [54] MakeFloat2 output 0

### [57] Append

- `A` <= [42] TexCoord[2] output 0
- `B` <= [56] BreakOutFloat2Components output 0

### [58] Append

- `A` <= [56] BreakOutFloat2Components output 1
- `B` <= [44] TexCoord[4] output 0

### [60] Append

- `A` <= [51] BreakOutFloat3Components output 0
- `B` <= [51] BreakOutFloat3Components output 1

### [62] Output Custom UV 6

- `A` <= [49] BreakOutFloat3Components output 2

### [64] Append

- `A` <= [55] TexCoord[5] output 0
- `B` <= [59] BreakOutFloat2Components output 0

### [65] Input World Position (Vector3)

- `Preview` <= [12] Absolute World Position output 0

### [66] Output Updated Pixel Shader X Vector

- `A` <= [57] Append output 0

### [67] Output Updated Pixel Shader Y Vector

- `A` <= [58] Append output 0

### [68] Output Updated Pixel Shader Z Vector

- `A` <= [64] Append output 0

### [69] Normalize

- `VectorInput` <= [6] Instance & Particle Space to World Space TransformVector output 0

## Connection List

- [1] Add output 0 -> Output World Position Offset.A
- [5] Instance & Particle Space to World Space TransformVector output 0 -> Add.A
- [11] RotateAboutAxis output 0 -> Add.B
- [8] MakeFloat2 output 0 -> Texture Sample.Coordinates
- [19] Input Position Animation (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] MakeFloat2 output 0 -> Texture Sample.Coordinates
- [16] Input Rotation Animation (Texture2D) output 0 -> Texture Sample.TextureObject
- [14] Input Vertex Shader Bone Index (Scalar) output 0 -> Texture Sample.Coordinates
- [20] Input Rest Pose Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [2] Texture Sample output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [3] Texture Sample output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [9] Time output 0 -> Frac.Input
- [69] Normalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [3] Texture Sample output 4 -> RotateAboutAxis.RotationAngle
- [13] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> RotateAboutAxis.PivotPoint
- [65] Input World Position (Vector3) output 0 -> RotateAboutAxis.Position
- [4] Texture Sample output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [7] Vertex Color output 4 -> Input Vertex Shader Bone Index (Scalar).Preview
- [10] Frac output 0 -> Input Animation Phase (Scalar).Preview
- [48] Transform3x3Matrix output 0 -> Output World Space Normals.A
- [25] Tangent Space to World Space TransformVector output 0 -> Input Tangent Basis X In WS (Vector3).Preview
- [27] Tangent Space to World Space TransformVector output 0 -> Input Tangent Basis Y In WS (Vector3).Preview
- [29] Tangent Space to World Space TransformVector output 0 -> Input Tangent Basis Z In WS (Vector3).Preview
- [24] 1,0,0 output 0 -> Tangent Space to World Space TransformVector.Input
- [26] 0,1,0 output 0 -> Tangent Space to World Space TransformVector.Input
- [28] 0,0,1 output 0 -> Tangent Space to World Space TransformVector.Input
- [69] Normalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [3] Texture Sample output 4 -> RotateAboutAxis.RotationAngle
- [35] 0 output 0 -> RotateAboutAxis.PivotPoint
- [21] Input Tangent Basis X In WS (Vector3) output 0 -> RotateAboutAxis.Position
- [69] Normalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [3] Texture Sample output 4 -> RotateAboutAxis.RotationAngle
- [34] 0 output 0 -> RotateAboutAxis.PivotPoint
- [22] Input Tangent Basis Y In WS (Vector3) output 0 -> RotateAboutAxis.Position
- [69] Normalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [3] Texture Sample output 4 -> RotateAboutAxis.RotationAngle
- [33] 0 output 0 -> RotateAboutAxis.PivotPoint
- [23] Input Tangent Basis Z In WS (Vector3) output 0 -> RotateAboutAxis.Position
- [45] Add output 0 -> Output Updated Vertex Shader X Vector.A
- [46] Add output 0 -> Output Updated Vertex Shader Y Vector.A
- [47] Add output 0 -> Output Updated Vertex Shader Z Vector.A
- [60] Append output 0 -> Output Custom UV 2.A
- [52] MakeFloat2 output 0 -> Output Custom UV 3.A
- [53] MakeFloat2 output 0 -> Output Custom UV 4.A
- [30] RotateAboutAxis output 0 -> Add.A
- [21] Input Tangent Basis X In WS (Vector3) output 0 -> Add.B
- [31] RotateAboutAxis output 0 -> Add.A
- [22] Input Tangent Basis Y In WS (Vector3) output 0 -> Add.B
- [32] RotateAboutAxis output 0 -> Add.A
- [23] Input Tangent Basis Z In WS (Vector3) output 0 -> Add.B
- [54] MakeFloat2 output 0 -> Output Custom UV 5.A
- [42] TexCoord[2] output 0 -> Append.A
- [56] BreakOutFloat2Components output 0 -> Append.B
- [56] BreakOutFloat2Components output 1 -> Append.A
- [44] TexCoord[4] output 0 -> Append.B
- [51] BreakOutFloat3Components output 0 -> Append.A
- [51] BreakOutFloat3Components output 1 -> Append.B
- [49] BreakOutFloat3Components output 2 -> Output Custom UV 6.A
- [55] TexCoord[5] output 0 -> Append.A
- [59] BreakOutFloat2Components output 0 -> Append.B
- [12] Absolute World Position output 0 -> Input World Position (Vector3).Preview
- [57] Append output 0 -> Output Updated Pixel Shader X Vector.A
- [58] Append output 0 -> Output Updated Pixel Shader Y Vector.A
- [64] Append output 0 -> Output Updated Pixel Shader Z Vector.A
- [6] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `World Position Offset` is driven by [1] Add
- `World Space Normals` is driven by [48] Transform3x3Matrix
- `Updated Vertex Shader X Vector` is driven by [45] Add
- `Updated Vertex Shader Y Vector` is driven by [46] Add
- `Updated Vertex Shader Z Vector` is driven by [47] Add
- `Custom UV 2` is driven by [60] Append
- `Custom UV 3` is driven by [52] MakeFloat2
- `Custom UV 4` is driven by [53] MakeFloat2
- `Custom UV 5` is driven by [54] MakeFloat2
- `Custom UV 6` is driven by [49] BreakOutFloat3Components
- `Updated Pixel Shader X Vector` is driven by [57] Append
- `Updated Pixel Shader Y Vector` is driven by [58] Append
- `Updated Pixel Shader Z Vector` is driven by [64] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
