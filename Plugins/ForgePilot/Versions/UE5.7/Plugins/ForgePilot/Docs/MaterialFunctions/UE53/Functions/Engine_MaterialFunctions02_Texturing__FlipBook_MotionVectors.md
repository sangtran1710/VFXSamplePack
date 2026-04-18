# FlipBook_MotionVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_MotionVectors.FlipBook_MotionVectors`
- Category: Engine_MaterialFunctions02/Texturing
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Check texture inputs and motion-vector conventions before wiring; wrong pins can compile but look incorrect.
- Inspect status: PASS

## Description

Flipbook with additional options for Motion Vectors. Animates a texture in a flipbook like fashion. 

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Animation  Phase (0-1)` [FunctionInput_Scalar] - Apply a "frac" material node to the input value if time is used to control the animation phase.
- `UVs` [FunctionInput_Vector2] - Insert the texture coordinate of your choice.
- `Number of Columns X` [FunctionInput_Scalar] - Enter the number of images horizontally across the texture.
- `Number of Rows Y` [FunctionInput_Scalar] - Enter the number of images vertically across the texture.
- `Motion Vector Texture` [FunctionInput_Texture2D] - This texture should be linear and in the 0-1 color space. Internally it is bias-scaled into the -1 to 1 range.
- `Phase to Debug` [FunctionInput_Scalar] - If 0, Debug is Disable. If 1, Debug Forward Motion Vectors. If 2, debug Reverse Motion vectors.
- `Motion Vector Intensity` [FunctionInput_Scalar] - This texture should be linear and in the 0-1 color space. Internally it is bias-scaled into the -1 to 1 range.
- `Double Apply Motion Vectors` [FunctionInput_StaticBool] - This option is more expensive but higher quality. It uses a double dependent texture read (a texture fed into UVs twice) in order to 'update' the motion vectors so they can maintain their velocities through frame transition more accurately.
- `Use Custom UVs` [FunctionInput_StaticBool]
- `Custom UV Input` [FunctionInput_Vector2]
- `Custom UV2 Input` [FunctionInput_Vector2]

## Outputs

- `Result`
- `Alpha`
- `Blend Phase`
- `Forward Motion Vectors`
- `CustomUV Output`
- `CustomUV2 Output`

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
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionTime` | Time |  |
| 10 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Animation  Phase (0-1) (Scalar) | Animation  Phase (0-1) |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Number of Columns X (Scalar) | Number of Columns X |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Number of Rows Y (Scalar) | Number of Rows Y |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 17 | expression | `MaterialExpressionFrac` | Frac |  |
| 18 | expression | `MaterialExpressionFloor` | Floor |  |
| 19 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 20 | expression | `MaterialExpressionFrac` | Frac |  |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Blend Phase | Blend Phase |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Motion Vector Texture (Texture2D) | Motion Vector Texture |
| 23 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 24 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 25 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 26 | expression | `MaterialExpressionDivide` | Divide |  |
| 27 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 30 | expression | `MaterialExpressionAdd` | Add |  |
| 31 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 32 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 33 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 34 | expression | `MaterialExpressionAdd` | Add |  |
| 35 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 36 | function_input | `MaterialExpressionFunctionInput` | Input Phase to Debug (Scalar) | Phase to Debug |
| 37 | expression | `MaterialExpressionFloor` | Floor |  |
| 38 | expression | `MaterialExpressionMin` | Min(,1) |  |
| 39 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 40 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 41 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,) |  |
| 42 | expression | `MaterialExpressionClamp` | Clamp |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input Motion Vector Intensity (Scalar) | Motion Vector Intensity |
| 45 | function_output | `MaterialExpressionFunctionOutput` | Output Forward Motion Vectors | Forward Motion Vectors |
| 46 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 47 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 48 | expression | `MaterialExpressionDivide` | Divide |  |
| 49 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 50 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 51 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 52 | expression | `MaterialExpressionDivide` | Divide |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 54 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input Double Apply Motion Vectors (StaticBool) | Double Apply Motion Vectors |
| 56 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 57 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 58 | expression | `MaterialExpressionAdd` | Add |  |
| 59 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 60 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 61 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 62 | function_input | `MaterialExpressionFunctionInput` | Input Use Custom UVs (StaticBool) | Use Custom UVs |
| 63 | expression | `MaterialExpressionAppendVector` | Append |  |
| 64 | expression | `MaterialExpressionDivide` | Divide |  |
| 65 | expression | `MaterialExpressionFloor` | Floor |  |
| 66 | expression | `MaterialExpressionDivide` | Divide |  |
| 67 | expression | `MaterialExpressionAppendVector` | Append |  |
| 68 | expression | `MaterialExpressionDivide` | Divide |  |
| 69 | expression | `MaterialExpressionFloor` | Floor |  |
| 70 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 71 | function_output | `MaterialExpressionFunctionOutput` | Output CustomUV Output | CustomUV Output |
| 72 | function_input | `MaterialExpressionFunctionInput` | Input Custom UV Input (Vector2) | Custom UV Input |
| 73 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 74 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 75 | function_output | `MaterialExpressionFunctionOutput` | Output CustomUV2 Output | CustomUV2 Output |
| 76 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 77 | function_input | `MaterialExpressionFunctionInput` | Input Custom UV2 Input (Vector2) | Custom UV2 Input |
| 78 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 79 | expression | `MaterialExpressionMin` | Min(,1) |  |
| 80 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 81 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 82 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 83 | expression | `MaterialExpressionDivide` | Divide |  |
| 84 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 85 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 86 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 87 | expression | `MaterialExpressionConstant2Vector` | -1,1 |  |
| 88 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 89 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 90 | expression | `MaterialExpressionScalarParameter` | Param (0) 'TestAlpha' |  |
| 91 | expression | `MaterialExpressionConstant2Vector` | -1,1 |  |
| 92 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 93 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 94 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 95 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 96 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 97 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [33] Lerp output 0

