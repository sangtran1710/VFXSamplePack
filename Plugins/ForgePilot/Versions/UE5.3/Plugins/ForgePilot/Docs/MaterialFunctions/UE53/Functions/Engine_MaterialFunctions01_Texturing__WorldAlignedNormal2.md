# WorldAlignedNormal2

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal2.WorldAlignedNormal2`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Tiles a texture in worldspace

## Inputs

- `Normal` [FunctionInput_Vector3]
- `TextureObject` [FunctionInput_Texture2D]
- `TextureSize` [FunctionInput_Vector3] - Size of texture in worldspace units

## Outputs

- `Z Texture`
- `XY Texture`
- `XYZ Texture`
- `XYZFlatTop`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Z Texture | Z Texture |
| 1 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 7 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 9 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 11 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 12 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (Vector3) | TextureSize |
| 16 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionConstant3Vector` | -1,-1,1 |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 22 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture | XY Texture |
| 24 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 27 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 28 | expression | `MaterialExpressionAbs` | Abs |  |
| 29 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 30 | expression | `MaterialExpressionAppendVector` | Append |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 33 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 34 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 35 | expression | `MaterialExpressionAppendVector` | Append |  |
| 36 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 37 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 38 | expression | `MaterialExpressionAppendVector` | Append |  |
| 39 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 41 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 42 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 43 | expression | `MaterialExpressionAppendVector` | Append |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 45 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 46 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 47 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture | XYZ Texture |
| 48 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 49 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 50 | expression | `MaterialExpressionAdd` | Add |  |
| 51 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 52 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 53 | expression | `MaterialExpressionAbs` | Abs |  |
| 54 | function_output | `MaterialExpressionFunctionOutput` | Output XYZFlatTop | XYZFlatTop |
| 55 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Z Texture

- `A` <= [1] World Space to Tangent Space TransformVector output 0

### [1] World Space to Tangent Space TransformVector

- `Input` <= [2] Add output 0

### [2] Add

- `A` <= [20] Multiply output 0
- `B` <= [3] Normalize output 0

### [3] Normalize

- `VectorInput` <= [4] Append output 0

### [4] Append

- `A` <= [17] Mask ( R G ) output 0
- `B` <= [5] Multiply output 0

### [5] Multiply

- `A` <= [9] Texture Sample output 3
- `B` <= [6] Mask ( B ) output 0

### [6] Mask ( B )

- `Input` <= [7] Tangent Space to World Space TransformVector output 0

### [7] Tangent Space to World Space TransformVector

- `Input` <= [8] Input Normal (Vector3) output 0

### [9] Texture Sample

- `Coordinates` <= [12] Mask ( R G ) output 0
- `TextureObject` <= [10] Input TextureObject (Texture2D) output 0

### [10] Input TextureObject (Texture2D)

- `Preview` <= [11] Texture Object output 0

### [12] Mask ( R G )

- `Input` <= [13] Divide output 0

### [13] Divide

- `A` <= [16] Absolute World Position (Excluding Material Offsets) output 0
- `B` <= [14] Multiply(,-1) output 0

### [14] Multiply(,-1)

- `A` <= [15] Input TextureSize (Vector3) output 0

### [17] Mask ( R G )

- `Input` <= [18] Multiply output 0

### [18] Multiply

- `A` <= [9] Texture Sample output 0
- `B` <= [19] -1,-1,1 output 0

### [20] Multiply

- `A` <= [22] VertexNormalWS output 0
- `B` <= [21] 0.5 output 0

### [23] Output XY Texture

- `A` <= [24] World Space to Tangent Space TransformVector output 0

### [24] World Space to Tangent Space TransformVector

- `Input` <= [25] Add output 0

### [25] Add

- `A` <= [20] Multiply output 0
- `B` <= [26] Normalize output 0

### [26] Normalize

- `VectorInput` <= [27] Lerp output 0

### [27] Lerp

- `A` <= [38] Append output 0
- `B` <= [30] Append output 0
- `Alpha` <= [28] Abs output 0

### [28] Abs

- `Input` <= [29] Mask ( R ) output 0

### [29] Mask ( R )

- `Input` <= [7] Tangent Space to World Space TransformVector output 0

### [30] Append

- `A` <= [35] Append output 0
- `B` <= [31] Mask ( G ) output 0

### [31] Mask ( G )

- `Input` <= [32] Multiply(,-1) output 0

### [32] Multiply(,-1)

- `A` <= [33] Texture Sample output 0

### [33] Texture Sample

- `Coordinates` <= [34] Mask ( G B ) output 0
- `TextureObject` <= [10] Input TextureObject (Texture2D) output 0

### [34] Mask ( G B )

- `Input` <= [13] Divide output 0

### [35] Append

- `A` <= [37] Multiply output 0
- `B` <= [36] Mask ( R ) output 0

### [36] Mask ( R )

- `Input` <= [32] Multiply(,-1) output 0

### [37] Multiply

- `A` <= [29] Mask ( R ) output 0
- `B` <= [33] Texture Sample output 3

### [38] Append

- `A` <= [43] Append output 0
- `B` <= [39] Mask ( G ) output 0

### [39] Mask ( G )

- `Input` <= [40] Multiply(,-1) output 0

### [40] Multiply(,-1)

- `A` <= [41] Texture Sample output 0

### [41] Texture Sample

- `Coordinates` <= [42] Mask ( R B ) output 0
- `TextureObject` <= [10] Input TextureObject (Texture2D) output 0

### [42] Mask ( R B )

