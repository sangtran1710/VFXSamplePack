# Gravity_WPO

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Gravity_WPO.Gravity_WPO`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Find the velocity and updated position of an object that's falling on earth. 

## Inputs

- `Current Time` [FunctionInput_Scalar]
- `Fall Start Time` [FunctionInput_Scalar]
- `Gravitational Acceleration` [FunctionInput_Vector3]

## Outputs

- `Offset From Start Position`
- `Instantaneous Velocity`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Offset From Start Position | Offset From Start Position |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Current Time (Scalar) | Current Time |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Fall Start Time (Scalar) | Fall Start Time |
| 3 | expression | `MaterialExpressionTime` | Time |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Gravitational Acceleration (Vector3) | Gravitational Acceleration |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Instantaneous Velocity | Instantaneous Velocity |
| 11 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 12 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 13 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Offset From Start Position

- `A` <= [5] Multiply output 0

### [1] Input Current Time (Scalar)

- `Preview` <= [3] Time output 0

### [4] Subtract

- `A` <= [1] Input Current Time (Scalar) output 0
- `B` <= [2] Input Fall Start Time (Scalar) output 0

### [5] Multiply

- `A` <= [6] Multiply output 0
- `B` <= [7] Multiply(,0.5) output 0

### [6] Multiply

- `A` <= [13] Clamp output 0
- `B` <= [13] Clamp output 0

### [7] Multiply(,0.5)

- `A` <= [8] Input Gravitational Acceleration (Vector3) output 0

### [9] Multiply

- `A` <= [12] Reroute Node (reroutes wires) output 0
- `B` <= [11] Reroute Node (reroutes wires) output 0

### [10] Output Instantaneous Velocity

- `A` <= [9] Multiply output 0

### [11] Reroute Node (reroutes wires)

- `Input` <= [8] Input Gravitational Acceleration (Vector3) output 0

### [12] Reroute Node (reroutes wires)

- `Input` <= [13] Clamp output 0

### [13] Clamp

- `Input` <= [4] Subtract output 0

## Connection List

- [5] Multiply output 0 -> Output Offset From Start Position.A
- [3] Time output 0 -> Input Current Time (Scalar).Preview
- [1] Input Current Time (Scalar) output 0 -> Subtract.A
- [2] Input Fall Start Time (Scalar) output 0 -> Subtract.B
- [6] Multiply output 0 -> Multiply.A
- [7] Multiply(,0.5) output 0 -> Multiply.B
- [13] Clamp output 0 -> Multiply.A
- [13] Clamp output 0 -> Multiply.B
- [8] Input Gravitational Acceleration (Vector3) output 0 -> Multiply(,0.5).A
- [12] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [11] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [9] Multiply output 0 -> Output Instantaneous Velocity.A
- [8] Input Gravitational Acceleration (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [13] Clamp output 0 -> Reroute Node (reroutes wires).Input
- [4] Subtract output 0 -> Clamp.Input

## Output Trace Roots

- `Offset From Start Position` is driven by [5] Multiply
- `Instantaneous Velocity` is driven by [9] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
