# GenerateASpline

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/GenerateASpline.GenerateASpline`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Generate a Catmull-Rom spline.

## Inputs

- `P0` [FunctionInput_Vector3] - Tangent handle 1
- `P1` [FunctionInput_Vector3] - Control point 1
- `P2` [FunctionInput_Vector3] - Control point 2
- `t` [FunctionInput_Scalar] - Spline sample position. (0-1)
- `P3` [FunctionInput_Vector3] - Tangent Handle 2
- `Normal/Bi-Normal Cross product vector` [FunctionInput_Vector3] - The default value is object orientation.

## Outputs

- `T position` - A world position sampled at t.
- `T Tangent` - Vector down the length of the spline.
- `T Bi-normal`
- `T Normal`
- `UnnormalizedTangent`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output T position | T position |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input P0 (Vector3) | P0 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input P1 (Vector3) | P1 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input P2 (Vector3) | P2 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input t (Scalar) | t |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,5) |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply(,4) |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input P3 (Vector3) | P3 |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 16 | expression | `MaterialExpressionPower` | Power(X, 3) |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 22 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionPower` | Power(X, 2) |  |
| 27 | expression | `MaterialExpressionAdd` | Add |  |
| 28 | expression | `MaterialExpressionAdd` | Add |  |
| 29 | expression | `MaterialExpressionAdd` | Add |  |
| 30 | function_output | `MaterialExpressionFunctionOutput` | Output T Tangent | T Tangent |
| 31 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 35 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 36 | expression | `MaterialExpressionMultiply` | Multiply(,5) |  |
| 37 | expression | `MaterialExpressionAdd` | Add |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply(,4) |  |
| 39 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 42 | expression | `MaterialExpressionAdd` | Add |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 44 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 45 | expression | `MaterialExpressionAdd` | Add |  |
| 46 | expression | `MaterialExpressionPower` | Power(X, 2) |  |
| 47 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 48 | expression | `MaterialExpressionAdd` | Add |  |
| 49 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 50 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 51 | expression | `MaterialExpressionAdd` | Add |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 53 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 54 | function_output | `MaterialExpressionFunctionOutput` | Output T Bi-normal | T Bi-normal |
| 55 | function_output | `MaterialExpressionFunctionOutput` | Output T Normal | T Normal |
| 56 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 57 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 58 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 59 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 60 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 61 | function_output | `MaterialExpressionFunctionOutput` | Output UnnormalizedTangent | UnnormalizedTangent |
| 62 | expression | `MaterialExpressionObjectOrientation` | ObjectOrientation |  |
| 63 | function_input | `MaterialExpressionFunctionInput` | Input Normal/Bi-Normal Cross product vector (Vector3) | Normal/Bi-Normal Cross product vector |

## Exact Input Wiring

### [0] Output T position

- `A` <= [15] Multiply(,0.5) output 0

### [5] Multiply(,-1)

- `A` <= [1] Input P0 (Vector3) output 0

### [6] Add

- `A` <= [5] Multiply(,-1) output 0
- `B` <= [3] Input P2 (Vector3) output 0

### [7] Multiply

- `A` <= [6] Add output 0
- `B` <= [4] Input t (Scalar) output 0

### [8] Multiply(,2)

- `A` <= [2] Input P1 (Vector3) output 0

### [9] Multiply(,5)

- `A` <= [2] Input P1 (Vector3) output 0

### [10] Multiply(,4)

- `A` <= [3] Input P2 (Vector3) output 0

### [11] Subtract

- `A` <= [10] Multiply(,4) output 0
- `B` <= [12] Input P3 (Vector3) output 0

### [13] Subtract

- `A` <= [24] Multiply(,2) output 0
- `B` <= [9] Multiply(,5) output 0

### [14] Add

- `A` <= [13] Subtract output 0
- `B` <= [11] Subtract output 0

### [15] Multiply(,0.5)

- `A` <= [29] Add output 0

### [16] Power(X, 3)

- `Base` <= [4] Input t (Scalar) output 0

### [17] Multiply

- `A` <= [23] Add output 0
- `B` <= [16] Power(X, 3) output 0

### [18] Multiply(,-1)

- `A` <= [1] Input P0 (Vector3) output 0

### [19] Add

- `A` <= [18] Multiply(,-1) output 0
- `B` <= [20] Multiply(,3) output 0

### [20] Multiply(,3)

- `A` <= [2] Input P1 (Vector3) output 0

### [21] Multiply(,3)

- `A` <= [3] Input P2 (Vector3) output 0

### [22] Subtract

- `A` <= [19] Add output 0
- `B` <= [21] Multiply(,3) output 0

### [23] Add

- `A` <= [22] Subtract output 0
- `B` <= [12] Input P3 (Vector3) output 0

### [24] Multiply(,2)

