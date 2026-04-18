# Refract

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/Refract.Refract`
- Category: Engine_MaterialFunctions01/Vectors
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Calculates vector based Refraction using the hlsl intrinsic function.

## Inputs

- `Ray Direction` [FunctionInput_Vector3] - The direction of the vector to refract.
- `Surface Normal` [FunctionInput_Vector3] - The Normal of the interface between the two media.
- `Refractive Index Origin` [FunctionInput_Scalar] - The refractive index of the media where the ray originates. The default is 1.0 for Air.
- `Refractive Index Target` [FunctionInput_Scalar] - The refractive index of the media the ray is intersecting. Defaults to 1.333 for Water.

## Outputs

- `Vector`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Vector | Vector |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Ray Direction (Vector3) | Ray Direction |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Surface Normal (Vector3) | Surface Normal |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Refractive Index Origin (Scalar) | Refractive Index Origin |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Refractive Index Target (Scalar) | Refractive Index Target |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output Vector

- `A` <= [1] Custom output 0

### [6] Divide

- `A` <= [4] Input Refractive Index Origin (Scalar) output 0
- `B` <= [5] Input Refractive Index Target (Scalar) output 0

## Connection List

- [1] Custom output 0 -> Output Vector.A
- [4] Input Refractive Index Origin (Scalar) output 0 -> Divide.A
- [5] Input Refractive Index Target (Scalar) output 0 -> Divide.B

## Output Trace Roots

- `Vector` is driven by [1] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


