# WorldAlignedNormals_HighQuality

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldAlignedNormals_HighQuality.WorldAlignedNormals_HighQuality`
- Category: Engine_MaterialFunctions02
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Applies normal maps to objects and correctly orients them. See WorldAlignedTexture_Complex for a more thorough implementation.  This version of world aligned normals is ~20 instructions more. The Normals are in worldspace by default. Transform them from world to tangent. 

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Negative World Position Divisor` [FunctionInput_Vector3] - All inputs need to be negative. For example: (-512,-256,-1024)
- `World Position` [FunctionInput_Vector3]
- `X lerp alpha` [FunctionInput_Scalar]
- `Z lerp Alpha` [FunctionInput_Scalar]
- `WorldNormal` [FunctionInput_Vector3] - All inputs need to be negative. For example: (-512,-256,-1024)

## Outputs

- `World Space Normals XYZ`
- `Z Projection World Space Normals`
- `XY Projection World Space Normals`
- `XY Flat Top Projection World Space Normals`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normals XYZ | World Space Normals XYZ |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 8 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 10 | expression | `MaterialExpressionConstant` | -1 |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 12 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 13 | expression | `MaterialExpressionAbs` | Abs |  |
| 14 | expression | `MaterialExpressionConstant` | 1 |  |
| 15 | expression | `MaterialExpressionConstant` | 2 |  |
| 16 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 19 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 20 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 21 | expression | `MaterialExpressionDivide` | Divide |  |
| 22 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 23 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 27 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 28 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 29 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 30 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 31 | expression | `MaterialExpressionConstant` | -1 |  |
| 32 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 33 | expression | `MaterialExpressionConstant` | 1 |  |
| 34 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 35 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 36 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 37 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 38 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 39 | expression | `MaterialExpressionConstant` | 1 |  |
| 40 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 41 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 42 | function_input | `MaterialExpressionFunctionInput` | Input Negative World Position Divisor (Vector3) | Negative World Position Divisor |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 44 | expression | `MaterialExpressionConstant` | -1 |  |
| 45 | function_input | `MaterialExpressionFunctionInput` | Input X lerp alpha (Scalar) | X lerp alpha |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Z lerp Alpha (Scalar) | Z lerp Alpha |
| 47 | function_output | `MaterialExpressionFunctionOutput` | Output Z Projection World Space Normals | Z Projection World Space Normals |
| 48 | function_output | `MaterialExpressionFunctionOutput` | Output XY Projection World Space Normals | XY Projection World Space Normals |
| 49 | function_output | `MaterialExpressionFunctionOutput` | Output XY Flat Top Projection World Space Normals | XY Flat Top Projection World Space Normals |
| 50 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 51 | function_input | `MaterialExpressionFunctionInput` | Input WorldNormal (Vector3) | WorldNormal |
| 52 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 53 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 54 | expression | `MaterialExpressionAbs` | Abs |  |
| 55 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 56 | expression | `MaterialExpressionAppendVector` | Append |  |
| 57 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 58 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 59 | expression | `MaterialExpressionConstant` | 0 |  |
| 60 | expression | `MaterialExpressionIf` | If |  |
| 61 | expression | `MaterialExpressionConstant` | 1 |  |
| 62 | expression | `MaterialExpressionConstant` | -1 |  |
| 63 | expression | `MaterialExpressionConstant` | 0 |  |
| 64 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 65 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 66 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 67 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 68 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 69 | expression | `MaterialExpressionIf` | If |  |
| 70 | expression | `MaterialExpressionConstant` | 0 |  |
| 71 | expression | `MaterialExpressionConstant` | 1 |  |
| 72 | expression | `MaterialExpressionConstant` | -1 |  |
| 73 | expression | `MaterialExpressionConstant` | 1 |  |
| 74 | expression | `MaterialExpressionConstant` | 1 |  |
| 75 | expression | `MaterialExpressionConstant` | -1 |  |
| 76 | expression | `MaterialExpressionConstant` | 0 |  |
| 77 | expression | `MaterialExpressionIf` | If |  |
| 78 | expression | `MaterialExpressionConstant` | 1 |  |
| 79 | expression | `MaterialExpressionConstant` | -1 |  |
| 80 | expression | `MaterialExpressionConstant` | 0 |  |
| 81 | expression | `MaterialExpressionIf` | If |  |

## Exact Input Wiring

### [0] Output World Space Normals XYZ

- `A` <= [16] Lerp output 0

### [1] Texture Sample

