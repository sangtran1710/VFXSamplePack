# ObjectScale

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ObjectScale.ObjectScale`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the object's XYZ scale together and seperately.This is not compatible with the pixel shader.

## Inputs

- None

## Outputs

- `Scale Z` - Returns a scalar value that is equal to the object's Z-scale. This is not compatible with the pixel shader.
- `Scale X` - Returns a scalar value that is equal to the object's X-scale. This is not compatible with the pixel shader.
- `Scale Y` - Returns a scalar value that is equal to the object's Y-scale. This is not compatible with the pixel shader.
- `Scale XYZ` - Returns a float3 value that is equal to the objects XYZ scale. This is not compatible with the pixel shader.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Scale Z | Scale Z |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Scale X | Scale X |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output Scale Y | Scale Y |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output Scale XYZ | Scale XYZ |
| 4 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 5 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 6 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 7 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 8 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 9 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output Scale Z

- `A` <= [12] VectorLength output 0

### [1] Output Scale X

- `A` <= [10] VectorLength output 0

### [2] Output Scale Y

- `A` <= [11] VectorLength output 0

### [3] Output Scale XYZ

- `A` <= [14] Append output 0

### [7] Instance & Particle Space to World Space TransformVector

- `Input` <= [4] 1,0,0 output 0

### [8] Instance & Particle Space to World Space TransformVector

- `Input` <= [5] 0,1,0 output 0

### [9] Instance & Particle Space to World Space TransformVector

- `Input` <= [6] 0,0,1 output 0

### [13] Append

- `A` <= [10] VectorLength output 0
- `B` <= [11] VectorLength output 0

### [14] Append

- `A` <= [13] Append output 0
- `B` <= [12] VectorLength output 0

## Connection List

- [12] VectorLength output 0 -> Output Scale Z.A
- [10] VectorLength output 0 -> Output Scale X.A
- [11] VectorLength output 0 -> Output Scale Y.A
- [14] Append output 0 -> Output Scale XYZ.A
- [4] 1,0,0 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [5] 0,1,0 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [6] 0,0,1 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [10] VectorLength output 0 -> Append.A
- [11] VectorLength output 0 -> Append.B
- [13] Append output 0 -> Append.A
- [12] VectorLength output 0 -> Append.B

## Output Trace Roots

- `Scale Z` is driven by [12] VectorLength
- `Scale X` is driven by [10] VectorLength
- `Scale Y` is driven by [11] VectorLength
- `Scale XYZ` is driven by [14] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
