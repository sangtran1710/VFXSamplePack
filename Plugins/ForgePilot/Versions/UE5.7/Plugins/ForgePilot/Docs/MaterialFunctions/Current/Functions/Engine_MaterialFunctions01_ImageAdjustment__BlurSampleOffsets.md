# BlurSampleOffsets

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/BlurSampleOffsets.BlurSampleOffsets`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Multiplies an input offset by a number of 2d vectors.

## Inputs

- `OffsetAmount` [FunctionInput_Vector2]

## Outputs

- `1,0`
- `-1,0`
- `0,1`
- `0,-1`
- `.5,.5`
- `-.5,-.5`
- `-.5,.5`
- `.5,-.5`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 1,0 | 1,0 |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionConstant2Vector` | 1,0 |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionConstant2Vector` | -1,0 |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 7 | expression | `MaterialExpressionConstant2Vector` | 0,-1 |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionConstant2Vector` | 0.5,0.5 |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant2Vector` | -0.5,-0.5 |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionConstant2Vector` | -0.5,0.5 |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionConstant2Vector` | 0.5,-0.5 |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input OffsetAmount (Vector2) | OffsetAmount |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output -1,0 | -1,0 |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output 0,1 | 0,1 |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output 0,-1 | 0,-1 |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output .5,.5 | .5,.5 |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output -.5,-.5 | -.5,-.5 |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output -.5,.5 | -.5,.5 |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output .5,-.5 | .5,-.5 |

## Exact Input Wiring

### [0] Output 1,0

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [2] 1,0 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [3] Multiply

- `A` <= [4] -1,0 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [5] Multiply

- `A` <= [6] 0,1 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [8] Multiply

- `A` <= [7] 0,-1 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [9] Multiply

- `A` <= [10] 0.5,0.5 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [11] Multiply

- `A` <= [12] -0.5,-0.5 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [13] Multiply

- `A` <= [14] -0.5,0.5 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [15] Multiply

- `A` <= [16] 0.5,-0.5 output 0
- `B` <= [17] Input OffsetAmount (Vector2) output 0

### [18] Output -1,0

- `A` <= [3] Multiply output 0

### [19] Output 0,1

- `A` <= [5] Multiply output 0

### [20] Output 0,-1

- `A` <= [8] Multiply output 0

### [21] Output .5,.5

- `A` <= [9] Multiply output 0

### [22] Output -.5,-.5

- `A` <= [11] Multiply output 0

### [23] Output -.5,.5

- `A` <= [13] Multiply output 0

### [24] Output .5,-.5

- `A` <= [15] Multiply output 0

## Connection List

- [1] Multiply output 0 -> Output 1,0.A
- [2] 1,0 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [4] -1,0 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [6] 0,1 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [7] 0,-1 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [10] 0.5,0.5 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [12] -0.5,-0.5 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [14] -0.5,0.5 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [16] 0.5,-0.5 output 0 -> Multiply.A
- [17] Input OffsetAmount (Vector2) output 0 -> Multiply.B
- [3] Multiply output 0 -> Output -1,0.A
- [5] Multiply output 0 -> Output 0,1.A
- [8] Multiply output 0 -> Output 0,-1.A
- [9] Multiply output 0 -> Output .5,.5.A
- [11] Multiply output 0 -> Output -.5,-.5.A
- [13] Multiply output 0 -> Output -.5,.5.A
- [15] Multiply output 0 -> Output .5,-.5.A

## Output Trace Roots

- `1,0` is driven by [1] Multiply
- `-1,0` is driven by [3] Multiply
- `0,1` is driven by [5] Multiply
- `0,-1` is driven by [8] Multiply
- `.5,.5` is driven by [9] Multiply
- `-.5,-.5` is driven by [11] Multiply
- `-.5,.5` is driven by [13] Multiply
- `.5,-.5` is driven by [15] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


