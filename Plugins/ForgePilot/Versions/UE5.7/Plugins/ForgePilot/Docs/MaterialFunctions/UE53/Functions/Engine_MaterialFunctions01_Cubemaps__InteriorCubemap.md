# InteriorCubemap

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Cubemaps/InteriorCubemap.InteriorCubemap`
- Category: Engine_MaterialFunctions01/Cubemaps
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Interior Cubemap conforms a cubemap to be in the form of a box shape inset from the surface UVs. It is useful for giving parallax to rooms visible through windows on city buildings.

## Inputs

- `Tiling` [FunctionInput_Vector2]
- `UVs` [FunctionInput_Vector2]
- `Randomize Rotation` [FunctionInput_StaticBool]

## Outputs

- `UVW`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVW | UVW |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 3 | expression | `MaterialExpressionConstant3Vector` | -1,1,1 |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 10 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 13 | expression | `MaterialExpressionFrac` | Frac |  |
| 14 | expression | `MaterialExpressionConstant2Vector` | 2,-2 |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionConstant2Vector` | 1,-1 |  |
| 17 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 18 | expression | `MaterialExpressionAppendVector` | Append |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Tiling (Vector2) | Tiling |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionConstant` | -1 |  |
| 23 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 24 | expression | `MaterialExpressionMin` | Min |  |
| 25 | expression | `MaterialExpressionMin` | Min |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Randomize Rotation (StaticBool) | Randomize Rotation |
| 28 | expression | `MaterialExpressionCeil` | Ceil |  |
| 29 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 30 | expression | `MaterialExpressionFmod` | Fmod |  |
| 31 | expression | `MaterialExpressionConstant` | 2 |  |
| 32 | expression | `MaterialExpressionFmod` | Fmod |  |
| 33 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 34 | expression | `MaterialExpressionConstant3Vector` | -1,1,1 |  |
| 35 | expression | `MaterialExpressionConstant3Vector` | 1,-1,1 |  |
| 36 | expression | `MaterialExpressionConstant3Vector` | -1,1,1 |  |
| 37 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 38 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 39 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 42 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 43 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 44 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 45 | expression | `MaterialExpressionFmod` | Fmod |  |
| 46 | expression | `MaterialExpressionFmod` | Fmod |  |
| 47 | expression | `MaterialExpressionConstant` | 3 |  |
| 48 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 49 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |

## Exact Input Wiring

### [0] Output UVW

- `A` <= [43] Switch output 0

### [4] Multiply

- `A` <= [3] -1,1,1 output 0
- `B` <= [10] World Space to Tangent Space TransformVector output 0

### [5] Abs

- `Input` <= [6] Divide(1,) output 0

### [6] Divide(1,)

- `B` <= [4] Multiply output 0

### [7] Multiply

- `A` <= [6] Divide(1,) output 0
- `B` <= [18] Append output 0

### [8] Subtract

- `A` <= [5] Abs output 0
- `B` <= [7] Multiply output 0

### [10] World Space to Tangent Space TransformVector

- `Input` <= [9] Camera Vector output 0

### [11] Multiply

- `A` <= [25] Min output 0
- `B` <= [4] Multiply output 0

### [13] Frac

- `Input` <= [21] Multiply output 0

### [15] Multiply

- `A` <= [13] Frac output 0
- `B` <= [14] 2,-2 output 0

### [17] Subtract

- `A` <= [15] Multiply output 0
- `B` <= [16] 1,-1 output 0

### [18] Append

- `A` <= [17] Subtract output 0
- `B` <= [22] -1 output 0

### [19] Add

- `A` <= [11] Multiply output 0
- `B` <= [18] Append output 0

### [21] Multiply

- `A` <= [26] Input UVs (Vector2) output 0
- `B` <= [20] Input Tiling (Vector2) output 0

### [24] Min

- `A` <= [23] BreakOutFloat3Components output 0
- `B` <= [23] BreakOutFloat3Components output 1

### [25] Min

- `A` <= [24] Min output 0
- `B` <= [23] BreakOutFloat3Components output 2

### [26] Input UVs (Vector2)

- `Preview` <= [12] TexCoord[0] output 0

### [27] Input Randomize Rotation (StaticBool)

- `Preview` <= [44] Static Bool (False) output 0

### [28] Ceil

- `Input` <= [21] Multiply output 0

### [30] Fmod

- `A` <= [29] BreakOutFloat2Components output 0
- `B` <= [31] 2 output 0

### [32] Fmod

- `A` <= [29] BreakOutFloat2Components output 1
- `B` <= [31] 2 output 0

### [33] Lerp

- `A` <= [34] -1,1,1 output 0
- `B` <= [35] 1,-1,1 output 0
- `Alpha` <= [30] Fmod output 0

### [38] Lerp

- `A` <= [37] 1,1,1 output 0
- `B` <= [36] -1,1,1 output 0
- `Alpha` <= [32] Fmod output 0

### [40] Multiply

- `A` <= [1] MakeFloat3 output 0
- `B` <= [41] Multiply output 0

### [41] Multiply

- `A` <= [33] Lerp output 0
- `B` <= [38] Lerp output 0

### [42] Lerp

- `A` <= [40] Multiply output 0
- `B` <= [39] Swizzle output 0
- `Alpha` <= [32] Fmod output 0

### [43] Switch

- `A` <= [42] Lerp output 0
- `B` <= [1] MakeFloat3 output 0
- `Value` <= [27] Input Randomize Rotation (StaticBool) output 0

### [45] Fmod

- `A` <= [29] BreakOutFloat2Components output 0
- `B` <= [47] 3 output 0

### [46] Fmod

- `A` <= [29] BreakOutFloat2Components output 1
- `B` <= [47] 3 output 0

### [49] Lerp

- `A` <= [41] Multiply output 0
- `B` <= [48] Swizzle output 0
- `Alpha` <= [46] Fmod output 0

## Connection List

- [43] Switch output 0 -> Output UVW.A
- [3] -1,1,1 output 0 -> Multiply.A
- [10] World Space to Tangent Space TransformVector output 0 -> Multiply.B
- [6] Divide(1,) output 0 -> Abs.Input
- [4] Multiply output 0 -> Divide(1,).B
- [6] Divide(1,) output 0 -> Multiply.A
- [18] Append output 0 -> Multiply.B
- [5] Abs output 0 -> Subtract.A
- [7] Multiply output 0 -> Subtract.B
- [9] Camera Vector output 0 -> World Space to Tangent Space TransformVector.Input
- [25] Min output 0 -> Multiply.A
- [4] Multiply output 0 -> Multiply.B
- [21] Multiply output 0 -> Frac.Input
- [13] Frac output 0 -> Multiply.A
- [14] 2,-2 output 0 -> Multiply.B
- [15] Multiply output 0 -> Subtract.A
- [16] 1,-1 output 0 -> Subtract.B
- [17] Subtract output 0 -> Append.A
- [22] -1 output 0 -> Append.B
- [11] Multiply output 0 -> Add.A
- [18] Append output 0 -> Add.B
- [26] Input UVs (Vector2) output 0 -> Multiply.A
- [20] Input Tiling (Vector2) output 0 -> Multiply.B
- [23] BreakOutFloat3Components output 0 -> Min.A
- [23] BreakOutFloat3Components output 1 -> Min.B
- [24] Min output 0 -> Min.A
- [23] BreakOutFloat3Components output 2 -> Min.B
- [12] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [44] Static Bool (False) output 0 -> Input Randomize Rotation (StaticBool).Preview
- [21] Multiply output 0 -> Ceil.Input
- [29] BreakOutFloat2Components output 0 -> Fmod.A
- [31] 2 output 0 -> Fmod.B
- [29] BreakOutFloat2Components output 1 -> Fmod.A
- [31] 2 output 0 -> Fmod.B
- [34] -1,1,1 output 0 -> Lerp.A
- [35] 1,-1,1 output 0 -> Lerp.B
- [30] Fmod output 0 -> Lerp.Alpha
- [37] 1,1,1 output 0 -> Lerp.A
- [36] -1,1,1 output 0 -> Lerp.B
- [32] Fmod output 0 -> Lerp.Alpha
- [1] MakeFloat3 output 0 -> Multiply.A
- [41] Multiply output 0 -> Multiply.B
- [33] Lerp output 0 -> Multiply.A
- [38] Lerp output 0 -> Multiply.B
- [40] Multiply output 0 -> Lerp.A
- [39] Swizzle output 0 -> Lerp.B
- [32] Fmod output 0 -> Lerp.Alpha
- [42] Lerp output 0 -> Switch.A
- [1] MakeFloat3 output 0 -> Switch.B
- [27] Input Randomize Rotation (StaticBool) output 0 -> Switch.Value
- [29] BreakOutFloat2Components output 0 -> Fmod.A
- [47] 3 output 0 -> Fmod.B
- [29] BreakOutFloat2Components output 1 -> Fmod.A
- [47] 3 output 0 -> Fmod.B
- [41] Multiply output 0 -> Lerp.A
- [48] Swizzle output 0 -> Lerp.B
- [46] Fmod output 0 -> Lerp.Alpha

## Output Trace Roots

- `UVW` is driven by [43] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
