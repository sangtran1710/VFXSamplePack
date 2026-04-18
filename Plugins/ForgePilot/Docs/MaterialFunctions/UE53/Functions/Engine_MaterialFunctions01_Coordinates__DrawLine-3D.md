# DrawLine-3D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/DrawLine-3D.DrawLine-3D`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Draws a line between 2 points using supplied 3D coordinates. 

## Inputs

- `Coordinates` [FunctionInput_Vector3]
- `P0` [FunctionInput_Vector3]
- `P1` [FunctionInput_Vector3]
- `Line Width` [FunctionInput_Scalar]
- `Edge Falloff` [FunctionInput_Scalar]
- `Square Corners?` [FunctionInput_StaticBool]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 9 | expression | `MaterialExpressionClamp` | Clamp |  |
| 10 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 11 | expression | `MaterialExpressionAbs` | Abs |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |
| 14 | expression | `MaterialExpressionClamp` | Clamp |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 18 | expression | `MaterialExpressionDivide` | Divide |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 21 | expression | `MaterialExpressionClamp` | Clamp |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 23 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector3) | Coordinates |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input P0 (Vector3) | P0 |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input P1 (Vector3) | P1 |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Line Width (Scalar) | Line Width |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Edge Falloff (Scalar) | Edge Falloff |
| 29 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 30 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 31 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 32 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 33 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 34 | expression | `MaterialExpressionDivide` | Divide |  |
| 35 | expression | `MaterialExpressionClamp` | Clamp |  |
| 36 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 39 | expression | `MaterialExpressionClamp` | Clamp |  |
| 40 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 41 | expression | `MaterialExpressionDivide` | Divide |  |
| 42 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input Square Corners? (StaticBool) | Square Corners? |
| 44 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 45 | expression | `MaterialExpressionMin` | Min |  |
| 46 | expression | `MaterialExpressionMin` | Min |  |
| 47 | expression | `MaterialExpressionMin` | Min |  |
| 48 | expression | `MaterialExpressionAdd` | Add |  |
| 49 | expression | `MaterialExpressionAppendVector` | Append |  |
| 50 | expression | `MaterialExpressionConstant` | 0 |  |
| 51 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 52 | expression | `MaterialExpressionDistance` | Distance |  |
| 53 | expression | `MaterialExpressionConstant` | 0 |  |
| 54 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 55 | expression | `MaterialExpressionConstant3Vector` | 0,0.001,1 |  |
| 56 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 57 | expression | `MaterialExpressionAppendVector` | Append |  |
| 58 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 59 | expression | `MaterialExpressionMin` | Min |  |
| 60 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 61 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [42] Switch output 0

### [1] Subtract

- `A` <= [26] Input P1 (Vector3) output 0
- `B` <= [25] Input P0 (Vector3) output 0

### [3] Dot

- `A` <= [6] Subtract output 0
- `B` <= [54] CreateThirdOrthogonalVector output 2

### [4] Dot

- `A` <= [6] Subtract output 0
- `B` <= [54] CreateThirdOrthogonalVector output 0

### [5] Append

- `A` <= [3] Dot output 0
- `B` <= [4] Dot output 0

### [6] Subtract

- `A` <= [24] Input Coordinates (Vector3) output 0
- `B` <= [25] Input P0 (Vector3) output 0

### [7] Mask ( R G )

- `Input` <= [57] Append output 0

### [8] Mask ( G )

- `Input` <= [57] Append output 0

### [9] Clamp

- `Input` <= [13] Divide output 0

### [10] 1-x

- `Input` <= [9] Clamp output 0

### [11] Abs

- `Input` <= [57] Append output 0

### [12] Subtract

- `A` <= [11] Abs output 0
- `B` <= [27] Input Line Width (Scalar) output 0

### [13] Divide

- `A` <= [12] Subtract output 0
- `B` <= [29] Multiply output 0

### [14] Clamp

- `Input` <= [16] Divide output 0

### [15] Add

- `A` <= [27] Input Line Width (Scalar) output 0
- `B` <= [48] Add output 0

### [16] Divide

- `A` <= [15] Add output 0
- `B` <= [29] Multiply output 0

### [17] Subtract

