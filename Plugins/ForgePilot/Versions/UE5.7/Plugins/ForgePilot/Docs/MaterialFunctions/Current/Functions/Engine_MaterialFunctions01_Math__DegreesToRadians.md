# DegreesToRadians

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/DegreesToRadians.DegreesToRadians`
- Category: Engine_MaterialFunctions01/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts from Degrees to Radians.

## Inputs

- `Degrees` [FunctionInput_Scalar]

## Outputs

- `Radians`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Radians | Radians |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Degrees (Scalar) | Degrees |
| 2 | expression | `MaterialExpressionDivide` | Divide(,57.3) |  |

## Exact Input Wiring

### [0] Output Radians

- `A` <= [2] Divide(,57.3) output 0

### [2] Divide(,57.3)

- `A` <= [1] Input Degrees (Scalar) output 0

## Connection List

- [2] Divide(,57.3) output 0 -> Output Radians.A
- [1] Input Degrees (Scalar) output 0 -> Divide(,57.3).A

## Output Trace Roots

- `Radians` is driven by [2] Divide(,57.3)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


