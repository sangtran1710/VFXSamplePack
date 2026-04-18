# ArrangePointsEvenlyAroundABox

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ArrangePointsEvenlyAroundABox.ArrangePointsEvenlyAroundABox`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This material function will evenly arrange points around a box. The dimensions and basis vectors are inputs along with a 0-1 placement value. This can be useful for evenly distributing models around an analytically described shape. 

## Inputs

- `XY Phase (0-1)` [FunctionInput_Scalar]
- `X Vector` [FunctionInput_Vector3] - Define the box's x basis vector and the length along it's axis.
- `Y Vector` [FunctionInput_Vector3] - Define the box's Y basis vector and the length along it's axis.
- `Position` [FunctionInput_Vector3] - This defines the boxes Center Location
- `Z Vector` [FunctionInput_Vector3] - Define the box's z basis vector and the length along it's axis.
- `Z Phase (0-1)` [FunctionInput_Scalar] - This value will place the points along the defined z vector.
- `Relative Pivot Location` [FunctionInput_Vector3]

## Outputs

- `Point Around Box` - If you wish to use this to generate a box in world space via world position offset, subtract world position from this output before running it into world position offset. 
- `World normal` - This is the world normal for the outward facing surface.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Point Around Box | Point Around Box |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionFloor` | Floor |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 11 | expression | `MaterialExpressionFloor` | Floor |  |
| 12 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 13 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 14 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 15 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | expression | `MaterialExpressionDivide` | Divide |  |
| 18 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 19 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 20 | expression | `MaterialExpressionFloor` | Floor |  |
| 21 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 22 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 23 | expression | `MaterialExpressionDivide` | Divide |  |
| 24 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 25 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 26 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 27 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 28 | expression | `MaterialExpressionAdd` | Add |  |
| 29 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 30 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 31 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 32 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 33 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionAdd` | Add |  |
| 36 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionAdd` | Add |  |
| 39 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input XY Phase (0-1) (Scalar) | XY Phase (0-1) |
| 42 | function_input | `MaterialExpressionFunctionInput` | Input X Vector (Vector3) | X Vector |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input Y Vector (Vector3) | Y Vector |
| 44 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 45 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 46 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 47 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 48 | expression | `MaterialExpressionDivide` | Divide |  |
| 49 | expression | `MaterialExpressionAdd` | Add |  |
| 50 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 51 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 52 | expression | `MaterialExpressionConstant` | 0 |  |
| 53 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 54 | function_input | `MaterialExpressionFunctionInput` | Input Position (Vector3) | Position |
| 55 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 56 | function_input | `MaterialExpressionFunctionInput` | Input Z Vector (Vector3) | Z Vector |
| 57 | expression | `MaterialExpressionAdd` | Add |  |
| 58 | function_input | `MaterialExpressionFunctionInput` | Input Z Phase (0-1) (Scalar) | Z Phase (0-1) |
| 59 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 60 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 61 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 62 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 63 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 64 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 65 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 66 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 67 | function_input | `MaterialExpressionFunctionInput` | Input Relative Pivot Location (Vector3) | Relative Pivot Location |
| 68 | expression | `MaterialExpressionAdd` | Add |  |
| 69 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 70 | function_output | `MaterialExpressionFunctionOutput` | Output World normal | World normal |
| 71 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 72 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 73 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 74 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 75 | expression | `MaterialExpressionConstant3Vector` | 0,-1,0 |  |
| 76 | expression | `MaterialExpressionConstant3Vector` | -1,0,0 |  |
| 77 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 78 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 79 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 80 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 81 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 82 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Point Around Box

- `A` <= [53] Transform3x3Matrix output 0

### [1] Lerp

- `A` <= [28] Add output 0
- `B` <= [10] Lerp output 0
- `Alpha` <= [81] Reroute Node (reroutes wires) output 0

### [3] Mask ( R )

- `Input` <= [2] TexCoord[0] output 0

### [4] Multiply(,0.5)

