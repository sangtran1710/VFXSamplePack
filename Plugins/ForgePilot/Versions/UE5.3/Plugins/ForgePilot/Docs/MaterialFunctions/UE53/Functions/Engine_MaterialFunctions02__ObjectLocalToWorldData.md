# ObjectLocalToWorldData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ObjectLocalToWorldData.ObjectLocalToWorldData`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns object level information.

## Inputs

- `Local Vector` [FunctionInput_Vector3] - Input a vector 3 value to be transformed by the objects rotation. For example, a value of (1,0,0) will return a world space vector along the objects x-axis regardless of object orientation.

## Outputs

- `Object Scale` - Returns a scalar value that is equal to the objects uniform scale.
- `World Vector` - Local vector input transformed into world space.
- `Object Pivot Location` - Object pivot point in world space.
- `Object Scale XYZ` - Returns a vector value that is equal to the objects scale on the x,y and z axes.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Object Scale | Object Scale |
| 1 | expression | `MaterialExpressionDistance` | Distance |  |
| 2 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output World Vector | World Vector |
| 4 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Local Vector (Vector3) | Local Vector |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Object Pivot Location | Object Pivot Location |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Object Scale XYZ | Object Scale XYZ |
| 9 | expression | `MaterialExpressionConstant3Vector` | 0.58,0.58,0.58 |  |
| 10 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 11 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 12 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 13 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 14 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 15 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Object Scale

- `A` <= [1] Distance output 0

### [1] Distance

- `A` <= [2] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `B` <= [12] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [2] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [15] Normalize output 0

### [3] Output World Vector

- `A` <= [4] Tangent Space to World Space TransformVector output 0

### [4] Tangent Space to World Space TransformVector

- `Input` <= [5] Input Local Vector (Vector3) output 0

### [6] Output Object Pivot Location

- `A` <= [12] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [7] Subtract

- `A` <= [11] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `B` <= [12] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [8] Output Object Scale XYZ

- `A` <= [7] Subtract output 0

### [11] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [10] 1,1,1 output 0

### [12] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [13] 0,0,0 output 0

### [15] Normalize

- `VectorInput` <= [14] 1,1,1 output 0

## Connection List

- [1] Distance output 0 -> Output Object Scale.A
- [2] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Distance.A
- [12] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Distance.B
- [15] Normalize output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [4] Tangent Space to World Space TransformVector output 0 -> Output World Vector.A
- [5] Input Local Vector (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [12] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Object Pivot Location.A
- [11] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Subtract.A
- [12] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Subtract.B
- [7] Subtract output 0 -> Output Object Scale XYZ.A
- [10] 1,1,1 output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [13] 0,0,0 output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [14] 1,1,1 output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Object Scale` is driven by [1] Distance
- `World Vector` is driven by [4] Tangent Space to World Space TransformVector
- `Object Pivot Location` is driven by [12] Instance & Particle Space to Absolute World Space TransformPosition
- `Object Scale XYZ` is driven by [7] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
