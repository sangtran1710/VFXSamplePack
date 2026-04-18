# UVRemap_0-1_ToRange

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/UVRemap_0-1_ToRange.UVRemap_0-1_ToRange`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Remaps UVs from 0-1 to the specified range. Useful for when you want control of range rather than tiling.

## Inputs

- `X range Y range` [FunctionInput_Vector4] - X range=R-G, Y range =B-A
- `UVs` [FunctionInput_Vector2] - Leave this input alone unless you want to change index or know the exact effect of remapping when the input range is not 0-1.

## Outputs

- `UV`
- `x`
- `y`
- `Range`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UV | UV |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input X range Y range (Vector4) | X range Y range |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output x | x |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output y | y |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Range | Range |
| 18 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 19 | expression | `MaterialExpressionAdd` | Add(,1) |  |

## Exact Input Wiring

### [0] Output UV

- `A` <= [14] Append output 0

### [2] Lerp

- `A` <= [5] Mask ( B ) output 0
- `B` <= [6] Mask ( A ) output 0
- `Alpha` <= [3] Mask ( G ) output 0

### [3] Mask ( G )

- `Input` <= [13] Input UVs (Vector2) output 0

### [4] Mask ( R )

- `Input` <= [13] Input UVs (Vector2) output 0

### [5] Mask ( B )

- `Input` <= [8] Input X range Y range (Vector4) output 0

### [6] Mask ( A )

- `Input` <= [8] Input X range Y range (Vector4) output 0

### [7] Lerp

- `A` <= [9] Mask ( R ) output 0
- `B` <= [10] Mask ( G ) output 0
- `Alpha` <= [4] Mask ( R ) output 0

### [9] Mask ( R )

- `Input` <= [8] Input X range Y range (Vector4) output 0

### [10] Mask ( G )

- `Input` <= [8] Input X range Y range (Vector4) output 0

### [11] Subtract

- `A` <= [10] Mask ( G ) output 0
- `B` <= [9] Mask ( R ) output 0

### [12] Subtract

- `A` <= [6] Mask ( A ) output 0
- `B` <= [5] Mask ( B ) output 0

### [13] Input UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [14] Append

- `A` <= [7] Lerp output 0
- `B` <= [2] Lerp output 0

### [15] Output x

- `A` <= [7] Lerp output 0

### [16] Output y

- `A` <= [2] Lerp output 0

### [17] Output Range

- `A` <= [8] Input X range Y range (Vector4) output 0

### [18] Add(,1)

- `A` <= [7] Lerp output 0

### [19] Add(,1)

- `A` <= [2] Lerp output 0

## Connection List

- [14] Append output 0 -> Output UV.A
- [5] Mask ( B ) output 0 -> Lerp.A
- [6] Mask ( A ) output 0 -> Lerp.B
- [3] Mask ( G ) output 0 -> Lerp.Alpha
- [13] Input UVs (Vector2) output 0 -> Mask ( G ).Input
- [13] Input UVs (Vector2) output 0 -> Mask ( R ).Input
- [8] Input X range Y range (Vector4) output 0 -> Mask ( B ).Input
- [8] Input X range Y range (Vector4) output 0 -> Mask ( A ).Input
- [9] Mask ( R ) output 0 -> Lerp.A
- [10] Mask ( G ) output 0 -> Lerp.B
- [4] Mask ( R ) output 0 -> Lerp.Alpha
- [8] Input X range Y range (Vector4) output 0 -> Mask ( R ).Input
- [8] Input X range Y range (Vector4) output 0 -> Mask ( G ).Input
- [10] Mask ( G ) output 0 -> Subtract.A
- [9] Mask ( R ) output 0 -> Subtract.B
- [6] Mask ( A ) output 0 -> Subtract.A
- [5] Mask ( B ) output 0 -> Subtract.B
- [1] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [7] Lerp output 0 -> Append.A
- [2] Lerp output 0 -> Append.B
- [7] Lerp output 0 -> Output x.A
- [2] Lerp output 0 -> Output y.A
- [8] Input X range Y range (Vector4) output 0 -> Output Range.A
- [7] Lerp output 0 -> Add(,1).A
- [2] Lerp output 0 -> Add(,1).A

## Output Trace Roots

- `UV` is driven by [14] Append
- `x` is driven by [7] Lerp
- `y` is driven by [2] Lerp
- `Range` is driven by [8] Input X range Y range (Vector4)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
