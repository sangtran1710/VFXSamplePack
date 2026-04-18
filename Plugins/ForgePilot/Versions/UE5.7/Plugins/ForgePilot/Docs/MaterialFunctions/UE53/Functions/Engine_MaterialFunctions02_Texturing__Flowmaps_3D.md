# Flowmaps_3D

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/Flowmaps_3D.Flowmaps_3D`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Performs Flowmap blending using a Volume Texture.

## Inputs

- `Volume Texture` [FunctionInput_VolumeTexture]
- `UVW` [FunctionInput_Vector3]
- `Velocity` [FunctionInput_Vector3]
- `Override Time` [FunctionInput_Scalar]
- `Mip Level` [FunctionInput_Scalar]

## Outputs

- `Result RGB`
- `Result RGBA`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result RGB | Result RGB |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Volume Texture (VolumeTexture) | Volume Texture |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input UVW (Vector3) | UVW |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Velocity (Vector3) | Velocity |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Override Time (Scalar) | Override Time |
| 6 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 7 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 8 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 9 | expression | `MaterialExpressionTime` | Time |  |
| 10 | expression | `MaterialExpressionFrac` | Frac |  |
| 11 | expression | `MaterialExpressionFrac` | Frac |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 17 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 18 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 19 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Result RGBA | Result RGBA |
| 21 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 22 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 23 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Mip Level (Scalar) | Mip Level |
| 25 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 26 | expression | `MaterialExpressionSine` | Sine |  |
| 27 | expression | `MaterialExpressionAdd` | Add(,-0.25) |  |

## Exact Input Wiring

### [0] Output Result RGB

- `A` <= [19] Lerp output 0

### [1] Input Volume Texture (VolumeTexture)

- `Preview` <= [7] Texture Object output 0

### [2] Input UVW (Vector3)

- `Preview` <= [8] Absolute World Position output 0

### [3] Texture Sample

- `Coordinates` <= [13] Subtract output 0
- `TextureObject` <= [23] Reroute Node (reroutes wires) output 0
- `MipValue` <= [24] Input Mip Level (Scalar) output 0

### [5] Input Override Time (Scalar)

- `Preview` <= [9] Time output 0

### [6] Texture Sample

- `Coordinates` <= [14] Subtract output 0
- `TextureObject` <= [23] Reroute Node (reroutes wires) output 0
- `MipValue` <= [24] Input Mip Level (Scalar) output 0

### [10] Frac

- `Input` <= [22] Reroute Node (reroutes wires) output 0

### [11] Frac

- `Input` <= [25] Subtract(,0.5) output 0

### [12] Multiply

- `A` <= [16] Reroute Node (reroutes wires) output 0
- `B` <= [11] Frac output 0

### [13] Subtract

- `A` <= [17] Reroute Node (reroutes wires) output 0
- `B` <= [12] Multiply output 0

### [14] Subtract

- `A` <= [17] Reroute Node (reroutes wires) output 0
- `B` <= [15] Multiply output 0

### [15] Multiply

- `A` <= [16] Reroute Node (reroutes wires) output 0
- `B` <= [10] Frac output 0

### [16] Reroute Node (reroutes wires)

- `Input` <= [4] Input Velocity (Vector3) output 0

### [17] Reroute Node (reroutes wires)

- `Input` <= [2] Input UVW (Vector3) output 0

### [18] ConstantBiasScale

- `Input` <= [26] Sine output 0

### [19] Lerp

- `A` <= [3] Texture Sample output 0
- `B` <= [6] Texture Sample output 0
- `Alpha` <= [18] ConstantBiasScale output 0

### [20] Output Result RGBA

- `A` <= [21] Lerp output 0

### [21] Lerp

- `A` <= [3] Texture Sample output 5
- `B` <= [6] Texture Sample output 5
- `Alpha` <= [18] ConstantBiasScale output 0

### [22] Reroute Node (reroutes wires)

- `Input` <= [5] Input Override Time (Scalar) output 0

### [23] Reroute Node (reroutes wires)

- `Input` <= [1] Input Volume Texture (VolumeTexture) output 0

### [25] Subtract(,0.5)

- `A` <= [5] Input Override Time (Scalar) output 0

### [26] Sine

- `Input` <= [27] Add(,-0.25) output 0

### [27] Add(,-0.25)

- `A` <= [22] Reroute Node (reroutes wires) output 0

## Connection List

- [19] Lerp output 0 -> Output Result RGB.A
- [7] Texture Object output 0 -> Input Volume Texture (VolumeTexture).Preview
- [8] Absolute World Position output 0 -> Input UVW (Vector3).Preview
- [13] Subtract output 0 -> Texture Sample.Coordinates
- [23] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [24] Input Mip Level (Scalar) output 0 -> Texture Sample.MipValue
- [9] Time output 0 -> Input Override Time (Scalar).Preview
- [14] Subtract output 0 -> Texture Sample.Coordinates
- [23] Reroute Node (reroutes wires) output 0 -> Texture Sample.TextureObject
- [24] Input Mip Level (Scalar) output 0 -> Texture Sample.MipValue
- [22] Reroute Node (reroutes wires) output 0 -> Frac.Input
- [25] Subtract(,0.5) output 0 -> Frac.Input
- [16] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [11] Frac output 0 -> Multiply.B
- [17] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [12] Multiply output 0 -> Subtract.B
- [17] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [15] Multiply output 0 -> Subtract.B
- [16] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [10] Frac output 0 -> Multiply.B
- [4] Input Velocity (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [2] Input UVW (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [26] Sine output 0 -> ConstantBiasScale.Input
- [3] Texture Sample output 0 -> Lerp.A
- [6] Texture Sample output 0 -> Lerp.B
- [18] ConstantBiasScale output 0 -> Lerp.Alpha
- [21] Lerp output 0 -> Output Result RGBA.A
- [3] Texture Sample output 5 -> Lerp.A
- [6] Texture Sample output 5 -> Lerp.B
- [18] ConstantBiasScale output 0 -> Lerp.Alpha
- [5] Input Override Time (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [1] Input Volume Texture (VolumeTexture) output 0 -> Reroute Node (reroutes wires).Input
- [5] Input Override Time (Scalar) output 0 -> Subtract(,0.5).A
- [27] Add(,-0.25) output 0 -> Sine.Input
- [22] Reroute Node (reroutes wires) output 0 -> Add(,-0.25).A

## Output Trace Roots

- `Result RGB` is driven by [19] Lerp
- `Result RGBA` is driven by [21] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
