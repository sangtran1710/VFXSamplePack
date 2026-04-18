# LinearGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/LinearGradient.LinearGradient`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Uses UV Channel 0 to generate a linear gradient along the U or V axis

## Inputs

- `UV Channel` [FunctionInput_Vector2]

## Outputs

- `VGradient` - Gradient along V axis of UV0
- `UGradient` - Gradient along U axis of UV0

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output VGradient | VGradient |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output UGradient | UGradient |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input UV Channel (Vector2) | UV Channel |

## Exact Input Wiring

### [0] Output VGradient

- `A` <= [1] Mask ( G ) output 0

### [1] Mask ( G )

- `Input` <= [5] Input UV Channel (Vector2) output 0

### [3] Output UGradient

- `A` <= [4] Mask ( R ) output 0

### [4] Mask ( R )

- `Input` <= [5] Input UV Channel (Vector2) output 0

### [5] Input UV Channel (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

## Connection List

- [1] Mask ( G ) output 0 -> Output VGradient.A
- [5] Input UV Channel (Vector2) output 0 -> Mask ( G ).Input
- [4] Mask ( R ) output 0 -> Output UGradient.A
- [5] Input UV Channel (Vector2) output 0 -> Mask ( R ).Input
- [2] TexCoord[0] output 0 -> Input UV Channel (Vector2).Preview

## Output Trace Roots

- `VGradient` is driven by [1] Mask ( G )
- `UGradient` is driven by [4] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
