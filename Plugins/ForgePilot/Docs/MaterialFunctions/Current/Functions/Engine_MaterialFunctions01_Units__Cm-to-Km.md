# Cm-to-Km

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Units/Cm-to-Km.Cm-to-Km`
- Category: Engine_MaterialFunctions01/Units
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts between Centimeters and Kilometers

## Inputs

- `Cm` [FunctionInput_Scalar]

## Outputs

- `Km`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Km | Km |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Cm (Scalar) | Cm |
| 2 | expression | `MaterialExpressionDivide` | Divide(,1e+05) |  |

## Exact Input Wiring

### [0] Output Km

- `A` <= [2] Divide(,1e+05) output 0

### [2] Divide(,1e+05)

- `A` <= [1] Input Cm (Scalar) output 0

## Connection List

- [2] Divide(,1e+05) output 0 -> Output Km.A
- [1] Input Cm (Scalar) output 0 -> Divide(,1e+05).A

## Output Trace Roots

- `Km` is driven by [2] Divide(,1e+05)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


