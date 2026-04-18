# Blend_Exclusion

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Exclusion.Blend_Exclusion`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Halves the Blend and Base, combines them together, then partially inverts the results.

## Inputs

- `Base` [FunctionInput_Vector3] - Base texture input
- `Blend` [FunctionInput_Vector3] - Blend texture input

## Outputs

- `Result` - Adds the base and blend together.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Blend (Vector3) | Blend |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant` | 2 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [9] Subtract output 0

### [1] Input Base (Vector3)

- `Preview` <= [3] Texture Sample output 0

### [2] Input Blend (Vector3)

- `Preview` <= [4] Texture Sample output 0

### [5] Add

- `A` <= [1] Input Base (Vector3) output 0
- `B` <= [10] -0.5 output 0

### [6] Add

- `A` <= [2] Input Blend (Vector3) output 0
- `B` <= [10] -0.5 output 0

### [7] Multiply

- `A` <= [5] Add output 0
- `B` <= [6] Add output 0

### [9] Subtract

- `A` <= [8] 0.5 output 0
- `B` <= [11] Multiply output 0

### [11] Multiply

- `A` <= [7] Multiply output 0
- `B` <= [12] 2 output 0

## Connection List

- [9] Subtract output 0 -> Output Result.A
- [3] Texture Sample output 0 -> Input Base (Vector3).Preview
- [4] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [1] Input Base (Vector3) output 0 -> Add.A
- [10] -0.5 output 0 -> Add.B
- [2] Input Blend (Vector3) output 0 -> Add.A
- [10] -0.5 output 0 -> Add.B
- [5] Add output 0 -> Multiply.A
- [6] Add output 0 -> Multiply.B
- [8] 0.5 output 0 -> Subtract.A
- [11] Multiply output 0 -> Subtract.B
- [7] Multiply output 0 -> Multiply.A
- [12] 2 output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [9] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
