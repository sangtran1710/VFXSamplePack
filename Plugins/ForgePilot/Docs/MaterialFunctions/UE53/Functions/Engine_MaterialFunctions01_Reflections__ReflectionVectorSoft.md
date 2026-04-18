# ReflectionVectorSoft

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/ReflectionVectorSoft.ReflectionVectorSoft`
- Category: Engine_MaterialFunctions01/Reflections
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Softens the normal for a smoother reflection

## Inputs

- `Normal` [FunctionInput_Vector3] - Normal used for the reflection to be softened
- `Softness` [FunctionInput_Scalar] - value from 0 to 1

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | CustomReflectionVector |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Softness (Scalar) | Softness |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] CustomReflectionVector output 0

### [5] Lerp

- `A` <= [3] Input Normal (Vector3) output 0
- `B` <= [2] 0,0,1 output 0
- `Alpha` <= [4] Input Softness (Scalar) output 0

## Connection List

- [1] CustomReflectionVector output 0 -> Output Result.A
- [3] Input Normal (Vector3) output 0 -> Lerp.A
- [2] 0,0,1 output 0 -> Lerp.B
- [4] Input Softness (Scalar) output 0 -> Lerp.Alpha

## Output Trace Roots

- `Result` is driven by [1] CustomReflectionVector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
