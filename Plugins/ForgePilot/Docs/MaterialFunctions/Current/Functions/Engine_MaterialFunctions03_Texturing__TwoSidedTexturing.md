# TwoSidedTexturing

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TwoSidedTexturing.TwoSidedTexturing`
- Category: Engine_MaterialFunctions03/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Gives independent texturing control for both sides of a two-sided material.  Will have no effect on non-two sided materials.

## Inputs

- `Texture Side B` [FunctionInput_Vector3] - Input for second side
- `Texture Side A` [FunctionInput_Vector3] - Input for first side

## Outputs

- `Texture Blend Output` - Result
- `Mask` - Outputs only the mask, ignoring texturing

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Texture Blend Output | Texture Blend Output |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Texture Side B (Vector3) | Texture Side B |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Texture Side A (Vector3) | Texture Side A |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Mask | Mask |
| 7 | expression | `MaterialExpressionClamp` | Clamp |  |
| 8 | expression | `MaterialExpressionTwoSidedSign` | TwoSidedSign |  |

## Exact Input Wiring

### [0] Output Texture Blend Output

- `A` <= [3] Lerp output 0

### [1] Input Texture Side B (Vector3)

- `Preview` <= [4] Texture Sample output 0

### [2] Input Texture Side A (Vector3)

- `Preview` <= [4] Texture Sample output 0

### [3] Lerp

- `A` <= [1] Input Texture Side B (Vector3) output 0
- `B` <= [2] Input Texture Side A (Vector3) output 0
- `Alpha` <= [7] Clamp output 0

### [4] Texture Sample

- `TextureObject` <= [5] Texture Object output 0

### [6] Output Mask

- `A` <= [7] Clamp output 0

### [7] Clamp

- `Input` <= [8] TwoSidedSign output 0

## Connection List

- [3] Lerp output 0 -> Output Texture Blend Output.A
- [4] Texture Sample output 0 -> Input Texture Side B (Vector3).Preview
- [4] Texture Sample output 0 -> Input Texture Side A (Vector3).Preview
- [1] Input Texture Side B (Vector3) output 0 -> Lerp.A
- [2] Input Texture Side A (Vector3) output 0 -> Lerp.B
- [7] Clamp output 0 -> Lerp.Alpha
- [5] Texture Object output 0 -> Texture Sample.TextureObject
- [7] Clamp output 0 -> Output Mask.A
- [8] TwoSidedSign output 0 -> Clamp.Input

## Output Trace Roots

- `Texture Blend Output` is driven by [3] Lerp
- `Mask` is driven by [7] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