- `A` <= [8] Mask ( G ) output 0
- `B` <= [22] VectorLength output 1

### [18] Divide

- `A` <= [19] Subtract output 0
- `B` <= [29] Multiply output 0

### [19] Subtract

- `A` <= [17] Subtract output 0
- `B` <= [27] Input Line Width (Scalar) output 0

### [20] 1-x

- `Input` <= [21] Clamp output 0

### [21] Clamp

- `Input` <= [18] Divide output 0

### [23] Normalize

- `VectorInput` <= [1] Subtract output 0

### [24] Input Coordinates (Vector3)

- `Preview` <= [49] Append output 0

### [29] Multiply

- `A` <= [27] Input Line Width (Scalar) output 0
- `B` <= [28] Input Edge Falloff (Scalar) output 0

### [30] Subtract

- `A` <= [24] Input Coordinates (Vector3) output 0
- `B` <= [25] Input P0 (Vector3) output 0

### [31] Subtract

- `A` <= [26] Input P1 (Vector3) output 0
- `B` <= [25] Input P0 (Vector3) output 0

### [32] Dot

- `A` <= [30] Subtract output 0
- `B` <= [31] Subtract output 0

### [33] Dot

- `A` <= [31] Subtract output 0
- `B` <= [31] Subtract output 0

### [34] Divide

- `A` <= [32] Dot output 0
- `B` <= [33] Dot output 0

### [35] Clamp

- `Input` <= [34] Divide output 0

### [36] Subtract

- `A` <= [30] Subtract output 0
- `B` <= [37] Multiply output 0

### [37] Multiply

- `A` <= [35] Clamp output 0
- `B` <= [31] Subtract output 0

### [38] Subtract

- `A` <= [52] Distance output 0
- `B` <= [27] Input Line Width (Scalar) output 0

### [39] Clamp

- `Input` <= [41] Divide output 0

### [40] 1-x

- `Input` <= [39] Clamp output 0

### [41] Divide

- `A` <= [38] Subtract output 0
- `B` <= [29] Multiply output 0

### [42] Switch

- `A` <= [46] Min output 0
- `B` <= [40] 1-x output 0
- `Value` <= [43] Input Square Corners? (StaticBool) output 0

### [43] Input Square Corners? (StaticBool)

- `Preview` <= [44] Static Bool (True) output 0

### [45] Min

- `A` <= [47] Min output 0
- `B` <= [14] Clamp output 0

### [46] Min

- `A` <= [45] Min output 0
- `B` <= [20] 1-x output 0

### [47] Min

- `A` <= [59] Min output 0
- `B` <= [14] Clamp output 0

### [48] Add

- `A` <= [8] Mask ( G ) output 0
- `B` <= [29] Multiply output 0

### [49] Append

- `A` <= [2] TexCoord[0] output 0
- `B` <= [50] 0 output 0

### [51] Mask ( R G )

- `Input` <= [1] Subtract output 0

### [52] Distance

- `A` <= [36] Subtract output 0
- `B` <= [53] 0 output 0

### [56] Dot

- `A` <= [6] Subtract output 0
- `B` <= [54] CreateThirdOrthogonalVector output 1

### [57] Append

- `A` <= [5] Append output 0
- `B` <= [56] Dot output 0

### [59] Min

- `A` <= [60] Mask ( R ) output 0
- `B` <= [61] Mask ( B ) output 0

### [60] Mask ( R )

- `Input` <= [10] 1-x output 0

### [61] Mask ( B )

- `Input` <= [10] 1-x output 0

## Connection List

