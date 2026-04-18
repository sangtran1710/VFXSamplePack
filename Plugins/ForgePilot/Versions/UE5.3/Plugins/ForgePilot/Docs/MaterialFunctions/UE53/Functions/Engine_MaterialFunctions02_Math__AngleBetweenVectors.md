# AngleBetweenVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/AngleBetweenVectors.AngleBetweenVectors`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Find the angle between two normalized vectors in degrees.

## Inputs

- `Normalized V1` [FunctionInput_Vector3]
- `Normalized V2` [FunctionInput_Vector3]

## Outputs

- `Angle In Degrees`
- `0-1`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Angle In Degrees | Angle In Degrees |
| 1 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 2 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionConstant` | 0 |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply(,0.3183) |  |
| 11 | expression | `MaterialExpressionArccosineFast` | ArccosineFast |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output 0-1 | 0-1 |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Normalized V1 (Vector3) | Normalized V1 |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Normalized V2 (Vector3) | Normalized V2 |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,0.005556) |  |
| 16 | expression | `MaterialExpressionIf` | If |  |
| 17 | expression | `MaterialExpressionConstant` | 90 |  |
| 18 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 19 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 20 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |

## Exact Input Wiring

### [0] Output Angle In Degrees

- `A` <= [9] Multiply(,57.3) output 0

### [2] Dot

- `A` <= [13] Input Normalized V1 (Vector3) output 0
- `B` <= [14] Input Normalized V2 (Vector3) output 0

### [5] Subtract(,0.5)

- `A` <= [4] TexCoord[0] output 0

### [7] Append

- `A` <= [5] Subtract(,0.5) output 0
- `B` <= [8] 0 output 0

### [9] Multiply(,57.3)

- `A` <= [11] ArccosineFast output 0

### [10] Multiply(,0.3183)

- `A` <= [11] ArccosineFast output 0

### [11] ArccosineFast

- `Input` <= [2] Dot output 0

### [12] Output 0-1

- `A` <= [15] Multiply(,0.005556) output 0

### [13] Input Normalized V1 (Vector3)

- `Preview` <= [6] SafeNormalize output 0

### [14] Input Normalized V2 (Vector3)

- `Preview` <= [3] SafeNormalize output 0

### [15] Multiply(,0.005556)

- `A` <= [9] Multiply(,57.3) output 0

### [16] If

- `A` <= [9] Multiply(,57.3) output 0
- `B` <= [17] 90 output 0
- `AGreaterThanB` <= [18] 1,0,0 output 0
- `AEqualsB` <= [19] 0,1,0 output 0
- `ALessThanB` <= [20] 0,0,1 output 0

## Connection List

- [9] Multiply(,57.3) output 0 -> Output Angle In Degrees.A
- [13] Input Normalized V1 (Vector3) output 0 -> Dot.A
- [14] Input Normalized V2 (Vector3) output 0 -> Dot.B
- [4] TexCoord[0] output 0 -> Subtract(,0.5).A
- [5] Subtract(,0.5) output 0 -> Append.A
- [8] 0 output 0 -> Append.B
- [11] ArccosineFast output 0 -> Multiply(,57.3).A
- [11] ArccosineFast output 0 -> Multiply(,0.3183).A
- [2] Dot output 0 -> ArccosineFast.Input
- [15] Multiply(,0.005556) output 0 -> Output 0-1.A
- [6] SafeNormalize output 0 -> Input Normalized V1 (Vector3).Preview
- [3] SafeNormalize output 0 -> Input Normalized V2 (Vector3).Preview
- [9] Multiply(,57.3) output 0 -> Multiply(,0.005556).A
- [9] Multiply(,57.3) output 0 -> If.A
- [17] 90 output 0 -> If.B
- [18] 1,0,0 output 0 -> If.AGreaterThanB
- [19] 0,1,0 output 0 -> If.AEqualsB
- [20] 0,0,1 output 0 -> If.ALessThanB

## Output Trace Roots

- `Angle In Degrees` is driven by [9] Multiply(,57.3)
- `0-1` is driven by [15] Multiply(,0.005556)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
