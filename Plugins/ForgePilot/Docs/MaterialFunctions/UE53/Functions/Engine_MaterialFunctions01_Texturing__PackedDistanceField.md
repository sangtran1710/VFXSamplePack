# PackedDistanceField

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/PackedDistanceField.PackedDistanceField`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Reads Distance Field textures packed as a subUV

## Inputs

- `ImageCount` [FunctionInput_Vector2] - How many images horizontally and vertically
- `ImageNumber` [FunctionInput_Scalar] - Which image to grab, left to right top to bottom
- `PackedDistanceField` [FunctionInput_Texture2D] - DistanceField Texture packed as SubUV
- `UVs` [FunctionInput_Vector2]
- `EdgeSoftness` [FunctionInput_Scalar] - How soft the distance field edge should be

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | SubUV_Function |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | DistanceField |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input ImageCount (Vector2) | ImageCount |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input ImageNumber (Scalar) | ImageNumber |
| 5 | expression | `MaterialExpressionCeil` | Ceil |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input PackedDistanceField (Texture2D) | PackedDistanceField |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input EdgeSoftness (Scalar) | EdgeSoftness |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] DistanceField output 0

### [5] Ceil

- `Input` <= [4] Input ImageNumber (Scalar) output 0

### [7] Input UVs (Vector2)

- `Preview` <= [9] TexCoord[0] output 0

## Connection List

- [2] DistanceField output 0 -> Output Result.A
- [4] Input ImageNumber (Scalar) output 0 -> Ceil.Input
- [9] TexCoord[0] output 0 -> Input UVs (Vector2).Preview

## Output Trace Roots

- `Result` is driven by [2] DistanceField

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