- [42] Switch output 0 -> Output Result.A
- [26] Input P1 (Vector3) output 0 -> Subtract.A
- [25] Input P0 (Vector3) output 0 -> Subtract.B
- [6] Subtract output 0 -> Dot.A
- [54] CreateThirdOrthogonalVector output 2 -> Dot.B
- [6] Subtract output 0 -> Dot.A
- [54] CreateThirdOrthogonalVector output 0 -> Dot.B
- [3] Dot output 0 -> Append.A
- [4] Dot output 0 -> Append.B
- [24] Input Coordinates (Vector3) output 0 -> Subtract.A
- [25] Input P0 (Vector3) output 0 -> Subtract.B
- [57] Append output 0 -> Mask ( R G ).Input
- [57] Append output 0 -> Mask ( G ).Input
- [13] Divide output 0 -> Clamp.Input
- [9] Clamp output 0 -> 1-x.Input
- [57] Append output 0 -> Abs.Input
- [11] Abs output 0 -> Subtract.A
- [27] Input Line Width (Scalar) output 0 -> Subtract.B
- [12] Subtract output 0 -> Divide.A
- [29] Multiply output 0 -> Divide.B
- [16] Divide output 0 -> Clamp.Input
- [27] Input Line Width (Scalar) output 0 -> Add.A
- [48] Add output 0 -> Add.B
- [15] Add output 0 -> Divide.A
- [29] Multiply output 0 -> Divide.B
- [8] Mask ( G ) output 0 -> Subtract.A
- [22] VectorLength output 1 -> Subtract.B
- [19] Subtract output 0 -> Divide.A
- [29] Multiply output 0 -> Divide.B
- [17] Subtract output 0 -> Subtract.A
- [27] Input Line Width (Scalar) output 0 -> Subtract.B
- [21] Clamp output 0 -> 1-x.Input
- [18] Divide output 0 -> Clamp.Input
- [1] Subtract output 0 -> Normalize.VectorInput
- [49] Append output 0 -> Input Coordinates (Vector3).Preview
- [27] Input Line Width (Scalar) output 0 -> Multiply.A
- [28] Input Edge Falloff (Scalar) output 0 -> Multiply.B
- [24] Input Coordinates (Vector3) output 0 -> Subtract.A
- [25] Input P0 (Vector3) output 0 -> Subtract.B
- [26] Input P1 (Vector3) output 0 -> Subtract.A
- [25] Input P0 (Vector3) output 0 -> Subtract.B
- [30] Subtract output 0 -> Dot.A
- [31] Subtract output 0 -> Dot.B
- [31] Subtract output 0 -> Dot.A
- [31] Subtract output 0 -> Dot.B
- [32] Dot output 0 -> Divide.A
- [33] Dot output 0 -> Divide.B
- [34] Divide output 0 -> Clamp.Input
- [30] Subtract output 0 -> Subtract.A
- [37] Multiply output 0 -> Subtract.B
- [35] Clamp output 0 -> Multiply.A
- [31] Subtract output 0 -> Multiply.B
- [52] Distance output 0 -> Subtract.A
- [27] Input Line Width (Scalar) output 0 -> Subtract.B
- [41] Divide output 0 -> Clamp.Input
- [39] Clamp output 0 -> 1-x.Input
- [38] Subtract output 0 -> Divide.A
- [29] Multiply output 0 -> Divide.B
- [46] Min output 0 -> Switch.A
- [40] 1-x output 0 -> Switch.B
- [43] Input Square Corners? (StaticBool) output 0 -> Switch.Value
- [44] Static Bool (True) output 0 -> Input Square Corners? (StaticBool).Preview
- [47] Min output 0 -> Min.A
- [14] Clamp output 0 -> Min.B
- [45] Min output 0 -> Min.A
- [20] 1-x output 0 -> Min.B
- [59] Min output 0 -> Min.A
- [14] Clamp output 0 -> Min.B
- [8] Mask ( G ) output 0 -> Add.A
- [29] Multiply output 0 -> Add.B
- [2] TexCoord[0] output 0 -> Append.A
- [50] 0 output 0 -> Append.B
- [1] Subtract output 0 -> Mask ( R G ).Input
- [36] Subtract output 0 -> Distance.A
- [53] 0 output 0 -> Distance.B
- [6] Subtract output 0 -> Dot.A
- [54] CreateThirdOrthogonalVector output 1 -> Dot.B
- [5] Append output 0 -> Append.A
- [56] Dot output 0 -> Append.B
- [60] Mask ( R ) output 0 -> Min.A
- [61] Mask ( B ) output 0 -> Min.B
- [10] 1-x output 0 -> Mask ( R ).Input
- [10] 1-x output 0 -> Mask ( B ).Input

## Output Trace Roots

- `Result` is driven by [42] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
