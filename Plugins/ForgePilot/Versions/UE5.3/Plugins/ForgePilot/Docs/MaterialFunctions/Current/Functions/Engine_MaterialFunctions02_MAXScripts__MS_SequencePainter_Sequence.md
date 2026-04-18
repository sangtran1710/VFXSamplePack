# MS_SequencePainter_Sequence

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_SequencePainter_Sequence.MS_SequencePainter_Sequence`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Inputs

- None

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 3 | expression | `MaterialExpressionCeil` | Ceil |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Ceil output 0

### [3] Ceil

- `Input` <= [2] BreakOutFloat2Components output 0

## Connection List

- [3] Ceil output 0 -> Output Result.A
- [2] BreakOutFloat2Components output 0 -> Ceil.Input

## Output Trace Roots

- `Result` is driven by [3] Ceil

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


