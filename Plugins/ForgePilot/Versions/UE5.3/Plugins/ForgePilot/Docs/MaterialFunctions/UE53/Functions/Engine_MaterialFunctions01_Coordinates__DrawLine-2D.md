# DrawLine-2D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/DrawLine-2D.DrawLine-2D`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Draws a line between 2 points using supplied 2D coordinates. 

## Inputs

- `Coordinates` [FunctionInput_Vector2]
- `P0` [FunctionInput_Vector2]
- `P1` [FunctionInput_Vector2]
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
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 10 | expression | `MaterialExpressionAbs` | Abs |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionDivide` | Divide |  |
| 13 | expression | `MaterialExpressionClamp` | Clamp |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionDivide` | Divide |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | expression | `MaterialExpressionDivide` | Divide |  |
| 18 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 19 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 20 | expression | `MaterialExpressionClamp` | Clamp |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 22 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector2) | Coordinates |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input P0 (Vector2) | P0 |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input P1 (Vector2) | P1 |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Line Width (Scalar) | Line Width |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Edge Falloff (Scalar) | Edge Falloff |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 30 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 31 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 32 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 33 | expression | `MaterialExpressionDivide` | Divide |  |
| 34 | expression | `MaterialExpressionClamp` | Clamp |  |
| 35 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 36 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 37 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 38 | expression | `MaterialExpressionClamp` | Clamp |  |
| 39 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 40 | expression | `MaterialExpressionDivide` | Divide |  |
| 41 | expression | `MaterialExpressionMin` | Min |  |
| 42 | expression | `MaterialExpressionMin` | Min |  |
| 43 | expression | `MaterialExpressionMin` | Min |  |
| 44 | expression | `MaterialExpressionAdd` | Add |  |
| 45 | expression | `MaterialExpressionDistance` | Distance |  |
| 46 | expression | `MaterialExpressionConstant` | 0 |  |
| 47 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 48 | expression | `MaterialExpressionMin` | Min |  |
| 49 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 50 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 51 | function_input | `MaterialExpressionFunctionInput` | Input Square Corners? (StaticBool) | Square Corners? |
| 52 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 53 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 54 | function_call | `MaterialExpressionMaterialFunctionCall` | Swizzle |  |
| 55 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 56 | expression | `MaterialExpressionConstant2Vector` | 1,-1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [53] Switch output 0

### [1] Subtract

- `A` <= [25] Input P1 (Vector2) output 0
- `B` <= [24] Input P0 (Vector2) output 0

### [3] Dot

- `A` <= [6] Subtract output 0
- `B` <= [55] Multiply output 0

### [4] Dot

- `A` <= [6] Subtract output 0
- `B` <= [22] Normalize output 0

### [5] Append

- `A` <= [3] Dot output 0
- `B` <= [4] Dot output 0

### [6] Subtract

- `A` <= [23] Input Coordinates (Vector2) output 0
- `B` <= [24] Input P0 (Vector2) output 0

### [7] Mask ( G )

- `Input` <= [5] Append output 0

### [8] Clamp

- `Input` <= [12] Divide output 0

### [9] 1-x

- `Input` <= [8] Clamp output 0

### [10] Abs

- `Input` <= [5] Append output 0

### [11] Subtract

- `A` <= [10] Abs output 0
- `B` <= [26] Input Line Width (Scalar) output 0

### [12] Divide

- `A` <= [11] Subtract output 0
- `B` <= [28] Multiply output 0

### [13] Clamp

- `Input` <= [15] Divide output 0

### [14] Add

- `A` <= [26] Input Line Width (Scalar) output 0
- `B` <= [44] Add output 0

### [15] Divide

- `A` <= [14] Add output 0
- `B` <= [28] Multiply output 0

### [16] Subtract

- `A` <= [7] Mask ( G ) output 0
- `B` <= [21] VectorLength output 1

### [17] Divide

- `A` <= [18] Subtract output 0
- `B` <= [28] Multiply output 0

### [18] Subtract

- `A` <= [16] Subtract output 0
- `B` <= [26] Input Line Width (Scalar) output 0

### [19] 1-x

- `Input` <= [20] Clamp output 0

### [20] Clamp

- `Input` <= [17] Divide output 0

### [22] Normalize

- `VectorInput` <= [1] Subtract output 0

### [23] Input Coordinates (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [28] Multiply

- `A` <= [26] Input Line Width (Scalar) output 0
- `B` <= [27] Input Edge Falloff (Scalar) output 0

### [29] Subtract

- `A` <= [23] Input Coordinates (Vector2) output 0
- `B` <= [24] Input P0 (Vector2) output 0

### [30] Subtract

- `A` <= [25] Input P1 (Vector2) output 0
- `B` <= [24] Input P0 (Vector2) output 0

### [31] Dot

- `A` <= [29] Subtract output 0
- `B` <= [30] Subtract output 0

### [32] Dot

- `A` <= [30] Subtract output 0
- `B` <= [30] Subtract output 0

### [33] Divide

- `A` <= [31] Dot output 0
- `B` <= [32] Dot output 0

### [34] Clamp

- `Input` <= [33] Divide output 0

### [35] Subtract

- `A` <= [29] Subtract output 0
- `B` <= [36] Multiply output 0

### [36] Multiply

- `A` <= [34] Clamp output 0
- `B` <= [30] Subtract output 0

### [37] Subtract

- `A` <= [45] Distance output 0
- `B` <= [26] Input Line Width (Scalar) output 0

### [38] Clamp

- `Input` <= [40] Divide output 0

### [39] 1-x

- `Input` <= [38] Clamp output 0

### [40] Divide

