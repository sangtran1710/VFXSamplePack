# BumpOffset_advanced

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BumpOffset_Advanced.BumpOffset_advanced`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `HeightTexture` [FunctionInput_Scalar]
- `ReferencePlane` [FunctionInput_Scalar]
- `HeightRatioInput` [FunctionInput_Scalar]
- `TextureCoordinates` [FunctionInput_Vector2]
- `CameraVector` [FunctionInput_Vector3] - By default the camera vector in tangent space but use world space if the uv coordinates are in world space.

## Outputs

- `UVs`
- `Distortion` - Add uvs to the distortion before using this value as uvs.
- `Undeformed UVs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 1 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 2 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Distortion | Distortion |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input HeightTexture (Scalar) | HeightTexture |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input ReferencePlane (Scalar) | ReferencePlane |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input HeightRatioInput (Scalar) | HeightRatioInput |
| 11 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input TextureCoordinates (Vector2) | TextureCoordinates |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input CameraVector (Vector3) | CameraVector |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Undeformed UVs | Undeformed UVs |

## Exact Input Wiring

### [0] Output UVs

- `A` <= [4] Add output 0

### [1] World Space to Tangent Space TransformVector

- `Input` <= [2] Camera Vector output 0

### [3] Multiply

- `A` <= [14] Mask ( R G ) output 0
- `B` <= [9] Subtract output 0

### [4] Add

- `A` <= [5] Multiply output 0
- `B` <= [12] Input TextureCoordinates (Vector2) output 0

### [5] Multiply

- `A` <= [3] Multiply output 0
- `B` <= [10] Input HeightRatioInput (Scalar) output 0

### [6] Output Distortion

- `A` <= [5] Multiply output 0

### [9] Subtract

- `A` <= [7] Input HeightTexture (Scalar) output 0
- `B` <= [8] Input ReferencePlane (Scalar) output 0

### [12] Input TextureCoordinates (Vector2)

- `Preview` <= [11] TexCoord[0] output 0

### [13] Input CameraVector (Vector3)

- `Preview` <= [1] World Space to Tangent Space TransformVector output 0

### [14] Mask ( R G )

- `Input` <= [13] Input CameraVector (Vector3) output 0

### [15] Output Undeformed UVs

- `A` <= [12] Input TextureCoordinates (Vector2) output 0

## Connection List

- [4] Add output 0 -> Output UVs.A
- [2] Camera Vector output 0 -> World Space to Tangent Space TransformVector.Input
- [14] Mask ( R G ) output 0 -> Multiply.A
- [9] Subtract output 0 -> Multiply.B
- [5] Multiply output 0 -> Add.A
- [12] Input TextureCoordinates (Vector2) output 0 -> Add.B
- [3] Multiply output 0 -> Multiply.A
- [10] Input HeightRatioInput (Scalar) output 0 -> Multiply.B
- [5] Multiply output 0 -> Output Distortion.A
- [7] Input HeightTexture (Scalar) output 0 -> Subtract.A
- [8] Input ReferencePlane (Scalar) output 0 -> Subtract.B
- [11] TexCoord[0] output 0 -> Input TextureCoordinates (Vector2).Preview
- [1] World Space to Tangent Space TransformVector output 0 -> Input CameraVector (Vector3).Preview
- [13] Input CameraVector (Vector3) output 0 -> Mask ( R G ).Input
- [12] Input TextureCoordinates (Vector2) output 0 -> Output Undeformed UVs.A

## Output Trace Roots

- `UVs` is driven by [4] Add
- `Distortion` is driven by [5] Multiply
- `Undeformed UVs` is driven by [12] Input TextureCoordinates (Vector2)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


