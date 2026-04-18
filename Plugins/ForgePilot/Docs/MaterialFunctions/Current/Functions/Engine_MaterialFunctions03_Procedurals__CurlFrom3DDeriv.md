# CurlFrom3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/CurlFrom3DDeriv.CurlFrom3DDeriv`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Compute curl of at 3D vector field from result of Prepare3DDeriv/Compute3DDeriv

## Inputs

- `dx` [FunctionInput_Vector3] - Derivative in X direction
- `dy` [FunctionInput_Vector3] - Derivative in Y direction
- `dz` [FunctionInput_Vector3] - Derivative in Z direction

## Outputs

- `Curl` - Curl of vector field from 3D derivatives

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Curl | Curl |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input dx (Vector3) | dx |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input dy (Vector3) | dy |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input dz (Vector3) | dz |

## Exact Input Wiring

### [0] Output Curl

- `A` <= [1] MakeFloat3 output 0

### [2] Subtract

- `A` <= [7] BreakOutFloat3Components output 1
- `B` <= [6] BreakOutFloat3Components output 0

### [3] Subtract

- `A` <= [5] BreakOutFloat3Components output 0
- `B` <= [7] BreakOutFloat3Components output 2

### [4] Subtract

- `A` <= [6] BreakOutFloat3Components output 2
- `B` <= [5] BreakOutFloat3Components output 1

## Connection List

- [1] MakeFloat3 output 0 -> Output Curl.A
- [7] BreakOutFloat3Components output 1 -> Subtract.A
- [6] BreakOutFloat3Components output 0 -> Subtract.B
- [5] BreakOutFloat3Components output 0 -> Subtract.A
- [7] BreakOutFloat3Components output 2 -> Subtract.B
- [6] BreakOutFloat3Components output 2 -> Subtract.A
- [5] BreakOutFloat3Components output 1 -> Subtract.B

## Output Trace Roots

- `Curl` is driven by [1] MakeFloat3

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