- `A` <= [55] Reroute Node (reroutes wires) output 0

### [5] Multiply(,0.5)

- `A` <= [13] 1-x output 0

### [6] Divide

- `A` <= [41] Input XY Phase (0-1) (Scalar) output 0
- `B` <= [4] Multiply(,0.5) output 0

### [7] Subtract

- `A` <= [41] Input XY Phase (0-1) (Scalar) output 0
- `B` <= [4] Multiply(,0.5) output 0

### [8] Floor

- `Input` <= [6] Divide output 0

### [9] Divide

- `A` <= [7] Subtract output 0
- `B` <= [26] Reroute Node (reroutes wires) output 0

### [10] Lerp

- `A` <= [35] Add output 0
- `B` <= [15] Lerp output 0
- `Alpha` <= [78] Reroute Node (reroutes wires) output 0

### [11] Floor

- `Input` <= [9] Divide output 0

### [12] Saturate

- `Input` <= [8] Floor output 0

### [13] 1-x

- `Input` <= [55] Reroute Node (reroutes wires) output 0

### [14] Saturate

- `Input` <= [11] Floor output 0

### [15] Lerp

- `A` <= [38] Add output 0
- `B` <= [40] Add output 0
- `Alpha` <= [77] Reroute Node (reroutes wires) output 0

### [16] Subtract

- `A` <= [7] Subtract output 0
- `B` <= [5] Multiply(,0.5) output 0

### [17] Divide

- `A` <= [16] Subtract output 0
- `B` <= [18] Reroute Node (reroutes wires) output 0

### [18] Reroute Node (reroutes wires)

- `Input` <= [19] Reroute Node (reroutes wires) output 0

### [19] Reroute Node (reroutes wires)

- `Input` <= [4] Multiply(,0.5) output 0

### [20] Floor

- `Input` <= [17] Divide output 0

### [21] Saturate

- `Input` <= [20] Floor output 0

### [22] Subtract

- `A` <= [16] Subtract output 0
- `B` <= [25] Reroute Node (reroutes wires) output 0

### [23] Divide

- `A` <= [22] Subtract output 0
- `B` <= [24] Reroute Node (reroutes wires) output 0

### [24] Reroute Node (reroutes wires)

- `Input` <= [5] Multiply(,0.5) output 0

### [25] Reroute Node (reroutes wires)

- `Input` <= [19] Reroute Node (reroutes wires) output 0

### [26] Reroute Node (reroutes wires)

- `Input` <= [5] Multiply(,0.5) output 0

### [27] Multiply

- `A` <= [30] Reroute Node (reroutes wires) output 0
- `B` <= [6] Divide output 0

### [28] Add

- `A` <= [27] Multiply output 0
- `B` <= [32] Reroute Node (reroutes wires) output 0

### [29] Reroute Node (reroutes wires)

- `Input` <= [45] Reroute Node (reroutes wires) output 0

### [30] Reroute Node (reroutes wires)

- `Input` <= [29] Reroute Node (reroutes wires) output 0

### [31] Reroute Node (reroutes wires)

- `Input` <= [44] Reroute Node (reroutes wires) output 0

### [32] Reroute Node (reroutes wires)

- `Input` <= [31] Reroute Node (reroutes wires) output 0

### [33] 1-x

- `Input` <= [9] Divide output 0

### [34] Multiply

- `A` <= [33] 1-x output 0
- `B` <= [32] Reroute Node (reroutes wires) output 0

### [35] Add

- `A` <= [34] Multiply output 0
- `B` <= [30] Reroute Node (reroutes wires) output 0

### [36] 1-x

- `Input` <= [17] Divide output 0

### [37] Multiply

- `A` <= [36] 1-x output 0
- `B` <= [30] Reroute Node (reroutes wires) output 0

### [38] Add

- `A` <= [37] Multiply output 0
- `B` <= [52] 0 output 0

### [39] Multiply

- `A` <= [23] Divide output 0
- `B` <= [32] Reroute Node (reroutes wires) output 0

