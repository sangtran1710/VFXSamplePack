# DeriveHDRfromLDR

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/DeriveHDRfromLDR.DeriveHDRfromLDR`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Derive an HDR range from an LDR texture while maintaining chroma and minimizing artifacts

## Inputs

- `LDR Input` [FunctionInput_Vector3] - Input an LDR texture or a material chain resulting in a Vector3.
- `Derive Luminance from Linear` [FunctionInput_StaticBool] - This can create a softer result by deriving the Luminance from the input in Linear space instead of SRGB
- `Desaturate Luminance` [FunctionInput_Scalar] - Lower this value to derive more of the HDR values from the base input rather than its' Luminance. Artistic parameter, should probably stay around 1.
- `Derive HDR Power` [FunctionInput_Scalar] - Power function applied to the Luminance which is used to derive the HDR range
- `HDR Dynamic Range` [FunctionInput_Scalar] - The dynamic range of the HDR values derived from your LDR Input
- `HDR Tint` [FunctionInput_Vector3] - Tint only the HDR range of the output
- `HDR Intensity Multiplier` [FunctionInput_Scalar] - A multiplier against the overall intensity of the HDR range

## Outputs

- `RemappedRange`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RemappedRange | RemappedRange |
| 1 | expression | `MaterialExpressionDivide` | Divide |  |
| 2 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract(1,) |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionPower` | Power |  |
| 9 | expression | `MaterialExpressionPower` | Power(X, 0.4545) |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 11 | expression | `MaterialExpressionConstant` | 1 |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input LDR Input (Vector3) | LDR Input |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Derive Luminance from Linear (StaticBool) | Derive Luminance from Linear |
| 15 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 16 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Luminance from Linear' |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Desaturate Luminance (Scalar) | Desaturate Luminance |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Derive HDR Power (Scalar) | Derive HDR Power |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input HDR Dynamic Range (Scalar) | HDR Dynamic Range |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input HDR Tint (Vector3) | HDR Tint |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input HDR Intensity Multiplier (Scalar) | HDR Intensity Multiplier |

## Exact Input Wiring

### [0] Output RemappedRange

- `A` <= [1] Divide output 0

### [1] Divide

- `A` <= [3] Multiply output 0
- `B` <= [7] Divide output 0

### [2] Desaturation

- `Input` <= [15] Switch output 0
- `Fraction` <= [17] Input Desaturate Luminance (Scalar) output 0

### [3] Multiply

- `A` <= [13] Input LDR Input (Vector3) output 0
- `B` <= [19] Input HDR Dynamic Range (Scalar) output 0

### [4] Multiply

- `A` <= [19] Input HDR Dynamic Range (Scalar) output 0
- `B` <= [8] Power output 0

### [5] Add

- `A` <= [19] Input HDR Dynamic Range (Scalar) output 0
- `B` <= [6] Subtract(1,) output 0

### [6] Subtract(1,)

- `B` <= [4] Multiply output 0

### [7] Divide

- `A` <= [5] Add output 0
- `B` <= [12] Multiply output 0

### [8] Power

- `Base` <= [2] Desaturation output 0
- `Exponent` <= [18] Input Derive HDR Power (Scalar) output 0

### [9] Power(X, 0.4545)

- `Base` <= [13] Input LDR Input (Vector3) output 0

### [10] Lerp

- `A` <= [11] 1 output 0
- `B` <= [20] Input HDR Tint (Vector3) output 0
- `Alpha` <= [8] Power output 0

### [12] Multiply

- `A` <= [21] Input HDR Intensity Multiplier (Scalar) output 0
- `B` <= [10] Lerp output 0

### [14] Input Derive Luminance from Linear (StaticBool)

- `Preview` <= [16] Static Bool Param (False) 'Luminance from Linear' output 0

### [15] Switch

- `A` <= [9] Power(X, 0.4545) output 0
- `B` <= [13] Input LDR Input (Vector3) output 0
- `Value` <= [14] Input Derive Luminance from Linear (StaticBool) output 0

## Connection List

- [1] Divide output 0 -> Output RemappedRange.A
- [3] Multiply output 0 -> Divide.A
- [7] Divide output 0 -> Divide.B
- [15] Switch output 0 -> Desaturation.Input
- [17] Input Desaturate Luminance (Scalar) output 0 -> Desaturation.Fraction
- [13] Input LDR Input (Vector3) output 0 -> Multiply.A
- [19] Input HDR Dynamic Range (Scalar) output 0 -> Multiply.B
- [19] Input HDR Dynamic Range (Scalar) output 0 -> Multiply.A
- [8] Power output 0 -> Multiply.B
- [19] Input HDR Dynamic Range (Scalar) output 0 -> Add.A
- [6] Subtract(1,) output 0 -> Add.B
- [4] Multiply output 0 -> Subtract(1,).B
- [5] Add output 0 -> Divide.A
- [12] Multiply output 0 -> Divide.B
- [2] Desaturation output 0 -> Power.Base
- [18] Input Derive HDR Power (Scalar) output 0 -> Power.Exponent
- [13] Input LDR Input (Vector3) output 0 -> Power(X, 0.4545).Base
- [11] 1 output 0 -> Lerp.A
- [20] Input HDR Tint (Vector3) output 0 -> Lerp.B
- [8] Power output 0 -> Lerp.Alpha
- [21] Input HDR Intensity Multiplier (Scalar) output 0 -> Multiply.A
- [10] Lerp output 0 -> Multiply.B
- [16] Static Bool Param (False) 'Luminance from Linear' output 0 -> Input Derive Luminance from Linear (StaticBool).Preview
- [9] Power(X, 0.4545) output 0 -> Switch.A
- [13] Input LDR Input (Vector3) output 0 -> Switch.B
- [14] Input Derive Luminance from Linear (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `RemappedRange` is driven by [1] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


