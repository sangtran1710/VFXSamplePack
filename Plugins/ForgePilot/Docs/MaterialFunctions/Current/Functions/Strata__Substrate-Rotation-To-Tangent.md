# Substrate-Rotation-To-Tangent

- Path: `/Engine/Functions/Strata/Substrate-Rotation-To-Tangent.Substrate-Rotation-To-Tangent`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Convert a rotation angle into a tangent vector

## Inputs

- `Rotation` [FunctionInput_Scalar] - Normalized rotation angle (0..1), defining the rotation aroung the surface normal

## Outputs

- `Tangent` - Rotated tangent vector

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Tangent | Tangent |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Rotation (Scalar) | Rotation |
| 2 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 3 | custom | `MaterialExpressionCustom` | Custom |  |
| 4 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 5 | expression | `MaterialExpressionVertexTangentWS` | VertexTangentWS |  |

## Exact Input Wiring

### [0] Output Tangent

- `A` <= [3] Custom output 0

### [2] Saturate

- `Input` <= [1] Input Rotation (Scalar) output 0

## Connection List

- [3] Custom output 0 -> Output Tangent.A
- [1] Input Rotation (Scalar) output 0 -> Saturate.Input

## Output Trace Roots

- `Tangent` is driven by [3] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


