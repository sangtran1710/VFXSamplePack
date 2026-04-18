# TriplanarCameraVector

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TriplanarCameraVector.TriplanarCameraVector`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Applies a texture to the background, similar to a cubemap but uses the same 2d texture for all sides

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Tiling` [FunctionInput_Vector3]
- `AxisFadeContrast` [FunctionInput_Scalar] - Used to fade the seams between the 6 textures
- `Offset` [FunctionInput_Vector3]
- `CameraVector` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | SCurve |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 7 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 8 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 10 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 11 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | SCurve |  |
| 13 | expression | `MaterialExpressionAbs` | Abs |  |
| 14 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 15 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 19 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Tiling (Vector3) | Tiling |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input AxisFadeContrast (Scalar) | AxisFadeContrast |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Vector3) | Offset |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input CameraVector (Vector3) | CameraVector |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [2] Lerp output 0
- `B` <= [3] Texture Sample output 0
- `Alpha` <= [4] SCurve output 0

### [2] Lerp

- `A` <= [10] Texture Sample output 0
- `B` <= [11] Texture Sample output 0
- `Alpha` <= [12] SCurve output 0

### [3] Texture Sample

- `Coordinates` <= [9] Mask ( R G ) output 0
- `TextureObject` <= [18] Input Texture (Texture2D) output 0

### [5] Abs

- `Input` <= [6] Dot output 0

### [6] Dot

- `A` <= [25] Input CameraVector (Vector3) output 0
- `B` <= [8] 0,0,1 output 0

### [9] Mask ( R G )

- `Input` <= [23] Add output 0

### [10] Texture Sample

- `Coordinates` <= [17] Mask ( R B ) output 0
- `TextureObject` <= [18] Input Texture (Texture2D) output 0

### [11] Texture Sample

- `Coordinates` <= [16] Mask ( G B ) output 0
- `TextureObject` <= [18] Input Texture (Texture2D) output 0

### [13] Abs

- `Input` <= [14] Dot output 0

### [14] Dot

- `A` <= [25] Input CameraVector (Vector3) output 0
- `B` <= [15] 1,0,0 output 0

### [16] Mask ( G B )

- `Input` <= [23] Add output 0

### [17] Mask ( R B )

- `Input` <= [23] Add output 0

### [18] Input Texture (Texture2D)

- `Preview` <= [19] Texture Object output 0

### [20] Multiply

- `A` <= [25] Input CameraVector (Vector3) output 0
- `B` <= [21] Input Tiling (Vector3) output 0

### [23] Add

- `A` <= [24] Input Offset (Vector3) output 0
- `B` <= [20] Multiply output 0

### [25] Input CameraVector (Vector3)

- `Preview` <= [7] Camera Vector output 0

## Connection List

- [1] Lerp output 0 -> Output Result.A
- [2] Lerp output 0 -> Lerp.A
- [3] Texture Sample output 0 -> Lerp.B
- [4] SCurve output 0 -> Lerp.Alpha
- [10] Texture Sample output 0 -> Lerp.A
- [11] Texture Sample output 0 -> Lerp.B
- [12] SCurve output 0 -> Lerp.Alpha
- [9] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [18] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [6] Dot output 0 -> Abs.Input
- [25] Input CameraVector (Vector3) output 0 -> Dot.A
- [8] 0,0,1 output 0 -> Dot.B
- [23] Add output 0 -> Mask ( R G ).Input
- [17] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [18] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [16] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [18] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [14] Dot output 0 -> Abs.Input
- [25] Input CameraVector (Vector3) output 0 -> Dot.A
- [15] 1,0,0 output 0 -> Dot.B
- [23] Add output 0 -> Mask ( G B ).Input
- [23] Add output 0 -> Mask ( R B ).Input
- [19] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [25] Input CameraVector (Vector3) output 0 -> Multiply.A
- [21] Input Tiling (Vector3) output 0 -> Multiply.B
- [24] Input Offset (Vector3) output 0 -> Add.A
- [20] Multiply output 0 -> Add.B
- [7] Camera Vector output 0 -> Input CameraVector (Vector3).Preview

## Output Trace Roots

- `Result` is driven by [1] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


