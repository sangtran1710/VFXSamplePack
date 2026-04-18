# LinearTosRGB

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/LinearTosRGB.LinearTosRGB`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Convert Linear to sRGB

## Inputs

- `Linear` [FunctionInput_Scalar] - Linear Value

## Outputs

- `sRGB`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output sRGB | sRGB |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Linear (Scalar) | Linear |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | expression | `MaterialExpressionConstant` | 0.003131 |  |
| 4 | expression | `MaterialExpressionPower` | Power |  |
| 5 | expression | `MaterialExpressionConstant` | 0.4167 |  |
| 6 | expression | `MaterialExpressionConstant` | 12.92 |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant` | -0.055 |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionConstant` | 1.055 |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output sRGB

- `A` <= [2] If output 0

### [2] If

- `A` <= [1] Input Linear (Scalar) output 0
- `B` <= [3] 0.003131 output 0
- `AGreaterThanB` <= [9] Add output 0
- `AEqualsB` <= [7] Multiply output 0
- `ALessThanB` <= [7] Multiply output 0

### [4] Power

- `Base` <= [1] Input Linear (Scalar) output 0
- `Exponent` <= [5] 0.4167 output 0

### [7] Multiply

- `A` <= [1] Input Linear (Scalar) output 0
- `B` <= [6] 12.92 output 0

### [9] Add

- `A` <= [11] Multiply output 0
- `B` <= [8] -0.055 output 0

### [11] Multiply

- `A` <= [4] Power output 0
- `B` <= [10] 1.055 output 0

## Connection List

- [2] If output 0 -> Output sRGB.A
- [1] Input Linear (Scalar) output 0 -> If.A
- [3] 0.003131 output 0 -> If.B
- [9] Add output 0 -> If.AGreaterThanB
- [7] Multiply output 0 -> If.AEqualsB
- [7] Multiply output 0 -> If.ALessThanB
- [1] Input Linear (Scalar) output 0 -> Power.Base
- [5] 0.4167 output 0 -> Power.Exponent
- [1] Input Linear (Scalar) output 0 -> Multiply.A
- [6] 12.92 output 0 -> Multiply.B
- [11] Multiply output 0 -> Add.A
- [8] -0.055 output 0 -> Add.B
- [4] Power output 0 -> Multiply.A
- [10] 1.055 output 0 -> Multiply.B

## Output Trace Roots

- `sRGB` is driven by [2] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
