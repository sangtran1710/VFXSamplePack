# GetUserInterfaceUV

- Path: `/Engine/Functions/UserInterface/GetUserInterfaceUV.GetUserInterfaceUV`
- Category: UserInterface
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Provides access to various built in UV sets for UI materials rendered with Slate or UMG

## Inputs

- None

## Outputs

- `9-Slice UV` - The computed UVs for a 9-slice box with tiling information.  If the element has no 9 slice then this just returns a tiled 0-1 uv set
- `Tiling` - The computed horizontal and vertical tiling for this element
- `9-Slice UV (No Tiling)` - The computed UVs for a 9-slice box without tiling information.  If the element has no 9 slice then this just returns a 0-1 uv set
- `Pixel Size` - The screen space size of the element being drawn
- `Normalized UV` - A 0-1 UV set that spans the entire element

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 9-Slice UV | 9-Slice UV |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output Tiling | Tiling |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output 9-Slice UV (No Tiling) | 9-Slice UV (No Tiling) |
| 5 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 6 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Size | Pixel Size |
| 8 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[4] |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output Normalized UV | Normalized UV |

## Exact Input Wiring

### [0] Output 9-Slice UV

- `A` <= [1] TexCoord[0] output 0

### [2] Output Tiling

- `A` <= [3] TexCoord[2] output 0

### [4] Output 9-Slice UV (No Tiling)

- `A` <= [5] TexCoord[1] output 0

### [7] Output Pixel Size

- `A` <= [6] TexCoord[3] output 0

### [9] Output Normalized UV

- `A` <= [8] TexCoord[4] output 0

## Connection List

- [1] TexCoord[0] output 0 -> Output 9-Slice UV.A
- [3] TexCoord[2] output 0 -> Output Tiling.A
- [5] TexCoord[1] output 0 -> Output 9-Slice UV (No Tiling).A
- [6] TexCoord[3] output 0 -> Output Pixel Size.A
- [8] TexCoord[4] output 0 -> Output Normalized UV.A

## Output Trace Roots

- `9-Slice UV` is driven by [1] TexCoord[0]
- `Tiling` is driven by [3] TexCoord[2]
- `9-Slice UV (No Tiling)` is driven by [5] TexCoord[1]
- `Pixel Size` is driven by [6] TexCoord[3]
- `Normalized UV` is driven by [8] TexCoord[4]

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


