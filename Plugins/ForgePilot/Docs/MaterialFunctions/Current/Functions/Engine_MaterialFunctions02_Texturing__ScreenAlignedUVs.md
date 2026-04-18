# ScreenAlignedUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/ScreenAlignedUVs.ScreenAlignedUVs`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Maps the 0-1 uv range to the screen. Don't use as UV for SceneTexture material expression

## Inputs

- `Texture Dimensions` [FunctionInput_Vector2] - Enter the resolution of the texture that you will be sampling if you're sampling uvs via the scaled outputs.

## Outputs

- `X 100%, Y 100%` - X and Y are scaled to fit the screen
- `X Scale to Ratio, Y 100%` - This function output outputs uvs that range from 0-1 on the Y axis and are center scaled on X to maintain the images aspect ratio.
- `X 100%, Y Scale to Ratio` - This function output outputs uvs that range from 0-1 on the X axis and the Y uvs are center scaled to maintain the images resolution.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output X 100%, Y 100% | X 100%, Y 100% |
| 1 | expression | `MaterialExpressionScreenPosition` | ScreenPosition |  |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output X Scale to Ratio, Y 100% | X Scale to Ratio, Y 100% |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | ScaleUVsByCenter |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | ScreenResolution |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 12 | expression | `MaterialExpressionConstant` | 1 |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | ScaleUVsByCenter |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | ScreenResolution |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 19 | expression | `MaterialExpressionDivide` | Divide |  |
| 20 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 21 | expression | `MaterialExpressionAppendVector` | Append |  |
| 22 | expression | `MaterialExpressionConstant` | 1 |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output X 100%, Y Scale to Ratio | X 100%, Y Scale to Ratio |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Texture Dimensions (Vector2) | Texture Dimensions |

## Exact Input Wiring

### [0] Output X 100%, Y 100%

- `A` <= [1] ScreenPosition output 0

### [2] Output X Scale to Ratio, Y 100%

- `A` <= [3] ScaleUVsByCenter output 0

### [5] Divide

- `A` <= [4] BreakOutFloat2Components output 1
- `B` <= [4] BreakOutFloat2Components output 0

### [8] Divide

- `A` <= [7] BreakOutFloat2Components output 0
- `B` <= [7] BreakOutFloat2Components output 1

### [9] Divide(1,)

- `B` <= [13] Multiply output 0

### [10] Append

- `A` <= [11] BreakOutFloat2Components output 0
- `B` <= [12] 1 output 0

### [13] Multiply

- `A` <= [8] Divide output 0
- `B` <= [5] Divide output 0

### [16] Divide

- `A` <= [15] BreakOutFloat2Components output 0
- `B` <= [15] BreakOutFloat2Components output 1

### [19] Divide

- `A` <= [18] BreakOutFloat2Components output 1
- `B` <= [18] BreakOutFloat2Components output 0

### [20] Divide(1,)

- `B` <= [23] Multiply output 0

### [21] Append

- `A` <= [22] 1 output 0
- `B` <= [20] Divide(1,) output 0

### [23] Multiply

- `A` <= [19] Divide output 0
- `B` <= [16] Divide output 0

### [24] Output X 100%, Y Scale to Ratio

- `A` <= [14] ScaleUVsByCenter output 0

## Connection List

- [1] ScreenPosition output 0 -> Output X 100%, Y 100%.A
- [3] ScaleUVsByCenter output 0 -> Output X Scale to Ratio, Y 100%.A
- [4] BreakOutFloat2Components output 1 -> Divide.A
- [4] BreakOutFloat2Components output 0 -> Divide.B
- [7] BreakOutFloat2Components output 0 -> Divide.A
- [7] BreakOutFloat2Components output 1 -> Divide.B
- [13] Multiply output 0 -> Divide(1,).B
- [11] BreakOutFloat2Components output 0 -> Append.A
- [12] 1 output 0 -> Append.B
- [8] Divide output 0 -> Multiply.A
- [5] Divide output 0 -> Multiply.B
- [15] BreakOutFloat2Components output 0 -> Divide.A
- [15] BreakOutFloat2Components output 1 -> Divide.B
- [18] BreakOutFloat2Components output 1 -> Divide.A
- [18] BreakOutFloat2Components output 0 -> Divide.B
- [23] Multiply output 0 -> Divide(1,).B
- [22] 1 output 0 -> Append.A
- [20] Divide(1,) output 0 -> Append.B
- [19] Divide output 0 -> Multiply.A
- [16] Divide output 0 -> Multiply.B
- [14] ScaleUVsByCenter output 0 -> Output X 100%, Y Scale to Ratio.A

## Output Trace Roots

- `X 100%, Y 100%` is driven by [1] ScreenPosition
- `X Scale to Ratio, Y 100%` is driven by [3] ScaleUVsByCenter
- `X 100%, Y Scale to Ratio` is driven by [14] ScaleUVsByCenter

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


