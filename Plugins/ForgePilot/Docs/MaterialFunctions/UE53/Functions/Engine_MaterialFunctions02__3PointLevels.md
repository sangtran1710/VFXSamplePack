# 3PointLevels

- Path: `/Engine/Functions/Engine_MaterialFunctions02/3PointLevels.3PointLevels`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Remaps 0-1 values by linearly interpolating through 3 new values.

## Inputs

- `New Black Value` [FunctionInput_Scalar] - Set the new value for the previous value of 0.
- `New Middle Value` [FunctionInput_Scalar] - Replaces the old value that matches the Middle Point input value.
- `New White Value` [FunctionInput_Scalar] - Set the new value for the previous value of 1.
- `Middle Point` [FunctionInput_Scalar] - Pick a value from the input image that will get replaced with the value from Middle Value.
- `Texture` [FunctionInput_Scalar] - Pick a value from the input image that will get replaced with the value from Middle Value.
- `---------------` [FunctionInput_StaticBool] - Pick a value from the input image that will get replaced with the value from Middle Value.
- `--------------- ` [FunctionInput_StaticBool] - Pick a value from the input image that will get replaced with the value from Middle Value.
- `Define Interpolation Curve` [FunctionInput_StaticBool] - Pick a value from the input image that will get replaced with the value from Middle Value.
- `Interpolation Power` [FunctionInput_Scalar] - A power node applied to the linear interpolation. This defines the curve of the interpolation.
- `Invert Interpolation Power` [FunctionInput_StaticBool] - Invert the power curve.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | expression | `MaterialExpressionClamp` | Clamp |  |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |
| 11 | expression | `MaterialExpressionFloor` | Floor |  |
| 12 | expression | `MaterialExpressionClamp` | Clamp |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input New Black Value (Scalar) | New Black Value |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input New Middle Value (Scalar) | New Middle Value |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input New White Value (Scalar) | New White Value |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Middle Point (Scalar) | Middle Point |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Scalar) | Texture |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input --------------- (StaticBool) | --------------- |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input ---------------  (StaticBool) | ---------------  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Define Interpolation Curve (StaticBool) | Define Interpolation Curve |
| 21 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 22 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 23 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 24 | expression | `MaterialExpressionPower` | Power |  |
| 25 | expression | `MaterialExpressionPower` | Power |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Interpolation Power (Scalar) | Interpolation Power |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Invert Interpolation Power (StaticBool) | Invert Interpolation Power |
| 28 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 29 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 30 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 31 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 32 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 33 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 34 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 35 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 36 | expression | `MaterialExpressionStaticSwitch` | Switch |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] Lerp output 0

### [1] Lerp

- `A` <= [13] Input New Black Value (Scalar) output 0
- `B` <= [14] Input New Middle Value (Scalar) output 0
- `Alpha` <= [23] Switch output 0

### [2] Lerp

- `A` <= [1] Lerp output 0
- `B` <= [3] Lerp output 0
- `Alpha` <= [11] Floor output 0

### [3] Lerp

- `A` <= [15] Input New White Value (Scalar) output 0
- `B` <= [14] Input New Middle Value (Scalar) output 0
- `Alpha` <= [22] Switch output 0

### [5] Clamp

- `Input` <= [8] Divide output 0

### [6] 1-x

- `Input` <= [17] Input Texture (Scalar) output 0

### [7] Mask ( R )

- `Input` <= [4] TexCoord[0] output 0

### [8] Divide

- `A` <= [6] 1-x output 0
- `B` <= [9] 1-x output 0

### [9] 1-x

- `Input` <= [16] Input Middle Point (Scalar) output 0

### [10] Divide

- `A` <= [17] Input Texture (Scalar) output 0
- `B` <= [16] Input Middle Point (Scalar) output 0

### [11] Floor

- `Input` <= [23] Switch output 0

### [12] Clamp

- `Input` <= [10] Divide output 0

### [17] Input Texture (Scalar)

- `Preview` <= [7] Mask ( R ) output 0

### [20] Input Define Interpolation Curve (StaticBool)

- `Preview` <= [21] Static Bool (False) output 0

### [22] Switch

- `A` <= [31] Switch output 0
- `B` <= [5] Clamp output 0
- `Value` <= [20] Input Define Interpolation Curve (StaticBool) output 0

### [23] Switch

- `A` <= [36] Switch output 0
- `B` <= [12] Clamp output 0
- `Value` <= [20] Input Define Interpolation Curve (StaticBool) output 0

### [24] Power

