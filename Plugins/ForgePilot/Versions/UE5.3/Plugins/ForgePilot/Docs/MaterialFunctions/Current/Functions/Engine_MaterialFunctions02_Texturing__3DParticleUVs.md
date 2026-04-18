# 3DParticleUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/3DParticleUVs.3DParticleUVs`
- Category: Engine_MaterialFunctions02/Texturing
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Moves through a flipbook texture based on the cameras position compared to the object center.

## Inputs

- `Number of horizontal Images` [FunctionInput_Scalar]
- `UVs` [FunctionInput_Vector2] - Default Value = UV coordinate 0
- `Number of texture rotations per world rotation` [FunctionInput_Scalar]

## Outputs

- `Uvs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Uvs | Uvs |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorToRadialValue |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionFloor` | Floor |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 11 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Number of horizontal Images (Scalar) | Number of horizontal Images |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 16 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 17 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Number of texture rotations per world rotation (Scalar) | Number of texture rotations per world rotation |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Uvs

- `A` <= [12] Append output 0

### [2] Divide

- `A` <= [6] Floor output 0
- `B` <= [14] Input Number of horizontal Images (Scalar) output 0

### [3] Mask ( R G )

- `Input` <= [4] Normalize output 0

### [4] Normalize

- `VectorInput` <= [5] Subtract output 0

### [5] Subtract

- `A` <= [16] Camera Position output 0
- `B` <= [17] Particle Position (Absolute) output 0

### [6] Floor

- `Input` <= [7] Multiply output 0

### [7] Multiply

- `A` <= [1] VectorToRadialValue output 1
- `B` <= [19] Multiply output 0

### [8] Add

- `A` <= [2] Divide output 0
- `B` <= [9] Divide output 0

### [9] Divide

- `A` <= [10] Mask ( R ) output 0
- `B` <= [14] Input Number of horizontal Images (Scalar) output 0

### [10] Mask ( R )

- `Input` <= [15] Input UVs (Vector2) output 0

### [12] Append

- `A` <= [8] Add output 0
- `B` <= [13] Mask ( G ) output 0

### [13] Mask ( G )

- `Input` <= [15] Input UVs (Vector2) output 0

### [15] Input UVs (Vector2)

- `Preview` <= [11] TexCoord[0] output 0

### [19] Multiply

- `A` <= [18] Input Number of texture rotations per world rotation (Scalar) output 0
- `B` <= [14] Input Number of horizontal Images (Scalar) output 0

## Connection List

- [12] Append output 0 -> Output Uvs.A
- [6] Floor output 0 -> Divide.A
- [14] Input Number of horizontal Images (Scalar) output 0 -> Divide.B
- [4] Normalize output 0 -> Mask ( R G ).Input
- [5] Subtract output 0 -> Normalize.VectorInput
- [16] Camera Position output 0 -> Subtract.A
- [17] Particle Position (Absolute) output 0 -> Subtract.B
- [7] Multiply output 0 -> Floor.Input
- [1] VectorToRadialValue output 1 -> Multiply.A
- [19] Multiply output 0 -> Multiply.B
- [2] Divide output 0 -> Add.A
- [9] Divide output 0 -> Add.B
- [10] Mask ( R ) output 0 -> Divide.A
- [14] Input Number of horizontal Images (Scalar) output 0 -> Divide.B
- [15] Input UVs (Vector2) output 0 -> Mask ( R ).Input
- [8] Add output 0 -> Append.A
- [13] Mask ( G ) output 0 -> Append.B
- [15] Input UVs (Vector2) output 0 -> Mask ( G ).Input
- [11] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [18] Input Number of texture rotations per world rotation (Scalar) output 0 -> Multiply.A
- [14] Input Number of horizontal Images (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Uvs` is driven by [12] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


