# DebugFloat2Values

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugFloat2Values.DebugFloat2Values`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Plug a vector into the function and then preview the output to view the numeric values of the input vector.

## Inputs

- `UVs` [FunctionInput_Vector2]
- `DebugTextLocation RG_UpperRight BA_LowerLeft` [FunctionInput_Vector4]
- `MaximumNumberOfDigits` [FunctionInput_Scalar] - Max number of digits on either side of the decimal place.
- `Vector2` [FunctionInput_Vector2] - Enter a Vector to view.
- `Component Spacing` [FunctionInput_Vector4] - RG_UpperRight BA_LowerLeft

## Outputs

- `ColorCodedOutput`
- `GreyScaleOutput`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output ColorCodedOutput | ColorCodedOutput |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugScalarValues |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugScalarValues |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureCropping |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) | DebugTextLocation RG_UpperRight BA_LowerLeft |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( B A ) |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input MaximumNumberOfDigits (Scalar) | MaximumNumberOfDigits |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionConstant2Vector` | 1,2 |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Vector2 (Vector2) | Vector2 |
| 15 | expression | `MaterialExpressionAppendVector` | Append |  |
| 16 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Component Spacing (Vector4) | Component Spacing |
| 18 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 19 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output GreyScaleOutput | GreyScaleOutput |

## Exact Input Wiring

### [0] Output ColorCodedOutput

- `A` <= [15] Append output 0

### [3] Input UVs (Vector2)

- `Preview` <= [4] TexCoord[0] output 0

### [6] Mask ( R G )

- `Input` <= [7] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [8] Mask ( B A )

- `Input` <= [7] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [10] Multiply

- `A` <= [5] TextureCropping output 2
- `B` <= [11] 1,2 output 0

### [12] Subtract

- `A` <= [10] Multiply output 0
- `B` <= [13] 0,1 output 0

### [15] Append

- `A` <= [1] DebugScalarValues output 0
- `B` <= [2] DebugScalarValues output 0

### [18] Dot

- `A` <= [15] Append output 0
- `B` <= [19] 1,1 output 0

### [20] Output GreyScaleOutput

- `A` <= [18] Dot output 0

## Connection List

- [15] Append output 0 -> Output ColorCodedOutput.A
- [4] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [7] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( R G ).Input
- [7] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( B A ).Input
- [5] TextureCropping output 2 -> Multiply.A
- [11] 1,2 output 0 -> Multiply.B
- [10] Multiply output 0 -> Subtract.A
- [13] 0,1 output 0 -> Subtract.B
- [1] DebugScalarValues output 0 -> Append.A
- [2] DebugScalarValues output 0 -> Append.B
- [15] Append output 0 -> Dot.A
- [19] 1,1 output 0 -> Dot.B
- [18] Dot output 0 -> Output GreyScaleOutput.A

## Output Trace Roots

- `ColorCodedOutput` is driven by [15] Append
- `GreyScaleOutput` is driven by [18] Dot

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
