# WorldAlignedTexture_MipBias

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture_MipBias.WorldAlignedTexture_MipBias`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Tiles a texture in worldspace

## Inputs

- `World Space Normal` [FunctionInput_Vector3]
- `TextureObject` [FunctionInput_Texture2D]
- `TextureSize` [FunctionInput_Vector3] - Size of texture in worldspace units
- `WorldPosition` [FunctionInput_Vector3]
- `Export Float 4` [FunctionInput_StaticBool] - Default = False
- `ProjectionTransitionContrast` [FunctionInput_Scalar]
- `MipBias` [FunctionInput_Scalar]

## Outputs

- `XYZ Texture`
- `XY Texture`
- `Z Texture`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture | XYZ Texture |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 3 | expression | `MaterialExpressionAbs` | Abs |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input World Space Normal (Vector3) | World Space Normal |
| 6 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 8 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (Vector3) | TextureSize |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 13 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 16 | expression | `MaterialExpressionAbs` | Abs |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 18 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 19 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 20 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 21 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture | XY Texture |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output Z Texture | Z Texture |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | expression | `MaterialExpressionAppendVector` | Append |  |
| 26 | expression | `MaterialExpressionAppendVector` | Append |  |
| 27 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 28 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 29 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Export Float 4 (StaticBool) | Export Float 4 |
| 31 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 32 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 33 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 34 | function_input | `MaterialExpressionFunctionInput` | Input ProjectionTransitionContrast (Scalar) | ProjectionTransitionContrast |
| 35 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 36 | expression | `MaterialExpressionAbs` | Abs |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 38 | function_input | `MaterialExpressionFunctionInput` | Input MipBias (Scalar) | MipBias |

## Exact Input Wiring

### [0] Output XYZ Texture

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [14] Lerp output 0
- `B` <= [29] Switch output 0
- `Alpha` <= [33] Mask ( R ) output 0

### [3] Abs

- `Input` <= [4] Mask ( B ) output 0

### [4] Mask ( B )

- `Input` <= [5] Input World Space Normal (Vector3) output 0

### [5] Input World Space Normal (Vector3)

- `Preview` <= [35] VertexNormalWS output 0

### [6] Texture Sample

- `Coordinates` <= [9] Mask ( R G ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0
- `MipValue` <= [38] Input MipBias (Scalar) output 0

### [7] Input TextureObject (Texture2D)

- `Preview` <= [8] Texture Object output 0

### [9] Mask ( R G )

- `Input` <= [10] Divide output 0

### [10] Divide

- `A` <= [12] Input WorldPosition (Vector3) output 0
- `B` <= [37] Multiply(,-1) output 0

### [12] Input WorldPosition (Vector3)

- `Preview` <= [13] Absolute World Position (Excluding Material Offsets) output 0

### [14] Lerp

- `A` <= [28] Switch output 0
- `B` <= [27] Switch output 0
- `Alpha` <= [32] Mask ( R ) output 0

### [16] Abs

- `Input` <= [17] Mask ( R ) output 0

### [17] Mask ( R )

- `Input` <= [5] Input World Space Normal (Vector3) output 0

### [18] Texture Sample

- `Coordinates` <= [19] Mask ( G B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0
- `MipValue` <= [38] Input MipBias (Scalar) output 0

### [19] Mask ( G B )

- `Input` <= [10] Divide output 0

### [20] Texture Sample

- `Coordinates` <= [21] Mask ( R B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0
- `MipValue` <= [38] Input MipBias (Scalar) output 0

### [21] Mask ( R B )

- `Input` <= [10] Divide output 0

### [22] Output XY Texture

- `A` <= [14] Lerp output 0

### [23] Output Z Texture

- `A` <= [29] Switch output 0

### [24] Append

- `A` <= [18] Texture Sample output 0
- `B` <= [18] Texture Sample output 4

### [25] Append

- `A` <= [20] Texture Sample output 0
- `B` <= [20] Texture Sample output 4

### [26] Append

- `A` <= [6] Texture Sample output 0
- `B` <= [6] Texture Sample output 4

### [27] Switch

- `A` <= [24] Append output 0
- `B` <= [18] Texture Sample output 0
- `Value` <= [30] Input Export Float 4 (StaticBool) output 0

### [28] Switch

- `A` <= [25] Append output 0
- `B` <= [20] Texture Sample output 0
- `Value` <= [30] Input Export Float 4 (StaticBool) output 0

### [29] Switch

- `A` <= [26] Append output 0
- `B` <= [6] Texture Sample output 0
- `Value` <= [30] Input Export Float 4 (StaticBool) output 0

### [30] Input Export Float 4 (StaticBool)

- `Preview` <= [31] Static Bool (False) output 0

### [32] Mask ( R )

- `Input` <= [15] CheapContrast output 0

### [33] Mask ( R )

- `Input` <= [2] CheapContrast output 0

### [36] Abs

- `Input` <= [11] Input TextureSize (Vector3) output 0

### [37] Multiply(,-1)

- `A` <= [36] Abs output 0

## Connection List

- [1] Lerp output 0 -> Output XYZ Texture.A
- [14] Lerp output 0 -> Lerp.A
- [29] Switch output 0 -> Lerp.B
- [33] Mask ( R ) output 0 -> Lerp.Alpha
- [4] Mask ( B ) output 0 -> Abs.Input
- [5] Input World Space Normal (Vector3) output 0 -> Mask ( B ).Input
- [35] VertexNormalWS output 0 -> Input World Space Normal (Vector3).Preview
- [9] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [38] Input MipBias (Scalar) output 0 -> Texture Sample.MipValue
- [8] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [10] Divide output 0 -> Mask ( R G ).Input
- [12] Input WorldPosition (Vector3) output 0 -> Divide.A
- [37] Multiply(,-1) output 0 -> Divide.B
- [13] Absolute World Position (Excluding Material Offsets) output 0 -> Input WorldPosition (Vector3).Preview
- [28] Switch output 0 -> Lerp.A
- [27] Switch output 0 -> Lerp.B
- [32] Mask ( R ) output 0 -> Lerp.Alpha
- [17] Mask ( R ) output 0 -> Abs.Input
- [5] Input World Space Normal (Vector3) output 0 -> Mask ( R ).Input
- [19] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [38] Input MipBias (Scalar) output 0 -> Texture Sample.MipValue
- [10] Divide output 0 -> Mask ( G B ).Input
- [21] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [38] Input MipBias (Scalar) output 0 -> Texture Sample.MipValue
- [10] Divide output 0 -> Mask ( R B ).Input
- [14] Lerp output 0 -> Output XY Texture.A
- [29] Switch output 0 -> Output Z Texture.A
- [18] Texture Sample output 0 -> Append.A
- [18] Texture Sample output 4 -> Append.B
- [20] Texture Sample output 0 -> Append.A
- [20] Texture Sample output 4 -> Append.B
- [6] Texture Sample output 0 -> Append.A
- [6] Texture Sample output 4 -> Append.B
- [24] Append output 0 -> Switch.A
- [18] Texture Sample output 0 -> Switch.B
- [30] Input Export Float 4 (StaticBool) output 0 -> Switch.Value
- [25] Append output 0 -> Switch.A
- [20] Texture Sample output 0 -> Switch.B
- [30] Input Export Float 4 (StaticBool) output 0 -> Switch.Value
- [26] Append output 0 -> Switch.A
- [6] Texture Sample output 0 -> Switch.B
- [30] Input Export Float 4 (StaticBool) output 0 -> Switch.Value
- [31] Static Bool (False) output 0 -> Input Export Float 4 (StaticBool).Preview
- [15] CheapContrast output 0 -> Mask ( R ).Input
- [2] CheapContrast output 0 -> Mask ( R ).Input
- [11] Input TextureSize (Vector3) output 0 -> Abs.Input
- [36] Abs output 0 -> Multiply(,-1).A

## Output Trace Roots

- `XYZ Texture` is driven by [1] Lerp
- `XY Texture` is driven by [14] Lerp
- `Z Texture` is driven by [29] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
