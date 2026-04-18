# ComputeFilterWidth

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/ComputeFilterWidth.ComputeFilterWidth`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `In` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In (Vector3) | In |
| 2 | expression | `MaterialExpressionDDX` | DDX |  |
| 3 | expression | `MaterialExpressionDDY` | DDY |  |
| 4 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionIf` | If |  |
| 7 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Sqrt output 0

### [2] DDX

- `Value` <= [1] Input In (Vector3) output 0

### [3] DDY

- `Value` <= [1] Input In (Vector3) output 0

### [4] Dot

- `A` <= [2] DDX output 0
- `B` <= [2] DDX output 0

### [5] Dot

- `A` <= [3] DDY output 0
- `B` <= [3] DDY output 0

### [6] If

- `A` <= [4] Dot output 0
- `B` <= [5] Dot output 0
- `AGreaterThanB` <= [4] Dot output 0
- `AEqualsB` <= [5] Dot output 0
- `ALessThanB` <= [5] Dot output 0

### [7] Sqrt

- `Input` <= [6] If output 0

## Connection List

- [7] Sqrt output 0 -> Output Result.A
- [1] Input In (Vector3) output 0 -> DDX.Value
- [1] Input In (Vector3) output 0 -> DDY.Value
- [2] DDX output 0 -> Dot.A
- [2] DDX output 0 -> Dot.B
- [3] DDY output 0 -> Dot.A
- [3] DDY output 0 -> Dot.B
- [4] Dot output 0 -> If.A
- [5] Dot output 0 -> If.B
- [4] Dot output 0 -> If.AGreaterThanB
- [5] Dot output 0 -> If.AEqualsB
- [5] Dot output 0 -> If.ALessThanB
- [6] If output 0 -> Sqrt.Input

## Output Trace Roots

- `Result` is driven by [7] Sqrt

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
