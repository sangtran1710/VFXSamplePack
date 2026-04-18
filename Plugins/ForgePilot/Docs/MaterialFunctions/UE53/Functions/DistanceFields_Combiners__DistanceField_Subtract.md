# DistanceField_Subtract

- Path: `/Engine/Functions/DistanceFields/Combiners/DistanceField_Subtract.DistanceField_Subtract`
- Category: DistanceFields/Combiners
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Subtracts Distance Field 2 from Distance Field 1

## Inputs

- `Distance Field 1` [FunctionInput_Scalar] - The fist shape
- `Distance Field 2` [FunctionInput_Scalar] - The second shape. This is the shape that will be subtracted.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Distance Field 1 (Scalar) | Distance Field 1 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Distance Field 2 (Scalar) | Distance Field 2 |
| 3 | expression | `MaterialExpressionMax` | Max |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Max output 0

### [3] Max

- `A` <= [1] Input Distance Field 1 (Scalar) output 0
- `B` <= [4] Multiply(,-1) output 0

### [4] Multiply(,-1)

- `A` <= [2] Input Distance Field 2 (Scalar) output 0

## Connection List

- [3] Max output 0 -> Output Result.A
- [1] Input Distance Field 1 (Scalar) output 0 -> Max.A
- [4] Multiply(,-1) output 0 -> Max.B
- [2] Input Distance Field 2 (Scalar) output 0 -> Multiply(,-1).A

## Output Trace Roots

- `Result` is driven by [3] Max

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
