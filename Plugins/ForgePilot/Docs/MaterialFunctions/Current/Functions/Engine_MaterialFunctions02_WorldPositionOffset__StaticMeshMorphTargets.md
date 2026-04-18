# StaticMeshMorphTargets

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/StaticMeshMorphTargets.StaticMeshMorphTargets`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Unpacks Morph Data From the 3ds Max Morph Packer Script

## Inputs

- None

## Outputs

- `Morph Target 1 WPO`
- `Morph Target 2 WPO`
- `Morph Target 1 Normals`
- `---------------`
- `Pivot Position WS` - Returns the pivot point information for each element in world space if that option was chosen in the script at the time the model was processed. The output of this pin is in world space.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Morph Target 1 WPO | Morph Target 1 WPO |
| 1 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 2 | expression | `MaterialExpressionTransform` | Instance & Particle Space to Tangent Space TransformVector |  |
| 3 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 7 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 12 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 13 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Morph Target 2 WPO | Morph Target 2 WPO |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Morph Target 1 Normals | Morph Target 1 Normals |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output --------------- | --------------- |
| 17 | expression | `MaterialExpressionAppendVector` | Append |  |
| 18 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 19 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 21 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Pivot Position WS | Pivot Position WS |

## Exact Input Wiring

### [0] Output Morph Target 1 WPO

- `A` <= [12] Instance & Particle Space to World Space TransformVector output 0

### [2] Instance & Particle Space to Tangent Space TransformVector

- `Input` <= [3] ConstantBiasScale output 0

### [3] ConstantBiasScale

- `Input` <= [1] Vertex Color output 0

### [5] Append

- `A` <= [4] TexCoord[1] output 0
- `B` <= [8] Mask ( R ) output 0

### [6] Instance & Particle Space to World Space TransformVector

- `Input` <= [5] Append output 0

### [8] Mask ( R )

- `Input` <= [7] TexCoord[2] output 0

### [10] Append

- `A` <= [11] Mask ( G ) output 0
- `B` <= [13] TexCoord[3] output 0

### [11] Mask ( G )

- `Input` <= [9] TexCoord[2] output 0

### [12] Instance & Particle Space to World Space TransformVector

- `Input` <= [10] Append output 0

### [14] Output Morph Target 2 WPO

- `A` <= [6] Instance & Particle Space to World Space TransformVector output 0

### [15] Output Morph Target 1 Normals

- `A` <= [2] Instance & Particle Space to Tangent Space TransformVector output 0

### [17] Append

- `A` <= [18] TexCoord[1] output 0
- `B` <= [20] Mask ( R ) output 0

### [19] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [17] Append output 0

### [20] Mask ( R )

- `Input` <= [21] TexCoord[2] output 0

### [22] Output Pivot Position WS

- `A` <= [19] Instance & Particle Space to Absolute World Space TransformPosition output 0

## Connection List

- [12] Instance & Particle Space to World Space TransformVector output 0 -> Output Morph Target 1 WPO.A
- [3] ConstantBiasScale output 0 -> Instance & Particle Space to Tangent Space TransformVector.Input
- [1] Vertex Color output 0 -> ConstantBiasScale.Input
- [4] TexCoord[1] output 0 -> Append.A
- [8] Mask ( R ) output 0 -> Append.B
- [5] Append output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [7] TexCoord[2] output 0 -> Mask ( R ).Input
- [11] Mask ( G ) output 0 -> Append.A
- [13] TexCoord[3] output 0 -> Append.B
- [9] TexCoord[2] output 0 -> Mask ( G ).Input
- [10] Append output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [6] Instance & Particle Space to World Space TransformVector output 0 -> Output Morph Target 2 WPO.A
- [2] Instance & Particle Space to Tangent Space TransformVector output 0 -> Output Morph Target 1 Normals.A
- [18] TexCoord[1] output 0 -> Append.A
- [20] Mask ( R ) output 0 -> Append.B
- [17] Append output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [21] TexCoord[2] output 0 -> Mask ( R ).Input
- [19] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Pivot Position WS.A

## Output Trace Roots

- `Morph Target 1 WPO` is driven by [12] Instance & Particle Space to World Space TransformVector
- `Morph Target 2 WPO` is driven by [6] Instance & Particle Space to World Space TransformVector
- `Morph Target 1 Normals` is driven by [2] Instance & Particle Space to Tangent Space TransformVector
- `---------------` has no captured source.
- `Pivot Position WS` is driven by [19] Instance & Particle Space to Absolute World Space TransformPosition

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


