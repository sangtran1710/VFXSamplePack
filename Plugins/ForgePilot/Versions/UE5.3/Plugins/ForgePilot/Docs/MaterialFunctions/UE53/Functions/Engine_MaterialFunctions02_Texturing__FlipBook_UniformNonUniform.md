# FlipBook_UniformNonUniform

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_UniformNonUniform.FlipBook_UniformNonUniform`
- Category: Engine_MaterialFunctions02/Texturing
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This is a special version of the flipbook function only useful to the RenderToTexture blueprint. It allows you to sample Non-Uniform flipbooks from within Uniform Textures. Ie, like a letterboxed flipbook which is how all RenderTarget Previews are set up.

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Animation  Phase (0-1)` [FunctionInput_Scalar] - Apply a "frac" material node to the input value if time is used to control the animation phase.
- `UVs` [FunctionInput_Vector2] - Insert the texture coordinate of your choice.
- `Number of Columns` [FunctionInput_Scalar] - Enter the number of images horizontally across the texture.
- `Number of Rows` [FunctionInput_Scalar] - Enter the number of images vertically across the texture.
- `Clamp Anim (see tooltip)` [FunctionInput_StaticBool] - A frac operation is applied to the animation phase. This causes a value of exactly 1 in the animation phase input to return 0. Enabling this feature will clamp the animation phase value from 0 to .9999 which avoids the issue. 
- `MipBias` [FunctionInput_Scalar]

## Outputs

- `Result`
- `UVs`
- `Alpha`
- `Blend Phase`
- `Y Row`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionDivide` | Divide |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionTime` | Time |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Animation  Phase (0-1) (Scalar) | Animation  Phase (0-1) |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Number of Columns (Scalar) | Number of Columns |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Number of Rows (Scalar) | Number of Rows |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 18 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Clamp Anim (see tooltip) (StaticBool) | Clamp Anim (see tooltip) |
| 20 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 21 | expression | `MaterialExpressionClamp` | Clamp |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input MipBias (Scalar) | MipBias |
| 24 | expression | `MaterialExpressionFrac` | Frac |  |
| 25 | function_call | `MaterialExpressionMaterialFunctionCall` | Round |  |
| 26 | expression | `MaterialExpressionFloor` | Floor |  |
| 27 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 28 | expression | `MaterialExpressionFrac` | Frac |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Blend Phase | Blend Phase |
| 30 | expression | `MaterialExpressionDivide` | Divide |  |
| 31 | expression | `MaterialExpressionAppendVector` | Append |  |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | ScaleUVsByCenter |  |
| 33 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 34 | expression | `MaterialExpressionConstant2Vector` | 1,0.5 |  |
| 35 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 36 | expression | `MaterialExpressionFrac` | Frac |  |
| 37 | expression | `MaterialExpressionConstant` | 1 |  |
| 38 | expression | `MaterialExpressionDivide` | Divide |  |
| 39 | expression | `MaterialExpressionConstant` | 1 |  |
| 40 | expression | `MaterialExpressionMax` | Max |  |
| 41 | expression | `MaterialExpressionMax` | Max |  |
| 42 | function_output | `MaterialExpressionFunctionOutput` | Output Y Row | Y Row |
| 43 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Texture Sample output 0

### [2] Append

- `A` <= [15] Input Number of Columns (Scalar) output 0
- `B` <= [16] Input Number of Rows (Scalar) output 0

### [3] Texture Sample

- `Coordinates` <= [5] Add output 0
- `TextureObject` <= [12] Input Texture (Texture2D) output 0
- `MipValue` <= [23] Input MipBias (Scalar) output 0

### [4] Divide

- `A` <= [14] Input UVs (Vector2) output 0
- `B` <= [2] Append output 0

### [5] Add

- `A` <= [32] ScaleUVsByCenter output 0
- `B` <= [33] Multiply output 0

### [6] Append

- `A` <= [35] Multiply output 0
- `B` <= [16] Input Number of Rows (Scalar) output 0

### [7] Multiply

- `A` <= [6] Append output 0
- `B` <= [10] Append output 0

### [8] Divide

- `A` <= [26] Floor output 0
- `B` <= [2] Append output 0

### [10] Append

- `A` <= [24] Frac output 0
- `B` <= [24] Frac output 0

### [12] Input Texture (Texture2D)

- `Preview` <= [11] Texture Object output 0

### [13] Input Animation  Phase (0-1) (Scalar)

- `Preview` <= [9] Time output 0

### [14] Input UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [17] Output UVs

- `A` <= [5] Add output 0

### [19] Input Clamp Anim (see tooltip) (StaticBool)

- `Preview` <= [18] Static Bool (False) output 0

### [20] Switch

- `A` <= [21] Clamp output 0
- `B` <= [13] Input Animation  Phase (0-1) (Scalar) output 0
- `Value` <= [19] Input Clamp Anim (see tooltip) (StaticBool) output 0

