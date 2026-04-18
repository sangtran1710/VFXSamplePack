# MakeVectorsOrthogonal

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/MakeVectorsOrthogonal.MakeVectorsOrthogonal`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Makes vectors orthagonal by using the cross product of the vectors.

## Inputs

- `Vector1` [FunctionInput_Vector3]
- `Vector2` [FunctionInput_Vector3]
- `Vector3` [FunctionInput_Vector3]

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
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Vector2 | Vector2 |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Vector3 | Vector3 |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Vector3 (Vector3) | Vector3 |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 11 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 12 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 13 | expression | `MaterialExpressionCrossProduct` | Cross |  |

## Exact Input Wiring

### [0] Output Vector1

- `A` <= [11] Cross output 0

### [3] Cross

- `A` <= [9] Normalize output 0
- `B` <= [4] Normalize output 0

### [4] Normalize

- `VectorInput` <= [1] Input Vector1 (Vector3) output 0

### [5] Normalize

- `VectorInput` <= [2] Input Vector2 (Vector3) output 0

### [6] Output Vector2

- `A` <= [12] Cross output 0

### [7] Output Vector3

- `A` <= [13] Cross output 0

### [9] Normalize

- `VectorInput` <= [8] Input Vector3 (Vector3) output 0

### [10] Cross

- `A` <= [4] Normalize output 0
- `B` <= [5] Normalize output 0

### [11] Cross

- `A` <= [3] Cross output 0
- `B` <= [10] Cross output 0

### [12] Cross

- `A` <= [10] Cross output 0
- `B` <= [11] Cross output 0

### [13] Cross

- `A` <= [11] Cross output 0
- `B` <= [12] Cross output 0

## Connection List

- [11] Cross output 0 -> Output Vector1.A
- [9] Normalize output 0 -> Cross.A
- [4] Normalize output 0 -> Cross.B
- [1] Input Vector1 (Vector3) output 0 -> Normalize.VectorInput
- [2] Input Vector2 (Vector3) output 0 -> Normalize.VectorInput
- [12] Cross output 0 -> Output Vector2.A
- [13] Cross output 0 -> Output Vector3.A
- [8] Input Vector3 (Vector3) output 0 -> Normalize.VectorInput
- [4] Normalize output 0 -> Cross.A
- [5] Normalize output 0 -> Cross.B
- [3] Cross output 0 -> Cross.A
- [10] Cross output 0 -> Cross.B
- [10] Cross output 0 -> Cross.A
- [11] Cross output 0 -> Cross.B
- [11] Cross output 0 -> Cross.A
- [12] Cross output 0 -> Cross.B

## Output Trace Roots

- `Vector1` is driven by [11] Cross
- `Vector2` is driven by [12] Cross
- `Vector3` is driven by [13] Cross

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
