# MatLayerBlend_Multiply

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Multiply.MatLayerBlend_Multiply`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Multiplies all elements of Material Attributes by Multiplier

## Inputs

- `Base Material` [FunctionInput_MaterialAttributes]
- `Multiplier` [FunctionInput_Scalar]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 2 | expression | `MaterialExpressionBreakMaterialAttributes` | BreakMaterialAttributes |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Multiplier (Scalar) | Multiplier |
| 6 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(0,) |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [1] MakeMaterialAttributes output 0

### [1] MakeMaterialAttributes

- `BaseColor` <= [7] Multiply(,1) output 0
- `Metallic` <= [8] Multiply output 0
- `Specular` <= [9] Multiply output 0
- `Roughness` <= [4] Multiply output 0
- `EmissiveColor` <= [10] Multiply output 0
- `Opacity` <= [11] Multiply output 0
- `OpacityMask` <= [12] Multiply output 0
- `Normal` <= [13] Multiply output 0
- `WorldPositionOffset` <= [14] Multiply output 0
- `SubsurfaceColor` <= [16] Multiply output 0
- `ClearCoat` <= [17] Multiply output 0
- `ClearCoatRoughness` <= [18] Multiply output 0
- `AmbientOcclusion` <= [2] BreakMaterialAttributes output 14
- `Refraction` <= [2] BreakMaterialAttributes output 15
- `CustomizedUVs` <= [2] BreakMaterialAttributes output 16
- `PixelDepthOffset` <= [2] BreakMaterialAttributes output 24

### [3] Input Base Material (MaterialAttributes)

- `Preview` <= [6] MakeMaterialAttributes output 0

### [4] Multiply

- `A` <= [2] BreakMaterialAttributes output 3
- `B` <= [5] Input Multiplier (Scalar) output 0

### [7] Multiply(,1)

- `A` <= [2] BreakMaterialAttributes output 0

### [8] Multiply

- `A` <= [2] BreakMaterialAttributes output 1
- `B` <= [5] Input Multiplier (Scalar) output 0

### [9] Multiply

- `A` <= [2] BreakMaterialAttributes output 2
- `B` <= [5] Input Multiplier (Scalar) output 0

### [10] Multiply

- `A` <= [2] BreakMaterialAttributes output 5
- `B` <= [5] Input Multiplier (Scalar) output 0

### [11] Multiply

- `A` <= [2] BreakMaterialAttributes output 6
- `B` <= [5] Input Multiplier (Scalar) output 0

### [12] Multiply

- `A` <= [2] BreakMaterialAttributes output 7
- `B` <= [5] Input Multiplier (Scalar) output 0

### [13] Multiply

- `A` <= [2] BreakMaterialAttributes output 8
- `B` <= [5] Input Multiplier (Scalar) output 0

### [14] Multiply

- `A` <= [2] BreakMaterialAttributes output 10
- `B` <= [5] Input Multiplier (Scalar) output 0

### [15] Multiply(0,)

- `B` <= [5] Input Multiplier (Scalar) output 0

### [16] Multiply

- `A` <= [2] BreakMaterialAttributes output 11
- `B` <= [5] Input Multiplier (Scalar) output 0

### [17] Multiply

- `A` <= [2] BreakMaterialAttributes output 12
- `B` <= [5] Input Multiplier (Scalar) output 0

### [18] Multiply

- `A` <= [2] BreakMaterialAttributes output 13
- `B` <= [5] Input Multiplier (Scalar) output 0

## Connection List

- [1] MakeMaterialAttributes output 0 -> Output Blended Material.A
- [7] Multiply(,1) output 0 -> MakeMaterialAttributes.BaseColor
- [8] Multiply output 0 -> MakeMaterialAttributes.Metallic
- [9] Multiply output 0 -> MakeMaterialAttributes.Specular
- [4] Multiply output 0 -> MakeMaterialAttributes.Roughness
- [10] Multiply output 0 -> MakeMaterialAttributes.EmissiveColor
- [11] Multiply output 0 -> MakeMaterialAttributes.Opacity
- [12] Multiply output 0 -> MakeMaterialAttributes.OpacityMask
- [13] Multiply output 0 -> MakeMaterialAttributes.Normal
- [14] Multiply output 0 -> MakeMaterialAttributes.WorldPositionOffset
- [16] Multiply output 0 -> MakeMaterialAttributes.SubsurfaceColor
- [17] Multiply output 0 -> MakeMaterialAttributes.ClearCoat
- [18] Multiply output 0 -> MakeMaterialAttributes.ClearCoatRoughness
- [2] BreakMaterialAttributes output 14 -> MakeMaterialAttributes.AmbientOcclusion
- [2] BreakMaterialAttributes output 15 -> MakeMaterialAttributes.Refraction
- [2] BreakMaterialAttributes output 16 -> MakeMaterialAttributes.CustomizedUVs
- [2] BreakMaterialAttributes output 24 -> MakeMaterialAttributes.PixelDepthOffset
- [6] MakeMaterialAttributes output 0 -> Input Base Material (MaterialAttributes).Preview
- [2] BreakMaterialAttributes output 3 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 0 -> Multiply(,1).A
- [2] BreakMaterialAttributes output 1 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 2 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 5 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 6 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 7 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 8 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 10 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [5] Input Multiplier (Scalar) output 0 -> Multiply(0,).B
- [2] BreakMaterialAttributes output 11 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 12 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B
- [2] BreakMaterialAttributes output 13 -> Multiply.A
- [5] Input Multiplier (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Blended Material` is driven by [1] MakeMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
