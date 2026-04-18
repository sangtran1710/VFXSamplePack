# TextureVariation_RotateNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation_RotateNormals.TextureVariation_RotateNormals`
- Category: Engine_MaterialFunctions03/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `Vector` [FunctionInput_Vector2]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector2) | Vector |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 6 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] MakeFloat3 output 0

### [4] Append

- `A` <= [5] Multiply(,-1) output 0
- `B` <= [3] BreakOutFloat2Components output 0

### [5] Multiply(,-1)

- `A` <= [3] BreakOutFloat2Components output 1

### [6] Reroute Node (reroutes wires)

- `Input` <= [1] Input Vector (Vector2) output 0

### [7] Dot

- `A` <= [6] Reroute Node (reroutes wires) output 0
- `B` <= [9] Mask ( R G ) output 0

### [9] Mask ( R G )

- `Input` <= [2] Input Normal (Vector3) output 0

### [10] Dot

- `A` <= [4] Append output 0
- `B` <= [9] Mask ( R G ) output 0

### [11] Mask ( B )

- `Input` <= [2] Input Normal (Vector3) output 0

## Connection List

- [8] MakeFloat3 output 0 -> Output Result.A
- [5] Multiply(,-1) output 0 -> Append.A
- [3] BreakOutFloat2Components output 0 -> Append.B
- [3] BreakOutFloat2Components output 1 -> Multiply(,-1).A
- [1] Input Vector (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [6] Reroute Node (reroutes wires) output 0 -> Dot.A
- [9] Mask ( R G ) output 0 -> Dot.B
- [2] Input Normal (Vector3) output 0 -> Mask ( R G ).Input
- [4] Append output 0 -> Dot.A
- [9] Mask ( R G ) output 0 -> Dot.B
- [2] Input Normal (Vector3) output 0 -> Mask ( B ).Input

## Output Trace Roots

- `Result` is driven by [8] MakeFloat3

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
