# DistanceField_Capsule

- Path: `/Engine/Functions/DistanceFields/Shapes/DistanceField_Capsule.DistanceField_Capsule`
- Category: DistanceFields/Shapes
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Draws a line between 2 points using supplied 3D coordinates. 

## Inputs

- `Coordinates` [FunctionInput_Vector3]
- `P0` [FunctionInput_Vector3]
- `P1` [FunctionInput_Vector3]
- `Width` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector3) | Coordinates |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input P0 (Vector3) | P0 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input P1 (Vector3) | P1 |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Width (Scalar) | Width |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 15 | expression | `MaterialExpressionAppendVector` | Append |  |
| 16 | expression | `MaterialExpressionConstant` | 0 |  |
| 17 | expression | `MaterialExpressionDistance` | Distance |  |
| 18 | expression | `MaterialExpressionConstant` | 0 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [14] Subtract output 0

### [2] Input Coordinates (Vector3)

- `Preview` <= [15] Append output 0

### [6] Subtract

- `A` <= [2] Input Coordinates (Vector3) output 0
- `B` <= [3] Input P0 (Vector3) output 0

### [7] Subtract

- `A` <= [4] Input P1 (Vector3) output 0
- `B` <= [3] Input P0 (Vector3) output 0

### [8] Dot

- `A` <= [6] Subtract output 0
- `B` <= [7] Subtract output 0

### [9] Dot

- `A` <= [7] Subtract output 0
- `B` <= [7] Subtract output 0

### [10] Divide

- `A` <= [8] Dot output 0
- `B` <= [9] Dot output 0

### [11] Clamp

- `Input` <= [10] Divide output 0

### [12] Subtract

- `A` <= [6] Subtract output 0
- `B` <= [13] Multiply output 0

### [13] Multiply

- `A` <= [11] Clamp output 0
- `B` <= [7] Subtract output 0

### [14] Subtract

- `A` <= [17] Distance output 0
- `B` <= [5] Input Width (Scalar) output 0

### [15] Append

- `A` <= [1] TexCoord[0] output 0
- `B` <= [16] 0 output 0

### [17] Distance

- `A` <= [12] Subtract output 0
- `B` <= [18] 0 output 0

## Connection List

- [14] Subtract output 0 -> Output Result.A
- [15] Append output 0 -> Input Coordinates (Vector3).Preview
- [2] Input Coordinates (Vector3) output 0 -> Subtract.A
- [3] Input P0 (Vector3) output 0 -> Subtract.B
- [4] Input P1 (Vector3) output 0 -> Subtract.A
- [3] Input P0 (Vector3) output 0 -> Subtract.B
- [6] Subtract output 0 -> Dot.A
- [7] Subtract output 0 -> Dot.B
- [7] Subtract output 0 -> Dot.A
- [7] Subtract output 0 -> Dot.B
- [8] Dot output 0 -> Divide.A
- [9] Dot output 0 -> Divide.B
- [10] Divide output 0 -> Clamp.Input
- [6] Subtract output 0 -> Subtract.A
- [13] Multiply output 0 -> Subtract.B
- [11] Clamp output 0 -> Multiply.A
- [7] Subtract output 0 -> Multiply.B
- [17] Distance output 0 -> Subtract.A
- [5] Input Width (Scalar) output 0 -> Subtract.B
- [1] TexCoord[0] output 0 -> Append.A
- [16] 0 output 0 -> Append.B
- [12] Subtract output 0 -> Distance.A
- [18] 0 output 0 -> Distance.B

## Output Trace Roots

- `Result` is driven by [14] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


