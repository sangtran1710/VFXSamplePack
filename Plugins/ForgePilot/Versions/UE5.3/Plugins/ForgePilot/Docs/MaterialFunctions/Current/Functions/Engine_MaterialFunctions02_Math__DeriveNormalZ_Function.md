# DeriveNormalZ_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/DeriveNormalZ_Function.DeriveNormalZ_Function`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

DeriveNormalZ expanded into a material function, including helpful features like safe normalization and a z sign input.

## Inputs

- `Z sign` [FunctionInput_Scalar] - Defaults to 1.0. Alternatively enter -1.0.
- `XY vector` [FunctionInput_Vector2]
- `Default Normalization vector` [FunctionInput_Vector3] - If the function fails this value will be used instead. It could fail if the vectors length is 0.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Z sign (Scalar) | Z sign |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input XY vector (Vector2) | XY vector |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Default Normalization vector (Vector3) | Default Normalization vector |
| 7 | expression | `MaterialExpressionConstant3Vector` | 0.604,0.565,0.563 |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] SafeNormalize output 0

### [2] Input XY vector (Vector2)

- `Preview` <= [8] Mask ( R G ) output 0

### [3] Multiply

- `A` <= [11] Mask ( B ) output 0
- `B` <= [1] Input Z sign (Scalar) output 0

### [5] Append

- `A` <= [2] Input XY vector (Vector2) output 0
- `B` <= [3] Multiply output 0

### [8] Mask ( R G )

- `Input` <= [9] Normalize output 0

### [9] Normalize

- `VectorInput` <= [7] 0.604,0.565,0.563 output 0

### [10] DeriveNormalZ

- `InXY` <= [2] Input XY vector (Vector2) output 0

### [11] Mask ( B )

- `Input` <= [10] DeriveNormalZ output 0

## Connection List

- [4] SafeNormalize output 0 -> Output Result.A
- [8] Mask ( R G ) output 0 -> Input XY vector (Vector2).Preview
- [11] Mask ( B ) output 0 -> Multiply.A
- [1] Input Z sign (Scalar) output 0 -> Multiply.B
- [2] Input XY vector (Vector2) output 0 -> Append.A
- [3] Multiply output 0 -> Append.B
- [9] Normalize output 0 -> Mask ( R G ).Input
- [7] 0.604,0.565,0.563 output 0 -> Normalize.VectorInput
- [2] Input XY vector (Vector2) output 0 -> DeriveNormalZ.InXY
- [10] DeriveNormalZ output 0 -> Mask ( B ).Input

## Output Trace Roots

- `Result` is driven by [4] SafeNormalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