### [2] Append

- `A` <= [14] Input Number of Columns X (Scalar) output 0
- `B` <= [15] Input Number of Rows Y (Scalar) output 0

### [3] Texture Sample

- `Coordinates` <= [57] Switch output 0
- `TextureObject` <= [11] Input Texture (Texture2D) output 0

### [4] Divide

- `A` <= [13] Input UVs (Vector2) output 0
- `B` <= [2] Append output 0

### [5] Add

- `A` <= [4] Divide output 0
- `B` <= [8] Divide output 0

### [6] Multiply

- `A` <= [14] Input Number of Columns X (Scalar) output 0
- `B` <= [15] Input Number of Rows Y (Scalar) output 0

### [7] Multiply

- `A` <= [6] Multiply output 0
- `B` <= [79] Min(,1) output 0

### [8] Divide

- `A` <= [63] Append output 0
- `B` <= [2] Append output 0

### [11] Input Texture (Texture2D)

- `Preview` <= [10] Texture Object output 0

### [12] Input Animation  Phase (0-1) (Scalar)

- `Preview` <= [9] Time output 0

### [13] Input UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [16] Output Alpha

- `A` <= [35] Lerp output 0

### [17] Frac

- `Input` <= [12] Input Animation  Phase (0-1) (Scalar) output 0

### [18] Floor

- `Input` <= [7] Multiply output 0

### [19] Mask ( R )

- `Input` <= [7] Multiply output 0

### [20] Frac

- `Input` <= [19] Mask ( R ) output 0

### [21] Output Blend Phase

- `A` <= [20] Frac output 0

### [22] Input Motion Vector Texture (Texture2D)

- `Preview` <= [23] Texture Object output 0

### [24] Mask ( R G )

- `Input` <= [25] Texture Sample output 0

### [25] Texture Sample

- `Coordinates` <= [5] Add output 0
- `TextureObject` <= [22] Input Motion Vector Texture (Texture2D) output 0

### [26] Divide

