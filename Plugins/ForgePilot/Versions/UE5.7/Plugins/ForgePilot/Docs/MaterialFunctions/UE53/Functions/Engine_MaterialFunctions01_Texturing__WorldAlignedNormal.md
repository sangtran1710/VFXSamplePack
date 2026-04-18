# WorldAlignedNormal

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal.WorldAlignedNormal`
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
- `WorldPosition` [FunctionInput_Vector3]
- `WorldSpace` [FunctionInput_StaticBool] - This switch defaults to false.
- `Use High Quality Normals` [FunctionInput_StaticBool] - Default is true. Adds ~ 20 instructions.
- `ProjectionTransitionContrast` [FunctionInput_Scalar]

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
| 2 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 3 | expression | `MaterialExpressionAppendVector` | Append |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionConstant` | 1 |  |
| 8 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 10 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 12 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 14 | expression | `MaterialExpressionDivide` | Divide |  |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (Vector3) | TextureSize |
| 16 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output XY Texture | XY Texture |
| 19 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 20 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 21 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 23 | expression | `MaterialExpressionConstant` | 0 |  |
| 24 | expression | `MaterialExpressionAbs` | Abs |  |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 26 | expression | `MaterialExpressionAppendVector` | Append |  |
| 27 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 28 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 29 | expression | `MaterialExpressionAppendVector` | Append |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 31 | expression | `MaterialExpressionAppendVector` | Append |  |
| 32 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 33 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 34 | expression | `MaterialExpressionAppendVector` | Append |  |
| 35 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 36 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 37 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Texture | XYZ Texture |
| 38 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 39 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 40 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 41 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 42 | expression | `MaterialExpressionAbs` | Abs |  |
| 43 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 44 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 45 | function_output | `MaterialExpressionFunctionOutput` | Output XYZFlatTop | XYZFlatTop |
| 46 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 47 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 48 | expression | `MaterialExpressionConstant3Vector` | -1,-1,1 |  |
| 49 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 50 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 51 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 52 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 53 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 54 | function_input | `MaterialExpressionFunctionInput` | Input WorldSpace (StaticBool) | WorldSpace |
| 55 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 56 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 57 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 58 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 59 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 60 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 61 | expression | `MaterialExpressionAbs` | Abs |  |
| 62 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldAlignedNormals_HighQuality |  |
| 63 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 64 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 65 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 66 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 67 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 68 | function_input | `MaterialExpressionFunctionInput` | Input Use High Quality Normals (StaticBool) | Use High Quality Normals |
| 69 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 70 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 71 | function_input | `MaterialExpressionFunctionInput` | Input ProjectionTransitionContrast (Scalar) | ProjectionTransitionContrast |

## Exact Input Wiring

### [0] Output Z Texture

- `A` <= [59] Switch output 0

### [1] World Space to Tangent Space TransformVector

- `Input` <= [64] Switch output 0

### [2] Normalize

- `VectorInput` <= [3] Append output 0

### [3] Append

- `A` <= [17] Mask ( R G ) output 0
- `B` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [10] Texture Sample output 3
- `B` <= [5] Mask ( B ) output 0

### [5] Mask ( B )

- `Input` <= [6] Multiply output 0

### [6] Multiply

- `A` <= [69] Reroute Node (reroutes wires) output 0
- `B` <= [7] 1 output 0

### [8] Tangent Space to World Space TransformVector

- `Input` <= [9] Input Normal (Vector3) output 0

### [10] Texture Sample

- `Coordinates` <= [13] Mask ( R G ) output 0
- `TextureObject` <= [11] Input TextureObject (Texture2D) output 0

### [11] Input TextureObject (Texture2D)

- `Preview` <= [12] Texture Object output 0

### [13] Mask ( R G )

- `Input` <= [14] Divide output 0

### [14] Divide

- `A` <= [53] Input WorldPosition (Vector3) output 0
- `B` <= [46] Multiply(,-1) output 0

### [17] Mask ( R G )

- `Input` <= [47] Multiply output 0

### [18] Output XY Texture

- `A` <= [58] Switch output 0

### [19] World Space to Tangent Space TransformVector

- `Input` <= [65] Switch output 0

### [20] Normalize

- `VectorInput` <= [21] Lerp output 0

### [21] Lerp

- `A` <= [31] Append output 0
- `B` <= [26] Append output 0
- `Alpha` <= [22] CheapContrast output 0

### [24] Abs

- `Input` <= [25] Mask ( R ) output 0

### [25] Mask ( R )

- `Input` <= [6] Multiply output 0

### [26] Append

- `A` <= [29] Append output 0
- `B` <= [49] Multiply(,-1) output 0

### [27] Texture Sample

- `Coordinates` <= [28] Mask ( G B ) output 0
- `TextureObject` <= [11] Input TextureObject (Texture2D) output 0

### [28] Mask ( G B )

- `Input` <= [14] Divide output 0

### [29] Append

- `A` <= [30] Multiply output 0
- `B` <= [50] Multiply(,-1) output 0

### [30] Multiply

- `A` <= [25] Mask ( R ) output 0
- `B` <= [27] Texture Sample output 3

