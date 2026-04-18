# PivotPainter_PerObjectData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_PerObjectData.PivotPainter_PerObjectData`
- Category: Engine_MaterialFunctions02/PivotPainter
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Processes and organizes world position and angle information stored in the model's uvs by the Pivot Painter MAXScript. 

## Inputs

- None

## Outputs

- `Pivot Position` - Returns the pivot point information for each element in world space.
- `Custom Alpha Values` - Returns the custom falloff values stored in the vertex alpha channel of the model.
- `Random Value Per Element` - Returns a random value per element in the 0-1 range.
- `X-Axis Vector` - Returns a normalized vector pointing down the elements X-Axis. Which points from the pivot point toward the averaged center of the mesh.
- `Object Scale` - Returns a scalar value that is equal to the objects uniform scale.
- `Black Mask` - Returns a black value for surfaces that are painted black by the pivot painter tool. This mask only works in the vertex shader.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Pivot Position | Pivot Position |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Custom Alpha Values | Custom Alpha Values |
| 7 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Random Value Per Element | Random Value Per Element |
| 11 | expression | `MaterialExpressionTransform` | Local Space to World Space TransformVector |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 14 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output X-Axis Vector | X-Axis Vector |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Object Scale | Object Scale |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionConstant3Vector` | 0,-1,0 |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Black Mask | Black Mask |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionIf` | If |  |
| 28 | expression | `MaterialExpressionConstant` | 0 |  |
| 29 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [1] Append

- `A` <= [0] TexCoord[2] output 0
- `B` <= [4] Mask ( R ) output 0

### [2] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [20] Add output 0

### [4] Mask ( R )

- `Input` <= [3] TexCoord[3] output 0

### [5] Output Pivot Position

- `A` <= [2] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [6] Output Custom Alpha Values

- `A` <= [18] Mask ( R ) output 0

### [8] Mask ( G )

- `Input` <= [9] TexCoord[3] output 0

### [10] Output Random Value Per Element

- `A` <= [8] Mask ( G ) output 0

### [11] Local Space to World Space TransformVector

- `Input` <= [12] Normalize output 0

### [12] Normalize

- `VectorInput` <= [13] ConstantBiasScale output 0

### [13] ConstantBiasScale

- `Input` <= [14] Vertex Color output 0

### [15] Output X-Axis Vector

- `A` <= [16] Normalize output 0

### [16] Normalize

- `VectorInput` <= [11] Local Space to World Space TransformVector output 0

### [17] Output Object Scale

- `A` <= [19] ObjectScale output 0

### [18] Mask ( R )

- `Input` <= [7] Vertex Color output 4

### [20] Add

- `A` <= [1] Append output 0
- `B` <= [21] 0,-1,0 output 0

### [22] Output Black Mask

- `A` <= [27] If output 0

### [23] Add

- `A` <= [24] Vertex Color output 1
- `B` <= [24] Vertex Color output 2

### [25] Add

- `A` <= [23] Add output 0
- `B` <= [24] Vertex Color output 3

### [26] Add

- `A` <= [25] Add output 0
- `B` <= [24] Vertex Color output 4

### [27] If

- `A` <= [26] Add output 0
- `B` <= [28] 0 output 0
- `AGreaterThanB` <= [29] 1 output 0
- `AEqualsB` <= [28] 0 output 0
- `ALessThanB` <= [29] 1 output 0

## Connection List

- [0] TexCoord[2] output 0 -> Append.A
- [4] Mask ( R ) output 0 -> Append.B
- [20] Add output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [3] TexCoord[3] output 0 -> Mask ( R ).Input
- [2] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Pivot Position.A
- [18] Mask ( R ) output 0 -> Output Custom Alpha Values.A
- [9] TexCoord[3] output 0 -> Mask ( G ).Input
- [8] Mask ( G ) output 0 -> Output Random Value Per Element.A
- [12] Normalize output 0 -> Local Space to World Space TransformVector.Input
- [13] ConstantBiasScale output 0 -> Normalize.VectorInput
- [14] Vertex Color output 0 -> ConstantBiasScale.Input
- [16] Normalize output 0 -> Output X-Axis Vector.A
- [11] Local Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [19] ObjectScale output 0 -> Output Object Scale.A
- [7] Vertex Color output 4 -> Mask ( R ).Input
- [1] Append output 0 -> Add.A
- [21] 0,-1,0 output 0 -> Add.B
- [27] If output 0 -> Output Black Mask.A
- [24] Vertex Color output 1 -> Add.A
- [24] Vertex Color output 2 -> Add.B
- [23] Add output 0 -> Add.A
- [24] Vertex Color output 3 -> Add.B
- [25] Add output 0 -> Add.A
- [24] Vertex Color output 4 -> Add.B
- [26] Add output 0 -> If.A
- [28] 0 output 0 -> If.B
- [29] 1 output 0 -> If.AGreaterThanB
- [28] 0 output 0 -> If.AEqualsB
- [29] 1 output 0 -> If.ALessThanB

## Output Trace Roots

- `Pivot Position` is driven by [2] Instance & Particle Space to Absolute World Space TransformPosition
- `Custom Alpha Values` is driven by [18] Mask ( R )
- `Random Value Per Element` is driven by [8] Mask ( G )
- `X-Axis Vector` is driven by [16] Normalize
- `Object Scale` is driven by [19] ObjectScale
- `Black Mask` is driven by [27] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
