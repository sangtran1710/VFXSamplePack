# RotateAboutWorldAxis_cheap

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/RotateAboutWorldAxis_cheap.RotateAboutWorldAxis_cheap`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function cheaply rotates objects around world axes. Input the angle you would like to use and attach the output to world position offset.

## Inputs

- `PivotPoint` [FunctionInput_Vector3] - World space location of pivot point to rotate around. The default value is the model's pivot point.
- `WorldPosition` [FunctionInput_Vector3] - World space location of individual vertices. The WorldPosition node is typically used.
- `Rotation Amount` [FunctionInput_Scalar] - A value of one is equal to one full rotation.

## Outputs

- `Z-Axis` - Cheaply rotates the object around the world Z-axis. 
- `Y-Axis` - Cheaply rotates the object around the world Y-axis. 
- `X-Axis` - Cheaply rotates the object around the world X-axis. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Z-Axis | Z-Axis |
| 1 | expression | `MaterialExpressionCosine` | Cosine |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionSine` | Sine |  |
| 11 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionSine` | Sine |  |
| 15 | expression | `MaterialExpressionConstant` | 0 |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 18 | expression | `MaterialExpressionAppendVector` | Append |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionCosine` | Cosine |  |
| 24 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input PivotPoint (Vector3) | PivotPoint |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Rotation Amount (Scalar) | Rotation Amount |
| 28 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Y-Axis | Y-Axis |
| 30 | function_output | `MaterialExpressionFunctionOutput` | Output X-Axis | X-Axis |
| 31 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectPivotPoint |  |
| 32 | expression | `MaterialExpressionCosine` | Cosine |  |
| 33 | expression | `MaterialExpressionConstant` | 0 |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 36 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 37 | expression | `MaterialExpressionAdd` | Add |  |
| 38 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 39 | expression | `MaterialExpressionAppendVector` | Append |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionSine` | Sine |  |
| 42 | expression | `MaterialExpressionAdd` | Add |  |
| 43 | expression | `MaterialExpressionSine` | Sine |  |
| 44 | expression | `MaterialExpressionConstant` | 0 |  |
| 45 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 46 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 47 | expression | `MaterialExpressionAppendVector` | Append |  |
| 48 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 49 | expression | `MaterialExpressionAdd` | Add |  |
| 50 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 51 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 52 | expression | `MaterialExpressionCosine` | Cosine |  |
| 53 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 54 | expression | `MaterialExpressionConstant` | 0 |  |
| 55 | expression | `MaterialExpressionAdd` | Add |  |
| 56 | expression | `MaterialExpressionAppendVector` | Append |  |
| 57 | expression | `MaterialExpressionAdd` | Add |  |
| 58 | expression | `MaterialExpressionConstant` | 0 |  |
| 59 | expression | `MaterialExpressionAppendVector` | Append |  |
| 60 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 61 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 62 | expression | `MaterialExpressionCosine` | Cosine |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 64 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 65 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 66 | expression | `MaterialExpressionSine` | Sine |  |
| 67 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 68 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 69 | expression | `MaterialExpressionSine` | Sine |  |
| 70 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 72 | expression | `MaterialExpressionAdd` | Add |  |
| 73 | expression | `MaterialExpressionCosine` | Cosine |  |
| 74 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 75 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |

## Exact Input Wiring

### [0] Output Z-Axis

- `A` <= [19] Subtract output 0

### [1] Cosine

- `Input` <= [6] Add output 0

### [2] Multiply

- `A` <= [1] Cosine output 0
- `B` <= [3] Mask ( R ) output 0

### [3] Mask ( R )

- `Input` <= [13] Add output 0

### [5] Subtract

- `A` <= [2] Multiply output 0
- `B` <= [9] Multiply output 0

### [6] Add

- `A` <= [4] 0 output 0
- `B` <= [28] Mask ( R ) output 0

### [7] Mask ( G )

- `Input` <= [13] Add output 0

### [8] Append

- `A` <= [5] Subtract output 0
- `B` <= [20] Add output 0

### [9] Multiply

- `A` <= [7] Mask ( G ) output 0
- `B` <= [10] Sine output 0

### [10] Sine

- `Input` <= [6] Add output 0

### [12] Subtract

- `A` <= [26] Input WorldPosition (Vector3) output 0
- `B` <= [25] Input PivotPoint (Vector3) output 0

### [13] Add

- `A` <= [12] Subtract output 0
- `B` <= [15] 0 output 0

### [14] Sine

- `Input` <= [6] Add output 0

### [16] Multiply

- `A` <= [14] Sine output 0
- `B` <= [17] Mask ( R ) output 0

### [17] Mask ( R )

- `Input` <= [13] Add output 0

### [18] Append

- `A` <= [8] Append output 0
- `B` <= [24] Mask ( B ) output 0

### [19] Subtract

- `A` <= [18] Append output 0
- `B` <= [13] Add output 0

### [20] Add

- `A` <= [16] Multiply output 0
- `B` <= [22] Multiply output 0

### [21] Mask ( G )

- `Input` <= [13] Add output 0

### [22] Multiply

- `A` <= [21] Mask ( G ) output 0
- `B` <= [23] Cosine output 0

### [23] Cosine

- `Input` <= [6] Add output 0

### [24] Mask ( B )

- `Input` <= [13] Add output 0

### [25] Input PivotPoint (Vector3)

- `Preview` <= [31] ObjectPivotPoint output 0

### [26] Input WorldPosition (Vector3)

- `Preview` <= [11] Absolute World Position output 0

### [28] Mask ( R )

- `Input` <= [27] Input Rotation Amount (Scalar) output 0

### [29] Output Y-Axis

- `A` <= [48] Subtract output 0

### [30] Output X-Axis

- `A` <= [60] Subtract output 0

### [32] Cosine

- `Input` <= [37] Add output 0

### [34] Multiply

- `A` <= [32] Cosine output 0
- `B` <= [35] Mask ( B ) output 0

### [35] Mask ( B )

- `Input` <= [42] Add output 0

### [36] Subtract

- `A` <= [34] Multiply output 0
- `B` <= [40] Multiply output 0

### [37] Add

- `A` <= [33] 0 output 0
- `B` <= [28] Mask ( R ) output 0

### [38] Mask ( R )

- `Input` <= [42] Add output 0

### [39] Append

- `A` <= [36] Subtract output 0
- `B` <= [53] Mask ( G ) output 0

### [40] Multiply

- `A` <= [38] Mask ( R ) output 0
- `B` <= [41] Sine output 0

### [41] Sine

- `Input` <= [37] Add output 0

### [42] Add

- `A` <= [12] Subtract output 0
- `B` <= [44] 0 output 0

### [43] Sine

- `Input` <= [37] Add output 0

### [45] Multiply

- `A` <= [43] Sine output 0
- `B` <= [46] Mask ( B ) output 0

### [46] Mask ( B )

- `Input` <= [42] Add output 0

### [47] Append

- `A` <= [39] Append output 0
- `B` <= [49] Add output 0

### [48] Subtract

- `A` <= [47] Append output 0
- `B` <= [42] Add output 0

### [49] Add

- `A` <= [45] Multiply output 0
- `B` <= [51] Multiply output 0

### [50] Mask ( R )

- `Input` <= [42] Add output 0

### [51] Multiply

- `A` <= [50] Mask ( R ) output 0
- `B` <= [52] Cosine output 0

### [52] Cosine

- `Input` <= [37] Add output 0

### [53] Mask ( G )

- `Input` <= [42] Add output 0

### [55] Add

- `A` <= [54] 0 output 0
- `B` <= [28] Mask ( R ) output 0

### [56] Append

- `A` <= [61] Mask ( R ) output 0
- `B` <= [65] Subtract output 0

### [57] Add

- `A` <= [12] Subtract output 0
- `B` <= [58] 0 output 0

### [59] Append

- `A` <= [56] Append output 0
- `B` <= [72] Add output 0

### [60] Subtract

- `A` <= [59] Append output 0
- `B` <= [57] Add output 0

### [61] Mask ( R )

- `Input` <= [57] Add output 0

### [62] Cosine

- `Input` <= [55] Add output 0

### [63] Multiply

- `A` <= [62] Cosine output 0
- `B` <= [64] Mask ( G ) output 0

### [64] Mask ( G )

- `Input` <= [57] Add output 0

### [65] Subtract

- `A` <= [63] Multiply output 0
- `B` <= [67] Multiply output 0

### [66] Sine

- `Input` <= [55] Add output 0

### [67] Multiply

- `A` <= [66] Sine output 0
- `B` <= [68] Mask ( B ) output 0

### [68] Mask ( B )

- `Input` <= [57] Add output 0

### [69] Sine

- `Input` <= [55] Add output 0

### [70] Mask ( G )

- `Input` <= [57] Add output 0

### [71] Multiply

- `A` <= [69] Sine output 0
- `B` <= [70] Mask ( G ) output 0

### [72] Add

- `A` <= [71] Multiply output 0
- `B` <= [74] Multiply output 0

### [73] Cosine

- `Input` <= [55] Add output 0

### [74] Multiply

- `A` <= [73] Cosine output 0
- `B` <= [75] Mask ( B ) output 0

### [75] Mask ( B )

- `Input` <= [57] Add output 0

## Connection List

- [19] Subtract output 0 -> Output Z-Axis.A
- [6] Add output 0 -> Cosine.Input
- [1] Cosine output 0 -> Multiply.A
- [3] Mask ( R ) output 0 -> Multiply.B
- [13] Add output 0 -> Mask ( R ).Input
- [2] Multiply output 0 -> Subtract.A
- [9] Multiply output 0 -> Subtract.B
- [4] 0 output 0 -> Add.A
- [28] Mask ( R ) output 0 -> Add.B
- [13] Add output 0 -> Mask ( G ).Input
- [5] Subtract output 0 -> Append.A
- [20] Add output 0 -> Append.B
- [7] Mask ( G ) output 0 -> Multiply.A
- [10] Sine output 0 -> Multiply.B
- [6] Add output 0 -> Sine.Input
- [26] Input WorldPosition (Vector3) output 0 -> Subtract.A
- [25] Input PivotPoint (Vector3) output 0 -> Subtract.B
- [12] Subtract output 0 -> Add.A
- [15] 0 output 0 -> Add.B
- [6] Add output 0 -> Sine.Input
- [14] Sine output 0 -> Multiply.A
- [17] Mask ( R ) output 0 -> Multiply.B
- [13] Add output 0 -> Mask ( R ).Input
- [8] Append output 0 -> Append.A
- [24] Mask ( B ) output 0 -> Append.B
- [18] Append output 0 -> Subtract.A
- [13] Add output 0 -> Subtract.B
- [16] Multiply output 0 -> Add.A
- [22] Multiply output 0 -> Add.B
- [13] Add output 0 -> Mask ( G ).Input
- [21] Mask ( G ) output 0 -> Multiply.A
- [23] Cosine output 0 -> Multiply.B
- [6] Add output 0 -> Cosine.Input
- [13] Add output 0 -> Mask ( B ).Input
- [31] ObjectPivotPoint output 0 -> Input PivotPoint (Vector3).Preview
- [11] Absolute World Position output 0 -> Input WorldPosition (Vector3).Preview
- [27] Input Rotation Amount (Scalar) output 0 -> Mask ( R ).Input
- [48] Subtract output 0 -> Output Y-Axis.A
- [60] Subtract output 0 -> Output X-Axis.A
- [37] Add output 0 -> Cosine.Input
- [32] Cosine output 0 -> Multiply.A
- [35] Mask ( B ) output 0 -> Multiply.B
- [42] Add output 0 -> Mask ( B ).Input
- [34] Multiply output 0 -> Subtract.A
- [40] Multiply output 0 -> Subtract.B
- [33] 0 output 0 -> Add.A
- [28] Mask ( R ) output 0 -> Add.B
- [42] Add output 0 -> Mask ( R ).Input
- [36] Subtract output 0 -> Append.A
- [53] Mask ( G ) output 0 -> Append.B
- [38] Mask ( R ) output 0 -> Multiply.A
- [41] Sine output 0 -> Multiply.B
- [37] Add output 0 -> Sine.Input
- [12] Subtract output 0 -> Add.A
- [44] 0 output 0 -> Add.B
- [37] Add output 0 -> Sine.Input
- [43] Sine output 0 -> Multiply.A
- [46] Mask ( B ) output 0 -> Multiply.B
- [42] Add output 0 -> Mask ( B ).Input
- [39] Append output 0 -> Append.A
- [49] Add output 0 -> Append.B
- [47] Append output 0 -> Subtract.A
- [42] Add output 0 -> Subtract.B
- [45] Multiply output 0 -> Add.A
- [51] Multiply output 0 -> Add.B
- [42] Add output 0 -> Mask ( R ).Input
- [50] Mask ( R ) output 0 -> Multiply.A
- [52] Cosine output 0 -> Multiply.B
- [37] Add output 0 -> Cosine.Input
- [42] Add output 0 -> Mask ( G ).Input
- [54] 0 output 0 -> Add.A
- [28] Mask ( R ) output 0 -> Add.B
- [61] Mask ( R ) output 0 -> Append.A
- [65] Subtract output 0 -> Append.B
- [12] Subtract output 0 -> Add.A
- [58] 0 output 0 -> Add.B
- [56] Append output 0 -> Append.A
- [72] Add output 0 -> Append.B
- [59] Append output 0 -> Subtract.A
- [57] Add output 0 -> Subtract.B
- [57] Add output 0 -> Mask ( R ).Input
- [55] Add output 0 -> Cosine.Input
- [62] Cosine output 0 -> Multiply.A
- [64] Mask ( G ) output 0 -> Multiply.B
- [57] Add output 0 -> Mask ( G ).Input
- [63] Multiply output 0 -> Subtract.A
- [67] Multiply output 0 -> Subtract.B
- [55] Add output 0 -> Sine.Input
- [66] Sine output 0 -> Multiply.A
- [68] Mask ( B ) output 0 -> Multiply.B
- [57] Add output 0 -> Mask ( B ).Input
- [55] Add output 0 -> Sine.Input
- [57] Add output 0 -> Mask ( G ).Input
- [69] Sine output 0 -> Multiply.A
- [70] Mask ( G ) output 0 -> Multiply.B
- [71] Multiply output 0 -> Add.A
- [74] Multiply output 0 -> Add.B
- [55] Add output 0 -> Cosine.Input
- [73] Cosine output 0 -> Multiply.A
- [75] Mask ( B ) output 0 -> Multiply.B
- [57] Add output 0 -> Mask ( B ).Input

## Output Trace Roots

- `Z-Axis` is driven by [19] Subtract
- `Y-Axis` is driven by [48] Subtract
- `X-Axis` is driven by [60] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
