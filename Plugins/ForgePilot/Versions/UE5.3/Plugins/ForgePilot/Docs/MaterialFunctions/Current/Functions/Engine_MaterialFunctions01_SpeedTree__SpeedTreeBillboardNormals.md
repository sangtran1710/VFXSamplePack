# SpeedTreeBillboardNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions01/SpeedTree/SpeedTreeBillboardNormals.SpeedTreeBillboardNormals`
- Category: Engine_MaterialFunctions01/SpeedTree
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Move normals into screen space for smoother billboard lighting

## Inputs

- `Normal` [FunctionInput_Vector3] - Tangent space normal map

## Outputs

- `NormalWS`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output NormalWS | NormalWS |
| 1 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 3 | expression | `MaterialExpressionAppendVector` | Append |  |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 9 | expression | `MaterialExpressionWorldPosition` | Camera Relative World Position |  |
| 10 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output NormalWS

- `A` <= [6] Add output 0

### [1] Tangent Space to World Space TransformVector

- `Input` <= [3] Append output 0

### [2] Mask ( R G )

- `Input` <= [7] Input Normal (Vector3) output 0

### [3] Append

- `A` <= [2] Mask ( R G ) output 0
- `B` <= [4] 0 output 0

### [5] Multiply

- `A` <= [11] Multiply(,-1) output 0
- `B` <= [10] Normalize output 0

### [6] Add

- `A` <= [1] Tangent Space to World Space TransformVector output 0
- `B` <= [5] Multiply output 0

### [8] Mask ( B )

- `Input` <= [7] Input Normal (Vector3) output 0

### [10] Normalize

- `VectorInput` <= [9] Camera Relative World Position output 0

### [11] Multiply(,-1)

- `A` <= [8] Mask ( B ) output 0

## Connection List

- [6] Add output 0 -> Output NormalWS.A
- [3] Append output 0 -> Tangent Space to World Space TransformVector.Input
- [7] Input Normal (Vector3) output 0 -> Mask ( R G ).Input
- [2] Mask ( R G ) output 0 -> Append.A
- [4] 0 output 0 -> Append.B
- [11] Multiply(,-1) output 0 -> Multiply.A
- [10] Normalize output 0 -> Multiply.B
- [1] Tangent Space to World Space TransformVector output 0 -> Add.A
- [5] Multiply output 0 -> Add.B
- [7] Input Normal (Vector3) output 0 -> Mask ( B ).Input
- [9] Camera Relative World Position output 0 -> Normalize.VectorInput
- [8] Mask ( B ) output 0 -> Multiply(,-1).A

## Output Trace Roots

- `NormalWS` is driven by [6] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