- `A` <= [94] ConstantBiasScale output 0
- `B` <= [84] Reroute Node (reroutes wires) output 0

### [27] Multiply

- `A` <= [21] Output Blend Phase output 0
- `B` <= [85] Multiply output 0

### [28] Multiply

- `A` <= [29] 1-x output 0
- `B` <= [86] Multiply output 0

### [29] 1-x

- `Input` <= [20] Frac output 0

### [30] Add

- `A` <= [74] Switch output 0
- `B` <= [27] Multiply output 0

### [31] Subtract

- `A` <= [76] Switch output 0
- `B` <= [28] Multiply output 0

### [32] Texture Sample

- `Coordinates` <= [54] Switch output 0
- `TextureObject` <= [11] Input Texture (Texture2D) output 0

### [33] Lerp

- `A` <= [3] Texture Sample output 0
- `B` <= [32] Texture Sample output 0
- `Alpha` <= [39] Lerp output 0

### [34] Add

- `A` <= [4] Divide output 0
- `B` <= [66] Divide output 0

### [35] Lerp

- `A` <= [3] Texture Sample output 4
- `B` <= [32] Texture Sample output 4
- `Alpha` <= [39] Lerp output 0

### [37] Floor

- `Input` <= [42] Clamp output 0

### [38] Min(,1)

- `A` <= [37] Floor output 0

### [39] Lerp

- `A` <= [21] Output Blend Phase output 0
- `B` <= [41] Lerp(0,1,) output 0
- `Alpha` <= [38] Min(,1) output 0

### [40] Subtract(,1)

- `A` <= [37] Floor output 0

### [41] Lerp(0,1,)

- `Alpha` <= [40] Subtract(,1) output 0

### [42] Clamp

- `Input` <= [36] Input Phase to Debug (Scalar) output 0

### [43] Multiply

- `A` <= [88] Lerp output 0
- `B` <= [44] Input Motion Vector Intensity (Scalar) output 0

### [45] Output Forward Motion Vectors

- `A` <= [78] Lerp output 0

### [46] Mask ( R G )

- `Input` <= [96] ConstantBiasScale output 0

### [47] Texture Sample

- `Coordinates` <= [30] Add output 0
- `TextureObject` <= [22] Input Motion Vector Texture (Texture2D) output 0

### [48] Divide

- `A` <= [46] Mask ( R G ) output 0
- `B` <= [2] Append output 0

### [49] Multiply

- `A` <= [92] Multiply output 0
- `B` <= [21] Output Blend Phase output 0

### [50] Mask ( R G )

- `Input` <= [97] ConstantBiasScale output 0

### [51] Texture Sample

- `Coordinates` <= [31] Subtract output 0
- `TextureObject` <= [22] Input Motion Vector Texture (Texture2D) output 0

### [52] Divide

- `A` <= [50] Mask ( R G ) output 0
- `B` <= [2] Append output 0

### [53] Multiply

- `A` <= [93] Multiply output 0
- `B` <= [29] 1-x output 0

### [54] Switch

- `A` <= [59] Subtract output 0
- `B` <= [31] Subtract output 0
- `Value` <= [55] Input Double Apply Motion Vectors (StaticBool) output 0

### [55] Input Double Apply Motion Vectors (StaticBool)

- `Preview` <= [56] Static Bool (False) output 0

### [57] Switch

- `A` <= [58] Add output 0
- `B` <= [30] Add output 0
- `Value` <= [55] Input Double Apply Motion Vectors (StaticBool) output 0

### [58] Add

- `A` <= [74] Switch output 0
- `B` <= [49] Multiply output 0

### [59] Subtract

- `A` <= [76] Switch output 0
- `B` <= [53] Multiply output 0

### [60] Multiply

- `A` <= [44] Input Motion Vector Intensity (Scalar) output 0
- `B` <= [48] Divide output 0

### [61] Multiply

- `A` <= [44] Input Motion Vector Intensity (Scalar) output 0
- `B` <= [52] Divide output 0

### [62] Input Use Custom UVs (StaticBool)

