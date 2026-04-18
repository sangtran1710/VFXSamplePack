# BoxIntersection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/BoxIntersection.BoxIntersection`
- Category: Engine_MaterialFunctions01/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns Entry and Exit points for a Ray intersecting an Axis Aligned Bounding Box. If you need it to be non axis aligned then you will have to Inverse Transform the Ray Origin and Ray End into the local space of the box.

## Inputs

- `Ray Origin` [FunctionInput_Vector3]
- `Ray Direction` [FunctionInput_Vector3]
- `Box Min` [FunctionInput_Vector3]
- `Box Max` [FunctionInput_Vector3]

## Outputs

- `Ray Entry Position`
- `Ray Exit Position`
- `Box Thickness`
- `T1`
- `T0`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Ray Entry Position | Ray Entry Position |
| 1 | expression | `MaterialExpressionVectorParameter` | Param (0,0,1,0) 'P0' |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Ray Origin (Vector3) | Ray Origin |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Ray Direction (Vector3) | Ray Direction |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Box Min (Vector3) | Box Min |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Box Max (Vector3) | Box Max |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output Ray Exit Position | Ray Exit Position |
| 12 | expression | `MaterialExpressionDistance` | Distance |  |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output Box Thickness | Box Thickness |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output T1 | T1 |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output T0 | T0 |
| 16 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 17 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 21 | expression | `MaterialExpressionMin` | Min |  |
| 22 | expression | `MaterialExpressionMax` | Max |  |
| 23 | expression | `MaterialExpressionMax` | Max |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 25 | expression | `MaterialExpressionMax` | Max |  |
| 26 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 27 | expression | `MaterialExpressionMin` | Min |  |
| 28 | expression | `MaterialExpressionMin` | Min |  |
| 29 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 30 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |

## Exact Input Wiring

### [0] Output Ray Entry Position

- `A` <= [10] Add output 0

### [2] Input Ray Origin (Vector3)

- `Preview` <= [30] Camera Position output 0

### [3] Input Ray Direction (Vector3)

- `Preview` <= [29] Camera Vector output 0

### [6] Multiply(,1)

- `A` <= [2] Input Ray Origin (Vector3) output 0

### [7] Add

- `A` <= [8] Multiply output 0
- `B` <= [6] Multiply(,1) output 0

### [8] Multiply

- `A` <= [14] Output T1 output 0
- `B` <= [31] Multiply(,1) output 0

### [9] Multiply

- `A` <= [15] Output T0 output 0
- `B` <= [31] Multiply(,1) output 0

### [10] Add

- `A` <= [9] Multiply output 0
- `B` <= [6] Multiply(,1) output 0

### [11] Output Ray Exit Position

- `A` <= [7] Add output 0

### [12] Distance

- `A` <= [0] Output Ray Entry Position output 0
- `B` <= [11] Output Ray Exit Position output 0

### [13] Output Box Thickness

- `A` <= [12] Distance output 0

### [14] Output T1

- `A` <= [28] Min output 0

### [15] Output T0

- `A` <= [25] Max output 0

### [16] Divide(1,)

- `B` <= [31] Multiply(,1) output 0

### [17] Subtract

- `A` <= [4] Input Box Min (Vector3) output 0
- `B` <= [2] Input Ray Origin (Vector3) output 0

### [18] Multiply

- `A` <= [16] Divide(1,) output 0
- `B` <= [17] Subtract output 0

### [19] Multiply

- `A` <= [16] Divide(1,) output 0
- `B` <= [20] Subtract output 0

### [20] Subtract

- `A` <= [5] Input Box Max (Vector3) output 0
- `B` <= [2] Input Ray Origin (Vector3) output 0

### [21] Min

- `A` <= [18] Multiply output 0
- `B` <= [19] Multiply output 0

### [22] Max

- `A` <= [18] Multiply output 0
- `B` <= [19] Multiply output 0

### [23] Max

- `A` <= [24] BreakOutFloat3Components output 0
- `B` <= [24] BreakOutFloat3Components output 1

### [25] Max

- `A` <= [23] Max output 0
- `B` <= [24] BreakOutFloat3Components output 2

### [27] Min

- `A` <= [26] BreakOutFloat3Components output 0
- `B` <= [26] BreakOutFloat3Components output 1

### [28] Min

- `A` <= [27] Min output 0
- `B` <= [26] BreakOutFloat3Components output 2

### [31] Multiply(,1)

- `A` <= [3] Input Ray Direction (Vector3) output 0

## Connection List

- [10] Add output 0 -> Output Ray Entry Position.A
- [30] Camera Position output 0 -> Input Ray Origin (Vector3).Preview
- [29] Camera Vector output 0 -> Input Ray Direction (Vector3).Preview
- [2] Input Ray Origin (Vector3) output 0 -> Multiply(,1).A
- [8] Multiply output 0 -> Add.A
- [6] Multiply(,1) output 0 -> Add.B
- [14] Output T1 output 0 -> Multiply.A
- [31] Multiply(,1) output 0 -> Multiply.B
- [15] Output T0 output 0 -> Multiply.A
- [31] Multiply(,1) output 0 -> Multiply.B
- [9] Multiply output 0 -> Add.A
- [6] Multiply(,1) output 0 -> Add.B
- [7] Add output 0 -> Output Ray Exit Position.A
- [0] Output Ray Entry Position output 0 -> Distance.A
- [11] Output Ray Exit Position output 0 -> Distance.B
- [12] Distance output 0 -> Output Box Thickness.A
- [28] Min output 0 -> Output T1.A
- [25] Max output 0 -> Output T0.A
- [31] Multiply(,1) output 0 -> Divide(1,).B
- [4] Input Box Min (Vector3) output 0 -> Subtract.A
- [2] Input Ray Origin (Vector3) output 0 -> Subtract.B
- [16] Divide(1,) output 0 -> Multiply.A
- [17] Subtract output 0 -> Multiply.B
- [16] Divide(1,) output 0 -> Multiply.A
- [20] Subtract output 0 -> Multiply.B
- [5] Input Box Max (Vector3) output 0 -> Subtract.A
- [2] Input Ray Origin (Vector3) output 0 -> Subtract.B
- [18] Multiply output 0 -> Min.A
- [19] Multiply output 0 -> Min.B
- [18] Multiply output 0 -> Max.A
- [19] Multiply output 0 -> Max.B
- [24] BreakOutFloat3Components output 0 -> Max.A
- [24] BreakOutFloat3Components output 1 -> Max.B
- [23] Max output 0 -> Max.A
- [24] BreakOutFloat3Components output 2 -> Max.B
- [26] BreakOutFloat3Components output 0 -> Min.A
- [26] BreakOutFloat3Components output 1 -> Min.B
- [27] Min output 0 -> Min.A
- [26] BreakOutFloat3Components output 2 -> Min.B
- [3] Input Ray Direction (Vector3) output 0 -> Multiply(,1).A

## Output Trace Roots

- `Ray Entry Position` is driven by [10] Add
- `Ray Exit Position` is driven by [7] Add
- `Box Thickness` is driven by [12] Distance
- `T1` is driven by [28] Min
- `T0` is driven by [25] Max

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


