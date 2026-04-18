# ms_PivotPainter2_SampleLayerData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_SampleLayerData.ms_PivotPainter2_SampleLayerData`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `UVs` [FunctionInput_Vector2]
- `Texture resolution (ParentUVs)` [FunctionInput_Vector2]
- `Current Index (ParentUVs)` [FunctionInput_Scalar]

## Outputs

- `Parent UVs`
- `X axis vector and X extent`
- `Position and Parent Index`
- `Parent Index`
- `Is Child?`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionTextureSampleParameter2D` | Param2D 'Position and Index Texture' |  |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 2 | expression | `MaterialExpressionTextureSampleParameter2D` | Param2D 'X-Axis Texture And X Extent Alpha' |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_Decode8BitAlphaAxisExtent |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_DecodeAxisVector |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_DecodePostion |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_ReturnParentTextureInfo |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Parent UVs | Parent UVs |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Texture resolution (ParentUVs) (Vector2) | Texture resolution (ParentUVs) |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output X axis vector and X extent | X axis vector and X extent |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Position and Parent Index | Position and Parent Index |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | ms_PivotPainter2_UnpackIntegerAsFloat |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Current Index (ParentUVs) (Scalar) | Current Index (ParentUVs) |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Parent Index | Parent Index |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output Is Child? | Is Child? |
| 17 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'Position and Index Texture' |  |
| 18 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'X-Axis Texture And X Extent Alpha' |  |

## Exact Input Wiring

### [0] Param2D 'Position and Index Texture'

- `Coordinates` <= [1] Input UVs (Vector2) output 0

### [2] Param2D 'X-Axis Texture And X Extent Alpha'

- `Coordinates` <= [1] Input UVs (Vector2) output 0

### [7] Output Parent UVs

- `A` <= [6] ms_PivotPainter2_ReturnParentTextureInfo output 0

### [9] Output X axis vector and X extent

- `A` <= [12] Append output 0

### [10] Output Position and Parent Index

- `A` <= [11] Append output 0

### [11] Append

- `A` <= [5] ms_PivotPainter2_DecodePostion output 0
- `B` <= [13] ms_PivotPainter2_UnpackIntegerAsFloat output 0

### [12] Append

- `A` <= [4] ms_PivotPainter2_DecodeAxisVector output 0
- `B` <= [3] ms_PivotPainter2_Decode8BitAlphaAxisExtent output 0

### [15] Output Parent Index

- `A` <= [13] ms_PivotPainter2_UnpackIntegerAsFloat output 0

### [16] Output Is Child?

- `A` <= [6] ms_PivotPainter2_ReturnParentTextureInfo output 1

## Connection List

- [1] Input UVs (Vector2) output 0 -> Param2D 'Position and Index Texture'.Coordinates
- [1] Input UVs (Vector2) output 0 -> Param2D 'X-Axis Texture And X Extent Alpha'.Coordinates
- [6] ms_PivotPainter2_ReturnParentTextureInfo output 0 -> Output Parent UVs.A
- [12] Append output 0 -> Output X axis vector and X extent.A
- [11] Append output 0 -> Output Position and Parent Index.A
- [5] ms_PivotPainter2_DecodePostion output 0 -> Append.A
- [13] ms_PivotPainter2_UnpackIntegerAsFloat output 0 -> Append.B
- [4] ms_PivotPainter2_DecodeAxisVector output 0 -> Append.A
- [3] ms_PivotPainter2_Decode8BitAlphaAxisExtent output 0 -> Append.B
- [13] ms_PivotPainter2_UnpackIntegerAsFloat output 0 -> Output Parent Index.A
- [6] ms_PivotPainter2_ReturnParentTextureInfo output 1 -> Output Is Child?.A

## Output Trace Roots

- `Parent UVs` is driven by [6] ms_PivotPainter2_ReturnParentTextureInfo
- `X axis vector and X extent` is driven by [12] Append
- `Position and Parent Index` is driven by [11] Append
- `Parent Index` is driven by [13] ms_PivotPainter2_UnpackIntegerAsFloat
- `Is Child?` is driven by [6] ms_PivotPainter2_ReturnParentTextureInfo

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


