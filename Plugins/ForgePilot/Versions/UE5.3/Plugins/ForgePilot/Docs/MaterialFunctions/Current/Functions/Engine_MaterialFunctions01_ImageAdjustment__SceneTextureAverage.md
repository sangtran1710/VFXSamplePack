# SceneTextureAverage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/SceneTextureAverage.SceneTextureAverage`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Samples and averages the scene texture 8 times in a circular pattern.

## Inputs

- `OffsetAmount` [FunctionInput_Vector2] - Scene color offset fraction. 

## Outputs

- `8 Samples`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 8 Samples | 8 Samples |
| 1 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionSceneColor` | Scene Color |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input OffsetAmount (Vector2) | OffsetAmount |
| 17 | expression | `MaterialExpressionDivide` | Divide(,8) |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | BlurSampleOffsets |  |

## Exact Input Wiring

### [0] Output 8 Samples

- `A` <= [17] Divide(,8) output 0

### [1] Scene Color

- `Input` <= [18] BlurSampleOffsets output 0

### [2] Add

- `A` <= [1] Scene Color output 0
- `B` <= [3] Scene Color output 0

### [3] Scene Color

- `Input` <= [18] BlurSampleOffsets output 1

### [4] Add

- `A` <= [2] Add output 0
- `B` <= [6] Add output 0

### [5] Scene Color

- `Input` <= [18] BlurSampleOffsets output 2

### [6] Add

- `A` <= [5] Scene Color output 0
- `B` <= [7] Scene Color output 0

### [7] Scene Color

- `Input` <= [18] BlurSampleOffsets output 3

### [8] Add

- `A` <= [4] Add output 0
- `B` <= [12] Add output 0

### [9] Scene Color

- `Input` <= [18] BlurSampleOffsets output 4

### [10] Add

- `A` <= [9] Scene Color output 0
- `B` <= [11] Scene Color output 0

### [11] Scene Color

- `Input` <= [18] BlurSampleOffsets output 5

### [12] Add

- `A` <= [10] Add output 0
- `B` <= [14] Add output 0

### [13] Scene Color

- `Input` <= [18] BlurSampleOffsets output 6

### [14] Add

- `A` <= [13] Scene Color output 0
- `B` <= [15] Scene Color output 0

### [15] Scene Color

- `Input` <= [18] BlurSampleOffsets output 7

### [17] Divide(,8)

- `A` <= [8] Add output 0

## Connection List

- [17] Divide(,8) output 0 -> Output 8 Samples.A
- [18] BlurSampleOffsets output 0 -> Scene Color.Input
- [1] Scene Color output 0 -> Add.A
- [3] Scene Color output 0 -> Add.B
- [18] BlurSampleOffsets output 1 -> Scene Color.Input
- [2] Add output 0 -> Add.A
- [6] Add output 0 -> Add.B
- [18] BlurSampleOffsets output 2 -> Scene Color.Input
- [5] Scene Color output 0 -> Add.A
- [7] Scene Color output 0 -> Add.B
- [18] BlurSampleOffsets output 3 -> Scene Color.Input
- [4] Add output 0 -> Add.A
- [12] Add output 0 -> Add.B
- [18] BlurSampleOffsets output 4 -> Scene Color.Input
- [9] Scene Color output 0 -> Add.A
- [11] Scene Color output 0 -> Add.B
- [18] BlurSampleOffsets output 5 -> Scene Color.Input
- [10] Add output 0 -> Add.A
- [14] Add output 0 -> Add.B
- [18] BlurSampleOffsets output 6 -> Scene Color.Input
- [13] Scene Color output 0 -> Add.A
- [15] Scene Color output 0 -> Add.B
- [18] BlurSampleOffsets output 7 -> Scene Color.Input
- [8] Add output 0 -> Divide(,8).A

## Output Trace Roots

- `8 Samples` is driven by [17] Divide(,8)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


