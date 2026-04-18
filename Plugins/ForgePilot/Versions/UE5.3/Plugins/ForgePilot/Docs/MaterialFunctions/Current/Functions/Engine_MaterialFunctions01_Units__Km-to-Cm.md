# Km-to-Cm

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Units/Km-to-Cm.Km-to-Cm`
- Category: Engine_MaterialFunctions01/Units
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts between Kilometers and Centimeters.

## Inputs

- `Km` [FunctionInput_Scalar]

## Outputs

- `Cm`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Cm | Cm |
| 1 | expression | `MaterialExpressionMultiply` | Multiply(,1e+05) |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Km (Scalar) | Km |

## Exact Input Wiring

### [0] Output Cm

- `A` <= [1] Multiply(,1e+05) output 0

### [1] Multiply(,1e+05)

- `A` <= [2] Input Km (Scalar) output 0

## Connection List

- [1] Multiply(,1e+05) output 0 -> Output Cm.A
- [2] Input Km (Scalar) output 0 -> Multiply(,1e+05).A

## Output Trace Roots

- `Cm` is driven by [1] Multiply(,1e+05)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


