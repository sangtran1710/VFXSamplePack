# DistanceLimitedReflections

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/DistanceLimitedReflections.DistanceLimitedReflections`
- Category: Engine_MaterialFunctions01/Reflections
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Distance Limited Reflections give some parallax to cubemaps by fixing them in a point in space and performing a ray sphere intersection. Can be used to help anchor manually placed cubemaps in the world.

## Inputs

- `CapturePosition` [FunctionInput_Vector3] - The position that the cubemap was captured from, or the place you want it to be centered in the world.
- `Radius` [FunctionInput_Scalar] - The radius of the sphere used for projection.
- `Cubemap` [FunctionInput_TextureCube]

## Outputs

- `Result`
- `Sphere Hit Mask` - Rays that do not hit the sphere will write black pixels into the output color as well as this mask output.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | expression | `MaterialExpressionReflectionVectorWS` | Reflection Vector |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply(,1.2) |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(,4) |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | expression | `MaterialExpressionConstant` | 0 |  |
| 18 | expression | `MaterialExpressionConstant` | 1e-06 |  |
| 19 | expression | `MaterialExpressionDivide` | Divide(0.5,) |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 22 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input CapturePosition (Vector3) | CapturePosition |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Cubemap (TextureCube) | Cubemap |
| 31 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 33 | function_output | `MaterialExpressionFunctionOutput` | Output Sphere Hit Mask | Sphere Hit Mask |
| 34 | expression | `MaterialExpressionCeil` | Ceil |  |
| 35 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] If output 0

### [1] If

- `A` <= [16] Subtract output 0
- `B` <= [17] 0 output 0
- `AGreaterThanB` <= [31] Texture Sample output 0
- `AEqualsB` <= [17] 0 output 0
- `ALessThanB` <= [17] 0 output 0

### [3] Multiply

- `A` <= [29] Input Radius (Scalar) output 0
- `B` <= [2] Reflection Vector output 0

### [4] Multiply(,1.2)

- `A` <= [29] Input Radius (Scalar) output 0

### [5] Multiply

- `A` <= [4] Multiply(,1.2) output 0
- `B` <= [4] Multiply(,1.2) output 0

### [7] Subtract

- `A` <= [6] Absolute World Position output 0
- `B` <= [28] Input CapturePosition (Vector3) output 0

### [8] Dot

- `A` <= [7] Subtract output 0
- `B` <= [7] Subtract output 0

### [9] Dot

- `A` <= [3] Multiply output 0
- `B` <= [3] Multiply output 0

### [10] Dot

- `A` <= [3] Multiply output 0
- `B` <= [7] Subtract output 0

### [11] Multiply(,2)

- `A` <= [10] Dot output 0

### [12] Subtract

- `A` <= [8] Dot output 0
- `B` <= [5] Multiply output 0

### [13] Multiply

- `A` <= [12] Subtract output 0
- `B` <= [9] Dot output 0

### [14] Multiply(,4)

- `A` <= [13] Multiply output 0

### [15] Multiply

- `A` <= [11] Multiply(,2) output 0
- `B` <= [11] Multiply(,2) output 0

### [16] Subtract

- `A` <= [15] Multiply output 0
- `B` <= [14] Multiply(,4) output 0

### [19] Divide(0.5,)

- `B` <= [20] Add output 0

### [20] Add

- `A` <= [9] Dot output 0
- `B` <= [18] 1e-06 output 0

### [21] Sqrt

- `Input` <= [16] Subtract output 0

### [22] Subtract

- `A` <= [21] Sqrt output 0
- `B` <= [11] Multiply(,2) output 0

### [23] Multiply

- `A` <= [22] Subtract output 0
- `B` <= [19] Divide(0.5,) output 0

### [25] Add

- `A` <= [26] Multiply output 0
- `B` <= [24] Absolute World Position output 0

### [26] Multiply

- `A` <= [23] Multiply output 0
- `B` <= [3] Multiply output 0

### [27] Subtract

- `A` <= [25] Add output 0
- `B` <= [32] Multiply(,1) output 0

### [31] Texture Sample

- `Coordinates` <= [27] Subtract output 0
- `TextureObject` <= [30] Input Cubemap (TextureCube) output 0

### [32] Multiply(,1)

- `A` <= [28] Input CapturePosition (Vector3) output 0

### [33] Output Sphere Hit Mask

- `A` <= [35] Clamp output 0

### [34] Ceil

- `Input` <= [16] Subtract output 0

### [35] Clamp

- `Input` <= [34] Ceil output 0

## Connection List

- [1] If output 0 -> Output Result.A
- [16] Subtract output 0 -> If.A
- [17] 0 output 0 -> If.B
- [31] Texture Sample output 0 -> If.AGreaterThanB
- [17] 0 output 0 -> If.AEqualsB
- [17] 0 output 0 -> If.ALessThanB
- [29] Input Radius (Scalar) output 0 -> Multiply.A
- [2] Reflection Vector output 0 -> Multiply.B
- [29] Input Radius (Scalar) output 0 -> Multiply(,1.2).A
- [4] Multiply(,1.2) output 0 -> Multiply.A
- [4] Multiply(,1.2) output 0 -> Multiply.B
- [6] Absolute World Position output 0 -> Subtract.A
- [28] Input CapturePosition (Vector3) output 0 -> Subtract.B
- [7] Subtract output 0 -> Dot.A
- [7] Subtract output 0 -> Dot.B
- [3] Multiply output 0 -> Dot.A
- [3] Multiply output 0 -> Dot.B
- [3] Multiply output 0 -> Dot.A
- [7] Subtract output 0 -> Dot.B
- [10] Dot output 0 -> Multiply(,2).A
- [8] Dot output 0 -> Subtract.A
- [5] Multiply output 0 -> Subtract.B
- [12] Subtract output 0 -> Multiply.A
- [9] Dot output 0 -> Multiply.B
- [13] Multiply output 0 -> Multiply(,4).A
- [11] Multiply(,2) output 0 -> Multiply.A
- [11] Multiply(,2) output 0 -> Multiply.B
- [15] Multiply output 0 -> Subtract.A
- [14] Multiply(,4) output 0 -> Subtract.B
- [20] Add output 0 -> Divide(0.5,).B
- [9] Dot output 0 -> Add.A
- [18] 1e-06 output 0 -> Add.B
- [16] Subtract output 0 -> Sqrt.Input
- [21] Sqrt output 0 -> Subtract.A
- [11] Multiply(,2) output 0 -> Subtract.B
- [22] Subtract output 0 -> Multiply.A
- [19] Divide(0.5,) output 0 -> Multiply.B
- [26] Multiply output 0 -> Add.A
- [24] Absolute World Position output 0 -> Add.B
- [23] Multiply output 0 -> Multiply.A
- [3] Multiply output 0 -> Multiply.B
- [25] Add output 0 -> Subtract.A
- [32] Multiply(,1) output 0 -> Subtract.B
- [27] Subtract output 0 -> Texture Sample.Coordinates
- [30] Input Cubemap (TextureCube) output 0 -> Texture Sample.TextureObject
- [28] Input CapturePosition (Vector3) output 0 -> Multiply(,1).A
- [35] Clamp output 0 -> Output Sphere Hit Mask.A
- [16] Subtract output 0 -> Ceil.Input
- [34] Ceil output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [1] If
- `Sphere Hit Mask` is driven by [35] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
