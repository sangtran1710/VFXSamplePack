# 3dParticleOpacity

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/3dParticleOpacity.3dParticleOpacity`
- Category: Engine_MaterialFunctions02/Texturing
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Explicit Texture Depth (WS)` [FunctionInput_Scalar] - World space scale of the depth texture. A value of 256 will make the depth texture 256 units deep.
- `Depth Texture` [FunctionInput_Scalar] - Add an inverted z-depth render (black near the camera and white values in the background). Be sure that the textures SRGB switch is disabled in the texture properties window.
- `Use Near Camera Falloff` [FunctionInput_StaticBool] - Makes the object falloff as they approach the camera. The default value is false.
- `Use Particle Alpha` [FunctionInput_StaticBool] - Multiplies the result by the particle color alpha channel. The default value is false.
- `Depth Texture Falloff Softness (WS) (1/n)` [FunctionInput_Scalar] - The distance in world space units that the objects opacity gradually fades out across. Enter 1 divided by the number of world space units you would like the transition to take place across. For instance enter 1/16 in the input field and Unreal will convert that to 0.062500. Doing this ahead of time removes an instruction from the effect. 
- `Near Camera Fade Distance (1/n)` [FunctionInput_Scalar] - The distance from the camera where the opacity fades to 0. Enter the reciprocal of the target distance. For example to fade the particle out over 256 units enter 1/256 which Unreal will turn into 0.003906.
- `Near Camera Falloff Start Distance` [FunctionInput_Scalar] - The distance in world space units from the camera where the object remains transparent.
- `-------Camera Falloff Group-------` [FunctionInput_StaticBool]
- `-------Particle Alpha Group-------` [FunctionInput_StaticBool]
- `Use Depth In Camera Falloff Calculation` [FunctionInput_StaticBool] - Makes closer pixels (according to the depth map) fade out before farther pixels. The default value is false.
- `-------WPO Group-------` [FunctionInput_StaticBool]
- `Camera Offset` [FunctionInput_Scalar] - This value should be the same value as "Texture Depth." The "Texture Depth" value is used as this inputs default value. 
- `Dynamic Texture Depth Ratio` [FunctionInput_Scalar] - Enter the ratio of the height textures depth to the height textures width (use 3ds max world unit measurements). For instance, if you create a plane in a 3D application that is 512x512 and you render a height texture from that plane that represents 256 units away from the plane then you would calculate this number by dividing 256/512 which would be a ratio of .5. 
- `Use dynamic (True) or explicit texture depth settings (False)` [FunctionInput_StaticBool] - The default setting is dynamic texture depth. Using explicit texture settings will allow you to specifiy exactly how many world units your depth texture represents. For example 512 units would make the texture 512 units deep. The dynamic texture depth option will use the ratio between the particles size and it's depth to scale the depth for differently scaled particles. For example if your ratio was set to 1, a square particle that was 512 units would set the 512 unit depth texture. A ratio value of .5 would generate a texture depth of 256. This helps the effect scale properly with various particle sizes.
- `-------------Depth Texture Options-----------` [FunctionInput_StaticBool]

## Outputs

- `Opacity`
- `World Position Offset - Camera Offset` - Emulates what camera offset does in the particle editor. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Opacity | Opacity |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionSceneDepth` | Scene Depth |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 5 | expression | `MaterialExpressionClamp` | Clamp |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionParticleColor` | Particle Color |  |
| 10 | expression | `MaterialExpressionClamp` | Clamp |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Explicit Texture Depth (WS) (Scalar) | Explicit Texture Depth (WS) |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Depth Texture (Scalar) | Depth Texture |
| 14 | expression | `MaterialExpressionClamp` | Clamp |  |
| 15 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Use Near Camera Falloff (StaticBool) | Use Near Camera Falloff |
| 17 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 18 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Use Particle Alpha (StaticBool) | Use Particle Alpha |
| 20 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Depth Texture Falloff Softness (WS) (1/n) (Scalar) | Depth Texture Falloff Softness (WS) (1/n) |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Near Camera Fade Distance (1/n) (Scalar) | Near Camera Fade Distance (1/n) |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Near Camera Falloff Start Distance (Scalar) | Near Camera Falloff Start Distance |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 27 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input -------Camera Falloff Group------- (StaticBool) | -------Camera Falloff Group------- |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input -------Particle Alpha Group------- (StaticBool) | -------Particle Alpha Group------- |
| 30 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 31 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 32 | function_input | `MaterialExpressionFunctionInput` | Input Use Depth In Camera Falloff Calculation (StaticBool) | Use Depth In Camera Falloff Calculation |
| 33 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 34 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 35 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 36 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset - Camera Offset | World Position Offset - Camera Offset |
| 40 | function_input | `MaterialExpressionFunctionInput` | Input -------WPO Group------- (StaticBool) | -------WPO Group------- |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input Camera Offset (Scalar) | Camera Offset |
| 42 | expression | `MaterialExpressionParticleRadius` | Particle Radius |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input Dynamic Texture Depth Ratio (Scalar) | Dynamic Texture Depth Ratio |
| 45 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 46 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 47 | function_input | `MaterialExpressionFunctionInput` | Input Use dynamic (True) or explicit texture depth settings (False) (StaticBool) | Use dynamic (True) or explicit texture depth settings (False) |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input -------------Depth Texture Options----------- (StaticBool) | -------------Depth Texture Options----------- |