### [40] Add

- `A` <= [39] Multiply output 0
- `B` <= [52] 0 output 0

### [41] Input XY Phase (0-1) (Scalar)

- `Preview` <= [3] Mask ( R ) output 0

### [44] Reroute Node (reroutes wires)

- `Input` <= [51] 0,1,0 output 0

### [45] Reroute Node (reroutes wires)

- `Input` <= [50] 1,0,0 output 0

### [48] Divide

- `A` <= [46] VectorLength output 0
- `B` <= [49] Add output 0

### [49] Add

- `A` <= [46] VectorLength output 0
- `B` <= [47] VectorLength output 0

### [55] Reroute Node (reroutes wires)

- `Input` <= [48] Divide output 0

### [57] Add

- `A` <= [1] Lerp output 0
- `B` <= [59] Multiply output 0

### [58] Input Z Phase (0-1) (Scalar)

- `Preview` <= [62] Mask ( G ) output 0

### [59] Multiply

- `A` <= [58] Input Z Phase (0-1) (Scalar) output 0
- `B` <= [60] 0,0,1 output 0

### [62] Mask ( G )

- `Input` <= [61] TexCoord[0] output 0

### [63] Reroute Node (reroutes wires)

- `Input` <= [42] Input X Vector (Vector3) output 0

### [64] Reroute Node (reroutes wires)

- `Input` <= [63] Reroute Node (reroutes wires) output 0

### [65] Reroute Node (reroutes wires)

- `Input` <= [66] Reroute Node (reroutes wires) output 0

### [66] Reroute Node (reroutes wires)

- `Input` <= [43] Input Y Vector (Vector3) output 0

### [68] Add

- `A` <= [57] Add output 0
- `B` <= [67] Input Relative Pivot Location (Vector3) output 0

### [70] Output World normal

- `A` <= [82] Normalize output 0

### [72] Lerp

- `A` <= [79] 0,1,0 output 0
- `B` <= [73] Lerp output 0
- `Alpha` <= [81] Reroute Node (reroutes wires) output 0

### [73] Lerp

- `A` <= [80] 1,0,0 output 0
- `B` <= [74] Lerp output 0
- `Alpha` <= [78] Reroute Node (reroutes wires) output 0

### [74] Lerp

- `A` <= [75] 0,-1,0 output 0
- `B` <= [76] -1,0,0 output 0
- `Alpha` <= [77] Reroute Node (reroutes wires) output 0

### [77] Reroute Node (reroutes wires)

- `Input` <= [21] Saturate output 0

### [78] Reroute Node (reroutes wires)

- `Input` <= [14] Saturate output 0

### [81] Reroute Node (reroutes wires)

- `Input` <= [12] Saturate output 0

### [82] Normalize

- `VectorInput` <= [69] Transform3x3Matrix output 0

## Connection List

