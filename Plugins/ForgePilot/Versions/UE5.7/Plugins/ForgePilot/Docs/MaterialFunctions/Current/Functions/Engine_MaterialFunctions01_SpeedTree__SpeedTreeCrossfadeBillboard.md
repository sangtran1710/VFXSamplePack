# SpeedTreeCrossfadeBillboard

- Path: `/Engine/Functions/Engine_MaterialFunctions01/SpeedTree/SpeedTreeCrossfadeBillboard.SpeedTreeCrossfadeBillboard`
- Category: Engine_MaterialFunctions01/SpeedTree
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Handles crossfading between billboard faces

## Inputs

- `Opacity` [FunctionInput_Scalar] - Original opacity

## Outputs

- `OpacityMask`
- `CustomUV`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output OpacityMask | OpacityMask |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 4 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 7 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 8 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output CustomUV | CustomUV |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionConstant` | 0 |  |
| 13 | expression | `MaterialExpressionMin` | Min |  |
| 14 | expression | `MaterialExpressionObjectPositionWS` | Object Position (Absolute) |  |
| 15 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 18 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 24 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0.18,0.38,) |  |

## Exact Input Wiring

### [0] Output OpacityMask

- `A` <= [2] Multiply output 0

### [2] Multiply

- `A` <= [9] Mask ( R ) output 0
- `B` <= [1] Input Opacity (Scalar) output 0

### [4] Dot

- `A` <= [3] VertexNormalWS output 0
- `B` <= [6] View Space to World Space TransformVector output 0

### [5] Multiply

- `A` <= [13] Min output 0
- `B` <= [13] Min output 0

### [6] View Space to World Space TransformVector

- `Input` <= [7] 0,0,1 output 0

### [9] Mask ( R )

- `Input` <= [8] TexCoord[2] output 0

### [10] Output CustomUV

- `A` <= [11] Append output 0

### [11] Append

- `A` <= [20] Saturate output 0
- `B` <= [12] 0 output 0

### [13] Min

- `A` <= [18] Dot output 0
- `B` <= [4] Dot output 0

### [16] Subtract

- `A` <= [14] Object Position (Absolute) output 0
- `B` <= [15] Camera Position output 0

### [17] Normalize

- `VectorInput` <= [16] Subtract output 0

### [18] Dot

- `A` <= [17] Normalize output 0
- `B` <= [3] VertexNormalWS output 0

### [19] Add

- `A` <= [21] Multiply output 0
- `B` <= [24] Lerp(0.18,0.38,) output 0

### [20] Saturate

- `Input` <= [19] Add output 0

### [21] Multiply

- `A` <= [5] Multiply output 0
- `B` <= [5] Multiply output 0

### [23] Mask ( R )

- `Input` <= [22] TexCoord[2] output 0

### [24] Lerp(0.18,0.38,)

- `Alpha` <= [23] Mask ( R ) output 0

## Connection List

- [2] Multiply output 0 -> Output OpacityMask.A
- [9] Mask ( R ) output 0 -> Multiply.A
- [1] Input Opacity (Scalar) output 0 -> Multiply.B
- [3] VertexNormalWS output 0 -> Dot.A
- [6] View Space to World Space TransformVector output 0 -> Dot.B
- [13] Min output 0 -> Multiply.A
- [13] Min output 0 -> Multiply.B
- [7] 0,0,1 output 0 -> View Space to World Space TransformVector.Input
- [8] TexCoord[2] output 0 -> Mask ( R ).Input
- [11] Append output 0 -> Output CustomUV.A
- [20] Saturate output 0 -> Append.A
- [12] 0 output 0 -> Append.B
- [18] Dot output 0 -> Min.A
- [4] Dot output 0 -> Min.B
- [14] Object Position (Absolute) output 0 -> Subtract.A
- [15] Camera Position output 0 -> Subtract.B
- [16] Subtract output 0 -> Normalize.VectorInput
- [17] Normalize output 0 -> Dot.A
- [3] VertexNormalWS output 0 -> Dot.B
- [21] Multiply output 0 -> Add.A
- [24] Lerp(0.18,0.38,) output 0 -> Add.B
- [19] Add output 0 -> Saturate.Input
- [5] Multiply output 0 -> Multiply.A
- [5] Multiply output 0 -> Multiply.B
- [22] TexCoord[2] output 0 -> Mask ( R ).Input
- [23] Mask ( R ) output 0 -> Lerp(0.18,0.38,).Alpha

## Output Trace Roots

- `OpacityMask` is driven by [2] Multiply
- `CustomUV` is driven by [11] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


