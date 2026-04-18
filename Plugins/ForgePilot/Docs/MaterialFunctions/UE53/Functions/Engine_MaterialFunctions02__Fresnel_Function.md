# Fresnel_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Fresnel_Function.Fresnel_Function`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Fresnel with more properties exposed, less instructions and a cheap alternative falloff.

## Inputs

- `Normal Vector` [FunctionInput_Vector3]
- `Camera Vector` [FunctionInput_Vector3] - Default Value = Camera Vector
- `Power` [FunctionInput_Scalar] - This is used when cheap contrast is set to false.
- `Invert Fresnel` [FunctionInput_StaticBool] - Default Value = False
- `Use Cheap contrast` [FunctionInput_StaticBool] - Default Value = False
- `Cheap contrast dark` [FunctionInput_Scalar] - Black level
- `Cheap contrast bright` [FunctionInput_Scalar] - Bright level
- `Clamp Fresnel Dot Product` [FunctionInput_StaticBool] - Default Value = True

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Normal Vector (Vector3) | Normal Vector |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Camera Vector (Vector3) | Camera Vector |
| 4 | expression | `MaterialExpressionPower` | Power |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Power (Scalar) | Power |
| 6 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 7 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Invert Fresnel (StaticBool) | Invert Fresnel |
| 9 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Use Cheap contrast (StaticBool) | Use Cheap contrast |
| 12 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Cheap contrast dark (Scalar) | Cheap contrast dark |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Cheap contrast bright (Scalar) | Cheap contrast bright |
| 15 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 16 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 17 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 18 | expression | `MaterialExpressionClamp` | Clamp |  |
| 19 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Clamp Fresnel Dot Product (StaticBool) | Clamp Fresnel Dot Product |
| 21 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [12] Switch output 0

### [1] Dot

- `A` <= [2] Input Normal Vector (Vector3) output 0
- `B` <= [3] Input Camera Vector (Vector3) output 0

### [2] Input Normal Vector (Vector3)

- `Preview` <= [17] VertexNormalWS output 0

### [3] Input Camera Vector (Vector3)

- `Preview` <= [16] Camera Vector output 0

### [4] Power

- `Base` <= [6] Switch output 0
- `Exponent` <= [5] Input Power (Scalar) output 0

### [6] Switch

- `A` <= [19] Switch output 0
- `B` <= [7] 1-x output 0
- `Value` <= [8] Input Invert Fresnel (StaticBool) output 0

### [7] 1-x

- `Input` <= [19] Switch output 0

### [8] Input Invert Fresnel (StaticBool)

- `Preview` <= [9] Static Bool (False) output 0

### [10] Lerp

- `A` <= [13] Input Cheap contrast dark (Scalar) output 0
- `B` <= [14] Input Cheap contrast bright (Scalar) output 0
- `Alpha` <= [6] Switch output 0

### [11] Input Use Cheap contrast (StaticBool)

- `Preview` <= [15] Static Bool (False) output 0

### [12] Switch

- `A` <= [10] Lerp output 0
- `B` <= [4] Power output 0
- `Value` <= [11] Input Use Cheap contrast (StaticBool) output 0

### [18] Clamp

- `Input` <= [1] Dot output 0

### [19] Switch

- `A` <= [18] Clamp output 0
- `B` <= [1] Dot output 0
- `Value` <= [20] Input Clamp Fresnel Dot Product (StaticBool) output 0

### [20] Input Clamp Fresnel Dot Product (StaticBool)

- `Preview` <= [21] Static Bool (True) output 0

## Connection List

- [12] Switch output 0 -> Output Result.A
- [2] Input Normal Vector (Vector3) output 0 -> Dot.A
- [3] Input Camera Vector (Vector3) output 0 -> Dot.B
- [17] VertexNormalWS output 0 -> Input Normal Vector (Vector3).Preview
- [16] Camera Vector output 0 -> Input Camera Vector (Vector3).Preview
- [6] Switch output 0 -> Power.Base
- [5] Input Power (Scalar) output 0 -> Power.Exponent
- [19] Switch output 0 -> Switch.A
- [7] 1-x output 0 -> Switch.B
- [8] Input Invert Fresnel (StaticBool) output 0 -> Switch.Value
- [19] Switch output 0 -> 1-x.Input
- [9] Static Bool (False) output 0 -> Input Invert Fresnel (StaticBool).Preview
- [13] Input Cheap contrast dark (Scalar) output 0 -> Lerp.A
- [14] Input Cheap contrast bright (Scalar) output 0 -> Lerp.B
- [6] Switch output 0 -> Lerp.Alpha
- [15] Static Bool (False) output 0 -> Input Use Cheap contrast (StaticBool).Preview
- [10] Lerp output 0 -> Switch.A
- [4] Power output 0 -> Switch.B
- [11] Input Use Cheap contrast (StaticBool) output 0 -> Switch.Value
- [1] Dot output 0 -> Clamp.Input
- [18] Clamp output 0 -> Switch.A
- [1] Dot output 0 -> Switch.B
- [20] Input Clamp Fresnel Dot Product (StaticBool) output 0 -> Switch.Value
- [21] Static Bool (True) output 0 -> Input Clamp Fresnel Dot Product (StaticBool).Preview

## Output Trace Roots

- `Result` is driven by [12] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
