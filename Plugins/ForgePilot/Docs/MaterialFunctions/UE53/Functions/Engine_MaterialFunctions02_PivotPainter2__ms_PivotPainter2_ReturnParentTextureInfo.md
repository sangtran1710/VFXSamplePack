# ms_PivotPainter2_ReturnParentTextureInfo

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_ReturnParentTextureInfo.ms_PivotPainter2_ReturnParentTextureInfo`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Read a parent sub object's texture data using Pivot Painter 2.0 parent index integer as float texture data.

## Inputs

- `Parent Index As Float (See note)` [FunctionInput_Scalar] - This input assumes that the data is in FLOAT form. If you're reading from a parent index int as float texture please first decode the asset using MaterialFunction'/Engine/Functions/Engine_MaterialFunctions02/ms_PivotPainter2_UnpackIntegerAsFloat.ms_PivotPainter2_UnpackIntegerAsFloat'. 
- `Texture Dimensions` [FunctionInput_Vector2]
- `Current Index (see note)` [FunctionInput_Vector3] - Providing this value is only nessesary if one would like to determine if this asset is a child of another component. 

## Outputs

- `Parent UVs` - Returns the UV coordinates for the parent object's data location.
- `Is Child? (See note)` - Returns 1 if the object is a child of another object. Returns 0 otherwise. This requires that the current index be entered in the current index input. The current index can be found using the ms_PivotPainter2_CalculateMeshElementIndex material function if you are referencing your textures using the models uvs.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_input | `MaterialExpressionFunctionInput` | Input Parent Index As Float (See note) (Scalar) | Parent Index As Float (See note) |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | 2dArrayLookupByIndex |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Texture Dimensions (Vector2) | Texture Dimensions |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output Parent UVs | Parent UVs |
| 4 | expression | `MaterialExpressionIf` | If |  |
| 5 | expression | `MaterialExpressionConstant` | 1 |  |
| 6 | expression | `MaterialExpressionConstant` | 0 |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Is Child? (See note) | Is Child? (See note) |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Current Index (see note) (Vector3) | Current Index (see note) |

## Exact Input Wiring

### [3] Output Parent UVs

- `A` <= [1] 2dArrayLookupByIndex output 0

### [4] If

- `A` <= [0] Input Parent Index As Float (See note) (Scalar) output 0
- `B` <= [9] Input Current Index (see note) (Vector3) output 0
- `AGreaterThanB` <= [5] 1 output 0
- `AEqualsB` <= [6] 0 output 0
- `ALessThanB` <= [5] 1 output 0

### [7] Output Is Child? (See note)

- `A` <= [8] Mask ( R ) output 0

### [8] Mask ( R )

- `Input` <= [4] If output 0

## Connection List

- [1] 2dArrayLookupByIndex output 0 -> Output Parent UVs.A
- [0] Input Parent Index As Float (See note) (Scalar) output 0 -> If.A
- [9] Input Current Index (see note) (Vector3) output 0 -> If.B
- [5] 1 output 0 -> If.AGreaterThanB
- [6] 0 output 0 -> If.AEqualsB
- [5] 1 output 0 -> If.ALessThanB
- [8] Mask ( R ) output 0 -> Output Is Child? (See note).A
- [4] If output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Parent UVs` is driven by [1] 2dArrayLookupByIndex
- `Is Child? (See note)` is driven by [8] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
