# Flowmaps_2D

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/Flowmaps_2D.Flowmaps_2D`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This 2D version of flowmaps is designed to be the 2D version of the 3D flowmap function. It is designed to work using World XY Coordinates instead of UVs.

## Inputs

- `Volume Texture` [FunctionInput_Texture2D]
- `UV` [FunctionInput_Vector2]
- `Velocity` [FunctionInput_Vector2]
- `Override Time` [FunctionInput_Scalar]
- `Mip Level` [FunctionInput_Scalar]

## Outputs

- `Result RGB`
- `Result RGBA`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result RGB | Result RGB |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Volume Texture (Texture2D) | Volume Texture |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input UV (Vector2) | UV |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Velocity (Vector2) | Velocity |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Override Time (Scalar) | Override Time |
| 5 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 7 | expression | `MaterialExpressionTime` | Time |  |
| 8 | expression | `MaterialExpressionFrac` | Frac |  |
| 9 | expression | `MaterialExpressionFrac` | Frac |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 15 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 16 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 17 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Result RGBA | Result RGBA |
| 19 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 20 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 21 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Mip Level (Scalar) | Mip Level |
| 23 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 24 | expression | `MaterialExpressionSine` | Sine |  |
| 25 | expression | `MaterialExpressionAdd` | Add(,-0.25) |  |
| 26 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 27 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |

## Exact Input Wiring

### [0] Output Result RGB

- `A` <= [17] Lerp output 0

### [1] Input Volume Texture (Texture2D)

- `Preview` <= [5] Texture Object output 0

### [2] Input UV (Vector2)

- `Preview` <= [6] Absolute World Position output 0

### [4] Input Override Time (Scalar)

- `Preview` <= [7] Time output 0

### [8] Frac

- `Input` <= [20] Reroute Node (reroutes wires) output 0

### [9] Frac

- `Input` <= [23] Subtract(,0.5) output 0

### [10] Multiply

- `A` <= [14] Reroute Node (reroutes wires) output 0
- `B` <= [9] Frac output 0

### [11] Subtract

- `A` <= [15] Reroute Node (reroutes wires) output 0
- `B` <= [10] Multiply output 0

### [12] Subtract

- `A` <= [15] Reroute Node (reroutes wires) output 0
- `B` <= [13] Multiply output 0

### [13] Multiply

- `A` <= [14] Reroute Node (reroutes wires) output 0
- `B` <= [8] Frac output 0

### [14] Reroute Node (reroutes wires)

- `Input` <= [3] Input Velocity (Vector2) output 0

### [15] Reroute Node (reroutes wires)

- `Input` <= [2] Input UV (Vector2) output 0

### [16] ConstantBiasScale

- `Input` <= [24] Sine output 0

### [17] Lerp

- `A` <= [26] Texture Sample output 0
- `B` <= [27] Texture Sample output 0
- `Alpha` <= [16] ConstantBiasScale output 0

### [18] Output Result RGBA

- `A` <= [19] Lerp output 0

### [19] Lerp

- `A` <= [26] Texture Sample output 4
- `B` <= [27] Texture Sample output 4
- `Alpha` <= [16] ConstantBiasScale output 0

### [20] Reroute Node (reroutes wires)

- `Input` <= [4] Input Override Time (Scalar) output 0

### [21] Reroute Node (reroutes wires)

- `Input` <= [1] Input Volume Texture (Texture2D) output 0

### [23] Subtract(,0.5)

- `A` <= [4] Input Override Time (Scalar) output 0

### [24] Sine

- `Input` <= [25] Add(,-0.25) output 0

### [25] Add(,-0.25)

- `A` <= [20] Reroute Node (reroutes wires) output 0

### [26] Texture Sample

- `Coordinates` <= [11] Subtract output 0
- `TextureObject` <= [21] Reroute Node (reroutes wires) output 0
- `MipValue` <= [22] Input Mip Level (Scalar) output 0

### [27] Texture Sample

- `Coordinates` <= [12] Subtract output 0
- `TextureObject` <= [21] Reroute Node (reroutes wires) output 0
- `MipValue` <= [22] Input Mip Level (Scalar) output 0

## Connection List

- [17] Lerp output 0 -> Output Result RGB.A
- [5] Texture Object output 0 -> Input Volume Texture (Texture2D).Preview
- [6] Absolute World Position output 0 -> Input UV (Vector2).Preview
- [7] Time output 0 -> Input Override Time (Scalar).Preview
- [20] Reroute Node (reroutes wires) output 0 -> Frac.Input
- [23] Subtract(,0.5) output 0 -> Frac.Input
- [14] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [9] Frac output 0 -> Multiply.B
- [15] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [10] Multiply output 0 -> Subtract.B
- [15] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [13] Multiply output 0 -> Subtract.B
- [14] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [8] Frac output 0 -> Multiply.B
- [3] Input Velocity (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [2] Input UV (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [24] Sine output 0 -> ConstantBiasScale.Input
- [26] Texture Sample output 0 -> Lerp.A
- [27] Texture Sample output 0 -> Lerp.B
- [16] ConstantBiasScale output 0 -> Lerp.Alpha
- [19] Lerp output 0 -> Output Result RGBA.A
- [26] Texture Sample output 4 -> Lerp.A
- [27] Texture Sample output 4 -> Lerp.B
- [16] ConstantBiasScale output 0 -> Lerp.Alpha
- [4] Input Override Time (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [1] Input Volume Texture (Texture2D) output 0 -> Reroute Node (reroutes wires).Input
- [4] Input Override Time (Scalar) output 0 -> Subtract(,0.5).A
- [25] Add(,-0.25) output 0 -> Sine.Input
- [20] Reroute Node (reroutes wires) output 0 -> Add(,-0.25).A
- [11] Subtract output 0 -> Texture Sample.Coordinates
- [21] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [22] Input Mip Level (Scalar) output 0 -> Texture Sample.MipValue
- [12] Subtract output 0 -> Texture Sample.Coordinates
- [21] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [22] Input Mip Level (Scalar) output 0 -> Texture Sample.MipValue

## Output Trace Roots

- `Result RGB` is driven by [17] Lerp
- `Result RGBA` is driven by [19] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


