# CameraVectorToLatLongUV

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/CameraVectorToLatLongUV.CameraVectorToLatLongUV`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts camera vector to latlong UVs. Helpful for skybox textures

## Inputs

- `CameraVector` [FunctionInput_Vector3]

## Outputs

- `Uvs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Uvs | Uvs |
| 1 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input CameraVector (Vector3) | CameraVector |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorToRadialValue |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 6 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |

## Exact Input Wiring

### [0] Output Uvs

- `A` <= [7] Append output 0

### [2] Input CameraVector (Vector3)

- `Preview` <= [1] Camera Vector output 0

### [3] Mask ( R G )

- `Input` <= [2] Input CameraVector (Vector3) output 0

### [5] Mask ( B )

- `Input` <= [2] Input CameraVector (Vector3) output 0

### [6] ConstantBiasScale

- `Input` <= [5] Mask ( B ) output 0

### [7] Append

- `A` <= [8] Mask ( R ) output 0
- `B` <= [6] ConstantBiasScale output 0

### [8] Mask ( R )

- `Input` <= [4] VectorToRadialValue output 0

## Connection List

- [7] Append output 0 -> Output Uvs.A
- [1] Camera Vector output 0 -> Input CameraVector (Vector3).Preview
- [2] Input CameraVector (Vector3) output 0 -> Mask ( R G ).Input
- [2] Input CameraVector (Vector3) output 0 -> Mask ( B ).Input
- [5] Mask ( B ) output 0 -> ConstantBiasScale.Input
- [8] Mask ( R ) output 0 -> Append.A
- [6] ConstantBiasScale output 0 -> Append.B
- [4] VectorToRadialValue output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Uvs` is driven by [7] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


