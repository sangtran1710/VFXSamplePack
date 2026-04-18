# Prepare3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/Prepare3DDeriv.Prepare3DDeriv`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Uses positions offset in a tetrahedral pattern to compute 3D derivatives. Evaluate the same 3D function at each offset position produced by this function, then feed the resulting values into Compute3DDeriv.

## Inputs

- `Position` [FunctionInput_Vector3] - Position to compute derivative
- `Tetrahedral Offset` [FunctionInput_Scalar] - Small offset distance to apply from original Position. Default offset (0.0001) should be fine unless using on very small or large scale positions. If changed, plug the same value into Prepare3DDeriv and Compute3DDeriv.

## Outputs

- `Offset1` - First offset position for computing 3D derivatives.
- `Offset2` - Second offset position for computing 3D derivatives.
- `Offset3` - Third offset position for computing 3D derivatives.
- `Offset4` - Fourth offset position for computing 3D derivatives.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Offset1 | Offset1 |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Position (Vector3) | Position |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | -1,-1,1 |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionConstant3Vector` | -1,1,-1 |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionConstant3Vector` | 1,-1,-1 |  |
| 8 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Offset2 | Offset2 |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output Offset3 | Offset3 |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Offset4 | Offset4 |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Tetrahedral Offset (Scalar) | Tetrahedral Offset |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Offset1

- `A` <= [9] Add output 0

### [2] Add

- `A` <= [17] Multiply output 0
- `B` <= [1] Input Position (Vector3) output 0

### [4] Add

- `A` <= [16] Multiply output 0
- `B` <= [1] Input Position (Vector3) output 0

### [6] Add

- `A` <= [15] Multiply output 0
- `B` <= [1] Input Position (Vector3) output 0

### [9] Add

- `A` <= [14] Multiply output 0
- `B` <= [1] Input Position (Vector3) output 0

### [10] Output Offset2

- `A` <= [6] Add output 0

### [11] Output Offset3

- `A` <= [4] Add output 0

### [12] Output Offset4

- `A` <= [2] Add output 0

### [14] Multiply

- `A` <= [13] Input Tetrahedral Offset (Scalar) output 0
- `B` <= [8] 1,1,1 output 0

### [15] Multiply

- `A` <= [13] Input Tetrahedral Offset (Scalar) output 0
- `B` <= [7] 1,-1,-1 output 0

### [16] Multiply

- `A` <= [13] Input Tetrahedral Offset (Scalar) output 0
- `B` <= [5] -1,1,-1 output 0

### [17] Multiply

- `A` <= [13] Input Tetrahedral Offset (Scalar) output 0
- `B` <= [3] -1,-1,1 output 0

## Connection List

- [9] Add output 0 -> Output Offset1.A
- [17] Multiply output 0 -> Add.A
- [1] Input Position (Vector3) output 0 -> Add.B
- [16] Multiply output 0 -> Add.A
- [1] Input Position (Vector3) output 0 -> Add.B
- [15] Multiply output 0 -> Add.A
- [1] Input Position (Vector3) output 0 -> Add.B
- [14] Multiply output 0 -> Add.A
- [1] Input Position (Vector3) output 0 -> Add.B
- [6] Add output 0 -> Output Offset2.A
- [4] Add output 0 -> Output Offset3.A
- [2] Add output 0 -> Output Offset4.A
- [13] Input Tetrahedral Offset (Scalar) output 0 -> Multiply.A
- [8] 1,1,1 output 0 -> Multiply.B
- [13] Input Tetrahedral Offset (Scalar) output 0 -> Multiply.A
- [7] 1,-1,-1 output 0 -> Multiply.B
- [13] Input Tetrahedral Offset (Scalar) output 0 -> Multiply.A
- [5] -1,1,-1 output 0 -> Multiply.B
- [13] Input Tetrahedral Offset (Scalar) output 0 -> Multiply.A
- [3] -1,-1,1 output 0 -> Multiply.B

## Output Trace Roots

- `Offset1` is driven by [9] Add
- `Offset2` is driven by [6] Add
- `Offset3` is driven by [4] Add
- `Offset4` is driven by [2] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


