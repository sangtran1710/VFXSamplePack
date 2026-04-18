# HueShift

- Path: `/Engine/Functions/Engine_MaterialFunctions02/HueShift.HueShift`
- Category: Engine_MaterialFunctions02
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Adjusts the hue of an image.

## Inputs

- `Texture` [FunctionInput_Vector3]
- `Hue Shift Percentage` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 3 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Vector3) | Texture |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Hue Shift Percentage (Scalar) | Hue Shift Percentage |
| 8 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [5] Add output 0

### [1] Normalize

- `VectorInput` <= [2] 1,1,1 output 0

### [3] RotateAboutAxis

- `NormalizedRotationAxis` <= [1] Normalize output 0
- `RotationAngle` <= [7] Input Hue Shift Percentage (Scalar) output 0
- `PivotPoint` <= [4] 0 output 0
- `Position` <= [6] Input Texture (Vector3) output 0

### [5] Add

- `A` <= [3] RotateAboutAxis output 0
- `B` <= [6] Input Texture (Vector3) output 0

### [6] Input Texture (Vector3)

- `Preview` <= [8] 0,1,0 output 0

## Connection List

- [5] Add output 0 -> Output Result.A
- [2] 1,1,1 output 0 -> Normalize.VectorInput
- [1] Normalize output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [7] Input Hue Shift Percentage (Scalar) output 0 -> RotateAboutAxis.RotationAngle
- [4] 0 output 0 -> RotateAboutAxis.PivotPoint
- [6] Input Texture (Vector3) output 0 -> RotateAboutAxis.Position
- [3] RotateAboutAxis output 0 -> Add.A
- [6] Input Texture (Vector3) output 0 -> Add.B
- [8] 0,1,0 output 0 -> Input Texture (Vector3).Preview

## Output Trace Roots

- `Result` is driven by [5] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


