# ms_PivotPainter2_CalculateMeshElementIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_CalculateMeshElementIndex.ms_PivotPainter2_CalculateMeshElementIndex`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Pulls the model elements element ID from the models uvs.

## Inputs

- `Pivot Painter UV Coordinates` [FunctionInput_Vector2]
- `Data Texture Dimensions` [FunctionInput_Vector2] - Use the texture property node to gather the texture's dimensions

## Outputs

- `Index`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Index | Index |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionFloor` | Floor |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Pivot Painter UV Coordinates (Vector2) | Pivot Painter UV Coordinates |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Data Texture Dimensions (Vector2) | Data Texture Dimensions |

## Exact Input Wiring

### [0] Output Index

- `A` <= [5] Add output 0

### [3] Multiply

- `A` <= [4] BreakOutFloat2Components output 1
- `B` <= [2] BreakOutFloat2Components output 0

### [5] Add

- `A` <= [4] BreakOutFloat2Components output 0
- `B` <= [3] Multiply output 0

### [6] Multiply

- `A` <= [8] Input Pivot Painter UV Coordinates (Vector2) output 0
- `B` <= [9] Input Data Texture Dimensions (Vector2) output 0

### [7] Floor

- `Input` <= [6] Multiply output 0

### [8] Input Pivot Painter UV Coordinates (Vector2)

- `Preview` <= [1] TexCoord[1] output 0

## Connection List

- [5] Add output 0 -> Output Index.A
- [4] BreakOutFloat2Components output 1 -> Multiply.A
- [2] BreakOutFloat2Components output 0 -> Multiply.B
- [4] BreakOutFloat2Components output 0 -> Add.A
- [3] Multiply output 0 -> Add.B
- [8] Input Pivot Painter UV Coordinates (Vector2) output 0 -> Multiply.A
- [9] Input Data Texture Dimensions (Vector2) output 0 -> Multiply.B
- [6] Multiply output 0 -> Floor.Input
- [1] TexCoord[1] output 0 -> Input Pivot Painter UV Coordinates (Vector2).Preview

## Output Trace Roots

- `Index` is driven by [5] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
