# DistanceField_Sphere

- Path: `/Engine/Functions/DistanceFields/Shapes/DistanceField_Sphere.DistanceField_Sphere`
- Category: DistanceFields/Shapes
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Draws a line between 2 points using supplied 3D coordinates. 

## Inputs

- `Coordinates` [FunctionInput_Vector3]
- `Radius` [FunctionInput_Scalar]
- `Center` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector3) | Coordinates |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | expression | `MaterialExpressionConstant` | 0 |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Center (Vector3) | Center |
| 9 | expression | `MaterialExpressionDistance` | Distance |  |
| 10 | expression | `MaterialExpressionConstant` | 0 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Subtract output 0

### [2] Input Coordinates (Vector3)

- `Preview` <= [5] Append output 0

### [4] Subtract

- `A` <= [9] Distance output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [5] Append

- `A` <= [1] TexCoord[0] output 0
- `B` <= [6] 0 output 0

### [7] Subtract

- `A` <= [2] Input Coordinates (Vector3) output 0
- `B` <= [8] Input Center (Vector3) output 0

### [9] Distance

- `A` <= [7] Subtract output 0
- `B` <= [10] 0 output 0

## Connection List

- [4] Subtract output 0 -> Output Result.A
- [5] Append output 0 -> Input Coordinates (Vector3).Preview
- [9] Distance output 0 -> Subtract.A
- [3] Input Radius (Scalar) output 0 -> Subtract.B
- [1] TexCoord[0] output 0 -> Append.A
- [6] 0 output 0 -> Append.B
- [2] Input Coordinates (Vector3) output 0 -> Subtract.A
- [8] Input Center (Vector3) output 0 -> Subtract.B
- [7] Subtract output 0 -> Distance.A
- [10] 0 output 0 -> Distance.B

## Output Trace Roots

- `Result` is driven by [4] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


