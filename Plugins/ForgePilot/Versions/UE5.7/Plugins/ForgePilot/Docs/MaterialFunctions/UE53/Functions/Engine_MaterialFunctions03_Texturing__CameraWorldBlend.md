# CameraWorldBlend

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/CameraWorldBlend.CameraWorldBlend`
- Category: Engine_MaterialFunctions03/Texturing
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Outputs falloff results for the 3 primary world vectors based on the camera angle.  Can be used to blend between textures based on direction the camera is looking.

## Inputs

- `Blend Power` [FunctionInput_Scalar]
- `Use Reflection Vector` [FunctionInput_StaticBool] - If TRUE, uses ReflectionVector instead of CameraVector (Material's Normal input will effect results)
- `Use Smoothable Normals` [FunctionInput_StaticBool] - If TRUE, uses ReflectionVector instead of CameraVector (Material's Normal input will effect results)
- `Smooth Reflection Percentage` [FunctionInput_Scalar]

## Outputs

- `YZ True`
- `XZ True`
- `XY True`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output YZ True | YZ True |
| 1 | expression | `MaterialExpressionConstant` | 0 |  |
| 2 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 3 | expression | `MaterialExpressionAbs` | Abs |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 6 | expression | `MaterialExpressionPower` | Power |  |
| 7 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Blend Power (Scalar) | Blend Power |
| 9 | expression | `MaterialExpressionConstant` | 3 |  |
| 10 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 11 | expression | `MaterialExpressionAbs` | Abs |  |
| 12 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 13 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 14 | expression | `MaterialExpressionPower` | Power |  |
| 15 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 16 | expression | `MaterialExpressionAbs` | Abs |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 18 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 19 | expression | `MaterialExpressionPower` | Power |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output XZ True | XZ True |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output XY True | XY True |
| 22 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Use Reflection Vector (StaticBool) | Use Reflection Vector |
| 24 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 25 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Use Smoothable Normals (StaticBool) | Use Smoothable Normals |
| 27 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 28 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,) |  |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input Smooth Reflection Percentage (Scalar) | Smooth Reflection Percentage |
| 30 | expression | `MaterialExpressionConstant` | 0 |  |

## Exact Input Wiring

### [0] Output YZ True

- `A` <= [6] Power output 0

### [2] SphereMask

- `A` <= [1] 0 output 0
- `B` <= [3] Abs output 0

### [3] Abs

- `Input` <= [4] Mask ( R ) output 0

### [4] Mask ( R )

- `Input` <= [7] Tangent Space to World Space TransformVector output 0

### [5] 1-x

- `Input` <= [2] SphereMask output 0

### [6] Power

- `Base` <= [5] 1-x output 0
- `Exponent` <= [8] Input Blend Power (Scalar) output 0

### [7] Tangent Space to World Space TransformVector

- `Input` <= [22] Switch output 0

### [8] Input Blend Power (Scalar)

- `Preview` <= [9] 3 output 0

### [10] SphereMask

- `A` <= [1] 0 output 0
- `B` <= [11] Abs output 0

### [11] Abs

- `Input` <= [12] Mask ( G ) output 0

### [12] Mask ( G )

- `Input` <= [7] Tangent Space to World Space TransformVector output 0

### [13] 1-x

- `Input` <= [10] SphereMask output 0

### [14] Power

- `Base` <= [13] 1-x output 0
- `Exponent` <= [8] Input Blend Power (Scalar) output 0

### [15] SphereMask

- `A` <= [1] 0 output 0
- `B` <= [16] Abs output 0

### [16] Abs

- `Input` <= [17] Mask ( B ) output 0

### [17] Mask ( B )

- `Input` <= [7] Tangent Space to World Space TransformVector output 0

### [18] 1-x

- `Input` <= [15] SphereMask output 0

### [19] Power

- `Base` <= [18] 1-x output 0
- `Exponent` <= [8] Input Blend Power (Scalar) output 0

### [20] Output XZ True

- `A` <= [14] Power output 0

### [21] Output XY True

- `A` <= [19] Power output 0

### [22] Switch

- `A` <= [25] Switch output 0
- `Value` <= [23] Input Use Reflection Vector (StaticBool) output 0

### [23] Input Use Reflection Vector (StaticBool)

- `Preview` <= [24] Static Bool (False) output 0

### [25] Switch

- `A` <= [28] Lerp(0,1,) output 0
- `Value` <= [26] Input Use Smoothable Normals (StaticBool) output 0

### [26] Input Use Smoothable Normals (StaticBool)

- `Preview` <= [27] Static Bool (False) output 0

### [28] Lerp(0,1,)

- `Alpha` <= [29] Input Smooth Reflection Percentage (Scalar) output 0

### [29] Input Smooth Reflection Percentage (Scalar)

- `Preview` <= [30] 0 output 0

## Connection List

- [6] Power output 0 -> Output YZ True.A
- [1] 0 output 0 -> SphereMask.A
- [3] Abs output 0 -> SphereMask.B
- [4] Mask ( R ) output 0 -> Abs.Input
- [7] Tangent Space to World Space TransformVector output 0 -> Mask ( R ).Input
- [2] SphereMask output 0 -> 1-x.Input
- [5] 1-x output 0 -> Power.Base
- [8] Input Blend Power (Scalar) output 0 -> Power.Exponent
- [22] Switch output 0 -> Tangent Space to World Space TransformVector.Input
- [9] 3 output 0 -> Input Blend Power (Scalar).Preview
- [1] 0 output 0 -> SphereMask.A
- [11] Abs output 0 -> SphereMask.B
- [12] Mask ( G ) output 0 -> Abs.Input
- [7] Tangent Space to World Space TransformVector output 0 -> Mask ( G ).Input
- [10] SphereMask output 0 -> 1-x.Input
- [13] 1-x output 0 -> Power.Base
- [8] Input Blend Power (Scalar) output 0 -> Power.Exponent
- [1] 0 output 0 -> SphereMask.A
- [16] Abs output 0 -> SphereMask.B
- [17] Mask ( B ) output 0 -> Abs.Input
- [7] Tangent Space to World Space TransformVector output 0 -> Mask ( B ).Input
- [15] SphereMask output 0 -> 1-x.Input
- [18] 1-x output 0 -> Power.Base
- [8] Input Blend Power (Scalar) output 0 -> Power.Exponent
- [14] Power output 0 -> Output XZ True.A
- [19] Power output 0 -> Output XY True.A
- [25] Switch output 0 -> Switch.A
- [23] Input Use Reflection Vector (StaticBool) output 0 -> Switch.Value
- [24] Static Bool (False) output 0 -> Input Use Reflection Vector (StaticBool).Preview
- [28] Lerp(0,1,) output 0 -> Switch.A
- [26] Input Use Smoothable Normals (StaticBool) output 0 -> Switch.Value
- [27] Static Bool (False) output 0 -> Input Use Smoothable Normals (StaticBool).Preview
- [29] Input Smooth Reflection Percentage (Scalar) output 0 -> Lerp(0,1,).Alpha
- [30] 0 output 0 -> Input Smooth Reflection Percentage (Scalar).Preview

## Output Trace Roots

- `YZ True` is driven by [6] Power
- `XZ True` is driven by [14] Power
- `XY True` is driven by [19] Power

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
