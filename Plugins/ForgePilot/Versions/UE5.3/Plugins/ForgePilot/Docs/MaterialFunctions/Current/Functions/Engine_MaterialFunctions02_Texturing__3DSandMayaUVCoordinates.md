# 3DSandMayaUVCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/3DSandMayaUVCoordinates.3DSandMayaUVCoordinates`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Inverts the green channel in each set of model uvs to place 0,0 in the bottom left hand corner to match 3D Studio Max and Mayas UV layouts. Flipping the values are important if you're storing specific values in the uv channels.

## Inputs

- None

## Outputs

- `UVChannel0` - Unreal channel 0, Max channel 1
- `UVChannel1` - Unreal channel 1, Max channel 2
- `UVChannel2` - Unreal channel 2, Max channel 3
- `UVChannel3` - Unreal channel 3, Max channel 4

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVChannel0 | UVChannel0 |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output UVChannel1 | UVChannel1 |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output UVChannel2 | UVChannel2 |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 15 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 17 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output UVChannel3 | UVChannel3 |
| 19 | expression | `MaterialExpressionAppendVector` | Append |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 21 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 22 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 23 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output UVChannel0

- `A` <= [4] Append output 0

### [2] Mask ( R )

- `Input` <= [1] TexCoord[0] output 0

### [3] Mask ( G )

- `Input` <= [1] TexCoord[0] output 0

### [4] Append

- `A` <= [2] Mask ( R ) output 0
- `B` <= [5] 1-x output 0

### [5] 1-x

- `Input` <= [3] Mask ( G ) output 0

### [6] Output UVChannel1

- `A` <= [7] Append output 0

### [7] Append

- `A` <= [8] Mask ( R ) output 0
- `B` <= [11] 1-x output 0

### [8] Mask ( R )

- `Input` <= [9] TexCoord[1] output 0

### [10] Mask ( G )

- `Input` <= [9] TexCoord[1] output 0

### [11] 1-x

- `Input` <= [10] Mask ( G ) output 0

### [12] Output UVChannel2

- `A` <= [13] Append output 0

### [13] Append

- `A` <= [14] Mask ( R ) output 0
- `B` <= [17] 1-x output 0

### [14] Mask ( R )

- `Input` <= [15] TexCoord[2] output 0

### [16] Mask ( G )

- `Input` <= [15] TexCoord[2] output 0

### [17] 1-x

- `Input` <= [16] Mask ( G ) output 0

### [18] Output UVChannel3

- `A` <= [19] Append output 0

### [19] Append

- `A` <= [20] Mask ( R ) output 0
- `B` <= [23] 1-x output 0

### [20] Mask ( R )

- `Input` <= [21] TexCoord[3] output 0

### [22] Mask ( G )

- `Input` <= [21] TexCoord[3] output 0

### [23] 1-x

- `Input` <= [22] Mask ( G ) output 0

## Connection List

- [4] Append output 0 -> Output UVChannel0.A
- [1] TexCoord[0] output 0 -> Mask ( R ).Input
- [1] TexCoord[0] output 0 -> Mask ( G ).Input
- [2] Mask ( R ) output 0 -> Append.A
- [5] 1-x output 0 -> Append.B
- [3] Mask ( G ) output 0 -> 1-x.Input
- [7] Append output 0 -> Output UVChannel1.A
- [8] Mask ( R ) output 0 -> Append.A
- [11] 1-x output 0 -> Append.B
- [9] TexCoord[1] output 0 -> Mask ( R ).Input
- [9] TexCoord[1] output 0 -> Mask ( G ).Input
- [10] Mask ( G ) output 0 -> 1-x.Input
- [13] Append output 0 -> Output UVChannel2.A
- [14] Mask ( R ) output 0 -> Append.A
- [17] 1-x output 0 -> Append.B
- [15] TexCoord[2] output 0 -> Mask ( R ).Input
- [15] TexCoord[2] output 0 -> Mask ( G ).Input
- [16] Mask ( G ) output 0 -> 1-x.Input
- [19] Append output 0 -> Output UVChannel3.A
- [20] Mask ( R ) output 0 -> Append.A
- [23] 1-x output 0 -> Append.B
- [21] TexCoord[3] output 0 -> Mask ( R ).Input
- [21] TexCoord[3] output 0 -> Mask ( G ).Input
- [22] Mask ( G ) output 0 -> 1-x.Input

## Output Trace Roots

- `UVChannel0` is driven by [4] Append
- `UVChannel1` is driven by [7] Append
- `UVChannel2` is driven by [13] Append
- `UVChannel3` is driven by [19] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


