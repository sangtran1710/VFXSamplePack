# LocalAlignedTexture_TransformedWorldSpace

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/LocalAlignedTexture_TransformedWorldSpace.LocalAlignedTexture_TransformedWorldSpace`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Tiles a texture in worldspace

## Inputs

- `TextureObject` [FunctionInput_Texture2D]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Z Texture`
- `XY Texture`
- `XYZ Texture`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Z Texture | Z Texture |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 3 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture | XY Texture |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 8 | expression | `MaterialExpressionConstant` | 1 |  |
| 9 | expression | `MaterialExpressionAbs` | Abs |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 11 | expression | `MaterialExpressionTransform` | World Space to Local Space TransformVector |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 13 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 14 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 15 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 16 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture | XYZ Texture |
| 18 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 20 | expression | `MaterialExpressionAbs` | Abs |  |
| 21 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 22 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |

## Exact Input Wiring

### [0] Output Z Texture

- `A` <= [1] Texture Sample output 0

### [1] Texture Sample

- `Coordinates` <= [4] TexCoord[2] output 0
- `TextureObject` <= [2] Input TextureObject (Texture2D) output 0

### [2] Input TextureObject (Texture2D)

- `Preview` <= [3] Texture Object output 0

### [5] Output XY Texture

- `A` <= [6] Lerp output 0

### [6] Lerp

- `A` <= [15] Texture Sample output 0
- `B` <= [13] Texture Sample output 0
- `Alpha` <= [7] CheapContrast output 0

### [9] Abs

- `Input` <= [10] Mask ( R ) output 0

### [10] Mask ( R )

- `Input` <= [22] VertexNormalWS output 0

### [11] World Space to Local Space TransformVector

- `Input` <= [22] VertexNormalWS output 0

### [13] Texture Sample

- `Coordinates` <= [14] TexCoord[0] output 0
- `TextureObject` <= [2] Input TextureObject (Texture2D) output 0

### [15] Texture Sample

- `Coordinates` <= [16] TexCoord[1] output 0
- `TextureObject` <= [2] Input TextureObject (Texture2D) output 0

### [17] Output XYZ Texture

- `A` <= [18] Lerp output 0

### [18] Lerp

- `A` <= [6] Lerp output 0
- `B` <= [1] Texture Sample output 0
- `Alpha` <= [19] CheapContrast output 0

### [20] Abs

- `Input` <= [21] Mask ( B ) output 0

### [21] Mask ( B )

- `Input` <= [22] VertexNormalWS output 0

## Connection List

- [1] Texture Sample output 0 -> Output Z Texture.A
- [4] TexCoord[2] output 0 -> Texture Sample.Coordinates
- [2] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [3] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [6] Lerp output 0 -> Output XY Texture.A
- [15] Texture Sample output 0 -> Lerp.A
- [13] Texture Sample output 0 -> Lerp.B
- [7] CheapContrast output 0 -> Lerp.Alpha
- [10] Mask ( R ) output 0 -> Abs.Input
- [22] VertexNormalWS output 0 -> Mask ( R ).Input
- [22] VertexNormalWS output 0 -> World Space to Local Space TransformVector.Input
- [14] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [2] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [16] TexCoord[1] output 0 -> Texture Sample.Coordinates
- [2] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [18] Lerp output 0 -> Output XYZ Texture.A
- [6] Lerp output 0 -> Lerp.A
- [1] Texture Sample output 0 -> Lerp.B
- [19] CheapContrast output 0 -> Lerp.Alpha
- [21] Mask ( B ) output 0 -> Abs.Input
- [22] VertexNormalWS output 0 -> Mask ( B ).Input

## Output Trace Roots

- `Z Texture` is driven by [1] Texture Sample
- `XY Texture` is driven by [6] Lerp
- `XYZ Texture` is driven by [18] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
