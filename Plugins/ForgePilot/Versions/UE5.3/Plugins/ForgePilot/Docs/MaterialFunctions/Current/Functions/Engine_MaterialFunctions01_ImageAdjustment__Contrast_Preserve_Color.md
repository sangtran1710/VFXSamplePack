# Contrast_Preserve_Color

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/Contrast_Preserve_Color.Contrast_Preserve_Color`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

This function allows you to modify the contrast of an image without affecting the relative color values.

## Inputs

- `Color` [FunctionInput_Vector3] - The input image. Luminosity will be calculated by desaturating this input.
- `Contrast` [FunctionInput_Scalar] - This input is applied as an exponent to the luminosity only.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 5 | expression | `MaterialExpressionPower` | Power |  |
| 6 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 7 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [2] Normalize output 0
- `B` <= [5] Power output 0

### [2] Normalize

- `VectorInput` <= [3] Input Color (Vector3) output 0

### [5] Power

- `Base` <= [6] Desaturation output 0
- `Exponent` <= [4] Input Contrast (Scalar) output 0

### [6] Desaturation

- `Input` <= [3] Input Color (Vector3) output 0
- `Fraction` <= [7] 1 output 0

## Connection List

- [1] Multiply output 0 -> Output Result.A
- [2] Normalize output 0 -> Multiply.A
- [5] Power output 0 -> Multiply.B
- [3] Input Color (Vector3) output 0 -> Normalize.VectorInput
- [6] Desaturation output 0 -> Power.Base
- [4] Input Contrast (Scalar) output 0 -> Power.Exponent
- [3] Input Color (Vector3) output 0 -> Desaturation.Input
- [7] 1 output 0 -> Desaturation.Fraction

## Output Trace Roots

- `Result` is driven by [1] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


