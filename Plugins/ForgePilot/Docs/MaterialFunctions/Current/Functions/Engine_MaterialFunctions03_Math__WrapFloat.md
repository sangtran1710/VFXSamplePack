# WrapFloat

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WrapFloat.WrapFloat`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Similar to fmod with support for negative floats : returns a value between 0 and Wrap

## Inputs

- `Value` [FunctionInput_Scalar]
- `Wrap` [FunctionInput_Scalar]

## Outputs

- `Result` - Wrapped around value

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionFloor` | Floor |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Value (Scalar) | Value |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Wrap (Scalar) | Wrap |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Subtract output 0

### [1] Multiply

- `A` <= [6] Input Wrap (Scalar) output 0
- `B` <= [3] Floor output 0

### [2] Divide

- `A` <= [5] Input Value (Scalar) output 0
- `B` <= [6] Input Wrap (Scalar) output 0

### [3] Floor

- `Input` <= [2] Divide output 0

### [4] Subtract

- `A` <= [5] Input Value (Scalar) output 0
- `B` <= [1] Multiply output 0

## Connection List

- [4] Subtract output 0 -> Output Result.A
- [6] Input Wrap (Scalar) output 0 -> Multiply.A
- [3] Floor output 0 -> Multiply.B
- [5] Input Value (Scalar) output 0 -> Divide.A
- [6] Input Wrap (Scalar) output 0 -> Divide.B
- [2] Divide output 0 -> Floor.Input
- [5] Input Value (Scalar) output 0 -> Subtract.A
- [1] Multiply output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [4] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


