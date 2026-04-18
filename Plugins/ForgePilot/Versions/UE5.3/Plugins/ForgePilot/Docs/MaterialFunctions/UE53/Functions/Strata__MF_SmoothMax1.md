# MF_SmoothMax1

- Path: `/Engine/Functions/Strata/MF_SmoothMax1.MF_SmoothMax1`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `A` [FunctionInput_Scalar]
- `B` [FunctionInput_Scalar]
- `distance` [FunctionInput_Scalar] - blend distance of the two inputs (called k)

## Outputs

- `SmoothMax`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output SmoothMax | SmoothMax |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input A (Scalar) | A |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input B (Scalar) | B |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input distance (Scalar) | distance |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | MF_SmoothMin1 |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output SmoothMax

- `A` <= [7] Multiply(,-1) output 0

### [5] Multiply(,-1)

- `A` <= [1] Input A (Scalar) output 0

### [6] Multiply(,-1)

- `A` <= [2] Input B (Scalar) output 0

### [7] Multiply(,-1)

- `A` <= [4] MF_SmoothMin1 output 0

## Connection List

- [7] Multiply(,-1) output 0 -> Output SmoothMax.A
- [1] Input A (Scalar) output 0 -> Multiply(,-1).A
- [2] Input B (Scalar) output 0 -> Multiply(,-1).A
- [4] MF_SmoothMin1 output 0 -> Multiply(,-1).A

## Output Trace Roots

- `SmoothMax` is driven by [7] Multiply(,-1)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
