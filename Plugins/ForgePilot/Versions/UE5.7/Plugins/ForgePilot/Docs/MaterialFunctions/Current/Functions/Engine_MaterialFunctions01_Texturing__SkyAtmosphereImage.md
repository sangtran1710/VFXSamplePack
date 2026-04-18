# SkyAtmosphereImage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SkyAtmosphereImage.SkyAtmosphereImage`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Rotation (radian)` [FunctionInput_Scalar]
- `Direction` [FunctionInput_Vector3] - The direction in which the sky sprite needs to appear.
- `TextureObject` [FunctionInput_Texture2D] - The texture applied on the sky sprite.
- `Scale` [FunctionInput_Vector2]

## Outputs

- `Result` - UnMasked RGB image, may contain stretching, use output mask 
- `Alpha` - UnMasked image alpha, may contain stretching, use output mask 
- `Mask` - Mask for the area of correct projection

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Rotation (radian) (Scalar) | Rotation (radian) |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Direction (Vector3) | Direction |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 4 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Scale (Vector2) | Scale |
| 6 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Mask | Mask |
| 9 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 10 | custom | `MaterialExpressionCustom` | SkyAtmosphereImage |  |
| 11 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [11] Texture Sample output 0

### [3] Input TextureObject (Texture2D)

- `Preview` <= [4] Texture Object output 0

### [6] Normalize

- `VectorInput` <= [2] Input Direction (Vector3) output 0

### [7] Output Alpha

- `A` <= [11] Texture Sample output 4

### [8] Output Mask

- `A` <= [10] SkyAtmosphereImage output 1

### [11] Texture Sample

- `Coordinates` <= [10] SkyAtmosphereImage output 0
- `TextureObject` <= [3] Input TextureObject (Texture2D) output 0

## Connection List

- [11] Texture Sample output 0 -> Output Result.A
- [4] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [2] Input Direction (Vector3) output 0 -> Normalize.VectorInput
- [11] Texture Sample output 4 -> Output Alpha.A
- [10] SkyAtmosphereImage output 1 -> Output Mask.A
- [10] SkyAtmosphereImage output 0 -> Texture Sample.Coordinates
- [3] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject

## Output Trace Roots

- `Result` is driven by [11] Texture Sample
- `Alpha` is driven by [11] Texture Sample
- `Mask` is driven by [10] SkyAtmosphereImage

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


