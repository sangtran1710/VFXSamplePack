# CustomReflectionVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/CustomReflectionVector.CustomReflectionVector`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Input a normal to generate a reflection vector independant of the default reflection vector and the normals input on the base shader.

## Inputs

- `Normal` [FunctionInput_Vector3] - Input a normalized vector or a normal map.
- `CameraVector` [FunctionInput_Vector3] - Input a normalized vector or a normal map.

## Outputs

- `Reflection Vector` - World space reflection vector

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Reflection Vector | Reflection Vector |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 6 | expression | `MaterialExpressionConstant` | 2 |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 8 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 9 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 10 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input CameraVector (Vector3) | CameraVector |

## Exact Input Wiring

### [0] Output Reflection Vector

- `A` <= [4] Subtract output 0

### [1] Dot

- `A` <= [11] Input CameraVector (Vector3) output 0
- `B` <= [5] Normalize output 0

### [2] Multiply

- `A` <= [1] Dot output 0
- `B` <= [5] Normalize output 0

### [3] Multiply

- `A` <= [2] Multiply output 0
- `B` <= [6] 2 output 0

### [4] Subtract

- `A` <= [3] Multiply output 0
- `B` <= [11] Input CameraVector (Vector3) output 0

### [5] Normalize

- `VectorInput` <= [7] Input Normal (Vector3) output 0

### [7] Input Normal (Vector3)

- `Preview` <= [8] VertexNormalWS output 0

### [11] Input CameraVector (Vector3)

- `Preview` <= [9] Camera Vector output 0

## Connection List

- [4] Subtract output 0 -> Output Reflection Vector.A
- [11] Input CameraVector (Vector3) output 0 -> Dot.A
- [5] Normalize output 0 -> Dot.B
- [1] Dot output 0 -> Multiply.A
- [5] Normalize output 0 -> Multiply.B
- [2] Multiply output 0 -> Multiply.A
- [6] 2 output 0 -> Multiply.B
- [3] Multiply output 0 -> Subtract.A
- [11] Input CameraVector (Vector3) output 0 -> Subtract.B
- [7] Input Normal (Vector3) output 0 -> Normalize.VectorInput
- [8] VertexNormalWS output 0 -> Input Normal (Vector3).Preview
- [9] Camera Vector output 0 -> Input CameraVector (Vector3).Preview

## Output Trace Roots

- `Reflection Vector` is driven by [4] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
