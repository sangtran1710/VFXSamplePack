# MF_SmoothMin

- Path: `/Engine/Functions/Strata/MF_SmoothMin.MF_SmoothMin`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `A` [FunctionInput_Scalar]
- `B` [FunctionInput_Scalar]
- `distance` [FunctionInput_Scalar] - blend distance of the two inputs (called k)

## Outputs

- `SmoothMin`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output SmoothMin | SmoothMin |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input A (Scalar) | A |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input B (Scalar) | B |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input distance (Scalar) | distance |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionMin` | Min |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionPower` | Power(X, 3) |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,0.1667) |  |
| 14 | expression | `MaterialExpressionIf` | If |  |
| 15 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 16 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 17 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output SmoothMin

- `A` <= [14] If output 0

### [4] Subtract

- `A` <= [1] Input A (Scalar) output 0
- `B` <= [2] Input B (Scalar) output 0

### [5] Abs

- `Input` <= [4] Subtract output 0

### [6] Subtract

- `A` <= [3] Input distance (Scalar) output 0
- `B` <= [5] Abs output 0

### [7] Max(,0)

- `A` <= [6] Subtract output 0

### [8] Divide

- `A` <= [7] Max(,0) output 0
- `B` <= [16] Reroute Node (reroutes wires) output 0

### [9] Min

- `A` <= [1] Input A (Scalar) output 0
- `B` <= [2] Input B (Scalar) output 0

### [10] Multiply

- `A` <= [12] Power(X, 3) output 0
- `B` <= [17] Reroute Node (reroutes wires) output 0

### [11] Subtract

- `A` <= [9] Min output 0
- `B` <= [13] Multiply(,0.1667) output 0

### [12] Power(X, 3)

- `Base` <= [8] Divide output 0

### [13] Multiply(,0.1667)

- `A` <= [10] Multiply output 0

### [14] If

- `A` <= [17] Reroute Node (reroutes wires) output 0
- `AGreaterThanB` <= [11] Subtract output 0
- `AEqualsB` <= [15] Reroute Node (reroutes wires) output 0
- `ALessThanB` <= [11] Subtract output 0

### [15] Reroute Node (reroutes wires)

- `Input` <= [9] Min output 0

### [16] Reroute Node (reroutes wires)

- `Input` <= [3] Input distance (Scalar) output 0

### [17] Reroute Node (reroutes wires)

- `Input` <= [16] Reroute Node (reroutes wires) output 0

## Connection List

- [14] If output 0 -> Output SmoothMin.A
- [1] Input A (Scalar) output 0 -> Subtract.A
- [2] Input B (Scalar) output 0 -> Subtract.B
- [4] Subtract output 0 -> Abs.Input
- [3] Input distance (Scalar) output 0 -> Subtract.A
- [5] Abs output 0 -> Subtract.B
- [6] Subtract output 0 -> Max(,0).A
- [7] Max(,0) output 0 -> Divide.A
- [16] Reroute Node (reroutes wires) output 0 -> Divide.B
- [1] Input A (Scalar) output 0 -> Min.A
- [2] Input B (Scalar) output 0 -> Min.B
- [12] Power(X, 3) output 0 -> Multiply.A
- [17] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [9] Min output 0 -> Subtract.A
- [13] Multiply(,0.1667) output 0 -> Subtract.B
- [8] Divide output 0 -> Power(X, 3).Base
- [10] Multiply output 0 -> Multiply(,0.1667).A
- [17] Reroute Node (reroutes wires) output 0 -> If.A
- [11] Subtract output 0 -> If.AGreaterThanB
- [15] Reroute Node (reroutes wires) output 0 -> If.AEqualsB
- [11] Subtract output 0 -> If.ALessThanB
- [9] Min output 0 -> Reroute Node (reroutes wires).Input
- [3] Input distance (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [16] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `SmoothMin` is driven by [14] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
