# BrickAndTileUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/BrickAndTileUVs.BrickAndTileUVs`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Bricks and Tile UVs create a semi procedural UV offset pattern useful for the creation of patterned textures.

## Inputs

- `Tiles X` [FunctionInput_Scalar]
- `Tiles Y` [FunctionInput_Scalar]
- `Tile Shift` [FunctionInput_Scalar]
- `Texture Tiling` [FunctionInput_Scalar]
- `Rotate 90` [FunctionInput_StaticBool]
- `Macro Tiling` [FunctionInput_Scalar]
- `Pattern Distortion` [FunctionInput_Vector2] - Allows you to distort the random tile edge pattern.
- `UV` [FunctionInput_Vector2]

## Outputs

- `DiffuseUVs`
- `MacroUVs`
- `TileUVs`
- `Macro Offsets only`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output DiffuseUVs | DiffuseUVs |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionFloor` | Floor |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Tiles X (Scalar) | Tiles X |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Tiles Y (Scalar) | Tiles Y |
| 8 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionConstant` | 0.1 |  |
| 15 | expression | `MaterialExpressionSine` | Sine |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Tile Shift (Scalar) | Tile Shift |
| 17 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Texture Tiling (Scalar) | Texture Tiling |
| 19 | expression | `MaterialExpressionConstant` | 1 |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 22 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Rotate 90 (StaticBool) | Rotate 90 |
| 24 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Macro Tiling (Scalar) | Macro Tiling |
| 26 | expression | `MaterialExpressionConstant` | 0.18 |  |
| 27 | expression | `MaterialExpressionAdd` | Add |  |
| 28 | expression | `MaterialExpressionAdd` | Add(,0) |  |
| 29 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 30 | function_output | `MaterialExpressionFunctionOutput` | Output MacroUVs | MacroUVs |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output TileUVs | TileUVs |
| 32 | expression | `MaterialExpressionFrac` | Frac |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionFloor` | Floor |  |
| 36 | expression | `MaterialExpressionDivide` | Divide |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply(,0.31) |  |
| 39 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionConstant2Vector` | 0,0.411 |  |
| 42 | expression | `MaterialExpressionAdd` | Add |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | expression | `MaterialExpressionDivide` | Divide |  |
| 45 | expression | `MaterialExpressionConstant` | 2 |  |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Pattern Distortion (Vector2) | Pattern Distortion |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | expression | `MaterialExpressionAdd` | Add |  |
| 49 | expression | `MaterialExpressionAdd` | Add |  |
| 50 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 51 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 52 | expression | `MaterialExpressionConstant2Vector` | 0.13,0 |  |
| 53 | expression | `MaterialExpressionAdd` | Add |  |
| 54 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 55 | function_output | `MaterialExpressionFunctionOutput` | Output Macro Offsets only | Macro Offsets only |
| 56 | custom | `MaterialExpressionCustom` | Pseudo Random |  |
| 57 | function_input | `MaterialExpressionFunctionInput` | Input UV (Vector2) | UV |

## Exact Input Wiring

### [0] Output DiffuseUVs

- `A` <= [12] Add output 0

### [2] Floor

- `Input` <= [47] Add output 0

### [3] Multiply

- `A` <= [2] Floor output 0
- `B` <= [44] Divide output 0

### [4] Add

- `A` <= [43] Multiply output 0
- `B` <= [56] Pseudo Random output 0

### [5] Append

- `A` <= [6] Input Tiles X (Scalar) output 0
- `B` <= [7] Input Tiles Y (Scalar) output 0

### [8] Multiply(,1)

- `A` <= [29] Multiply output 0

### [9] Mask ( G )

- `Input` <= [3] Multiply output 0

### [10] Append

- `A` <= [9] Mask ( G ) output 0
- `B` <= [11] 0 output 0

### [12] Add

- `A` <= [43] Multiply output 0
- `B` <= [56] Pseudo Random output 0

