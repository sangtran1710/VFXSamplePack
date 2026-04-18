# DiffColorKeyerErodeSinglePass

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DiffColorKeyerErodeSinglePass.DiffColorKeyerErodeSinglePass`
- Category: Engine_MaterialFunctions01/Chromakeying
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Color difference based keyer

## Inputs

- `TextureExternal` [FunctionInput_TextureExternal]
- `KeyColor` [FunctionInput_Vector3]
- `KernalSize` [FunctionInput_Scalar]
- `NumSamples` [FunctionInput_Scalar]
- `UnPremult` [FunctionInput_Scalar]
- `AlphaThreshold` [FunctionInput_Scalar]
- `AlphaOffset` [FunctionInput_Scalar]
- `WeightRed` [FunctionInput_Scalar]
- `WeightBlue` [FunctionInput_Scalar]
- `ClipBlack` [FunctionInput_Scalar]
- `ClipWhite` [FunctionInput_Scalar]
- `ErodeAlpha` [FunctionInput_StaticBool]
- `---------------------` [FunctionInput_StaticBool] - ---------------------
- `IsExternalSource` [FunctionInput_StaticBool]
- `Texture2D` [FunctionInput_Texture2D]

## Outputs

- `RGB`
- `Alpha`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 1 | custom | `MaterialExpressionCustom` | ErodeAlphaDiffColor |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionAppendVector` | Append |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input TextureExternal (External) | TextureExternal |
| 9 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 10 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input KeyColor (Vector3) | KeyColor |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input KernalSize (Scalar) | KernalSize |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input NumSamples (Scalar) | NumSamples |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input UnPremult (Scalar) | UnPremult |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input AlphaThreshold (Scalar) | AlphaThreshold |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input AlphaOffset (Scalar) | AlphaOffset |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input WeightRed (Scalar) | WeightRed |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input WeightBlue (Scalar) | WeightBlue |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input ClipBlack (Scalar) | ClipBlack |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input ClipWhite (Scalar) | ClipWhite |
| 21 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input ErodeAlpha (StaticBool) | ErodeAlpha |
| 23 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input --------------------- (StaticBool) | --------------------- |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input IsExternalSource (StaticBool) | IsExternalSource |
| 26 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 27 | expression | `MaterialExpressionConstant2Vector` | 1.02e+03,1.02e+03 |  |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Texture2D (Texture2D) | Texture2D |
| 29 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 30 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 31 | function_call | `MaterialExpressionMaterialFunctionCall` | DiffColorKeyer |  |

## Exact Input Wiring

### [0] Output RGB

- `A` <= [5] Mask ( R G B ) output 0

### [2] Append

- `A` <= [15] Input AlphaThreshold (Scalar) output 0
- `B` <= [16] Input AlphaOffset (Scalar) output 0

### [3] Append

- `A` <= [17] Input WeightRed (Scalar) output 0
- `B` <= [18] Input WeightBlue (Scalar) output 0

### [4] Append

- `A` <= [19] Input ClipBlack (Scalar) output 0
- `B` <= [20] Input ClipWhite (Scalar) output 0

### [5] Mask ( R G B )

- `Input` <= [21] Switch output 0

### [6] Mask ( A )

- `Input` <= [21] Switch output 0

### [7] Output Alpha

- `A` <= [6] Mask ( A ) output 0

### [9] Texture Sample

- `Coordinates` <= [10] TexCoord[0] output 0
- `TextureObject` <= [26] Switch output 0

### [21] Switch

- `A` <= [1] ErodeAlphaDiffColor output 0
- `B` <= [31] DiffColorKeyer output 0
- `Value` <= [22] Input ErodeAlpha (StaticBool) output 0

### [22] Input ErodeAlpha (StaticBool)

- `Preview` <= [23] Static Bool (False) output 0

### [25] Input IsExternalSource (StaticBool)

- `Preview` <= [29] Static Bool (False) output 0

### [26] Switch

- `A` <= [8] Input TextureExternal (External) output 0
- `B` <= [28] Input Texture2D (Texture2D) output 0
- `Value` <= [25] Input IsExternalSource (StaticBool) output 0

### [28] Input Texture2D (Texture2D)

- `Preview` <= [30] Texture Object output 0

## Connection List

- [5] Mask ( R G B ) output 0 -> Output RGB.A
- [15] Input AlphaThreshold (Scalar) output 0 -> Append.A
- [16] Input AlphaOffset (Scalar) output 0 -> Append.B
- [17] Input WeightRed (Scalar) output 0 -> Append.A
- [18] Input WeightBlue (Scalar) output 0 -> Append.B
- [19] Input ClipBlack (Scalar) output 0 -> Append.A
- [20] Input ClipWhite (Scalar) output 0 -> Append.B
- [21] Switch output 0 -> Mask ( R G B ).Input
- [21] Switch output 0 -> Mask ( A ).Input
- [6] Mask ( A ) output 0 -> Output Alpha.A
- [10] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [26] Switch output 0 -> Texture Sample.TextureObject
- [1] ErodeAlphaDiffColor output 0 -> Switch.A
- [31] DiffColorKeyer output 0 -> Switch.B
- [22] Input ErodeAlpha (StaticBool) output 0 -> Switch.Value
- [23] Static Bool (False) output 0 -> Input ErodeAlpha (StaticBool).Preview
- [29] Static Bool (False) output 0 -> Input IsExternalSource (StaticBool).Preview
- [8] Input TextureExternal (External) output 0 -> Switch.A
- [28] Input Texture2D (Texture2D) output 0 -> Switch.B
- [25] Input IsExternalSource (StaticBool) output 0 -> Switch.Value
- [30] Texture Object output 0 -> Input Texture2D (Texture2D).Preview

## Output Trace Roots

- `RGB` is driven by [5] Mask ( R G B )
- `Alpha` is driven by [6] Mask ( A )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


