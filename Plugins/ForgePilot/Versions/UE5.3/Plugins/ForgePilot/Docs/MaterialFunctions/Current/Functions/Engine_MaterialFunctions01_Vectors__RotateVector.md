# RotateVector

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/RotateVector.RotateVector`
- Category: Engine_MaterialFunctions01/Vectors
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Rotates an input vector to point toward a given direction.

## Inputs

- `LookAtVector` [FunctionInput_Vector3] - the direction to point to
- `VectorToRotate` [FunctionInput_Vector3]
- `RestingVector` [FunctionInput_Vector3] - the starting direction

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input LookAtVector (Vector3) | LookAtVector |
| 2 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 3 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 4 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 5 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 8 | custom | `MaterialExpressionCustom` | Custom |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply(,57.29) |  |
| 11 | expression | `MaterialExpressionDivide` | Divide(,360) |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input VectorToRotate (Vector3) | VectorToRotate |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input RestingVector (Vector3) | RestingVector |

## Exact Input Wiring

### [0] Output Result

- `A` <= [6] Add output 0

### [2] Normalize

- `VectorInput` <= [1] Input LookAtVector (Vector3) output 0

### [3] RotateAboutAxis

- `NormalizedRotationAxis` <= [7] Cross output 0
- `RotationAngle` <= [11] Divide(,360) output 0
- `PivotPoint` <= [5] 0,0,0 output 0
- `Position` <= [12] Input VectorToRotate (Vector3) output 0

### [6] Add

- `A` <= [3] RotateAboutAxis output 0
- `B` <= [12] Input VectorToRotate (Vector3) output 0

### [7] Cross

- `A` <= [13] Input RestingVector (Vector3) output 0
- `B` <= [2] Normalize output 0

### [9] Dot

- `A` <= [13] Input RestingVector (Vector3) output 0
- `B` <= [2] Normalize output 0

### [10] Multiply(,57.29)

- `A` <= [8] Custom output 0

### [11] Divide(,360)

- `A` <= [10] Multiply(,57.29) output 0

## Connection List

- [6] Add output 0 -> Output Result.A
- [1] Input LookAtVector (Vector3) output 0 -> Normalize.VectorInput
- [7] Cross output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [11] Divide(,360) output 0 -> RotateAboutAxis.RotationAngle
- [5] 0,0,0 output 0 -> RotateAboutAxis.PivotPoint
- [12] Input VectorToRotate (Vector3) output 0 -> RotateAboutAxis.Position
- [3] RotateAboutAxis output 0 -> Add.A
- [12] Input VectorToRotate (Vector3) output 0 -> Add.B
- [13] Input RestingVector (Vector3) output 0 -> Cross.A
- [2] Normalize output 0 -> Cross.B
- [13] Input RestingVector (Vector3) output 0 -> Dot.A
- [2] Normalize output 0 -> Dot.B
- [8] Custom output 0 -> Multiply(,57.29).A
- [10] Multiply(,57.29) output 0 -> Divide(,360).A

## Output Trace Roots

- `Result` is driven by [6] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


