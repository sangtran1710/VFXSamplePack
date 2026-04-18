# Distance_Blend

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Particles/Distance_Blend.Distance_Blend`
- Category: Engine_MaterialFunctions03/Particles
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Sets a distance and range to blend from 0 to 1

## Inputs

- `Start Offset` [FunctionInput_Scalar] - Input should be a negative value in units
- `Blend Range` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Start Offset (Scalar) | Start Offset |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Blend Range (Scalar) | Blend Range |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Clamp output 0

### [1] Add

- `A` <= [2] PixelDepth output 0
- `B` <= [5] Input Start Offset (Scalar) output 0

### [3] Divide

- `A` <= [1] Add output 0
- `B` <= [6] Input Blend Range (Scalar) output 0

### [4] Clamp

- `Input` <= [3] Divide output 0

## Connection List

- [4] Clamp output 0 -> Output Result.A
- [2] PixelDepth output 0 -> Add.A
- [5] Input Start Offset (Scalar) output 0 -> Add.B
- [1] Add output 0 -> Divide.A
- [6] Input Blend Range (Scalar) output 0 -> Divide.B
- [3] Divide output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [4] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
