# NormalFromHeightmap

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/NormalFromHeightmap.NormalFromHeightmap`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Takes a heightmap input and generates a normal map from it.

## Inputs

- `Height Map` [FunctionInput_Texture2D] - Uses the R channel as the heightmap by default.
- `Coordinates` [FunctionInput_Vector2]
- `Height Map UV Offset` [FunctionInput_Scalar] - Spread used to derive the height from a scalar input. Default is .005
- `Normal Map Intensity` [FunctionInput_Scalar] - How strong the normal map will be.  Default is 8.
- `Height Map Channel Selector` [FunctionInput_Vector4] - If you would like the red channel to be used to generate the normal map enter (1,0,0,0). If the green channel is preferred enter (0,1,0,0) etc.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Height Map (Texture2D) | Height Map |
| 7 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector2) | Coordinates |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 10 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Height Map UV Offset (Scalar) | Height Map UV Offset |
| 14 | expression | `MaterialExpressionConstant` | 0.005 |  |
| 15 | expression | `MaterialExpressionConstant` | 0 |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Normal Map Intensity (Scalar) | Normal Map Intensity |
| 17 | expression | `MaterialExpressionConstant` | 8 |  |
| 18 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 19 | expression | `MaterialExpressionAppendVector` | Append |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 22 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | expression | `MaterialExpressionConstant2Vector` | 1,0 |  |
| 26 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Height Map Channel Selector (Vector4) | Height Map Channel Selector |
| 28 | expression | `MaterialExpressionAppendVector` | Append |  |
| 29 | expression | `MaterialExpressionAppendVector` | Append |  |
| 30 | expression | `MaterialExpressionAppendVector` | Append |  |
| 31 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 32 | expression | `MaterialExpressionDotProduct` | Dot |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Cross output 0

### [1] Cross

- `A` <= [19] Append output 0
- `B` <= [2] Append output 0

### [2] Append

- `A` <= [18] 0,1 output 0
- `B` <= [3] Multiply output 0

### [3] Multiply

- `A` <= [16] Input Normal Map Intensity (Scalar) output 0
- `B` <= [4] Subtract output 0

### [4] Subtract

- `A` <= [26] Dot output 0
- `B` <= [31] Dot output 0

### [5] Texture Sample

- `Coordinates` <= [8] Input Coordinates (Vector2) output 0
- `TextureObject` <= [6] Input Height Map (Texture2D) output 0

### [6] Input Height Map (Texture2D)

- `Preview` <= [7] Texture Object output 0

### [8] Input Coordinates (Vector2)

- `Preview` <= [9] TexCoord[0] output 0

### [10] Texture Sample

- `Coordinates` <= [11] Add output 0
- `TextureObject` <= [6] Input Height Map (Texture2D) output 0

### [11] Add

- `A` <= [8] Input Coordinates (Vector2) output 0
- `B` <= [12] Append output 0

### [12] Append

- `A` <= [15] 0 output 0
- `B` <= [13] Input Height Map UV Offset (Scalar) output 0

### [13] Input Height Map UV Offset (Scalar)

- `Preview` <= [14] 0.005 output 0

### [16] Input Normal Map Intensity (Scalar)

- `Preview` <= [17] 8 output 0

### [19] Append

- `A` <= [25] 1,0 output 0
- `B` <= [20] Multiply output 0

### [20] Multiply

- `A` <= [21] Subtract output 0
- `B` <= [16] Input Normal Map Intensity (Scalar) output 0

### [21] Subtract

- `A` <= [32] Dot output 0
- `B` <= [31] Dot output 0

### [22] Texture Sample

- `Coordinates` <= [23] Add output 0
- `TextureObject` <= [6] Input Height Map (Texture2D) output 0

### [23] Add

- `A` <= [24] Append output 0
- `B` <= [8] Input Coordinates (Vector2) output 0

### [24] Append

- `A` <= [13] Input Height Map UV Offset (Scalar) output 0
- `B` <= [15] 0 output 0

### [26] Dot

- `A` <= [30] Append output 0
- `B` <= [27] Input Height Map Channel Selector (Vector4) output 0

### [28] Append

- `A` <= [22] Texture Sample output 0
- `B` <= [22] Texture Sample output 4

### [29] Append

- `A` <= [5] Texture Sample output 0
- `B` <= [5] Texture Sample output 4

### [30] Append

- `A` <= [10] Texture Sample output 0
- `B` <= [10] Texture Sample output 4

### [31] Dot

- `A` <= [29] Append output 0
- `B` <= [27] Input Height Map Channel Selector (Vector4) output 0

### [32] Dot

- `A` <= [28] Append output 0
- `B` <= [27] Input Height Map Channel Selector (Vector4) output 0

## Connection List

- [1] Cross output 0 -> Output Result.A
- [19] Append output 0 -> Cross.A
- [2] Append output 0 -> Cross.B
- [18] 0,1 output 0 -> Append.A
- [3] Multiply output 0 -> Append.B
- [16] Input Normal Map Intensity (Scalar) output 0 -> Multiply.A
- [4] Subtract output 0 -> Multiply.B
- [26] Dot output 0 -> Subtract.A
- [31] Dot output 0 -> Subtract.B
- [8] Input Coordinates (Vector2) output 0 -> Texture Sample.Coordinates
- [6] Input Height Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [7] Texture Object output 0 -> Input Height Map (Texture2D).Preview
- [9] TexCoord[0] output 0 -> Input Coordinates (Vector2).Preview
- [11] Add output 0 -> Texture Sample.Coordinates
- [6] Input Height Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Input Coordinates (Vector2) output 0 -> Add.A
- [12] Append output 0 -> Add.B
- [15] 0 output 0 -> Append.A
- [13] Input Height Map UV Offset (Scalar) output 0 -> Append.B
- [14] 0.005 output 0 -> Input Height Map UV Offset (Scalar).Preview
- [17] 8 output 0 -> Input Normal Map Intensity (Scalar).Preview
- [25] 1,0 output 0 -> Append.A
- [20] Multiply output 0 -> Append.B
- [21] Subtract output 0 -> Multiply.A
- [16] Input Normal Map Intensity (Scalar) output 0 -> Multiply.B
- [32] Dot output 0 -> Subtract.A
- [31] Dot output 0 -> Subtract.B
- [23] Add output 0 -> Texture Sample.Coordinates
- [6] Input Height Map (Texture2D) output 0 -> Texture Sample.TextureObject
- [24] Append output 0 -> Add.A
- [8] Input Coordinates (Vector2) output 0 -> Add.B
- [13] Input Height Map UV Offset (Scalar) output 0 -> Append.A
- [15] 0 output 0 -> Append.B
- [30] Append output 0 -> Dot.A
- [27] Input Height Map Channel Selector (Vector4) output 0 -> Dot.B
- [22] Texture Sample output 0 -> Append.A
- [22] Texture Sample output 4 -> Append.B
- [5] Texture Sample output 0 -> Append.A
- [5] Texture Sample output 4 -> Append.B
- [10] Texture Sample output 0 -> Append.A
- [10] Texture Sample output 4 -> Append.B
- [29] Append output 0 -> Dot.A
- [27] Input Height Map Channel Selector (Vector4) output 0 -> Dot.B
- [28] Append output 0 -> Dot.A
- [27] Input Height Map Channel Selector (Vector4) output 0 -> Dot.B

## Output Trace Roots

- `Result` is driven by [1] Cross

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


