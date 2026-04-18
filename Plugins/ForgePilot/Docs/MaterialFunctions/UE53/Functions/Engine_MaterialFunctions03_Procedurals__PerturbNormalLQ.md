# PerturbNormalLQ

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/PerturbNormalLQ.PerturbNormalLQ`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `Bump` [FunctionInput_Scalar]

## Outputs

- `WS Normal`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output WS Normal | WS Normal |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionDDX` | DDX |  |
| 3 | expression | `MaterialExpressionDDY` | DDY |  |
| 4 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 5 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 6 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Bump (Scalar) | Bump |
| 8 | expression | `MaterialExpressionDDY` | DDY |  |
| 9 | expression | `MaterialExpressionDDX` | DDX |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 14 | expression | `MaterialExpressionIf` | If |  |
| 15 | expression | `MaterialExpressionConstant` | 0 |  |
| 16 | expression | `MaterialExpressionConstant` | 1 |  |
| 17 | expression | `MaterialExpressionConstant` | -1 |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionAbs` | Abs |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 22 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output WS Normal

- `A` <= [22] Normalize output 0

### [2] DDX

- `Value` <= [1] Absolute World Position output 0

### [3] DDY

- `Value` <= [1] Absolute World Position output 0

### [5] Cross

- `A` <= [3] DDY output 0
- `B` <= [4] VertexNormalWS output 0

### [6] Cross

- `A` <= [4] VertexNormalWS output 0
- `B` <= [2] DDX output 0

### [8] DDY

- `Value` <= [7] Input Bump (Scalar) output 0

### [9] DDX

- `Value` <= [7] Input Bump (Scalar) output 0

### [10] Multiply

- `A` <= [8] DDY output 0
- `B` <= [5] Cross output 0

### [11] Multiply

- `A` <= [9] DDX output 0
- `B` <= [6] Cross output 0

### [12] Add

- `A` <= [10] Multiply output 0
- `B` <= [11] Multiply output 0

### [13] Dot

- `A` <= [5] Cross output 0
- `B` <= [2] DDX output 0

### [14] If

- `A` <= [13] Dot output 0
- `B` <= [15] 0 output 0
- `AGreaterThanB` <= [16] 1 output 0
- `AEqualsB` <= [15] 0 output 0
- `ALessThanB` <= [17] -1 output 0

### [18] Multiply

- `A` <= [12] Add output 0
- `B` <= [14] If output 0

### [19] Abs

- `Input` <= [13] Dot output 0

### [20] Multiply

- `A` <= [19] Abs output 0
- `B` <= [4] VertexNormalWS output 0

### [21] Subtract

- `A` <= [20] Multiply output 0
- `B` <= [18] Multiply output 0

### [22] Normalize

- `VectorInput` <= [21] Subtract output 0

## Connection List

- [22] Normalize output 0 -> Output WS Normal.A
- [1] Absolute World Position output 0 -> DDX.Value
- [1] Absolute World Position output 0 -> DDY.Value
- [3] DDY output 0 -> Cross.A
- [4] VertexNormalWS output 0 -> Cross.B
- [4] VertexNormalWS output 0 -> Cross.A
- [2] DDX output 0 -> Cross.B
- [7] Input Bump (Scalar) output 0 -> DDY.Value
- [7] Input Bump (Scalar) output 0 -> DDX.Value
- [8] DDY output 0 -> Multiply.A
- [5] Cross output 0 -> Multiply.B
- [9] DDX output 0 -> Multiply.A
- [6] Cross output 0 -> Multiply.B
- [10] Multiply output 0 -> Add.A
- [11] Multiply output 0 -> Add.B
- [5] Cross output 0 -> Dot.A
- [2] DDX output 0 -> Dot.B
- [13] Dot output 0 -> If.A
- [15] 0 output 0 -> If.B
- [16] 1 output 0 -> If.AGreaterThanB
- [15] 0 output 0 -> If.AEqualsB
- [17] -1 output 0 -> If.ALessThanB
- [12] Add output 0 -> Multiply.A
- [14] If output 0 -> Multiply.B
- [13] Dot output 0 -> Abs.Input
- [19] Abs output 0 -> Multiply.A
- [4] VertexNormalWS output 0 -> Multiply.B
- [20] Multiply output 0 -> Subtract.A
- [18] Multiply output 0 -> Subtract.B
- [21] Subtract output 0 -> Normalize.VectorInput

## Output Trace Roots

- `WS Normal` is driven by [22] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