## Exact Input Wiring

### [0] Output Opacity

- `A` <= [14] Clamp output 0

### [3] Subtract

- `A` <= [2] Scene Depth output 0
- `B` <= [25] Add output 0

### [5] Clamp

- `Input` <= [22] Multiply output 0

### [6] Multiply

- `A` <= [13] Input Depth Texture (Scalar) output 0
- `B` <= [46] Switch output 0

### [7] Multiply

- `A` <= [10] Clamp output 0
- `B` <= [18] Switch output 0

### [8] Multiply

- `A` <= [5] Clamp output 0
- `B` <= [9] Particle Color output 4

### [10] Clamp

- `Input` <= [30] Multiply output 0

### [11] Subtract

- `A` <= [31] Switch output 0
- `B` <= [24] Input Near Camera Falloff Start Distance (Scalar) output 0

### [13] Input Depth Texture (Scalar)

- `Preview` <= [1] Texture Sample output 1

### [14] Clamp

- `Input` <= [15] Switch output 0

### [15] Switch

- `A` <= [7] Multiply output 0
- `B` <= [18] Switch output 0
- `Value` <= [16] Input Use Near Camera Falloff (StaticBool) output 0

### [16] Input Use Near Camera Falloff (StaticBool)

- `Preview` <= [17] Static Bool (False) output 0

### [18] Switch

- `A` <= [8] Multiply output 0
- `B` <= [5] Clamp output 0
- `Value` <= [19] Input Use Particle Alpha (StaticBool) output 0

### [19] Input Use Particle Alpha (StaticBool)

- `Preview` <= [20] Static Bool (False) output 0

### [22] Multiply

- `A` <= [3] Subtract output 0
- `B` <= [21] Input Depth Texture Falloff Softness (WS) (1/n) (Scalar) output 0

### [25] Add

- `A` <= [4] PixelDepth output 0
- `B` <= [6] Multiply output 0

### [26] Subtract

- `A` <= [4] PixelDepth output 0
- `B` <= [27] Subtract output 0

### [27] Subtract

- `A` <= [46] Switch output 0
- `B` <= [6] Multiply output 0

### [30] Multiply

- `A` <= [11] Subtract output 0
- `B` <= [23] Input Near Camera Fade Distance (1/n) (Scalar) output 0

### [31] Switch

- `A` <= [26] Subtract output 0
- `B` <= [4] PixelDepth output 0
- `Value` <= [32] Input Use Depth In Camera Falloff Calculation (StaticBool) output 0

### [32] Input Use Depth In Camera Falloff Calculation (StaticBool)

- `Preview` <= [33] Static Bool (False) output 0

### [35] Subtract

- `A` <= [38] Camera Position output 0
- `B` <= [34] Particle Position (Absolute) output 0

### [36] Normalize

- `VectorInput` <= [35] Subtract output 0

### [37] Multiply

- `A` <= [36] Normalize output 0
- `B` <= [41] Input Camera Offset (Scalar) output 0

### [39] Output World Position Offset - Camera Offset

- `A` <= [37] Multiply output 0

### [41] Input Camera Offset (Scalar)

