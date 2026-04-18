# MatLayerBlend_OverrideDisplacement

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideDisplacement.MatLayerBlend_OverrideDisplacement`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Replaces WorldPositionDisplacement

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `WorldPositionDisplacement` [FunctionInput_Vector3]
- `Mask` [FunctionInput_Scalar]
- `TessellationMultiplier` [FunctionInput_Scalar]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 2 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input WorldPositionDisplacement (Vector3) | WorldPositionDisplacement |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Mask (Scalar) | Mask |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,,) |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0,,) |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input TessellationMultiplier (Scalar) | TessellationMultiplier |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [1] MakeMaterialAttributes output 0

### [1] MakeMaterialAttributes

- `BaseColor` <= [2] BreakMaterialAttributes output 0
- `Metallic` <= [2] BreakMaterialAttributes output 1
- `Specular` <= [2] BreakMaterialAttributes output 2
- `Roughness` <= [2] BreakMaterialAttributes output 3
- `EmissiveColor` <= [2] BreakMaterialAttributes output 5
- `Opacity` <= [2] BreakMaterialAttributes output 6
- `OpacityMask` <= [2] BreakMaterialAttributes output 7
- `Normal` <= [2] BreakMaterialAttributes output 8
- `WorldPositionOffset` <= [2] BreakMaterialAttributes output 10
- `SubsurfaceColor` <= [2] BreakMaterialAttributes output 11
- `ClearCoat` <= [2] BreakMaterialAttributes output 12
- `ClearCoatRoughness` <= [2] BreakMaterialAttributes output 13
- `AmbientOcclusion` <= [2] BreakMaterialAttributes output 14
- `Refraction` <= [2] BreakMaterialAttributes output 15
- `CustomizedUVs` <= [2] BreakMaterialAttributes output 16
- `PixelDepthOffset` <= [2] BreakMaterialAttributes output 24

### [6] Lerp(0,,)

- `B` <= [4] Input WorldPositionDisplacement (Vector3) output 0
- `Alpha` <= [5] Input Mask (Scalar) output 0

### [7] Lerp(0,,)

- `B` <= [8] Input TessellationMultiplier (Scalar) output 0
- `Alpha` <= [5] Input Mask (Scalar) output 0

## Connection List

- [1] MakeMaterialAttributes output 0 -> Output Blended Material.A
- [2] BreakMaterialAttributes output 0 -> MakeMaterialAttributes.BaseColor
- [2] BreakMaterialAttributes output 1 -> MakeMaterialAttributes.Metallic
- [2] BreakMaterialAttributes output 2 -> MakeMaterialAttributes.Specular
- [2] BreakMaterialAttributes output 3 -> MakeMaterialAttributes.Roughness
- [2] BreakMaterialAttributes output 5 -> MakeMaterialAttributes.EmissiveColor
- [2] BreakMaterialAttributes output 6 -> MakeMaterialAttributes.Opacity
- [2] BreakMaterialAttributes output 7 -> MakeMaterialAttributes.OpacityMask
- [2] BreakMaterialAttributes output 8 -> MakeMaterialAttributes.Normal
- [2] BreakMaterialAttributes output 10 -> MakeMaterialAttributes.WorldPositionOffset
- [2] BreakMaterialAttributes output 11 -> MakeMaterialAttributes.SubsurfaceColor
- [2] BreakMaterialAttributes output 12 -> MakeMaterialAttributes.ClearCoat
- [2] BreakMaterialAttributes output 13 -> MakeMaterialAttributes.ClearCoatRoughness
- [2] BreakMaterialAttributes output 14 -> MakeMaterialAttributes.AmbientOcclusion
- [2] BreakMaterialAttributes output 15 -> MakeMaterialAttributes.Refraction
- [2] BreakMaterialAttributes output 16 -> MakeMaterialAttributes.CustomizedUVs
- [2] BreakMaterialAttributes output 24 -> MakeMaterialAttributes.PixelDepthOffset
- [4] Input WorldPositionDisplacement (Vector3) output 0 -> Lerp(0,,).B
- [5] Input Mask (Scalar) output 0 -> Lerp(0,,).Alpha
- [8] Input TessellationMultiplier (Scalar) output 0 -> Lerp(0,,).B
- [5] Input Mask (Scalar) output 0 -> Lerp(0,,).Alpha

## Output Trace Roots

- `Blended Material` is driven by [1] MakeMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


