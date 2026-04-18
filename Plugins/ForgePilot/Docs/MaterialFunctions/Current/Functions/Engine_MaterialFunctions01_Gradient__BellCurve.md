# BellCurve

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/BellCurve.BellCurve`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Gives a Bell Curve also known as a Normal Distribution

## Inputs

- `x` [FunctionInput_Scalar] - The input to map to the curve

## Outputs

- `0-1` - Takes input on x from -1 to 1 and Returns values of 0-1
- `Standard` - Takes input on x from -pi to pi and Returns values scaled so the 2d area of the curve equals 1. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 0-1 | 0-1 |
| 1 | expression | `MaterialExpressionConstant` | 2.718 |  |
| 2 | expression | `MaterialExpressionPower` | Power |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input x (Scalar) | x |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-0.5) |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Standard | Standard |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,-0.5) |  |
| 10 | expression | `MaterialExpressionPower` | Power |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 13 | expression | `MaterialExpressionConstant` | 2 |  |
| 14 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output 0-1

- `A` <= [2] Power output 0

### [2] Power

- `Base` <= [1] 2.718 output 0
- `Exponent` <= [5] Multiply(,-0.5) output 0

### [3] Multiply

- `A` <= [15] Multiply output 0
- `B` <= [15] Multiply output 0

### [5] Multiply(,-0.5)

- `A` <= [3] Multiply output 0

### [7] Output Standard

- `A` <= [11] Divide output 0

### [8] Multiply

- `A` <= [4] Input x (Scalar) output 0
- `B` <= [4] Input x (Scalar) output 0

### [9] Multiply(,-0.5)

- `A` <= [8] Multiply output 0

### [10] Power

- `Base` <= [1] 2.718 output 0
- `Exponent` <= [9] Multiply(,-0.5) output 0

### [11] Divide

- `A` <= [10] Power output 0
- `B` <= [14] Sqrt output 0

### [14] Sqrt

- `Input` <= [12] Pi output 0

### [15] Multiply

- `A` <= [4] Input x (Scalar) output 0
- `B` <= [6] Pi output 0

## Connection List

- [2] Power output 0 -> Output 0-1.A
- [1] 2.718 output 0 -> Power.Base
- [5] Multiply(,-0.5) output 0 -> Power.Exponent
- [15] Multiply output 0 -> Multiply.A
- [15] Multiply output 0 -> Multiply.B
- [3] Multiply output 0 -> Multiply(,-0.5).A
- [11] Divide output 0 -> Output Standard.A
- [4] Input x (Scalar) output 0 -> Multiply.A
- [4] Input x (Scalar) output 0 -> Multiply.B
- [8] Multiply output 0 -> Multiply(,-0.5).A
- [1] 2.718 output 0 -> Power.Base
- [9] Multiply(,-0.5) output 0 -> Power.Exponent
- [10] Power output 0 -> Divide.A
- [14] Sqrt output 0 -> Divide.B
- [12] Pi output 0 -> Sqrt.Input
- [4] Input x (Scalar) output 0 -> Multiply.A
- [6] Pi output 0 -> Multiply.B

## Output Trace Roots

- `0-1` is driven by [2] Power
- `Standard` is driven by [11] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


