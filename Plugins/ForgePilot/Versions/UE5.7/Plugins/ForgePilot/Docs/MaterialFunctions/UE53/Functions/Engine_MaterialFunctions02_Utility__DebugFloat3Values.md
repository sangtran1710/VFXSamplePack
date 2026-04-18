# DebugFloat3Values

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugFloat3Values.DebugFloat3Values`
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
- `Vector3` [FunctionInput_Vector3] - Enter a Vector to view.
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
| 12 | expression | `MaterialExpressionConstant2Vector` | 1,3 |  |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 15 | expression | `MaterialExpressionConstant2Vector` | 0,2 |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Vector3 (Vector3) | Vector3 |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 19 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output GreyScaleOutput | GreyScaleOutput |
| 21 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionAppendVector` | Append |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Component Spacing (Vector4) | Component Spacing |

## Exact Input Wiring

### [0] Output ColorCodedOutput

- `A` <= [23] Append output 0

### [4] Input UVs (Vector2)

- `Preview` <= [5] TexCoord[0] output 0

### [7] Mask ( R G )

- `Input` <= [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [9] Mask ( B A )

- `Input` <= [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0

### [11] Multiply

- `A` <= [6] TextureCropping output 2
- `B` <= [12] 1,3 output 0

### [13] Subtract

- `A` <= [11] Multiply output 0
- `B` <= [14] 0,1 output 0

### [16] Subtract

- `A` <= [11] Multiply output 0
- `B` <= [15] 0,2 output 0

### [19] Dot

- `A` <= [23] Append output 0
- `B` <= [21] 1,1,1 output 0

### [20] Output GreyScaleOutput

- `A` <= [19] Dot output 0

### [22] Append

- `A` <= [1] DebugScalarValues output 0
- `B` <= [2] DebugScalarValues output 0

### [23] Append

- `A` <= [22] Append output 0
- `B` <= [3] DebugScalarValues output 0

## Connection List

- [23] Append output 0 -> Output ColorCodedOutput.A
- [5] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( R G ).Input
- [8] Input DebugTextLocation RG_UpperRight BA_LowerLeft (Vector4) output 0 -> Mask ( B A ).Input
- [6] TextureCropping output 2 -> Multiply.A
- [12] 1,3 output 0 -> Multiply.B
- [11] Multiply output 0 -> Subtract.A
- [14] 0,1 output 0 -> Subtract.B
- [11] Multiply output 0 -> Subtract.A
- [15] 0,2 output 0 -> Subtract.B
- [23] Append output 0 -> Dot.A
- [21] 1,1,1 output 0 -> Dot.B
- [19] Dot output 0 -> Output GreyScaleOutput.A
- [1] DebugScalarValues output 0 -> Append.A
- [2] DebugScalarValues output 0 -> Append.B
- [22] Append output 0 -> Append.A
- [3] DebugScalarValues output 0 -> Append.B

## Output Trace Roots

- `ColorCodedOutput` is driven by [23] Append
- `GreyScaleOutput` is driven by [19] Dot

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
