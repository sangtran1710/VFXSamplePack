# DebugBinaryValues-Float

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/DebugBinaryValues-Float.DebugBinaryValues-Float`
- Category: Engine_MaterialFunctions01/Debug
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Displays the bit pattern of the input value as a floating point number.

## Inputs

- `Number of Bits` [FunctionInput_Scalar] - The default is 32-bit since all HLSL float and int operations are 32-bit. It may be desirable to view only 8 or 16-bits when debugging custom bit packing functions. Note that this bit setting is display only, it does not affect how any calculations are done.
- `Number To Convert` [FunctionInput_Scalar] - Since this is the (float) version, the input will be read as a float.
- `UVs` [FunctionInput_Vector2]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | expression | `MaterialExpressionFrac` | Frac |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 6 | expression | `MaterialExpressionClamp` | Clamp |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionDDX` | DDX |  |
| 9 | expression | `MaterialExpressionDDY` | DDY |  |
| 10 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | expression | `MaterialExpressionFloor` | Floor |  |
| 15 | custom | `MaterialExpressionCustom` | Binary Digit Check |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionConstant2Vector` | 0,0.1 |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionConstant2Vector` | 0,-0.45 |  |
| 22 | expression | `MaterialExpressionAdd` | Add |  |
| 23 | expression | `MaterialExpressionConstant2Vector` | 0,0 |  |
| 24 | expression | `MaterialExpressionDistance` | Distance |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionCeil` | Ceil |  |
| 27 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 28 | expression | `MaterialExpressionFrac` | Frac |  |
| 29 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | AlphaOffset |  |
| 31 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 33 | expression | `MaterialExpressionConstant3Vector` | 0,0.2,0 |  |
| 34 | expression | `MaterialExpressionAdd` | Add |  |
| 35 | expression | `MaterialExpressionFrac` | Frac |  |
| 36 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 37 | expression | `MaterialExpressionConstant` | -0.05 |  |
| 38 | function_input | `MaterialExpressionFunctionInput` | Input Number of Bits (Scalar) | Number of Bits |
| 39 | function_input | `MaterialExpressionFunctionInput` | Input Number To Convert (Scalar) | Number To Convert |
| 40 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugIntValues |  |
| 41 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 42 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 43 | expression | `MaterialExpressionFrac` | Frac |  |
| 44 | expression | `MaterialExpressionAdd` | Add |  |
| 45 | expression | `MaterialExpressionScalarParameter` | Param (2) 'Binary Debug Digits' |  |
| 46 | expression | `MaterialExpressionAdd` | Add |  |
| 47 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 48 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 49 | expression | `MaterialExpressionIf` | If |  |
| 50 | expression | `MaterialExpressionConstant` | 0 |  |
| 51 | expression | `MaterialExpressionAppendVector` | Append |  |
| 52 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 53 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 54 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 55 | expression | `MaterialExpressionAppendVector` | Append |  |
| 56 | expression | `MaterialExpressionConstant` | -0.55 |  |
| 57 | expression | `MaterialExpressionMultiply` | Multiply(,-0.2) |  |
| 58 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |

## Exact Input Wiring

### [0] Output Result

- `A` <= [44] Add output 0

### [1] Texture Sample

- `Coordinates` <= [18] Add output 0
- `CoordinatesDX` <= [8] DDX output 0
- `CoordinatesDY` <= [9] DDY output 0

### [3] Frac

- `Input` <= [13] Append output 0

### [5] Mask ( G )

- `Input` <= [35] Frac output 0

### [6] Clamp

- `Input` <= [27] 1-x output 0

### [7] Multiply

- `A` <= [6] Clamp output 0
- `B` <= [1] Texture Sample output 0

### [8] DDX

- `Value` <= [58] Input UVs (Vector2) output 0

### [9] DDY

- `Value` <= [58] Input UVs (Vector2) output 0

### [11] 1-x

- `Input` <= [19] BreakOutFloat2Components output 0

### [12] Multiply

- `A` <= [38] Input Number of Bits (Scalar) output 0
- `B` <= [11] 1-x output 0

### [13] Append

- `A` <= [12] Multiply output 0
- `B` <= [19] BreakOutFloat2Components output 1

### [14] Floor

- `Input` <= [12] Multiply output 0

### [16] Multiply

- `A` <= [15] Binary Digit Check output 0
- `B` <= [17] 0,0.1 output 0

### [18] Add

- `A` <= [22] Add output 0
- `B` <= [3] Frac output 0

### [20] Add

- `A` <= [58] Input UVs (Vector2) output 0
- `B` <= [21] 0,-0.45 output 0

### [22] Add

- `A` <= [16] Multiply output 0
- `B` <= [23] 0,0 output 0

### [24] Distance

- `A` <= [5] Mask ( G ) output 0
- `B` <= [36] 0.5 output 0

### [25] Add

- `A` <= [24] Distance output 0
- `B` <= [37] -0.05 output 0

### [26] Ceil

- `Input` <= [25] Add output 0

### [27] 1-x

- `Input` <= [26] Ceil output 0

### [28] Frac

- `Input` <= [20] Add output 0

### [29] Texture Sample

- `Coordinates` <= [58] Input UVs (Vector2) output 0

### [32] Multiply

- `A` <= [30] AlphaOffset output 0
- `B` <= [33] 0,0.2,0 output 0

### [34] Add

- `A` <= [7] Multiply output 0
- `B` <= [32] Multiply output 0

### [35] Frac

- `Input` <= [58] Input UVs (Vector2) output 0

### [42] Multiply

- `A` <= [53] Multiply(,1) output 0
- `B` <= [46] Add output 0

### [43] Frac

- `Input` <= [51] Append output 0

### [44] Add

- `A` <= [34] Add output 0
- `B` <= [40] DebugIntValues output 0

### [46] Add

- `A` <= [58] Input UVs (Vector2) output 0
- `B` <= [55] Append output 0

### [48] Multiply(,1)

- `A` <= [14] Floor output 0

### [49] If

- `A` <= [47] BreakOutFloat2Components output 1
- `B` <= [52] 0.5 output 0
- `AGreaterThanB` <= [50] 0 output 0
- `AEqualsB` <= [47] BreakOutFloat2Components output 1
- `ALessThanB` <= [50] 0 output 0

### [51] Append

- `A` <= [47] BreakOutFloat2Components output 0
- `B` <= [49] If output 0

### [53] Multiply(,1)

- `A` <= [38] Input Number of Bits (Scalar) output 0

### [54] Divide(1,)

- `B` <= [53] Multiply(,1) output 0

### [55] Append

- `A` <= [57] Multiply(,-0.2) output 0
- `B` <= [56] -0.55 output 0

### [57] Multiply(,-0.2)

- `A` <= [54] Divide(1,) output 0

### [58] Input UVs (Vector2)

- `Preview` <= [10] TexCoord[0] output 0

## Connection List

- [44] Add output 0 -> Output Result.A
- [18] Add output 0 -> Texture Sample.Coordinates
- [8] DDX output 0 -> Texture Sample.CoordinatesDX
- [9] DDY output 0 -> Texture Sample.CoordinatesDY
- [13] Append output 0 -> Frac.Input
- [35] Frac output 0 -> Mask ( G ).Input
- [27] 1-x output 0 -> Clamp.Input
- [6] Clamp output 0 -> Multiply.A
- [1] Texture Sample output 0 -> Multiply.B
- [58] Input UVs (Vector2) output 0 -> DDX.Value
- [58] Input UVs (Vector2) output 0 -> DDY.Value
- [19] BreakOutFloat2Components output 0 -> 1-x.Input
- [38] Input Number of Bits (Scalar) output 0 -> Multiply.A
- [11] 1-x output 0 -> Multiply.B
- [12] Multiply output 0 -> Append.A
- [19] BreakOutFloat2Components output 1 -> Append.B
- [12] Multiply output 0 -> Floor.Input
- [15] Binary Digit Check output 0 -> Multiply.A
- [17] 0,0.1 output 0 -> Multiply.B
- [22] Add output 0 -> Add.A
- [3] Frac output 0 -> Add.B
- [58] Input UVs (Vector2) output 0 -> Add.A
- [21] 0,-0.45 output 0 -> Add.B
- [16] Multiply output 0 -> Add.A
- [23] 0,0 output 0 -> Add.B
- [5] Mask ( G ) output 0 -> Distance.A
- [36] 0.5 output 0 -> Distance.B
- [24] Distance output 0 -> Add.A
- [37] -0.05 output 0 -> Add.B
- [25] Add output 0 -> Ceil.Input
- [26] Ceil output 0 -> 1-x.Input
- [20] Add output 0 -> Frac.Input
- [58] Input UVs (Vector2) output 0 -> Texture Sample.Coordinates
- [30] AlphaOffset output 0 -> Multiply.A
- [33] 0,0.2,0 output 0 -> Multiply.B
- [7] Multiply output 0 -> Add.A
- [32] Multiply output 0 -> Add.B
- [58] Input UVs (Vector2) output 0 -> Frac.Input
- [53] Multiply(,1) output 0 -> Multiply.A
- [46] Add output 0 -> Multiply.B
- [51] Append output 0 -> Frac.Input
- [34] Add output 0 -> Add.A
- [40] DebugIntValues output 0 -> Add.B
- [58] Input UVs (Vector2) output 0 -> Add.A
- [55] Append output 0 -> Add.B
- [14] Floor output 0 -> Multiply(,1).A
- [47] BreakOutFloat2Components output 1 -> If.A
- [52] 0.5 output 0 -> If.B
- [50] 0 output 0 -> If.AGreaterThanB
- [47] BreakOutFloat2Components output 1 -> If.AEqualsB
- [50] 0 output 0 -> If.ALessThanB
- [47] BreakOutFloat2Components output 0 -> Append.A
- [49] If output 0 -> Append.B
- [38] Input Number of Bits (Scalar) output 0 -> Multiply(,1).A
- [53] Multiply(,1) output 0 -> Divide(1,).B
- [57] Multiply(,-0.2) output 0 -> Append.A
- [56] -0.55 output 0 -> Append.B
- [54] Divide(1,) output 0 -> Multiply(,-0.2).A
- [10] TexCoord[0] output 0 -> Input UVs (Vector2).Preview

## Output Trace Roots

- `Result` is driven by [44] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


