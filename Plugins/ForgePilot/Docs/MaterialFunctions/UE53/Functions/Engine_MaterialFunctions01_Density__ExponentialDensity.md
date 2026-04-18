# ExponentialDensity

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Density/ExponentialDensity.ExponentialDensity`
- Category: Engine_MaterialFunctions01/Density
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Uses the D3DFOG_EXP function to map exponential density. The result is inverted so that 0 is returned when depth = 0 and approaches 1 as depth increases.

## Inputs

- `Depth` [FunctionInput_Scalar]
- `Density` [FunctionInput_Scalar]
- `Use Exp2` [FunctionInput_StaticBool] - Switch to the D3DFOG_EXP2 version (more contrast)

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Depth (Scalar) | Depth |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Density (Scalar) | Density |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionConstant` | 2.718 |  |
| 5 | expression | `MaterialExpressionPower` | Power |  |
| 6 | expression | `MaterialExpressionConstant` | 1 |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionIf` | If |  |
| 9 | expression | `MaterialExpressionConstant` | 0 |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Use Exp2 (StaticBool) | Use Exp2 |
| 11 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 14 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] If output 0

### [3] Multiply

- `A` <= [1] Input Depth (Scalar) output 0
- `B` <= [2] Input Density (Scalar) output 0

### [5] Power

- `Base` <= [4] 2.718 output 0
- `Exponent` <= [11] Switch output 0

### [7] Divide

- `A` <= [6] 1 output 0
- `B` <= [5] Power output 0

### [8] If

- `A` <= [1] Input Depth (Scalar) output 0
- `B` <= [9] 0 output 0
- `AGreaterThanB` <= [7] Divide output 0
- `AEqualsB` <= [14] 1 output 0
- `ALessThanB` <= [14] 1 output 0

### [10] Input Use Exp2 (StaticBool)

- `Preview` <= [13] Static Bool (True) output 0

### [11] Switch

- `A` <= [12] Multiply output 0
- `B` <= [3] Multiply output 0
- `Value` <= [10] Input Use Exp2 (StaticBool) output 0

### [12] Multiply

- `A` <= [3] Multiply output 0
- `B` <= [3] Multiply output 0

## Connection List

- [8] If output 0 -> Output Result.A
- [1] Input Depth (Scalar) output 0 -> Multiply.A
- [2] Input Density (Scalar) output 0 -> Multiply.B
- [4] 2.718 output 0 -> Power.Base
- [11] Switch output 0 -> Power.Exponent
- [6] 1 output 0 -> Divide.A
- [5] Power output 0 -> Divide.B
- [1] Input Depth (Scalar) output 0 -> If.A
- [9] 0 output 0 -> If.B
- [7] Divide output 0 -> If.AGreaterThanB
- [14] 1 output 0 -> If.AEqualsB
- [14] 1 output 0 -> If.ALessThanB
- [13] Static Bool (True) output 0 -> Input Use Exp2 (StaticBool).Preview
- [12] Multiply output 0 -> Switch.A
- [3] Multiply output 0 -> Switch.B
- [10] Input Use Exp2 (StaticBool) output 0 -> Switch.Value
- [3] Multiply output 0 -> Multiply.A
- [3] Multiply output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [8] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
