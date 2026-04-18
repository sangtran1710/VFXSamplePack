# Blend_ColorBurn

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_ColorBurn.Blend_ColorBurn`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

The darker the Blend texture, the more color in the final result.  If the Blend is white, there will be no change.

## Inputs

- `Blend` [FunctionInput_Vector3] - Blend texture input
- `Base` [FunctionInput_Vector3] - Base texture input

## Outputs

- `Result` - The darker the Blend texture, the more color in the final result.  If the Blend is white, there will be no change.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Blend (Vector3) | Blend |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 7 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] 1-x output 0

### [1] 1-x

- `Input` <= [2] Divide output 0

### [2] Divide

- `A` <= [5] 1-x output 0
- `B` <= [3] Input Blend (Vector3) output 0

### [3] Input Blend (Vector3)

- `Preview` <= [4] Texture Sample output 0

### [5] 1-x

- `Input` <= [6] Input Base (Vector3) output 0

### [6] Input Base (Vector3)

- `Preview` <= [7] Texture Sample output 0

## Connection List

- [1] 1-x output 0 -> Output Result.A
- [2] Divide output 0 -> 1-x.Input
- [5] 1-x output 0 -> Divide.A
- [3] Input Blend (Vector3) output 0 -> Divide.B
- [4] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [6] Input Base (Vector3) output 0 -> 1-x.Input
- [7] Texture Sample output 0 -> Input Base (Vector3).Preview

## Output Trace Roots

- `Result` is driven by [1] 1-x

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