- `Coordinates` <= [18] Mask ( G B ) output 0
- `TextureObject` <= [40] Input Texture (Texture2D) output 0

### [2] Multiply

- `A` <= [1] Texture Sample output 0
- `B` <= [7] MakeFloat3 output 0

### [3] Lerp

- `A` <= [25] Multiply output 0
- `B` <= [2] Multiply output 0
- `Alpha` <= [45] Input X lerp alpha (Scalar) output 0

### [4] Dot

- `A` <= [51] Input WorldNormal (Vector3) output 0
- `B` <= [8] 1,0,0 output 0

### [13] Abs

- `Input` <= [51] Input WorldNormal (Vector3) output 0

### [16] Lerp

- `A` <= [5] Transform3x3Matrix output 0
- `B` <= [24] Transform3x3Matrix output 0
- `Alpha` <= [46] Input Z lerp Alpha (Scalar) output 0

### [18] Mask ( G B )

- `Input` <= [21] Divide output 0

### [19] Texture Sample

- `Coordinates` <= [22] Mask ( R B ) output 0
- `TextureObject` <= [40] Input Texture (Texture2D) output 0

### [21] Divide

- `A` <= [43] Input World Position (Vector3) output 0
- `B` <= [42] Input Negative World Position Divisor (Vector3) output 0

### [22] Mask ( R B )

- `Input` <= [21] Divide output 0

### [23] Dot

- `A` <= [51] Input WorldNormal (Vector3) output 0
- `B` <= [30] 0,1,0 output 0

### [25] Multiply

- `A` <= [19] Texture Sample output 0
- `B` <= [29] MakeFloat3 output 0

### [26] Mask ( R G )

- `Input` <= [21] Divide output 0

### [34] Texture Sample

- `Coordinates` <= [26] Mask ( R G ) output 0
- `TextureObject` <= [40] Input Texture (Texture2D) output 0

### [35] Dot

- `A` <= [51] Input WorldNormal (Vector3) output 0
- `B` <= [38] 0,0,1 output 0

### [36] Multiply

- `A` <= [34] Texture Sample output 0
- `B` <= [37] MakeFloat3 output 0

### [40] Input Texture (Texture2D)

- `Preview` <= [41] Texture Object output 0

### [43] Input World Position (Vector3)

- `Preview` <= [20] Absolute World Position (Excluding Material Offsets) output 0

### [45] Input X lerp alpha (Scalar)

- `Preview` <= [9] CheapContrast output 0

### [46] Input Z lerp Alpha (Scalar)

- `Preview` <= [17] CheapContrast output 0

### [47] Output Z Projection World Space Normals

- `A` <= [24] Transform3x3Matrix output 0

### [48] Output XY Projection World Space Normals

- `A` <= [5] Transform3x3Matrix output 0

### [49] Output XY Flat Top Projection World Space Normals

- `A` <= [50] Lerp output 0

### [50] Lerp

- `A` <= [5] Transform3x3Matrix output 0
- `B` <= [51] Input WorldNormal (Vector3) output 0
- `Alpha` <= [46] Input Z lerp Alpha (Scalar) output 0

### [51] Input WorldNormal (Vector3)

- `Preview` <= [28] VertexNormalWS output 0

### [54] Abs

- `Input` <= [53] BreakOutFloat2Components output 0

### [55] ConstantBiasScale

- `Input` <= [54] Abs output 0

### [56] Append

- `A` <= [55] ConstantBiasScale output 0
- `B` <= [53] BreakOutFloat2Components output 1

### [57] DeriveNormalZ

- `InXY` <= [56] Append output 0

### [60] If

- `A` <= [53] BreakOutFloat2Components output 0
- `B` <= [63] 0 output 0
- `AGreaterThanB` <= [61] 1 output 0
- `AEqualsB` <= [61] 1 output 0
- `ALessThanB` <= [62] -1 output 0

### [64] Multiply

- `A` <= [58] MakeFloat3 output 0
- `B` <= [57] DeriveNormalZ output 0

### [69] If

- `A` <= [23] Dot output 0
- `B` <= [70] 0 output 0
- `AGreaterThanB` <= [72] -1 output 0
- `ALessThanB` <= [73] 1 output 0

### [77] If

- `A` <= [4] Dot output 0
- `B` <= [76] 0 output 0
- `AGreaterThanB` <= [74] 1 output 0
- `ALessThanB` <= [75] -1 output 0

### [81] If

- `A` <= [35] Dot output 0
- `B` <= [80] 0 output 0
- `AGreaterThanB` <= [78] 1 output 0
- `ALessThanB` <= [79] -1 output 0

