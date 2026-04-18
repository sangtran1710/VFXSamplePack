# RayTraceSphereFalloff

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/RayTraceSphereFalloff.RayTraceSphereFalloff`
- Category: Engine_MaterialFunctions02/Gradients
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

finds the distance to the sphere

## Inputs

- `Sphere Radius` [FunctionInput_Scalar]
- `Sphere Falloff` [FunctionInput_Scalar]
- `Ray Origin` [FunctionInput_Vector3]
- `Sphere Center` [FunctionInput_Vector3]
- `Unit Ray Direction` [FunctionInput_Vector3]

## Outputs

- `Clamped Falloff`
- `Unclamped Results`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Clamped Falloff | Clamped Falloff |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 5 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionDistance` | Distance |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | expression | `MaterialExpressionDivide` | Divide |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Radius (Scalar) | Sphere Radius |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Falloff (Scalar) | Sphere Falloff |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Ray Origin (Vector3) | Ray Origin |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Center (Vector3) | Sphere Center |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Unclamped Results | Unclamped Results |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Unit Ray Direction (Vector3) | Unit Ray Direction |
| 19 | expression | `MaterialExpressionConstant` | 0 |  |
| 20 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 21 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 22 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 23 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 24 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 28 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 29 | expression | `MaterialExpressionClamp` | Clamp |  |
| 30 | expression | `MaterialExpressionDivide` | Divide |  |
| 31 | expression | `MaterialExpressionScalarParameter` | Param (10) 'Sphere Falloff' |  |
| 32 | expression | `MaterialExpressionScalarParameter` | Param (10) 'Sphere Radius' |  |
| 33 | expression | `MaterialExpressionDistance` | Distance |  |
| 34 | expression | `MaterialExpressionConstant` | 0 |  |
| 35 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output Clamped Falloff

- `A` <= [11] Clamp output 0

### [1] Dot

- `A` <= [18] Input Unit Ray Direction (Vector3) output 0
- `B` <= [3] Subtract output 0

### [3] Subtract

- `A` <= [16] Input Sphere Center (Vector3) output 0
- `B` <= [15] Input Ray Origin (Vector3) output 0

### [6] Add

- `A` <= [7] Multiply output 0
- `B` <= [15] Input Ray Origin (Vector3) output 0

### [7] Multiply

- `A` <= [1] Dot output 0
- `B` <= [18] Input Unit Ray Direction (Vector3) output 0

### [8] Distance

- `A` <= [19] 0 output 0
- `B` <= [9] Subtract output 0

### [9] Subtract

- `A` <= [6] Add output 0
- `B` <= [16] Input Sphere Center (Vector3) output 0

### [10] Subtract

- `A` <= [8] Distance output 0
- `B` <= [13] Input Sphere Radius (Scalar) output 0

### [11] Clamp

- `Input` <= [12] Divide output 0

### [12] Divide

- `A` <= [10] Subtract output 0
- `B` <= [14] Input Sphere Falloff (Scalar) output 0

### [15] Input Ray Origin (Vector3)

- `Preview` <= [4] Camera Position output 0

### [16] Input Sphere Center (Vector3)

- `Preview` <= [2] Actor Position (Absolute) output 0

### [17] Output Unclamped Results

- `A` <= [12] Divide output 0

### [18] Input Unit Ray Direction (Vector3)

- `Preview` <= [5] Camera Vector output 0

### [20] Dot

- `A` <= [24] Camera Vector output 0
- `B` <= [22] Subtract output 0

### [22] Subtract

- `A` <= [21] Actor Position (Absolute) output 0
- `B` <= [23] Camera Position output 0

### [25] Add

- `A` <= [23] Camera Position output 0
- `B` <= [26] Multiply output 0

### [26] Multiply

- `A` <= [20] Dot output 0
- `B` <= [24] Camera Vector output 0

### [27] Subtract

- `A` <= [25] Add output 0
- `B` <= [21] Actor Position (Absolute) output 0

### [28] Subtract

- `A` <= [33] Distance output 0
- `B` <= [32] Param (10) 'Sphere Radius' output 0

### [29] Clamp

- `Input` <= [30] Divide output 0

### [30] Divide

- `A` <= [28] Subtract output 0
- `B` <= [31] Param (10) 'Sphere Falloff' output 0

### [33] Distance

- `A` <= [34] 0 output 0
- `B` <= [27] Subtract output 0

### [35] 1-x

- `Input` <= [29] Clamp output 0

## Connection List

- [11] Clamp output 0 -> Output Clamped Falloff.A
- [18] Input Unit Ray Direction (Vector3) output 0 -> Dot.A
- [3] Subtract output 0 -> Dot.B
- [16] Input Sphere Center (Vector3) output 0 -> Subtract.A
- [15] Input Ray Origin (Vector3) output 0 -> Subtract.B
- [7] Multiply output 0 -> Add.A
- [15] Input Ray Origin (Vector3) output 0 -> Add.B
- [1] Dot output 0 -> Multiply.A
- [18] Input Unit Ray Direction (Vector3) output 0 -> Multiply.B
- [19] 0 output 0 -> Distance.A
- [9] Subtract output 0 -> Distance.B
- [6] Add output 0 -> Subtract.A
- [16] Input Sphere Center (Vector3) output 0 -> Subtract.B
- [8] Distance output 0 -> Subtract.A
- [13] Input Sphere Radius (Scalar) output 0 -> Subtract.B
- [12] Divide output 0 -> Clamp.Input
- [10] Subtract output 0 -> Divide.A
- [14] Input Sphere Falloff (Scalar) output 0 -> Divide.B
- [4] Camera Position output 0 -> Input Ray Origin (Vector3).Preview
- [2] Actor Position (Absolute) output 0 -> Input Sphere Center (Vector3).Preview
- [12] Divide output 0 -> Output Unclamped Results.A
- [5] Camera Vector output 0 -> Input Unit Ray Direction (Vector3).Preview
- [24] Camera Vector output 0 -> Dot.A
- [22] Subtract output 0 -> Dot.B
- [21] Actor Position (Absolute) output 0 -> Subtract.A
- [23] Camera Position output 0 -> Subtract.B
- [23] Camera Position output 0 -> Add.A
- [26] Multiply output 0 -> Add.B
- [20] Dot output 0 -> Multiply.A
- [24] Camera Vector output 0 -> Multiply.B
- [25] Add output 0 -> Subtract.A
- [21] Actor Position (Absolute) output 0 -> Subtract.B
- [33] Distance output 0 -> Subtract.A
- [32] Param (10) 'Sphere Radius' output 0 -> Subtract.B
- [30] Divide output 0 -> Clamp.Input
- [28] Subtract output 0 -> Divide.A
- [31] Param (10) 'Sphere Falloff' output 0 -> Divide.B
- [34] 0 output 0 -> Distance.A
- [27] Subtract output 0 -> Distance.B
- [29] Clamp output 0 -> 1-x.Input

## Output Trace Roots

- `Clamped Falloff` is driven by [11] Clamp
- `Unclamped Results` is driven by [12] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