- `Preview` <= [46] Switch output 0

### [43] Multiply

- `A` <= [42] Particle Radius output 0
- `B` <= [44] Input Dynamic Texture Depth Ratio (Scalar) output 0

### [46] Switch

- `A` <= [43] Multiply output 0
- `B` <= [12] Input Explicit Texture Depth (WS) (Scalar) output 0
- `Value` <= [47] Input Use dynamic (True) or explicit texture depth settings (False) (StaticBool) output 0

### [47] Input Use dynamic (True) or explicit texture depth settings (False) (StaticBool)

- `Preview` <= [45] Static Bool (True) output 0

## Connection List

- [14] Clamp output 0 -> Output Opacity.A
- [2] Scene Depth output 0 -> Subtract.A
- [25] Add output 0 -> Subtract.B
- [22] Multiply output 0 -> Clamp.Input
- [13] Input Depth Texture (Scalar) output 0 -> Multiply.A
- [46] Switch output 0 -> Multiply.B
- [10] Clamp output 0 -> Multiply.A
- [18] Switch output 0 -> Multiply.B
- [5] Clamp output 0 -> Multiply.A
- [9] Particle Color output 4 -> Multiply.B
- [30] Multiply output 0 -> Clamp.Input
- [31] Switch output 0 -> Subtract.A
- [24] Input Near Camera Falloff Start Distance (Scalar) output 0 -> Subtract.B
- [1] Texture Sample output 1 -> Input Depth Texture (Scalar).Preview
- [15] Switch output 0 -> Clamp.Input
- [7] Multiply output 0 -> Switch.A
- [18] Switch output 0 -> Switch.B
- [16] Input Use Near Camera Falloff (StaticBool) output 0 -> Switch.Value
- [17] Static Bool (False) output 0 -> Input Use Near Camera Falloff (StaticBool).Preview
- [8] Multiply output 0 -> Switch.A
- [5] Clamp output 0 -> Switch.B
- [19] Input Use Particle Alpha (StaticBool) output 0 -> Switch.Value
- [20] Static Bool (False) output 0 -> Input Use Particle Alpha (StaticBool).Preview
- [3] Subtract output 0 -> Multiply.A
- [21] Input Depth Texture Falloff Softness (WS) (1/n) (Scalar) output 0 -> Multiply.B
- [4] PixelDepth output 0 -> Add.A
- [6] Multiply output 0 -> Add.B
- [4] PixelDepth output 0 -> Subtract.A
- [27] Subtract output 0 -> Subtract.B
- [46] Switch output 0 -> Subtract.A
- [6] Multiply output 0 -> Subtract.B
- [11] Subtract output 0 -> Multiply.A
- [23] Input Near Camera Fade Distance (1/n) (Scalar) output 0 -> Multiply.B
- [26] Subtract output 0 -> Switch.A
- [4] PixelDepth output 0 -> Switch.B
- [32] Input Use Depth In Camera Falloff Calculation (StaticBool) output 0 -> Switch.Value
- [33] Static Bool (False) output 0 -> Input Use Depth In Camera Falloff Calculation (StaticBool).Preview
- [38] Camera Position output 0 -> Subtract.A
- [34] Particle Position (Absolute) output 0 -> Subtract.B
- [35] Subtract output 0 -> Normalize.VectorInput
- [36] Normalize output 0 -> Multiply.A
- [41] Input Camera Offset (Scalar) output 0 -> Multiply.B
- [37] Multiply output 0 -> Output World Position Offset - Camera Offset.A
- [46] Switch output 0 -> Input Camera Offset (Scalar).Preview
- [42] Particle Radius output 0 -> Multiply.A
- [44] Input Dynamic Texture Depth Ratio (Scalar) output 0 -> Multiply.B
- [43] Multiply output 0 -> Switch.A
- [12] Input Explicit Texture Depth (WS) (Scalar) output 0 -> Switch.B
- [47] Input Use dynamic (True) or explicit texture depth settings (False) (StaticBool) output 0 -> Switch.Value
- [45] Static Bool (True) output 0 -> Input Use dynamic (True) or explicit texture depth settings (False) (StaticBool).Preview

## Output Trace Roots

- `Opacity` is driven by [14] Clamp
- `World Position Offset - Camera Offset` is driven by [37] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


