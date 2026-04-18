# ScaleUVsAroundPoint

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ScaleUVsAroundPoint.ScaleUVsAroundPoint`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Offset XY Texturesize Z` [FunctionInput_Vector3] - X and Y should specify the desired center of scaling in pixels. Z should be the texture size in pixels (ie, 1024).  Currently only supports square textures!
- `Texture Scale` [FunctionInput_Vector2] - Scale factor. Default = 1
- `UVs` [FunctionInput_Vector2] - UVs. Only need to plug in if you want a different UV channel.

## Outputs

- `UVs`
- `0-1 mask`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Offset XY Texturesize Z (Vector3) | Offset XY Texturesize Z |
| 2 | expression | `MaterialExpressionConstant3Vector` | 1.02e+03,1.02e+03,2.05e+03 |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Texture Scale (Vector2) | Texture Scale |
| 8 | expression | `MaterialExpressionConstant` | 1 |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 15 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 16 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 17 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output 0-1 mask | 0-1 mask |

## Exact Input Wiring

### [0] Output UVs

- `A` <= [9] Subtract output 0

### [1] Input Offset XY Texturesize Z (Vector3)

- `Preview` <= [2] 1.02e+03,1.02e+03,2.05e+03 output 0

### [3] Mask ( R G )

- `Input` <= [1] Input Offset XY Texturesize Z (Vector3) output 0

### [4] Add

- `A` <= [10] Divide output 0
- `B` <= [6] Divide output 0

### [5] Mask ( B )

- `Input` <= [1] Input Offset XY Texturesize Z (Vector3) output 0

### [6] Divide

- `A` <= [3] Mask ( R G ) output 0
- `B` <= [5] Mask ( B ) output 0

### [7] Input Texture Scale (Vector2)

- `Preview` <= [8] 1 output 0

### [9] Subtract

- `A` <= [4] Add output 0
- `B` <= [11] Divide output 0

### [10] Divide

- `A` <= [13] Input UVs (Vector2) output 0
- `B` <= [7] Input Texture Scale (Vector2) output 0

### [11] Divide

- `A` <= [6] Divide output 0
- `B` <= [7] Input Texture Scale (Vector2) output 0

### [13] Input UVs (Vector2)

- `Preview` <= [12] TexCoord[0] output 0

### [15] SphereMask

- `A` <= [14] BreakOutFloat2Components output 1
- `B` <= [16] 0.5 output 0

### [17] SphereMask

- `A` <= [16] 0.5 output 0
- `B` <= [14] BreakOutFloat2Components output 0

### [18] Multiply

- `A` <= [17] SphereMask output 0
- `B` <= [15] SphereMask output 0

### [19] Output 0-1 mask

- `A` <= [18] Multiply output 0

## Connection List

- [9] Subtract output 0 -> Output UVs.A
- [2] 1.02e+03,1.02e+03,2.05e+03 output 0 -> Input Offset XY Texturesize Z (Vector3).Preview
- [1] Input Offset XY Texturesize Z (Vector3) output 0 -> Mask ( R G ).Input
- [10] Divide output 0 -> Add.A
- [6] Divide output 0 -> Add.B
- [1] Input Offset XY Texturesize Z (Vector3) output 0 -> Mask ( B ).Input
- [3] Mask ( R G ) output 0 -> Divide.A
- [5] Mask ( B ) output 0 -> Divide.B
- [8] 1 output 0 -> Input Texture Scale (Vector2).Preview
- [4] Add output 0 -> Subtract.A
- [11] Divide output 0 -> Subtract.B
- [13] Input UVs (Vector2) output 0 -> Divide.A
- [7] Input Texture Scale (Vector2) output 0 -> Divide.B
- [6] Divide output 0 -> Divide.A
- [7] Input Texture Scale (Vector2) output 0 -> Divide.B
- [12] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [14] BreakOutFloat2Components output 1 -> SphereMask.A
- [16] 0.5 output 0 -> SphereMask.B
- [16] 0.5 output 0 -> SphereMask.A
- [14] BreakOutFloat2Components output 0 -> SphereMask.B
- [17] SphereMask output 0 -> Multiply.A
- [15] SphereMask output 0 -> Multiply.B
- [18] Multiply output 0 -> Output 0-1 mask.A

## Output Trace Roots

- `UVs` is driven by [9] Subtract
- `0-1 mask` is driven by [18] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


