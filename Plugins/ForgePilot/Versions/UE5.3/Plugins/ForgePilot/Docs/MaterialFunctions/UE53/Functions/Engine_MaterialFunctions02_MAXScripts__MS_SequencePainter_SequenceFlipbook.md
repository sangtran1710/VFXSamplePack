# MS_SequencePainter_SequenceFlipbook

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_SequencePainter_SequenceFlipbook.MS_SequencePainter_SequenceFlipbook`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This function will create a mesh flipbook from assets processed with the "Sequence Painter" section of the "Texture Morph"  MAXScript.

## Inputs

- `Number Of Frames` [FunctionInput_Scalar] - Enter the number of static meshes that were processed by the script.
- `0-1 Animation` [FunctionInput_Scalar]

## Outputs

- `World Position Offset`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | ComponentPivotLocation |  |
| 3 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 9 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 10 | expression | `MaterialExpressionCeil` | Ceil |  |
| 11 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 13 | expression | `MaterialExpressionCeil` | Ceil |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Number Of Frames (Scalar) | Number Of Frames |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input 0-1 Animation (Scalar) | 0-1 Animation |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [9] 1-x output 0
- `B` <= [4] Subtract output 0

### [4] Subtract

- `A` <= [2] ComponentPivotLocation output 0
- `B` <= [3] Absolute World Position output 0

### [5] SphereMask

- `A` <= [13] Ceil output 0
- `B` <= [10] Ceil output 0
- `Radius` <= [8] 0.5 output 0

### [7] Multiply

- `A` <= [15] Input 0-1 Animation (Scalar) output 0
- `B` <= [14] Input Number Of Frames (Scalar) output 0

### [9] 1-x

- `Input` <= [5] SphereMask output 0

### [10] Ceil

- `Input` <= [7] Multiply output 0

### [13] Ceil

- `Input` <= [12] BreakOutFloat2Components output 0

### [15] Input 0-1 Animation (Scalar)

- `Preview` <= [6] TimeWithSpeedVariable output 0

## Connection List

- [1] Multiply output 0 -> Output World Position Offset.A
- [9] 1-x output 0 -> Multiply.A
- [4] Subtract output 0 -> Multiply.B
- [2] ComponentPivotLocation output 0 -> Subtract.A
- [3] Absolute World Position output 0 -> Subtract.B
- [13] Ceil output 0 -> SphereMask.A
- [10] Ceil output 0 -> SphereMask.B
- [8] 0.5 output 0 -> SphereMask.Radius
- [15] Input 0-1 Animation (Scalar) output 0 -> Multiply.A
- [14] Input Number Of Frames (Scalar) output 0 -> Multiply.B
- [5] SphereMask output 0 -> 1-x.Input
- [7] Multiply output 0 -> Ceil.Input
- [12] BreakOutFloat2Components output 0 -> Ceil.Input
- [6] TimeWithSpeedVariable output 0 -> Input 0-1 Animation (Scalar).Preview

## Output Trace Roots

- `World Position Offset` is driven by [1] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
