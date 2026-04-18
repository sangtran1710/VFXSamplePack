# UVToLongLat

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/UVToLongLat.UVToLongLat`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This node transforms a UV value into a directional vector which can be used to sample a Cubemap texture as a regular 2D texture.

## Inputs

- `UV` [FunctionInput_Vector2] - The UV to transform into a directional Vector

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input UV (Vector2) | UV |
| 2 | custom | `MaterialExpressionCustom` | Custom |  |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 5 | expression | `MaterialExpressionConstant` | 2 |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 7 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Custom output 0

### [1] Input UV (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

### [7] Add(,0.5)

- `A` <= [6] BreakOutFloat2Components output 0

## Connection List

- [2] Custom output 0 -> Output Result.A
- [3] TexCoord[0] output 0 -> Input UV (Vector2).Preview
- [6] BreakOutFloat2Components output 0 -> Add(,0.5).A

## Output Trace Roots

- `Result` is driven by [2] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