- `Preview` <= [73] Static Bool (False) output 0

### [63] Append

- `A` <= [18] Floor output 0
- `B` <= [65] Floor output 0

### [64] Divide

- `A` <= [18] Floor output 0
- `B` <= [15] Input Number of Rows Y (Scalar) output 0

### [65] Floor

- `Input` <= [64] Divide output 0

### [66] Divide

- `A` <= [67] Append output 0
- `B` <= [2] Append output 0

### [67] Append

- `A` <= [70] Add(,1) output 0
- `B` <= [69] Floor output 0

### [68] Divide

- `A` <= [70] Add(,1) output 0
- `B` <= [15] Input Number of Rows Y (Scalar) output 0

### [69] Floor

- `Input` <= [68] Divide output 0

### [70] Add(,1)

- `A` <= [18] Floor output 0

### [71] Output CustomUV Output

- `A` <= [5] Add output 0

### [74] Switch

- `A` <= [72] Input Custom UV Input (Vector2) output 0
- `B` <= [5] Add output 0
- `Value` <= [62] Input Use Custom UVs (StaticBool) output 0

### [75] Output CustomUV2 Output

- `A` <= [34] Add output 0

### [76] Switch

- `A` <= [77] Input Custom UV2 Input (Vector2) output 0
- `B` <= [34] Add output 0
- `Value` <= [62] Input Use Custom UVs (StaticBool) output 0

### [78] Lerp

- `A` <= [24] Mask ( R G ) output 0
- `B` <= [46] Mask ( R G ) output 0
- `Alpha` <= [38] Min(,1) output 0

### [79] Min(,1)

- `A` <= [17] Frac output 0

### [80] Mask ( R G )

- `Input` <= [81] Texture Sample output 0

### [81] Texture Sample

- `Coordinates` <= [34] Add output 0
- `TextureObject` <= [22] Input Motion Vector Texture (Texture2D) output 0

### [82] Multiply

- `A` <= [89] Lerp output 0
- `B` <= [44] Input Motion Vector Intensity (Scalar) output 0

### [83] Divide

- `A` <= [95] ConstantBiasScale output 0
- `B` <= [84] Reroute Node (reroutes wires) output 0

### [84] Reroute Node (reroutes wires)

- `Input` <= [2] Append output 0

### [85] Multiply

- `A` <= [43] Multiply output 0
- `B` <= [87] -1,1 output 0

### [86] Multiply

- `A` <= [82] Multiply output 0
- `B` <= [87] -1,1 output 0

### [88] Lerp

- `A` <= [26] Divide output 0
- `B` <= [83] Divide output 0
- `Alpha` <= [90] Param (0) 'TestAlpha' output 0

### [89] Lerp

- `A` <= [83] Divide output 0
- `B` <= [26] Divide output 0
- `Alpha` <= [90] Param (0) 'TestAlpha' output 0

### [92] Multiply

- `A` <= [60] Multiply output 0
- `B` <= [91] -1,1 output 0

### [93] Multiply

- `A` <= [61] Multiply output 0
- `B` <= [91] -1,1 output 0

### [94] ConstantBiasScale

- `Input` <= [24] Mask ( R G ) output 0

### [95] ConstantBiasScale

- `Input` <= [80] Mask ( R G ) output 0

### [96] ConstantBiasScale

- `Input` <= [47] Texture Sample output 0

### [97] ConstantBiasScale

- `Input` <= [51] Texture Sample output 0

## Connection List

