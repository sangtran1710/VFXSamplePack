# FlipBook

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook.FlipBook`
- Category: Engine_MaterialFunctions02/Texturing
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Animates a texture in a flipbook like fashion. 

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Animation  Phase (0-1)` [FunctionInput_Scalar] - Apply a "frac" material node to the input value if time is used to control the animation phase.
- `UVs` [FunctionInput_Vector2] - Insert the texture coordinate of your choice.
- `Number of Columns` [FunctionInput_Scalar] - Enter the number of images horizontally across the texture.
- `Number of Rows` [FunctionInput_Scalar] - Enter the number of images vertically across the texture.
- `Clamp Anim (see tooltip)` [FunctionInput_StaticBool] - A frac operation is applied to the animation phase. This causes a value of exactly 1 in the animation phase input to return 0. Enabling this feature will clamp the animation phase value from 0 to .9999 which avoids the issue. 
- `MipBias/Level` [FunctionInput_Scalar]
- `Use Mip Bias (T) Level (F)` [FunctionInput_StaticBool] - Defaults to true.

## Outputs

- `Result`
- `UVs`
- `Alpha`
- `UV Center` - This value can be used for subsquent rotaiton operatons etc.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionDivide` | Divide |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionTime` | Time |  |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Animation  Phase (0-1) (Scalar) | Animation  Phase (0-1) |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Number of Columns (Scalar) | Number of Columns |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Number of Rows (Scalar) | Number of Rows |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 19 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Clamp Anim (see tooltip) (StaticBool) | Clamp Anim (see tooltip) |
| 21 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 22 | expression | `MaterialExpressionClamp` | Clamp |  |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input MipBias/Level (Scalar) | MipBias/Level |
| 25 | expression | `MaterialExpressionFrac` | Frac |  |
| 26 | expression | `MaterialExpressionFloor` | Floor |  |
| 27 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 28 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 29 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 30 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Use Mip Bias (T) Level (F) (StaticBool) | Use Mip Bias (T) Level (F) |
| 32 | function_output | `MaterialExpressionFunctionOutput` | Output UV Center | UV Center |
| 33 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugFloat2Values |  |
| 34 | expression | `MaterialExpressionDivide` | Divide |  |
| 35 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 36 | expression | `MaterialExpressionFrac` | Frac |  |
| 37 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [28] Switch output 0

### [2] Append

- `A` <= [16] Input Number of Columns (Scalar) output 0
- `B` <= [17] Input Number of Rows (Scalar) output 0

### [3] Texture Sample

- `Coordinates` <= [5] Add output 0
- `TextureObject` <= [13] Input Texture (Texture2D) output 0
- `MipValue` <= [24] Input MipBias/Level (Scalar) output 0

### [4] Divide

- `A` <= [15] Input UVs (Vector2) output 0
- `B` <= [2] Append output 0

### [5] Add

- `A` <= [4] Divide output 0
- `B` <= [9] Divide output 0

### [6] Multiply

- `A` <= [16] Input Number of Columns (Scalar) output 0
- `B` <= [17] Input Number of Rows (Scalar) output 0

### [7] Append

- `A` <= [6] Multiply output 0
- `B` <= [17] Input Number of Rows (Scalar) output 0

### [8] Multiply

- `A` <= [7] Append output 0
- `B` <= [11] Append output 0

### [9] Divide

- `A` <= [26] Floor output 0
- `B` <= [35] Reroute Node (reroutes wires) output 0

### [11] Append

- `A` <= [25] Frac output 0
- `B` <= [25] Frac output 0

### [13] Input Texture (Texture2D)

- `Preview` <= [12] Texture Object output 0

### [14] Input Animation  Phase (0-1) (Scalar)

- `Preview` <= [10] Time output 0

### [15] Input UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [18] Output UVs

- `A` <= [5] Add output 0

### [20] Input Clamp Anim (see tooltip) (StaticBool)

- `Preview` <= [19] Static Bool (False) output 0

### [21] Switch

- `A` <= [22] Clamp output 0
- `B` <= [14] Input Animation  Phase (0-1) (Scalar) output 0
- `Value` <= [20] Input Clamp Anim (see tooltip) (StaticBool) output 0

### [22] Clamp

- `Input` <= [14] Input Animation  Phase (0-1) (Scalar) output 0

### [23] Output Alpha

- `A` <= [29] Switch output 0

### [25] Frac

- `Input` <= [21] Switch output 0

