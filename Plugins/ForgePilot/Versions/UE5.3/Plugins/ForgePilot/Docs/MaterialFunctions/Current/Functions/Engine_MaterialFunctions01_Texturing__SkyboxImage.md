# SkyboxImage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SkyboxImage.SkyboxImage`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Projects an image onto an infinite sphere, useful for putting images in the skybox

## Inputs

- `Rotation 0-1` [FunctionInput_Scalar]
- `ImageVector` [FunctionInput_Vector3]
- `TextureObject` [FunctionInput_Texture2D]
- `Scale` [FunctionInput_Vector2]

## Outputs

- `RGB` - UnMasked RGB image, may contain stretching, use output mask 
- `Mask` - Mask for the area of correct projection
- `Alpha` - UnMasked image alpha, may contain stretching, use output mask 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 3 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 4 | expression | `MaterialExpressionRotator` | Rotator |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Rotation 0-1 (Scalar) | Rotation 0-1 |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,6.28) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | ScaleUVsByCenter |  |
| 10 | expression | `MaterialExpressionCeil` | Ceil |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | expression | `MaterialExpressionMin` | Min |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 14 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 15 | expression | `MaterialExpressionCeil` | Ceil |  |
| 16 | expression | `MaterialExpressionClamp` | Clamp |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 18 | expression | `MaterialExpressionMin` | Min |  |
| 19 | expression | `MaterialExpressionMin` | Min |  |
| 20 | expression | `MaterialExpressionCeil` | Ceil |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionConstant2Vector` | 1,-1 |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output Mask | Mask |
| 25 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input ImageVector (Vector3) | ImageVector |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 28 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input Scale (Vector2) | Scale |
| 30 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 31 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 32 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 33 | function_call | `MaterialExpressionMaterialFunctionCall` | RotateVector |  |
| 34 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 35 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output RGB

- `A` <= [1] Texture Sample output 0

### [1] Texture Sample

- `Coordinates` <= [9] ScaleUVsByCenter output 0
- `TextureObject` <= [27] Input TextureObject (Texture2D) output 0

### [2] Mask ( R G )

- `Input` <= [33] RotateVector output 0

### [3] Add(,0.5)

- `A` <= [2] Mask ( R G ) output 0

### [4] Rotator

- `Coordinate` <= [3] Add(,0.5) output 0
- `Time` <= [6] Multiply(,6.28) output 0

### [6] Multiply(,6.28)

- `A` <= [5] Input Rotation 0-1 (Scalar) output 0

### [7] Multiply(,-1)

- `A` <= [32] Mask ( B ) output 0

### [8] Clamp

- `Input` <= [7] Multiply(,-1) output 0

### [10] Ceil

- `Input` <= [9] ScaleUVsByCenter output 0

### [11] Clamp

- `Input` <= [10] Ceil output 0

### [12] Min

- `A` <= [13] BreakOutFloat2Components output 0
- `B` <= [13] BreakOutFloat2Components output 1

### [14] 1-x

- `Input` <= [9] ScaleUVsByCenter output 0

### [15] Ceil

- `Input` <= [14] 1-x output 0

### [16] Clamp

- `Input` <= [15] Ceil output 0

### [18] Min

- `A` <= [17] BreakOutFloat2Components output 0
- `B` <= [17] BreakOutFloat2Components output 1

### [19] Min

- `A` <= [12] Min output 0
- `B` <= [18] Min output 0

### [20] Ceil

- `Input` <= [8] Clamp output 0

### [21] Multiply

- `A` <= [19] Min output 0
- `B` <= [20] Ceil output 0

### [23] Multiply

- `A` <= [35] Append output 0
- `B` <= [22] 1,-1 output 0

### [24] Output Mask

- `A` <= [21] Multiply output 0

### [25] Output Alpha

- `A` <= [1] Texture Sample output 4

### [27] Input TextureObject (Texture2D)

- `Preview` <= [28] Texture Object output 0

### [31] Normalize

- `VectorInput` <= [26] Input ImageVector (Vector3) output 0

### [32] Mask ( B )

- `Input` <= [33] RotateVector output 0

### [35] Append

- `A` <= [34] BreakOutFloat2Components output 1
- `B` <= [34] BreakOutFloat2Components output 0

## Connection List

- [1] Texture Sample output 0 -> Output RGB.A
- [9] ScaleUVsByCenter output 0 -> Texture Sample.Coordinates
- [27] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [33] RotateVector output 0 -> Mask ( R G ).Input
- [2] Mask ( R G ) output 0 -> Add(,0.5).A
- [3] Add(,0.5) output 0 -> Rotator.Coordinate
- [6] Multiply(,6.28) output 0 -> Rotator.Time
- [5] Input Rotation 0-1 (Scalar) output 0 -> Multiply(,6.28).A
- [32] Mask ( B ) output 0 -> Multiply(,-1).A
- [7] Multiply(,-1) output 0 -> Clamp.Input
- [9] ScaleUVsByCenter output 0 -> Ceil.Input
- [10] Ceil output 0 -> Clamp.Input
- [13] BreakOutFloat2Components output 0 -> Min.A
- [13] BreakOutFloat2Components output 1 -> Min.B
- [9] ScaleUVsByCenter output 0 -> 1-x.Input
- [14] 1-x output 0 -> Ceil.Input
- [15] Ceil output 0 -> Clamp.Input
- [17] BreakOutFloat2Components output 0 -> Min.A
- [17] BreakOutFloat2Components output 1 -> Min.B
- [12] Min output 0 -> Min.A
- [18] Min output 0 -> Min.B
- [8] Clamp output 0 -> Ceil.Input
- [19] Min output 0 -> Multiply.A
- [20] Ceil output 0 -> Multiply.B
- [35] Append output 0 -> Multiply.A
- [22] 1,-1 output 0 -> Multiply.B
- [21] Multiply output 0 -> Output Mask.A
- [1] Texture Sample output 4 -> Output Alpha.A
- [28] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [26] Input ImageVector (Vector3) output 0 -> Normalize.VectorInput
- [33] RotateVector output 0 -> Mask ( B ).Input
- [34] BreakOutFloat2Components output 1 -> Append.A
- [34] BreakOutFloat2Components output 0 -> Append.B

## Output Trace Roots

- `RGB` is driven by [1] Texture Sample
- `Mask` is driven by [21] Multiply
- `Alpha` is driven by [1] Texture Sample

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


