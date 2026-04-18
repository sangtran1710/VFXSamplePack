# TreeAnimationSines

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/TreeAnimationSines.TreeAnimationSines`
- Category: Engine_MaterialFunctions02/PivotPainter
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Add time and a gradient to create an animation used for trees

## Inputs

- `LS4` [FunctionInput_Scalar]
- `Time and Grad` [FunctionInput_Scalar] - FoliageSineWaves
- `LS3` [FunctionInput_Scalar]
- `LS2` [FunctionInput_Scalar]
- `LS1` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearSine |  |
| 5 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input LS4 (Scalar) | LS4 |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Time and Grad (Scalar) | Time and Grad |
| 9 | expression | `MaterialExpressionTime` | Time |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearSine |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input LS3 (Scalar) | LS3 |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearSine |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input LS2 (Scalar) | LS2 |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearSine |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input LS1 (Scalar) | LS1 |
| 17 | expression | `MaterialExpressionClamp` | Clamp |  |
| 18 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Mask ( R ) output 0

### [1] Mask ( R )

- `Input` <= [2] Add output 0

### [2] Add

- `A` <= [17] Clamp output 0
- `B` <= [3] Add output 0

### [3] Add

- `A` <= [10] Multiply output 0
- `B` <= [18] Clamp output 0

### [7] Mask ( R )

- `Input` <= [8] Input Time and Grad (Scalar) output 0

### [8] Input Time and Grad (Scalar)

- `Preview` <= [9] Time output 0

### [10] Multiply

- `A` <= [13] LinearSine output 1
- `B` <= [11] LinearSine output 1

### [17] Clamp

- `Input` <= [15] LinearSine output 0

### [18] Clamp

- `Input` <= [4] LinearSine output 0

## Connection List

- [1] Mask ( R ) output 0 -> Output Result.A
- [2] Add output 0 -> Mask ( R ).Input
- [17] Clamp output 0 -> Add.A
- [3] Add output 0 -> Add.B
- [10] Multiply output 0 -> Add.A
- [18] Clamp output 0 -> Add.B
- [8] Input Time and Grad (Scalar) output 0 -> Mask ( R ).Input
- [9] Time output 0 -> Input Time and Grad (Scalar).Preview
- [13] LinearSine output 1 -> Multiply.A
- [11] LinearSine output 1 -> Multiply.B
- [15] LinearSine output 0 -> Clamp.Input
- [4] LinearSine output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [1] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


