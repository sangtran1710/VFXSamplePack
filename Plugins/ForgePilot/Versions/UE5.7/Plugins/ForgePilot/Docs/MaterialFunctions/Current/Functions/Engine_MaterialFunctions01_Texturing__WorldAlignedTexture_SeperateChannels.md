# WorldAlignedTexture_SeperateChannels

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture_SeperateChannels.WorldAlignedTexture_SeperateChannels`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Tiles a texture in worldspace

## Inputs

- `World Space Normal` [FunctionInput_Vector3]
- `TextureObject` [FunctionInput_Texture2D]
- `TextureSize (R)` [FunctionInput_Vector3] - Size of texture in worldspace units
- `WorldPosition` [FunctionInput_Vector3]
- `ProjectionTransitionContrast` [FunctionInput_Scalar]
- `TextureSize (G)` [FunctionInput_Vector3] - Size of texture in worldspace units
- `TextureSize (B)` [FunctionInput_Vector3] - Size of texture in worldspace units

## Outputs

- `XYZ Texture (R)`
- `XY Texture (R)`
- `Z Texture (R)`
- `XYZ Texture (G)`
- `XY Texture (G)`
- `Z Texture (G)`
- `XYZ Texture (B)`
- `XY Texture (B)`
- `Z Texture (B)`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture (R) | XYZ Texture (R) |
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
| 11 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (R) (Vector3) | TextureSize (R) |
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
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture (R) | XY Texture (R) |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output Z Texture (R) | Z Texture (R) |
| 24 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input ProjectionTransitionContrast (Scalar) | ProjectionTransitionContrast |
| 27 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 28 | expression | `MaterialExpressionAbs` | Abs |  |
| 29 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 30 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture (G) | XYZ Texture (G) |
| 31 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 32 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 33 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 34 | expression | `MaterialExpressionDivide` | Divide |  |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (G) (Vector3) | TextureSize (G) |
| 36 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 37 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 38 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 39 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 40 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture (G) | XY Texture (G) |
| 42 | function_output | `MaterialExpressionFunctionOutput` | Output Z Texture (G) | Z Texture (G) |
| 43 | expression | `MaterialExpressionAbs` | Abs |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 45 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture (B) | XYZ Texture (B) |
| 46 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 47 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 48 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 49 | expression | `MaterialExpressionDivide` | Divide |  |
| 50 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (B) (Vector3) | TextureSize (B) |
| 51 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 52 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 53 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 54 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 55 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 56 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture (B) | XY Texture (B) |
| 57 | function_output | `MaterialExpressionFunctionOutput` | Output Z Texture (B) | Z Texture (B) |
| 58 | expression | `MaterialExpressionAbs` | Abs |  |
| 59 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output XYZ Texture (R)

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [14] Lerp output 0
- `B` <= [6] Texture Sample output 1
- `Alpha` <= [25] Mask ( R ) output 0

### [3] Abs

- `Input` <= [4] Mask ( B ) output 0

### [4] Mask ( B )

- `Input` <= [5] Input World Space Normal (Vector3) output 0

### [5] Input World Space Normal (Vector3)

- `Preview` <= [27] VertexNormalWS output 0

### [6] Texture Sample

- `Coordinates` <= [9] Mask ( R G ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [7] Input TextureObject (Texture2D)

- `Preview` <= [8] Texture Object output 0

### [9] Mask ( R G )

- `Input` <= [10] Divide output 0

### [10] Divide

- `A` <= [12] Input WorldPosition (Vector3) output 0
- `B` <= [29] Multiply(,-1) output 0

### [12] Input WorldPosition (Vector3)

- `Preview` <= [13] Absolute World Position (Excluding Material Offsets) output 0

### [14] Lerp

- `A` <= [20] Texture Sample output 1
- `B` <= [18] Texture Sample output 1
- `Alpha` <= [24] Mask ( R ) output 0

### [16] Abs

- `Input` <= [17] Mask ( R ) output 0

### [17] Mask ( R )

- `Input` <= [5] Input World Space Normal (Vector3) output 0

### [18] Texture Sample

- `Coordinates` <= [19] Mask ( G B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [19] Mask ( G B )

- `Input` <= [10] Divide output 0

### [20] Texture Sample

- `Coordinates` <= [21] Mask ( R B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [21] Mask ( R B )

- `Input` <= [10] Divide output 0

### [22] Output XY Texture (R)

- `A` <= [14] Lerp output 0

### [23] Output Z Texture (R)

- `A` <= [18] Texture Sample output 1

### [24] Mask ( R )

- `Input` <= [15] CheapContrast output 0

### [25] Mask ( R )

- `Input` <= [2] CheapContrast output 0

### [28] Abs

- `Input` <= [11] Input TextureSize (R) (Vector3) output 0

### [29] Multiply(,-1)

- `A` <= [28] Abs output 0

### [30] Output XYZ Texture (G)

- `A` <= [31] Lerp output 0

### [31] Lerp

- `A` <= [36] Lerp output 0
- `B` <= [32] Texture Sample output 2
- `Alpha` <= [25] Mask ( R ) output 0

### [32] Texture Sample

- `Coordinates` <= [33] Mask ( R G ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [33] Mask ( R G )

- `Input` <= [34] Divide output 0

### [34] Divide

- `A` <= [12] Input WorldPosition (Vector3) output 0
- `B` <= [44] Multiply(,-1) output 0

### [36] Lerp

- `A` <= [39] Texture Sample output 2
- `B` <= [37] Texture Sample output 2
- `Alpha` <= [24] Mask ( R ) output 0

### [37] Texture Sample

- `Coordinates` <= [38] Mask ( G B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [38] Mask ( G B )

- `Input` <= [34] Divide output 0

### [39] Texture Sample

- `Coordinates` <= [40] Mask ( R B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [40] Mask ( R B )

- `Input` <= [34] Divide output 0

### [41] Output XY Texture (G)

- `A` <= [36] Lerp output 0

### [42] Output Z Texture (G)

- `A` <= [37] Texture Sample output 2

### [43] Abs

- `Input` <= [35] Input TextureSize (G) (Vector3) output 0

### [44] Multiply(,-1)

- `A` <= [43] Abs output 0

### [45] Output XYZ Texture (B)

- `A` <= [46] Lerp output 0

### [46] Lerp

- `A` <= [51] Lerp output 0
- `B` <= [47] Texture Sample output 3
- `Alpha` <= [25] Mask ( R ) output 0

### [47] Texture Sample

- `Coordinates` <= [48] Mask ( R G ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [48] Mask ( R G )

- `Input` <= [49] Divide output 0

### [49] Divide

- `A` <= [12] Input WorldPosition (Vector3) output 0
- `B` <= [59] Multiply(,-1) output 0

### [51] Lerp

- `A` <= [54] Texture Sample output 3
- `B` <= [52] Texture Sample output 3
- `Alpha` <= [24] Mask ( R ) output 0

### [52] Texture Sample

- `Coordinates` <= [53] Mask ( G B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [53] Mask ( G B )

- `Input` <= [49] Divide output 0

### [54] Texture Sample

- `Coordinates` <= [55] Mask ( R B ) output 0
- `TextureObject` <= [7] Input TextureObject (Texture2D) output 0

### [55] Mask ( R B )

- `Input` <= [49] Divide output 0

### [56] Output XY Texture (B)

- `A` <= [51] Lerp output 0

### [57] Output Z Texture (B)

- `A` <= [52] Texture Sample output 3

### [58] Abs

- `Input` <= [50] Input TextureSize (B) (Vector3) output 0

### [59] Multiply(,-1)

- `A` <= [58] Abs output 0

## Connection List

- [1] Lerp output 0 -> Output XYZ Texture (R).A
- [14] Lerp output 0 -> Lerp.A
- [6] Texture Sample output 1 -> Lerp.B
- [25] Mask ( R ) output 0 -> Lerp.Alpha
- [4] Mask ( B ) output 0 -> Abs.Input
- [5] Input World Space Normal (Vector3) output 0 -> Mask ( B ).Input
- [27] VertexNormalWS output 0 -> Input World Space Normal (Vector3).Preview
- [9] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [10] Divide output 0 -> Mask ( R G ).Input
- [12] Input WorldPosition (Vector3) output 0 -> Divide.A
- [29] Multiply(,-1) output 0 -> Divide.B
- [13] Absolute World Position (Excluding Material Offsets) output 0 -> Input WorldPosition (Vector3).Preview
- [20] Texture Sample output 1 -> Lerp.A
- [18] Texture Sample output 1 -> Lerp.B
- [24] Mask ( R ) output 0 -> Lerp.Alpha
- [17] Mask ( R ) output 0 -> Abs.Input
- [5] Input World Space Normal (Vector3) output 0 -> Mask ( R ).Input
- [19] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [10] Divide output 0 -> Mask ( G B ).Input
- [21] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [10] Divide output 0 -> Mask ( R B ).Input
- [14] Lerp output 0 -> Output XY Texture (R).A
- [18] Texture Sample output 1 -> Output Z Texture (R).A
- [15] CheapContrast output 0 -> Mask ( R ).Input
- [2] CheapContrast output 0 -> Mask ( R ).Input
- [11] Input TextureSize (R) (Vector3) output 0 -> Abs.Input
- [28] Abs output 0 -> Multiply(,-1).A
- [31] Lerp output 0 -> Output XYZ Texture (G).A
- [36] Lerp output 0 -> Lerp.A
- [32] Texture Sample output 2 -> Lerp.B
- [25] Mask ( R ) output 0 -> Lerp.Alpha
- [33] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [34] Divide output 0 -> Mask ( R G ).Input
- [12] Input WorldPosition (Vector3) output 0 -> Divide.A
- [44] Multiply(,-1) output 0 -> Divide.B
- [39] Texture Sample output 2 -> Lerp.A
- [37] Texture Sample output 2 -> Lerp.B
- [24] Mask ( R ) output 0 -> Lerp.Alpha
- [38] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [34] Divide output 0 -> Mask ( G B ).Input
- [40] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [34] Divide output 0 -> Mask ( R B ).Input
- [36] Lerp output 0 -> Output XY Texture (G).A
- [37] Texture Sample output 2 -> Output Z Texture (G).A
- [35] Input TextureSize (G) (Vector3) output 0 -> Abs.Input
- [43] Abs output 0 -> Multiply(,-1).A
- [46] Lerp output 0 -> Output XYZ Texture (B).A
- [51] Lerp output 0 -> Lerp.A
- [47] Texture Sample output 3 -> Lerp.B
- [25] Mask ( R ) output 0 -> Lerp.Alpha
- [48] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [49] Divide output 0 -> Mask ( R G ).Input
- [12] Input WorldPosition (Vector3) output 0 -> Divide.A
- [59] Multiply(,-1) output 0 -> Divide.B
- [54] Texture Sample output 3 -> Lerp.A
- [52] Texture Sample output 3 -> Lerp.B
- [24] Mask ( R ) output 0 -> Lerp.Alpha
- [53] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [49] Divide output 0 -> Mask ( G B ).Input
- [55] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [7] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [49] Divide output 0 -> Mask ( R B ).Input
- [51] Lerp output 0 -> Output XY Texture (B).A
- [52] Texture Sample output 3 -> Output Z Texture (B).A
- [50] Input TextureSize (B) (Vector3) output 0 -> Abs.Input
- [58] Abs output 0 -> Multiply(,-1).A

## Output Trace Roots

- `XYZ Texture (R)` is driven by [1] Lerp
- `XY Texture (R)` is driven by [14] Lerp
- `Z Texture (R)` is driven by [18] Texture Sample
- `XYZ Texture (G)` is driven by [31] Lerp
- `XY Texture (G)` is driven by [36] Lerp
- `Z Texture (G)` is driven by [37] Texture Sample
- `XYZ Texture (B)` is driven by [46] Lerp
- `XY Texture (B)` is driven by [51] Lerp
- `Z Texture (B)` is driven by [52] Texture Sample

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


