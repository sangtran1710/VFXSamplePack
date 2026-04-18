# DiamondGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/DiamondGradient.DiamondGradient`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Uses UV Channel 0 to generate a Diamond Gradient

## Inputs

- `Falloff` [FunctionInput_Scalar] - Power Exponent for the Diamond

## Outputs

- `DiamondGradient`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output DiamondGradient | DiamondGradient |
| 1 | expression | `MaterialExpressionPower` | Power |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Falloff (Scalar) | Falloff |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionConstant` | 2 |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | expression | `MaterialExpressionAbs` | Abs |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output DiamondGradient

- `A` <= [1] Power output 0

### [1] Power

- `Base` <= [3] Multiply output 0
- `Exponent` <= [2] Input Falloff (Scalar) output 0

### [3] Multiply

- `A` <= [11] 1-x output 0
- `B` <= [4] 1-x output 0

### [4] 1-x

- `Input` <= [5] Abs output 0

### [5] Abs

- `Input` <= [6] Multiply output 0

### [6] Multiply

- `A` <= [8] Add output 0
- `B` <= [7] 2 output 0

### [8] Add

- `A` <= [10] LinearGradient output 1
- `B` <= [9] -0.5 output 0

### [11] 1-x

- `Input` <= [12] Abs output 0

### [12] Abs

- `Input` <= [13] Multiply output 0

### [13] Multiply

- `A` <= [14] Add output 0
- `B` <= [7] 2 output 0

### [14] Add

- `A` <= [10] LinearGradient output 0
- `B` <= [9] -0.5 output 0

## Connection List

- [1] Power output 0 -> Output DiamondGradient.A
- [3] Multiply output 0 -> Power.Base
- [2] Input Falloff (Scalar) output 0 -> Power.Exponent
- [11] 1-x output 0 -> Multiply.A
- [4] 1-x output 0 -> Multiply.B
- [5] Abs output 0 -> 1-x.Input
- [6] Multiply output 0 -> Abs.Input
- [8] Add output 0 -> Multiply.A
- [7] 2 output 0 -> Multiply.B
- [10] LinearGradient output 1 -> Add.A
- [9] -0.5 output 0 -> Add.B
- [12] Abs output 0 -> 1-x.Input
- [13] Multiply output 0 -> Abs.Input
- [14] Add output 0 -> Multiply.A
- [7] 2 output 0 -> Multiply.B
- [10] LinearGradient output 0 -> Add.A
- [9] -0.5 output 0 -> Add.B

## Output Trace Roots

- `DiamondGradient` is driven by [1] Power

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
