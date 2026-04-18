# UnSharpMaskTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/UnSharpMaskTexture.UnSharpMaskTexture`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs a high pass type operation on an input texture object. This filters out low frequency information.

## Inputs

- `Texture Object` [FunctionInput_Texture2D]
- `Amount` [FunctionInput_Scalar]
- `UVs` [FunctionInput_Vector2]
- `Sample Offset` [FunctionInput_Scalar] - The width of the unsharp mask offset in texels.
- `High Quality` [FunctionInput_StaticBool] - When set to true, performs 8 sample instead of 4 to perform an average. Set to False by default.
- `TextureSize` [FunctionInput_Scalar] - Size of Texture. This overrides the derived texture size which often does not work for parameters.

## Outputs

- `Result` - The high pass result.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Texture Object (Texture2D) | Texture Object |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Amount (Scalar) | Amount |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | BlurSampleOffsets |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Sample Offset (Scalar) | Sample Offset |
| 7 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 8 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 15 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionAdd` | Add |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | expression | `MaterialExpressionDivide` | Divide(,4) |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input High Quality (StaticBool) | High Quality |
| 21 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 22 | expression | `MaterialExpressionAdd` | Add |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 27 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 28 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 29 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 30 | expression | `MaterialExpressionAdd` | Add |  |
| 31 | expression | `MaterialExpressionAdd` | Add |  |
| 32 | expression | `MaterialExpressionAdd` | Add |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionDivide` | Divide(,8) |  |
| 35 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 36 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 39 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 40 | expression | `MaterialExpressionTextureProperty` | Texture Size |  |
| 41 | expression | `MaterialExpressionDivide` | Divide |  |
| 42 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (Scalar) | TextureSize |

## Exact Input Wiring

### [0] Output Result

- `A` <= [39] Subtract output 0

### [3] Input UVs (Vector2)

- `Preview` <= [4] TexCoord[0] output 0

### [7] Texture Sample

- `Coordinates` <= [3] Input UVs (Vector2) output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [8] Texture Sample

- `Coordinates` <= [9] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [9] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 0

### [10] Texture Sample

- `Coordinates` <= [11] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [11] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 1

### [12] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 2

### [13] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 3

### [14] Texture Sample

- `Coordinates` <= [12] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [15] Texture Sample

- `Coordinates` <= [13] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [16] Add

- `A` <= [8] Texture Sample output 0
- `B` <= [10] Texture Sample output 0

### [17] Add

- `A` <= [14] Texture Sample output 0
- `B` <= [15] Texture Sample output 0

### [18] Add

- `A` <= [16] Add output 0
- `B` <= [17] Add output 0

### [19] Divide(,4)

- `A` <= [18] Add output 0

### [20] Input High Quality (StaticBool)

- `Preview` <= [21] Static Bool (False) output 0

### [22] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 7

### [23] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 6

### [24] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 5

### [25] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 4

### [26] Texture Sample

- `Coordinates` <= [22] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [27] Texture Sample

- `Coordinates` <= [23] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [28] Texture Sample

- `Coordinates` <= [24] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [29] Texture Sample

- `Coordinates` <= [25] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [30] Add

- `A` <= [32] Add output 0
- `B` <= [31] Add output 0

### [31] Add

- `A` <= [27] Texture Sample output 0
- `B` <= [26] Texture Sample output 0

### [32] Add

- `A` <= [29] Texture Sample output 0
- `B` <= [28] Texture Sample output 0

### [33] Add

- `A` <= [18] Add output 0
- `B` <= [30] Add output 0

### [34] Divide(,8)

- `A` <= [33] Add output 0

### [35] Switch

- `A` <= [34] Divide(,8) output 0
- `B` <= [19] Divide(,4) output 0
- `Value` <= [20] Input High Quality (StaticBool) output 0

### [36] Multiply

- `A` <= [2] Input Amount (Scalar) output 0
- `B` <= [35] Switch output 0

### [37] Multiply

- `A` <= [7] Texture Sample output 0
- `B` <= [38] Add(,1) output 0

### [38] Add(,1)

- `A` <= [2] Input Amount (Scalar) output 0

### [39] Subtract

- `A` <= [37] Multiply output 0
- `B` <= [36] Multiply output 0

### [40] Texture Size

- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [41] Divide

- `A` <= [6] Input Sample Offset (Scalar) output 0
- `B` <= [42] Input TextureSize (Scalar) output 0

### [42] Input TextureSize (Scalar)

- `Preview` <= [40] Texture Size output 0

## Connection List

- [39] Subtract output 0 -> Output Result.A
- [4] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [3] Input UVs (Vector2) output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [9] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 0 -> Add.B
- [11] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 1 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 2 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 3 -> Add.B
- [12] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [13] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Texture Sample output 0 -> Add.A
- [10] Texture Sample output 0 -> Add.B
- [14] Texture Sample output 0 -> Add.A
- [15] Texture Sample output 0 -> Add.B
- [16] Add output 0 -> Add.A
- [17] Add output 0 -> Add.B
- [18] Add output 0 -> Divide(,4).A
- [21] Static Bool (False) output 0 -> Input High Quality (StaticBool).Preview
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 7 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 6 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 5 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 4 -> Add.B
- [22] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [23] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [24] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [25] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [32] Add output 0 -> Add.A
- [31] Add output 0 -> Add.B
- [27] Texture Sample output 0 -> Add.A
- [26] Texture Sample output 0 -> Add.B
- [29] Texture Sample output 0 -> Add.A
- [28] Texture Sample output 0 -> Add.B
- [18] Add output 0 -> Add.A
- [30] Add output 0 -> Add.B
- [33] Add output 0 -> Divide(,8).A
- [34] Divide(,8) output 0 -> Switch.A
- [19] Divide(,4) output 0 -> Switch.B
- [20] Input High Quality (StaticBool) output 0 -> Switch.Value
- [2] Input Amount (Scalar) output 0 -> Multiply.A
- [35] Switch output 0 -> Multiply.B
- [7] Texture Sample output 0 -> Multiply.A
- [38] Add(,1) output 0 -> Multiply.B
- [2] Input Amount (Scalar) output 0 -> Add(,1).A
- [37] Multiply output 0 -> Subtract.A
- [36] Multiply output 0 -> Subtract.B
- [1] Input Texture Object (Texture2D) output 0 -> Texture Size.TextureObject
- [6] Input Sample Offset (Scalar) output 0 -> Divide.A
- [42] Input TextureSize (Scalar) output 0 -> Divide.B
- [40] Texture Size output 0 -> Input TextureSize (Scalar).Preview

## Output Trace Roots

- `Result` is driven by [39] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
