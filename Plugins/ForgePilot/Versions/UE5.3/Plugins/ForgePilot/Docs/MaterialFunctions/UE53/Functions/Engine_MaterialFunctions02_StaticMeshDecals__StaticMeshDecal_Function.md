# StaticMeshDecal_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions02/StaticMeshDecals/StaticMeshDecal_Function.StaticMeshDecal_Function`
- Category: Engine_MaterialFunctions02/StaticMeshDecals
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Use this function with "1x1x1_Box_Pivot_-XYZ" to draw decal like projections on to surfaces. The material should use shader domains other than the decal domain.

## Inputs

- `Local Projection Angle Mask Vector And Falloff Power (V4)` [FunctionInput_Vector4]
- `XYZ Opacity Falloff Hardness` [FunctionInput_Vector3]
- `Camera Clip Mask Distance` [FunctionInput_Scalar]

## Outputs

- `0-1 RGB UVW`
- `Extent bounds masks`
- `Camera Clip Mask`
- `Projection Angle Mask`
- `Combined Mask`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 0-1 RGB UVW | 0-1 RGB UVW |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldPositionBehindTranslucency |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectScale |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectPivotPoint |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | ComponentBasisVectors |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 10 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 11 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |
| 15 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 16 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 17 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 20 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 23 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output Extent bounds masks | Extent bounds masks |
| 25 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 26 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 27 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 28 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Camera Clip Mask | Camera Clip Mask |
| 30 | expression | `MaterialExpressionSceneTexture` | SceneTexture:WorldNormal |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 32 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 33 | expression | `MaterialExpressionClamp` | Clamp |  |
| 34 | expression | `MaterialExpressionPower` | Power |  |
| 35 | function_output | `MaterialExpressionFunctionOutput` | Output Projection Angle Mask | Projection Angle Mask |
| 36 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output Combined Mask | Combined Mask |
| 40 | function_input | `MaterialExpressionFunctionInput` | Input Local Projection Angle Mask Vector And Falloff Power (V4) (Vector4) | Local Projection Angle Mask Vector And Falloff Power (V4) |
| 41 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 42 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input XYZ Opacity Falloff Hardness (Vector3) | XYZ Opacity Falloff Hardness |
| 44 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 45 | function_input | `MaterialExpressionFunctionInput` | Input Camera Clip Mask Distance (Scalar) | Camera Clip Mask Distance |
| 46 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 47 | expression | `MaterialExpressionFeatureLevelSwitch` | Feature Level Switch |  |
| 48 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |

## Exact Input Wiring

### [0] Output 0-1 RGB UVW

- `A` <= [13] Divide output 0

### [3] Subtract

- `A` <= [1] WorldPositionBehindTranslucency output 0
- `B` <= [4] ObjectPivotPoint output 0

### [5] Dot

- `A` <= [9] Normalize output 0
- `B` <= [3] Subtract output 0

### [7] Dot

- `A` <= [10] Normalize output 0
- `B` <= [3] Subtract output 0

### [8] Append

- `A` <= [5] Dot output 0
- `B` <= [7] Dot output 0

### [9] Normalize

- `VectorInput` <= [6] ComponentBasisVectors output 0

### [10] Normalize

- `VectorInput` <= [6] ComponentBasisVectors output 1

### [11] Dot

- `A` <= [12] Normalize output 0
- `B` <= [3] Subtract output 0

### [12] Normalize

- `VectorInput` <= [6] ComponentBasisVectors output 2

### [13] Divide

- `A` <= [14] Append output 0
- `B` <= [2] ObjectScale output 0

### [14] Append

- `A` <= [8] Append output 0
- `B` <= [11] Dot output 0

### [15] SphereMask

- `A` <= [22] BreakOutFloat3Components output 0
- `B` <= [16] 0.5 output 0
- `Hardness` <= [44] BreakOutFloat3Components output 0

### [17] SphereMask

- `A` <= [22] BreakOutFloat3Components output 1
- `B` <= [23] 0.5 output 0
- `Hardness` <= [44] BreakOutFloat3Components output 1

### [18] Multiply

- `A` <= [15] SphereMask output 0
- `B` <= [17] SphereMask output 0

### [19] SphereMask

- `A` <= [22] BreakOutFloat3Components output 2
- `B` <= [20] 0.5 output 0
- `Hardness` <= [44] BreakOutFloat3Components output 2

### [21] Multiply

- `A` <= [18] Multiply output 0
- `B` <= [19] SphereMask output 0

### [24] Output Extent bounds masks

- `A` <= [21] Multiply output 0

### [27] SphereMask

- `A` <= [25] Camera Position output 0
- `B` <= [26] Absolute World Position output 0
- `Radius` <= [45] Input Camera Clip Mask Distance (Scalar) output 0

### [28] 1-x

- `Input` <= [27] SphereMask output 0

### [29] Output Camera Clip Mask

- `A` <= [28] 1-x output 0

### [31] Mask ( R G B )

- `Input` <= [47] Feature Level Switch output 0

### [32] Dot

- `A` <= [31] Mask ( R G B ) output 0
- `B` <= [46] Normalize output 0

### [33] Clamp

