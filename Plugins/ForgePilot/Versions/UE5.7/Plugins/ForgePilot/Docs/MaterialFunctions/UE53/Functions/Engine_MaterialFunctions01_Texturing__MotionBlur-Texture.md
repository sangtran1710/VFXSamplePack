# MotionBlur-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/MotionBlur-Texture.MotionBlur-Texture`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Blurs a Texture along a specified 2D Axis

## Inputs

- `TextureObject` [FunctionInput_Texture2D] - Supply the Texture to Blur as a TextureObject node
- `UV` [FunctionInput_Vector2] - UVs for blurring
- `Steps` [FunctionInput_Scalar] - Number of steps
- `Distance` [FunctionInput_Scalar] - Total Distance of Blur in UV Space
- `Vector` [FunctionInput_Vector2] - V2 vector for blur direction

## Outputs

- `Result`
- `Results with Chromatic Shift` - Blurs a texture along a given axis with a chromatic shift operation applied.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input UV (Vector2) | UV |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Steps (Scalar) | Steps |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Distance (Scalar) | Distance |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector2) | Vector |
| 8 | custom | `MaterialExpressionCustom` | Custom |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output Results with Chromatic Shift | Results with Chromatic Shift |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Custom output 0

### [4] Input UV (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

### [9] Output Results with Chromatic Shift

- `A` <= [8] Custom output 0

## Connection List

- [1] Custom output 0 -> Output Result.A
- [3] TexCoord[0] output 0 -> Input UV (Vector2).Preview
- [8] Custom output 0 -> Output Results with Chromatic Shift.A

## Output Trace Roots

- `Result` is driven by [1] Custom
- `Results with Chromatic Shift` is driven by [8] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
