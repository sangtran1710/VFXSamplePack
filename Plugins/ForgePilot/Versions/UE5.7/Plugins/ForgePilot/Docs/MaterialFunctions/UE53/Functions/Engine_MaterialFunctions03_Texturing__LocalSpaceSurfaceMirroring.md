# LocalSpaceSurfaceMirroring

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/LocalSpaceSurfaceMirroring.LocalSpaceSurfaceMirroring`
- Category: Engine_MaterialFunctions03/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Generates mask results based on the local orientation of surfaces.  Useful for mirroring texture information based on the object's topology.

## Inputs

- `In` [FunctionInput_Scalar] - Biases the mask results.
- `Use NormalMap` [FunctionInput_StaticBool] - Use texture-based normals instead of surface normals.
- `Normal Map` [FunctionInput_Vector3] - Normal input for surface calculation

## Outputs

- `Local Z`
- `Local Y`
- `Local X`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Local Z | Local Z |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | expression | `MaterialExpressionCeil` | Ceil |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 6 | expression | `MaterialExpressionConstant` | 0 |  |
| 7 | expression | `MaterialExpressionTransform` | Tangent Space to Local Space TransformVector |  |
| 8 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Use NormalMap (StaticBool) | Use NormalMap |
| 10 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 11 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Normal Map (Vector3) | Normal Map |
| 13 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 14 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Local Y | Local Y |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Local X | Local X |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |

## Exact Input Wiring

### [0] Output Local Z

- `A` <= [1] Mask ( B ) output 0

### [1] Mask ( B )

- `Input` <= [2] 1-x output 0

### [3] Ceil

- `Input` <= [4] Add output 0

### [4] Add

- `A` <= [7] Tangent Space to Local Space TransformVector output 0
- `B` <= [5] Input In (Scalar) output 0

### [5] Input In (Scalar)

- `Preview` <= [6] 0 output 0

### [7] Tangent Space to Local Space TransformVector

- `Input` <= [8] Switch output 0

### [8] Switch

- `A` <= [12] Input Normal Map (Vector3) output 0
- `B` <= [11] 0,0,1 output 0
- `Value` <= [9] Input Use NormalMap (StaticBool) output 0

### [9] Input Use NormalMap (StaticBool)

- `Preview` <= [10] Static Bool (False) output 0

### [12] Input Normal Map (Vector3)

- `Preview` <= [13] Texture Sample output 0

### [13] Texture Sample

- `TextureObject` <= [14] Texture Object output 0

### [15] Output Local Y

- `A` <= [16] Mask ( G ) output 0

### [16] Mask ( G )

- `Input` <= [2] 1-x output 0

### [17] Output Local X

- `A` <= [18] Mask ( R ) output 0

### [18] Mask ( R )

- `Input` <= [2] 1-x output 0

## Connection List

- [1] Mask ( B ) output 0 -> Output Local Z.A
- [2] 1-x output 0 -> Mask ( B ).Input
- [4] Add output 0 -> Ceil.Input
- [7] Tangent Space to Local Space TransformVector output 0 -> Add.A
- [5] Input In (Scalar) output 0 -> Add.B
- [6] 0 output 0 -> Input In (Scalar).Preview
- [8] Switch output 0 -> Tangent Space to Local Space TransformVector.Input
- [12] Input Normal Map (Vector3) output 0 -> Switch.A
- [11] 0,0,1 output 0 -> Switch.B
- [9] Input Use NormalMap (StaticBool) output 0 -> Switch.Value
- [10] Static Bool (False) output 0 -> Input Use NormalMap (StaticBool).Preview
- [13] Texture Sample output 0 -> Input Normal Map (Vector3).Preview
- [14] Texture Object output 0 -> Texture Sample.TextureObject
- [16] Mask ( G ) output 0 -> Output Local Y.A
- [2] 1-x output 0 -> Mask ( G ).Input
- [18] Mask ( R ) output 0 -> Output Local X.A
- [2] 1-x output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Local Z` is driven by [1] Mask ( B )
- `Local Y` is driven by [16] Mask ( G )
- `Local X` is driven by [18] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
