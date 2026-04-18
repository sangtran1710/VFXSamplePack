# Henyey-Greenstein-Phase

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Volumetrics/Henyey-Greenstein-Phase.Henyey-Greenstein-Phase`
- Category: Engine_MaterialFunctions01/Volumetrics
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `AngleDegrees` [FunctionInput_Scalar] - By default this input is not used and the angle is calculated using the input Light Vector. If this input is specified, light vector will be disregarded and the input angle will be used. In Degrees.
- `Anisotropy` [FunctionInput_Scalar] - 0 equals isotropic or equal scattering in all directions. 1 means full forward scattering.
- `Light Vector` [FunctionInput_Vector3] - The light vector to calculate the viewing angle from. Note that if AngleDegrees is specified, this input will be ignored.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input AngleDegrees (Scalar) | AngleDegrees |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Anisotropy (Scalar) | Anisotropy |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 5 | expression | `MaterialExpressionConstant` | 4 |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionPower` | Power |  |
| 13 | expression | `MaterialExpressionDivide` | Divide(3,2) |  |
| 14 | expression | `MaterialExpressionCosine` | Cosine |  |
| 15 | expression | `MaterialExpressionDivide` | Divide |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply(,0.01745) |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Light Vector (Vector3) | Light Vector |
| 18 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 19 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 20 | custom | `MaterialExpressionCustom` | acos(x) |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [15] Divide output 0

### [1] Input AngleDegrees (Scalar)

- `Preview` <= [21] Multiply(,57.3) output 0

### [2] 1-x

- `Input` <= [6] Multiply output 0

### [6] Multiply

- `A` <= [3] Input Anisotropy (Scalar) output 0
- `B` <= [3] Input Anisotropy (Scalar) output 0

### [7] Divide

- `A` <= [2] 1-x output 0
- `B` <= [12] Power output 0

### [8] Add(,1)

- `A` <= [6] Multiply output 0

### [9] Subtract

- `A` <= [8] Add(,1) output 0
- `B` <= [11] Multiply output 0

### [10] Multiply(,2)

- `A` <= [3] Input Anisotropy (Scalar) output 0

### [11] Multiply

- `A` <= [10] Multiply(,2) output 0
- `B` <= [14] Cosine output 0

### [12] Power

- `Base` <= [9] Subtract output 0
- `Exponent` <= [13] Divide(3,2) output 0

### [14] Cosine

- `Input` <= [16] Multiply(,0.01745) output 0

### [15] Divide

- `A` <= [7] Divide output 0
- `B` <= [4] Pi output 0

### [16] Multiply(,0.01745)

- `A` <= [1] Input AngleDegrees (Scalar) output 0

### [19] Dot

- `A` <= [17] Input Light Vector (Vector3) output 0
- `B` <= [18] Camera Vector output 0

### [21] Multiply(,57.3)

- `A` <= [20] acos(x) output 0

## Connection List

- [15] Divide output 0 -> Output Result.A
- [21] Multiply(,57.3) output 0 -> Input AngleDegrees (Scalar).Preview
- [6] Multiply output 0 -> 1-x.Input
- [3] Input Anisotropy (Scalar) output 0 -> Multiply.A
- [3] Input Anisotropy (Scalar) output 0 -> Multiply.B
- [2] 1-x output 0 -> Divide.A
- [12] Power output 0 -> Divide.B
- [6] Multiply output 0 -> Add(,1).A
- [8] Add(,1) output 0 -> Subtract.A
- [11] Multiply output 0 -> Subtract.B
- [3] Input Anisotropy (Scalar) output 0 -> Multiply(,2).A
- [10] Multiply(,2) output 0 -> Multiply.A
- [14] Cosine output 0 -> Multiply.B
- [9] Subtract output 0 -> Power.Base
- [13] Divide(3,2) output 0 -> Power.Exponent
- [16] Multiply(,0.01745) output 0 -> Cosine.Input
- [7] Divide output 0 -> Divide.A
- [4] Pi output 0 -> Divide.B
- [1] Input AngleDegrees (Scalar) output 0 -> Multiply(,0.01745).A
- [17] Input Light Vector (Vector3) output 0 -> Dot.A
- [18] Camera Vector output 0 -> Dot.B
- [20] acos(x) output 0 -> Multiply(,57.3).A

## Output Trace Roots

- `Result` is driven by [15] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
