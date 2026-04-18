# Luminosity_And_Color

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/Luminosity_And_Color.Luminosity_And_Color`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function takes an input color and an input luminosity and merges them.

## Inputs

- `Color` [FunctionInput_Vector3] - The color that will be applied. It is normalized inside of the function.
- `Luminosity` [FunctionInput_Scalar] - Luminosity used for result.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Luminosity (Scalar) | Luminosity |
| 3 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Multiply output 0

### [3] Normalize

- `VectorInput` <= [1] Input Color (Vector3) output 0

### [4] Multiply

- `A` <= [3] Normalize output 0
- `B` <= [2] Input Luminosity (Scalar) output 0

## Connection List

- [4] Multiply output 0 -> Output Result.A
- [1] Input Color (Vector3) output 0 -> Normalize.VectorInput
- [3] Normalize output 0 -> Multiply.A
- [2] Input Luminosity (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [4] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