- `Base` <= [30] Switch output 0
- `Exponent` <= [26] Input Interpolation Power (Scalar) output 0

### [25] Power

- `Base` <= [33] Switch output 0
- `Exponent` <= [26] Input Interpolation Power (Scalar) output 0

### [27] Input Invert Interpolation Power (StaticBool)

- `Preview` <= [28] Static Bool (False) output 0

### [29] 1-x

- `Input` <= [5] Clamp output 0

### [30] Switch

- `A` <= [29] 1-x output 0
- `B` <= [5] Clamp output 0
- `Value` <= [27] Input Invert Interpolation Power (StaticBool) output 0

### [31] Switch

- `A` <= [32] 1-x output 0
- `B` <= [24] Power output 0
- `Value` <= [27] Input Invert Interpolation Power (StaticBool) output 0

### [32] 1-x

- `Input` <= [24] Power output 0

### [33] Switch

- `A` <= [34] 1-x output 0
- `B` <= [12] Clamp output 0
- `Value` <= [27] Input Invert Interpolation Power (StaticBool) output 0

### [34] 1-x

- `Input` <= [12] Clamp output 0

### [35] 1-x

- `Input` <= [25] Power output 0

### [36] Switch

- `A` <= [35] 1-x output 0
- `B` <= [25] Power output 0
- `Value` <= [27] Input Invert Interpolation Power (StaticBool) output 0

## Connection List

- [2] Lerp output 0 -> Output Result.A
- [13] Input New Black Value (Scalar) output 0 -> Lerp.A
- [14] Input New Middle Value (Scalar) output 0 -> Lerp.B
- [23] Switch output 0 -> Lerp.Alpha
- [1] Lerp output 0 -> Lerp.A
- [3] Lerp output 0 -> Lerp.B
- [11] Floor output 0 -> Lerp.Alpha
- [15] Input New White Value (Scalar) output 0 -> Lerp.A
- [14] Input New Middle Value (Scalar) output 0 -> Lerp.B
- [22] Switch output 0 -> Lerp.Alpha
- [8] Divide output 0 -> Clamp.Input
- [17] Input Texture (Scalar) output 0 -> 1-x.Input
- [4] TexCoord[0] output 0 -> Mask ( R ).Input
- [6] 1-x output 0 -> Divide.A
- [9] 1-x output 0 -> Divide.B
- [16] Input Middle Point (Scalar) output 0 -> 1-x.Input
- [17] Input Texture (Scalar) output 0 -> Divide.A
- [16] Input Middle Point (Scalar) output 0 -> Divide.B
- [23] Switch output 0 -> Floor.Input
- [10] Divide output 0 -> Clamp.Input
- [7] Mask ( R ) output 0 -> Input Texture (Scalar).Preview
- [21] Static Bool (False) output 0 -> Input Define Interpolation Curve (StaticBool).Preview
- [31] Switch output 0 -> Switch.A
- [5] Clamp output 0 -> Switch.B
- [20] Input Define Interpolation Curve (StaticBool) output 0 -> Switch.Value
- [36] Switch output 0 -> Switch.A
- [12] Clamp output 0 -> Switch.B
- [20] Input Define Interpolation Curve (StaticBool) output 0 -> Switch.Value
- [30] Switch output 0 -> Power.Base
- [26] Input Interpolation Power (Scalar) output 0 -> Power.Exponent
- [33] Switch output 0 -> Power.Base
- [26] Input Interpolation Power (Scalar) output 0 -> Power.Exponent
- [28] Static Bool (False) output 0 -> Input Invert Interpolation Power (StaticBool).Preview
- [5] Clamp output 0 -> 1-x.Input
- [29] 1-x output 0 -> Switch.A
- [5] Clamp output 0 -> Switch.B
- [27] Input Invert Interpolation Power (StaticBool) output 0 -> Switch.Value
- [32] 1-x output 0 -> Switch.A
- [24] Power output 0 -> Switch.B
- [27] Input Invert Interpolation Power (StaticBool) output 0 -> Switch.Value
- [24] Power output 0 -> 1-x.Input
- [34] 1-x output 0 -> Switch.A
- [12] Clamp output 0 -> Switch.B
- [27] Input Invert Interpolation Power (StaticBool) output 0 -> Switch.Value
- [12] Clamp output 0 -> 1-x.Input
- [25] Power output 0 -> 1-x.Input
- [35] 1-x output 0 -> Switch.A
- [25] Power output 0 -> Switch.B
- [27] Input Invert Interpolation Power (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `Result` is driven by [2] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
