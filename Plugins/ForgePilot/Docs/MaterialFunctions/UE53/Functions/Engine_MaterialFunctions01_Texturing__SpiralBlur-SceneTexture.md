# SpiralBlur-SceneTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SpiralBlur-SceneTexture.SpiralBlur-SceneTexture`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs a Spiral Blur with controllable number of steps and offests.

## Inputs

- `Distance Steps` [FunctionInput_Scalar] - Number of steps along distance
- `Distance` [FunctionInput_Scalar] - Total Distance of Blur in UV Space
- `Radial Steps` [FunctionInput_Scalar] - Number of radial steps to perform
- `Radial Offset` [FunctionInput_Scalar] - Number of radial steps to perform
- `TempAA Radial Blur` [FunctionInput_Scalar] - Number of radial steps to perform
- `TempAA Distance Blur` [FunctionInput_Scalar] - Number of radial steps to perform
- `Distance Mask` [FunctionInput_Scalar] - Supply a mask for objects closer to the surface to receive less blur

## Outputs

- `Result`
- `SceneColor clamp to 0` - You will need to hook this up to your opacity chain for the scenetexture to work. Since it has been clamped to 0 you should simply Add this to the rest of your opacity chain

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Distance Steps (Scalar) | Distance Steps |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Distance (Scalar) | Distance |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Radial Steps (Scalar) | Radial Steps |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Radial Offset (Scalar) | Radial Offset |
| 5 | custom | `MaterialExpressionCustom` | Custom |  |
| 6 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input TempAA Radial Blur (Scalar) | TempAA Radial Blur |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input TempAA Distance Blur (Scalar) | TempAA Distance Blur |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Distance Mask (Scalar) | Distance Mask |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output SceneColor clamp to 0 | SceneColor clamp to 0 |
| 11 | expression | `MaterialExpressionMax` | Max(,1) |  |
| 12 | expression | `MaterialExpressionSceneColor` | Scene Color |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [5] Custom output 0

### [10] Output SceneColor clamp to 0

- `A` <= [11] Max(,1) output 0

### [11] Max(,1)

- `A` <= [12] Scene Color output 0

## Connection List

- [5] Custom output 0 -> Output Result.A
- [11] Max(,1) output 0 -> Output SceneColor clamp to 0.A
- [12] Scene Color output 0 -> Max(,1).A

## Output Trace Roots

- `Result` is driven by [5] Custom
- `SceneColor clamp to 0` is driven by [11] Max(,1)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
