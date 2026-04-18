# PanTextureCoordinateFrom-1toN+1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/PanTextureCoordinateFrom-1toN+1.PanTextureCoordinateFrom-1toN+1`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function pans 0-1 uv coordinates from -1 to the number of tiled units +1. This is useful when used in conjunction with textures with clamped coordinates.

## Inputs

- `UV Tiling` [FunctionInput_Vector2]
- `UVs` [FunctionInput_Vector2]
- `Time (0-1)` [FunctionInput_Vector2]

## Outputs

- `UVs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_call | `MaterialExpressionMaterialFunctionCall` | PanTextureCoordinateChannelfrom-1ton+1 |  |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | PanTextureCoordinateChannelfrom-1ton+1 |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input UV Tiling (Vector2) | UV Tiling |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Time (0-1) (Vector2) | Time (0-1) |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |

## Exact Input Wiring

### [7] Input UVs (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [8] Append

- `A` <= [0] PanTextureCoordinateChannelfrom-1ton+1 output 0
- `B` <= [1] PanTextureCoordinateChannelfrom-1ton+1 output 0

### [9] Output UVs

- `A` <= [8] Append output 0

### [10] Input Time (0-1) (Vector2)

- `Preview` <= [11] TimeWithSpeedVariable output 0

## Connection List

- [2] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [0] PanTextureCoordinateChannelfrom-1ton+1 output 0 -> Append.A
- [1] PanTextureCoordinateChannelfrom-1ton+1 output 0 -> Append.B
- [8] Append output 0 -> Output UVs.A
- [11] TimeWithSpeedVariable output 0 -> Input Time (0-1) (Vector2).Preview

## Output Trace Roots

- `UVs` is driven by [8] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