### [31] Append

- `A` <= [34] Append output 0
- `B` <= [52] Multiply(,-1) output 0

### [32] Texture Sample

- `Coordinates` <= [33] Mask ( R B ) output 0
- `TextureObject` <= [11] Input TextureObject (Texture2D) output 0

### [33] Mask ( R B )

- `Input` <= [14] Divide output 0

### [34] Append

- `A` <= [51] Multiply(,-1) output 0
- `B` <= [35] Multiply output 0

### [35] Multiply

- `A` <= [32] Texture Sample output 3
- `B` <= [36] Mask ( G ) output 0

### [36] Mask ( G )

- `Input` <= [6] Multiply output 0

### [37] Output XYZ Texture

- `A` <= [55] Switch output 0

### [38] World Space to Tangent Space TransformVector

- `Input` <= [67] Switch output 0

### [39] Normalize

- `VectorInput` <= [40] Lerp output 0

### [40] Lerp

- `A` <= [21] Lerp output 0
- `B` <= [3] Append output 0
- `Alpha` <= [41] CheapContrast output 0

### [42] Abs

- `Input` <= [5] Mask ( B ) output 0

### [43] Lerp

- `A` <= [21] Lerp output 0
- `B` <= [70] Reroute Node (reroutes wires) output 0
- `Alpha` <= [41] CheapContrast output 0

### [44] World Space to Tangent Space TransformVector

- `Input` <= [66] Switch output 0

### [45] Output XYZFlatTop

- `A` <= [57] Switch output 0

### [46] Multiply(,-1)

- `A` <= [61] Abs output 0

### [47] Multiply

- `A` <= [10] Texture Sample output 0
- `B` <= [48] -1,-1,1 output 0

### [49] Multiply(,-1)

- `A` <= [27] Texture Sample output 2

### [50] Multiply(,-1)

- `A` <= [27] Texture Sample output 1

### [51] Multiply(,-1)

- `A` <= [32] Texture Sample output 1

### [52] Multiply(,-1)

- `A` <= [32] Texture Sample output 2

### [53] Input WorldPosition (Vector3)

- `Preview` <= [16] Absolute World Position (Excluding Material Offsets) output 0

### [54] Input WorldSpace (StaticBool)

- `Preview` <= [56] Static Bool (False) output 0

### [55] Switch

- `A` <= [67] Switch output 0
- `B` <= [38] World Space to Tangent Space TransformVector output 0
- `Value` <= [54] Input WorldSpace (StaticBool) output 0

### [57] Switch

- `A` <= [66] Switch output 0
- `B` <= [44] World Space to Tangent Space TransformVector output 0
- `Value` <= [54] Input WorldSpace (StaticBool) output 0

### [58] Switch

- `A` <= [65] Switch output 0
- `B` <= [19] World Space to Tangent Space TransformVector output 0
- `Value` <= [54] Input WorldSpace (StaticBool) output 0

### [59] Switch

- `A` <= [64] Switch output 0
- `B` <= [1] World Space to Tangent Space TransformVector output 0
- `Value` <= [54] Input WorldSpace (StaticBool) output 0

### [60] Normalize

- `VectorInput` <= [43] Lerp output 0

### [61] Abs

- `Input` <= [15] Input TextureSize (Vector3) output 0

### [64] Switch

- `A` <= [62] WorldAlignedNormals_HighQuality output 3
- `B` <= [2] Normalize output 0
- `Value` <= [68] Input Use High Quality Normals (StaticBool) output 0

### [65] Switch

- `A` <= [62] WorldAlignedNormals_HighQuality output 2
- `B` <= [20] Normalize output 0
- `Value` <= [68] Input Use High Quality Normals (StaticBool) output 0

### [66] Switch

- `A` <= [62] WorldAlignedNormals_HighQuality output 1
- `B` <= [60] Normalize output 0
- `Value` <= [68] Input Use High Quality Normals (StaticBool) output 0

### [67] Switch

- `A` <= [62] WorldAlignedNormals_HighQuality output 0
- `B` <= [39] Normalize output 0
- `Value` <= [68] Input Use High Quality Normals (StaticBool) output 0

### [68] Input Use High Quality Normals (StaticBool)

- `Preview` <= [63] Static Bool (True) output 0

### [69] Reroute Node (reroutes wires)

- `Input` <= [8] Tangent Space to World Space TransformVector output 0

### [70] Reroute Node (reroutes wires)

- `Input` <= [8] Tangent Space to World Space TransformVector output 0

## Connection List

