# RadiansToDegrees

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/RadiansToDegrees.RadiansToDegrees`
- Category: Engine_MaterialFunctions01/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts from Radians to Degrees.

## Inputs

- `Radians` [FunctionInput_Scalar]

## Outputs

- `Degrees`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Degrees | Degrees |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Radians (Scalar) | Radians |
| 2 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |

## Exact Input Wiring

### [0] Output Degrees

- `A` <= [2] Multiply(,57.3) output 0

### [2] Multiply(,57.3)

- `A` <= [1] Input Radians (Scalar) output 0

## Connection List

- [2] Multiply(,57.3) output 0 -> Output Degrees.A
- [1] Input Radians (Scalar) output 0 -> Multiply(,57.3).A

## Output Trace Roots

- `Degrees` is driven by [2] Multiply(,57.3)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


