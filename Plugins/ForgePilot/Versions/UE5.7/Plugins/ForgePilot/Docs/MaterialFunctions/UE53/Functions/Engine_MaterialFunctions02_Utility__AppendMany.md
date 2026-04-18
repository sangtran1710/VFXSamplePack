# AppendMany

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/AppendMany.AppendMany`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Allows you to quickly append up to 4 scalars in one function

## Inputs

- `R` [FunctionInput_Scalar]
- `G` [FunctionInput_Scalar]
- `B` [FunctionInput_Scalar]
- `A` [FunctionInput_Scalar]

## Outputs

- `RG`
- `RGB`
- `RGBA`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RG | RG |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input R (Scalar) | R |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input G (Scalar) | G |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input B (Scalar) | B |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input A (Scalar) | A |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output RGBA | RGBA |

## Exact Input Wiring

### [0] Output RG

- `A` <= [2] Append output 0

### [2] Append

- `A` <= [1] Input R (Scalar) output 0
- `B` <= [3] Input G (Scalar) output 0

### [4] Append

- `A` <= [2] Append output 0
- `B` <= [5] Input B (Scalar) output 0

### [7] Append

- `A` <= [4] Append output 0
- `B` <= [6] Input A (Scalar) output 0

### [8] Output RGB

- `A` <= [4] Append output 0

### [9] Output RGBA

- `A` <= [7] Append output 0

## Connection List

- [2] Append output 0 -> Output RG.A
- [1] Input R (Scalar) output 0 -> Append.A
- [3] Input G (Scalar) output 0 -> Append.B
- [2] Append output 0 -> Append.A
- [5] Input B (Scalar) output 0 -> Append.B
- [4] Append output 0 -> Append.A
- [6] Input A (Scalar) output 0 -> Append.B
- [4] Append output 0 -> Output RGB.A
- [7] Append output 0 -> Output RGBA.A

## Output Trace Roots

- `RG` is driven by [2] Append
- `RGB` is driven by [4] Append
- `RGBA` is driven by [7] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
