# DebugFloat4Values

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugFloat4Values.DebugFloat4Values`
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
- `Vector4` [FunctionInput_Vector4] - Enter a number to view.
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
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugScalarValues |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 5 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | TextureCropping |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) | DebugTextLocation RG_UpperRight BA_LowerLeft |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( B A ) |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input MaximumNumberOfDigits (Scalar) | MaximumNumberOfDigits |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant2Vector` | 1,4 |  |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 15 | expression | `MaterialExpressionConstant2Vector` | 0,2 |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Vector4 (Vector4) | Vector4 |
| 18 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output GreyScaleOutput | GreyScaleOutput |
| 20 | expression | `MaterialExpressionConstant4Vector` | 1,1,1,1 |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | DebugScalarValues |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 23 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 24 | expression | `MaterialExpressionConstant2Vector` | 0,3 |  |
| 25 | expression | `MaterialExpressionAppendVector` | Append |  |
| 26 | expression | `MaterialExpressionAppendVector` | Append |  |
| 27 | expression | `MaterialExpressionAdd` | Add |  |
| 28 | expression | `MaterialExpressionAppendVector` | Append |  |
| 29 | expression | `MaterialExpressionAppendVector` | Append |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Component Spacing (Vector4) | Component Spacing |

## Exact Input Wiring

### [0] Output ColorCodedOutput

- `A` <= [29] Append output 0

### [4] Input UVs (Vector2)

- `Preview` <= [5] TexCoord[0] output 0

### [7] Mask ( R G )

- `Input` <= [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [9] Mask ( B A )

- `Input` <= [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [11] Multiply

- `A` <= [6] TextureCropping output 2
- `B` <= [12] 1,4 output 0

### [13] Subtract

- `A` <= [11] Multiply output 0
- `B` <= [14] 0,1 output 0

### [16] Subtract

- `A` <= [11] Multiply output 0
- `B` <= [15] 0,2 output 0

### [18] Dot

- `A` <= [28] Append output 0
- `B` <= [20] 1,1,1,1 output 0

### [19] Output GreyScaleOutput

- `A` <= [18] Dot output 0

### [23] Subtract

- `A` <= [11] Multiply output 0
- `B` <= [24] 0,3 output 0

### [25] Append

- `A` <= [1] DebugScalarValues output 0
- `B` <= [2] DebugScalarValues output 0

### [26] Append

- `A` <= [25] Append output 0
- `B` <= [3] DebugScalarValues output 0

### [27] Add

- `A` <= [26] Append output 0
- `B` <= [21] DebugScalarValues output 0

### [28] Append

- `A` <= [26] Append output 0
- `B` <= [21] DebugScalarValues output 0

### [29] Append

- `A` <= [27] Add output 0
- `B` <= [21] DebugScalarValues output 0

## Connection List

- [29] Append output 0 -> Output ColorCodedOutput.A
- [5] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( R G ).Input
- [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( B A ).Input
- [6] TextureCropping output 2 -> Multiply.A
- [12] 1,4 output 0 -> Multiply.B
- [11] Multiply output 0 -> Subtract.A
- [14] 0,1 output 0 -> Subtract.B
- [11] Multiply output 0 -> Subtract.A
- [15] 0,2 output 0 -> Subtract.B
- [28] Append output 0 -> Dot.A
- [20] 1,1,1,1 output 0 -> Dot.B
- [18] Dot output 0 -> Output GreyScaleOutput.A
- [11] Multiply output 0 -> Subtract.A
- [24] 0,3 output 0 -> Subtract.B
- [1] DebugScalarValues output 0 -> Append.A
- [2] DebugScalarValues output 0 -> Append.B
- [25] Append output 0 -> Append.A
- [3] DebugScalarValues output 0 -> Append.B
- [26] Append output 0 -> Add.A
- [21] DebugScalarValues output 0 -> Add.B
- [26] Append output 0 -> Append.A
- [21] DebugScalarValues output 0 -> Append.B
- [27] Add output 0 -> Append.A
- [21] DebugScalarValues output 0 -> Append.B

## Output Trace Roots

- `ColorCodedOutput` is driven by [29] Append
- `GreyScaleOutput` is driven by [18] Dot

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


