# LinearSine

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/LinearSine.LinearSine`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

A sine-like function that transitions linearly in the 0-1 range using a period of 1.

## Inputs

- `Value` [FunctionInput_Scalar] - Insert a gradient or float value to operate on
- `Period` [FunctionInput_Scalar] - Insert float value. Defaults to 1.
- `-1 to 1` [FunctionInput_StaticBool] - Constant bias scale the sine wave to a -1 to 1 range if true.  The default value is false. 
- `Sine Phase` [FunctionInput_StaticBool] - Aligns phase with standard sine function. Defaults to true.

## Outputs

- `Linear Sine` - Returns 0-1 linear sine with a period of 1
- `Rounded Linear Sine ` - Returns 0-1 linear sine with rounded edges with a period of 1
- `Direction` - Returns 0 or 1 based on whether the slope of the sine wave is traveling downward (0) or upward(1).

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Linear Sine | Linear Sine |
| 1 | expression | `MaterialExpressionFrac` | Frac |  |
| 2 | expression | `MaterialExpressionConstant` | 2 |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 6 | expression | `MaterialExpressionFloor` | Floor |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Value (Scalar) | Value |
| 9 | expression | `MaterialExpressionConstant` | 3 |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionConstant` | 2 |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Rounded Linear Sine  | Rounded Linear Sine  |
| 16 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 17 | expression | `MaterialExpressionDivide` | Divide |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Period (Scalar) | Period |
| 19 | expression | `MaterialExpressionConstant` | 1 |  |
| 20 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 21 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 22 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 23 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 24 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input -1 to 1 (StaticBool) | -1 to 1 |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionConstant` | 0.25 |  |
| 28 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 29 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Sine Phase (StaticBool) | Sine Phase |
| 32 | function_output | `MaterialExpressionFunctionOutput` | Output Direction | Direction |
| 33 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,0,) |  |

## Exact Input Wiring

### [0] Output Linear Sine

- `A` <= [23] Switch output 0

### [1] Frac

- `Input` <= [17] Divide output 0

### [3] Multiply

- `A` <= [1] Frac output 0
- `B` <= [2] 2 output 0

### [4] Multiply

- `A` <= [2] 2 output 0
- `B` <= [5] 1-x output 0

### [5] 1-x

- `Input` <= [1] Frac output 0

### [6] Floor

- `Input` <= [3] Multiply output 0

### [7] Lerp

- `A` <= [3] Multiply output 0
- `B` <= [4] Multiply output 0
- `Alpha` <= [6] Floor output 0

### [8] Input Value (Scalar)

- `Preview` <= [16] 0.5 output 0

### [10] Subtract

- `A` <= [9] 3 output 0
- `B` <= [13] Multiply output 0

### [11] Multiply

- `A` <= [10] Subtract output 0
- `B` <= [7] Lerp output 0

### [12] Multiply

- `A` <= [11] Multiply output 0
- `B` <= [7] Lerp output 0

### [13] Multiply

- `A` <= [7] Lerp output 0
- `B` <= [14] 2 output 0

### [15] Output Rounded Linear Sine 

- `A` <= [24] Switch output 0

### [17] Divide

- `A` <= [28] Switch output 0
- `B` <= [18] Input Period (Scalar) output 0

### [20] ConstantBiasScale

- `Input` <= [7] Lerp output 0

### [21] ConstantBiasScale

- `Input` <= [12] Multiply output 0

### [23] Switch

- `A` <= [20] ConstantBiasScale output 0
- `B` <= [7] Lerp output 0
- `Value` <= [25] Input -1 to 1 (StaticBool) output 0

### [24] Switch

- `A` <= [21] ConstantBiasScale output 0
- `B` <= [12] Multiply output 0
- `Value` <= [25] Input -1 to 1 (StaticBool) output 0

### [25] Input -1 to 1 (StaticBool)

- `Preview` <= [22] Static Bool (False) output 0

### [26] Add

- `A` <= [8] Input Value (Scalar) output 0
- `B` <= [30] Multiply output 0

### [28] Switch

- `A` <= [26] Add output 0
- `B` <= [8] Input Value (Scalar) output 0
- `Value` <= [31] Input Sine Phase (StaticBool) output 0

### [30] Multiply

- `A` <= [27] 0.25 output 0
- `B` <= [18] Input Period (Scalar) output 0

### [31] Input Sine Phase (StaticBool)

- `Preview` <= [29] Static Bool (True) output 0

### [32] Output Direction

- `A` <= [33] Lerp(1,0,) output 0

### [33] Lerp(1,0,)

- `Alpha` <= [6] Floor output 0

## Connection List

- [23] Switch output 0 -> Output Linear Sine.A
- [17] Divide output 0 -> Frac.Input
- [1] Frac output 0 -> Multiply.A
- [2] 2 output 0 -> Multiply.B
- [2] 2 output 0 -> Multiply.A
- [5] 1-x output 0 -> Multiply.B
- [1] Frac output 0 -> 1-x.Input
- [3] Multiply output 0 -> Floor.Input
- [3] Multiply output 0 -> Lerp.A
- [4] Multiply output 0 -> Lerp.B
- [6] Floor output 0 -> Lerp.Alpha
- [16] 0.5 output 0 -> Input Value (Scalar).Preview
- [9] 3 output 0 -> Subtract.A
- [13] Multiply output 0 -> Subtract.B
- [10] Subtract output 0 -> Multiply.A
- [7] Lerp output 0 -> Multiply.B
- [11] Multiply output 0 -> Multiply.A
- [7] Lerp output 0 -> Multiply.B
- [7] Lerp output 0 -> Multiply.A
- [14] 2 output 0 -> Multiply.B
- [24] Switch output 0 -> Output Rounded Linear Sine .A
- [28] Switch output 0 -> Divide.A
- [18] Input Period (Scalar) output 0 -> Divide.B
- [7] Lerp output 0 -> ConstantBiasScale.Input
- [12] Multiply output 0 -> ConstantBiasScale.Input
- [20] ConstantBiasScale output 0 -> Switch.A
- [7] Lerp output 0 -> Switch.B
- [25] Input -1 to 1 (StaticBool) output 0 -> Switch.Value
- [21] ConstantBiasScale output 0 -> Switch.A
- [12] Multiply output 0 -> Switch.B
- [25] Input -1 to 1 (StaticBool) output 0 -> Switch.Value
- [22] Static Bool (False) output 0 -> Input -1 to 1 (StaticBool).Preview
- [8] Input Value (Scalar) output 0 -> Add.A
- [30] Multiply output 0 -> Add.B
- [26] Add output 0 -> Switch.A
- [8] Input Value (Scalar) output 0 -> Switch.B
- [31] Input Sine Phase (StaticBool) output 0 -> Switch.Value
- [27] 0.25 output 0 -> Multiply.A
- [18] Input Period (Scalar) output 0 -> Multiply.B
- [29] Static Bool (True) output 0 -> Input Sine Phase (StaticBool).Preview
- [33] Lerp(1,0,) output 0 -> Output Direction.A
- [6] Floor output 0 -> Lerp(1,0,).Alpha

## Output Trace Roots

- `Linear Sine` is driven by [23] Switch
- `Rounded Linear Sine ` is driven by [24] Switch
- `Direction` is driven by [33] Lerp(1,0,)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
