# CreateThirdOrthogonalVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/CreateThirdOrthogonalVector.CreateThirdOrthogonalVector`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Takes two vectors and generates a third orthogonal vector from them. It also ensures that all of the vectors are orthogonal to each other.  The first vectors directon is favored over the second.

## Inputs

- `Vector1` [FunctionInput_Vector3]
- `Vector2` [FunctionInput_Vector3]

## Outputs

- `Vector1`
- `Vector2`
- `Vector3`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Vector1 | Vector1 |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector1 (Vector3) | Vector1 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Vector2 (Vector3) | Vector2 |
| 3 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 6 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Vector2 | Vector2 |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Vector3 | Vector3 |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |

## Exact Input Wiring

### [0] Output Vector1

- `A` <= [4] Normalize output 0

### [3] Cross

- `A` <= [4] Normalize output 0
- `B` <= [5] Normalize output 0

### [4] Normalize

- `VectorInput` <= [1] Input Vector1 (Vector3) output 0

### [5] Normalize

- `VectorInput` <= [2] Input Vector2 (Vector3) output 0

### [6] Cross

- `A` <= [3] Cross output 0
- `B` <= [4] Normalize output 0

### [7] Output Vector2

- `A` <= [12] SafeNormalize output 0

### [8] Output Vector3

- `A` <= [11] SafeNormalize output 0

### [9] Normalize

- `VectorInput` <= [6] Cross output 0

### [10] Normalize

- `VectorInput` <= [3] Cross output 0

## Connection List

- [4] Normalize output 0 -> Output Vector1.A
- [4] Normalize output 0 -> Cross.A
- [5] Normalize output 0 -> Cross.B
- [1] Input Vector1 (Vector3) output 0 -> Normalize.VectorInput
- [2] Input Vector2 (Vector3) output 0 -> Normalize.VectorInput
- [3] Cross output 0 -> Cross.A
- [4] Normalize output 0 -> Cross.B
- [12] SafeNormalize output 0 -> Output Vector2.A
- [11] SafeNormalize output 0 -> Output Vector3.A
- [6] Cross output 0 -> Normalize.VectorInput
- [3] Cross output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Vector1` is driven by [4] Normalize
- `Vector2` is driven by [12] SafeNormalize
- `Vector3` is driven by [11] SafeNormalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
