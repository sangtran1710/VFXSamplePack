# InverseTransformMatrix

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/InverseTransformMatrix.InverseTransformMatrix`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Transform a vector into a new basis

## Inputs

- `BasisX` [FunctionInput_Vector3]
- `BasisY` [FunctionInput_Vector3]
- `BasisZ` [FunctionInput_Vector3]
- `VectorToTransform` [FunctionInput_Vector3]

## Outputs

- `Transformed Vector 3`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Transformed Vector 3 | Transformed Vector 3 |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input BasisX (Vector3) | BasisX |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input BasisY (Vector3) | BasisY |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input BasisZ (Vector3) | BasisZ |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input VectorToTransform (Vector3) | VectorToTransform |

## Exact Input Wiring

### [0] Output Transformed Vector 3

- `A` <= [4] Append output 0

### [1] Dot

- `A` <= [6] Input BasisX (Vector3) output 0
- `B` <= [9] Input VectorToTransform (Vector3) output 0

### [2] Append

- `A` <= [1] Dot output 0
- `B` <= [3] Dot output 0

### [3] Dot

- `A` <= [7] Input BasisY (Vector3) output 0
- `B` <= [9] Input VectorToTransform (Vector3) output 0

### [4] Append

- `A` <= [2] Append output 0
- `B` <= [5] Dot output 0

### [5] Dot

- `A` <= [8] Input BasisZ (Vector3) output 0
- `B` <= [9] Input VectorToTransform (Vector3) output 0

## Connection List

- [4] Append output 0 -> Output Transformed Vector 3.A
- [6] Input BasisX (Vector3) output 0 -> Dot.A
- [9] Input VectorToTransform (Vector3) output 0 -> Dot.B
- [1] Dot output 0 -> Append.A
- [3] Dot output 0 -> Append.B
- [7] Input BasisY (Vector3) output 0 -> Dot.A
- [9] Input VectorToTransform (Vector3) output 0 -> Dot.B
- [2] Append output 0 -> Append.A
- [5] Dot output 0 -> Append.B
- [8] Input BasisZ (Vector3) output 0 -> Dot.A
- [9] Input VectorToTransform (Vector3) output 0 -> Dot.B

## Output Trace Roots

- `Transformed Vector 3` is driven by [4] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


