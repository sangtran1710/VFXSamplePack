# PanTextureCoordinateChannelfrom-1ton+1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/PanTextureCoordinateChannelfrom-1toN+1.PanTextureCoordinateChannelfrom-1ton+1`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function pans a 0-1 texture coordinate channel from -1 to the number of tiled units +1. This is useful when used in conjunction with textures with clamped coordinates.

## Inputs

- `UV Channel` [FunctionInput_Scalar]
- `Tiling Amount` [FunctionInput_Scalar]
- `Time (0-1)` [FunctionInput_Scalar]

## Outputs

- `UV Channel output`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UV Channel output | UV Channel output |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input UV Channel (Scalar) | UV Channel |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Tiling Amount (Scalar) | Tiling Amount |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Time (0-1) (Scalar) | Time (0-1) |

## Exact Input Wiring

### [0] Output UV Channel output

- `A` <= [5] Subtract output 0

### [2] Mask ( R )

- `Input` <= [1] TexCoord[0] output 0

### [3] Multiply

- `A` <= [9] Input UV Channel (Scalar) output 0
- `B` <= [10] Input Tiling Amount (Scalar) output 0

### [4] Add(,1)

- `A` <= [3] Multiply output 0

### [5] Subtract

- `A` <= [4] Add(,1) output 0
- `B` <= [7] Multiply output 0

### [6] Add(,1)

- `A` <= [10] Input Tiling Amount (Scalar) output 0

### [7] Multiply

- `A` <= [6] Add(,1) output 0
- `B` <= [11] Input Time (0-1) (Scalar) output 0

### [9] Input UV Channel (Scalar)

- `Preview` <= [2] Mask ( R ) output 0

### [11] Input Time (0-1) (Scalar)

- `Preview` <= [8] TimeWithSpeedVariable output 0

## Connection List

- [5] Subtract output 0 -> Output UV Channel output.A
- [1] TexCoord[0] output 0 -> Mask ( R ).Input
- [9] Input UV Channel (Scalar) output 0 -> Multiply.A
- [10] Input Tiling Amount (Scalar) output 0 -> Multiply.B
- [3] Multiply output 0 -> Add(,1).A
- [4] Add(,1) output 0 -> Subtract.A
- [7] Multiply output 0 -> Subtract.B
- [10] Input Tiling Amount (Scalar) output 0 -> Add(,1).A
- [6] Add(,1) output 0 -> Multiply.A
- [11] Input Time (0-1) (Scalar) output 0 -> Multiply.B
- [2] Mask ( R ) output 0 -> Input UV Channel (Scalar).Preview
- [8] TimeWithSpeedVariable output 0 -> Input Time (0-1) (Scalar).Preview

## Output Trace Roots

- `UV Channel output` is driven by [5] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
