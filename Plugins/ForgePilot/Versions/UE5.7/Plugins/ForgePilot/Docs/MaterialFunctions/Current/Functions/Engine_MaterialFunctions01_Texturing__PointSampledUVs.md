# PointSampledUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/PointSampledUVs.PointSampledUVs`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns UVs that are point sampled which is a way to force nearest neighbor sampling. This should only be used with a forced mip level and only in situations that require it (such as writing render targets in non power of 2 sizes).

## Inputs

- `UVs` [FunctionInput_Vector2]
- `Texture Size` [FunctionInput_Vector2]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Texture Size (Vector2) | Texture Size |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionFloor` | Floor |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [6] Divide output 0

### [1] Input UVs (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

### [4] Multiply

- `A` <= [1] Input UVs (Vector2) output 0
- `B` <= [2] Input Texture Size (Vector2) output 0

### [5] Floor

- `Input` <= [4] Multiply output 0

### [6] Divide

- `A` <= [7] Add(,0.5) output 0
- `B` <= [2] Input Texture Size (Vector2) output 0

### [7] Add(,0.5)

- `A` <= [5] Floor output 0

## Connection List

- [6] Divide output 0 -> Output Result.A
- [3] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [1] Input UVs (Vector2) output 0 -> Multiply.A
- [2] Input Texture Size (Vector2) output 0 -> Multiply.B
- [4] Multiply output 0 -> Floor.Input
- [7] Add(,0.5) output 0 -> Divide.A
- [2] Input Texture Size (Vector2) output 0 -> Divide.B
- [5] Floor output 0 -> Add(,0.5).A

## Output Trace Roots

- `Result` is driven by [6] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


