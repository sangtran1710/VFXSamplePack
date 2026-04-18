# TangentBasis

- Path: `/Engine/Functions/Engine_MaterialFunctions02/TangentBasis.TangentBasis`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Retrieves the tangent basis in world space

## Inputs

- None

## Outputs

- `X U`
- `Y V`
- `Z W`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output X U | X U |
| 1 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 4 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 5 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Y V | Y V |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Z W | Z W |
| 8 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 11 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output X U

- `A` <= [9] Normalize output 0

### [4] Tangent Space to World Space TransformVector

- `Input` <= [1] 1,0,0 output 0

### [5] Tangent Space to World Space TransformVector

- `Input` <= [2] 0,1,0 output 0

### [6] Output Y V

- `A` <= [10] Normalize output 0

### [7] Output Z W

- `A` <= [11] Normalize output 0

### [8] Tangent Space to World Space TransformVector

- `Input` <= [3] 0,0,1 output 0

### [9] Normalize

- `VectorInput` <= [4] Tangent Space to World Space TransformVector output 0

### [10] Normalize

- `VectorInput` <= [5] Tangent Space to World Space TransformVector output 0

### [11] Normalize

- `VectorInput` <= [8] Tangent Space to World Space TransformVector output 0

## Connection List

- [9] Normalize output 0 -> Output X U.A
- [1] 1,0,0 output 0 -> Tangent Space to World Space TransformVector.Input
- [2] 0,1,0 output 0 -> Tangent Space to World Space TransformVector.Input
- [10] Normalize output 0 -> Output Y V.A
- [11] Normalize output 0 -> Output Z W.A
- [3] 0,0,1 output 0 -> Tangent Space to World Space TransformVector.Input
- [4] Tangent Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [5] Tangent Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [8] Tangent Space to World Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `X U` is driven by [9] Normalize
- `Y V` is driven by [10] Normalize
- `Z W` is driven by [11] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


