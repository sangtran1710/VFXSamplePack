# DeleteReference4x4Matrix

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/DeleteReference4x4Matrix.DeleteReference4x4Matrix`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Transform a vector into a new basis

## Inputs

- `BasisX` [FunctionInput_Vector4]
- `BasisY` [FunctionInput_Vector4]
- `BasisZ` [FunctionInput_Vector4]
- `BasisPosition` [FunctionInput_Vector4]
- `VectorToTransform` [FunctionInput_Vector4] - The alpha can be a 1 if the vector contains a position and a 0 if the basis contains direction. 

## Outputs

- `Transformed Vector 4`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Transformed Vector 4 | Transformed Vector 4 |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input BasisX (Vector4) | BasisX |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input BasisY (Vector4) | BasisY |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input BasisZ (Vector4) | BasisZ |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input BasisPosition (Vector4) | BasisPosition |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input VectorToTransform (Vector4) | VectorToTransform |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |

## Exact Input Wiring

### [0] Output Transformed Vector 4

- `A` <= [10] Add output 0

### [5] Multiply

- `A` <= [1] Input BasisX (Vector4) output 0
- `B` <= [13] BreakOutFloat4Components output 0

### [6] Multiply

- `A` <= [2] Input BasisY (Vector4) output 0
- `B` <= [13] BreakOutFloat4Components output 1

### [7] Multiply

- `A` <= [3] Input BasisZ (Vector4) output 0
- `B` <= [13] BreakOutFloat4Components output 2

### [8] Add

- `A` <= [5] Multiply output 0
- `B` <= [6] Multiply output 0

### [9] Add

- `A` <= [7] Multiply output 0
- `B` <= [11] Multiply output 0

### [10] Add

- `A` <= [8] Add output 0
- `B` <= [9] Add output 0

### [11] Multiply

- `A` <= [4] Input BasisPosition (Vector4) output 0
- `B` <= [13] BreakOutFloat4Components output 3

## Connection List

- [10] Add output 0 -> Output Transformed Vector 4.A
- [1] Input BasisX (Vector4) output 0 -> Multiply.A
- [13] BreakOutFloat4Components output 0 -> Multiply.B
- [2] Input BasisY (Vector4) output 0 -> Multiply.A
- [13] BreakOutFloat4Components output 1 -> Multiply.B
- [3] Input BasisZ (Vector4) output 0 -> Multiply.A
- [13] BreakOutFloat4Components output 2 -> Multiply.B
- [5] Multiply output 0 -> Add.A
- [6] Multiply output 0 -> Add.B
- [7] Multiply output 0 -> Add.A
- [11] Multiply output 0 -> Add.B
- [8] Add output 0 -> Add.A
- [9] Add output 0 -> Add.B
- [4] Input BasisPosition (Vector4) output 0 -> Multiply.A
- [13] BreakOutFloat4Components output 3 -> Multiply.B

## Output Trace Roots

- `Transformed Vector 4` is driven by [10] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
