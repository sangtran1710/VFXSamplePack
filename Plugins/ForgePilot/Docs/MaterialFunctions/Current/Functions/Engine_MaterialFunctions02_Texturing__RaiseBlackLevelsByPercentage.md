# RaiseBlackLevelsByPercentage

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/RaiseBlackLevelsByPercentage.RaiseBlackLevelsByPercentage`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Raises the black levels of an input value based on a percentage provided in the "black intensity level" parameter. Note: the image input values should range from 0-1.  

## Inputs

- `Image` [FunctionInput_Vector3]
- `Black intensity level` [FunctionInput_Vector3] - Enter 1 for unmodified black levels and 0 to raise the black levels to white. 

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Image (Vector3) | Image |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Black intensity level (Vector3) | Black intensity level |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] 1-x output 0

### [1] 1-x

- `Input` <= [4] Input Image (Vector3) output 0

### [2] Multiply

- `A` <= [1] 1-x output 0
- `B` <= [5] Input Black intensity level (Vector3) output 0

### [3] 1-x

- `Input` <= [2] Multiply output 0

## Connection List

- [3] 1-x output 0 -> Output Result.A
- [4] Input Image (Vector3) output 0 -> 1-x.Input
- [1] 1-x output 0 -> Multiply.A
- [5] Input Black intensity level (Vector3) output 0 -> Multiply.B
- [2] Multiply output 0 -> 1-x.Input

## Output Trace Roots

- `Result` is driven by [3] 1-x

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


