# RadialGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/RadialGradient.RadialGradient`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Uses UV Channel 0 to create a radial gradient

## Inputs

- `CenterPosition` [FunctionInput_Vector2] - Position in UV space
- `Radius` [FunctionInput_Scalar] - Radius in fraction of UV Space

## Outputs

- `RadialGradient`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RadialGradient | RadialGradient |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input CenterPosition (Vector2) | CenterPosition |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |

## Exact Input Wiring

### [0] Output RadialGradient

- `A` <= [2] SphereMask output 0

### [2] SphereMask

- `A` <= [1] TexCoord[0] output 0
- `B` <= [3] Input CenterPosition (Vector2) output 0
- `Radius` <= [4] Input Radius (Scalar) output 0

## Connection List

- [2] SphereMask output 0 -> Output RadialGradient.A
- [1] TexCoord[0] output 0 -> SphereMask.A
- [3] Input CenterPosition (Vector2) output 0 -> SphereMask.B
- [4] Input Radius (Scalar) output 0 -> SphereMask.Radius

## Output Trace Roots

- `RadialGradient` is driven by [2] SphereMask

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


