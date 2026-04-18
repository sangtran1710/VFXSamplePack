# MatLayerBlend_StandardWithDisplacement

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_StandardWithDisplacement.MatLayerBlend_StandardWithDisplacement`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends all attributes of 2 Materials

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `Top Material` [FunctionInput_MaterialAttributes]
- `Base Material` [FunctionInput_MaterialAttributes]

## Outputs

- `Blended Material`
- `Displacement`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 4 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Top Material (MaterialAttributes) | Top Material |
| 6 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 8 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 9 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 11 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 15 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 16 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,) |  |
| 17 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,1,) |  |
| 18 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 19 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 20 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 21 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Displacement | Displacement |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [1] MakeMaterialAttributes output 0

### [1] MakeMaterialAttributes

- `BaseColor` <= [11] Lerp output 0
- `Metallic` <= [10] Lerp output 0
- `Specular` <= [9] Lerp output 0
- `Roughness` <= [8] Lerp output 0
- `EmissiveColor` <= [12] Lerp output 0
- `Opacity` <= [19] Lerp output 0
- `OpacityMask` <= [20] Lerp output 0
- `Normal` <= [2] Lerp output 0
- `WorldPositionOffset` <= [6] BreakMaterialAttributes output 10
- `SubsurfaceColor` <= [15] Lerp output 0
- `ClearCoat` <= [14] Lerp output 0
- `ClearCoatRoughness` <= [18] Lerp output 0
- `AmbientOcclusion` <= [21] Lerp output 0
- `Refraction` <= [4] BreakMaterialAttributes output 15
- `CustomizedUVs` <= [4] BreakMaterialAttributes output 16
- `PixelDepthOffset` <= [4] BreakMaterialAttributes output 24

### [2] Lerp

- `A` <= [6] BreakMaterialAttributes output 8
- `B` <= [4] BreakMaterialAttributes output 8
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [8] Lerp

- `A` <= [6] BreakMaterialAttributes output 3
- `B` <= [4] BreakMaterialAttributes output 3
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [9] Lerp

- `A` <= [6] BreakMaterialAttributes output 2
- `B` <= [4] BreakMaterialAttributes output 2
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [10] Lerp

- `A` <= [6] BreakMaterialAttributes output 1
- `B` <= [4] BreakMaterialAttributes output 1
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [11] Lerp

- `A` <= [6] BreakMaterialAttributes output 0
- `B` <= [23] Multiply output 0
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [12] Lerp

- `A` <= [6] BreakMaterialAttributes output 5
- `B` <= [4] BreakMaterialAttributes output 5
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [13] Lerp

- `A` <= [6] BreakMaterialAttributes output 10
- `B` <= [4] BreakMaterialAttributes output 10
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [14] Lerp

- `A` <= [6] BreakMaterialAttributes output 12
- `B` <= [4] BreakMaterialAttributes output 12
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [15] Lerp

- `A` <= [6] BreakMaterialAttributes output 11
- `B` <= [4] BreakMaterialAttributes output 11
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [16] Lerp(0,1,)

- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [17] Lerp(0,1,)

- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [18] Lerp

- `A` <= [6] BreakMaterialAttributes output 13
- `B` <= [4] BreakMaterialAttributes output 13
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [19] Lerp

- `A` <= [6] BreakMaterialAttributes output 6
- `B` <= [4] BreakMaterialAttributes output 6
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [20] Lerp

- `A` <= [6] BreakMaterialAttributes output 7
- `B` <= [4] BreakMaterialAttributes output 7
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [21] Lerp

- `A` <= [6] BreakMaterialAttributes output 14
- `B` <= [4] BreakMaterialAttributes output 14
- `Alpha` <= [3] Input Alpha (Scalar) output 0

### [22] Output Displacement

- `A` <= [17] Lerp(0,1,) output 0

### [23] Multiply

- `A` <= [4] BreakMaterialAttributes output 0
- `B` <= [3] Input Alpha (Scalar) output 0

## Connection List

- [1] MakeMaterialAttributes output 0 -> Output Blended Material.A
- [11] Lerp output 0 -> MakeMaterialAttributes.BaseColor
- [10] Lerp output 0 -> MakeMaterialAttributes.Metallic
- [9] Lerp output 0 -> MakeMaterialAttributes.Specular
- [8] Lerp output 0 -> MakeMaterialAttributes.Roughness
- [12] Lerp output 0 -> MakeMaterialAttributes.EmissiveColor
- [19] Lerp output 0 -> MakeMaterialAttributes.Opacity
- [20] Lerp output 0 -> MakeMaterialAttributes.OpacityMask
- [2] Lerp output 0 -> MakeMaterialAttributes.Normal
- [6] BreakMaterialAttributes output 10 -> MakeMaterialAttributes.WorldPositionOffset
- [15] Lerp output 0 -> MakeMaterialAttributes.SubsurfaceColor
- [14] Lerp output 0 -> MakeMaterialAttributes.ClearCoat
- [18] Lerp output 0 -> MakeMaterialAttributes.ClearCoatRoughness
- [21] Lerp output 0 -> MakeMaterialAttributes.AmbientOcclusion
- [4] BreakMaterialAttributes output 15 -> MakeMaterialAttributes.Refraction
- [4] BreakMaterialAttributes output 16 -> MakeMaterialAttributes.CustomizedUVs
- [4] BreakMaterialAttributes output 24 -> MakeMaterialAttributes.PixelDepthOffset
- [6] BreakMaterialAttributes output 8 -> Lerp.A
- [4] BreakMaterialAttributes output 8 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 3 -> Lerp.A
- [4] BreakMaterialAttributes output 3 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 2 -> Lerp.A
- [4] BreakMaterialAttributes output 2 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 1 -> Lerp.A
- [4] BreakMaterialAttributes output 1 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 0 -> Lerp.A
- [23] Multiply output 0 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 5 -> Lerp.A
- [4] BreakMaterialAttributes output 5 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 10 -> Lerp.A
- [4] BreakMaterialAttributes output 10 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 12 -> Lerp.A
- [4] BreakMaterialAttributes output 12 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 11 -> Lerp.A
- [4] BreakMaterialAttributes output 11 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [3] Input Alpha (Scalar) output 0 -> Lerp(0,1,).Alpha
- [3] Input Alpha (Scalar) output 0 -> Lerp(0,1,).Alpha
- [6] BreakMaterialAttributes output 13 -> Lerp.A
- [4] BreakMaterialAttributes output 13 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 6 -> Lerp.A
- [4] BreakMaterialAttributes output 6 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 7 -> Lerp.A
- [4] BreakMaterialAttributes output 7 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [6] BreakMaterialAttributes output 14 -> Lerp.A
- [4] BreakMaterialAttributes output 14 -> Lerp.B
- [3] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [17] Lerp(0,1,) output 0 -> Output Displacement.A
- [4] BreakMaterialAttributes output 0 -> Multiply.A
- [3] Input Alpha (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Blended Material` is driven by [1] MakeMaterialAttributes
- `Displacement` is driven by [17] Lerp(0,1,)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


