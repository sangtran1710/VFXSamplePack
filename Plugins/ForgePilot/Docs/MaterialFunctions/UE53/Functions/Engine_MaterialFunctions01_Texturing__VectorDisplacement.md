# VectorDisplacement

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/VectorDisplacement.VectorDisplacement`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Use a Baked Vector displacement map from Xnormal

## Inputs

- `Vector` [FunctionInput_Vector3] - RGB of Vector displacement map, should have sRGB disabled on texture properties
- `ScalarDisplacement` [FunctionInput_Vector3] - alpha of displacement map
- `Min` [FunctionInput_Scalar]
- `Max` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input ScalarDisplacement (Vector3) | ScalarDisplacement |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 5 | expression | `MaterialExpressionConstant3Vector` | -1,-1,-1 |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Min (Scalar) | Min |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Max (Scalar) | Max |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] Tangent Space to World Space TransformVector output 0

### [3] Lerp

- `A` <= [5] -1,-1,-1 output 0
- `B` <= [4] 1,1,1 output 0
- `Alpha` <= [1] Input Vector (Vector3) output 0

### [6] Multiply

- `A` <= [3] Lerp output 0
- `B` <= [7] Lerp output 0

### [7] Lerp

- `A` <= [12] Append output 0
- `B` <= [14] Append output 0
- `Alpha` <= [2] Input ScalarDisplacement (Vector3) output 0

### [8] Tangent Space to World Space TransformVector

- `Input` <= [6] Multiply output 0

### [11] Append

- `A` <= [9] Input Min (Scalar) output 0
- `B` <= [9] Input Min (Scalar) output 0

### [12] Append

- `A` <= [11] Append output 0
- `B` <= [9] Input Min (Scalar) output 0

### [13] Append

- `A` <= [10] Input Max (Scalar) output 0
- `B` <= [10] Input Max (Scalar) output 0

### [14] Append

- `A` <= [13] Append output 0
- `B` <= [10] Input Max (Scalar) output 0

## Connection List

- [8] Tangent Space to World Space TransformVector output 0 -> Output Result.A
- [5] -1,-1,-1 output 0 -> Lerp.A
- [4] 1,1,1 output 0 -> Lerp.B
- [1] Input Vector (Vector3) output 0 -> Lerp.Alpha
- [3] Lerp output 0 -> Multiply.A
- [7] Lerp output 0 -> Multiply.B
- [12] Append output 0 -> Lerp.A
- [14] Append output 0 -> Lerp.B
- [2] Input ScalarDisplacement (Vector3) output 0 -> Lerp.Alpha
- [6] Multiply output 0 -> Tangent Space to World Space TransformVector.Input
- [9] Input Min (Scalar) output 0 -> Append.A
- [9] Input Min (Scalar) output 0 -> Append.B
- [11] Append output 0 -> Append.A
- [9] Input Min (Scalar) output 0 -> Append.B
- [10] Input Max (Scalar) output 0 -> Append.A
- [10] Input Max (Scalar) output 0 -> Append.B
- [13] Append output 0 -> Append.A
- [10] Input Max (Scalar) output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [8] Tangent Space to World Space TransformVector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