### [13] Multiply

- `A` <= [16] Input Tile Shift (Scalar) output 0
- `B` <= [15] Sine output 0

### [15] Sine

- `Input` <= [10] Append output 0

### [16] Input Tile Shift (Scalar)

- `Preview` <= [14] 0.1 output 0

### [18] Input Texture Tiling (Scalar)

- `Preview` <= [19] 1 output 0

### [20] Add

- `A` <= [49] Add output 0
- `B` <= [13] Multiply output 0

### [22] Switch

- `A` <= [21] Swizzle output 1
- `B` <= [57] Input UV (Vector2) output 0
- `Value` <= [23] Input Rotate 90 (StaticBool) output 0

### [23] Input Rotate 90 (StaticBool)

- `Preview` <= [24] Static Bool (False) output 0

### [25] Input Macro Tiling (Scalar)

- `Preview` <= [26] 0.18 output 0

### [27] Add

- `A` <= [56] Pseudo Random output 0
- `B` <= [37] Multiply output 0

### [28] Add(,0)

- `A` <= [27] Add output 0

### [29] Multiply

- `A` <= [5] Append output 0
- `B` <= [22] Switch output 0

### [30] Output MacroUVs

- `A` <= [28] Add(,0) output 0

### [31] Output TileUVs

- `A` <= [20] Add output 0

### [32] Frac

- `Input` <= [33] Add output 0

### [33] Add

- `A` <= [36] Divide output 0
- `B` <= [22] Switch output 0

### [34] Multiply

- `A` <= [5] Append output 0
- `B` <= [32] Frac output 0

### [35] Floor

- `Input` <= [48] Add output 0

### [36] Divide

- `A` <= [13] Multiply output 0
- `B` <= [6] Input Tiles X (Scalar) output 0

### [37] Multiply

- `A` <= [25] Input Macro Tiling (Scalar) output 0
- `B` <= [32] Frac output 0

### [38] Multiply(,0.31)

- `A` <= [35] Floor output 0

### [39] Mask ( R )

- `Input` <= [38] Multiply(,0.31) output 0

### [40] Multiply

- `A` <= [39] Mask ( R ) output 0
- `B` <= [41] 0,0.411 output 0

### [42] Add

- `A` <= [38] Multiply(,0.31) output 0
- `B` <= [40] Multiply output 0

### [43] Multiply

- `A` <= [18] Input Texture Tiling (Scalar) output 0
- `B` <= [32] Frac output 0

### [44] Divide

- `A` <= [17] 0.5 output 0
- `B` <= [45] 2 output 0

### [47] Add

- `A` <= [29] Multiply output 0
- `B` <= [46] Input Pattern Distortion (Vector2) output 0

### [48] Add

- `A` <= [54] Multiply(,1) output 0
- `B` <= [34] Multiply output 0

### [49] Add

- `A` <= [54] Multiply(,1) output 0
- `B` <= [8] Multiply(,1) output 0

### [50] Mask ( G )

- `Input` <= [38] Multiply(,0.31) output 0

### [51] Multiply

- `A` <= [50] Mask ( G ) output 0
- `B` <= [52] 0.13,0 output 0

### [53] Add

- `A` <= [42] Add output 0
- `B` <= [51] Multiply output 0

### [54] Multiply(,1)

- `A` <= [46] Input Pattern Distortion (Vector2) output 0

### [55] Output Macro Offsets only

- `A` <= [56] Pseudo Random output 0

### [57] Input UV (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

## Connection List

