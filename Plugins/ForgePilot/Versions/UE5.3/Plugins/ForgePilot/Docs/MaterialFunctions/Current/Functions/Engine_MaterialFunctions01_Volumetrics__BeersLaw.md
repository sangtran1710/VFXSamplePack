# BeersLaw

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Volumetrics/BeersLaw.BeersLaw`
- Category: Engine_MaterialFunctions01/Volumetrics
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns e^(-d) which is a standard exponential density function.

## Inputs

- `Depth Scale` [FunctionInput_Scalar] - This is simply a multiplier for the Thicknesss input. You can use this to make some objects more dense, such as rainclouds.
- `Thickness` [FunctionInput_Scalar] - The distance a ray travelled through some medium.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Depth Scale (Scalar) | Depth Scale |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Thickness (Scalar) | Thickness |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionPower` | Power |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 6 | expression | `MaterialExpressionConstant` | 2.71 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Power output 0

### [3] Multiply

- `A` <= [2] Input Thickness (Scalar) output 0
- `B` <= [1] Input Depth Scale (Scalar) output 0

### [4] Power

- `Base` <= [6] 2.71 output 0
- `Exponent` <= [5] Multiply(,-1) output 0

### [5] Multiply(,-1)

- `A` <= [3] Multiply output 0

## Connection List

- [4] Power output 0 -> Output Result.A
- [2] Input Thickness (Scalar) output 0 -> Multiply.A
- [1] Input Depth Scale (Scalar) output 0 -> Multiply.B
- [6] 2.71 output 0 -> Power.Base
- [5] Multiply(,-1) output 0 -> Power.Exponent
- [3] Multiply output 0 -> Multiply(,-1).A

## Output Trace Roots

- `Result` is driven by [4] Power

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


