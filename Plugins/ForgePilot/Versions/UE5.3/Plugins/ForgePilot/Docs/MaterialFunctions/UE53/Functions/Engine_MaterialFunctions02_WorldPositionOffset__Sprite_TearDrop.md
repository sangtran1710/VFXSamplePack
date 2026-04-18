# Sprite_TearDrop

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite_TearDrop.Sprite_TearDrop`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Stretch a particle out along the velocity vector when it's velocity is perpendicular to the view vector.

## Inputs

- `Min Tear Drop Width` [FunctionInput_Scalar] - The tail end of the sprite will shrink by this percentage when it's traveling perpendicular to the view and traveling at the maximum speed.
- `Max Stretch Factor` [FunctionInput_Scalar] - The tail end of the sprite will elongate by this percentage when it's traveling perpendicular to the view and traveling at the maximum speed.
- `Particle Position` [FunctionInput_Vector3]
- `------ Particle Defaults Below ------` [FunctionInput_StaticBool]
- `Particle Direction` [FunctionInput_Vector3]
- `Particle Size` [FunctionInput_Vector2]
- `Particle Speed` [FunctionInput_Scalar]
- `Minimum Speed` [FunctionInput_Scalar] - The particle must go this fast before beginning to warp along the velocity vector.
- `Max Speed` [FunctionInput_Scalar] - After reaching the minimum speed, the particle will start to warp until this speed is reached (in addition to the minimum speed).
- `UV Add` [FunctionInput_Vector2]

## Outputs

- `World Position Offset`
- `World Space Normals`
- `Opacity Mask`
- `Texture Coordinates`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 2 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 5 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 6 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 7 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 8 | expression | `MaterialExpressionTransform` | World Space to View Space TransformVector |  |
| 9 | expression | `MaterialExpressionTransform` | World Space to View Space TransformVector |  |
| 10 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 11 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 14 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 15 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 21 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 22 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 27 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 30 | expression | `MaterialExpressionAbs` | Abs |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 32 | expression | `MaterialExpressionConstant3Vector` | 1,1,0 |  |
| 33 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 34 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 35 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 36 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 37 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 38 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 39 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 40 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 41 | expression | `MaterialExpressionAppendVector` | Append |  |
| 42 | expression | `MaterialExpressionTransform` | World Space to View Space TransformVector |  |
| 43 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 45 | expression | `MaterialExpressionAdd` | Add |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 49 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 50 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 51 | expression | `MaterialExpressionConstant3Vector` | 0,0,-1 |  |
| 52 | expression | `MaterialExpressionDivide` | Divide |  |
| 53 | expression | `MaterialExpressionDivide` | Divide |  |
| 54 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 55 | expression | `MaterialExpressionDivide` | Divide |  |
| 56 | expression | `MaterialExpressionAppendVector` | Append |  |
| 57 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 58 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 59 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 60 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 61 | expression | `MaterialExpressionDivide` | Divide |  |
| 62 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 63 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 64 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 65 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normals | World Space Normals |
| 66 | function_output | `MaterialExpressionFunctionOutput` | Output Opacity Mask | Opacity Mask |
| 67 | function_input | `MaterialExpressionFunctionInput` | Input Min Tear Drop Width (Scalar) | Min Tear Drop Width |
| 68 | function_input | `MaterialExpressionFunctionInput` | Input Max Stretch Factor (Scalar) | Max Stretch Factor |
| 69 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 70 | function_input | `MaterialExpressionFunctionInput` | Input Particle Position (Vector3) | Particle Position |
| 71 | function_input | `MaterialExpressionFunctionInput` | Input ------ Particle Defaults Below ------ (StaticBool) | ------ Particle Defaults Below ------ |
| 72 | function_input | `MaterialExpressionFunctionInput` | Input Particle Direction (Vector3) | Particle Direction |
| 73 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 74 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 75 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 76 | function_input | `MaterialExpressionFunctionInput` | Input Particle Size (Vector2) | Particle Size |
| 77 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 78 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 79 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 80 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 81 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 82 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 83 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 84 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 85 | expression | `MaterialExpressionDivide` | Divide |  |
| 86 | expression | `MaterialExpressionClamp` | Clamp |  |
| 87 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 88 | function_input | `MaterialExpressionFunctionInput` | Input Particle Speed (Scalar) | Particle Speed |
| 89 | expression | `MaterialExpressionParticleSpeed` | Particle Speed |  |
| 90 | function_input | `MaterialExpressionFunctionInput` | Input Minimum Speed (Scalar) | Minimum Speed |
| 91 | function_input | `MaterialExpressionFunctionInput` | Input Max Speed (Scalar) | Max Speed |
| 92 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 93 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 94 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 95 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 96 | function_output | `MaterialExpressionFunctionOutput` | Output Texture Coordinates | Texture Coordinates |
| 97 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 98 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 99 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 100 | function_input | `MaterialExpressionFunctionInput` | Input UV Add (Vector2) | UV Add |
| 101 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [23] Add output 0

### [1] Cross

- `A` <= [74] Reroute Node (reroutes wires) output 0
- `B` <= [73] Reroute Node (reroutes wires) output 0

### [3] Subtract

- `A` <= [70] Input Particle Position (Vector3) output 0
- `B` <= [4] Camera Position output 0

### [5] Texture Sample

- `Coordinates` <= [96] Output Texture Coordinates output 0

### [6] Texture Sample

- `Coordinates` <= [96] Output Texture Coordinates output 0

### [7] Normalize

- `VectorInput` <= [3] Subtract output 0

### [8] World Space to View Space TransformVector

- `Input` <= [1] Cross output 0

### [9] World Space to View Space TransformVector

- `Input` <= [74] Reroute Node (reroutes wires) output 0

### [10] Cross

- `A` <= [33] Multiply output 0
- `B` <= [31] Multiply output 0

### [11] Cross

- `A` <= [31] Multiply output 0
- `B` <= [10] Cross output 0

### [12] Normalize

- `VectorInput` <= [11] Cross output 0

### [13] Normalize

- `VectorInput` <= [31] Multiply output 0

### [15] ConstantBiasScale

- `Input` <= [14] TexCoord[0] output 0

### [16] Multiply

- `A` <= [17] BreakOutFloat2Components output 0
- `B` <= [60] Multiply output 0

### [18] Multiply

- `A` <= [17] BreakOutFloat2Components output 1
- `B` <= [35] Multiply output 0

### [19] Add

- `A` <= [16] Multiply output 0
- `B` <= [18] Multiply output 0

### [20] View Space to World Space TransformVector

- `Input` <= [19] Add output 0

### [21] Subtract

- `A` <= [25] Multiply output 0
- `B` <= [22] Absolute World Position (Excluding Material Offsets) output 0

### [23] Add

- `A` <= [21] Subtract output 0
- `B` <= [81] Reroute Node (reroutes wires) output 0

### [25] Multiply

- `A` <= [20] View Space to World Space TransformVector output 0
- `B` <= [54] BreakOutFloat2Components output 0

### [28] Multiply

- `A` <= [5] Texture Sample output 1
- `B` <= [98] Reroute Node (reroutes wires) output 0

### [29] Dot

- `A` <= [83] SafeNormalize output 0
- `B` <= [7] Normalize output 0

### [30] Abs

- `Input` <= [29] Dot output 0

### [31] Multiply

- `A` <= [32] 1,1,0 output 0
- `B` <= [8] World Space to View Space TransformVector output 0

### [33] Multiply

- `A` <= [9] World Space to View Space TransformVector output 0
- `B` <= [32] 1,1,0 output 0

### [35] Multiply

- `A` <= [13] Normalize output 0
- `B` <= [58] Mask ( R ) output 0

### [36] Lerp

- `A` <= [50] Lerp output 0
- `B` <= [57] 1,1 output 0
- `Alpha` <= [34] BreakOutFloat2Components output 0

### [37] Dot

- `A` <= [13] Normalize output 0
- `B` <= [42] World Space to View Space TransformVector output 0

### [39] Subtract

- `A` <= [38] Absolute World Position output 0
- `B` <= [80] Reroute Node (reroutes wires) output 0

### [40] Dot

- `A` <= [12] Normalize output 0
- `B` <= [42] World Space to View Space TransformVector output 0

### [41] Append

- `A` <= [61] Divide output 0
- `B` <= [55] Divide output 0

### [42] World Space to View Space TransformVector

- `Input` <= [39] Subtract output 0

### [43] Add(,0.5)

- `A` <= [41] Append output 0

### [44] Multiply

- `A` <= [5] Texture Sample output 2
- `B` <= [97] Reroute Node (reroutes wires) output 0

### [45] Add

- `A` <= [28] Multiply output 0
- `B` <= [44] Multiply output 0

### [46] Multiply

- `A` <= [5] Texture Sample output 3
- `B` <= [51] 0,0,-1 output 0

### [47] Add

- `A` <= [45] Add output 0
- `B` <= [46] Multiply output 0

### [48] View Space to World Space TransformVector

- `Input` <= [47] Add output 0

### [49] Saturate

- `Input` <= [43] Add(,0.5) output 0

### [50] Lerp

- `A` <= [57] 1,1 output 0
- `B` <= [56] Append output 0
- `Alpha` <= [93] Multiply output 0

### [52] Divide

- `A` <= [40] Dot output 0
- `B` <= [27] BreakOutFloat2Components output 0

### [53] Divide

- `A` <= [37] Dot output 0
- `B` <= [27] BreakOutFloat2Components output 0

### [55] Divide

- `A` <= [53] Divide output 0
- `B` <= [58] Mask ( R ) output 0

### [56] Append

- `A` <= [67] Input Min Tear Drop Width (Scalar) output 0
- `B` <= [68] Input Max Stretch Factor (Scalar) output 0

### [58] Mask ( R )

- `Input` <= [36] Lerp output 0

### [59] Mask ( G )

- `Input` <= [36] Lerp output 0

### [60] Multiply

- `A` <= [12] Normalize output 0
- `B` <= [59] Mask ( G ) output 0

### [61] Divide

- `A` <= [52] Divide output 0
- `B` <= [59] Mask ( G ) output 0

### [62] Texture Sample

- `Coordinates` <= [96] Output Texture Coordinates output 0

### [63] 1-x

- `Input` <= [62] Texture Sample output 1

### [64] Multiply

- `A` <= [54] BreakOutFloat2Components output 0
- `B` <= [63] 1-x output 0

### [65] Output World Space Normals

- `A` <= [48] View Space to World Space TransformVector output 0

### [66] Output Opacity Mask

- `A` <= [6] Texture Sample output 1

### [70] Input Particle Position (Vector3)

- `Preview` <= [2] Particle Position (Absolute) output 0

### [72] Input Particle Direction (Vector3)

- `Preview` <= [69] Particle Direction output 0

### [73] Reroute Node (reroutes wires)

- `Input` <= [7] Normalize output 0

### [74] Reroute Node (reroutes wires)

- `Input` <= [83] SafeNormalize output 0

### [75] Reroute Node (reroutes wires)

- `Input` <= [70] Input Particle Position (Vector3) output 0

### [76] Input Particle Size (Vector2)

- `Preview` <= [26] Particle Size output 0

### [77] Reroute Node (reroutes wires)

- `Input` <= [76] Input Particle Size (Vector2) output 0

### [78] Reroute Node (reroutes wires)

- `Input` <= [76] Input Particle Size (Vector2) output 0

### [79] Reroute Node (reroutes wires)

- `Input` <= [78] Reroute Node (reroutes wires) output 0

### [80] Reroute Node (reroutes wires)

- `Input` <= [75] Reroute Node (reroutes wires) output 0

### [81] Reroute Node (reroutes wires)

- `Input` <= [82] Reroute Node (reroutes wires) output 0

### [82] Reroute Node (reroutes wires)

- `Input` <= [80] Reroute Node (reroutes wires) output 0

### [85] Divide

- `A` <= [87] Subtract output 0
- `B` <= [91] Input Max Speed (Scalar) output 0

### [86] Clamp

- `Input` <= [85] Divide output 0

### [87] Subtract

- `A` <= [88] Input Particle Speed (Scalar) output 0
- `B` <= [90] Input Minimum Speed (Scalar) output 0

### [88] Input Particle Speed (Scalar)

- `Preview` <= [89] Particle Speed output 0

### [92] 1-x

- `Input` <= [30] Abs output 0

### [93] Multiply

- `A` <= [92] 1-x output 0
- `B` <= [99] Multiply output 0

### [96] Output Texture Coordinates

- `A` <= [101] Add output 0

### [97] Reroute Node (reroutes wires)

- `Input` <= [13] Normalize output 0

### [98] Reroute Node (reroutes wires)

- `Input` <= [12] Normalize output 0

### [99] Multiply

- `A` <= [86] Clamp output 0
- `B` <= [86] Clamp output 0

### [101] Add

- `A` <= [49] Saturate output 0
- `B` <= [100] Input UV Add (Vector2) output 0

## Connection List

- [23] Add output 0 -> Output World Position Offset.A
- [74] Reroute Node (reroutes wires) output 0 -> Cross.A
- [73] Reroute Node (reroutes wires) output 0 -> Cross.B
- [70] Input Particle Position (Vector3) output 0 -> Subtract.A
- [4] Camera Position output 0 -> Subtract.B
- [96] Output Texture Coordinates output 0 -> Texture Sample.Coordinates
- [96] Output Texture Coordinates output 0 -> Texture Sample.Coordinates
- [3] Subtract output 0 -> Normalize.VectorInput
- [1] Cross output 0 -> World Space to View Space TransformVector.Input
- [74] Reroute Node (reroutes wires) output 0 -> World Space to View Space TransformVector.Input
- [33] Multiply output 0 -> Cross.A
- [31] Multiply output 0 -> Cross.B
- [31] Multiply output 0 -> Cross.A
- [10] Cross output 0 -> Cross.B
- [11] Cross output 0 -> Normalize.VectorInput
- [31] Multiply output 0 -> Normalize.VectorInput
- [14] TexCoord[0] output 0 -> ConstantBiasScale.Input
- [17] BreakOutFloat2Components output 0 -> Multiply.A
- [60] Multiply output 0 -> Multiply.B
- [17] BreakOutFloat2Components output 1 -> Multiply.A
- [35] Multiply output 0 -> Multiply.B
- [16] Multiply output 0 -> Add.A
- [18] Multiply output 0 -> Add.B
- [19] Add output 0 -> View Space to World Space TransformVector.Input
- [25] Multiply output 0 -> Subtract.A
- [22] Absolute World Position (Excluding Material Offsets) output 0 -> Subtract.B
- [21] Subtract output 0 -> Add.A
- [81] Reroute Node (reroutes wires) output 0 -> Add.B
- [20] View Space to World Space TransformVector output 0 -> Multiply.A
- [54] BreakOutFloat2Components output 0 -> Multiply.B
- [5] Texture Sample output 1 -> Multiply.A
- [98] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [83] SafeNormalize output 0 -> Dot.A
- [7] Normalize output 0 -> Dot.B
- [29] Dot output 0 -> Abs.Input
- [32] 1,1,0 output 0 -> Multiply.A
- [8] World Space to View Space TransformVector output 0 -> Multiply.B
- [9] World Space to View Space TransformVector output 0 -> Multiply.A
- [32] 1,1,0 output 0 -> Multiply.B
- [13] Normalize output 0 -> Multiply.A
- [58] Mask ( R ) output 0 -> Multiply.B
- [50] Lerp output 0 -> Lerp.A
- [57] 1,1 output 0 -> Lerp.B
- [34] BreakOutFloat2Components output 0 -> Lerp.Alpha
- [13] Normalize output 0 -> Dot.A
- [42] World Space to View Space TransformVector output 0 -> Dot.B
- [38] Absolute World Position output 0 -> Subtract.A
- [80] Reroute Node (reroutes wires) output 0 -> Subtract.B
- [12] Normalize output 0 -> Dot.A
- [42] World Space to View Space TransformVector output 0 -> Dot.B
- [61] Divide output 0 -> Append.A
- [55] Divide output 0 -> Append.B
- [39] Subtract output 0 -> World Space to View Space TransformVector.Input
- [41] Append output 0 -> Add(,0.5).A
- [5] Texture Sample output 2 -> Multiply.A
- [97] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [28] Multiply output 0 -> Add.A
- [44] Multiply output 0 -> Add.B
- [5] Texture Sample output 3 -> Multiply.A
- [51] 0,0,-1 output 0 -> Multiply.B
- [45] Add output 0 -> Add.A
- [46] Multiply output 0 -> Add.B
- [47] Add output 0 -> View Space to World Space TransformVector.Input
- [43] Add(,0.5) output 0 -> Saturate.Input
- [57] 1,1 output 0 -> Lerp.A
- [56] Append output 0 -> Lerp.B
- [93] Multiply output 0 -> Lerp.Alpha
- [40] Dot output 0 -> Divide.A
- [27] BreakOutFloat2Components output 0 -> Divide.B
- [37] Dot output 0 -> Divide.A
- [27] BreakOutFloat2Components output 0 -> Divide.B
- [53] Divide output 0 -> Divide.A
- [58] Mask ( R ) output 0 -> Divide.B
- [67] Input Min Tear Drop Width (Scalar) output 0 -> Append.A
- [68] Input Max Stretch Factor (Scalar) output 0 -> Append.B
- [36] Lerp output 0 -> Mask ( R ).Input
- [36] Lerp output 0 -> Mask ( G ).Input
- [12] Normalize output 0 -> Multiply.A
- [59] Mask ( G ) output 0 -> Multiply.B
- [52] Divide output 0 -> Divide.A
- [59] Mask ( G ) output 0 -> Divide.B
- [96] Output Texture Coordinates output 0 -> Texture Sample.Coordinates
- [62] Texture Sample output 1 -> 1-x.Input
- [54] BreakOutFloat2Components output 0 -> Multiply.A
- [63] 1-x output 0 -> Multiply.B
- [48] View Space to World Space TransformVector output 0 -> Output World Space Normals.A
- [6] Texture Sample output 1 -> Output Opacity Mask.A
- [2] Particle Position (Absolute) output 0 -> Input Particle Position (Vector3).Preview
- [69] Particle Direction output 0 -> Input Particle Direction (Vector3).Preview
- [7] Normalize output 0 -> Reroute Node (reroutes wires).Input
- [83] SafeNormalize output 0 -> Reroute Node (reroutes wires).Input
- [70] Input Particle Position (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [26] Particle Size output 0 -> Input Particle Size (Vector2).Preview
- [76] Input Particle Size (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [76] Input Particle Size (Vector2) output 0 -> Reroute Node (reroutes wires).Input
- [78] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [75] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [82] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [80] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [87] Subtract output 0 -> Divide.A
- [91] Input Max Speed (Scalar) output 0 -> Divide.B
- [85] Divide output 0 -> Clamp.Input
- [88] Input Particle Speed (Scalar) output 0 -> Subtract.A
- [90] Input Minimum Speed (Scalar) output 0 -> Subtract.B
- [89] Particle Speed output 0 -> Input Particle Speed (Scalar).Preview
- [30] Abs output 0 -> 1-x.Input
- [92] 1-x output 0 -> Multiply.A
- [99] Multiply output 0 -> Multiply.B
- [101] Add output 0 -> Output Texture Coordinates.A
- [13] Normalize output 0 -> Reroute Node (reroutes wires).Input
- [12] Normalize output 0 -> Reroute Node (reroutes wires).Input
- [86] Clamp output 0 -> Multiply.A
- [86] Clamp output 0 -> Multiply.B
- [49] Saturate output 0 -> Add.A
- [100] Input UV Add (Vector2) output 0 -> Add.B

## Output Trace Roots

- `World Position Offset` is driven by [23] Add
- `World Space Normals` is driven by [48] View Space to World Space TransformVector
- `Opacity Mask` is driven by [6] Texture Sample
- `Texture Coordinates` is driven by [101] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