- [12] Add output 0 -> Output DiffuseUVs.A
- [47] Add output 0 -> Floor.Input
- [2] Floor output 0 -> Multiply.A
- [44] Divide output 0 -> Multiply.B
- [43] Multiply output 0 -> Add.A
- [56] Pseudo Random output 0 -> Add.B
- [6] Input Tiles X (Scalar) output 0 -> Append.A
- [7] Input Tiles Y (Scalar) output 0 -> Append.B
- [29] Multiply output 0 -> Multiply(,1).A
- [3] Multiply output 0 -> Mask ( G ).Input
- [9] Mask ( G ) output 0 -> Append.A
- [11] 0 output 0 -> Append.B
- [43] Multiply output 0 -> Add.A
- [56] Pseudo Random output 0 -> Add.B
- [16] Input Tile Shift (Scalar) output 0 -> Multiply.A
- [15] Sine output 0 -> Multiply.B
- [10] Append output 0 -> Sine.Input
- [14] 0.1 output 0 -> Input Tile Shift (Scalar).Preview
- [19] 1 output 0 -> Input Texture Tiling (Scalar).Preview
- [49] Add output 0 -> Add.A
- [13] Multiply output 0 -> Add.B
- [21] Swizzle output 1 -> Switch.A
- [57] Input UV (Vector2) output 0 -> Switch.B
- [23] Input Rotate 90 (StaticBool) output 0 -> Switch.Value
- [24] Static Bool (False) output 0 -> Input Rotate 90 (StaticBool).Preview
- [26] 0.18 output 0 -> Input Macro Tiling (Scalar).Preview
- [56] Pseudo Random output 0 -> Add.A
- [37] Multiply output 0 -> Add.B
- [27] Add output 0 -> Add(,0).A
- [5] Append output 0 -> Multiply.A
- [22] Switch output 0 -> Multiply.B
- [28] Add(,0) output 0 -> Output MacroUVs.A
- [20] Add output 0 -> Output TileUVs.A
- [33] Add output 0 -> Frac.Input
- [36] Divide output 0 -> Add.A
- [22] Switch output 0 -> Add.B
- [5] Append output 0 -> Multiply.A
- [32] Frac output 0 -> Multiply.B
- [48] Add output 0 -> Floor.Input
- [13] Multiply output 0 -> Divide.A
- [6] Input Tiles X (Scalar) output 0 -> Divide.B
- [25] Input Macro Tiling (Scalar) output 0 -> Multiply.A
- [32] Frac output 0 -> Multiply.B
- [35] Floor output 0 -> Multiply(,0.31).A
- [38] Multiply(,0.31) output 0 -> Mask ( R ).Input
- [39] Mask ( R ) output 0 -> Multiply.A
- [41] 0,0.411 output 0 -> Multiply.B
- [38] Multiply(,0.31) output 0 -> Add.A
- [40] Multiply output 0 -> Add.B
- [18] Input Texture Tiling (Scalar) output 0 -> Multiply.A
- [32] Frac output 0 -> Multiply.B
- [17] 0.5 output 0 -> Divide.A
- [45] 2 output 0 -> Divide.B
- [29] Multiply output 0 -> Add.A
- [46] Input Pattern Distortion (Vector2) output 0 -> Add.B
- [54] Multiply(,1) output 0 -> Add.A
- [34] Multiply output 0 -> Add.B
- [54] Multiply(,1) output 0 -> Add.A
- [8] Multiply(,1) output 0 -> Add.B
- [38] Multiply(,0.31) output 0 -> Mask ( G ).Input
- [50] Mask ( G ) output 0 -> Multiply.A
- [52] 0.13,0 output 0 -> Multiply.B
- [42] Add output 0 -> Add.A
- [51] Multiply output 0 -> Add.B
- [46] Input Pattern Distortion (Vector2) output 0 -> Multiply(,1).A
- [56] Pseudo Random output 0 -> Output Macro Offsets only.A
- [1] TexCoord[0] output 0 -> Input UV (Vector2).Preview

## Output Trace Roots

- `DiffuseUVs` is driven by [12] Add
- `MacroUVs` is driven by [28] Add(,0)
- `TileUVs` is driven by [20] Add
- `Macro Offsets only` is driven by [56] Pseudo Random

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
