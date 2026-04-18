# ConstantScalebyDistance

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/ConstantScalebyDistance.ConstantScalebyDistance`
- Category: Engine_MaterialFunctions01/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Keeps a quad a constant scale on the screen, falls apart at close distances.

## Inputs

- `SizeScale` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionAdd` | Add(,-0.5) |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 13 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 14 | expression | `MaterialExpressionDistance` | Distance |  |
| 15 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input SizeScale (Scalar) | SizeScale |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [9] Multiply output 0

### [2] Mask ( R )

- `Input` <= [6] Multiply(,2) output 0

### [3] Add(,-0.5)

- `A` <= [4] TexCoord[0] output 0

### [5] Multiply

- `A` <= [1] 1,0,0 output 0
- `B` <= [2] Mask ( R ) output 0

### [6] Multiply(,2)

- `A` <= [3] Add(,-0.5) output 0

### [7] Add

- `A` <= [5] Multiply output 0
- `B` <= [11] Multiply output 0

### [8] Tangent Space to World Space TransformVector

- `Input` <= [7] Add output 0

### [9] Multiply

- `A` <= [8] Tangent Space to World Space TransformVector output 0
- `B` <= [17] Multiply output 0

### [10] Mask ( G )

- `Input` <= [6] Multiply(,2) output 0

### [11] Multiply

- `A` <= [10] Mask ( G ) output 0
- `B` <= [12] 0,1,0 output 0

### [14] Distance

- `A` <= [13] Absolute World Position output 0
- `B` <= [15] Camera Position output 0

### [17] Multiply

- `A` <= [14] Distance output 0
- `B` <= [16] Input SizeScale (Scalar) output 0

## Connection List

- [9] Multiply output 0 -> Output Result.A
- [6] Multiply(,2) output 0 -> Mask ( R ).Input
- [4] TexCoord[0] output 0 -> Add(,-0.5).A
- [1] 1,0,0 output 0 -> Multiply.A
- [2] Mask ( R ) output 0 -> Multiply.B
- [3] Add(,-0.5) output 0 -> Multiply(,2).A
- [5] Multiply output 0 -> Add.A
- [11] Multiply output 0 -> Add.B
- [7] Add output 0 -> Tangent Space to World Space TransformVector.Input
- [8] Tangent Space to World Space TransformVector output 0 -> Multiply.A
- [17] Multiply output 0 -> Multiply.B
- [6] Multiply(,2) output 0 -> Mask ( G ).Input
- [10] Mask ( G ) output 0 -> Multiply.A
- [12] 0,1,0 output 0 -> Multiply.B
- [13] Absolute World Position output 0 -> Distance.A
- [15] Camera Position output 0 -> Distance.B
- [14] Distance output 0 -> Multiply.A
- [16] Input SizeScale (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [9] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
