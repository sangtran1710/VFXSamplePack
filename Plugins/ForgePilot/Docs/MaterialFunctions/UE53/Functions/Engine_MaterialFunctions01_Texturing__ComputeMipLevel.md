# ComputeMipLevel

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ComputeMipLevel.ComputeMipLevel`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Texture Size` [FunctionInput_Vector2] - Custom Mip math needs to know the size of your texture in Pixels
- `UVs` [FunctionInput_Vector2] - UVs to compute Mips from

## Outputs

- `MipLevel`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output MipLevel | MipLevel |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionDDX` | DDX |  |
| 3 | expression | `MaterialExpressionIf` | If |  |
| 4 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionDDY` | DDY |  |
| 7 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 8 | expression | `MaterialExpressionConstant` | 2048 |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Texture Size (Vector2) | Texture Size |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 11 | custom | `MaterialExpressionCustom` | Custom |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |

## Exact Input Wiring

### [0] Output MipLevel

- `A` <= [11] Custom output 0

### [1] Dot

- `A` <= [12] Multiply output 0
- `B` <= [12] Multiply output 0

### [2] DDX

- `Value` <= [10] Input UVs (Vector2) output 0

### [3] If

- `A` <= [1] Dot output 0
- `B` <= [5] Dot output 0
- `AGreaterThanB` <= [1] Dot output 0
- `ALessThanB` <= [5] Dot output 0

### [4] Sqrt

- `Input` <= [3] If output 0

### [5] Dot

- `A` <= [13] Multiply output 0
- `B` <= [13] Multiply output 0

### [6] DDY

- `Value` <= [10] Input UVs (Vector2) output 0

### [9] Input Texture Size (Vector2)

- `Preview` <= [8] 2048 output 0

### [10] Input UVs (Vector2)

- `Preview` <= [7] TexCoord[0] output 0

### [12] Multiply

- `A` <= [2] DDX output 0
- `B` <= [14] Mask ( R G ) output 0

### [13] Multiply

- `A` <= [6] DDY output 0
- `B` <= [14] Mask ( R G ) output 0

### [14] Mask ( R G )

- `Input` <= [9] Input Texture Size (Vector2) output 0

## Connection List

- [11] Custom output 0 -> Output MipLevel.A
- [12] Multiply output 0 -> Dot.A
- [12] Multiply output 0 -> Dot.B
- [10] Input UVs (Vector2) output 0 -> DDX.Value
- [1] Dot output 0 -> If.A
- [5] Dot output 0 -> If.B
- [1] Dot output 0 -> If.AGreaterThanB
- [5] Dot output 0 -> If.ALessThanB
- [3] If output 0 -> Sqrt.Input
- [13] Multiply output 0 -> Dot.A
- [13] Multiply output 0 -> Dot.B
- [10] Input UVs (Vector2) output 0 -> DDY.Value
- [8] 2048 output 0 -> Input Texture Size (Vector2).Preview
- [7] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [2] DDX output 0 -> Multiply.A
- [14] Mask ( R G ) output 0 -> Multiply.B
- [6] DDY output 0 -> Multiply.A
- [14] Mask ( R G ) output 0 -> Multiply.B
- [9] Input Texture Size (Vector2) output 0 -> Mask ( R G ).Input

## Output Trace Roots

- `MipLevel` is driven by [11] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
