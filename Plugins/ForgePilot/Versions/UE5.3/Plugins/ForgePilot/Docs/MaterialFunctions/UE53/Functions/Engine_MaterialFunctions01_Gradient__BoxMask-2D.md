# BoxMask-2D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/BoxMask-2D.BoxMask-2D`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Draws a box in 2D Space. Controls for size and falloff.

## Inputs

- `A` [FunctionInput_Vector2] - The Position Coordinates to measure the box. Default is World Position and can be left default in most cases.
- `Bounds` [FunctionInput_Vector2] - The bounds of the cube in X Y Z.
- `B` [FunctionInput_Vector2] - The Center Point location of the box
- `Edge Falloff` [FunctionInput_Scalar] - Falloff of the edge gradient in units.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 2 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorLength |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 8 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input A (Vector2) | A |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Bounds (Vector2) | Bounds |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input B (Vector2) | B |
| 13 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Edge Falloff (Scalar) | Edge Falloff |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] 1-x output 0

### [1] Subtract

- `A` <= [5] Abs output 0
- `B` <= [11] Multiply(,0.5) output 0

### [2] Max(,0)

- `A` <= [1] Subtract output 0

### [4] Clamp

- `Input` <= [6] Divide output 0

### [5] Abs

- `Input` <= [13] Subtract output 0

### [6] Divide

- `A` <= [3] VectorLength output 1
- `B` <= [14] Input Edge Falloff (Scalar) output 0

### [7] 1-x

- `Input` <= [4] Clamp output 0

### [9] Input A (Vector2)

- `Preview` <= [8] Absolute World Position output 0

### [11] Multiply(,0.5)

- `A` <= [10] Input Bounds (Vector2) output 0

### [13] Subtract

- `A` <= [9] Input A (Vector2) output 0
- `B` <= [12] Input B (Vector2) output 0

## Connection List

- [7] 1-x output 0 -> Output Result.A
- [5] Abs output 0 -> Subtract.A
- [11] Multiply(,0.5) output 0 -> Subtract.B
- [1] Subtract output 0 -> Max(,0).A
- [6] Divide output 0 -> Clamp.Input
- [13] Subtract output 0 -> Abs.Input
- [3] VectorLength output 1 -> Divide.A
- [14] Input Edge Falloff (Scalar) output 0 -> Divide.B
- [4] Clamp output 0 -> 1-x.Input
- [8] Absolute World Position output 0 -> Input A (Vector2).Preview
- [10] Input Bounds (Vector2) output 0 -> Multiply(,0.5).A
- [9] Input A (Vector2) output 0 -> Subtract.A
- [12] Input B (Vector2) output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [7] 1-x

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
