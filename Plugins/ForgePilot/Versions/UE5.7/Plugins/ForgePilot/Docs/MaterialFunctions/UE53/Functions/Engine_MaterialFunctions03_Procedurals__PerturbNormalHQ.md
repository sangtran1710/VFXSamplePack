# PerturbNormalHQ

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/PerturbNormalHQ.PerturbNormalHQ`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Inputs

- `Bump center` [FunctionInput_Scalar]
- `Bump one pixel down` [FunctionInput_Scalar]
- `Bump one pixel right` [FunctionInput_Scalar]
- `WS Normal` [FunctionInput_Vector3]

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
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Bump center (Scalar) | Bump center |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 12 | expression | `MaterialExpressionIf` | If |  |
| 13 | expression | `MaterialExpressionConstant` | 0 |  |
| 14 | expression | `MaterialExpressionConstant` | 1 |  |
| 15 | expression | `MaterialExpressionConstant` | -1 |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionAbs` | Abs |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Bump one pixel down (Scalar) | Bump one pixel down |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Bump one pixel right (Scalar) | Bump one pixel right |
| 23 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 24 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input WS Normal (Vector3) | WS Normal |

## Exact Input Wiring

### [0] Output WS Normal

- `A` <= [20] Normalize output 0

### [2] DDX

- `Value` <= [1] Absolute World Position output 0

### [3] DDY

- `Value` <= [1] Absolute World Position output 0

### [5] Cross

- `A` <= [3] DDY output 0
- `B` <= [25] Input WS Normal (Vector3) output 0

### [6] Cross

- `A` <= [25] Input WS Normal (Vector3) output 0
- `B` <= [2] DDX output 0

### [8] Multiply

- `A` <= [23] Subtract output 0
- `B` <= [5] Cross output 0

### [9] Multiply

- `A` <= [24] Subtract output 0
- `B` <= [6] Cross output 0

### [10] Add

- `A` <= [8] Multiply output 0
- `B` <= [9] Multiply output 0

### [11] Dot

- `A` <= [5] Cross output 0
- `B` <= [2] DDX output 0

### [12] If

- `A` <= [11] Dot output 0
- `B` <= [13] 0 output 0
- `AGreaterThanB` <= [14] 1 output 0
- `AEqualsB` <= [13] 0 output 0
- `ALessThanB` <= [15] -1 output 0

### [16] Multiply

- `A` <= [10] Add output 0
- `B` <= [12] If output 0

### [17] Abs

- `Input` <= [11] Dot output 0

### [18] Multiply

- `A` <= [17] Abs output 0
- `B` <= [25] Input WS Normal (Vector3) output 0

### [19] Subtract

- `A` <= [18] Multiply output 0
- `B` <= [16] Multiply output 0

### [20] Normalize

- `VectorInput` <= [19] Subtract output 0

### [23] Subtract

- `A` <= [22] Input Bump one pixel right (Scalar) output 0
- `B` <= [7] Input Bump center (Scalar) output 0

### [24] Subtract

- `A` <= [21] Input Bump one pixel down (Scalar) output 0
- `B` <= [7] Input Bump center (Scalar) output 0

### [25] Input WS Normal (Vector3)

- `Preview` <= [4] VertexNormalWS output 0

## Connection List

- [20] Normalize output 0 -> Output WS Normal.A
- [1] Absolute World Position output 0 -> DDX.Value
- [1] Absolute World Position output 0 -> DDY.Value
- [3] DDY output 0 -> Cross.A
- [25] Input WS Normal (Vector3) output 0 -> Cross.B
- [25] Input WS Normal (Vector3) output 0 -> Cross.A
- [2] DDX output 0 -> Cross.B
- [23] Subtract output 0 -> Multiply.A
- [5] Cross output 0 -> Multiply.B
- [24] Subtract output 0 -> Multiply.A
- [6] Cross output 0 -> Multiply.B
- [8] Multiply output 0 -> Add.A
- [9] Multiply output 0 -> Add.B
- [5] Cross output 0 -> Dot.A
- [2] DDX output 0 -> Dot.B
- [11] Dot output 0 -> If.A
- [13] 0 output 0 -> If.B
- [14] 1 output 0 -> If.AGreaterThanB
- [13] 0 output 0 -> If.AEqualsB
- [15] -1 output 0 -> If.ALessThanB
- [10] Add output 0 -> Multiply.A
- [12] If output 0 -> Multiply.B
- [11] Dot output 0 -> Abs.Input
- [17] Abs output 0 -> Multiply.A
- [25] Input WS Normal (Vector3) output 0 -> Multiply.B
- [18] Multiply output 0 -> Subtract.A
- [16] Multiply output 0 -> Subtract.B
- [19] Subtract output 0 -> Normalize.VectorInput
- [22] Input Bump one pixel right (Scalar) output 0 -> Subtract.A
- [7] Input Bump center (Scalar) output 0 -> Subtract.B
- [21] Input Bump one pixel down (Scalar) output 0 -> Subtract.A
- [7] Input Bump center (Scalar) output 0 -> Subtract.B
- [4] VertexNormalWS output 0 -> Input WS Normal (Vector3).Preview

## Output Trace Roots

- `WS Normal` is driven by [20] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
