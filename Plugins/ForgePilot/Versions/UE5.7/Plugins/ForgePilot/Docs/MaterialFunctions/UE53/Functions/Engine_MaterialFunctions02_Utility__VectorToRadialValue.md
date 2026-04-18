# VectorToRadialValue

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/VectorToRadialValue.VectorToRadialValue`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The input coordinates or vector will be transformed into radial coordinates. In the case of a vector the angle will be output in one channel and the length of the vector in the other.

## Inputs

- `Swizzle Coordinate Output` [FunctionInput_StaticBool] - Switch the order of the channels exported from the "radial coordinates" output.
- `Vector or UVs` [FunctionInput_Vector2] - Insert a vector 2 value to retrieve the vectors angle or insert uvs to retrieve radial coordinates. Note that (0,0) is not an acceptable number.  

## Outputs

- `Radial Coordinates` - The input coordinates or vector will be transformed into radial coordinates. In the case of a vector the angle will be output in one channel and the length of the vector in the other.
- `Vector Converted to Angle` - This will return the angle of of the input vector or a radial gradient in the case of input uvs.
- `Linear Distance` - Returns the linear length of the input vector or a gradient of distances in the case of uvs.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Radial Coordinates | Radial Coordinates |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 3 | custom | `MaterialExpressionCustom` | atan2 |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | expression | `MaterialExpressionConstant` | 0 |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionConstant` | 6.283 |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionIf` | If |  |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Vector Converted to Angle | Vector Converted to Angle |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Linear Distance | Linear Distance |
| 19 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Swizzle Coordinate Output (StaticBool) | Swizzle Coordinate Output |
| 21 | expression | `MaterialExpressionAppendVector` | Append |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Vector or UVs (Vector2) | Vector or UVs |
| 23 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 24 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 25 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 26 | expression | `MaterialExpressionConstant` | 1 |  |
| 27 | expression | `MaterialExpressionFrac` | Frac |  |

## Exact Input Wiring

### [0] Output Radial Coordinates

- `A` <= [19] Switch output 0

### [1] Mask ( R )

- `Input` <= [22] Input Vector or UVs (Vector2) output 0

### [2] Mask ( G )

- `Input` <= [22] Input Vector or UVs (Vector2) output 0

### [4] Mask ( R )

- `Input` <= [3] atan2 output 0

### [6] Add

- `A` <= [4] Mask ( R ) output 0
- `B` <= [7] 6.283 output 0

### [8] Divide

- `A` <= [6] Add output 0
- `B` <= [7] 6.283 output 0

### [9] Divide

- `A` <= [4] Mask ( R ) output 0
- `B` <= [7] 6.283 output 0

### [10] If

- `A` <= [4] Mask ( R ) output 0
- `B` <= [5] 0 output 0
- `AGreaterThanB` <= [9] Divide output 0
- `ALessThanB` <= [8] Divide output 0

### [11] Append

- `A` <= [16] Sqrt output 0
- `B` <= [27] Frac output 0

### [12] Multiply

- `A` <= [22] Input Vector or UVs (Vector2) output 0
- `B` <= [22] Input Vector or UVs (Vector2) output 0

### [13] Mask ( R )

- `Input` <= [12] Multiply output 0

### [14] Mask ( G )

- `Input` <= [12] Multiply output 0

### [15] Add

- `A` <= [13] Mask ( R ) output 0
- `B` <= [14] Mask ( G ) output 0

### [16] Sqrt

- `Input` <= [15] Add output 0

### [17] Output Vector Converted to Angle

- `A` <= [27] Frac output 0

### [18] Output Linear Distance

- `A` <= [16] Sqrt output 0

### [19] Switch

- `A` <= [21] Append output 0
- `B` <= [11] Append output 0
- `Value` <= [20] Input Swizzle Coordinate Output (StaticBool) output 0

### [20] Input Swizzle Coordinate Output (StaticBool)

- `Preview` <= [23] Static Bool (True) output 0

### [21] Append

- `A` <= [27] Frac output 0
- `B` <= [16] Sqrt output 0

### [22] Input Vector or UVs (Vector2)

- `Preview` <= [25] Subtract output 0

### [25] Subtract

- `A` <= [24] TexCoord[0] output 0
- `B` <= [26] 1 output 0

### [27] Frac

- `Input` <= [9] Divide output 0

## Connection List

- [19] Switch output 0 -> Output Radial Coordinates.A
- [22] Input Vector or UVs (Vector2) output 0 -> Mask ( R ).Input
- [22] Input Vector or UVs (Vector2) output 0 -> Mask ( G ).Input
- [3] atan2 output 0 -> Mask ( R ).Input
- [4] Mask ( R ) output 0 -> Add.A
- [7] 6.283 output 0 -> Add.B
- [6] Add output 0 -> Divide.A
- [7] 6.283 output 0 -> Divide.B
- [4] Mask ( R ) output 0 -> Divide.A
- [7] 6.283 output 0 -> Divide.B
- [4] Mask ( R ) output 0 -> If.A
- [5] 0 output 0 -> If.B
- [9] Divide output 0 -> If.AGreaterThanB
- [8] Divide output 0 -> If.ALessThanB
- [16] Sqrt output 0 -> Append.A
- [27] Frac output 0 -> Append.B
- [22] Input Vector or UVs (Vector2) output 0 -> Multiply.A
- [22] Input Vector or UVs (Vector2) output 0 -> Multiply.B
- [12] Multiply output 0 -> Mask ( R ).Input
- [12] Multiply output 0 -> Mask ( G ).Input
- [13] Mask ( R ) output 0 -> Add.A
- [14] Mask ( G ) output 0 -> Add.B
- [15] Add output 0 -> Sqrt.Input
- [27] Frac output 0 -> Output Vector Converted to Angle.A
- [16] Sqrt output 0 -> Output Linear Distance.A
- [21] Append output 0 -> Switch.A
- [11] Append output 0 -> Switch.B
- [20] Input Swizzle Coordinate Output (StaticBool) output 0 -> Switch.Value
- [23] Static Bool (True) output 0 -> Input Swizzle Coordinate Output (StaticBool).Preview
- [27] Frac output 0 -> Append.A
- [16] Sqrt output 0 -> Append.B
- [25] Subtract output 0 -> Input Vector or UVs (Vector2).Preview
- [24] TexCoord[0] output 0 -> Subtract.A
- [26] 1 output 0 -> Subtract.B
- [9] Divide output 0 -> Frac.Input

## Output Trace Roots

- `Radial Coordinates` is driven by [19] Switch
- `Vector Converted to Angle` is driven by [27] Frac
- `Linear Distance` is driven by [16] Sqrt

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