- [33] Lerp output 0 -> Output Result.A
- [14] Input Number of Columns X (Scalar) output 0 -> Append.A
- [15] Input Number of Rows Y (Scalar) output 0 -> Append.B
- [57] Switch output 0 -> Texture Sample.Coordinates
- [11] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [13] Input UVs (Vector2) output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [4] Divide output 0 -> Add.A
- [8] Divide output 0 -> Add.B
- [14] Input Number of Columns X (Scalar) output 0 -> Multiply.A
- [15] Input Number of Rows Y (Scalar) output 0 -> Multiply.B
- [6] Multiply output 0 -> Multiply.A
- [79] Min(,1) output 0 -> Multiply.B
- [63] Append output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [10] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [9] Time output 0 -> Input Animation  Phase (0-1) (Scalar).Preview
- [1] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [35] Lerp output 0 -> Output Alpha.A
- [12] Input Animation  Phase (0-1) (Scalar) output 0 -> Frac.Input
- [7] Multiply output 0 -> Floor.Input
- [7] Multiply output 0 -> Mask ( R ).Input
- [19] Mask ( R ) output 0 -> Frac.Input
- [20] Frac output 0 -> Output Blend Phase.A
- [23] Texture Object output 0 -> Input Motion Vector Texture (Texture2D).Preview
- [25] Texture Sample output 0 -> Mask ( R G ).Input
- [5] Add output 0 -> Texture Sample.Coordinates
- [22] Input Motion Vector Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [94] ConstantBiasScale output 0 -> Divide.A
- [84] Reroute Node (reroutes wires) output 0 -> Divide.B
- [21] Output Blend Phase output 0 -> Multiply.A
- [85] Multiply output 0 -> Multiply.B
- [29] 1-x output 0 -> Multiply.A
- [86] Multiply output 0 -> Multiply.B
- [20] Frac output 0 -> 1-x.Input
- [74] Switch output 0 -> Add.A
- [27] Multiply output 0 -> Add.B
- [76] Switch output 0 -> Subtract.A
- [28] Multiply output 0 -> Subtract.B
- [54] Switch output 0 -> Texture Sample.Coordinates
- [11] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [3] Texture Sample output 0 -> Lerp.A
- [32] Texture Sample output 0 -> Lerp.B
- [39] Lerp output 0 -> Lerp.Alpha
- [4] Divide output 0 -> Add.A
- [66] Divide output 0 -> Add.B
- [3] Texture Sample output 4 -> Lerp.A
- [32] Texture Sample output 4 -> Lerp.B
- [39] Lerp output 0 -> Lerp.Alpha
- [42] Clamp output 0 -> Floor.Input
- [37] Floor output 0 -> Min(,1).A
- [21] Output Blend Phase output 0 -> Lerp.A
- [41] Lerp(0,1,) output 0 -> Lerp.B
- [38] Min(,1) output 0 -> Lerp.Alpha
- [37] Floor output 0 -> Subtract(,1).A
- [40] Subtract(,1) output 0 -> Lerp(0,1,).Alpha
- [36] Input Phase to Debug (Scalar) output 0 -> Clamp.Input
- [88] Lerp output 0 -> Multiply.A
- [44] Input Motion Vector Intensity (Scalar) output 0 -> Multiply.B
- [78] Lerp output 0 -> Output Forward Motion Vectors.A
- [96] ConstantBiasScale output 0 -> Mask ( R G ).Input
- [30] Add output 0 -> Texture Sample.Coordinates
- [22] Input Motion Vector Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [46] Mask ( R G ) output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [92] Multiply output 0 -> Multiply.A
- [21] Output Blend Phase output 0 -> Multiply.B
- [97] ConstantBiasScale output 0 -> Mask ( R G ).Input
- [31] Subtract output 0 -> Texture Sample.Coordinates
- [22] Input Motion Vector Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [50] Mask ( R G ) output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [93] Multiply output 0 -> Multiply.A
- [29] 1-x output 0 -> Multiply.B
- [59] Subtract output 0 -> Switch.A
- [31] Subtract output 0 -> Switch.B
- [55] Input Double Apply Motion Vectors (StaticBool) output 0 -> Switch.Value
- [56] Static Bool (False) output 0 -> Input Double Apply Motion Vectors (StaticBool).Preview
- [58] Add output 0 -> Switch.A
- [30] Add output 0 -> Switch.B
- [55] Input Double Apply Motion Vectors (StaticBool) output 0 -> Switch.Value
- [74] Switch output 0 -> Add.A
- [49] Multiply output 0 -> Add.B
- [76] Switch output 0 -> Subtract.A
- [53] Multiply output 0 -> Subtract.B
- [44] Input Motion Vector Intensity (Scalar) output 0 -> Multiply.A
- [48] Divide output 0 -> Multiply.B
- [44] Input Motion Vector Intensity (Scalar) output 0 -> Multiply.A
- [52] Divide output 0 -> Multiply.B
- [73] Static Bool (False) output 0 -> Input Use Custom UVs (StaticBool).Preview
- [18] Floor output 0 -> Append.A
- [65] Floor output 0 -> Append.B
- [18] Floor output 0 -> Divide.A
- [15] Input Number of Rows Y (Scalar) output 0 -> Divide.B
- [64] Divide output 0 -> Floor.Input
- [67] Append output 0 -> Divide.A
- [2] Append output 0 -> Divide.B
- [70] Add(,1) output 0 -> Append.A
- [69] Floor output 0 -> Append.B
- [70] Add(,1) output 0 -> Divide.A
- [15] Input Number of Rows Y (Scalar) output 0 -> Divide.B
- [68] Divide output 0 -> Floor.Input
- [18] Floor output 0 -> Add(,1).A
- [5] Add output 0 -> Output CustomUV Output.A
- [72] Input Custom UV Input (Vector2) output 0 -> Switch.A
- [5] Add output 0 -> Switch.B
- [62] Input Use Custom UVs (StaticBool) output 0 -> Switch.Value
- [34] Add output 0 -> Output CustomUV2 Output.A
- [77] Input Custom UV2 Input (Vector2) output 0 -> Switch.A
- [34] Add output 0 -> Switch.B
- [62] Input Use Custom UVs (StaticBool) output 0 -> Switch.Value
- [24] Mask ( R G ) output 0 -> Lerp.A
- [46] Mask ( R G ) output 0 -> Lerp.B
- [38] Min(,1) output 0 -> Lerp.Alpha
- [17] Frac output 0 -> Min(,1).A
- [81] Texture Sample output 0 -> Mask ( R G ).Input
- [34] Add output 0 -> Texture Sample.Coordinates
- [22] Input Motion Vector Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [89] Lerp output 0 -> Multiply.A
- [44] Input Motion Vector Intensity (Scalar) output 0 -> Multiply.B
- [95] ConstantBiasScale output 0 -> Divide.A
- [84] Reroute Node (reroutes wires) output 0 -> Divide.B
- [2] Append output 0 -> Reroute Node (reroutes wires).Input
- [43] Multiply output 0 -> Multiply.A
- [87] -1,1 output 0 -> Multiply.B
- [82] Multiply output 0 -> Multiply.A
- [87] -1,1 output 0 -> Multiply.B
- [26] Divide output 0 -> Lerp.A
- [83] Divide output 0 -> Lerp.B
- [90] Param (0) 'TestAlpha' output 0 -> Lerp.Alpha
- [83] Divide output 0 -> Lerp.A
- [26] Divide output 0 -> Lerp.B
- [90] Param (0) 'TestAlpha' output 0 -> Lerp.Alpha
- [60] Multiply output 0 -> Multiply.A
- [91] -1,1 output 0 -> Multiply.B
- [61] Multiply output 0 -> Multiply.A
- [91] -1,1 output 0 -> Multiply.B
- [24] Mask ( R G ) output 0 -> ConstantBiasScale.Input
- [80] Mask ( R G ) output 0 -> ConstantBiasScale.Input
- [47] Texture Sample output 0 -> ConstantBiasScale.Input
- [51] Texture Sample output 0 -> ConstantBiasScale.Input

## Output Trace Roots

- `Result` is driven by [33] Lerp
- `Alpha` is driven by [35] Lerp
- `Blend Phase` is driven by [20] Frac
- `Forward Motion Vectors` is driven by [78] Lerp
- `CustomUV Output` is driven by [5] Add
- `CustomUV2 Output` is driven by [34] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
