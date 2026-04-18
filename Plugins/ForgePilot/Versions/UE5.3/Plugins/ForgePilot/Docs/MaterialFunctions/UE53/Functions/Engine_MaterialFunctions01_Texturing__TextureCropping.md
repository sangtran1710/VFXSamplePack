# TextureCropping

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TextureCropping.TextureCropping`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Crops a texture, useful for Emissives.  Supply the top left and bottom right corners of the crop  in 0-1 range.

## Inputs

- `UpperLeftCorner` [FunctionInput_Vector2] - New UV location for UV value (0,0).
- `LowerRightCorner` [FunctionInput_Vector2] - New location for uv location (1,1).
- `TextureIn` [FunctionInput_Texture2D] - Texture to be positioned and cropped, make sure it's Address is set to Clamp
- `UVs` [FunctionInput_Vector2] - Default value is texture coordinate 0.
- `ExportFloat4` [FunctionInput_StaticBool] - False will export an RGB result and True will export a RGBA result.

## Outputs

- `CroppedMasked` - 11 instructions with parameter 7 without
- `Cropped` - 7 Instructions with parameters 2 without
- `Crop UVs` - 7 Instructions with parameters 2 without
- `Crop Mask` - 11 instructions with parameter 7 without

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output CroppedMasked | CroppedMasked |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 4 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input UpperLeftCorner (Vector2) | UpperLeftCorner |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input LowerRightCorner (Vector2) | LowerRightCorner |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant` | -1 |  |
| 13 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 14 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 15 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 16 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input TextureIn (Texture2D) | TextureIn |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Cropped | Cropped |
| 19 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Crop UVs | Crop UVs |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Crop Mask | Crop Mask |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input ExportFloat4 (StaticBool) | ExportFloat4 |
| 24 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 25 | expression | `MaterialExpressionAppendVector` | Append |  |
| 26 | expression | `MaterialExpressionStaticSwitch` | Switch |  |

## Exact Input Wiring

### [0] Output CroppedMasked

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [26] Switch output 0
- `B` <= [2] Multiply output 0

### [2] Multiply

- `A` <= [14] SphereMask output 0
- `B` <= [3] SphereMask output 0

### [3] SphereMask

- `A` <= [5] Mask ( G ) output 0
- `B` <= [4] 0.5 output 0
- `Radius` <= [4] 0.5 output 0

### [5] Mask ( G )

- `Input` <= [6] Divide output 0

### [6] Divide

- `A` <= [10] Add output 0
- `B` <= [7] Subtract output 0

### [7] Subtract

- `A` <= [9] Input LowerRightCorner (Vector2) output 0
- `B` <= [8] Input UpperLeftCorner (Vector2) output 0

### [10] Add

- `A` <= [22] Input UVs (Vector2) output 0
- `B` <= [11] Multiply output 0

### [11] Multiply

- `A` <= [12] -1 output 0
- `B` <= [8] Input UpperLeftCorner (Vector2) output 0

### [14] SphereMask

- `A` <= [15] Mask ( R ) output 0
- `B` <= [4] 0.5 output 0
- `Radius` <= [4] 0.5 output 0

### [15] Mask ( R )

- `Input` <= [6] Divide output 0

### [16] Texture Sample

- `Coordinates` <= [6] Divide output 0
- `TextureObject` <= [17] Input TextureIn (Texture2D) output 0

### [17] Input TextureIn (Texture2D)

- `Preview` <= [19] Texture Object output 0

### [18] Output Cropped

- `A` <= [26] Switch output 0

### [20] Output Crop UVs

- `A` <= [6] Divide output 0

### [21] Output Crop Mask

- `A` <= [2] Multiply output 0

### [22] Input UVs (Vector2)

- `Preview` <= [13] TexCoord[0] output 0

### [23] Input ExportFloat4 (StaticBool)

- `Preview` <= [24] Static Bool (False) output 0

### [25] Append

- `A` <= [16] Texture Sample output 0
- `B` <= [16] Texture Sample output 4

### [26] Switch

- `A` <= [25] Append output 0
- `B` <= [16] Texture Sample output 0
- `Value` <= [23] Input ExportFloat4 (StaticBool) output 0

## Connection List

- [1] Multiply output 0 -> Output CroppedMasked.A
- [26] Switch output 0 -> Multiply.A
- [2] Multiply output 0 -> Multiply.B
- [14] SphereMask output 0 -> Multiply.A
- [3] SphereMask output 0 -> Multiply.B
- [5] Mask ( G ) output 0 -> SphereMask.A
- [4] 0.5 output 0 -> SphereMask.B
- [4] 0.5 output 0 -> SphereMask.Radius
- [6] Divide output 0 -> Mask ( G ).Input
- [10] Add output 0 -> Divide.A
- [7] Subtract output 0 -> Divide.B
- [9] Input LowerRightCorner (Vector2) output 0 -> Subtract.A
- [8] Input UpperLeftCorner (Vector2) output 0 -> Subtract.B
- [22] Input UVs (Vector2) output 0 -> Add.A
- [11] Multiply output 0 -> Add.B
- [12] -1 output 0 -> Multiply.A
- [8] Input UpperLeftCorner (Vector2) output 0 -> Multiply.B
- [15] Mask ( R ) output 0 -> SphereMask.A
- [4] 0.5 output 0 -> SphereMask.B
- [4] 0.5 output 0 -> SphereMask.Radius
- [6] Divide output 0 -> Mask ( R ).Input
- [6] Divide output 0 -> Texture Sample.Coordinates
- [17] Input TextureIn (Texture2D) output 0 -> Texture Sample.TextureObject
- [19] Texture Object output 0 -> Input TextureIn (Texture2D).Preview
- [26] Switch output 0 -> Output Cropped.A
- [6] Divide output 0 -> Output Crop UVs.A
- [2] Multiply output 0 -> Output Crop Mask.A
- [13] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [24] Static Bool (False) output 0 -> Input ExportFloat4 (StaticBool).Preview
- [16] Texture Sample output 0 -> Append.A
- [16] Texture Sample output 4 -> Append.B
- [25] Append output 0 -> Switch.A
- [16] Texture Sample output 0 -> Switch.B
- [23] Input ExportFloat4 (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `CroppedMasked` is driven by [1] Multiply
- `Cropped` is driven by [26] Switch
- `Crop UVs` is driven by [6] Divide
- `Crop Mask` is driven by [2] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
