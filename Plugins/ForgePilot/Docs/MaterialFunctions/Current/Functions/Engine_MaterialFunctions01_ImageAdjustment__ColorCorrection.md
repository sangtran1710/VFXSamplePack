# ColorCorrection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/ColorCorrection.ColorCorrection`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Color correction

## Inputs

- `Color` [FunctionInput_Vector3] - Input color to apply correction
- `Gain` [FunctionInput_Vector3] - 3 Way Color Gain Control
- `Gamma` [FunctionInput_Vector3] - 3 Way Gamma Control
- `Lift` [FunctionInput_Vector3] - 3 Way Lift Control
- `Contrast` [FunctionInput_Scalar]
- `Saturation` [FunctionInput_Scalar]
- `Hue Shift` [FunctionInput_Scalar]
- `Brightness` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | HueShift |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | SCurve |  |
| 4 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 5 | expression | `MaterialExpressionConstant` | 0 |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Gain (Vector3) | Gain |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Gamma (Vector3) | Gamma |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Lift (Vector3) | Lift |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionPower` | Power |  |
| 16 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Saturation (Scalar) | Saturation |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Hue Shift (Scalar) | Hue Shift |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Brightness (Scalar) | Brightness |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [2] HueShift output 0
- `B` <= [20] Input Brightness (Scalar) output 0

### [4] Desaturation

- `Input` <= [3] SCurve output 0
- `Fraction` <= [6] Subtract output 0

### [6] Subtract

- `A` <= [5] 0 output 0
- `B` <= [18] Input Saturation (Scalar) output 0

### [11] 1-x

- `Input` <= [7] Input Color (Vector3) output 0

### [12] Multiply

- `A` <= [10] Input Lift (Vector3) output 0
- `B` <= [11] 1-x output 0

### [13] Add

- `A` <= [12] Multiply output 0
- `B` <= [7] Input Color (Vector3) output 0

### [14] Multiply

- `A` <= [13] Add output 0
- `B` <= [8] Input Gain (Vector3) output 0

### [15] Power

- `Base` <= [14] Multiply output 0
- `Exponent` <= [16] Divide(1,) output 0

### [16] Divide(1,)

- `B` <= [9] Input Gamma (Vector3) output 0

## Connection List

- [1] Multiply output 0 -> Output Result.A
- [2] HueShift output 0 -> Multiply.A
- [20] Input Brightness (Scalar) output 0 -> Multiply.B
- [3] SCurve output 0 -> Desaturation.Input
- [6] Subtract output 0 -> Desaturation.Fraction
- [5] 0 output 0 -> Subtract.A
- [18] Input Saturation (Scalar) output 0 -> Subtract.B
- [7] Input Color (Vector3) output 0 -> 1-x.Input
- [10] Input Lift (Vector3) output 0 -> Multiply.A
- [11] 1-x output 0 -> Multiply.B
- [12] Multiply output 0 -> Add.A
- [7] Input Color (Vector3) output 0 -> Add.B
- [13] Add output 0 -> Multiply.A
- [8] Input Gain (Vector3) output 0 -> Multiply.B
- [14] Multiply output 0 -> Power.Base
- [16] Divide(1,) output 0 -> Power.Exponent
- [9] Input Gamma (Vector3) output 0 -> Divide(1,).B

## Output Trace Roots

- `Result` is driven by [1] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


