# ScreenAlignedPixelToPixelUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/ScreenAlignedPixelToPixelUVs.ScreenAlignedPixelToPixelUVs`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Maps a texture to the screen without stretching any of the pixels. Each pixel on the screen will line up with a pixel from the sampled texture. Note that this will preview incorrectly in the material editor.

## Inputs

- `TextureResolution` [FunctionInput_Vector2] - Input the x and y dimensions of the texture you're referencing. 

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionDivide` | Divide |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input TextureResolution (Vector2) | TextureResolution |
| 3 | expression | `MaterialExpressionScreenPosition` | ScreenPosition |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Divide output 0

### [1] Divide

- `A` <= [3] ScreenPosition output 1
- `B` <= [2] Input TextureResolution (Vector2) output 0

## Connection List

- [1] Divide output 0 -> Output Result.A
- [3] ScreenPosition output 1 -> Divide.A
- [2] Input TextureResolution (Vector2) output 0 -> Divide.B

## Output Trace Roots

- `Result` is driven by [1] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
