# FunctionGraphSetup

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/FunctionGraphSetup.FunctionGraphSetup`
- Category: Engine_MaterialFunctions01/Debug
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `X range Y range` [FunctionInput_Vector4] - X range=R-G, Y range =B-A
- `Gradient Height` [FunctionInput_Scalar]
- `UVs` [FunctionInput_Vector2] - Leave this input alone unless you want to change index or know the exact effect of remapping when the input range is not 0-1.

## Outputs

- `UV`
- `x`
- `y`
- `Gradient Height`
- `Corrected Y`
- `Range`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input X range Y range (Vector4) | X range Y range |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Gradient Height (Scalar) | Gradient Height |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionAbs` | Abs |  |
| 15 | custom | `MaterialExpressionCustom` | Custom |  |
| 16 | expression | `MaterialExpressionIf` | If |  |
| 17 | expression | `MaterialExpressionConstant` | 0 |  |
| 18 | expression | `MaterialExpressionIf` | If |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 21 | expression | `MaterialExpressionAdd` | Add |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output UV | UV |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | function_output | `MaterialExpressionFunctionOutput` | Output x | x |
| 26 | function_output | `MaterialExpressionFunctionOutput` | Output y | y |
| 27 | expression | `MaterialExpressionAdd` | Add(0,1) |  |
| 28 | expression | `MaterialExpressionAdd` | Add(0,1) |  |
| 29 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output Gradient Height | Gradient Height |
| 32 | function_output | `MaterialExpressionFunctionOutput` | Output Corrected Y | Corrected Y |
| 33 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 34 | function_output | `MaterialExpressionFunctionOutput` | Output Range | Range |

## Exact Input Wiring

### [0] Lerp

- `A` <= [16] If output 0
- `B` <= [18] If output 0
- `Alpha` <= [1] Mask ( G ) output 0

### [1] Mask ( G )

- `Input` <= [30] Input UVs (Vector2) output 0

### [2] Mask ( R )

- `Input` <= [30] Input UVs (Vector2) output 0

### [3] Mask ( B )

- `Input` <= [6] Input X range Y range (Vector4) output 0

### [4] Mask ( A )

- `Input` <= [6] Input X range Y range (Vector4) output 0

### [5] Lerp

- `A` <= [20] Subtract output 0
- `B` <= [21] Add output 0
- `Alpha` <= [2] Mask ( R ) output 0

### [7] Mask ( R )

- `Input` <= [6] Input X range Y range (Vector4) output 0

### [8] Mask ( G )

- `Input` <= [6] Input X range Y range (Vector4) output 0

### [9] Subtract

- `A` <= [8] Mask ( G ) output 0
- `B` <= [7] Mask ( R ) output 0

### [10] Subtract

- `A` <= [4] Mask ( A ) output 0
- `B` <= [3] Mask ( B ) output 0

### [12] Multiply

- `A` <= [14] Abs output 0
- `B` <= [10] Subtract output 0

### [13] Add

- `A` <= [12] Multiply output 0
- `B` <= [4] Mask ( A ) output 0

### [14] Abs

- `Input` <= [11] Input Gradient Height (Scalar) output 0

### [16] If

- `A` <= [11] Input Gradient Height (Scalar) output 0
- `B` <= [17] 0 output 0
- `AGreaterThanB` <= [13] Add output 0
- `AEqualsB` <= [4] Mask ( A ) output 0
- `ALessThanB` <= [4] Mask ( A ) output 0

### [18] If

- `A` <= [11] Input Gradient Height (Scalar) output 0
- `B` <= [17] 0 output 0
- `AGreaterThanB` <= [3] Mask ( B ) output 0
- `AEqualsB` <= [3] Mask ( B ) output 0
- `ALessThanB` <= [19] Subtract output 0

### [19] Subtract

- `A` <= [3] Mask ( B ) output 0
- `B` <= [12] Multiply output 0

### [20] Subtract

- `A` <= [7] Mask ( R ) output 0
- `B` <= [22] Multiply(,0.5) output 0

### [21] Add

- `A` <= [8] Mask ( G ) output 0
- `B` <= [22] Multiply(,0.5) output 0

