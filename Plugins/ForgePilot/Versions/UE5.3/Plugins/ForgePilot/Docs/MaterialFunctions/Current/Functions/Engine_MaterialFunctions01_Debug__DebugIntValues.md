# DebugIntValues

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/DebugIntValues.DebugIntValues`
- Category: Engine_MaterialFunctions01/Debug
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Enter a value into the material function and read its return value as a number in uv space.

## Inputs

- `Number` [FunctionInput_Scalar]
- `MaximumNumberOfDigits` [FunctionInput_Scalar] - Max number of displayed integers.
- `UVs` [FunctionInput_Vector2]
- `DebugTextLocation RG_UpperRight BA_LowerLeft` [FunctionInput_Vector4]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Number (Scalar) | Number |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionConstant` | 10 |  |
| 10 | expression | `MaterialExpressionPower` | Power |  |
| 11 | expression | `MaterialExpressionFrac` | Frac |  |
| 12 | expression | `MaterialExpressionConstant` | 128.5 |  |
| 13 | expression | `MaterialExpressionFrac` | Frac |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(,100) |  |
| 15 | expression | `MaterialExpressionDivide` | Divide |  |
| 16 | expression | `MaterialExpressionFloor` | Floor |  |
| 17 | expression | `MaterialExpressionDivide` | Divide(,10) |  |
| 18 | expression | `MaterialExpressionIf` | If |  |
| 19 | expression | `MaterialExpressionConstant` | 0 |  |
| 20 | expression | `MaterialExpressionConstant` | 1 |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input MaximumNumberOfDigits (Scalar) | MaximumNumberOfDigits |
| 23 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 24 | expression | `MaterialExpressionCeil` | Ceil |  |
| 25 | expression | `MaterialExpressionClamp` | Clamp |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionClamp` | Clamp |  |
| 28 | expression | `MaterialExpressionAbs` | Abs |  |
| 29 | expression | `MaterialExpressionCeil` | Ceil |  |
| 30 | expression | `MaterialExpressionPower` | Power |  |
| 31 | expression | `MaterialExpressionFloor` | Floor |  |
| 32 | expression | `MaterialExpressionClamp` | Clamp |  |
| 33 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 34 | expression | `MaterialExpressionAdd` | Add |  |
| 35 | expression | `MaterialExpressionFrac` | Frac |  |
| 36 | expression | `MaterialExpressionMultiply` | Multiply(,10) |  |
| 37 | expression | `MaterialExpressionDivide` | Divide(,10) |  |
| 38 | expression | `MaterialExpressionFloor` | Floor |  |
| 39 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 40 | expression | `MaterialExpressionClamp` | Clamp |  |
| 41 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 42 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 43 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 44 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 45 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureCropping |  |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) | DebugTextLocation RG_UpperRight BA_LowerLeft |
| 47 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 48 | expression | `MaterialExpressionComponentMask` | Mask ( B A ) |  |
| 49 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 50 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 51 | expression | `MaterialExpressionConstant2Vector` | 2,0.1 |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 53 | expression | `MaterialExpressionAdd` | Add |  |
| 54 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 55 | expression | `MaterialExpressionClamp` | Clamp |  |
| 56 | expression | `MaterialExpressionAdd` | Add |  |
| 57 | expression | `MaterialExpressionDivide` | Divide(,1e+06) |  |
| 58 | expression | `MaterialExpressionDivide` | Divide |  |
| 59 | expression | `MaterialExpressionAdd` | Add(,3) |  |
| 60 | expression | `MaterialExpressionPower` | Power |  |
| 61 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 62 | expression | `MaterialExpressionAbs` | Abs |  |
| 63 | expression | `MaterialExpressionSubtract` | Subtract(,0) |  |
| 64 | expression | `MaterialExpressionCeil` | Ceil |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [61] Mask ( R ) output 0

### [1] Texture Sample

- `Coordinates` <= [5] Append output 0

### [2] Add

- `A` <= [4] Mask ( G ) output 0
- `B` <= [17] Divide(,10) output 0

### [3] Input Number (Scalar)

- `Preview` <= [12] 128.5 output 0

### [4] Mask ( G )

- `Input` <= [50] Multiply output 0

### [5] Append

- `A` <= [11] Frac output 0
- `B` <= [33] Lerp output 0

### [6] Mask ( R )

- `Input` <= [50] Multiply output 0

### [7] 1-x

- `Input` <= [6] Mask ( R ) output 0

### [8] Multiply

- `A` <= [7] 1-x output 0
- `B` <= [22] Input MaximumNumberOfDigits (Scalar) output 0

### [10] Power

- `Base` <= [9] 10 output 0
- `Exponent` <= [23] Add(,1) output 0

### [11] Frac

- `Input` <= [8] Multiply output 0

### [13] Frac

- `Input` <= [15] Divide output 0

### [14] Multiply(,100)

- `A` <= [13] Frac output 0

### [15] Divide

- `A` <= [62] Abs output 0
- `B` <= [10] Power output 0

### [16] Floor

- `Input` <= [14] Multiply(,100) output 0

### [17] Divide(,10)

- `A` <= [16] Floor output 0

### [18] If

- `A` <= [58] Divide output 0
- `B` <= [19] 0 output 0
- `AGreaterThanB` <= [20] 1 output 0
- `AEqualsB` <= [19] 0 output 0
- `ALessThanB` <= [19] 0 output 0

### [21] Multiply

- `A` <= [1] Texture Sample output 1
- `B` <= [54] Multiply output 0

### [23] Add(,1)

- `A` <= [24] Ceil output 0

### [24] Ceil

- `Input` <= [25] Clamp output 0

### [25] Clamp

- `Input` <= [8] Multiply output 0

### [26] Multiply

- `A` <= [56] Add output 0
- `B` <= [30] Power output 0

### [27] Clamp

- `Input` <= [8] Multiply output 0

### [28] Abs

- `Input` <= [27] Clamp output 0

### [29] Ceil

- `Input` <= [40] Clamp output 0

### [30] Power

- `Base` <= [9] 10 output 0
- `Exponent` <= [38] Floor output 0

### [31] Floor

- `Input` <= [36] Multiply(,10) output 0

### [32] Clamp

- `Input` <= [29] Ceil output 0

### [33] Lerp

- `A` <= [2] Add output 0
- `B` <= [34] Add output 0
- `Alpha` <= [32] Clamp output 0

### [34] Add

- `A` <= [4] Mask ( G ) output 0
- `B` <= [37] Divide(,10) output 0

### [35] Frac

- `Input` <= [26] Multiply output 0

### [36] Multiply(,10)

- `A` <= [35] Frac output 0

### [37] Divide(,10)

- `A` <= [31] Floor output 0

### [38] Floor

- `Input` <= [40] Clamp output 0

### [39] Subtract(,1)

- `A` <= [28] Abs output 0

### [40] Clamp

- `Input` <= [39] Subtract(,1) output 0

### [41] SphereMask

- `B` <= [42] -0.5 output 0

### [43] 1-x

- `Input` <= [41] SphereMask output 0

### [44] Input UVs (Vector2)

- `Preview` <= [49] TexCoord[0] output 0

### [47] Mask ( R G )

- `Input` <= [46] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [48] Mask ( B A )

- `Input` <= [46] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [50] Multiply

- `A` <= [45] TextureCropping output 2
- `B` <= [51] 2,0.1 output 0

### [52] Multiply

- `A` <= [21] Multiply output 0
- `B` <= [45] TextureCropping output 3

### [53] Add

- `A` <= [18] If output 0
- `B` <= [32] Clamp output 0

### [54] Multiply

- `A` <= [53] Add output 0
- `B` <= [64] Ceil output 0

### [55] Clamp

- `Input` <= [52] Multiply output 0

### [56] Add

- `A` <= [62] Abs output 0
- `B` <= [57] Divide(,1e+06) output 0

### [57] Divide(,1e+06)

- `A` <= [62] Abs output 0

### [58] Divide

- `A` <= [62] Abs output 0
- `B` <= [60] Power output 0

### [59] Add(,3)

- `A` <= [23] Add(,1) output 0

### [60] Power

- `Base` <= [9] 10 output 0
- `Exponent` <= [59] Add(,3) output 0

### [61] Mask ( R )

- `Input` <= [55] Clamp output 0

### [62] Abs

- `Input` <= [3] Input Number (Scalar) output 0

### [63] Subtract(,0)

- `A` <= [8] Multiply output 0

### [64] Ceil

- `Input` <= [63] Subtract(,0) output 0

## Connection List

- [61] Mask ( R ) output 0 -> Output Result.A
- [5] Append output 0 -> Texture Sample.Coordinates
- [4] Mask ( G ) output 0 -> Add.A
- [17] Divide(,10) output 0 -> Add.B
- [12] 128.5 output 0 -> Input Number (Scalar).Preview
- [50] Multiply output 0 -> Mask ( G ).Input
- [11] Frac output 0 -> Append.A
- [33] Lerp output 0 -> Append.B
- [50] Multiply output 0 -> Mask ( R ).Input
- [6] Mask ( R ) output 0 -> 1-x.Input
- [7] 1-x output 0 -> Multiply.A
- [22] Input MaximumNumberOfDigits (Scalar) output 0 -> Multiply.B
- [9] 10 output 0 -> Power.Base
- [23] Add(,1) output 0 -> Power.Exponent
- [8] Multiply output 0 -> Frac.Input
- [15] Divide output 0 -> Frac.Input
- [13] Frac output 0 -> Multiply(,100).A
- [62] Abs output 0 -> Divide.A
- [10] Power output 0 -> Divide.B
- [14] Multiply(,100) output 0 -> Floor.Input
- [16] Floor output 0 -> Divide(,10).A
- [58] Divide output 0 -> If.A
- [19] 0 output 0 -> If.B
- [20] 1 output 0 -> If.AGreaterThanB
- [19] 0 output 0 -> If.AEqualsB
- [19] 0 output 0 -> If.ALessThanB
- [1] Texture Sample output 1 -> Multiply.A
- [54] Multiply output 0 -> Multiply.B
- [24] Ceil output 0 -> Add(,1).A
- [25] Clamp output 0 -> Ceil.Input
- [8] Multiply output 0 -> Clamp.Input
- [56] Add output 0 -> Multiply.A
- [30] Power output 0 -> Multiply.B
- [8] Multiply output 0 -> Clamp.Input
- [27] Clamp output 0 -> Abs.Input
- [40] Clamp output 0 -> Ceil.Input
- [9] 10 output 0 -> Power.Base
- [38] Floor output 0 -> Power.Exponent
- [36] Multiply(,10) output 0 -> Floor.Input
- [29] Ceil output 0 -> Clamp.Input
- [2] Add output 0 -> Lerp.A
- [34] Add output 0 -> Lerp.B
- [32] Clamp output 0 -> Lerp.Alpha
- [4] Mask ( G ) output 0 -> Add.A
- [37] Divide(,10) output 0 -> Add.B
- [26] Multiply output 0 -> Frac.Input
- [35] Frac output 0 -> Multiply(,10).A
- [31] Floor output 0 -> Divide(,10).A
- [40] Clamp output 0 -> Floor.Input
- [28] Abs output 0 -> Subtract(,1).A
- [39] Subtract(,1) output 0 -> Clamp.Input
- [42] -0.5 output 0 -> SphereMask.B
- [41] SphereMask output 0 -> 1-x.Input
- [49] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [46] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( R G ).Input
- [46] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( B A ).Input
- [45] TextureCropping output 2 -> Multiply.A
- [51] 2,0.1 output 0 -> Multiply.B
- [21] Multiply output 0 -> Multiply.A
- [45] TextureCropping output 3 -> Multiply.B
- [18] If output 0 -> Add.A
- [32] Clamp output 0 -> Add.B
- [53] Add output 0 -> Multiply.A
- [64] Ceil output 0 -> Multiply.B
- [52] Multiply output 0 -> Clamp.Input
- [62] Abs output 0 -> Add.A
- [57] Divide(,1e+06) output 0 -> Add.B
- [62] Abs output 0 -> Divide(,1e+06).A
- [62] Abs output 0 -> Divide.A
- [60] Power output 0 -> Divide.B
- [23] Add(,1) output 0 -> Add(,3).A
- [9] 10 output 0 -> Power.Base
- [59] Add(,3) output 0 -> Power.Exponent
- [55] Clamp output 0 -> Mask ( R ).Input
- [3] Input Number (Scalar) output 0 -> Abs.Input
- [8] Multiply output 0 -> Subtract(,0).A
- [63] Subtract(,0) output 0 -> Ceil.Input

## Output Trace Roots

- `Result` is driven by [61] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


