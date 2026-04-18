# ThresholdWithRange

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Masking/ThresholdWithRange.ThresholdWithRange`
- Category: Engine_MaterialFunctions02/Masking
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `ThresholdValue` [FunctionInput_Scalar]
- `ThresholdRange` [FunctionInput_Scalar]
- `TwoSidedThreshold` [FunctionInput_StaticBool]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input ThresholdValue (Scalar) | ThresholdValue |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input ThresholdRange (Scalar) | ThresholdRange |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionAbs` | Abs |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionConstant` | 1 |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | expression | `MaterialExpressionClamp` | Clamp |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input TwoSidedThreshold (StaticBool) | TwoSidedThreshold |
| 14 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 15 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 16 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 17 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionConstant` | 1 |  |
| 20 | expression | `MaterialExpressionDivide` | Divide |  |
| 21 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 22 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 23 | expression | `MaterialExpressionClamp` | Clamp |  |
| 24 | expression | `MaterialExpressionClamp` | Clamp |  |
| 25 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [23] Clamp output 0

### [1] Input Alpha (Scalar)

- `Preview` <= [4] LinearGradient output 0

### [5] Subtract

- `A` <= [1] Input Alpha (Scalar) output 0
- `B` <= [2] Input ThresholdValue (Scalar) output 0

### [6] Abs

- `Input` <= [5] Subtract output 0

### [7] Subtract

- `A` <= [6] Abs output 0
- `B` <= [24] Clamp output 0

### [8] Multiply(,-1)

- `A` <= [7] Subtract output 0

### [9] Multiply

- `A` <= [8] Multiply(,-1) output 0
- `B` <= [11] Divide output 0

### [11] Divide

- `A` <= [10] 1 output 0
- `B` <= [24] Clamp output 0

### [12] Clamp

- `Input` <= [9] Multiply output 0

### [13] Input TwoSidedThreshold (StaticBool)

- `Preview` <= [14] Static Bool (False) output 0

### [15] Subtract

- `A` <= [1] Input Alpha (Scalar) output 0
- `B` <= [2] Input ThresholdValue (Scalar) output 0

### [16] Add(,1)

- `A` <= [15] Subtract output 0

### [17] 1-x

- `Input` <= [16] Add(,1) output 0

### [18] Multiply

- `A` <= [17] 1-x output 0
- `B` <= [20] Divide output 0

### [20] Divide

- `A` <= [19] 1 output 0
- `B` <= [24] Clamp output 0

### [21] 1-x

- `Input` <= [18] Multiply output 0

### [22] Switch

- `A` <= [12] Clamp output 0
- `B` <= [21] 1-x output 0
- `Value` <= [25] Reroute Node (reroutes wires) output 0

### [23] Clamp

- `Input` <= [22] Switch output 0

### [24] Clamp

- `Input` <= [3] Input ThresholdRange (Scalar) output 0

### [25] Reroute Node (reroutes wires)

- `Input` <= [13] Input TwoSidedThreshold (StaticBool) output 0

## Connection List

- [23] Clamp output 0 -> Output Result.A
- [4] LinearGradient output 0 -> Input Alpha (Scalar).Preview
- [1] Input Alpha (Scalar) output 0 -> Subtract.A
- [2] Input ThresholdValue (Scalar) output 0 -> Subtract.B
- [5] Subtract output 0 -> Abs.Input
- [6] Abs output 0 -> Subtract.A
- [24] Clamp output 0 -> Subtract.B
- [7] Subtract output 0 -> Multiply(,-1).A
- [8] Multiply(,-1) output 0 -> Multiply.A
- [11] Divide output 0 -> Multiply.B
- [10] 1 output 0 -> Divide.A
- [24] Clamp output 0 -> Divide.B
- [9] Multiply output 0 -> Clamp.Input
- [14] Static Bool (False) output 0 -> Input TwoSidedThreshold (StaticBool).Preview
- [1] Input Alpha (Scalar) output 0 -> Subtract.A
- [2] Input ThresholdValue (Scalar) output 0 -> Subtract.B
- [15] Subtract output 0 -> Add(,1).A
- [16] Add(,1) output 0 -> 1-x.Input
- [17] 1-x output 0 -> Multiply.A
- [20] Divide output 0 -> Multiply.B
- [19] 1 output 0 -> Divide.A
- [24] Clamp output 0 -> Divide.B
- [18] Multiply output 0 -> 1-x.Input
- [12] Clamp output 0 -> Switch.A
- [21] 1-x output 0 -> Switch.B
- [25] Reroute Node (reroutes wires) output 0 -> Switch.Value
- [22] Switch output 0 -> Clamp.Input
- [3] Input ThresholdRange (Scalar) output 0 -> Clamp.Input
- [13] Input TwoSidedThreshold (StaticBool) output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `Result` is driven by [23] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


