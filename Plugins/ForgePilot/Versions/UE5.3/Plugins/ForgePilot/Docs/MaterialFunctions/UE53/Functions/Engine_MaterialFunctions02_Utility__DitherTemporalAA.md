# DitherTemporalAA

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DitherTemporalAA.DitherTemporalAA`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Stipple pattern/Dither  in screen space and time to work well with TemporalAA for things like masked translucency or anisotropic materials

## Inputs

- `Alpha Threshold` [FunctionInput_Scalar]
- `Random` [FunctionInput_Scalar] - 0=no random 1:random(default)

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionConstant2Vector` | 64,64 |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | ScreenAlignedPixelToPixelUVs |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | custom | `MaterialExpressionCustom` | Custom |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Alpha Threshold (Scalar) | Alpha Threshold |
| 8 | expression | `MaterialExpressionAdd` | Add(,-0.5) |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Random (Scalar) | Random |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionViewProperty` | Temporal Sample Index |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | expression | `MaterialExpressionScreenPosition` | ScreenPosition |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,0.1666) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] Add(,-0.5) output 0

### [1] Add

- `A` <= [14] ScreenPosition output 1
- `B` <= [13] Append output 0

### [4] Texture Sample

- `Coordinates` <= [3] ScreenAlignedPixelToPixelUVs output 0

### [5] Add

- `A` <= [6] Custom output 0
- `B` <= [11] Multiply output 0

### [8] Add(,-0.5)

- `A` <= [9] Add output 0

### [9] Add

- `A` <= [7] Input Alpha Threshold (Scalar) output 0
- `B` <= [15] Multiply(,0.1666) output 0

### [11] Multiply

- `A` <= [4] Texture Sample output 1
- `B` <= [10] Input Random (Scalar) output 0

### [13] Append

- `A` <= [12] Temporal Sample Index output 0
- `B` <= [12] Temporal Sample Index output 0

### [15] Multiply(,0.1666)

- `A` <= [5] Add output 0

## Connection List

- [8] Add(,-0.5) output 0 -> Output Result.A
- [14] ScreenPosition output 1 -> Add.A
- [13] Append output 0 -> Add.B
- [3] ScreenAlignedPixelToPixelUVs output 0 -> Texture Sample.Coordinates
- [6] Custom output 0 -> Add.A
- [11] Multiply output 0 -> Add.B
- [9] Add output 0 -> Add(,-0.5).A
- [7] Input Alpha Threshold (Scalar) output 0 -> Add.A
- [15] Multiply(,0.1666) output 0 -> Add.B
- [4] Texture Sample output 1 -> Multiply.A
- [10] Input Random (Scalar) output 0 -> Multiply.B
- [12] Temporal Sample Index output 0 -> Append.A
- [12] Temporal Sample Index output 0 -> Append.B
- [5] Add output 0 -> Multiply(,0.1666).A

## Output Trace Roots

- `Result` is driven by [8] Add(,-0.5)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
