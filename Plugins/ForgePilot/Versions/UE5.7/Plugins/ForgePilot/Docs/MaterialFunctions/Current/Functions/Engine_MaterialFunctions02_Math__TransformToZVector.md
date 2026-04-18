# TransformToZVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TransformToZVector.TransformToZVector`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This transforms a vector based on a Z vector and generated orthogonal X and Y vectors. Useful for transforming worldposition based on Hit normals etc.

## Inputs

- `Vector to Transform` [FunctionInput_Vector3]
- `Z Vector` [FunctionInput_Vector3]
- `Center Location` [FunctionInput_Vector3] - Center Location will be subtracted from the Transformed Vector which is useful for centering the transformed vectors around something like ObjectPosition for example. Defaults to 0.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector to Transform (Vector3) | Vector to Transform |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Z Vector (Vector3) | Z Vector |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | InverseTransformMatrix |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 5 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 6 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Center Location (Vector3) | Center Location |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] InverseTransformMatrix output 0

### [1] Input Vector to Transform (Vector3)

- `Preview` <= [9] Absolute World Position output 0

### [6] Normalize

- `VectorInput` <= [2] Input Z Vector (Vector3) output 0

### [8] Subtract

- `A` <= [1] Input Vector to Transform (Vector3) output 0
- `B` <= [7] Input Center Location (Vector3) output 0

## Connection List

- [3] InverseTransformMatrix output 0 -> Output Result.A
- [9] Absolute World Position output 0 -> Input Vector to Transform (Vector3).Preview
- [2] Input Z Vector (Vector3) output 0 -> Normalize.VectorInput
- [1] Input Vector to Transform (Vector3) output 0 -> Subtract.A
- [7] Input Center Location (Vector3) output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [3] InverseTransformMatrix

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


