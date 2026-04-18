# SubUV_Function_MipDerivative

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/SubUV_Function_MipDerivative.SubUV_Function_MipDerivative`
- Category: Engine_MaterialFunctions03/Texturing
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Check texture inputs and motion-vector conventions before wiring; wrong pins can compile but look incorrect.
- Inspect status: PASS

## Description

Allows SubUV blending of multiple texture frames in a single texture. This MipDerivative version is used to fix mipmap artifacts coming from tiling textures.

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `SubImages` [FunctionInput_Vector2]
- `UVs` [FunctionInput_Vector2]
- `Frame` [FunctionInput_Scalar]

## Outputs

- `RGB`
- `Alpha`
- `UVs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 3 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input SubImages (Vector2) | SubImages |
| 5 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 8 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Frame (Scalar) | Frame |
| 10 | expression | `MaterialExpressionFrac` | Frac |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionAdd` | Add(1,) |  |
| 13 | expression | `MaterialExpressionFmod` | Fmod |  |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 15 | expression | `MaterialExpressionFmod` | Fmod |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionFloor` | Floor |  |
| 19 | expression | `MaterialExpressionFloor` | Floor |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionAppendVector` | Append |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 28 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 30 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 32 | expression | `MaterialExpressionDDX` | DDX |  |
| 33 | expression | `MaterialExpressionDDY` | DDY |  |
| 34 | expression | `MaterialExpressionFrac` | Frac |  |
| 35 | expression | `MaterialExpressionMultiply` | Multiply(0,1) |  |
| 36 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output RGB

- `A` <= [28] Lerp output 0

### [1] Texture Sample

- `Coordinates` <= [26] Multiply output 0
- `TextureObject` <= [2] Input Texture (Texture2D) output 0
- `CoordinatesDX` <= [32] DDX output 0
- `CoordinatesDY` <= [33] DDY output 0

### [2] Input Texture (Texture2D)

- `Preview` <= [3] Texture Object output 0

### [6] Divide

- `A` <= [5] 1,1 output 0
- `B` <= [4] Input SubImages (Vector2) output 0

### [7] Input UVs (Vector2)

- `Preview` <= [8] TexCoord[0] output 0

### [10] Frac

- `Input` <= [9] Input Frame (Scalar) output 0

### [11] Subtract

- `A` <= [9] Input Frame (Scalar) output 0
- `B` <= [10] Frac output 0

### [12] Add(1,)

- `B` <= [11] Subtract output 0

### [13] Fmod

- `A` <= [11] Subtract output 0
- `B` <= [14] Mask ( R ) output 0

### [14] Mask ( R )

- `Input` <= [4] Input SubImages (Vector2) output 0

### [15] Fmod

- `A` <= [12] Add(1,) output 0
- `B` <= [14] Mask ( R ) output 0

### [16] Mask ( R )

- `Input` <= [6] Divide output 0

### [17] Multiply

- `A` <= [11] Subtract output 0
- `B` <= [16] Mask ( R ) output 0

### [18] Floor

- `Input` <= [17] Multiply output 0

### [19] Floor

- `Input` <= [20] Multiply output 0

### [20] Multiply

- `A` <= [12] Add(1,) output 0
- `B` <= [16] Mask ( R ) output 0

### [21] Append

- `A` <= [13] Fmod output 0
- `B` <= [18] Floor output 0

### [22] Append

- `A` <= [15] Fmod output 0
- `B` <= [19] Floor output 0

### [23] Add

- `A` <= [21] Append output 0
- `B` <= [34] Frac output 0

### [24] Add

- `A` <= [34] Frac output 0
- `B` <= [22] Append output 0

### [25] Multiply

- `A` <= [23] Add output 0
- `B` <= [6] Divide output 0

### [26] Multiply

- `A` <= [24] Add output 0
- `B` <= [6] Divide output 0

### [27] Texture Sample

- `Coordinates` <= [25] Multiply output 0
- `TextureObject` <= [2] Input Texture (Texture2D) output 0
- `CoordinatesDX` <= [32] DDX output 0
- `CoordinatesDY` <= [33] DDY output 0

### [28] Lerp

- `A` <= [27] Texture Sample output 0
- `B` <= [1] Texture Sample output 0
- `Alpha` <= [10] Frac output 0

### [29] Output Alpha

- `A` <= [30] Lerp output 0

### [30] Lerp

- `A` <= [27] Texture Sample output 4
- `B` <= [1] Texture Sample output 4
- `Alpha` <= [10] Frac output 0

### [31] Output UVs

- `A` <= [36] Divide output 0

### [32] DDX

- `Value` <= [31] Output UVs output 0

### [33] DDY

- `Value` <= [31] Output UVs output 0

### [34] Frac

- `Input` <= [7] Input UVs (Vector2) output 0

### [36] Divide

- `A` <= [7] Input UVs (Vector2) output 0
- `B` <= [4] Input SubImages (Vector2) output 0

## Connection List

- [28] Lerp output 0 -> Output RGB.A
- [26] Multiply output 0 -> Texture Sample.Coordinates
- [2] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [32] DDX output 0 -> Texture Sample.CoordinatesDX
- [33] DDY output 0 -> Texture Sample.CoordinatesDY
- [3] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [5] 1,1 output 0 -> Divide.A
- [4] Input SubImages (Vector2) output 0 -> Divide.B
- [8] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [9] Input Frame (Scalar) output 0 -> Frac.Input
- [9] Input Frame (Scalar) output 0 -> Subtract.A
- [10] Frac output 0 -> Subtract.B
- [11] Subtract output 0 -> Add(1,).B
- [11] Subtract output 0 -> Fmod.A
- [14] Mask ( R ) output 0 -> Fmod.B
- [4] Input SubImages (Vector2) output 0 -> Mask ( R ).Input
- [12] Add(1,) output 0 -> Fmod.A
- [14] Mask ( R ) output 0 -> Fmod.B
- [6] Divide output 0 -> Mask ( R ).Input
- [11] Subtract output 0 -> Multiply.A
- [16] Mask ( R ) output 0 -> Multiply.B
- [17] Multiply output 0 -> Floor.Input
- [20] Multiply output 0 -> Floor.Input
- [12] Add(1,) output 0 -> Multiply.A
- [16] Mask ( R ) output 0 -> Multiply.B
- [13] Fmod output 0 -> Append.A
- [18] Floor output 0 -> Append.B
- [15] Fmod output 0 -> Append.A
- [19] Floor output 0 -> Append.B
- [21] Append output 0 -> Add.A
- [34] Frac output 0 -> Add.B
- [34] Frac output 0 -> Add.A
- [22] Append output 0 -> Add.B
- [23] Add output 0 -> Multiply.A
- [6] Divide output 0 -> Multiply.B
- [24] Add output 0 -> Multiply.A
- [6] Divide output 0 -> Multiply.B
- [25] Multiply output 0 -> Texture Sample.Coordinates
- [2] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [32] DDX output 0 -> Texture Sample.CoordinatesDX
- [33] DDY output 0 -> Texture Sample.CoordinatesDY
- [27] Texture Sample output 0 -> Lerp.A
- [1] Texture Sample output 0 -> Lerp.B
- [10] Frac output 0 -> Lerp.Alpha
- [30] Lerp output 0 -> Output Alpha.A
- [27] Texture Sample output 4 -> Lerp.A
- [1] Texture Sample output 4 -> Lerp.B
- [10] Frac output 0 -> Lerp.Alpha
- [36] Divide output 0 -> Output UVs.A
- [31] Output UVs output 0 -> DDX.Value
- [31] Output UVs output 0 -> DDY.Value
- [7] Input UVs (Vector2) output 0 -> Frac.Input
- [7] Input UVs (Vector2) output 0 -> Divide.A
- [4] Input SubImages (Vector2) output 0 -> Divide.B

## Output Trace Roots

- `RGB` is driven by [28] Lerp
- `Alpha` is driven by [30] Lerp
- `UVs` is driven by [36] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