### [21] Clamp

- `Input` <= [13] Input Animation  Phase (0-1) (Scalar) output 0

### [22] Output Alpha

- `A` <= [3] Texture Sample output 4

### [24] Frac

- `Input` <= [20] Switch output 0

### [26] Floor

- `Input` <= [7] Multiply output 0

### [27] Mask ( R )

- `Input` <= [7] Multiply output 0

### [28] Frac

- `Input` <= [27] Mask ( R ) output 0

### [29] Output Blend Phase

- `A` <= [28] Frac output 0

### [30] Divide

- `A` <= [16] Input Number of Rows (Scalar) output 0
- `B` <= [15] Input Number of Columns (Scalar) output 0

### [31] Append

- `A` <= [41] Max output 0
- `B` <= [40] Max output 0

### [33] Multiply

- `A` <= [36] Frac output 0
- `B` <= [31] Append output 0

### [35] Multiply

- `A` <= [15] Input Number of Columns (Scalar) output 0
- `B` <= [16] Input Number of Rows (Scalar) output 0

### [36] Frac

- `Input` <= [8] Divide output 0

### [38] Divide

- `A` <= [15] Input Number of Columns (Scalar) output 0
- `B` <= [16] Input Number of Rows (Scalar) output 0

### [40] Max

- `A` <= [38] Divide output 0
- `B` <= [39] 1 output 0

### [41] Max

- `A` <= [30] Divide output 0
- `B` <= [37] 1 output 0

### [42] Output Y Row

- `A` <= [43] Mask ( G ) output 0

### [43] Mask ( G )

- `Input` <= [26] Floor output 0

## Connection List

- [3] Texture Sample output 0 -> Output Result.A
- [15] Input Number of Columns (Scalar) output 0 -> Append.A
- [16] Input Number of Rows (Scalar) output 0 -> Append.B
- [5] Add output 0 -> Texture Sample.Coordinates
- [12] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [23] Input MipBias (Scalar) output 0 -> Texture Sample.MipValue
- [14] Input UVs (Vector2) output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [32] ScaleUVsByCenter output 0 -> Add.A
- [33] Multiply output 0 -> Add.B
- [35] Multiply output 0 -> Append.A
- [16] Input Number of Rows (Scalar) output 0 -> Append.B
- [6] Append output 0 -> Multiply.A
- [10] Append output 0 -> Multiply.B
- [26] Floor output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [24] Frac output 0 -> Append.A
- [24] Frac output 0 -> Append.B
- [11] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [9] Time output 0 -> Input Animation  Phase (0-1) (Scalar).Preview
- [1] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [5] Add output 0 -> Output UVs.A
- [18] Static Bool (False) output 0 -> Input Clamp Anim (see tooltip) (StaticBool).Preview
- [21] Clamp output 0 -> Switch.A
- [13] Input Animation  Phase (0-1) (Scalar) output 0 -> Switch.B
- [19] Input Clamp Anim (see tooltip) (StaticBool) output 0 -> Switch.Value
- [13] Input Animation  Phase (0-1) (Scalar) output 0 -> Clamp.Input
- [3] Texture Sample output 4 -> Output Alpha.A
- [20] Switch output 0 -> Frac.Input
- [7] Multiply output 0 -> Floor.Input
- [7] Multiply output 0 -> Mask ( R ).Input
- [27] Mask ( R ) output 0 -> Frac.Input
- [28] Frac output 0 -> Output Blend Phase.A
- [16] Input Number of Rows (Scalar) output 0 -> Divide.A
- [15] Input Number of Columns (Scalar) output 0 -> Divide.B
- [41] Max output 0 -> Append.A
- [40] Max output 0 -> Append.B
- [36] Frac output 0 -> Multiply.A
- [31] Append output 0 -> Multiply.B
- [15] Input Number of Columns (Scalar) output 0 -> Multiply.A
- [16] Input Number of Rows (Scalar) output 0 -> Multiply.B
- [8] Divide output 0 -> Frac.Input
- [15] Input Number of Columns (Scalar) output 0 -> Divide.A
- [16] Input Number of Rows (Scalar) output 0 -> Divide.B
- [38] Divide output 0 -> Max.A
- [39] 1 output 0 -> Max.B
- [30] Divide output 0 -> Max.A
- [37] 1 output 0 -> Max.B
- [43] Mask ( G ) output 0 -> Output Y Row.A
- [26] Floor output 0 -> Mask ( G ).Input

## Output Trace Roots

- `Result` is driven by [3] Texture Sample
- `UVs` is driven by [5] Add
- `Alpha` is driven by [3] Texture Sample
- `Blend Phase` is driven by [28] Frac
- `Y Row` is driven by [43] Mask ( G )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
