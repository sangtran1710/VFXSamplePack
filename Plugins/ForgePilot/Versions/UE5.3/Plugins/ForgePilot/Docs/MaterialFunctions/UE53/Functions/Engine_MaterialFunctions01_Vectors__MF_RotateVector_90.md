# MF_RotateVector_90

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/MF_RotateVector_90.MF_RotateVector_90`
- Category: Engine_MaterialFunctions01/Vectors
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Rotates vectors along any cardinal axis (Right hand coordinates)

## Inputs

- `Vector` [FunctionInput_Vector3]

## Outputs

- `CW X`
- `CCW X`
- `CW Y`
- `CCW Y`
- `CW Z`
- `CCW Z`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output CW X | CW X |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output CCW X | CCW X |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output CW Y | CW Y |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |
| 15 | expression | `MaterialExpressionAppendVector` | Append |  |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output CCW Y | CCW Y |
| 17 | expression | `MaterialExpressionAppendVector` | Append |  |
| 18 | expression | `MaterialExpressionAppendVector` | Append |  |
| 19 | expression | `MaterialExpressionAppendVector` | Append |  |
| 20 | expression | `MaterialExpressionAppendVector` | Append |  |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output CW Z | CW Z |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output CCW Z | CCW Z |
| 23 | expression | `MaterialExpressionAppendVector` | Append |  |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output CW X

- `A` <= [10] Append output 0

### [2] Mask ( R )

- `Input` <= [1] Input Vector (Vector3) output 0

### [3] Mask ( G )

- `Input` <= [1] Input Vector (Vector3) output 0

### [4] Mask ( B )

- `Input` <= [1] Input Vector (Vector3) output 0

### [5] Multiply(,-1)

- `A` <= [2] Mask ( R ) output 0

### [6] Multiply(,-1)

- `A` <= [3] Mask ( G ) output 0

### [7] Multiply(,-1)

- `A` <= [4] Mask ( B ) output 0

### [8] Append

- `A` <= [2] Mask ( R ) output 0
- `B` <= [7] Multiply(,-1) output 0

### [9] Output CCW X

- `A` <= [12] Append output 0

### [10] Append

- `A` <= [8] Append output 0
- `B` <= [3] Mask ( G ) output 0

### [11] Append

- `A` <= [2] Mask ( R ) output 0
- `B` <= [4] Mask ( B ) output 0

### [12] Append

- `A` <= [11] Append output 0
- `B` <= [6] Multiply(,-1) output 0

### [13] Output CW Y

- `A` <= [15] Append output 0

### [14] Append

- `A` <= [7] Multiply(,-1) output 0
- `B` <= [3] Mask ( G ) output 0

### [15] Append

- `A` <= [14] Append output 0
- `B` <= [2] Mask ( R ) output 0

### [16] Output CCW Y

- `A` <= [17] Append output 0

### [17] Append

- `A` <= [18] Append output 0
- `B` <= [5] Multiply(,-1) output 0

### [18] Append

- `A` <= [4] Mask ( B ) output 0
- `B` <= [3] Mask ( G ) output 0

### [19] Append

- `A` <= [3] Mask ( G ) output 0
- `B` <= [5] Multiply(,-1) output 0

### [20] Append

- `A` <= [19] Append output 0
- `B` <= [4] Mask ( B ) output 0

### [21] Output CW Z

- `A` <= [20] Append output 0

### [22] Output CCW Z

- `A` <= [23] Append output 0

### [23] Append

- `A` <= [24] Append output 0
- `B` <= [4] Mask ( B ) output 0

### [24] Append

- `A` <= [6] Multiply(,-1) output 0
- `B` <= [2] Mask ( R ) output 0

## Connection List

- [10] Append output 0 -> Output CW X.A
- [1] Input Vector (Vector3) output 0 -> Mask ( R ).Input
- [1] Input Vector (Vector3) output 0 -> Mask ( G ).Input
- [1] Input Vector (Vector3) output 0 -> Mask ( B ).Input
- [2] Mask ( R ) output 0 -> Multiply(,-1).A
- [3] Mask ( G ) output 0 -> Multiply(,-1).A
- [4] Mask ( B ) output 0 -> Multiply(,-1).A
- [2] Mask ( R ) output 0 -> Append.A
- [7] Multiply(,-1) output 0 -> Append.B
- [12] Append output 0 -> Output CCW X.A
- [8] Append output 0 -> Append.A
- [3] Mask ( G ) output 0 -> Append.B
- [2] Mask ( R ) output 0 -> Append.A
- [4] Mask ( B ) output 0 -> Append.B
- [11] Append output 0 -> Append.A
- [6] Multiply(,-1) output 0 -> Append.B
- [15] Append output 0 -> Output CW Y.A
- [7] Multiply(,-1) output 0 -> Append.A
- [3] Mask ( G ) output 0 -> Append.B
- [14] Append output 0 -> Append.A
- [2] Mask ( R ) output 0 -> Append.B
- [17] Append output 0 -> Output CCW Y.A
- [18] Append output 0 -> Append.A
- [5] Multiply(,-1) output 0 -> Append.B
- [4] Mask ( B ) output 0 -> Append.A
- [3] Mask ( G ) output 0 -> Append.B
- [3] Mask ( G ) output 0 -> Append.A
- [5] Multiply(,-1) output 0 -> Append.B
- [19] Append output 0 -> Append.A
- [4] Mask ( B ) output 0 -> Append.B
- [20] Append output 0 -> Output CW Z.A
- [23] Append output 0 -> Output CCW Z.A
- [24] Append output 0 -> Append.A
- [4] Mask ( B ) output 0 -> Append.B
- [6] Multiply(,-1) output 0 -> Append.A
- [2] Mask ( R ) output 0 -> Append.B

## Output Trace Roots

- `CW X` is driven by [10] Append
- `CCW X` is driven by [12] Append
- `CW Y` is driven by [15] Append
- `CCW Y` is driven by [17] Append
- `CW Z` is driven by [20] Append
- `CCW Z` is driven by [23] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
