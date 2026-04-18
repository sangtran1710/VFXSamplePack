# MatLayerBlend_Decal_UV3

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Decal_UV3.MatLayerBlend_Decal_UV3`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends in a decal sheet using the 2nd uv channel

## Inputs

- `Base Material` [FunctionInput_MaterialAttributes]
- `DecalRoughness` [FunctionInput_Scalar]
- `DecalSheet` [FunctionInput_Texture2D] - Color map with Alpha Channel
- `DecalNormalFlatness` [FunctionInput_Scalar]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 5 | expression | `MaterialExpressionConstant` | 0 |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input DecalRoughness (Scalar) | DecalRoughness |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input DecalSheet (Texture2D) | DecalSheet |
| 8 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input DecalNormalFlatness (Scalar) | DecalNormalFlatness |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 14 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 15 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [15] SetMaterialAttributes output 0

### [2] Lerp

- `A` <= [14] GetMaterialAttributes output 3
- `B` <= [6] Input DecalRoughness (Scalar) output 0
- `Alpha` <= [8] Texture Sample output 4

### [3] Lerp

- `A` <= [14] GetMaterialAttributes output 2
- `B` <= [5] 0 output 0
- `Alpha` <= [8] Texture Sample output 4

### [4] Lerp

- `A` <= [14] GetMaterialAttributes output 1
- `B` <= [8] Texture Sample output 0
- `Alpha` <= [8] Texture Sample output 4

### [8] Texture Sample

- `Coordinates` <= [9] TexCoord[2] output 0
- `TextureObject` <= [7] Input DecalSheet (Texture2D) output 0

### [10] Multiply

- `A` <= [8] Texture Sample output 4
- `B` <= [11] Input DecalNormalFlatness (Scalar) output 0

### [12] Lerp

- `A` <= [14] GetMaterialAttributes output 4
- `B` <= [13] 0,0,1 output 0
- `Alpha` <= [10] Multiply output 0

### [15] SetMaterialAttributes

- `Inputs[0]` <= [14] GetMaterialAttributes output 0
- `Inputs[1]` <= [4] Lerp output 0
- `Inputs[2]` <= [3] Lerp output 0
- `Inputs[3]` <= [2] Lerp output 0
- `Inputs[4]` <= [12] Lerp output 0

## Connection List

- [15] SetMaterialAttributes output 0 -> Output Blended Material.A
- [14] GetMaterialAttributes output 3 -> Lerp.A
- [6] Input DecalRoughness (Scalar) output 0 -> Lerp.B
- [8] Texture Sample output 4 -> Lerp.Alpha
- [14] GetMaterialAttributes output 2 -> Lerp.A
- [5] 0 output 0 -> Lerp.B
- [8] Texture Sample output 4 -> Lerp.Alpha
- [14] GetMaterialAttributes output 1 -> Lerp.A
- [8] Texture Sample output 0 -> Lerp.B
- [8] Texture Sample output 4 -> Lerp.Alpha
- [9] TexCoord[2] output 0 -> Texture Sample.Coordinates
- [7] Input DecalSheet (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Texture Sample output 4 -> Multiply.A
- [11] Input DecalNormalFlatness (Scalar) output 0 -> Multiply.B
- [14] GetMaterialAttributes output 4 -> Lerp.A
- [13] 0,0,1 output 0 -> Lerp.B
- [10] Multiply output 0 -> Lerp.Alpha
- [14] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [4] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [3] Lerp output 0 -> SetMaterialAttributes.Inputs[2]
- [2] Lerp output 0 -> SetMaterialAttributes.Inputs[3]
- [12] Lerp output 0 -> SetMaterialAttributes.Inputs[4]

## Output Trace Roots

- `Blended Material` is driven by [15] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