- `A` <= [1] Input P0 (Vector3) output 0

### [25] Multiply

- `A` <= [14] Add output 0
- `B` <= [26] Power(X, 2) output 0

### [26] Power(X, 2)

- `Base` <= [4] Input t (Scalar) output 0

### [27] Add

- `A` <= [8] Multiply(,2) output 0
- `B` <= [7] Multiply output 0

### [28] Add

- `A` <= [25] Multiply output 0
- `B` <= [17] Multiply output 0

### [29] Add

- `A` <= [27] Add output 0
- `B` <= [28] Add output 0

### [30] Output T Tangent

- `A` <= [53] Normalize output 0

### [31] Multiply(,0.5)

- `A` <= [45] Add output 0

### [32] Multiply(,-1)

- `A` <= [1] Input P0 (Vector3) output 0

### [33] Add

- `A` <= [32] Multiply(,-1) output 0
- `B` <= [3] Input P2 (Vector3) output 0

### [34] Multiply(,2)

- `A` <= [1] Input P0 (Vector3) output 0

### [35] Subtract

- `A` <= [34] Multiply(,2) output 0
- `B` <= [36] Multiply(,5) output 0

### [36] Multiply(,5)

- `A` <= [2] Input P1 (Vector3) output 0

### [37] Add

- `A` <= [35] Subtract output 0
- `B` <= [38] Multiply(,4) output 0

### [38] Multiply(,4)

- `A` <= [3] Input P2 (Vector3) output 0

### [39] Subtract

- `A` <= [37] Add output 0
- `B` <= [12] Input P3 (Vector3) output 0

### [40] Multiply

- `A` <= [39] Subtract output 0
- `B` <= [4] Input t (Scalar) output 0

### [41] Multiply(,3)

- `A` <= [47] Multiply output 0

### [42] Add

- `A` <= [33] Add output 0
- `B` <= [43] Multiply(,2) output 0

### [43] Multiply(,2)

- `A` <= [40] Multiply output 0

### [44] Subtract

- `A` <= [51] Add output 0
- `B` <= [49] Multiply(,3) output 0

### [45] Add

- `A` <= [42] Add output 0
- `B` <= [41] Multiply(,3) output 0

### [46] Power(X, 2)

- `Base` <= [4] Input t (Scalar) output 0

### [47] Multiply

- `A` <= [48] Add output 0
- `B` <= [46] Power(X, 2) output 0

### [48] Add

- `A` <= [44] Subtract output 0
- `B` <= [12] Input P3 (Vector3) output 0

### [49] Multiply(,3)

- `A` <= [3] Input P2 (Vector3) output 0

### [50] Multiply(,-1)

- `A` <= [1] Input P0 (Vector3) output 0

### [51] Add

- `A` <= [50] Multiply(,-1) output 0
- `B` <= [52] Multiply(,3) output 0

### [52] Multiply(,3)

- `A` <= [2] Input P1 (Vector3) output 0

### [53] Normalize

- `VectorInput` <= [31] Multiply(,0.5) output 0

### [54] Output T Bi-normal

- `A` <= [58] Normalize output 0

### [55] Output T Normal

- `A` <= [60] Normalize output 0

### [56] Cross

- `A` <= [53] Normalize output 0
- `B` <= [63] Input Normal/Bi-Normal Cross product vector (Vector3) output 0

### [57] Cross

- `A` <= [60] Normalize output 0
- `B` <= [53] Normalize output 0

### [58] Normalize

- `VectorInput` <= [57] Cross output 0

### [59] Cross

- `A` <= [56] Cross output 0
- `B` <= [53] Normalize output 0

### [60] Normalize

- `VectorInput` <= [59] Cross output 0

### [61] Output UnnormalizedTangent

- `A` <= [31] Multiply(,0.5) output 0

### [63] Input Normal/Bi-Normal Cross product vector (Vector3)

- `Preview` <= [62] ObjectOrientation output 0

## Connection List

