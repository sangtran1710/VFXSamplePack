# Blend_Screen

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Screen.Blend_Screen`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Inverts each texture, multiplies them together, then re-inverts the results.

## Inputs

- `Base` [FunctionInput_Vector3] - Base texture input
- `Blend` [FunctionInput_Vector3] - Blend texture input

## Outputs

- `Result` - Inverts each texture, multiplies them together, then re-inverts the results.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Blend (Vector3) | Blend |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] 1-x output 0

### [1] Input Base (Vector3)

- `Preview` <= [3] Texture Sample output 0

### [2] Input Blend (Vector3)

- `Preview` <= [4] Texture Sample output 0

### [5] 1-x

- `Input` <= [1] Input Base (Vector3) output 0

### [6] 1-x

- `Input` <= [2] Input Blend (Vector3) output 0

### [7] Multiply

- `A` <= [5] 1-x output 0
- `B` <= [6] 1-x output 0

### [8] 1-x

- `Input` <= [7] Multiply output 0

## Connection List

- [8] 1-x output 0 -> Output Result.A
- [3] Texture Sample output 0 -> Input Base (Vector3).Preview
- [4] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [1] Input Base (Vector3) output 0 -> 1-x.Input
- [2] Input Blend (Vector3) output 0 -> 1-x.Input
- [5] 1-x output 0 -> Multiply.A
- [6] 1-x output 0 -> Multiply.B
- [7] Multiply output 0 -> 1-x.Input

## Output Trace Roots

- `Result` is driven by [8] 1-x

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
