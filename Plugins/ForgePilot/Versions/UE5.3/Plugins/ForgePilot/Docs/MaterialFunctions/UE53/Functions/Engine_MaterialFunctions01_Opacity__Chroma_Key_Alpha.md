# Chroma_Key_Alpha

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/Chroma_Key_Alpha.Chroma_Key_Alpha`
- Category: Engine_MaterialFunctions01/Opacity
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Extracts an alpha mask from a chroma key aka green screen input image.

## Inputs

- `Image Color` [FunctionInput_Vector3] - The input image to create a mask from.
- `Chroma Color` [FunctionInput_Vector3] - The chroma color aka green screen color to match and remove from the input image color.
- `Alpha Cutoff Min` [FunctionInput_Scalar] - The initial comparison mask will be clipped to black at this specified value.
- `Alpha Cutoff Max` [FunctionInput_Scalar] - The initial comparison mask will reach full saturation or white at this level. 
- `Alpha Exponent` [FunctionInput_Scalar] - This applies an exponent to the comparison mask which affects how soft or hard it is. Larger number will cause the mask to have more contrast and the white area to shrink.
- `Despill Cuttoff Max` [FunctionInput_Scalar] - This is the maximum level from the original alpha mask where saturation will be reached. The "Alpha Cutoff Min" parameter also applies to the despill output before this cutoff.
- `Despill Exponent` [FunctionInput_Scalar] - This parameter applies an exponent to the despill mask which allows tweaking of the despill gradient sharpness.
- `LumaMask` [FunctionInput_Scalar]

## Outputs

- `Alpha` - Returns black where the chroma key color is and white where the rest of the image is.
- `Raw Comparison` - Returns the absolute value of the comparison between input color and chroma color. Useful for visualizing the mask before cutoff and power parameters.
- `Despill Alpha` - Useful for desaturating greenscreen color or adding in a hint of ambient bloom or lighting from the new scene.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Image Color (Vector3) | Image Color |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Chroma Color (Vector3) | Chroma Color |
| 3 | expression | `MaterialExpressionConstant3Vector` | 0.122,0.765,0.261 |  |
| 4 | expression | `MaterialExpressionPower` | Power |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Alpha Cutoff Min (Scalar) | Alpha Cutoff Min |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Alpha Cutoff Max (Scalar) | Alpha Cutoff Max |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Alpha Exponent (Scalar) | Alpha Exponent |
| 13 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Raw Comparison | Raw Comparison |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Despill Cuttoff Max (Scalar) | Despill Cuttoff Max |
| 16 | expression | `MaterialExpressionPower` | Power |  |
| 17 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Despill Exponent (Scalar) | Despill Exponent |
| 19 | expression | `MaterialExpressionClamp` | Clamp |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Despill Alpha | Despill Alpha |
| 21 | expression | `MaterialExpressionDivide` | Divide |  |
| 22 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 23 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input LumaMask (Scalar) | LumaMask |
| 25 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 26 | custom | `MaterialExpressionCustom` | ExtractColor |  |
| 27 | custom | `MaterialExpressionCustom` | ExtractColor |  |
| 28 | expression | `MaterialExpressionConstant` | 0.2 |  |

## Exact Input Wiring

### [0] Output Alpha

- `A` <= [8] Clamp output 0

### [2] Input Chroma Color (Vector3)

- `Preview` <= [3] 0.122,0.765,0.261 output 0

### [4] Power

- `Base` <= [13] Max(,0) output 0
- `Exponent` <= [12] Input Alpha Exponent (Scalar) output 0

### [8] Clamp

- `Input` <= [4] Power output 0

### [9] Subtract

- `A` <= [6] VectorLength output 0
- `B` <= [5] Input Alpha Cutoff Min (Scalar) output 0

### [10] Subtract

- `A` <= [7] Input Alpha Cutoff Max (Scalar) output 0
- `B` <= [5] Input Alpha Cutoff Min (Scalar) output 0

### [11] Divide

- `A` <= [9] Subtract output 0
- `B` <= [10] Subtract output 0

### [13] Max(,0)

- `A` <= [11] Divide output 0

### [14] Output Raw Comparison

- `A` <= [6] VectorLength output 0

### [16] Power

- `Base` <= [23] Max(,0) output 0
- `Exponent` <= [18] Input Despill Exponent (Scalar) output 0

### [17] 1-x

- `Input` <= [16] Power output 0

### [19] Clamp

- `Input` <= [17] 1-x output 0

### [20] Output Despill Alpha

- `A` <= [19] Clamp output 0

### [21] Divide

- `A` <= [9] Subtract output 0
- `B` <= [22] Subtract output 0

### [22] Subtract

- `A` <= [15] Input Despill Cuttoff Max (Scalar) output 0
- `B` <= [5] Input Alpha Cutoff Min (Scalar) output 0

### [23] Max(,0)

- `A` <= [21] Divide output 0

### [24] Input LumaMask (Scalar)

- `Preview` <= [28] 0.2 output 0

### [25] Subtract

- `A` <= [26] ExtractColor output 0
- `B` <= [27] ExtractColor output 0

## Connection List

- [8] Clamp output 0 -> Output Alpha.A
- [3] 0.122,0.765,0.261 output 0 -> Input Chroma Color (Vector3).Preview
- [13] Max(,0) output 0 -> Power.Base
- [12] Input Alpha Exponent (Scalar) output 0 -> Power.Exponent
- [4] Power output 0 -> Clamp.Input
- [6] VectorLength output 0 -> Subtract.A
- [5] Input Alpha Cutoff Min (Scalar) output 0 -> Subtract.B
- [7] Input Alpha Cutoff Max (Scalar) output 0 -> Subtract.A
- [5] Input Alpha Cutoff Min (Scalar) output 0 -> Subtract.B
- [9] Subtract output 0 -> Divide.A
- [10] Subtract output 0 -> Divide.B
- [11] Divide output 0 -> Max(,0).A
- [6] VectorLength output 0 -> Output Raw Comparison.A
- [23] Max(,0) output 0 -> Power.Base
- [18] Input Despill Exponent (Scalar) output 0 -> Power.Exponent
- [16] Power output 0 -> 1-x.Input
- [17] 1-x output 0 -> Clamp.Input
- [19] Clamp output 0 -> Output Despill Alpha.A
- [9] Subtract output 0 -> Divide.A
- [22] Subtract output 0 -> Divide.B
- [15] Input Despill Cuttoff Max (Scalar) output 0 -> Subtract.A
- [5] Input Alpha Cutoff Min (Scalar) output 0 -> Subtract.B
- [21] Divide output 0 -> Max(,0).A
- [28] 0.2 output 0 -> Input LumaMask (Scalar).Preview
- [26] ExtractColor output 0 -> Subtract.A
- [27] ExtractColor output 0 -> Subtract.B

## Output Trace Roots

- `Alpha` is driven by [8] Clamp
- `Raw Comparison` is driven by [6] VectorLength
- `Despill Alpha` is driven by [19] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
