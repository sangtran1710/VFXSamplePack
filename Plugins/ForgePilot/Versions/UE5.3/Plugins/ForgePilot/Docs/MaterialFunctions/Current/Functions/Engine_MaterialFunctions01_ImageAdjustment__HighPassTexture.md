# HighPassTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/HighPassTexture.HighPassTexture`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs a high pass type operation on an input texture object. This filters out low frequency information.

## Inputs

- `Texture Object` [FunctionInput_Texture2D]
- `Contrast` [FunctionInput_Scalar]
- `UVs` [FunctionInput_Vector2]
- `Highpass Offset` [FunctionInput_Scalar] - The width of the highpass offset, in texels.
- `High Quality` [FunctionInput_StaticBool] - When set to true, performs 8 sample instead of 4 to perform an average. Set to False by default.

## Outputs

- `Result` - The high pass result.
- `Result No Contrast` - The high pass result.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Texture Object (Texture2D) | Texture Object |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | BlurSampleOffsets |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Highpass Offset (Scalar) | Highpass Offset |
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
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionDivide` | Divide(,4) |  |
| 21 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 22 | expression | `MaterialExpressionPower` | Power |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input High Quality (StaticBool) | High Quality |
| 24 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionAdd` | Add |  |
| 28 | expression | `MaterialExpressionAdd` | Add |  |
| 29 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 30 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 31 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 32 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionAdd` | Add |  |
| 35 | expression | `MaterialExpressionAdd` | Add |  |
| 36 | expression | `MaterialExpressionAdd` | Add |  |
| 37 | expression | `MaterialExpressionDivide` | Divide(,8) |  |
| 38 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 39 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 40 | expression | `MaterialExpressionTextureProperty` | Texture Size |  |
| 41 | expression | `MaterialExpressionDivide` | Divide |  |
| 42 | function_output | `MaterialExpressionFunctionOutput` | Output Result No Contrast | Result No Contrast |

## Exact Input Wiring

### [0] Output Result

- `A` <= [39] Multiply(,0.5) output 0

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

### [19] Subtract

- `A` <= [7] Texture Sample output 0
- `B` <= [38] Switch output 0

### [20] Divide(,4)

- `A` <= [18] Add output 0

### [21] Add(,1)

- `A` <= [42] Output Result No Contrast output 0

### [22] Power

- `Base` <= [21] Add(,1) output 0
- `Exponent` <= [2] Input Contrast (Scalar) output 0

### [23] Input High Quality (StaticBool)

- `Preview` <= [24] Static Bool (False) output 0

### [25] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 7

### [26] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 6

### [27] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 5

### [28] Add

- `A` <= [3] Input UVs (Vector2) output 0
- `B` <= [5] BlurSampleOffsets output 4

### [29] Texture Sample

- `Coordinates` <= [25] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [30] Texture Sample

- `Coordinates` <= [26] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [31] Texture Sample

- `Coordinates` <= [27] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [32] Texture Sample

- `Coordinates` <= [28] Add output 0
- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [33] Add

- `A` <= [35] Add output 0
- `B` <= [34] Add output 0

### [34] Add

- `A` <= [30] Texture Sample output 0
- `B` <= [29] Texture Sample output 0

### [35] Add

- `A` <= [32] Texture Sample output 0
- `B` <= [31] Texture Sample output 0

### [36] Add

- `A` <= [18] Add output 0
- `B` <= [33] Add output 0

### [37] Divide(,8)

- `A` <= [36] Add output 0

### [38] Switch

- `A` <= [37] Divide(,8) output 0
- `B` <= [20] Divide(,4) output 0
- `Value` <= [23] Input High Quality (StaticBool) output 0

### [39] Multiply(,0.5)

- `A` <= [22] Power output 0

### [40] Texture Size

- `TextureObject` <= [1] Input Texture Object (Texture2D) output 0

### [41] Divide

- `A` <= [6] Input Highpass Offset (Scalar) output 0
- `B` <= [40] Texture Size output 0

### [42] Output Result No Contrast

- `A` <= [19] Subtract output 0

## Connection List

- [39] Multiply(,0.5) output 0 -> Output Result.A
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
- [7] Texture Sample output 0 -> Subtract.A
- [38] Switch output 0 -> Subtract.B
- [18] Add output 0 -> Divide(,4).A
- [42] Output Result No Contrast output 0 -> Add(,1).A
- [21] Add(,1) output 0 -> Power.Base
- [2] Input Contrast (Scalar) output 0 -> Power.Exponent
- [24] Static Bool (False) output 0 -> Input High Quality (StaticBool).Preview
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 7 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 6 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 5 -> Add.B
- [3] Input UVs (Vector2) output 0 -> Add.A
- [5] BlurSampleOffsets output 4 -> Add.B
- [25] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [26] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [27] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [28] Add output 0 -> Texture Sample.Coordinates
- [1] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [35] Add output 0 -> Add.A
- [34] Add output 0 -> Add.B
- [30] Texture Sample output 0 -> Add.A
- [29] Texture Sample output 0 -> Add.B
- [32] Texture Sample output 0 -> Add.A
- [31] Texture Sample output 0 -> Add.B
- [18] Add output 0 -> Add.A
- [33] Add output 0 -> Add.B
- [36] Add output 0 -> Divide(,8).A
- [37] Divide(,8) output 0 -> Switch.A
- [20] Divide(,4) output 0 -> Switch.B
- [23] Input High Quality (StaticBool) output 0 -> Switch.Value
- [22] Power output 0 -> Multiply(,0.5).A
- [1] Input Texture Object (Texture2D) output 0 -> Texture Size.TextureObject
- [6] Input Highpass Offset (Scalar) output 0 -> Divide.A
- [40] Texture Size output 0 -> Divide.B
- [19] Subtract output 0 -> Output Result No Contrast.A

## Output Trace Roots

- `Result` is driven by [39] Multiply(,0.5)
- `Result No Contrast` is driven by [19] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


