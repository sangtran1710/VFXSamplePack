# OctahedronToUnitVector

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/OctahedronToUnitVector.OctahedronToUnitVector`
- Category: Engine_MaterialFunctions01/Vectors
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Unpacks a v2 Octahedron to a v3 vector.

## Inputs

- `Octahedron` [FunctionInput_Vector2] - The V2 Octahedron to unpack to a V3 Vector.

## Outputs

- `Unit Vector`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Unit Vector | Unit Vector |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Octahedron (Vector2) | Octahedron |
| 2 | custom | `MaterialExpressionCustom` | Octahedron to Vector |  |

## Exact Input Wiring

### [0] Output Unit Vector

- `A` <= [2] Octahedron to Vector output 0

## Connection List

- [2] Octahedron to Vector output 0 -> Output Unit Vector.A

## Output Trace Roots

- `Unit Vector` is driven by [2] Octahedron to Vector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
