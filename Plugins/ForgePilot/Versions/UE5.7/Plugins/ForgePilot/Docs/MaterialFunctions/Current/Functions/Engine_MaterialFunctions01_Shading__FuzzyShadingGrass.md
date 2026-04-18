# FuzzyShadingGrass

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/FuzzyShadingGrass.FuzzyShadingGrass`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function is the diffuse portion of grass shading.

## Inputs

- `Diffuse` [FunctionInput_Vector3]
- `CoreDarkness` [FunctionInput_Scalar]
- `Normal` [FunctionInput_Vector3]
- `EdgeBrightness` [FunctionInput_Scalar]
- `Power` [FunctionInput_Scalar]
- `EdgeDesat` [FunctionInput_Scalar]
- `EdgeColor` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse (Vector3) | Diffuse |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionConstant` | 0 |  |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input CoreDarkness (Scalar) | CoreDarkness |
| 9 | expression | `MaterialExpressionClamp` | Clamp |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | expression | `MaterialExpressionTwoSidedSign` | TwoSidedSign |  |
| 15 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 17 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input EdgeBrightness (Scalar) | EdgeBrightness |
| 20 | expression | `MaterialExpressionPower` | Power |  |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Power (Scalar) | Power |
| 22 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 23 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 24 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 25 | expression | `MaterialExpressionConstant` | 0 |  |
| 26 | expression | `MaterialExpressionClamp` | Clamp |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input EdgeDesat (Scalar) | EdgeDesat |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input EdgeColor (Vector3) | EdgeColor |

## Exact Input Wiring

### [0] Output Result

- `A` <= [23] Lerp output 0

### [1] Multiply

- `A` <= [4] Add output 0
- `B` <= [2] Input Diffuse (Vector3) output 0

### [3] Add

- `A` <= [18] Multiply output 0
- `B` <= [4] Add output 0

### [4] Add

- `A` <= [6] 1-x output 0
- `B` <= [5] 0 output 0

### [6] 1-x

- `Input` <= [7] Multiply output 0

### [7] Multiply

- `A` <= [9] Clamp output 0
- `B` <= [8] Input CoreDarkness (Scalar) output 0

### [9] Clamp

- `Input` <= [10] Dot output 0

### [10] Dot

- `A` <= [17] Camera Vector output 0
- `B` <= [11] Tangent Space to World Space TransformVector output 0

### [11] Tangent Space to World Space TransformVector

- `Input` <= [12] Multiply output 0

### [12] Multiply

- `A` <= [16] Input Normal (Vector3) output 0
- `B` <= [13] Append output 0

### [13] Append

- `A` <= [15] 1,1 output 0
- `B` <= [14] TwoSidedSign output 0

### [18] Multiply

- `A` <= [20] Power output 0
- `B` <= [19] Input EdgeBrightness (Scalar) output 0

### [20] Power

- `Base` <= [22] 1-x output 0
- `Exponent` <= [21] Input Power (Scalar) output 0

### [22] 1-x

- `Input` <= [9] Clamp output 0

### [23] Lerp

- `A` <= [1] Multiply output 0
- `B` <= [28] Multiply output 0
- `Alpha` <= [26] Clamp output 0

### [24] Desaturation

- `Input` <= [2] Input Diffuse (Vector3) output 0
- `Fraction` <= [27] Input EdgeDesat (Scalar) output 0

### [26] Clamp

- `Input` <= [18] Multiply output 0

### [28] Multiply

- `A` <= [24] Desaturation output 0
- `B` <= [29] Input EdgeColor (Vector3) output 0

## Connection List

- [23] Lerp output 0 -> Output Result.A
- [4] Add output 0 -> Multiply.A
- [2] Input Diffuse (Vector3) output 0 -> Multiply.B
- [18] Multiply output 0 -> Add.A
- [4] Add output 0 -> Add.B
- [6] 1-x output 0 -> Add.A
- [5] 0 output 0 -> Add.B
- [7] Multiply output 0 -> 1-x.Input
- [9] Clamp output 0 -> Multiply.A
- [8] Input CoreDarkness (Scalar) output 0 -> Multiply.B
- [10] Dot output 0 -> Clamp.Input
- [17] Camera Vector output 0 -> Dot.A
- [11] Tangent Space to World Space TransformVector output 0 -> Dot.B
- [12] Multiply output 0 -> Tangent Space to World Space TransformVector.Input
- [16] Input Normal (Vector3) output 0 -> Multiply.A
- [13] Append output 0 -> Multiply.B
- [15] 1,1 output 0 -> Append.A
- [14] TwoSidedSign output 0 -> Append.B
- [20] Power output 0 -> Multiply.A
- [19] Input EdgeBrightness (Scalar) output 0 -> Multiply.B
- [22] 1-x output 0 -> Power.Base
- [21] Input Power (Scalar) output 0 -> Power.Exponent
- [9] Clamp output 0 -> 1-x.Input
- [1] Multiply output 0 -> Lerp.A
- [28] Multiply output 0 -> Lerp.B
- [26] Clamp output 0 -> Lerp.Alpha
- [2] Input Diffuse (Vector3) output 0 -> Desaturation.Input
- [27] Input EdgeDesat (Scalar) output 0 -> Desaturation.Fraction
- [18] Multiply output 0 -> Clamp.Input
- [24] Desaturation output 0 -> Multiply.A
- [29] Input EdgeColor (Vector3) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [23] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


