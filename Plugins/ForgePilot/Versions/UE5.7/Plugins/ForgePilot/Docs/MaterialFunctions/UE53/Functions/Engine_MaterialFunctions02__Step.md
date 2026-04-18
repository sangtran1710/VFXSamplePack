# Step

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Step.Step`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Low` [FunctionInput_Scalar] - Low threshold value
- `High` [FunctionInput_Scalar] - High threshold point for comparision

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Low (Scalar) | Low |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input High (Scalar) | High |
| 4 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 5 | expression | `MaterialExpressionConstant` | 0 |  |
| 6 | expression | `MaterialExpressionConstant` | 1 |  |
| 7 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] If output 0

### [1] If

- `A` <= [2] Input Low (Scalar) output 0
- `B` <= [3] Input High (Scalar) output 0
- `AGreaterThanB` <= [5] 0 output 0
- `ALessThanB` <= [6] 1 output 0

### [2] Input Low (Scalar)

- `Preview` <= [4] 0.5 output 0

### [3] Input High (Scalar)

- `Preview` <= [8] Mask ( G ) output 0

### [8] Mask ( G )

- `Input` <= [7] TexCoord[0] output 0

## Connection List

- [1] If output 0 -> Output Result.A
- [2] Input Low (Scalar) output 0 -> If.A
- [3] Input High (Scalar) output 0 -> If.B
- [5] 0 output 0 -> If.AGreaterThanB
- [6] 1 output 0 -> If.ALessThanB
- [4] 0.5 output 0 -> Input Low (Scalar).Preview
- [8] Mask ( G ) output 0 -> Input High (Scalar).Preview
- [7] TexCoord[0] output 0 -> Mask ( G ).Input

## Output Trace Roots

- `Result` is driven by [1] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
