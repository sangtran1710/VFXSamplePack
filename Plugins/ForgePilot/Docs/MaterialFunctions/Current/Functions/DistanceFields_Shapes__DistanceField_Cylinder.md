# DistanceField_Cylinder

- Path: `/Engine/Functions/DistanceFields/Shapes/DistanceField_Cylinder.DistanceField_Cylinder`
- Category: DistanceFields/Shapes
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Generates a distance field shape for a capped cylinder.

## Inputs

- `Coordinates` [FunctionInput_Vector3]
- `h` [FunctionInput_Scalar]
- `Center` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector3) | Coordinates |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input h (Scalar) | h |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | expression | `MaterialExpressionConstant` | 0 |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Center (Vector3) | Center |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 11 | expression | `MaterialExpressionDistance` | Distance |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | expression | `MaterialExpressionConstant` | 0 |  |
| 14 | expression | `MaterialExpressionAbs` | Abs |  |
| 15 | expression | `MaterialExpressionMax` | Max |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 18 | expression | `MaterialExpressionMin` | Min(,0) |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionDistance` | Distance |  |
| 21 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 22 | expression | `MaterialExpressionConstant` | 0 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [19] Add output 0

### [2] Input Coordinates (Vector3)

- `Preview` <= [5] Append output 0

### [4] Subtract

- `A` <= [14] Abs output 0
- `B` <= [3] Input h (Scalar) output 0

### [5] Append

- `A` <= [1] TexCoord[0] output 0
- `B` <= [6] 0 output 0

### [7] Subtract

- `A` <= [2] Input Coordinates (Vector3) output 0
- `B` <= [8] Input Center (Vector3) output 0

### [9] Mask ( R B )

- `Input` <= [7] Subtract output 0

### [10] Mask ( G )

- `Input` <= [7] Subtract output 0

### [11] Distance

- `A` <= [9] Mask ( R B ) output 0
- `B` <= [13] 0 output 0

### [12] Append

- `A` <= [11] Distance output 0
- `B` <= [10] Mask ( G ) output 0

### [14] Abs

- `Input` <= [12] Append output 0

### [15] Max

- `A` <= [16] Mask ( R ) output 0
- `B` <= [17] Mask ( G ) output 0

### [16] Mask ( R )

- `Input` <= [4] Subtract output 0

### [17] Mask ( G )

- `Input` <= [4] Subtract output 0

### [18] Min(,0)

- `A` <= [15] Max output 0

### [19] Add

- `A` <= [18] Min(,0) output 0
- `B` <= [20] Distance output 0

### [20] Distance

- `A` <= [21] Max(,0) output 0
- `B` <= [22] 0 output 0

### [21] Max(,0)

- `A` <= [4] Subtract output 0

## Connection List

- [19] Add output 0 -> Output Result.A
- [5] Append output 0 -> Input Coordinates (Vector3).Preview
- [14] Abs output 0 -> Subtract.A
- [3] Input h (Scalar) output 0 -> Subtract.B
- [1] TexCoord[0] output 0 -> Append.A
- [6] 0 output 0 -> Append.B
- [2] Input Coordinates (Vector3) output 0 -> Subtract.A
- [8] Input Center (Vector3) output 0 -> Subtract.B
- [7] Subtract output 0 -> Mask ( R B ).Input
- [7] Subtract output 0 -> Mask ( G ).Input
- [9] Mask ( R B ) output 0 -> Distance.A
- [13] 0 output 0 -> Distance.B
- [11] Distance output 0 -> Append.A
- [10] Mask ( G ) output 0 -> Append.B
- [12] Append output 0 -> Abs.Input
- [16] Mask ( R ) output 0 -> Max.A
- [17] Mask ( G ) output 0 -> Max.B
- [4] Subtract output 0 -> Mask ( R ).Input
- [4] Subtract output 0 -> Mask ( G ).Input
- [15] Max output 0 -> Min(,0).A
- [18] Min(,0) output 0 -> Add.A
- [20] Distance output 0 -> Add.B
- [21] Max(,0) output 0 -> Distance.A
- [22] 0 output 0 -> Distance.B
- [4] Subtract output 0 -> Max(,0).A

## Output Trace Roots

- `Result` is driven by [19] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


