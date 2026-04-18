# RadialGradientExponential

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/RadialGradientExponential.RadialGradientExponential`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Creates a radial gradient using the input UV coordinates and applies exponential density with specified density.

## Inputs

- `CenterPosition` [FunctionInput_Vector2] - Position in UV space
- `Radius` [FunctionInput_Scalar] - Radius in fraction of UV Space
- `UVs` [FunctionInput_Vector2] - Position in UV space
- `Density` [FunctionInput_Scalar]
- `Invert Density` [FunctionInput_StaticBool]

## Outputs

- `RadialGradientExponential`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RadialGradientExponential | RadialGradientExponential |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input CenterPosition (Vector2) | CenterPosition |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | ExponentialDensity |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Density (Scalar) | Density |
| 7 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 8 | expression | `MaterialExpressionDistance` | Distance |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 14 | expression | `MaterialExpressionDivide` | Divide |  |
| 15 | expression | `MaterialExpressionDivide` | Divide |  |
| 16 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 17 | expression | `MaterialExpressionClamp` | Clamp |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | ExponentialDensity |  |
| 19 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 20 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Invert Density (StaticBool) | Invert Density |
| 22 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 23 | expression | `MaterialExpressionDivide` | Divide(1,1) |  |

## Exact Input Wiring

### [0] Output RadialGradientExponential

- `A` <= [19] Switch output 0

### [5] Input UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [7] 1-x

- `Input` <= [4] ExponentialDensity output 0

### [8] Distance

- `A` <= [5] Input UVs (Vector2) output 0
- `B` <= [2] Input CenterPosition (Vector2) output 0

### [9] Multiply(,1)

- `A` <= [8] Distance output 0

### [10] Multiply

- `A` <= [3] Input Radius (Scalar) output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [11] Multiply

- `A` <= [9] Multiply(,1) output 0
- `B` <= [9] Multiply(,1) output 0

### [12] Subtract

- `A` <= [10] Multiply output 0
- `B` <= [11] Multiply output 0

### [13] Sqrt

- `Input` <= [12] Subtract output 0

### [14] Divide

- `A` <= [5] Input UVs (Vector2) output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [15] Divide

- `A` <= [9] Multiply(,1) output 0
- `B` <= [3] Input Radius (Scalar) output 0

### [16] 1-x

- `Input` <= [15] Divide output 0

### [17] Clamp

- `Input` <= [16] 1-x output 0

### [19] Switch

- `A` <= [4] ExponentialDensity output 0
- `B` <= [20] 1-x output 0
- `Value` <= [21] Input Invert Density (StaticBool) output 0

### [20] 1-x

- `Input` <= [18] ExponentialDensity output 0

### [21] Input Invert Density (StaticBool)

- `Preview` <= [22] Static Bool (False) output 0

## Connection List

- [19] Switch output 0 -> Output RadialGradientExponential.A
- [1] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [4] ExponentialDensity output 0 -> 1-x.Input
- [5] Input UVs (Vector2) output 0 -> Distance.A
- [2] Input CenterPosition (Vector2) output 0 -> Distance.B
- [8] Distance output 0 -> Multiply(,1).A
- [3] Input Radius (Scalar) output 0 -> Multiply.A
- [3] Input Radius (Scalar) output 0 -> Multiply.B
- [9] Multiply(,1) output 0 -> Multiply.A
- [9] Multiply(,1) output 0 -> Multiply.B
- [10] Multiply output 0 -> Subtract.A
- [11] Multiply output 0 -> Subtract.B
- [12] Subtract output 0 -> Sqrt.Input
- [5] Input UVs (Vector2) output 0 -> Divide.A
- [3] Input Radius (Scalar) output 0 -> Divide.B
- [9] Multiply(,1) output 0 -> Divide.A
- [3] Input Radius (Scalar) output 0 -> Divide.B
- [15] Divide output 0 -> 1-x.Input
- [16] 1-x output 0 -> Clamp.Input
- [4] ExponentialDensity output 0 -> Switch.A
- [20] 1-x output 0 -> Switch.B
- [21] Input Invert Density (StaticBool) output 0 -> Switch.Value
- [18] ExponentialDensity output 0 -> 1-x.Input
- [22] Static Bool (False) output 0 -> Input Invert Density (StaticBool).Preview

## Output Trace Roots

- `RadialGradientExponential` is driven by [19] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


