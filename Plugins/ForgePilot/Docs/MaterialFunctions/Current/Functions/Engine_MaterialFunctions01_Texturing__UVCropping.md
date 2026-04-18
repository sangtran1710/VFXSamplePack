# UVCropping

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/UVCropping.UVCropping`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Creates a cropped region to be multiplied to texture

## Inputs

- `Crop Left` [FunctionInput_Scalar]
- `Crop Right` [FunctionInput_Scalar]
- `Crop Bottom` [FunctionInput_Scalar]
- `Crop Top` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | expression | `MaterialExpressionConstant` | 1 |  |
| 3 | expression | `MaterialExpressionConstant` | 0 |  |
| 4 | expression | `MaterialExpressionIf` | If |  |
| 5 | expression | `MaterialExpressionIf` | If |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionConstant` | 0 |  |
| 8 | expression | `MaterialExpressionConstant` | 1 |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionIf` | If |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 13 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 15 | expression | `MaterialExpressionConstant` | 1 |  |
| 16 | expression | `MaterialExpressionConstant` | 1 |  |
| 17 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Crop Left (Scalar) | Crop Left |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Crop Right (Scalar) | Crop Right |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Crop Bottom (Scalar) | Crop Bottom |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Crop Top (Scalar) | Crop Top |

## Exact Input Wiring

### [0] Output Result

- `A` <= [10] Multiply output 0

### [1] If

- `A` <= [12] BreakOutFloat2Components output 0
- `B` <= [18] Input Crop Left (Scalar) output 0
- `AGreaterThanB` <= [2] 1 output 0
- `ALessThanB` <= [3] 0 output 0

### [4] If

- `A` <= [12] BreakOutFloat2Components output 1
- `B` <= [17] Subtract output 0
- `AGreaterThanB` <= [7] 0 output 0
- `ALessThanB` <= [8] 1 output 0

### [5] If

- `A` <= [12] BreakOutFloat2Components output 0
- `B` <= [14] Subtract output 0
- `AGreaterThanB` <= [3] 0 output 0
- `ALessThanB` <= [2] 1 output 0

### [6] Multiply

- `A` <= [1] If output 0
- `B` <= [5] If output 0

### [9] Multiply

- `A` <= [4] If output 0
- `B` <= [11] If output 0

### [10] Multiply

- `A` <= [6] Multiply output 0
- `B` <= [9] Multiply output 0

### [11] If

- `A` <= [12] BreakOutFloat2Components output 1
- `B` <= [21] Input Crop Top (Scalar) output 0
- `AGreaterThanB` <= [8] 1 output 0
- `ALessThanB` <= [7] 0 output 0

### [14] Subtract

- `A` <= [15] 1 output 0
- `B` <= [19] Input Crop Right (Scalar) output 0

### [17] Subtract

- `A` <= [16] 1 output 0
- `B` <= [20] Input Crop Bottom (Scalar) output 0

## Connection List

- [10] Multiply output 0 -> Output Result.A
- [12] BreakOutFloat2Components output 0 -> If.A
- [18] Input Crop Left (Scalar) output 0 -> If.B
- [2] 1 output 0 -> If.AGreaterThanB
- [3] 0 output 0 -> If.ALessThanB
- [12] BreakOutFloat2Components output 1 -> If.A
- [17] Subtract output 0 -> If.B
- [7] 0 output 0 -> If.AGreaterThanB
- [8] 1 output 0 -> If.ALessThanB
- [12] BreakOutFloat2Components output 0 -> If.A
- [14] Subtract output 0 -> If.B
- [3] 0 output 0 -> If.AGreaterThanB
- [2] 1 output 0 -> If.ALessThanB
- [1] If output 0 -> Multiply.A
- [5] If output 0 -> Multiply.B
- [4] If output 0 -> Multiply.A
- [11] If output 0 -> Multiply.B
- [6] Multiply output 0 -> Multiply.A
- [9] Multiply output 0 -> Multiply.B
- [12] BreakOutFloat2Components output 1 -> If.A
- [21] Input Crop Top (Scalar) output 0 -> If.B
- [8] 1 output 0 -> If.AGreaterThanB
- [7] 0 output 0 -> If.ALessThanB
- [15] 1 output 0 -> Subtract.A
- [19] Input Crop Right (Scalar) output 0 -> Subtract.B
- [16] 1 output 0 -> Subtract.A
- [20] Input Crop Bottom (Scalar) output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [10] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


