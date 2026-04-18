# QuatAxisAngleConversions

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/QuatAxisAngleConversions.QuatAxisAngleConversions`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Convert a quat to an angle axis and vice versa.

## Inputs

- `AxisAngle` [FunctionInput_Vector4]
- `Quaternion` [FunctionInput_Vector4]

## Outputs

- `Axis Angle As Quat`
- `Quat as Axis Angle`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Axis Angle As Quat | Axis Angle As Quat |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input AxisAngle (Vector4) | AxisAngle |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Quaternion (Vector4) | Quaternion |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output Quat as Axis Angle | Quat as Axis Angle |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 6 | expression | `MaterialExpressionCosine` | Cosine |  |
| 7 | expression | `MaterialExpressionSine` | Sine |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionArccosine` | Arccosine |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |
| 14 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 15 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 18 | expression | `MaterialExpressionAppendVector` | Append |  |
| 19 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |

## Exact Input Wiring

### [0] Output Axis Angle As Quat

- `A` <= [10] Append output 0

### [3] Output Quat as Axis Angle

- `A` <= [18] Append output 0

### [5] Multiply(,0.5)

- `A` <= [4] BreakOutFloat4Components output 3

### [6] Cosine

- `Input` <= [5] Multiply(,0.5) output 0

### [7] Sine

- `Input` <= [5] Multiply(,0.5) output 0

### [8] Mask ( R G B )

- `Input` <= [1] Input AxisAngle (Vector4) output 0

### [9] Multiply

- `A` <= [8] Mask ( R G B ) output 0
- `B` <= [7] Sine output 0

### [10] Append

- `A` <= [9] Multiply output 0
- `B` <= [6] Cosine output 0

### [11] Arccosine

- `Input` <= [19] Mask ( A ) output 0

### [12] Multiply(,2)

- `A` <= [11] Arccosine output 0

### [13] Divide

- `A` <= [17] Mask ( R G B ) output 0
- `B` <= [14] Sqrt output 0

### [14] Sqrt

- `Input` <= [16] Multiply output 0

### [15] 1-x

- `Input` <= [19] Mask ( A ) output 0

### [16] Multiply

- `A` <= [15] 1-x output 0
- `B` <= [19] Mask ( A ) output 0

### [17] Mask ( R G B )

- `Input` <= [2] Input Quaternion (Vector4) output 0

### [18] Append

- `A` <= [13] Divide output 0
- `B` <= [12] Multiply(,2) output 0

### [19] Mask ( A )

- `Input` <= [2] Input Quaternion (Vector4) output 0

## Connection List

- [10] Append output 0 -> Output Axis Angle As Quat.A
- [18] Append output 0 -> Output Quat as Axis Angle.A
- [4] BreakOutFloat4Components output 3 -> Multiply(,0.5).A
- [5] Multiply(,0.5) output 0 -> Cosine.Input
- [5] Multiply(,0.5) output 0 -> Sine.Input
- [1] Input AxisAngle (Vector4) output 0 -> Mask ( R G B ).Input
- [8] Mask ( R G B ) output 0 -> Multiply.A
- [7] Sine output 0 -> Multiply.B
- [9] Multiply output 0 -> Append.A
- [6] Cosine output 0 -> Append.B
- [19] Mask ( A ) output 0 -> Arccosine.Input
- [11] Arccosine output 0 -> Multiply(,2).A
- [17] Mask ( R G B ) output 0 -> Divide.A
- [14] Sqrt output 0 -> Divide.B
- [16] Multiply output 0 -> Sqrt.Input
- [19] Mask ( A ) output 0 -> 1-x.Input
- [15] 1-x output 0 -> Multiply.A
- [19] Mask ( A ) output 0 -> Multiply.B
- [2] Input Quaternion (Vector4) output 0 -> Mask ( R G B ).Input
- [13] Divide output 0 -> Append.A
- [12] Multiply(,2) output 0 -> Append.B
- [2] Input Quaternion (Vector4) output 0 -> Mask ( A ).Input

## Output Trace Roots

- `Axis Angle As Quat` is driven by [10] Append
- `Quat as Axis Angle` is driven by [18] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


