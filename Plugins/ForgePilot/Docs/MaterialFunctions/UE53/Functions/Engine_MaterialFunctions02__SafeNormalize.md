# SafeNormalize

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SafeNormalize.SafeNormalize`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

If the input vectors length == 0 then return 0 else return the normalized vector by default. This avoids returning NAN or NULL. Optionally, one can pass in a default vector that should be used when the vector 

## Inputs

- `Vector` [FunctionInput_Vector3]
- `Default` [FunctionInput_Vector3] - Vector to be used when the normalized vector reaches 0 in length.

## Outputs

- `Result`
- `Length==0` - Returns 1 if the vector's length was 0. Otherwise, this output returns 0.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | expression | `MaterialExpressionConstant` | 1e-06 |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Default (Vector3) | Default |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output Length==0 | Length==0 |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] Mask ( R G B ) output 0

### [1] Dot

- `A` <= [5] Input Vector (Vector3) output 0
- `B` <= [5] Input Vector (Vector3) output 0

### [2] If

- `A` <= [1] Dot output 0
- `B` <= [3] 1e-06 output 0
- `AGreaterThanB` <= [10] Append output 0
- `AEqualsB` <= [12] Append output 0
- `ALessThanB` <= [12] Append output 0

### [4] Normalize

- `VectorInput` <= [5] Input Vector (Vector3) output 0

### [7] Output Length==0

- `A` <= [9] Mask ( A ) output 0

### [8] Mask ( R G B )

- `Input` <= [2] If output 0

### [9] Mask ( A )

- `Input` <= [2] If output 0

### [10] Append

- `A` <= [4] Normalize output 0
- `B` <= [11] 0 output 0

### [12] Append

- `A` <= [6] Input Default (Vector3) output 0
- `B` <= [13] 1 output 0

## Connection List

- [8] Mask ( R G B ) output 0 -> Output Result.A
- [5] Input Vector (Vector3) output 0 -> Dot.A
- [5] Input Vector (Vector3) output 0 -> Dot.B
- [1] Dot output 0 -> If.A
- [3] 1e-06 output 0 -> If.B
- [10] Append output 0 -> If.AGreaterThanB
- [12] Append output 0 -> If.AEqualsB
- [12] Append output 0 -> If.ALessThanB
- [5] Input Vector (Vector3) output 0 -> Normalize.VectorInput
- [9] Mask ( A ) output 0 -> Output Length==0.A
- [2] If output 0 -> Mask ( R G B ).Input
- [2] If output 0 -> Mask ( A ).Input
- [4] Normalize output 0 -> Append.A
- [11] 0 output 0 -> Append.B
- [6] Input Default (Vector3) output 0 -> Append.A
- [13] 1 output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [8] Mask ( R G B )
- `Length==0` is driven by [9] Mask ( A )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