### [26] Floor

- `Input` <= [8] Multiply output 0

### [27] Texture Sample

- `Coordinates` <= [5] Add output 0
- `TextureObject` <= [13] Input Texture (Texture2D) output 0
- `MipValue` <= [24] Input MipBias/Level (Scalar) output 0

### [28] Switch

- `A` <= [3] Texture Sample output 0
- `B` <= [27] Texture Sample output 0
- `Value` <= [31] Input Use Mip Bias (T) Level (F) (StaticBool) output 0

### [29] Switch

- `A` <= [3] Texture Sample output 4
- `B` <= [27] Texture Sample output 4
- `Value` <= [31] Input Use Mip Bias (T) Level (F) (StaticBool) output 0

### [31] Input Use Mip Bias (T) Level (F) (StaticBool)

- `Preview` <= [30] Static Bool (True) output 0

### [32] Output UV Center

- `A` <= [36] Frac output 0

### [34] Divide

- `A` <= [37] Add(,0.5) output 0
- `B` <= [35] Reroute Node (reroutes wires) output 0

### [35] Reroute Node (reroutes wires)

- `Input` <= [2] Append output 0

### [36] Frac

- `Input` <= [34] Divide output 0

### [37] Add(,0.5)

- `A` <= [26] Floor output 0

## Connection List

- [28] Switch output 0 -> Output Result.A
- [16] Input Number of Columns (Scalar) output 0 -> Append.A
- [17] Input Number of Rows (Scalar) output 0 -> Append.B
- [5] Add output 0 -> Texture Sample.Coordinates
- [13] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [24] Input MipBias/Level (Scalar) output 0 -> Texture Sample.MipValue
- [15] Input UVs (Vector2) output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [4] Divide output 0 -> Add.A
- [9] Divide output 0 -> Add.B
- [16] Input Number of Columns (Scalar) output 0 -> Multiply.A
- [17] Input Number of Rows (Scalar) output 0 -> Multiply.B
- [6] Multiply output 0 -> Append.A
- [17] Input Number of Rows (Scalar) output 0 -> Append.B
- [7] Append output 0 -> Multiply.A
- [11] Append output 0 -> Multiply.B
- [26] Floor output 0 -> Divide.A
- [35] Reroute Node (reroutes wires) output 0 -> Divide.B
- [25] Frac output 0 -> Append.A
- [25] Frac output 0 -> Append.B
- [12] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [10] Time output 0 -> Input Animation  Phase (0-1) (Scalar).Preview
- [1] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [5] Add output 0 -> Output UVs.A
- [19] Static Bool (False) output 0 -> Input Clamp Anim (see tooltip) (StaticBool).Preview
- [22] Clamp output 0 -> Switch.A
- [14] Input Animation  Phase (0-1) (Scalar) output 0 -> Switch.B
- [20] Input Clamp Anim (see tooltip) (StaticBool) output 0 -> Switch.Value
- [14] Input Animation  Phase (0-1) (Scalar) output 0 -> Clamp.Input
- [29] Switch output 0 -> Output Alpha.A
- [21] Switch output 0 -> Frac.Input
- [8] Multiply output 0 -> Floor.Input
- [5] Add output 0 -> Texture Sample.Coordinates
- [13] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [24] Input MipBias/Level (Scalar) output 0 -> Texture Sample.MipValue
- [3] Texture Sample output 0 -> Switch.A
- [27] Texture Sample output 0 -> Switch.B
- [31] Input Use Mip Bias (T) Level (F) (StaticBool) output 0 -> Switch.Value
- [3] Texture Sample output 4 -> Switch.A
- [27] Texture Sample output 4 -> Switch.B
- [31] Input Use Mip Bias (T) Level (F) (StaticBool) output 0 -> Switch.Value
- [30] Static Bool (True) output 0 -> Input Use Mip Bias (T) Level (F) (StaticBool).Preview
- [36] Frac output 0 -> Output UV Center.A
- [37] Add(,0.5) output 0 -> Divide.A
- [35] Reroute Node (reroutes wires) output 0 -> Divide.B
- [2] Append output 0 -> Reroute Node (reroutes wires).Input
- [34] Divide output 0 -> Frac.Input
- [26] Floor output 0 -> Add(,0.5).A

## Output Trace Roots

- `Result` is driven by [28] Switch
- `UVs` is driven by [5] Add
- `Alpha` is driven by [29] Switch
- `UV Center` is driven by [36] Frac

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
