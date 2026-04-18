# PixelDepthOffset_Foliage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Lighting/PixelDepthOffset_Foliage.PixelDepthOffset_Foliage`
- Category: Engine_MaterialFunctions01/Lighting
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function helps set up PixelDepthOffset for foliage meshes.

## Inputs

- `WorldPositionOffset` [FunctionInput_Vector3] - WorldPositionOffset before PixelDepth modifications
- `Dynamic Shadow Distance` [FunctionInput_Scalar] - This will mask the PixelDepthOffset by the dynamic shadow distance. This is necessary when using for lighting since PDO only works with Distance Field shadows currently. If 0 it is disabled.
- `Dynamic Shadow Fade Length` [FunctionInput_Scalar] - This is how long the fade length for the dynamic shadow mask is. Defaults to 10% of Shadow Distance input.
- `Initial Radius` [FunctionInput_Scalar] - This is the radius of the mesh this will be used on, without any scaling. This is required since foliage does not have access to the objectradius or bounds.
- `CustomUV Input` [FunctionInput_Vector2] - This needs to be the same UV channel as the one connected to the CustomUV output of this function.
- `Depth Texture` [FunctionInput_Scalar] - This should be a linear depth texture. It should be in the 0-1 range with values closest to the camera being black and values further from the camera being white. This is inverse of typical depth/heightmaps but makes this function cheaper.

## Outputs

- `World Position Offset` - Pulle the object fowards the camera by object Radius. Connect this to WorldPositionOffset in the material.
- `Custom UVs` - This output needs to be hooked up to one of the material's unused CustomUV input pins. Then the matching TextureCoordinate node needs to be input on the INPUT side of this function node.
- `Pixel Depth Offset`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UVs | Custom UVs |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Depth Offset | Pixel Depth Offset |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 7 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDepthFade |  |
| 10 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionScalarParameter` | Param (1) 'PixelOffset' |  |
| 16 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | FoliageScaleFactor |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input WorldPositionOffset (Vector3) | WorldPositionOffset |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Dynamic Shadow Distance (Scalar) | Dynamic Shadow Distance |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Dynamic Shadow Fade Length (Scalar) | Dynamic Shadow Fade Length |
| 23 | expression | `MaterialExpressionMultiply` | Multiply(,0.1) |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Initial Radius (Scalar) | Initial Radius |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input CustomUV Input (Vector2) | CustomUV Input |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Depth Texture (Scalar) | Depth Texture |
| 27 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [8] Subtract output 0

### [1] Output Custom UVs

- `A` <= [11] Multiply output 0

### [2] Output Pixel Depth Offset

- `A` <= [12] Multiply output 0

### [3] Multiply

- `A` <= [7] Normalize output 0
- `B` <= [18] Multiply output 0

### [5] Subtract

- `A` <= [27] Add output 0
- `B` <= [4] Camera Position output 0

### [7] Normalize

- `VectorInput` <= [5] Subtract output 0

### [8] Subtract

- `A` <= [20] Input WorldPositionOffset (Vector3) output 0
- `B` <= [3] Multiply output 0

### [11] Multiply

- `A` <= [9] CameraDepthFade output 0
- `B` <= [19] FoliageScaleFactor output 0

### [12] Multiply

- `A` <= [13] Multiply(,2) output 0
- `B` <= [26] Input Depth Texture (Scalar) output 0

### [13] Multiply(,2)

- `A` <= [14] Multiply output 0

### [14] Multiply

- `A` <= [15] Param (1) 'PixelOffset' output 0
- `B` <= [17] Mask ( R ) output 0

### [17] Mask ( R )

- `Input` <= [25] Input CustomUV Input (Vector2) output 0

### [18] Multiply

- `A` <= [1] Output Custom UVs output 0
- `B` <= [15] Param (1) 'PixelOffset' output 0

### [22] Input Dynamic Shadow Fade Length (Scalar)

- `Preview` <= [23] Multiply(,0.1) output 0

### [23] Multiply(,0.1)

- `A` <= [21] Input Dynamic Shadow Distance (Scalar) output 0

### [25] Input CustomUV Input (Vector2)

- `Preview` <= [16] TexCoord[2] output 0

### [27] Add

- `A` <= [20] Input WorldPositionOffset (Vector3) output 0
- `B` <= [6] Absolute World Position (Excluding Material Offsets) output 0

## Connection List

- [8] Subtract output 0 -> Output World Position Offset.A
- [11] Multiply output 0 -> Output Custom UVs.A
- [12] Multiply output 0 -> Output Pixel Depth Offset.A
- [7] Normalize output 0 -> Multiply.A
- [18] Multiply output 0 -> Multiply.B
- [27] Add output 0 -> Subtract.A
- [4] Camera Position output 0 -> Subtract.B
- [5] Subtract output 0 -> Normalize.VectorInput
- [20] Input WorldPositionOffset (Vector3) output 0 -> Subtract.A
- [3] Multiply output 0 -> Subtract.B
- [9] CameraDepthFade output 0 -> Multiply.A
- [19] FoliageScaleFactor output 0 -> Multiply.B
- [13] Multiply(,2) output 0 -> Multiply.A
- [26] Input Depth Texture (Scalar) output 0 -> Multiply.B
- [14] Multiply output 0 -> Multiply(,2).A
- [15] Param (1) 'PixelOffset' output 0 -> Multiply.A
- [17] Mask ( R ) output 0 -> Multiply.B
- [25] Input CustomUV Input (Vector2) output 0 -> Mask ( R ).Input
- [1] Output Custom UVs output 0 -> Multiply.A
- [15] Param (1) 'PixelOffset' output 0 -> Multiply.B
- [23] Multiply(,0.1) output 0 -> Input Dynamic Shadow Fade Length (Scalar).Preview
- [21] Input Dynamic Shadow Distance (Scalar) output 0 -> Multiply(,0.1).A
- [16] TexCoord[2] output 0 -> Input CustomUV Input (Vector2).Preview
- [20] Input WorldPositionOffset (Vector3) output 0 -> Add.A
- [6] Absolute World Position (Excluding Material Offsets) output 0 -> Add.B

## Output Trace Roots

- `World Position Offset` is driven by [8] Subtract
- `Custom UVs` is driven by [11] Multiply
- `Pixel Depth Offset` is driven by [12] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