### [22] Multiply(,0.5)

- `A` <= [12] Multiply output 0

### [23] Output UV

- `A` <= [24] Append output 0

### [24] Append

- `A` <= [5] Lerp output 0
- `B` <= [0] Lerp output 0

### [25] Output x

- `A` <= [5] Lerp output 0

### [26] Output y

- `A` <= [0] Lerp output 0

### [30] Input UVs (Vector2)

- `Preview` <= [29] TexCoord[0] output 0

### [31] Output Gradient Height

- `A` <= [11] Input Gradient Height (Scalar) output 0

### [32] Output Corrected Y

- `A` <= [33] Subtract output 0

### [33] Subtract

- `A` <= [16] If output 0
- `B` <= [18] If output 0

### [34] Output Range

- `A` <= [6] Input X range Y range (Vector4) output 0

## Connection List

- [16] If output 0 -> Lerp.A
- [18] If output 0 -> Lerp.B
- [1] Mask ( G ) output 0 -> Lerp.Alpha
- [30] Input UVs (Vector2) output 0 -> Mask ( G ).Input
- [30] Input UVs (Vector2) output 0 -> Mask ( R ).Input
- [6] Input X range Y range (Vector4) output 0 -> Mask ( B ).Input
- [6] Input X range Y range (Vector4) output 0 -> Mask ( A ).Input
- [20] Subtract output 0 -> Lerp.A
- [21] Add output 0 -> Lerp.B
- [2] Mask ( R ) output 0 -> Lerp.Alpha
- [6] Input X range Y range (Vector4) output 0 -> Mask ( R ).Input
- [6] Input X range Y range (Vector4) output 0 -> Mask ( G ).Input
- [8] Mask ( G ) output 0 -> Subtract.A
- [7] Mask ( R ) output 0 -> Subtract.B
- [4] Mask ( A ) output 0 -> Subtract.A
- [3] Mask ( B ) output 0 -> Subtract.B
- [14] Abs output 0 -> Multiply.A
- [10] Subtract output 0 -> Multiply.B
- [12] Multiply output 0 -> Add.A
- [4] Mask ( A ) output 0 -> Add.B
- [11] Input Gradient Height (Scalar) output 0 -> Abs.Input
- [11] Input Gradient Height (Scalar) output 0 -> If.A
- [17] 0 output 0 -> If.B
- [13] Add output 0 -> If.AGreaterThanB
- [4] Mask ( A ) output 0 -> If.AEqualsB
- [4] Mask ( A ) output 0 -> If.ALessThanB
- [11] Input Gradient Height (Scalar) output 0 -> If.A
- [17] 0 output 0 -> If.B
- [3] Mask ( B ) output 0 -> If.AGreaterThanB
- [3] Mask ( B ) output 0 -> If.AEqualsB
- [19] Subtract output 0 -> If.ALessThanB
- [3] Mask ( B ) output 0 -> Subtract.A
- [12] Multiply output 0 -> Subtract.B
- [7] Mask ( R ) output 0 -> Subtract.A
- [22] Multiply(,0.5) output 0 -> Subtract.B
- [8] Mask ( G ) output 0 -> Add.A
- [22] Multiply(,0.5) output 0 -> Add.B
- [12] Multiply output 0 -> Multiply(,0.5).A
- [24] Append output 0 -> Output UV.A
- [5] Lerp output 0 -> Append.A
- [0] Lerp output 0 -> Append.B
- [5] Lerp output 0 -> Output x.A
- [0] Lerp output 0 -> Output y.A
- [29] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [11] Input Gradient Height (Scalar) output 0 -> Output Gradient Height.A
- [33] Subtract output 0 -> Output Corrected Y.A
- [16] If output 0 -> Subtract.A
- [18] If output 0 -> Subtract.B
- [6] Input X range Y range (Vector4) output 0 -> Output Range.A

## Output Trace Roots

- `UV` is driven by [24] Append
- `x` is driven by [5] Lerp
- `y` is driven by [0] Lerp
- `Gradient Height` is driven by [11] Input Gradient Height (Scalar)
- `Corrected Y` is driven by [33] Subtract
- `Range` is driven by [6] Input X range Y range (Vector4)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


