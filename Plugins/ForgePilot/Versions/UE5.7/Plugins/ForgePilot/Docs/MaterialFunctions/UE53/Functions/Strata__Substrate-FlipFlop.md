# Substrate-FlipFlop

- Path: `/Engine/Functions/Strata/Substrate-FlipFlop.Substrate-FlipFlop`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `F0` [FunctionInput_Vector3] - Specular color when the surface is facing the camera.
- `F90` [FunctionInput_Vector3] - Specular color when the surface is tangent to the camera.
- `Falloff` [FunctionInput_Scalar] - Describes the rate at which both facing and tangent colors are lerped. The curve is a power with exponent in [1/10, 10] mapped from the Falloff in [0,1]. Default to 0.722, leading to a power of 5 similar to Schlick
- `WorldNormal` [FunctionInput_Vector3] - The surface world normal.

## Outputs

- `Result` - The resulting specular color.
- `LerpFactor` - The linear interpolation factor used to interpolate facing and tangent colors.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input F0 (Vector3) | F0 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input F90 (Vector3) | F90 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Falloff (Scalar) | Falloff |
| 4 | expression | `MaterialExpressionConstant` | 0.7222 |  |
| 5 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 6 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 7 | custom | `MaterialExpressionCustom` | Custom |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output LerpFactor | LerpFactor |
| 9 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input WorldNormal (Vector3) | WorldNormal |
| 13 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 14 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 15 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Custom output 0

### [1] Input F0 (Vector3)

- `Preview` <= [5] 1,0,0 output 0

### [2] Input F90 (Vector3)

- `Preview` <= [6] 0,1,0 output 0

### [3] Input Falloff (Scalar)

- `Preview` <= [4] 0.7222 output 0

### [8] Output LerpFactor

- `A` <= [7] Custom output 1

### [10] Dot

- `A` <= [9] Camera Vector output 0
- `B` <= [12] Input WorldNormal (Vector3) output 0

### [11] Saturate

- `Input` <= [10] Dot output 0

### [12] Input WorldNormal (Vector3)

- `Preview` <= [15] Normalize output 0

### [13] Saturate

- `Input` <= [3] Input Falloff (Scalar) output 0

### [15] Normalize

- `VectorInput` <= [14] VertexNormalWS output 0

## Connection List

- [7] Custom output 0 -> Output Result.A
- [5] 1,0,0 output 0 -> Input F0 (Vector3).Preview
- [6] 0,1,0 output 0 -> Input F90 (Vector3).Preview
- [4] 0.7222 output 0 -> Input Falloff (Scalar).Preview
- [7] Custom output 1 -> Output LerpFactor.A
- [9] Camera Vector output 0 -> Dot.A
- [12] Input WorldNormal (Vector3) output 0 -> Dot.B
- [10] Dot output 0 -> Saturate.Input
- [15] Normalize output 0 -> Input WorldNormal (Vector3).Preview
- [3] Input Falloff (Scalar) output 0 -> Saturate.Input
- [14] VertexNormalWS output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Result` is driven by [7] Custom
- `LerpFactor` is driven by [7] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
