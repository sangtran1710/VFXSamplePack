# TransformNormals_Tangent_to_Vertex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TransformNormals_Tangent_to_Vertex.TransformNormals_Tangent_to_Vertex`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Useful for when you want a World Aligned texture to pick up on mesh vertex normals but NOT tangent vectors.

## Inputs

- `Vector to Transform` [FunctionInput_Vector3]
- `Squash Vertex Normals` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 4 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,-1,) |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionCeil` | Ceil |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 10 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 11 | expression | `MaterialExpressionConstant` | 1 |  |
| 12 | expression | `MaterialExpressionConstant` | 1 |  |
| 13 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Vector to Transform (Vector3) | Vector to Transform |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Squash Vertex Normals (Vector3) | Squash Vertex Normals |
| 17 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Transform3x3Matrix output 0

### [5] Multiply

- `A` <= [14] Input Vector to Transform (Vector3) output 0
- `B` <= [9] MakeFloat3 output 0

### [6] Lerp(1,-1,)

- `Alpha` <= [8] Ceil output 0

### [7] Dot

- `A` <= [13] VertexNormalWS output 0
- `B` <= [10] 0,0,1 output 0

### [8] Ceil

- `Input` <= [7] Dot output 0

### [15] Multiply

- `A` <= [4] VertexNormalWS output 0
- `B` <= [16] Input Squash Vertex Normals (Vector3) output 0

### [17] Normalize

- `VectorInput` <= [15] Multiply output 0

## Connection List

- [3] Transform3x3Matrix output 0 -> Output Result.A
- [14] Input Vector to Transform (Vector3) output 0 -> Multiply.A
- [9] MakeFloat3 output 0 -> Multiply.B
- [8] Ceil output 0 -> Lerp(1,-1,).Alpha
- [13] VertexNormalWS output 0 -> Dot.A
- [10] 0,0,1 output 0 -> Dot.B
- [7] Dot output 0 -> Ceil.Input
- [4] VertexNormalWS output 0 -> Multiply.A
- [16] Input Squash Vertex Normals (Vector3) output 0 -> Multiply.B
- [15] Multiply output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Result` is driven by [3] Transform3x3Matrix

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


