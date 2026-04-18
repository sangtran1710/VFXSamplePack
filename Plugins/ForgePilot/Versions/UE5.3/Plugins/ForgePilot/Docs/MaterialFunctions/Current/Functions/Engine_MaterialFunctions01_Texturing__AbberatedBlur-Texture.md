# AbberatedBlur-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/AbberatedBlur-Texture.AbberatedBlur-Texture`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Blurs a Texture along a specified 2D Axis with chromatic abberation.

## Inputs

- `TextureObject` [FunctionInput_Texture2D] - Supply the Texture to Blur as a TextureObject node
- `UV` [FunctionInput_Vector2] - UVs for blurring
- `Steps` [FunctionInput_Scalar] - Number of steps
- `Distance` [FunctionInput_Scalar] - Total Distance of Blur in UV Space
- `Vector` [FunctionInput_Vector2] - V2 vector for blur direction

## Outputs

- `Result With Abberation`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input UV (Vector2) | UV |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Steps (Scalar) | Steps |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Distance (Scalar) | Distance |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector2) | Vector |
| 6 | custom | `MaterialExpressionCustom` | Custom |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Result With Abberation | Result With Abberation |

## Exact Input Wiring

### [2] Input UV (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [7] Output Result With Abberation

- `A` <= [6] Custom output 0

## Connection List

- [1] TexCoord[0] output 0 -> Input UV (Vector2).Preview
- [6] Custom output 0 -> Output Result With Abberation.A

## Output Trace Roots

- `Result With Abberation` is driven by [6] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


