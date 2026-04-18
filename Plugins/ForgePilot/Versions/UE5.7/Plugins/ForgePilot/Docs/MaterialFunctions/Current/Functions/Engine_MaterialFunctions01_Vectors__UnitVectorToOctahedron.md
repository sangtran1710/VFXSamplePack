# UnitVectorToOctahedron

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/UnitVectorToOctahedron.UnitVectorToOctahedron`
- Category: Engine_MaterialFunctions01/Vectors
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Packs a v3 vector to a v2 Octahedron.

## Inputs

- `Vector` [FunctionInput_Vector3] - Make sure the input vector is normalized first.

## Outputs

- `Octahedron`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Octahedron | Octahedron |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 2 | custom | `MaterialExpressionCustom` | Vector to Octahedron |  |

## Exact Input Wiring

### [0] Output Octahedron

- `A` <= [2] Vector to Octahedron output 0

## Connection List

- [2] Vector to Octahedron output 0 -> Output Octahedron.A

## Output Trace Roots

- `Octahedron` is driven by [2] Vector to Octahedron

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