- [15] Multiply(,0.5) output 0 -> Output T position.A
- [1] Input P0 (Vector3) output 0 -> Multiply(,-1).A
- [5] Multiply(,-1) output 0 -> Add.A
- [3] Input P2 (Vector3) output 0 -> Add.B
- [6] Add output 0 -> Multiply.A
- [4] Input t (Scalar) output 0 -> Multiply.B
- [2] Input P1 (Vector3) output 0 -> Multiply(,2).A
- [2] Input P1 (Vector3) output 0 -> Multiply(,5).A
- [3] Input P2 (Vector3) output 0 -> Multiply(,4).A
- [10] Multiply(,4) output 0 -> Subtract.A
- [12] Input P3 (Vector3) output 0 -> Subtract.B
- [24] Multiply(,2) output 0 -> Subtract.A
- [9] Multiply(,5) output 0 -> Subtract.B
- [13] Subtract output 0 -> Add.A
- [11] Subtract output 0 -> Add.B
- [29] Add output 0 -> Multiply(,0.5).A
- [4] Input t (Scalar) output 0 -> Power(X, 3).Base
- [23] Add output 0 -> Multiply.A
- [16] Power(X, 3) output 0 -> Multiply.B
- [1] Input P0 (Vector3) output 0 -> Multiply(,-1).A
- [18] Multiply(,-1) output 0 -> Add.A
- [20] Multiply(,3) output 0 -> Add.B
- [2] Input P1 (Vector3) output 0 -> Multiply(,3).A
- [3] Input P2 (Vector3) output 0 -> Multiply(,3).A
- [19] Add output 0 -> Subtract.A
- [21] Multiply(,3) output 0 -> Subtract.B
- [22] Subtract output 0 -> Add.A
- [12] Input P3 (Vector3) output 0 -> Add.B
- [1] Input P0 (Vector3) output 0 -> Multiply(,2).A
- [14] Add output 0 -> Multiply.A
- [26] Power(X, 2) output 0 -> Multiply.B
- [4] Input t (Scalar) output 0 -> Power(X, 2).Base
- [8] Multiply(,2) output 0 -> Add.A
- [7] Multiply output 0 -> Add.B
- [25] Multiply output 0 -> Add.A
- [17] Multiply output 0 -> Add.B
- [27] Add output 0 -> Add.A
- [28] Add output 0 -> Add.B
- [53] Normalize output 0 -> Output T Tangent.A
- [45] Add output 0 -> Multiply(,0.5).A
- [1] Input P0 (Vector3) output 0 -> Multiply(,-1).A
- [32] Multiply(,-1) output 0 -> Add.A
- [3] Input P2 (Vector3) output 0 -> Add.B
- [1] Input P0 (Vector3) output 0 -> Multiply(,2).A
- [34] Multiply(,2) output 0 -> Subtract.A
- [36] Multiply(,5) output 0 -> Subtract.B
- [2] Input P1 (Vector3) output 0 -> Multiply(,5).A
- [35] Subtract output 0 -> Add.A
- [38] Multiply(,4) output 0 -> Add.B
- [3] Input P2 (Vector3) output 0 -> Multiply(,4).A
- [37] Add output 0 -> Subtract.A
- [12] Input P3 (Vector3) output 0 -> Subtract.B
- [39] Subtract output 0 -> Multiply.A
- [4] Input t (Scalar) output 0 -> Multiply.B
- [47] Multiply output 0 -> Multiply(,3).A
- [33] Add output 0 -> Add.A
- [43] Multiply(,2) output 0 -> Add.B
- [40] Multiply output 0 -> Multiply(,2).A
- [51] Add output 0 -> Subtract.A
- [49] Multiply(,3) output 0 -> Subtract.B
- [42] Add output 0 -> Add.A
- [41] Multiply(,3) output 0 -> Add.B
- [4] Input t (Scalar) output 0 -> Power(X, 2).Base
- [48] Add output 0 -> Multiply.A
- [46] Power(X, 2) output 0 -> Multiply.B
- [44] Subtract output 0 -> Add.A
- [12] Input P3 (Vector3) output 0 -> Add.B
- [3] Input P2 (Vector3) output 0 -> Multiply(,3).A
- [1] Input P0 (Vector3) output 0 -> Multiply(,-1).A
- [50] Multiply(,-1) output 0 -> Add.A
- [52] Multiply(,3) output 0 -> Add.B
- [2] Input P1 (Vector3) output 0 -> Multiply(,3).A
- [31] Multiply(,0.5) output 0 -> Normalize.VectorInput
- [58] Normalize output 0 -> Output T Bi-normal.A
- [60] Normalize output 0 -> Output T Normal.A
- [53] Normalize output 0 -> Cross.A
- [63] Input Normal/Bi-Normal Cross product vector (Vector3) output 0 -> Cross.B
- [60] Normalize output 0 -> Cross.A
- [53] Normalize output 0 -> Cross.B
- [57] Cross output 0 -> Normalize.VectorInput
- [56] Cross output 0 -> Cross.A
- [53] Normalize output 0 -> Cross.B
- [59] Cross output 0 -> Normalize.VectorInput
- [31] Multiply(,0.5) output 0 -> Output UnnormalizedTangent.A
- [62] ObjectOrientation output 0 -> Input Normal/Bi-Normal Cross product vector (Vector3).Preview

## Output Trace Roots

- `T position` is driven by [15] Multiply(,0.5)
- `T Tangent` is driven by [53] Normalize
- `T Bi-normal` is driven by [58] Normalize
- `T Normal` is driven by [60] Normalize
- `UnnormalizedTangent` is driven by [31] Multiply(,0.5)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