## Connection List

- [16] Lerp output 0 -> Output World Space Normals XYZ.A
- [18] Mask ( G B ) output 0 -> Texture Sample.Coordinates
- [40] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [1] Texture Sample output 0 -> Multiply.A
- [7] MakeFloat3 output 0 -> Multiply.B
- [25] Multiply output 0 -> Lerp.A
- [2] Multiply output 0 -> Lerp.B
- [45] Input X lerp alpha (Scalar) output 0 -> Lerp.Alpha
- [51] Input WorldNormal (Vector3) output 0 -> Dot.A
- [8] 1,0,0 output 0 -> Dot.B
- [51] Input WorldNormal (Vector3) output 0 -> Abs.Input
- [5] Transform3x3Matrix output 0 -> Lerp.A
- [24] Transform3x3Matrix output 0 -> Lerp.B
- [46] Input Z lerp Alpha (Scalar) output 0 -> Lerp.Alpha
- [21] Divide output 0 -> Mask ( G B ).Input
- [22] Mask ( R B ) output 0 -> Texture Sample.Coordinates
- [40] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [43] Input World Position (Vector3) output 0 -> Divide.A
- [42] Input Negative World Position Divisor (Vector3) output 0 -> Divide.B
- [21] Divide output 0 -> Mask ( R B ).Input
- [51] Input WorldNormal (Vector3) output 0 -> Dot.A
- [30] 0,1,0 output 0 -> Dot.B
- [19] Texture Sample output 0 -> Multiply.A
- [29] MakeFloat3 output 0 -> Multiply.B
- [21] Divide output 0 -> Mask ( R G ).Input
- [26] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [40] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [51] Input WorldNormal (Vector3) output 0 -> Dot.A
- [38] 0,0,1 output 0 -> Dot.B
- [34] Texture Sample output 0 -> Multiply.A
- [37] MakeFloat3 output 0 -> Multiply.B
- [41] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [20] Absolute World Position (Excluding Material Offsets) output 0 -> Input World Position (Vector3).Preview
- [9] CheapContrast output 0 -> Input X lerp alpha (Scalar).Preview
- [17] CheapContrast output 0 -> Input Z lerp Alpha (Scalar).Preview
- [24] Transform3x3Matrix output 0 -> Output Z Projection World Space Normals.A
- [5] Transform3x3Matrix output 0 -> Output XY Projection World Space Normals.A
- [50] Lerp output 0 -> Output XY Flat Top Projection World Space Normals.A
- [5] Transform3x3Matrix output 0 -> Lerp.A
- [51] Input WorldNormal (Vector3) output 0 -> Lerp.B
- [46] Input Z lerp Alpha (Scalar) output 0 -> Lerp.Alpha
- [28] VertexNormalWS output 0 -> Input WorldNormal (Vector3).Preview
- [53] BreakOutFloat2Components output 0 -> Abs.Input
- [54] Abs output 0 -> ConstantBiasScale.Input
- [55] ConstantBiasScale output 0 -> Append.A
- [53] BreakOutFloat2Components output 1 -> Append.B
- [56] Append output 0 -> DeriveNormalZ.InXY
- [53] BreakOutFloat2Components output 0 -> If.A
- [63] 0 output 0 -> If.B
- [61] 1 output 0 -> If.AGreaterThanB
- [61] 1 output 0 -> If.AEqualsB
- [62] -1 output 0 -> If.ALessThanB
- [58] MakeFloat3 output 0 -> Multiply.A
- [57] DeriveNormalZ output 0 -> Multiply.B
- [23] Dot output 0 -> If.A
- [70] 0 output 0 -> If.B
- [72] -1 output 0 -> If.AGreaterThanB
- [73] 1 output 0 -> If.ALessThanB
- [4] Dot output 0 -> If.A
- [76] 0 output 0 -> If.B
- [74] 1 output 0 -> If.AGreaterThanB
- [75] -1 output 0 -> If.ALessThanB
- [35] Dot output 0 -> If.A
- [80] 0 output 0 -> If.B
- [78] 1 output 0 -> If.AGreaterThanB
- [79] -1 output 0 -> If.ALessThanB

## Output Trace Roots

- `World Space Normals XYZ` is driven by [16] Lerp
- `Z Projection World Space Normals` is driven by [24] Transform3x3Matrix
- `XY Projection World Space Normals` is driven by [5] Transform3x3Matrix
- `XY Flat Top Projection World Space Normals` is driven by [50] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


