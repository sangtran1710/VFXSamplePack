# RayTracedSphere

- Path: `/Engine/Functions/Engine_MaterialFunctions02/RayTracedSphere.RayTracedSphere`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Sphere Position` [FunctionInput_Vector3]
- `Sphere Radius` [FunctionInput_Scalar]
- `RayDirection` [FunctionInput_Vector3] - Defaults to Camera Direction
- `World position` [FunctionInput_Vector3]

## Outputs

- `Ray Intersection Position `
- `Intersection End Depth `
- `0-1 Sphere density`
- `Projected Capture Vector `
- `Intersection End World Position`
- `Ray Hit Bool`
- `Intersection Surface Normal`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Ray Intersection Position  | Ray Intersection Position  |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Position (Vector3) | Sphere Position |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Radius (Scalar) | Sphere Radius |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input RayDirection (Vector3) | RayDirection |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 9 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | expression | `MaterialExpressionDivide` | Divide |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 14 | expression | `MaterialExpressionConstant` | 1 |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 16 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 17 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply(4,) |  |
| 21 | expression | `MaterialExpressionIf` | If |  |
| 22 | expression | `MaterialExpressionConstant` | 0 |  |
| 23 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 24 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 28 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 29 | expression | `MaterialExpressionSubtract` | Subtract(,0.6) |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply(,2.5) |  |
| 31 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 32 | function_output | `MaterialExpressionFunctionOutput` | Output Intersection End Depth  | Intersection End Depth  |
| 33 | function_output | `MaterialExpressionFunctionOutput` | Output 0-1 Sphere density | 0-1 Sphere density |
| 34 | function_output | `MaterialExpressionFunctionOutput` | Output Projected Capture Vector  | Projected Capture Vector  |
| 35 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 36 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 37 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 38 | function_input | `MaterialExpressionFunctionInput` | Input World position (Vector3) | World position |
| 39 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output Intersection End World Position | Intersection End World Position |
| 42 | function_output | `MaterialExpressionFunctionOutput` | Output Ray Hit Bool | Ray Hit Bool |
| 43 | expression | `MaterialExpressionConstant` | 1 |  |
| 44 | function_output | `MaterialExpressionFunctionOutput` | Output Intersection Surface Normal | Intersection Surface Normal |
| 45 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 46 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 47 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output Ray Intersection Position 

- `A` <= [26] Add output 0

### [3] Input RayDirection (Vector3)

- `Preview` <= [37] Normalize output 0

### [4] Multiply

- `A` <= [2] Input Sphere Radius (Scalar) output 0
- `B` <= [2] Input Sphere Radius (Scalar) output 0

### [5] Subtract

- `A` <= [38] Input World position (Vector3) output 0
- `B` <= [1] Input Sphere Position (Vector3) output 0

### [7] Dot

- `A` <= [5] Subtract output 0
- `B` <= [5] Subtract output 0

### [8] Subtract

- `A` <= [38] Input World position (Vector3) output 0
- `B` <= [1] Input Sphere Position (Vector3) output 0

### [9] Sqrt

- `Input` <= [10] Dot output 0

### [10] Dot

- `A` <= [8] Subtract output 0
- `B` <= [8] Subtract output 0

### [11] Clamp

- `Input` <= [12] Divide output 0

### [12] Divide

- `A` <= [9] Sqrt output 0
- `B` <= [2] Input Sphere Radius (Scalar) output 0

### [15] Multiply(,2)

- `A` <= [16] Dot output 0

### [16] Dot

- `A` <= [3] Input RayDirection (Vector3) output 0
- `B` <= [5] Subtract output 0

### [17] Subtract

- `A` <= [7] Dot output 0
- `B` <= [4] Multiply output 0

### [18] Multiply

- `A` <= [15] Multiply(,2) output 0
- `B` <= [15] Multiply(,2) output 0

### [19] Subtract

- `A` <= [18] Multiply output 0
- `B` <= [20] Multiply(4,) output 0

### [20] Multiply(4,)

- `B` <= [17] Subtract output 0

### [21] If

- `A` <= [19] Subtract output 0
- `B` <= [22] 0 output 0
- `AGreaterThanB` <= [43] 1 output 0
- `ALessThanB` <= [22] 0 output 0

### [23] Sqrt

- `Input` <= [19] Subtract output 0

### [24] Subtract

- `A` <= [23] Sqrt output 0
- `B` <= [15] Multiply(,2) output 0

### [25] Multiply(,0.5)

- `A` <= [24] Subtract output 0

### [26] Add

- `A` <= [38] Input World position (Vector3) output 0
- `B` <= [27] Multiply output 0

### [27] Multiply

- `A` <= [25] Multiply(,0.5) output 0
- `B` <= [3] Input RayDirection (Vector3) output 0

### [28] Subtract

- `A` <= [26] Add output 0
- `B` <= [1] Input Sphere Position (Vector3) output 0

### [29] Subtract(,0.6)

- `A` <= [11] Clamp output 0

### [30] Multiply(,2.5)

- `A` <= [29] Subtract(,0.6) output 0

### [31] 1-x

- `Input` <= [30] Multiply(,2.5) output 0

### [32] Output Intersection End Depth 

- `A` <= [25] Multiply(,0.5) output 0

### [33] Output 0-1 Sphere density

- `A` <= [31] 1-x output 0

### [34] Output Projected Capture Vector 

- `A` <= [28] Subtract output 0

### [35] Subtract

- `A` <= [36] Camera Position output 0
- `B` <= [38] Input World position (Vector3) output 0

### [37] Normalize

- `VectorInput` <= [35] Subtract output 0

### [38] Input World position (Vector3)

- `Preview` <= [6] Absolute World Position output 0

### [39] Multiply

- `A` <= [25] Multiply(,0.5) output 0
- `B` <= [3] Input RayDirection (Vector3) output 0

### [40] Add

- `A` <= [39] Multiply output 0
- `B` <= [38] Input World position (Vector3) output 0

### [41] Output Intersection End World Position

- `A` <= [40] Add output 0

### [42] Output Ray Hit Bool

- `A` <= [21] If output 0

### [44] Output Intersection Surface Normal

- `A` <= [46] Normalize output 0

### [45] Subtract

- `A` <= [26] Add output 0
- `B` <= [1] Input Sphere Position (Vector3) output 0

### [46] Normalize

- `VectorInput` <= [45] Subtract output 0

### [47] 1-x

- `Input` <= [11] Clamp output 0

## Connection List

- [26] Add output 0 -> Output Ray Intersection Position .A
- [37] Normalize output 0 -> Input RayDirection (Vector3).Preview
- [2] Input Sphere Radius (Scalar) output 0 -> Multiply.A
- [2] Input Sphere Radius (Scalar) output 0 -> Multiply.B
- [38] Input World position (Vector3) output 0 -> Subtract.A
- [1] Input Sphere Position (Vector3) output 0 -> Subtract.B
- [5] Subtract output 0 -> Dot.A
- [5] Subtract output 0 -> Dot.B
- [38] Input World position (Vector3) output 0 -> Subtract.A
- [1] Input Sphere Position (Vector3) output 0 -> Subtract.B
- [10] Dot output 0 -> Sqrt.Input
- [8] Subtract output 0 -> Dot.A
- [8] Subtract output 0 -> Dot.B
- [12] Divide output 0 -> Clamp.Input
- [9] Sqrt output 0 -> Divide.A
- [2] Input Sphere Radius (Scalar) output 0 -> Divide.B
- [16] Dot output 0 -> Multiply(,2).A
- [3] Input RayDirection (Vector3) output 0 -> Dot.A
- [5] Subtract output 0 -> Dot.B
- [7] Dot output 0 -> Subtract.A
- [4] Multiply output 0 -> Subtract.B
- [15] Multiply(,2) output 0 -> Multiply.A
- [15] Multiply(,2) output 0 -> Multiply.B
- [18] Multiply output 0 -> Subtract.A
- [20] Multiply(4,) output 0 -> Subtract.B
- [17] Subtract output 0 -> Multiply(4,).B
- [19] Subtract output 0 -> If.A
- [22] 0 output 0 -> If.B
- [43] 1 output 0 -> If.AGreaterThanB
- [22] 0 output 0 -> If.ALessThanB
- [19] Subtract output 0 -> Sqrt.Input
- [23] Sqrt output 0 -> Subtract.A
- [15] Multiply(,2) output 0 -> Subtract.B
- [24] Subtract output 0 -> Multiply(,0.5).A
- [38] Input World position (Vector3) output 0 -> Add.A
- [27] Multiply output 0 -> Add.B
- [25] Multiply(,0.5) output 0 -> Multiply.A
- [3] Input RayDirection (Vector3) output 0 -> Multiply.B
- [26] Add output 0 -> Subtract.A
- [1] Input Sphere Position (Vector3) output 0 -> Subtract.B
- [11] Clamp output 0 -> Subtract(,0.6).A
- [29] Subtract(,0.6) output 0 -> Multiply(,2.5).A
- [30] Multiply(,2.5) output 0 -> 1-x.Input
- [25] Multiply(,0.5) output 0 -> Output Intersection End Depth .A
- [31] 1-x output 0 -> Output 0-1 Sphere density.A
- [28] Subtract output 0 -> Output Projected Capture Vector .A
- [36] Camera Position output 0 -> Subtract.A
- [38] Input World position (Vector3) output 0 -> Subtract.B
- [35] Subtract output 0 -> Normalize.VectorInput
- [6] Absolute World Position output 0 -> Input World position (Vector3).Preview
- [25] Multiply(,0.5) output 0 -> Multiply.A
- [3] Input RayDirection (Vector3) output 0 -> Multiply.B
- [39] Multiply output 0 -> Add.A
- [38] Input World position (Vector3) output 0 -> Add.B
- [40] Add output 0 -> Output Intersection End World Position.A
- [21] If output 0 -> Output Ray Hit Bool.A
- [46] Normalize output 0 -> Output Intersection Surface Normal.A
- [26] Add output 0 -> Subtract.A
- [1] Input Sphere Position (Vector3) output 0 -> Subtract.B
- [45] Subtract output 0 -> Normalize.VectorInput
- [11] Clamp output 0 -> 1-x.Input

## Output Trace Roots

- `Ray Intersection Position ` is driven by [26] Add
- `Intersection End Depth ` is driven by [25] Multiply(,0.5)
- `0-1 Sphere density` is driven by [31] 1-x
- `Projected Capture Vector ` is driven by [28] Subtract
- `Intersection End World Position` is driven by [40] Add
- `Ray Hit Bool` is driven by [21] If
- `Intersection Surface Normal` is driven by [46] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