- [53] Transform3x3Matrix output 0 -> Output Point Around Box.A
- [28] Add output 0 -> Lerp.A
- [10] Lerp output 0 -> Lerp.B
- [81] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [2] TexCoord[0] output 0 -> Mask ( R ).Input
- [55] Reroute Node (reroutes wires) output 0 -> Multiply(,0.5).A
- [13] 1-x output 0 -> Multiply(,0.5).A
- [41] Input XY Phase (0-1) (Scalar) output 0 -> Divide.A
- [4] Multiply(,0.5) output 0 -> Divide.B
- [41] Input XY Phase (0-1) (Scalar) output 0 -> Subtract.A
- [4] Multiply(,0.5) output 0 -> Subtract.B
- [6] Divide output 0 -> Floor.Input
- [7] Subtract output 0 -> Divide.A
- [26] Reroute Node (reroutes wires) output 0 -> Divide.B
- [35] Add output 0 -> Lerp.A
- [15] Lerp output 0 -> Lerp.B
- [78] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [9] Divide output 0 -> Floor.Input
- [8] Floor output 0 -> Saturate.Input
- [55] Reroute Node (reroutes wires) output 0 -> 1-x.Input
- [11] Floor output 0 -> Saturate.Input
- [38] Add output 0 -> Lerp.A
- [40] Add output 0 -> Lerp.B
- [77] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [7] Subtract output 0 -> Subtract.A
- [5] Multiply(,0.5) output 0 -> Subtract.B
- [16] Subtract output 0 -> Divide.A
- [18] Reroute Node (reroutes wires) output 0 -> Divide.B
- [19] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [4] Multiply(,0.5) output 0 -> Reroute Node (reroutes wires).Input
- [17] Divide output 0 -> Floor.Input
- [20] Floor output 0 -> Saturate.Input
- [16] Subtract output 0 -> Subtract.A
- [25] Reroute Node (reroutes wires) output 0 -> Subtract.B
- [22] Subtract output 0 -> Divide.A
- [24] Reroute Node (reroutes wires) output 0 -> Divide.B
- [5] Multiply(,0.5) output 0 -> Reroute Node (reroutes wires).Input
- [19] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [5] Multiply(,0.5) output 0 -> Reroute Node (reroutes wires).Input
- [30] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [6] Divide output 0 -> Multiply.B
- [27] Multiply output 0 -> Add.A
- [32] Reroute Node (reroutes wires) output 0 -> Add.B
- [45] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [29] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [44] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [31] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [9] Divide output 0 -> 1-x.Input
- [33] 1-x output 0 -> Multiply.A
- [32] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [34] Multiply output 0 -> Add.A
- [30] Reroute Node (reroutes wires) output 0 -> Add.B
- [17] Divide output 0 -> 1-x.Input
- [36] 1-x output 0 -> Multiply.A
- [30] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [37] Multiply output 0 -> Add.A
- [52] 0 output 0 -> Add.B
- [23] Divide output 0 -> Multiply.A
- [32] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [39] Multiply output 0 -> Add.A
- [52] 0 output 0 -> Add.B
- [3] Mask ( R ) output 0 -> Input XY Phase (0-1) (Scalar).Preview
- [51] 0,1,0 output 0 -> Reroute Node (reroutes wires).Input
- [50] 1,0,0 output 0 -> Reroute Node (reroutes wires).Input
- [46] VectorLength output 0 -> Divide.A
- [49] Add output 0 -> Divide.B
- [46] VectorLength output 0 -> Add.A
- [47] VectorLength output 0 -> Add.B
- [48] Divide output 0 -> Reroute Node (reroutes wires).Input
- [1] Lerp output 0 -> Add.A
- [59] Multiply output 0 -> Add.B
- [62] Mask ( G ) output 0 -> Input Z Phase (0-1) (Scalar).Preview
- [58] Input Z Phase (0-1) (Scalar) output 0 -> Multiply.A
- [60] 0,0,1 output 0 -> Multiply.B
- [61] TexCoord[0] output 0 -> Mask ( G ).Input
- [42] Input X Vector (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [63] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [66] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [43] Input Y Vector (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [57] Add output 0 -> Add.A
- [67] Input Relative Pivot Location (Vector3) output 0 -> Add.B
- [82] Normalize output 0 -> Output World normal.A
- [79] 0,1,0 output 0 -> Lerp.A
- [73] Lerp output 0 -> Lerp.B
- [81] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [80] 1,0,0 output 0 -> Lerp.A
- [74] Lerp output 0 -> Lerp.B
- [78] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [75] 0,-1,0 output 0 -> Lerp.A
- [76] -1,0,0 output 0 -> Lerp.B
- [77] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [21] Saturate output 0 -> Reroute Node (reroutes wires).Input
- [14] Saturate output 0 -> Reroute Node (reroutes wires).Input
- [12] Saturate output 0 -> Reroute Node (reroutes wires).Input
- [69] Transform3x3Matrix output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Point Around Box` is driven by [53] Transform3x3Matrix
- `World normal` is driven by [82] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
