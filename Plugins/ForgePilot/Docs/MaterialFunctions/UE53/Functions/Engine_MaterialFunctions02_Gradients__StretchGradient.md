# StretchGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/StretchGradient.StretchGradient`
- Category: Engine_MaterialFunctions02/Gradients
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

A 0-1 uv gradient is scaled by a desired amount while retaining the uvs 0 and 1 value locations. It does this by stretching out the gradient at a point of the user's choosing.

## Inputs

- `Scale Factor` [FunctionInput_Scalar]
- `Center Point` [FunctionInput_Scalar]
- `0-1 Gradient` [FunctionInput_Scalar]

## Outputs

- `Resulting Gradient`
- `Clamped area` - This returns a white value for the pixels that have been stretched. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Resulting Gradient | Resulting Gradient |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionClamp` | Clamp |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 5 | expression | `MaterialExpressionClamp` | Clamp |  |
| 6 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 9 | expression | `MaterialExpressionCeil` | Ceil |  |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 11 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 12 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 13 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Scale Factor (Scalar) | Scale Factor |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Center Point (Scalar) | Center Point |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input 0-1 Gradient (Scalar) | 0-1 Gradient |
| 17 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Clamped area | Clamped area |
| 19 | expression | `MaterialExpressionSphereMask` | SphereMask |  |

## Exact Input Wiring

### [0] Output Resulting Gradient

- `A` <= [8] Lerp output 0

### [2] Multiply

- `A` <= [12] Reroute Node (reroutes wires) output 0
- `B` <= [13] Reroute Node (reroutes wires) output 0

### [3] Clamp

- `Input` <= [2] Multiply output 0
- `Max` <= [17] Reroute Node (reroutes wires) output 0

### [4] Subtract(,1)

- `A` <= [13] Reroute Node (reroutes wires) output 0

### [5] Clamp

- `Input` <= [6] Subtract output 0
- `Min` <= [17] Reroute Node (reroutes wires) output 0

### [6] Subtract

- `A` <= [2] Multiply output 0
- `B` <= [4] Subtract(,1) output 0

### [7] Subtract

- `A` <= [12] Reroute Node (reroutes wires) output 0
- `B` <= [15] Input Center Point (Scalar) output 0

### [8] Lerp

- `A` <= [3] Clamp output 0
- `B` <= [5] Clamp output 0
- `Alpha` <= [11] Reroute Node (reroutes wires) output 0

### [9] Ceil

- `Input` <= [7] Subtract output 0

### [11] Reroute Node (reroutes wires)

- `Input` <= [9] Ceil output 0

### [12] Reroute Node (reroutes wires)

- `Input` <= [16] Input 0-1 Gradient (Scalar) output 0

### [13] Reroute Node (reroutes wires)

- `Input` <= [14] Input Scale Factor (Scalar) output 0

### [16] Input 0-1 Gradient (Scalar)

- `Preview` <= [10] BreakOutFloat2Components output 0

### [17] Reroute Node (reroutes wires)

- `Input` <= [15] Input Center Point (Scalar) output 0

### [18] Output Clamped area

- `A` <= [19] SphereMask output 0

### [19] SphereMask

- `A` <= [8] Lerp output 0
- `B` <= [17] Reroute Node (reroutes wires) output 0

## Connection List

- [8] Lerp output 0 -> Output Resulting Gradient.A
- [12] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [13] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [2] Multiply output 0 -> Clamp.Input
- [17] Reroute Node (reroutes wires) output 0 -> Clamp.Max
- [13] Reroute Node (reroutes wires) output 0 -> Subtract(,1).A
- [6] Subtract output 0 -> Clamp.Input
- [17] Reroute Node (reroutes wires) output 0 -> Clamp.Min
- [2] Multiply output 0 -> Subtract.A
- [4] Subtract(,1) output 0 -> Subtract.B
- [12] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [15] Input Center Point (Scalar) output 0 -> Subtract.B
- [3] Clamp output 0 -> Lerp.A
- [5] Clamp output 0 -> Lerp.B
- [11] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [7] Subtract output 0 -> Ceil.Input
- [9] Ceil output 0 -> Reroute Node (reroutes wires).Input
- [16] Input 0-1 Gradient (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [14] Input Scale Factor (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [10] BreakOutFloat2Components output 0 -> Input 0-1 Gradient (Scalar).Preview
- [15] Input Center Point (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [19] SphereMask output 0 -> Output Clamped area.A
- [8] Lerp output 0 -> SphereMask.A
- [17] Reroute Node (reroutes wires) output 0 -> SphereMask.B

## Output Trace Roots

- `Resulting Gradient` is driven by [8] Lerp
- `Clamped area` is driven by [19] SphereMask

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
