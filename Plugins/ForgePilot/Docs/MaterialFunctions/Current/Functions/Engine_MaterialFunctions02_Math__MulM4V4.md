# MulM4V4

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/MulM4V4.MulM4V4`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `MatrixRow1` [FunctionInput_Vector4]
- `MatrixRow3` [FunctionInput_Vector4]
- `MatrixRow4` [FunctionInput_Vector4]
- `MatrixRow2` [FunctionInput_Vector4]
- `Vector` [FunctionInput_Vector4]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input MatrixRow1 (Vector4) | MatrixRow1 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input MatrixRow3 (Vector4) | MatrixRow3 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input MatrixRow4 (Vector4) | MatrixRow4 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input MatrixRow2 (Vector4) | MatrixRow2 |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector4) | Vector |
| 6 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionNamedRerouteDeclaration` | Vector4 |  |
| 11 | expression | `MaterialExpressionNamedRerouteUsage` | Vector4 |  |
| 12 | expression | `MaterialExpressionNamedRerouteUsage` | Vector4 |  |
| 13 | expression | `MaterialExpressionNamedRerouteUsage` | Vector4 |  |
| 14 | expression | `MaterialExpressionNamedRerouteUsage` | Vector4 |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat4 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [15] MakeFloat4 output 0

### [6] Dot

- `A` <= [1] Input MatrixRow1 (Vector4) output 0
- `B` <= [11] Vector4 output 0

### [7] Dot

- `A` <= [4] Input MatrixRow2 (Vector4) output 0
- `B` <= [12] Vector4 output 0

### [8] Dot

- `A` <= [2] Input MatrixRow3 (Vector4) output 0
- `B` <= [13] Vector4 output 0

### [9] Dot

- `A` <= [3] Input MatrixRow4 (Vector4) output 0
- `B` <= [14] Vector4 output 0

### [10] Vector4

- `Input` <= [5] Input Vector (Vector4) output 0

## Connection List

- [15] MakeFloat4 output 0 -> Output Result.A
- [1] Input MatrixRow1 (Vector4) output 0 -> Dot.A
- [11] Vector4 output 0 -> Dot.B
- [4] Input MatrixRow2 (Vector4) output 0 -> Dot.A
- [12] Vector4 output 0 -> Dot.B
- [2] Input MatrixRow3 (Vector4) output 0 -> Dot.A
- [13] Vector4 output 0 -> Dot.B
- [3] Input MatrixRow4 (Vector4) output 0 -> Dot.A
- [14] Vector4 output 0 -> Dot.B
- [5] Input Vector (Vector4) output 0 -> Vector4.Input

## Output Trace Roots

- `Result` is driven by [15] MakeFloat4

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


