# PreparePerturbNormalHQ

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/PreparePerturbNormalHQ.PreparePerturbNormalHQ`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `Value` [FunctionInput_Vector3]

## Outputs

- `Value one pixel right`
- `Value`
- `Value one pixel down`
- `FilterWidth`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Value one pixel right | Value one pixel right |
| 1 | expression | `MaterialExpressionDDX` | DDX |  |
| 2 | expression | `MaterialExpressionDDY` | DDY |  |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Value | Value |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Value one pixel down | Value one pixel down |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Value (Vector3) | Value |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output FilterWidth | FilterWidth |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionIf` | If |  |
| 12 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |

## Exact Input Wiring

### [0] Output Value one pixel right

- `A` <= [4] Add output 0

### [1] DDX

- `Value` <= [7] Input Value (Vector3) output 0

### [2] DDY

- `Value` <= [7] Input Value (Vector3) output 0

### [3] Add

- `A` <= [7] Input Value (Vector3) output 0
- `B` <= [2] DDY output 0

### [4] Add

- `A` <= [7] Input Value (Vector3) output 0
- `B` <= [1] DDX output 0

### [5] Output Value

- `A` <= [7] Input Value (Vector3) output 0

### [6] Output Value one pixel down

- `A` <= [3] Add output 0

### [8] Output FilterWidth

- `A` <= [12] Sqrt output 0

### [9] Dot

- `A` <= [1] DDX output 0
- `B` <= [1] DDX output 0

### [10] Dot

- `A` <= [2] DDY output 0
- `B` <= [2] DDY output 0

### [11] If

- `A` <= [9] Dot output 0
- `B` <= [10] Dot output 0
- `AGreaterThanB` <= [9] Dot output 0
- `ALessThanB` <= [10] Dot output 0

### [12] Sqrt

- `Input` <= [11] If output 0

## Connection List

- [4] Add output 0 -> Output Value one pixel right.A
- [7] Input Value (Vector3) output 0 -> DDX.Value
- [7] Input Value (Vector3) output 0 -> DDY.Value
- [7] Input Value (Vector3) output 0 -> Add.A
- [2] DDY output 0 -> Add.B
- [7] Input Value (Vector3) output 0 -> Add.A
- [1] DDX output 0 -> Add.B
- [7] Input Value (Vector3) output 0 -> Output Value.A
- [3] Add output 0 -> Output Value one pixel down.A
- [12] Sqrt output 0 -> Output FilterWidth.A
- [1] DDX output 0 -> Dot.A
- [1] DDX output 0 -> Dot.B
- [2] DDY output 0 -> Dot.A
- [2] DDY output 0 -> Dot.B
- [9] Dot output 0 -> If.A
- [10] Dot output 0 -> If.B
- [9] Dot output 0 -> If.AGreaterThanB
- [10] Dot output 0 -> If.ALessThanB
- [11] If output 0 -> Sqrt.Input

## Output Trace Roots

- `Value one pixel right` is driven by [4] Add
- `Value` is driven by [7] Input Value (Vector3)
- `Value one pixel down` is driven by [3] Add
- `FilterWidth` is driven by [12] Sqrt

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