- `Input` <= [32] Dot output 0

### [34] Power

- `Base` <= [33] Clamp output 0
- `Exponent` <= [42] Mask ( A ) output 0

### [35] Output Projection Angle Mask

- `A` <= [32] Dot output 0

### [36] Instance & Particle Space to World Space TransformVector

- `Input` <= [41] Mask ( R G B ) output 0

### [37] Multiply

- `A` <= [24] Output Extent bounds masks output 0
- `B` <= [29] Output Camera Clip Mask output 0

### [38] Multiply

- `A` <= [37] Multiply output 0
- `B` <= [35] Output Projection Angle Mask output 0

### [39] Output Combined Mask

- `A` <= [38] Multiply output 0

### [41] Mask ( R G B )

- `Input` <= [40] Input Local Projection Angle Mask Vector And Falloff Power (V4) (Vector4) output 0

### [42] Mask ( A )

- `Input` <= [40] Input Local Projection Angle Mask Vector And Falloff Power (V4) (Vector4) output 0

### [46] Normalize

- `VectorInput` <= [36] Instance & Particle Space to World Space TransformVector output 0

### [47] Feature Level Switch

- `Default` <= [30] SceneTexture:WorldNormal output 0
- `Inputs` <= [48] 1,1,1 output 0

## Connection List

- [13] Divide output 0 -> Output 0-1 RGB UVW.A
- [1] WorldPositionBehindTranslucency output 0 -> Subtract.A
- [4] ObjectPivotPoint output 0 -> Subtract.B
- [9] Normalize output 0 -> Dot.A
- [3] Subtract output 0 -> Dot.B
- [10] Normalize output 0 -> Dot.A
- [3] Subtract output 0 -> Dot.B
- [5] Dot output 0 -> Append.A
- [7] Dot output 0 -> Append.B
- [6] ComponentBasisVectors output 0 -> Normalize.VectorInput
- [6] ComponentBasisVectors output 1 -> Normalize.VectorInput
- [12] Normalize output 0 -> Dot.A
- [3] Subtract output 0 -> Dot.B
- [6] ComponentBasisVectors output 2 -> Normalize.VectorInput
- [14] Append output 0 -> Divide.A
- [2] ObjectScale output 0 -> Divide.B
- [8] Append output 0 -> Append.A
- [11] Dot output 0 -> Append.B
- [22] BreakOutFloat3Components output 0 -> SphereMask.A
- [16] 0.5 output 0 -> SphereMask.B
- [44] BreakOutFloat3Components output 0 -> SphereMask.Hardness
- [22] BreakOutFloat3Components output 1 -> SphereMask.A
- [23] 0.5 output 0 -> SphereMask.B
- [44] BreakOutFloat3Components output 1 -> SphereMask.Hardness
- [15] SphereMask output 0 -> Multiply.A
- [17] SphereMask output 0 -> Multiply.B
- [22] BreakOutFloat3Components output 2 -> SphereMask.A
- [20] 0.5 output 0 -> SphereMask.B
- [44] BreakOutFloat3Components output 2 -> SphereMask.Hardness
- [18] Multiply output 0 -> Multiply.A
- [19] SphereMask output 0 -> Multiply.B
- [21] Multiply output 0 -> Output Extent bounds masks.A
- [25] Camera Position output 0 -> SphereMask.A
- [26] Absolute World Position output 0 -> SphereMask.B
- [45] Input Camera Clip Mask Distance (Scalar) output 0 -> SphereMask.Radius
- [27] SphereMask output 0 -> 1-x.Input
- [28] 1-x output 0 -> Output Camera Clip Mask.A
- [47] Feature Level Switch output 0 -> Mask ( R G B ).Input
- [31] Mask ( R G B ) output 0 -> Dot.A
- [46] Normalize output 0 -> Dot.B
- [32] Dot output 0 -> Clamp.Input
- [33] Clamp output 0 -> Power.Base
- [42] Mask ( A ) output 0 -> Power.Exponent
- [32] Dot output 0 -> Output Projection Angle Mask.A
- [41] Mask ( R G B ) output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [24] Output Extent bounds masks output 0 -> Multiply.A
- [29] Output Camera Clip Mask output 0 -> Multiply.B
- [37] Multiply output 0 -> Multiply.A
- [35] Output Projection Angle Mask output 0 -> Multiply.B
- [38] Multiply output 0 -> Output Combined Mask.A
- [40] Input Local Projection Angle Mask Vector And Falloff Power (V4) (Vector4) output 0 -> Mask ( R G B ).Input
- [40] Input Local Projection Angle Mask Vector And Falloff Power (V4) (Vector4) output 0 -> Mask ( A ).Input
- [36] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [30] SceneTexture:WorldNormal output 0 -> Feature Level Switch.Default
- [48] 1,1,1 output 0 -> Feature Level Switch.Inputs

## Output Trace Roots

- `0-1 RGB UVW` is driven by [13] Divide
- `Extent bounds masks` is driven by [21] Multiply
- `Camera Clip Mask` is driven by [28] 1-x
- `Projection Angle Mask` is driven by [32] Dot
- `Combined Mask` is driven by [38] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
