# TextureBomb_SingleSample_POM

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TextureBomb_SingleSample_POM.TextureBomb_SingleSample_POM`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `UVs` [FunctionInput_Vector2]
- `Cell Index` [FunctionInput_Vector2]
- `Offset Strength` [FunctionInput_Scalar]
- `TexObject` [FunctionInput_Texture2D]
- `IsNormal?` [FunctionInput_StaticBool]
- `HeightTex` [FunctionInput_Texture2D]
- `HeightRatio` [FunctionInput_Scalar]

## Outputs

- `UVs`
- `Texture Sample`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionConstant2Vector` | 1,-1 |  |
| 3 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 9 | expression | `MaterialExpressionClamp` | Clamp |  |
| 10 | expression | `MaterialExpressionCeil` | Ceil |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 12 | custom | `MaterialExpressionCustom` | Pseudo Random |  |
| 13 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 16 | expression | `MaterialExpressionDDX` | DDX |  |
| 17 | expression | `MaterialExpressionDDY` | DDY |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Texture Sample | Texture Sample |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Cell Index (Vector2) | Cell Index |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Offset Strength (Scalar) | Offset Strength |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input TexObject (Texture2D) | TexObject |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input IsNormal? (StaticBool) | IsNormal? |
| 25 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 28 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 29 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 30 | expression | `MaterialExpressionAppendVector` | Append |  |
| 31 | expression | `MaterialExpressionConstant` | 1 |  |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 33 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 34 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 35 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 36 | expression | `MaterialExpressionConstant2Vector` | -1,1 |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 39 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 42 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 43 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 44 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 45 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 46 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 47 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 48 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 49 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 50 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 51 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 52 | function_call | `MaterialExpressionMaterialFunctionCall` | Parallax_For_Bomb |  |
| 53 | function_input | `MaterialExpressionFunctionInput` | Input HeightTex (Texture2D) | HeightTex |
| 54 | function_input | `MaterialExpressionFunctionInput` | Input HeightRatio (Scalar) | HeightRatio |
| 55 | function_call | `MaterialExpressionMaterialFunctionCall` | DitherTemporalAA |  |
| 56 | expression | `MaterialExpressionLinearInterpolate` | Lerp(1,1.2,) |  |
| 57 | expression | `MaterialExpressionMultiply` | Multiply(,64) |  |
| 58 | expression | `MaterialExpressionConstant4Vector` | 1,0,0,0 |  |
| 59 | expression | `MaterialExpressionConstant` | 1 |  |
| 60 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output UVs

- `A` <= [14] Add output 0

### [1] Multiply

- `A` <= [5] Lerp output 0
- `B` <= [19] Input UVs (Vector2) output 0

### [4] Mask ( G )

- `Input` <= [9] Clamp output 0

### [5] Lerp

- `A` <= [3] 1,1 output 0
- `B` <= [2] 1,-1 output 0
- `Alpha` <= [4] Mask ( G ) output 0

### [6] Mask ( R )

- `Input` <= [9] Clamp output 0

### [7] Lerp

- `A` <= [8] Swizzle output 1
- `B` <= [1] Multiply output 0
- `Alpha` <= [6] Mask ( R ) output 0

### [9] Clamp

- `Input` <= [10] Ceil output 0

### [10] Ceil

- `Input` <= [11] Subtract(,0.5) output 0

### [11] Subtract(,0.5)

- `A` <= [12] Pseudo Random output 0

### [13] Texture Sample

- `Coordinates` <= [52] Parallax_For_Bomb output 0
- `TextureObject` <= [23] Input TexObject (Texture2D) output 0
- `CoordinatesDX` <= [42] Lerp output 0
- `CoordinatesDY` <= [43] Lerp output 0

### [14] Add

- `A` <= [22] Multiply output 0
- `B` <= [7] Lerp output 0

### [16] DDX

- `Value` <= [19] Input UVs (Vector2) output 0

### [17] DDY

- `Value` <= [19] Input UVs (Vector2) output 0

### [18] Output Texture Sample

- `A` <= [29] Switch output 0

### [19] Input UVs (Vector2)

- `Preview` <= [15] TexCoord[0] output 0

### [22] Multiply

- `A` <= [12] Pseudo Random output 0
- `B` <= [21] Input Offset Strength (Scalar) output 0

### [24] Input IsNormal? (StaticBool)

- `Preview` <= [25] Static Bool (False) output 0

### [26] Multiply

- `A` <= [30] Append output 0
- `B` <= [13] Texture Sample output 0

### [27] Lerp

- `A` <= [28] Swizzle output 0
- `B` <= [26] Multiply output 0
- `Alpha` <= [6] Mask ( R ) output 0

### [29] Switch

- `A` <= [27] Lerp output 0
- `B` <= [13] Texture Sample output 0
- `Value` <= [24] Input IsNormal? (StaticBool) output 0

### [30] Append

- `A` <= [33] Lerp output 0
- `B` <= [31] 1 output 0

### [33] Lerp

- `A` <= [32] Swizzle output 1
- `B` <= [5] Lerp output 0
- `Alpha` <= [6] Mask ( R ) output 0

### [34] Lerp

- `A` <= [35] 1,1 output 0
- `B` <= [36] -1,1 output 0
- `Alpha` <= [6] Mask ( R ) output 0

### [37] Multiply

- `A` <= [1] Multiply output 0
- `B` <= [34] Lerp output 0

### [38] Multiply

- `A` <= [5] Lerp output 0
- `B` <= [34] Lerp output 0

### [39] Multiply

- `A` <= [16] DDX output 0
- `B` <= [41] Multiply(,1) output 0

