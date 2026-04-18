# SphereGradient-2D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/SphereGradient-2D.SphereGradient-2D`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Generates a gradient representing the depth of a shere centered on the desired point. 

## Inputs

- `CenterPosition` [FunctionInput_Vector2] - Position in UV space
- `Radius` [FunctionInput_Scalar] - Radius in fraction of UV Space
- `UVs` [FunctionInput_Vector2] - Position in UV space

## Outputs

- `Result 0-1`
- `Result Diameter`
- `Result Radius`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result 0-1 | Result 0-1 |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input CenterPosition (Vector2) | CenterPosition |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 5 | expression | `MaterialExpressionDistance` | Distance |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Result Diameter | Result Diameter |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Result Radius | Result Radius |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionDivide` | Divide |  |
| 18 | expression | `MaterialExpressionClamp` | Clamp |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 20 | expression | `MaterialExpressionMax` | Max(,0) |  |

## Exact Input Wiring

### [0] Output Result 0-1

- `A` <= [18] Clamp output 0

### [4] Input UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [5] Distance

- `A` <= [4] Input UVs (Vector2) output 0
- `B` <= [2] Input CenterPosition (Vector2) output 0

### [6] Multiply(,1)

- `A` <= [5] Distance output 0

### [7] Multiply

- `A` <= [3] Input Radius (Scalar) output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [8] Multiply

- `A` <= [11] Clamp output 0
- `B` <= [11] Clamp output 0

### [9] Subtract

- `A` <= [7] Multiply output 0
- `B` <= [8] Multiply output 0

### [10] Sqrt

- `Input` <= [20] Max(,0) output 0

### [11] Clamp

- `Input` <= [6] Multiply(,1) output 0
- `Max` <= [3] Input Radius (Scalar) output 0

### [12] Output Result Diameter

- `A` <= [13] Multiply(,2) output 0

### [13] Multiply(,2)

- `A` <= [10] Sqrt output 0

### [14] Multiply(,2)

- `A` <= [3] Input Radius (Scalar) output 0

### [15] Output Result Radius

- `A` <= [19] Multiply output 0

### [16] Multiply

- `A` <= [10] Sqrt output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [17] Divide

- `A` <= [10] Sqrt output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [18] Clamp

- `Input` <= [17] Divide output 0

### [19] Multiply

- `A` <= [18] Clamp output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [20] Max(,0)

- `A` <= [9] Subtract output 0

## Connection List

- [18] Clamp output 0 -> Output Result 0-1.A
- [1] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [4] Input UVs (Vector2) output 0 -> Distance.A
- [2] Input CenterPosition (Vector2) output 0 -> Distance.B
- [5] Distance output 0 -> Multiply(,1).A
- [3] Input Radius (Scalar) output 0 -> Multiply.A
- [3] Input Radius (Scalar) output 0 -> Multiply.B
- [11] Clamp output 0 -> Multiply.A
- [11] Clamp output 0 -> Multiply.B
- [7] Multiply output 0 -> Subtract.A
- [8] Multiply output 0 -> Subtract.B
- [20] Max(,0) output 0 -> Sqrt.Input
- [6] Multiply(,1) output 0 -> Clamp.Input
- [3] Input Radius (Scalar) output 0 -> Clamp.Max
- [13] Multiply(,2) output 0 -> Output Result Diameter.A
- [10] Sqrt output 0 -> Multiply(,2).A
- [3] Input Radius (Scalar) output 0 -> Multiply(,2).A
- [19] Multiply output 0 -> Output Result Radius.A
- [10] Sqrt output 0 -> Multiply.A
- [3] Input Radius (Scalar) output 0 -> Multiply.B
- [10] Sqrt output 0 -> Divide.A
- [3] Input Radius (Scalar) output 0 -> Divide.B
- [17] Divide output 0 -> Clamp.Input
- [18] Clamp output 0 -> Multiply.A
- [3] Input Radius (Scalar) output 0 -> Multiply.B
- [9] Subtract output 0 -> Max(,0).A

## Output Trace Roots

- `Result 0-1` is driven by [18] Clamp
- `Result Diameter` is driven by [13] Multiply(,2)
- `Result Radius` is driven by [19] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