- `A` <= [37] Subtract output 0
- `B` <= [28] Multiply output 0

### [41] Min

- `A` <= [43] Min output 0
- `B` <= [13] Clamp output 0

### [42] Min

- `A` <= [41] Min output 0
- `B` <= [19] 1-x output 0

### [43] Min

- `A` <= [49] Mask ( R ) output 0
- `B` <= [13] Clamp output 0

### [44] Add

- `A` <= [7] Mask ( G ) output 0
- `B` <= [28] Multiply output 0

### [45] Distance

- `A` <= [35] Subtract output 0
- `B` <= [46] 0 output 0

### [48] Min

- `A` <= [49] Mask ( R ) output 0
- `B` <= [50] Mask ( B ) output 0

### [49] Mask ( R )

- `Input` <= [9] 1-x output 0

### [50] Mask ( B )

- `Input` <= [9] 1-x output 0

### [51] Input Square Corners? (StaticBool)

- `Preview` <= [52] Static Bool (True) output 0

### [53] Switch

- `A` <= [42] Min output 0
- `B` <= [39] 1-x output 0
- `Value` <= [51] Input Square Corners? (StaticBool) output 0

### [55] Multiply

- `A` <= [54] Swizzle output 1
- `B` <= [56] 1,-1 output 0

## Connection List

- [53] Switch output 0 -> Output Result.A
- [25] Input P1 (Vector2) output 0 -> Subtract.A
- [24] Input P0 (Vector2) output 0 -> Subtract.B
- [6] Subtract output 0 -> Dot.A
- [55] Multiply output 0 -> Dot.B
- [6] Subtract output 0 -> Dot.A
- [22] Normalize output 0 -> Dot.B
- [3] Dot output 0 -> Append.A
- [4] Dot output 0 -> Append.B
- [23] Input Coordinates (Vector2) output 0 -> Subtract.A
- [24] Input P0 (Vector2) output 0 -> Subtract.B
- [5] Append output 0 -> Mask ( G ).Input
- [12] Divide output 0 -> Clamp.Input
- [8] Clamp output 0 -> 1-x.Input
- [5] Append output 0 -> Abs.Input
- [10] Abs output 0 -> Subtract.A
- [26] Input Line Width (Scalar) output 0 -> Subtract.B
- [11] Subtract output 0 -> Divide.A
- [28] Multiply output 0 -> Divide.B
- [15] Divide output 0 -> Clamp.Input
- [26] Input Line Width (Scalar) output 0 -> Add.A
- [44] Add output 0 -> Add.B
- [14] Add output 0 -> Divide.A
- [28] Multiply output 0 -> Divide.B
- [7] Mask ( G ) output 0 -> Subtract.A
- [21] VectorLength output 1 -> Subtract.B
- [18] Subtract output 0 -> Divide.A
- [28] Multiply output 0 -> Divide.B
- [16] Subtract output 0 -> Subtract.A
- [26] Input Line Width (Scalar) output 0 -> Subtract.B
- [20] Clamp output 0 -> 1-x.Input
- [17] Divide output 0 -> Clamp.Input
- [1] Subtract output 0 -> Normalize.VectorInput
- [2] TexCoord[0] output 0 -> Input Coordinates (Vector2).Preview
- [26] Input Line Width (Scalar) output 0 -> Multiply.A
- [27] Input Edge Falloff (Scalar) output 0 -> Multiply.B
- [23] Input Coordinates (Vector2) output 0 -> Subtract.A
- [24] Input P0 (Vector2) output 0 -> Subtract.B
- [25] Input P1 (Vector2) output 0 -> Subtract.A
- [24] Input P0 (Vector2) output 0 -> Subtract.B
- [29] Subtract output 0 -> Dot.A
- [30] Subtract output 0 -> Dot.B
- [30] Subtract output 0 -> Dot.A
- [30] Subtract output 0 -> Dot.B
- [31] Dot output 0 -> Divide.A
- [32] Dot output 0 -> Divide.B
- [33] Divide output 0 -> Clamp.Input
- [29] Subtract output 0 -> Subtract.A
- [36] Multiply output 0 -> Subtract.B
- [34] Clamp output 0 -> Multiply.A
- [30] Subtract output 0 -> Multiply.B
- [45] Distance output 0 -> Subtract.A
- [26] Input Line Width (Scalar) output 0 -> Subtract.B
- [40] Divide output 0 -> Clamp.Input
- [38] Clamp output 0 -> 1-x.Input
- [37] Subtract output 0 -> Divide.A
- [28] Multiply output 0 -> Divide.B
- [43] Min output 0 -> Min.A
- [13] Clamp output 0 -> Min.B
- [41] Min output 0 -> Min.A
- [19] 1-x output 0 -> Min.B
- [49] Mask ( R ) output 0 -> Min.A
- [13] Clamp output 0 -> Min.B
- [7] Mask ( G ) output 0 -> Add.A
- [28] Multiply output 0 -> Add.B
- [35] Subtract output 0 -> Distance.A
- [46] 0 output 0 -> Distance.B
- [49] Mask ( R ) output 0 -> Min.A
- [50] Mask ( B ) output 0 -> Min.B
- [9] 1-x output 0 -> Mask ( R ).Input
- [9] 1-x output 0 -> Mask ( B ).Input
- [52] Static Bool (True) output 0 -> Input Square Corners? (StaticBool).Preview
- [42] Min output 0 -> Switch.A
- [39] 1-x output 0 -> Switch.B
- [51] Input Square Corners? (StaticBool) output 0 -> Switch.Value
- [54] Swizzle output 1 -> Multiply.A
- [56] 1,-1 output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [53] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
