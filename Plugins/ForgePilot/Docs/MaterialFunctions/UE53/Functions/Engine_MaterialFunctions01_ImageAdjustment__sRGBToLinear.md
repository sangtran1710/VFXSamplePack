# sRGBToLinear

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/sRGBToLinear.sRGBToLinear`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Convert sRGB to Linear

## Inputs

- `sRGB` [FunctionInput_Scalar] - sRGB Value

## Outputs

- `Linear`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Linear | Linear |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input sRGB (Scalar) | sRGB |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | expression | `MaterialExpressionConstant` | 0.04045 |  |
| 4 | expression | `MaterialExpressionPower` | Power |  |
| 5 | expression | `MaterialExpressionConstant` | 2.4 |  |
| 6 | expression | `MaterialExpressionConstant` | 0.0774 |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant` | 0.055 |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionConstant` | 0.9479 |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Linear

- `A` <= [2] If output 0

### [2] If

- `A` <= [1] Input sRGB (Scalar) output 0
- `B` <= [3] 0.04045 output 0
- `AGreaterThanB` <= [4] Power output 0
- `AEqualsB` <= [7] Multiply output 0
- `ALessThanB` <= [7] Multiply output 0

### [4] Power

- `Base` <= [11] Multiply output 0
- `Exponent` <= [5] 2.4 output 0

### [7] Multiply

- `A` <= [1] Input sRGB (Scalar) output 0
- `B` <= [6] 0.0774 output 0

### [9] Add

- `A` <= [1] Input sRGB (Scalar) output 0
- `B` <= [8] 0.055 output 0

### [11] Multiply

- `A` <= [9] Add output 0
- `B` <= [10] 0.9479 output 0

## Connection List

- [2] If output 0 -> Output Linear.A
- [1] Input sRGB (Scalar) output 0 -> If.A
- [3] 0.04045 output 0 -> If.B
- [4] Power output 0 -> If.AGreaterThanB
- [7] Multiply output 0 -> If.AEqualsB
- [7] Multiply output 0 -> If.ALessThanB
- [11] Multiply output 0 -> Power.Base
- [5] 2.4 output 0 -> Power.Exponent
- [1] Input sRGB (Scalar) output 0 -> Multiply.A
- [6] 0.0774 output 0 -> Multiply.B
- [1] Input sRGB (Scalar) output 0 -> Add.A
- [8] 0.055 output 0 -> Add.B
- [9] Add output 0 -> Multiply.A
- [10] 0.9479 output 0 -> Multiply.B

## Output Trace Roots

- `Linear` is driven by [2] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
