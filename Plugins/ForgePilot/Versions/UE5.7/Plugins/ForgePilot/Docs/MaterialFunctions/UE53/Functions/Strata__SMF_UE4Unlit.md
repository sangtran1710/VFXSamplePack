# SMF_UE4Unlit

- Path: `/Engine/Functions/Strata/SMF_UE4Unlit.SMF_UE4Unlit`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Unlit material with Opacity input.

## Inputs

- `Color` [FunctionInput_Vector3] - Emissive color added to the scene.
- `Opacity` [FunctionInput_Scalar] - Represent the coverage of the material. 0 means 100% see through, 1 means fully opaque.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 3 | expression | `MaterialExpressionStrataUnlitBSDF` | Substrate Unlit BSDF |  |
| 4 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Substrate Unlit BSDF output 0

### [3] Substrate Unlit BSDF

- `EmissiveColor` <= [5] Multiply output 0
- `TransmittanceColor` <= [6] 1-x output 0

### [4] Saturate

- `Input` <= [2] Input Opacity (Scalar) output 0

### [5] Multiply

- `A` <= [1] Input Color (Vector3) output 0
- `B` <= [4] Saturate output 0

### [6] 1-x

- `Input` <= [4] Saturate output 0

## Connection List

- [3] Substrate Unlit BSDF output 0 -> Output Result.A
- [5] Multiply output 0 -> Substrate Unlit BSDF.EmissiveColor
- [6] 1-x output 0 -> Substrate Unlit BSDF.TransmittanceColor
- [2] Input Opacity (Scalar) output 0 -> Saturate.Input
- [1] Input Color (Vector3) output 0 -> Multiply.A
- [4] Saturate output 0 -> Multiply.B
- [4] Saturate output 0 -> 1-x.Input

## Output Trace Roots

- `Result` is driven by [3] Substrate Unlit BSDF

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
