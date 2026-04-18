# ComponentWise_SphereMask

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Masking/ComponentWise_SphereMask.ComponentWise_SphereMask`
- Category: Engine_MaterialFunctions02/Masking
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Apply a sphere mask for each channel of a vector independently.

## Inputs

- `A` [FunctionInput_Vector3]
- `B` [FunctionInput_Vector3]
- `Radius` [FunctionInput_Vector3]
- `Hardness` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input A (Vector3) | A |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input B (Vector3) | B |
| 4 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 5 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 6 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Vector3) | Radius |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Hardness (Vector3) | Hardness |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | AppendMany |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [12] AppendMany output 1

### [4] SphereMask

- `A` <= [2] BreakOutFloat3Components output 0
- `B` <= [7] BreakOutFloat3Components output 0
- `Radius` <= [9] BreakOutFloat3Components output 0
- `Hardness` <= [11] BreakOutFloat3Components output 0

### [5] SphereMask

- `A` <= [2] BreakOutFloat3Components output 1
- `B` <= [7] BreakOutFloat3Components output 1
- `Radius` <= [9] BreakOutFloat3Components output 1
- `Hardness` <= [11] BreakOutFloat3Components output 1

### [6] SphereMask

- `A` <= [2] BreakOutFloat3Components output 2
- `B` <= [7] BreakOutFloat3Components output 2
- `Radius` <= [9] BreakOutFloat3Components output 2
- `Hardness` <= [11] BreakOutFloat3Components output 2

## Connection List

- [12] AppendMany output 1 -> Output Result.A
- [2] BreakOutFloat3Components output 0 -> SphereMask.A
- [7] BreakOutFloat3Components output 0 -> SphereMask.B
- [9] BreakOutFloat3Components output 0 -> SphereMask.Radius
- [11] BreakOutFloat3Components output 0 -> SphereMask.Hardness
- [2] BreakOutFloat3Components output 1 -> SphereMask.A
- [7] BreakOutFloat3Components output 1 -> SphereMask.B
- [9] BreakOutFloat3Components output 1 -> SphereMask.Radius
- [11] BreakOutFloat3Components output 1 -> SphereMask.Hardness
- [2] BreakOutFloat3Components output 2 -> SphereMask.A
- [7] BreakOutFloat3Components output 2 -> SphereMask.B
- [9] BreakOutFloat3Components output 2 -> SphereMask.Radius
- [11] BreakOutFloat3Components output 2 -> SphereMask.Hardness

## Output Trace Roots

- `Result` is driven by [12] AppendMany

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