- [59] Switch output 0 -> Output Z Texture.A
- [64] Switch output 0 -> World Space to Tangent Space TransformVector.Input
- [3] Append output 0 -> Normalize.VectorInput
- [17] Mask ( R G ) output 0 -> Append.A
- [4] Multiply output 0 -> Append.B
- [10] Texture Sample output 3 -> Multiply.A
- [5] Mask ( B ) output 0 -> Multiply.B
- [6] Multiply output 0 -> Mask ( B ).Input
- [69] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [7] 1 output 0 -> Multiply.B
- [9] Input Normal (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [13] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [11] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [12] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [14] Divide output 0 -> Mask ( R G ).Input
- [53] Input WorldPosition (Vector3) output 0 -> Divide.A
- [46] Multiply(,-1) output 0 -> Divide.B
- [47] Multiply output 0 -> Mask ( R G ).Input
- [58] Switch output 0 -> Output XY Texture.A
- [65] Switch output 0 -> World Space to Tangent Space TransformVector.Input
- [21] Lerp output 0 -> Normalize.VectorInput
- [31] Append output 0 -> Lerp.A
- [26] Append output 0 -> Lerp.B
- [22] CheapContrast output 0 -> Lerp.Alpha
- [25] Mask ( R ) output 0 -> Abs.Input
- [6] Multiply output 0 -> Mask ( R ).Input
- [29] Append output 0 -> Append.A
- [49] Multiply(,-1) output 0 -> Append.B
- [28] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [11] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [14] Divide output 0 -> Mask ( G B ).Input
- [30] Multiply output 0 -> Append.A
- [50] Multiply(,-1) output 0 -> Append.B
- [25] Mask ( R ) output 0 -> Multiply.A
- [27] Texture Sample output 3 -> Multiply.B
- [34] Append output 0 -> Append.A
- [52] Multiply(,-1) output 0 -> Append.B
- [33] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [11] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [14] Divide output 0 -> Mask ( R B ).Input
- [51] Multiply(,-1) output 0 -> Append.A
- [35] Multiply output 0 -> Append.B
- [32] Texture Sample output 3 -> Multiply.A
- [36] Mask ( G ) output 0 -> Multiply.B
- [6] Multiply output 0 -> Mask ( G ).Input
- [55] Switch output 0 -> Output XYZ Texture.A
- [67] Switch output 0 -> World Space to Tangent Space TransformVector.Input
- [40] Lerp output 0 -> Normalize.VectorInput
- [21] Lerp output 0 -> Lerp.A
- [3] Append output 0 -> Lerp.B
- [41] CheapContrast output 0 -> Lerp.Alpha
- [5] Mask ( B ) output 0 -> Abs.Input
- [21] Lerp output 0 -> Lerp.A
- [70] Reroute Node (reroutes wires) output 0 -> Lerp.B
- [41] CheapContrast output 0 -> Lerp.Alpha
- [66] Switch output 0 -> World Space to Tangent Space TransformVector.Input
- [57] Switch output 0 -> Output XYZFlatTop.A
- [61] Abs output 0 -> Multiply(,-1).A
- [10] Texture Sample output 0 -> Multiply.A
- [48] -1,-1,1 output 0 -> Multiply.B
- [27] Texture Sample output 2 -> Multiply(,-1).A
- [27] Texture Sample output 1 -> Multiply(,-1).A
- [32] Texture Sample output 1 -> Multiply(,-1).A
- [32] Texture Sample output 2 -> Multiply(,-1).A
- [16] Absolute World Position (Excluding Material Offsets) output 0 -> Input WorldPosition (Vector3).Preview
- [56] Static Bool (False) output 0 -> Input WorldSpace (StaticBool).Preview
- [67] Switch output 0 -> Switch.A
- [38] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [54] Input WorldSpace (StaticBool) output 0 -> Switch.Value
- [66] Switch output 0 -> Switch.A
- [44] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [54] Input WorldSpace (StaticBool) output 0 -> Switch.Value
- [65] Switch output 0 -> Switch.A
- [19] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [54] Input WorldSpace (StaticBool) output 0 -> Switch.Value
- [64] Switch output 0 -> Switch.A
- [1] World Space to Tangent Space TransformVector output 0 -> Switch.B
- [54] Input WorldSpace (StaticBool) output 0 -> Switch.Value
- [43] Lerp output 0 -> Normalize.VectorInput
- [15] Input TextureSize (Vector3) output 0 -> Abs.Input
- [62] WorldAlignedNormals_HighQuality output 3 -> Switch.A
- [2] Normalize output 0 -> Switch.B
- [68] Input Use High Quality Normals (StaticBool) output 0 -> Switch.Value
- [62] WorldAlignedNormals_HighQuality output 2 -> Switch.A
- [20] Normalize output 0 -> Switch.B
- [68] Input Use High Quality Normals (StaticBool) output 0 -> Switch.Value
- [62] WorldAlignedNormals_HighQuality output 1 -> Switch.A
- [60] Normalize output 0 -> Switch.B
- [68] Input Use High Quality Normals (StaticBool) output 0 -> Switch.Value
- [62] WorldAlignedNormals_HighQuality output 0 -> Switch.A
- [39] Normalize output 0 -> Switch.B
- [68] Input Use High Quality Normals (StaticBool) output 0 -> Switch.Value
- [63] Static Bool (True) output 0 -> Input Use High Quality Normals (StaticBool).Preview
- [8] Tangent Space to World Space TransformVector output 0 -> Reroute Node (reroutes wires).Input
- [8] Tangent Space to World Space TransformVector output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `Z Texture` is driven by [59] Switch
- `XY Texture` is driven by [58] Switch
- `XYZ Texture` is driven by [55] Switch
- `XYZFlatTop` is driven by [57] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
