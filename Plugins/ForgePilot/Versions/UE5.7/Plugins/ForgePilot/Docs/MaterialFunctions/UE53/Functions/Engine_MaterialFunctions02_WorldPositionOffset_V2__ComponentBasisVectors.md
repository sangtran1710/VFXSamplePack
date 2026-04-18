# ComponentBasisVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/V2/ComponentBasisVectors.ComponentBasisVectors`
- Category: Engine_MaterialFunctions02/WorldPositionOffset/V2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The local XYZ component transform vectors in world space.

## Inputs

- None

## Outputs

- `X` - The local X vector transformed into world space.
- `Y` - The local Y vector transformed into world space.
- `Z` - The local Z vector transformed into world space.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output X | X |
| 1 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 4 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 5 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 6 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Y | Y |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Z | Z |

## Exact Input Wiring

### [0] Output X

- `A` <= [4] Instance & Particle Space to World Space TransformVector output 0

### [4] Instance & Particle Space to World Space TransformVector

- `Input` <= [1] 1,0,0 output 0

### [5] Instance & Particle Space to World Space TransformVector

- `Input` <= [2] 0,1,0 output 0

### [6] Instance & Particle Space to World Space TransformVector

- `Input` <= [3] 0,0,1 output 0

### [7] Output Y

- `A` <= [5] Instance & Particle Space to World Space TransformVector output 0

### [8] Output Z

- `A` <= [6] Instance & Particle Space to World Space TransformVector output 0

## Connection List

- [4] Instance & Particle Space to World Space TransformVector output 0 -> Output X.A
- [1] 1,0,0 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [2] 0,1,0 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [3] 0,0,1 output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [5] Instance & Particle Space to World Space TransformVector output 0 -> Output Y.A
- [6] Instance & Particle Space to World Space TransformVector output 0 -> Output Z.A

## Output Trace Roots

- `X` is driven by [4] Instance & Particle Space to World Space TransformVector
- `Y` is driven by [5] Instance & Particle Space to World Space TransformVector
- `Z` is driven by [6] Instance & Particle Space to World Space TransformVector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