- `Input` <= [13] Divide output 0

### [43] Append

- `A` <= [46] Mask ( R ) output 0
- `B` <= [44] Multiply output 0

### [44] Multiply

- `A` <= [41] Texture Sample output 3
- `B` <= [45] Mask ( G ) output 0

### [45] Mask ( G )

- `Input` <= [7] Tangent Space to World Space TransformVector output 0

### [46] Mask ( R )

- `Input` <= [40] Multiply(,-1) output 0

### [47] Output XYZ Texture

- `A` <= [48] World Space to Tangent Space TransformVector output 0

### [48] World Space to Tangent Space TransformVector

- `Input` <= [49] Normalize output 0

### [49] Normalize

- `VectorInput` <= [50] Add output 0

### [50] Add

- `A` <= [20] Multiply output 0
- `B` <= [51] Normalize output 0

### [51] Normalize

- `VectorInput` <= [52] Lerp output 0

### [52] Lerp

- `A` <= [27] Lerp output 0
- `B` <= [4] Append output 0
- `Alpha` <= [53] Abs output 0

### [53] Abs

- `Input` <= [6] Mask ( B ) output 0

### [54] Output XYZFlatTop

- `A` <= [55] Normalize output 0

### [55] Normalize

- `VectorInput` <= [48] World Space to Tangent Space TransformVector output 0

## Connection List

- [1] World Space to Tangent Space TransformVector output 0 -> Output Z Texture.A
- [2] Add output 0 -> World Space to Tangent Space TransformVector.Input
- [20] Multiply output 0 -> Add.A
- [3] Normalize output 0 -> Add.B
- [4] Append output 0 -> Normalize.VectorInput
- [17] Mask ( R G ) output 0 -> Append.A
- [5] Multiply output 0 -> Append.B
- [9] Texture Sample output 3 -> Multiply.A
- [6] Mask ( B ) output 0 -> Multiply.B
- [7] Tangent Space to World Space TransformVector output 0 -> Mask ( B ).Input
- [8] Input Normal (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [12] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [10] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [11] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [13] Divide output 0 -> Mask ( R G ).Input
- [16] Absolute World Position (Excluding Material Offsets) output 0 -> Divide.A
- [14] Multiply(,-1) output 0 -> Divide.B
- [15] Input TextureSize (Vector3) output 0 -> Multiply(,-1).A
- [18] Multiply output 0 -> Mask ( R G ).Input
- [9] Texture Sample output 0 -> Multiply.A
- [19] -1,-1,1 output 0 -> Multiply.B
- [22] VertexNormalWS output 0 -> Multiply.A
- [21] 0.5 output 0 -> Multiply.B
- [24] World Space to Tangent Space TransformVector output 0 -> Output XY Texture.A
- [25] Add output 0 -> World Space to Tangent Space TransformVector.Input
- [20] Multiply output 0 -> Add.A
- [26] Normalize output 0 -> Add.B
- [27] Lerp output 0 -> Normalize.VectorInput
- [38] Append output 0 -> Lerp.A
- [30] Append output 0 -> Lerp.B
- [28] Abs output 0 -> Lerp.Alpha
- [29] Mask ( R ) output 0 -> Abs.Input
- [7] Tangent Space to World Space TransformVector output 0 -> Mask ( R ).Input
- [35] Append output 0 -> Append.A
- [31] Mask ( G ) output 0 -> Append.B
- [32] Multiply(,-1) output 0 -> Mask ( G ).Input
- [33] Texture Sample output 0 -> Multiply(,-1).A
- [34] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [10] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [13] Divide output 0 -> Mask ( G B ).Input
- [37] Multiply output 0 -> Append.A
- [36] Mask ( R ) output 0 -> Append.B
- [32] Multiply(,-1) output 0 -> Mask ( R ).Input
- [29] Mask ( R ) output 0 -> Multiply.A
- [33] Texture Sample output 3 -> Multiply.B
- [43] Append output 0 -> Append.A
- [39] Mask ( G ) output 0 -> Append.B
- [40] Multiply(,-1) output 0 -> Mask ( G ).Input
- [41] Texture Sample output 0 -> Multiply(,-1).A
- [42] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [10] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [13] Divide output 0 -> Mask ( R B ).Input
- [46] Mask ( R ) output 0 -> Append.A
- [44] Multiply output 0 -> Append.B
- [41] Texture Sample output 3 -> Multiply.A
- [45] Mask ( G ) output 0 -> Multiply.B
- [7] Tangent Space to World Space TransformVector output 0 -> Mask ( G ).Input
- [40] Multiply(,-1) output 0 -> Mask ( R ).Input
- [48] World Space to Tangent Space TransformVector output 0 -> Output XYZ Texture.A
- [49] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [50] Add output 0 -> Normalize.VectorInput
- [20] Multiply output 0 -> Add.A
- [51] Normalize output 0 -> Add.B
- [52] Lerp output 0 -> Normalize.VectorInput
- [27] Lerp output 0 -> Lerp.A
- [4] Append output 0 -> Lerp.B
- [53] Abs output 0 -> Lerp.Alpha
- [6] Mask ( B ) output 0 -> Abs.Input
- [55] Normalize output 0 -> Output XYZFlatTop.A
- [48] World Space to Tangent Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Z Texture` is driven by [1] World Space to Tangent Space TransformVector
- `XY Texture` is driven by [24] World Space to Tangent Space TransformVector
- `XYZ Texture` is driven by [48] World Space to Tangent Space TransformVector
- `XYZFlatTop` is driven by [55] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