### [40] Multiply

- `A` <= [17] DDY output 0
- `B` <= [41] Multiply(,1) output 0

### [41] Multiply(,1)

- `A` <= [33] Lerp output 0

### [42] Lerp

- `A` <= [44] Swizzle output 1
- `B` <= [39] Multiply output 0
- `Alpha` <= [46] Mask ( R ) output 0

### [43] Lerp

- `A` <= [45] Swizzle output 1
- `B` <= [40] Multiply output 0
- `Alpha` <= [46] Mask ( R ) output 0

### [46] Mask ( R )

- `Input` <= [6] Mask ( R ) output 0

### [47] Multiply

- `A` <= [60] Append output 0
- `B` <= [50] World Space to Tangent Space TransformVector output 0

### [49] Lerp

- `A` <= [48] Swizzle output 0
- `B` <= [47] Multiply output 0
- `Alpha` <= [6] Mask ( R ) output 0

### [50] World Space to Tangent Space TransformVector

- `Input` <= [51] Camera Vector output 0

### [56] Lerp(1,1.2,)

- `Alpha` <= [55] DitherTemporalAA output 0

### [57] Multiply(,64)

- `A` <= [56] Lerp(1,1.2,) output 0

### [60] Append

- `A` <= [5] Lerp output 0
- `B` <= [59] 1 output 0

## Connection List

- [14] Add output 0 -> Output UVs.A
- [5] Lerp output 0 -> Multiply.A
- [19] Input UVs (Vector2) output 0 -> Multiply.B
- [9] Clamp output 0 -> Mask ( G ).Input
- [3] 1,1 output 0 -> Lerp.A
- [2] 1,-1 output 0 -> Lerp.B
- [4] Mask ( G ) output 0 -> Lerp.Alpha
- [9] Clamp output 0 -> Mask ( R ).Input
- [8] Swizzle output 1 -> Lerp.A
- [1] Multiply output 0 -> Lerp.B
- [6] Mask ( R ) output 0 -> Lerp.Alpha
- [10] Ceil output 0 -> Clamp.Input
- [11] Subtract(,0.5) output 0 -> Ceil.Input
- [12] Pseudo Random output 0 -> Subtract(,0.5).A
- [52] Parallax_For_Bomb output 0 -> Texture Sample.Coordinates
- [23] Input TexObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [42] Lerp output 0 -> Texture Sample.CoordinatesDX
- [43] Lerp output 0 -> Texture Sample.CoordinatesDY
- [22] Multiply output 0 -> Add.A
- [7] Lerp output 0 -> Add.B
- [19] Input UVs (Vector2) output 0 -> DDX.Value
- [19] Input UVs (Vector2) output 0 -> DDY.Value
- [29] Switch output 0 -> Output Texture Sample.A
- [15] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [12] Pseudo Random output 0 -> Multiply.A
- [21] Input Offset Strength (Scalar) output 0 -> Multiply.B
- [25] Static Bool (False) output 0 -> Input IsNormal? (StaticBool).Preview
- [30] Append output 0 -> Multiply.A
- [13] Texture Sample output 0 -> Multiply.B
- [28] Swizzle output 0 -> Lerp.A
- [26] Multiply output 0 -> Lerp.B
- [6] Mask ( R ) output 0 -> Lerp.Alpha
- [27] Lerp output 0 -> Switch.A
- [13] Texture Sample output 0 -> Switch.B
- [24] Input IsNormal? (StaticBool) output 0 -> Switch.Value
- [33] Lerp output 0 -> Append.A
- [31] 1 output 0 -> Append.B
- [32] Swizzle output 1 -> Lerp.A
- [5] Lerp output 0 -> Lerp.B
- [6] Mask ( R ) output 0 -> Lerp.Alpha
- [35] 1,1 output 0 -> Lerp.A
- [36] -1,1 output 0 -> Lerp.B
- [6] Mask ( R ) output 0 -> Lerp.Alpha
- [1] Multiply output 0 -> Multiply.A
- [34] Lerp output 0 -> Multiply.B
- [5] Lerp output 0 -> Multiply.A
- [34] Lerp output 0 -> Multiply.B
- [16] DDX output 0 -> Multiply.A
- [41] Multiply(,1) output 0 -> Multiply.B
- [17] DDY output 0 -> Multiply.A
- [41] Multiply(,1) output 0 -> Multiply.B
- [33] Lerp output 0 -> Multiply(,1).A
- [44] Swizzle output 1 -> Lerp.A
- [39] Multiply output 0 -> Lerp.B
- [46] Mask ( R ) output 0 -> Lerp.Alpha
- [45] Swizzle output 1 -> Lerp.A
- [40] Multiply output 0 -> Lerp.B
- [46] Mask ( R ) output 0 -> Lerp.Alpha
- [6] Mask ( R ) output 0 -> Mask ( R ).Input
- [60] Append output 0 -> Multiply.A
- [50] World Space to Tangent Space TransformVector output 0 -> Multiply.B
- [48] Swizzle output 0 -> Lerp.A
- [47] Multiply output 0 -> Lerp.B
- [6] Mask ( R ) output 0 -> Lerp.Alpha
- [51] Camera Vector output 0 -> World Space to Tangent Space TransformVector.Input
- [55] DitherTemporalAA output 0 -> Lerp(1,1.2,).Alpha
- [56] Lerp(1,1.2,) output 0 -> Multiply(,64).A
- [5] Lerp output 0 -> Append.A
- [59] 1 output 0 -> Append.B

## Output Trace Roots

- `UVs` is driven by [14] Add
- `Texture Sample` is driven by [29] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


