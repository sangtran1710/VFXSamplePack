# MultiplyAdd

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/MultiplyAdd.MultiplyAdd`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Modulate the "add" input by by the base input and then add the result to the base input.

## Inputs

- `Add` [FunctionInput_Vector3]
- `Base` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Add (Vector3) | Add |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Add output 0

### [1] Multiply

- `A` <= [3] Input Add (Vector3) output 0
- `B` <= [4] Input Base (Vector3) output 0

### [2] Add

- `A` <= [1] Multiply output 0
- `B` <= [4] Input Base (Vector3) output 0

## Connection List

- [2] Add output 0 -> Output Result.A
- [3] Input Add (Vector3) output 0 -> Multiply.A
- [4] Input Base (Vector3) output 0 -> Multiply.B
- [1] Multiply output 0 -> Add.A
- [4] Input Base (Vector3) output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [2] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
