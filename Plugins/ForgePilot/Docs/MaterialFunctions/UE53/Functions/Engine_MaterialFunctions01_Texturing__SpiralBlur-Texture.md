# SpiralBlur-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SpiralBlur-Texture.SpiralBlur-Texture`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs a Spiral Blur with controllable number of steps and offests.

## Inputs

- `TextureObject` [FunctionInput_Texture2D] - Supply the Texture to Blur as a TextureObject node
- `UV` [FunctionInput_Vector2] - UVs for blurring
- `Distance Steps` [FunctionInput_Scalar] - Number of steps along distance
- `Distance` [FunctionInput_Scalar] - Total Distance of Blur in UV Space
- `Radial Steps` [FunctionInput_Scalar] - Number of radial steps to perform
- `Radial Offset` [FunctionInput_Scalar] - Number of radial steps to perform
- `Kernel Power` [FunctionInput_Scalar] - Number of radial steps to perform

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input UV (Vector2) | UV |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Distance Steps (Scalar) | Distance Steps |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Distance (Scalar) | Distance |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Radial Steps (Scalar) | Radial Steps |
| 8 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Radial Offset (Scalar) | Radial Offset |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Kernel Power (Scalar) | Kernel Power |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Custom output 0

### [2] Input TextureObject (Texture2D)

- `Preview` <= [8] Texture Object output 0

### [4] Input UV (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

## Connection List

- [1] Custom output 0 -> Output Result.A
- [8] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [3] TexCoord[0] output 0 -> Input UV (Vector2).Preview

## Output Trace Roots

- `Result` is driven by [1] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
