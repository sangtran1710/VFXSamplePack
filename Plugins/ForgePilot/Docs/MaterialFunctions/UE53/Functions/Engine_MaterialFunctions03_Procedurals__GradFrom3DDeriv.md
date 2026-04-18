# GradFrom3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/GradFrom3DDeriv.GradFrom3DDeriv`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Compute 3D gradient vector from result of Prepare3DDeriv/Compute3DDeriv

## Inputs

- `dx` [FunctionInput_Vector3] - Derivative in X direction
- `dy` [FunctionInput_Vector3] - Derivative in Y direction
- `dz` [FunctionInput_Vector3] - Derivative in Z direction

## Outputs

- `Gradient` - Computed 3D gradient vector direction

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Gradient | Gradient |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input dx (Vector3) | dx |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input dy (Vector3) | dy |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input dz (Vector3) | dz |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | AppendMany |  |

## Exact Input Wiring

### [0] Output Gradient

- `A` <= [7] AppendMany output 1

### [2] Mask ( R )

- `Input` <= [1] Input dx (Vector3) output 0

### [4] Mask ( R )

- `Input` <= [3] Input dy (Vector3) output 0

### [6] Mask ( R )

- `Input` <= [5] Input dz (Vector3) output 0

## Connection List

- [7] AppendMany output 1 -> Output Gradient.A
- [1] Input dx (Vector3) output 0 -> Mask ( R ).Input
- [3] Input dy (Vector3) output 0 -> Mask ( R ).Input
- [5] Input dz (Vector3) output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Gradient` is driven by [7] AppendMany

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
