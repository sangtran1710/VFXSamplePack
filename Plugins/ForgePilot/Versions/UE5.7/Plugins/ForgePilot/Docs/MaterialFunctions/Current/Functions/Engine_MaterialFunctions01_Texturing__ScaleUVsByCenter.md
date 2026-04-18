# ScaleUVsByCenter

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ScaleUVsByCenter.ScaleUVsByCenter`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Texture Scale` [FunctionInput_Vector2] - Scale factor. Default = 1
- `UVs` [FunctionInput_Vector2] - UVs. Only need to plug in if you want a different UV channel.

## Outputs

- `UVs`
- `0-1 mask`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Texture Scale (Vector2) | Texture Scale |
| 4 | expression | `MaterialExpressionConstant` | 1 |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 11 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 12 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 13 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output 0-1 mask | 0-1 mask |

## Exact Input Wiring

### [0] Output UVs

- `A` <= [5] Subtract output 0

### [1] Add

- `A` <= [6] Divide output 0
- `B` <= [8] 0.5 output 0

### [3] Input Texture Scale (Vector2)

- `Preview` <= [4] 1 output 0

### [5] Subtract

- `A` <= [1] Add output 0
- `B` <= [7] Divide output 0

### [6] Divide

- `A` <= [9] Input UVs (Vector2) output 0
- `B` <= [3] Input Texture Scale (Vector2) output 0

### [7] Divide

- `A` <= [8] 0.5 output 0
- `B` <= [3] Input Texture Scale (Vector2) output 0

### [9] Input UVs (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [11] SphereMask

- `A` <= [10] BreakOutFloat2Components output 1
- `B` <= [12] 0.5 output 0

### [13] SphereMask

- `A` <= [12] 0.5 output 0
- `B` <= [10] BreakOutFloat2Components output 0

### [14] Multiply

- `A` <= [13] SphereMask output 0
- `B` <= [11] SphereMask output 0

### [15] Output 0-1 mask

- `A` <= [14] Multiply output 0

## Connection List

- [5] Subtract output 0 -> Output UVs.A
- [6] Divide output 0 -> Add.A
- [8] 0.5 output 0 -> Add.B
- [4] 1 output 0 -> Input Texture Scale (Vector2).Preview
- [1] Add output 0 -> Subtract.A
- [7] Divide output 0 -> Subtract.B
- [9] Input UVs (Vector2) output 0 -> Divide.A
- [3] Input Texture Scale (Vector2) output 0 -> Divide.B
- [8] 0.5 output 0 -> Divide.A
- [3] Input Texture Scale (Vector2) output 0 -> Divide.B
- [2] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [10] BreakOutFloat2Components output 1 -> SphereMask.A
- [12] 0.5 output 0 -> SphereMask.B
- [12] 0.5 output 0 -> SphereMask.A
- [10] BreakOutFloat2Components output 0 -> SphereMask.B
- [13] SphereMask output 0 -> Multiply.A
- [11] SphereMask output 0 -> Multiply.B
- [14] Multiply output 0 -> Output 0-1 mask.A

## Output Trace Roots

- `UVs` is driven by [5] Subtract
- `0-1 mask` is driven by [14] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


