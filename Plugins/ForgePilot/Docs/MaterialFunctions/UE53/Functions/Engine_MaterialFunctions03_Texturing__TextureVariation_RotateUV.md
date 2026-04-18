# TextureVariation_RotateUV

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation_RotateUV.TextureVariation_RotateUV`
- Category: Engine_MaterialFunctions03/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `UVs` [FunctionInput_Vector2]
- `Vector` [FunctionInput_Vector2]
- `Offset` [FunctionInput_Vector2]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector2) | Vector |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Vector2) | Offset |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [13] Add output 0

### [1] Input UVs (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

### [6] Multiply

- `A` <= [12] Reroute Node (reroutes wires) output 0
- `B` <= [5] BreakOutFloat2Components output 0

### [8] Append

- `A` <= [9] Multiply(,-1) output 0
- `B` <= [7] BreakOutFloat2Components output 0

### [9] Multiply(,-1)

- `A` <= [7] BreakOutFloat2Components output 1

### [10] Multiply

- `A` <= [8] Append output 0
- `B` <= [5] BreakOutFloat2Components output 1

### [11] Add

- `A` <= [6] Multiply output 0
- `B` <= [10] Multiply output 0

### [12] Reroute Node (reroutes wires)

- `Input` <= [2] Input Vector (Vector2) output 0

### [13] Add

- `A` <= [4] Input Offset (Vector2) output 0
- `B` <= [11] Add output 0

## Connection List

- [13] Add output 0 -> Output Result.A
- [3] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [12] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [5] BreakOutFloat2Components output 0 -> Multiply.B
- [9] Multiply(,-1) output 0 -> Append.A
- [7] BreakOutFloat2Components output 0 -> Append.B
- [7] BreakOutFloat2Components output 1 -> Multiply(,-1).A
- [8] Append output 0 -> Multiply.A
- [5] BreakOutFloat2Components output 1 -> Multiply.B
- [6] Multiply output 0 -> Add.A
- [10] Multiply output 0 -> Add.B
- [2] Input Vector (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [4] Input Offset (Vector2) output 0 -> Add.A
- [11] Add output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [13] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
