# Compute3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/Compute3DDeriv.Compute3DDeriv`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Uses positions offset in a tetrahedral pattern to compute 3D derivatives. Use with Prepare3DDeriv.

## Inputs

- `Value1` [FunctionInput_Vector3] - Value at Offset1 from Prepare3DDeriv
- `Value2` [FunctionInput_Vector3] - Value at Offset2 from Prepare3DDeriv
- `Value3` [FunctionInput_Vector3] - Value at Offset3 from Prepare3DDeriv
- `Value4` [FunctionInput_Vector3] - Value at Offset4 from Prepare3DDeriv
- `Tetrahedral Offset` [FunctionInput_Scalar] - Small offset distance to apply from original Position. Default offset (0.0001) should be fine unless using on very small or large scale positions. If changed, plug the same value into Prepare3DDeriv and Compute3DDeriv.

## Outputs

- `dx` - Derivative in X direction
- `dy` - Derivative in Y direction
- `dz` - Derivative in Z direction
- `Value` - Estimated value at original position without offsets

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output dx | dx |
| 1 | expression | `MaterialExpressionMultiply` | Multiply(,0.25) |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Value1 (Vector3) | Value1 |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Value2 (Vector3) | Value2 |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Value3 (Vector3) | Value3 |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Value4 (Vector3) | Value4 |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output dy | dy |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output dz | dz |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output Value | Value |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Tetrahedral Offset (Scalar) | Tetrahedral Offset |
| 21 | expression | `MaterialExpressionDivide` | Divide(0.25,) |  |

## Exact Input Wiring

### [0] Output dx

- `A` <= [4] Multiply output 0

### [1] Multiply(,0.25)

- `A` <= [5] Add output 0

### [2] Multiply

- `A` <= [6] Subtract output 0
- `B` <= [21] Divide(0.25,) output 0

### [3] Multiply

- `A` <= [7] Add output 0
- `B` <= [21] Divide(0.25,) output 0

### [4] Multiply

- `A` <= [8] Subtract output 0
- `B` <= [21] Divide(0.25,) output 0

### [5] Add

- `A` <= [12] Add output 0
- `B` <= [10] Add output 0

### [6] Subtract

- `A` <= [11] Subtract output 0
- `B` <= [9] Subtract output 0

### [7] Add

- `A` <= [11] Subtract output 0
- `B` <= [9] Subtract output 0

### [8] Subtract

- `A` <= [12] Add output 0
- `B` <= [10] Add output 0

### [9] Subtract

- `A` <= [15] Input Value3 (Vector3) output 0
- `B` <= [16] Input Value4 (Vector3) output 0

### [10] Add

- `A` <= [15] Input Value3 (Vector3) output 0
- `B` <= [16] Input Value4 (Vector3) output 0

### [11] Subtract

- `A` <= [13] Input Value1 (Vector3) output 0
- `B` <= [14] Input Value2 (Vector3) output 0

### [12] Add

- `A` <= [13] Input Value1 (Vector3) output 0
- `B` <= [14] Input Value2 (Vector3) output 0

### [17] Output dy

- `A` <= [3] Multiply output 0

### [18] Output dz

- `A` <= [2] Multiply output 0

### [19] Output Value

- `A` <= [1] Multiply(,0.25) output 0

### [21] Divide(0.25,)

- `B` <= [20] Input Tetrahedral Offset (Scalar) output 0

## Connection List

- [4] Multiply output 0 -> Output dx.A
- [5] Add output 0 -> Multiply(,0.25).A
- [6] Subtract output 0 -> Multiply.A
- [21] Divide(0.25,) output 0 -> Multiply.B
- [7] Add output 0 -> Multiply.A
- [21] Divide(0.25,) output 0 -> Multiply.B
- [8] Subtract output 0 -> Multiply.A
- [21] Divide(0.25,) output 0 -> Multiply.B
- [12] Add output 0 -> Add.A
- [10] Add output 0 -> Add.B
- [11] Subtract output 0 -> Subtract.A
- [9] Subtract output 0 -> Subtract.B
- [11] Subtract output 0 -> Add.A
- [9] Subtract output 0 -> Add.B
- [12] Add output 0 -> Subtract.A
- [10] Add output 0 -> Subtract.B
- [15] Input Value3 (Vector3) output 0 -> Subtract.A
- [16] Input Value4 (Vector3) output 0 -> Subtract.B
- [15] Input Value3 (Vector3) output 0 -> Add.A
- [16] Input Value4 (Vector3) output 0 -> Add.B
- [13] Input Value1 (Vector3) output 0 -> Subtract.A
- [14] Input Value2 (Vector3) output 0 -> Subtract.B
- [13] Input Value1 (Vector3) output 0 -> Add.A
- [14] Input Value2 (Vector3) output 0 -> Add.B
- [3] Multiply output 0 -> Output dy.A
- [2] Multiply output 0 -> Output dz.A
- [1] Multiply(,0.25) output 0 -> Output Value.A
- [20] Input Tetrahedral Offset (Scalar) output 0 -> Divide(0.25,).B

## Output Trace Roots

- `dx` is driven by [4] Multiply
- `dy` is driven by [3] Multiply
- `dz` is driven by [2] Multiply
- `Value` is driven by [1] Multiply(,0.25)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


