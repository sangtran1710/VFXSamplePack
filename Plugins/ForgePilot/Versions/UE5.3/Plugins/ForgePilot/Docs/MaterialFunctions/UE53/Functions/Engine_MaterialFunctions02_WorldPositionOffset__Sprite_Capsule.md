# Sprite_Capsule

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite_Capsule.Sprite_Capsule`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The material function performs a number of functions to turn a particle sprite, or plane, into a velocity aligned capsule. 

## Inputs

- `World Position to Negate` [FunctionInput_Vector3] - World position must be subtracted from the World Position Offset output before being inserted into the World Position Offset pin. This input allows advanced users to override the world position value that's subtracted from the output.
- `Particle Position` [FunctionInput_Vector3]
- `Normalized Particle Direction` [FunctionInput_Vector3] - Note: the particle will disappear if the system is fed a particle direction of  (0,0,0).
- `Scale Along Velocity Vector` [FunctionInput_Scalar] - Note: By default, an equation using the "Scale Over Velocity Parameters" sets this value. Plugging an input into this field will allow one to manually control the scale factor. 
- `Capsule Diameter` [FunctionInput_Scalar]
- `---------Scale Over Velocity Parameters---------` [FunctionInput_StaticBool]
- `Particle Speed` [FunctionInput_Scalar]
- `Minimum Speed` [FunctionInput_Scalar] - The particle must go this fast before beginning to warp along the velocity vector.
- `Max Speed` [FunctionInput_Scalar] - After reaching the minimum speed, the particle will start to warp until this speed is reached (in addition to the minimum speed). This value uses the sprites size's y coordinate by default.
- `Minimum Scale Factor` [FunctionInput_Scalar]
- `Maximum Scale Factor` [FunctionInput_Scalar]
- `Use Advanced Features` [FunctionInput_StaticBool] - This option will better recreates a capsule with a slight increase in cost. It's enabled by default.
- `Capsule Pivot Alignment` [FunctionInput_Scalar] - 1 will align the leading edge of the capsule to the particle position, -1 will align the trailing edge.
- `Use Camera Offset` [FunctionInput_StaticBool] - This feature pulls the cards closer to the camera. It along with PDO can better integrate the psuedo capsules into the scene. This option defaults to false.
- `Cap Size Multiplier` [FunctionInput_Scalar]
- `---------- Advanced -----------` [FunctionInput_Vector3]

## Outputs

- `World Position Offset` - Note that the output of this material function subtracts world position from the results. This produces the correct results when plugged directly into world position offset.
- `World Space Normals` - These normals are in world space. Ensure that tangent space normals are unchecked in your material's details panel. 
- `Opacity Mask`
- `Texture Coordinates` - These uvs are distorted to better represent a 3D capsule.
- `Pixel Depth Offset`
- `Scale Factor` - This output can be used to modulate your particles emissive value etc.
- `Front Face Mask` - The forward most edge returns a 1 whereas the receding edge returns a 0. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input World Position to Negate (Vector3) | World Position to Negate |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Particle Position (Vector3) | Particle Position |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Normalized Particle Direction (Vector3) | Normalized Particle Direction |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Scale Along Velocity Vector (Scalar) | Scale Along Velocity Vector |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Capsule Diameter (Scalar) | Capsule Diameter |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normals | World Space Normals |
| 11 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Opacity Mask | Opacity Mask |
| 13 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input ---------Scale Over Velocity Parameters--------- (StaticBool) | ---------Scale Over Velocity Parameters--------- |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | StretchGradient |  |
| 16 | expression | `MaterialExpressionAppendVector` | Append |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Texture Coordinates | Texture Coordinates |
| 18 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 20 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 23 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 24 | expression | `MaterialExpressionConstant2Vector` | 0.5,0.5 |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionAdd` | Add |  |
| 27 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 28 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 29 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 31 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 33 | expression | `MaterialExpressionAdd` | Add |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 36 | expression | `MaterialExpressionAdd` | Add |  |
| 37 | expression | `MaterialExpressionDivide` | Divide |  |
| 38 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 39 | expression | `MaterialExpressionAbs` | Abs |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | expression | `MaterialExpressionClamp` | Clamp |  |
| 42 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 44 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 45 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 46 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 47 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 48 | expression | `MaterialExpressionDivide` | Divide |  |
| 49 | expression | `MaterialExpressionClamp` | Clamp |  |
| 50 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 51 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 52 | function_input | `MaterialExpressionFunctionInput` | Input Particle Speed (Scalar) | Particle Speed |
| 53 | expression | `MaterialExpressionParticleSpeed` | Particle Speed |  |
| 54 | function_input | `MaterialExpressionFunctionInput` | Input Minimum Speed (Scalar) | Minimum Speed |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input Max Speed (Scalar) | Max Speed |
| 56 | function_input | `MaterialExpressionFunctionInput` | Input Minimum Scale Factor (Scalar) | Minimum Scale Factor |
| 57 | function_input | `MaterialExpressionFunctionInput` | Input Maximum Scale Factor (Scalar) | Maximum Scale Factor |
| 58 | expression | `MaterialExpressionMax` | Max(,0.001) |  |
| 59 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 60 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 61 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 62 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 63 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 64 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 65 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 66 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 67 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 68 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 69 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 70 | expression | `MaterialExpressionConstant` | 0.3794 |  |
| 71 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 72 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 73 | expression | `MaterialExpressionConstant` | 0.3683 |  |
| 74 | function_input | `MaterialExpressionFunctionInput` | Input Use Advanced Features (StaticBool) | Use Advanced Features |
| 75 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 76 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 77 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 78 | function_input | `MaterialExpressionFunctionInput` | Input Capsule Pivot Alignment (Scalar) | Capsule Pivot Alignment |
| 79 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 80 | expression | `MaterialExpressionAdd` | Add |  |
| 81 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 82 | expression | `MaterialExpressionMultiply` | Multiply(,-0.5) |  |
| 83 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 84 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 85 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 86 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 87 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 88 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Depth Offset | Pixel Depth Offset |
| 89 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 90 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 91 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 92 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 93 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 94 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 95 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 96 | expression | `MaterialExpressionAdd` | Add |  |
| 97 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 98 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 99 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 100 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 101 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 102 | function_input | `MaterialExpressionFunctionInput` | Input Use Camera Offset (StaticBool) | Use Camera Offset |
| 103 | function_input | `MaterialExpressionFunctionInput` | Input Cap Size Multiplier (Scalar) | Cap Size Multiplier |
| 104 | function_input | `MaterialExpressionFunctionInput` | Input ---------- Advanced ----------- (Vector3) | ---------- Advanced ----------- |
| 105 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 106 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 107 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 108 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 109 | expression | `MaterialExpressionClamp` | Clamp |  |
| 110 | expression | `MaterialExpressionSign` | Sign |  |
| 111 | expression | `MaterialExpressionMultiply` | Multiply(,-0.5) |  |
| 112 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 113 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 114 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 115 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 116 | function_output | `MaterialExpressionFunctionOutput` | Output Scale Factor | Scale Factor |
| 117 | expression | `MaterialExpressionConstant` | 1 |  |
| 118 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 119 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 120 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 121 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 122 | expression | `MaterialExpressionClamp` | Clamp |  |
| 123 | expression | `MaterialExpressionConstant` | 1 |  |
| 124 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 125 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 126 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 127 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 128 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 129 | expression | `MaterialExpressionDivide` | Divide(,10) |  |
| 130 | expression | `MaterialExpressionDivide` | Divide(,10) |  |
| 131 | expression | `MaterialExpressionPower` | Power |  |
| 132 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 133 | function_output | `MaterialExpressionFunctionOutput` | Output Front Face Mask | Front Face Mask |
| 134 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 135 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 136 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 137 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 138 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 139 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 140 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 141 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 142 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 143 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 144 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 145 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 146 | expression | `MaterialExpressionConstant` | 55 |  |
| 147 | expression | `MaterialExpressionConstant` | 4 |  |
| 148 | expression | `MaterialExpressionConstant` | 16 |  |
| 149 | expression | `MaterialExpressionConstant` | 9 |  |
| 150 | expression | `MaterialExpressionConstant` | 0.0036 |  |
| 151 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 152 | expression | `MaterialExpressionNamedRerouteDeclaration` | Normalized Particle Direction |  |
| 153 | expression | `MaterialExpressionNamedRerouteUsage` | Normalized Particle Direction |  |
| 154 | expression | `MaterialExpressionNamedRerouteUsage` | Normalized Particle Direction |  |
| 155 | expression | `MaterialExpressionNamedRerouteUsage` | Normalized Particle Direction |  |
| 156 | expression | `MaterialExpressionNamedRerouteUsage` | Normalized Particle Direction |  |
| 157 | expression | `MaterialExpressionNamedRerouteUsage` | Normalized Particle Direction |  |
| 158 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 159 | expression | `MaterialExpressionNamedRerouteDeclaration` | Particle Position |  |
| 160 | expression | `MaterialExpressionNamedRerouteUsage` | Particle Position |  |
| 161 | expression | `MaterialExpressionNamedRerouteDeclaration` | Vector to Camera From Particle |  |
| 162 | expression | `MaterialExpressionNamedRerouteUsage` | Vector to Camera From Particle |  |
| 163 | expression | `MaterialExpressionNamedRerouteUsage` | Vector to Camera From Particle |  |
| 164 | expression | `MaterialExpressionNamedRerouteDeclaration` | Capsule Diameter |  |
| 165 | expression | `MaterialExpressionNamedRerouteUsage` | Capsule Diameter |  |
| 166 | expression | `MaterialExpressionNamedRerouteUsage` | Capsule Diameter |  |
| 167 | expression | `MaterialExpressionNamedRerouteUsage` | Capsule Diameter |  |
| 168 | expression | `MaterialExpressionNamedRerouteUsage` | Capsule Diameter |  |
| 169 | expression | `MaterialExpressionNamedRerouteUsage` | Capsule Diameter |  |
| 170 | expression | `MaterialExpressionNamedRerouteUsage` | Capsule Diameter |  |
| 171 | expression | `MaterialExpressionNamedRerouteDeclaration` | Up Vector |  |
| 172 | expression | `MaterialExpressionNamedRerouteUsage` | Up Vector |  |
| 173 | expression | `MaterialExpressionNamedRerouteUsage` | Up Vector |  |
| 174 | expression | `MaterialExpressionNamedRerouteUsage` | Up Vector |  |
| 175 | expression | `MaterialExpressionNamedRerouteUsage` | Vector to Camera From Particle |  |
| 176 | expression | `MaterialExpressionNamedRerouteUsage` | Vector to Camera From Particle |  |
| 177 | expression | `MaterialExpressionNamedRerouteDeclaration` | Particle Alignment With Camera |  |
| 178 | expression | `MaterialExpressionNamedRerouteUsage` | Particle Alignment With Camera |  |
| 179 | expression | `MaterialExpressionNamedRerouteUsage` | Particle Alignment With Camera |  |
| 180 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 181 | expression | `MaterialExpressionAdd` | Add |  |
| 182 | expression | `MaterialExpressionNamedRerouteDeclaration` | Pivot offset |  |
| 183 | expression | `MaterialExpressionNamedRerouteUsage` | Pivot offset |  |
| 184 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 185 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 186 | expression | `MaterialExpressionNamedRerouteDeclaration` | Use Advanced Features Bool |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [101] Switch output 0

### [2] Subtract

- `A` <= [1] TexCoord[0] output 0
- `B` <= [24] 0.5,0.5 output 0

### [3] Subtract

- `A` <= [36] Add output 0
- `B` <= [5] Input World Position to Negate (Vector3) output 0

### [5] Input World Position to Negate (Vector3)

- `Preview` <= [4] Absolute World Position output 0

### [6] Input Particle Position (Vector3)

- `Preview` <= [105] Particle Position (Absolute) output 0

### [7] Input Normalized Particle Direction (Vector3)

- `Preview` <= [21] SafeNormalize output 0

### [8] Input Scale Along Velocity Vector (Scalar)

- `Preview` <= [50] Lerp output 0

### [9] Input Capsule Diameter (Scalar)

- `Preview` <= [61] Mask ( R ) output 0

### [10] Output World Space Normals

- `A` <= [44] Transform3x3Matrix output 0

### [11] Texture Sample

- `Coordinates` <= [17] Output Texture Coordinates output 0

### [12] Output Opacity Mask

- `A` <= [13] Texture Sample output 1

### [13] Texture Sample

- `Coordinates` <= [17] Output Texture Coordinates output 0

### [16] Append

- `A` <= [15] StretchGradient output 0
- `B` <= [23] LinearGradient output 1

### [17] Output Texture Coordinates

- `A` <= [16] Append output 0

### [19] Subtract

- `A` <= [159] Particle Position output 0
- `B` <= [18] Camera Position output 0

### [20] Normalize

- `VectorInput` <= [19] Subtract output 0

### [25] Multiply

- `A` <= [153] Normalized Particle Direction output 0
- `B` <= [47] Reroute Node (reroutes wires) output 0

### [26] Add

- `A` <= [3] Subtract output 0
- `B` <= [160] Particle Position output 0

### [27] Cross

- `A` <= [25] Multiply output 0
- `B` <= [162] Vector to Camera From Particle output 0

### [28] Normalize

- `VectorInput` <= [27] Cross output 0

### [29] Multiply

- `A` <= [22] BreakOutFloat2Components output 1
- `B` <= [168] Capsule Diameter output 0

### [30] Multiply

- `A` <= [29] Multiply output 0
- `B` <= [173] Up Vector output 0

### [31] Cross

- `A` <= [163] Vector to Camera From Particle output 0
- `B` <= [172] Up Vector output 0

### [32] Multiply

- `A` <= [34] Multiply output 0
- `B` <= [45] Reroute Node (reroutes wires) output 0

### [33] Add

- `A` <= [32] Multiply output 0
- `B` <= [30] Multiply output 0

### [34] Multiply

- `A` <= [31] Cross output 0
- `B` <= [167] Capsule Diameter output 0

### [35] Multiply

- `A` <= [112] Switch output 0
- `B` <= [46] Reroute Node (reroutes wires) output 0

### [36] Add

- `A` <= [33] Add output 0
- `B` <= [35] Multiply output 0

### [37] Divide

- `A` <= [86] Reroute Node (reroutes wires) output 0
- `B` <= [87] Multiply output 0

### [38] Dot

- `A` <= [175] Vector to Camera From Particle output 0
- `B` <= [154] Normalized Particle Direction output 0

### [39] Abs

- `Input` <= [179] Particle Alignment With Camera output 0

### [40] Multiply

- `A` <= [37] Divide output 0
- `B` <= [42] 1-x output 0

### [41] Clamp

- `Input` <= [40] Multiply output 0

### [42] 1-x

- `Input` <= [131] Power output 0

### [43] Multiply(,-1)

- `A` <= [176] Vector to Camera From Particle output 0

### [45] Reroute Node (reroutes wires)

- `Input` <= [22] BreakOutFloat2Components output 0

### [46] Reroute Node (reroutes wires)

- `Input` <= [25] Multiply output 0

### [47] Reroute Node (reroutes wires)

- `Input` <= [58] Max(,0.001) output 0

### [48] Divide

- `A` <= [51] Subtract output 0
- `B` <= [55] Input Max Speed (Scalar) output 0

### [49] Clamp

- `Input` <= [48] Divide output 0

### [50] Lerp

- `A` <= [56] Input Minimum Scale Factor (Scalar) output 0
- `B` <= [57] Input Maximum Scale Factor (Scalar) output 0
- `Alpha` <= [49] Clamp output 0

### [51] Subtract

- `A` <= [52] Input Particle Speed (Scalar) output 0
- `B` <= [54] Input Minimum Speed (Scalar) output 0

### [52] Input Particle Speed (Scalar)

- `Preview` <= [53] Particle Speed output 0

### [55] Input Max Speed (Scalar)

- `Preview` <= [180] Reroute Node (reroutes wires) output 0

### [57] Input Maximum Scale Factor (Scalar)

- `Preview` <= [180] Reroute Node (reroutes wires) output 0

### [58] Max(,0.001)

- `A` <= [83] Subtract output 0

### [59] Reroute Node (reroutes wires)

- `Input` <= [31] Cross output 0

### [61] Mask ( R )

- `Input` <= [60] Particle Size output 0

### [62] SphereMask

- `A` <= [72] LinearGradient output 1
- `B` <= [63] 0.5 output 0
- `Radius` <= [70] 0.3794 output 0

### [64] Multiply

- `A` <= [69] 1-x output 0
- `B` <= [66] Multiply output 0

### [65] Add(,0.5)

- `A` <= [64] Multiply output 0

### [66] Multiply

- `A` <= [177] Particle Alignment With Camera output 0
- `B` <= [73] 0.3683 output 0

### [67] Multiply

- `A` <= [68] 1-x output 0
- `B` <= [68] 1-x output 0

### [68] 1-x

- `Input` <= [62] SphereMask output 0

### [69] 1-x

- `Input` <= [71] Multiply output 0

### [71] Multiply

- `A` <= [67] Multiply output 0
- `B` <= [67] Multiply output 0

### [74] Input Use Advanced Features (StaticBool)

- `Preview` <= [75] Static Bool (True) output 0

### [76] Switch

- `A` <= [65] Add(,0.5) output 0
- `B` <= [77] 0.5 output 0
- `Value` <= [114] Reroute Node (reroutes wires) output 0

### [79] Multiply

- `A` <= [81] Multiply output 0
- `B` <= [157] Normalized Particle Direction output 0

### [80] Add

- `A` <= [79] Multiply output 0
- `B` <= [26] Add output 0

### [81] Multiply

- `A` <= [78] Input Capsule Pivot Alignment (Scalar) output 0
- `B` <= [82] Multiply(,-0.5) output 0

### [82] Multiply(,-0.5)

- `A` <= [85] Reroute Node (reroutes wires) output 0

### [83] Subtract

- `A` <= [8] Input Scale Along Velocity Vector (Scalar) output 0
- `B` <= [165] Capsule Diameter output 0

### [84] Reroute Node (reroutes wires)

- `Input` <= [47] Reroute Node (reroutes wires) output 0

### [85] Reroute Node (reroutes wires)

- `Input` <= [84] Reroute Node (reroutes wires) output 0

### [86] Reroute Node (reroutes wires)

- `Input` <= [84] Reroute Node (reroutes wires) output 0

### [87] Multiply

- `A` <= [166] Capsule Diameter output 0
- `B` <= [103] Input Cap Size Multiplier (Scalar) output 0

### [88] Output Pixel Depth Offset

- `A` <= [91] Multiply output 0

### [89] Texture Sample

- `Coordinates` <= [17] Output Texture Coordinates output 0

### [90] 1-x

- `Input` <= [89] Texture Sample output 1

### [91] Multiply

- `A` <= [90] 1-x output 0
- `B` <= [170] Capsule Diameter output 0

### [92] Subtract

- `A` <= [93] Camera Position output 0
- `B` <= [98] Reroute Node (reroutes wires) output 0

### [94] Normalize

- `VectorInput` <= [92] Subtract output 0

### [95] Multiply

- `A` <= [94] Normalize output 0
- `B` <= [169] Capsule Diameter output 0

### [96] Add

- `A` <= [97] Multiply(,0.5) output 0
- `B` <= [99] Reroute Node (reroutes wires) output 0

### [97] Multiply(,0.5)

- `A` <= [95] Multiply output 0

### [98] Reroute Node (reroutes wires)

- `Input` <= [80] Add output 0

### [99] Reroute Node (reroutes wires)

- `Input` <= [98] Reroute Node (reroutes wires) output 0

### [101] Switch

- `A` <= [96] Add output 0
- `B` <= [99] Reroute Node (reroutes wires) output 0
- `Value` <= [102] Input Use Camera Offset (StaticBool) output 0

### [102] Input Use Camera Offset (StaticBool)

- `Preview` <= [100] Static Bool (False) output 0

### [107] Mask ( G )

- `Input` <= [60] Particle Size output 0

### [108] Lerp

- `A` <= [113] Reroute Node (reroutes wires) output 0
- `B` <= [111] Multiply(,-0.5) output 0
- `Alpha` <= [109] Clamp output 0

### [109] Clamp

- `Input` <= [120] Multiply output 0

### [110] Sign

- `Input` <= [178] Particle Alignment With Camera output 0

### [111] Multiply(,-0.5)

- `A` <= [110] Sign output 0

### [112] Switch

- `A` <= [108] Lerp output 0
- `B` <= [113] Reroute Node (reroutes wires) output 0
- `Value` <= [114] Reroute Node (reroutes wires) output 0

### [113] Reroute Node (reroutes wires)

- `Input` <= [22] BreakOutFloat2Components output 0

### [114] Reroute Node (reroutes wires)

- `Input` <= [74] Input Use Advanced Features (StaticBool) output 0

### [116] Output Scale Factor

- `A` <= [8] Input Scale Along Velocity Vector (Scalar) output 0

### [118] Subtract

- `A` <= [117] 1 output 0
- `B` <= [150] 0.0036 output 0

### [119] Divide(1,)

- `B` <= [150] 0.0036 output 0

### [120] Multiply

- `A` <= [121] Subtract output 0
- `B` <= [119] Divide(1,) output 0

### [121] Subtract

- `A` <= [39] Abs output 0
- `B` <= [118] Subtract output 0

### [122] Clamp

- `Input` <= [126] Multiply output 0

### [124] Subtract

- `A` <= [123] 1 output 0
- `B` <= [128] Lerp output 0

### [125] Divide(1,)

- `B` <= [128] Lerp output 0

### [126] Multiply

- `A` <= [127] Subtract output 0
- `B` <= [125] Divide(1,) output 0

### [127] Subtract

- `A` <= [39] Abs output 0
- `B` <= [124] Subtract output 0

### [128] Lerp

- `A` <= [129] Divide(,10) output 0
- `B` <= [130] Divide(,10) output 0
- `Alpha` <= [151] Reroute Node (reroutes wires) output 0

### [129] Divide(,10)

- `A` <= [146] 55 output 0

### [130] Divide(,10)

- `A` <= [147] 4 output 0

### [131] Power

- `Base` <= [122] Clamp output 0
- `Exponent` <= [132] Lerp output 0

### [132] Lerp

- `A` <= [148] 16 output 0
- `B` <= [149] 9 output 0
- `Alpha` <= [145] Reroute Node (reroutes wires) output 0

### [133] Output Front Face Mask

- `A` <= [137] Saturate output 0

### [134] Subtract

- `A` <= [135] Absolute World Position output 0
- `B` <= [185] Reroute Node (reroutes wires) output 0

### [136] Normalize

- `VectorInput` <= [134] Subtract output 0

### [137] Saturate

- `Input` <= [144] Multiply output 0

### [138] Dot

- `A` <= [136] Normalize output 0
- `B` <= [156] Normalized Particle Direction output 0

### [139] Dot

- `A` <= [155] Normalized Particle Direction output 0
- `B` <= [143] Normalize output 0

### [141] Subtract

- `A` <= [140] Camera Position output 0
- `B` <= [184] Reroute Node (reroutes wires) output 0

### [143] Normalize

- `VectorInput` <= [141] Subtract output 0

### [144] Multiply

- `A` <= [139] Dot output 0
- `B` <= [138] Dot output 0

### [145] Reroute Node (reroutes wires)

- `Input` <= [151] Reroute Node (reroutes wires) output 0

### [151] Reroute Node (reroutes wires)

- `Input` <= [133] Output Front Face Mask output 0

### [152] Normalized Particle Direction

- `Input` <= [7] Input Normalized Particle Direction (Vector3) output 0

### [158] Subtract(,1)

- `A` <= [138] Dot output 0

### [159] Particle Position

- `Input` <= [6] Input Particle Position (Vector3) output 0

### [161] Vector to Camera From Particle

- `Input` <= [20] Normalize output 0

### [164] Capsule Diameter

- `Input` <= [9] Input Capsule Diameter (Scalar) output 0

### [171] Up Vector

- `Input` <= [28] Normalize output 0

### [177] Particle Alignment With Camera

- `Input` <= [38] Dot output 0

### [180] Reroute Node (reroutes wires)

- `Input` <= [107] Mask ( G ) output 0

### [181] Add

- `A` <= [183] Pivot offset output 0
- `B` <= [142] Particle Position (Absolute) output 0

### [182] Pivot offset

- `Input` <= [79] Multiply output 0

### [184] Reroute Node (reroutes wires)

- `Input` <= [181] Add output 0

### [185] Reroute Node (reroutes wires)

- `Input` <= [181] Add output 0

### [186] Use Advanced Features Bool

- `Input` <= [114] Reroute Node (reroutes wires) output 0

## Connection List

- [101] Switch output 0 -> Output World Position Offset.A
- [1] TexCoord[0] output 0 -> Subtract.A
- [24] 0.5,0.5 output 0 -> Subtract.B
- [36] Add output 0 -> Subtract.A
- [5] Input World Position to Negate (Vector3) output 0 -> Subtract.B
- [4] Absolute World Position output 0 -> Input World Position to Negate (Vector3).Preview
- [105] Particle Position (Absolute) output 0 -> Input Particle Position (Vector3).Preview
- [21] SafeNormalize output 0 -> Input Normalized Particle Direction (Vector3).Preview
- [50] Lerp output 0 -> Input Scale Along Velocity Vector (Scalar).Preview
- [61] Mask ( R ) output 0 -> Input Capsule Diameter (Scalar).Preview
- [44] Transform3x3Matrix output 0 -> Output World Space Normals.A
- [17] Output Texture Coordinates output 0 -> Texture Sample.Coordinates
- [13] Texture Sample output 1 -> Output Opacity Mask.A
- [17] Output Texture Coordinates output 0 -> Texture Sample.Coordinates
- [15] StretchGradient output 0 -> Append.A
- [23] LinearGradient output 1 -> Append.B
- [16] Append output 0 -> Output Texture Coordinates.A
- [159] Particle Position output 0 -> Subtract.A
- [18] Camera Position output 0 -> Subtract.B
- [19] Subtract output 0 -> Normalize.VectorInput
- [153] Normalized Particle Direction output 0 -> Multiply.A
- [47] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [3] Subtract output 0 -> Add.A
- [160] Particle Position output 0 -> Add.B
- [25] Multiply output 0 -> Cross.A
- [162] Vector to Camera From Particle output 0 -> Cross.B
- [27] Cross output 0 -> Normalize.VectorInput
- [22] BreakOutFloat2Components output 1 -> Multiply.A
- [168] Capsule Diameter output 0 -> Multiply.B
- [29] Multiply output 0 -> Multiply.A
- [173] Up Vector output 0 -> Multiply.B
- [163] Vector to Camera From Particle output 0 -> Cross.A
- [172] Up Vector output 0 -> Cross.B
- [34] Multiply output 0 -> Multiply.A
- [45] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [32] Multiply output 0 -> Add.A
- [30] Multiply output 0 -> Add.B
- [31] Cross output 0 -> Multiply.A
- [167] Capsule Diameter output 0 -> Multiply.B
- [112] Switch output 0 -> Multiply.A
- [46] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [33] Add output 0 -> Add.A
- [35] Multiply output 0 -> Add.B
- [86] Reroute Node (reroutes wires) output 0 -> Divide.A
- [87] Multiply output 0 -> Divide.B
- [175] Vector to Camera From Particle output 0 -> Dot.A
- [154] Normalized Particle Direction output 0 -> Dot.B
- [179] Particle Alignment With Camera output 0 -> Abs.Input
- [37] Divide output 0 -> Multiply.A
- [42] 1-x output 0 -> Multiply.B
- [40] Multiply output 0 -> Clamp.Input
- [131] Power output 0 -> 1-x.Input
- [176] Vector to Camera From Particle output 0 -> Multiply(,-1).A
- [22] BreakOutFloat2Components output 0 -> Reroute Node (reroutes wires).Input
- [25] Multiply output 0 -> Reroute Node (reroutes wires).Input
- [58] Max(,0.001) output 0 -> Reroute Node (reroutes wires).Input
- [51] Subtract output 0 -> Divide.A
- [55] Input Max Speed (Scalar) output 0 -> Divide.B
- [48] Divide output 0 -> Clamp.Input
- [56] Input Minimum Scale Factor (Scalar) output 0 -> Lerp.A
- [57] Input Maximum Scale Factor (Scalar) output 0 -> Lerp.B
- [49] Clamp output 0 -> Lerp.Alpha
- [52] Input Particle Speed (Scalar) output 0 -> Subtract.A
- [54] Input Minimum Speed (Scalar) output 0 -> Subtract.B
- [53] Particle Speed output 0 -> Input Particle Speed (Scalar).Preview
- [180] Reroute Node (reroutes wires) output 0 -> Input Max Speed (Scalar).Preview
- [180] Reroute Node (reroutes wires) output 0 -> Input Maximum Scale Factor (Scalar).Preview
- [83] Subtract output 0 -> Max(,0.001).A
- [31] Cross output 0 -> Reroute Node (reroutes wires).Input
- [60] Particle Size output 0 -> Mask ( R ).Input
- [72] LinearGradient output 1 -> SphereMask.A
- [63] 0.5 output 0 -> SphereMask.B
- [70] 0.3794 output 0 -> SphereMask.Radius
- [69] 1-x output 0 -> Multiply.A
- [66] Multiply output 0 -> Multiply.B
- [64] Multiply output 0 -> Add(,0.5).A
- [177] Particle Alignment With Camera output 0 -> Multiply.A
- [73] 0.3683 output 0 -> Multiply.B
- [68] 1-x output 0 -> Multiply.A
- [68] 1-x output 0 -> Multiply.B
- [62] SphereMask output 0 -> 1-x.Input
- [71] Multiply output 0 -> 1-x.Input
- [67] Multiply output 0 -> Multiply.A
- [67] Multiply output 0 -> Multiply.B
- [75] Static Bool (True) output 0 -> Input Use Advanced Features (StaticBool).Preview
- [65] Add(,0.5) output 0 -> Switch.A
- [77] 0.5 output 0 -> Switch.B
- [114] Reroute Node (reroutes wires) output 0 -> Switch.Value
- [81] Multiply output 0 -> Multiply.A
- [157] Normalized Particle Direction output 0 -> Multiply.B
- [79] Multiply output 0 -> Add.A
- [26] Add output 0 -> Add.B
- [78] Input Capsule Pivot Alignment (Scalar) output 0 -> Multiply.A
- [82] Multiply(,-0.5) output 0 -> Multiply.B
- [85] Reroute Node (reroutes wires) output 0 -> Multiply(,-0.5).A
- [8] Input Scale Along Velocity Vector (Scalar) output 0 -> Subtract.A
- [165] Capsule Diameter output 0 -> Subtract.B
- [47] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [84] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [84] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [166] Capsule Diameter output 0 -> Multiply.A
- [103] Input Cap Size Multiplier (Scalar) output 0 -> Multiply.B
- [91] Multiply output 0 -> Output Pixel Depth Offset.A
- [17] Output Texture Coordinates output 0 -> Texture Sample.Coordinates
- [89] Texture Sample output 1 -> 1-x.Input
- [90] 1-x output 0 -> Multiply.A
- [170] Capsule Diameter output 0 -> Multiply.B
- [93] Camera Position output 0 -> Subtract.A
- [98] Reroute Node (reroutes wires) output 0 -> Subtract.B
- [92] Subtract output 0 -> Normalize.VectorInput
- [94] Normalize output 0 -> Multiply.A
- [169] Capsule Diameter output 0 -> Multiply.B
- [97] Multiply(,0.5) output 0 -> Add.A
- [99] Reroute Node (reroutes wires) output 0 -> Add.B
- [95] Multiply output 0 -> Multiply(,0.5).A
- [80] Add output 0 -> Reroute Node (reroutes wires).Input
- [98] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [96] Add output 0 -> Switch.A
- [99] Reroute Node (reroutes wires) output 0 -> Switch.B
- [102] Input Use Camera Offset (StaticBool) output 0 -> Switch.Value
- [100] Static Bool (False) output 0 -> Input Use Camera Offset (StaticBool).Preview
- [60] Particle Size output 0 -> Mask ( G ).Input
- [113] Reroute Node (reroutes wires) output 0 -> Lerp.A
- [111] Multiply(,-0.5) output 0 -> Lerp.B
- [109] Clamp output 0 -> Lerp.Alpha
- [120] Multiply output 0 -> Clamp.Input
- [178] Particle Alignment With Camera output 0 -> Sign.Input
- [110] Sign output 0 -> Multiply(,-0.5).A
- [108] Lerp output 0 -> Switch.A
- [113] Reroute Node (reroutes wires) output 0 -> Switch.B
- [114] Reroute Node (reroutes wires) output 0 -> Switch.Value
- [22] BreakOutFloat2Components output 0 -> Reroute Node (reroutes wires).Input
- [74] Input Use Advanced Features (StaticBool) output 0 -> Reroute Node (reroutes wires).Input
- [8] Input Scale Along Velocity Vector (Scalar) output 0 -> Output Scale Factor.A
- [117] 1 output 0 -> Subtract.A
- [150] 0.0036 output 0 -> Subtract.B
- [150] 0.0036 output 0 -> Divide(1,).B
- [121] Subtract output 0 -> Multiply.A
- [119] Divide(1,) output 0 -> Multiply.B
- [39] Abs output 0 -> Subtract.A
- [118] Subtract output 0 -> Subtract.B
- [126] Multiply output 0 -> Clamp.Input
- [123] 1 output 0 -> Subtract.A
- [128] Lerp output 0 -> Subtract.B
- [128] Lerp output 0 -> Divide(1,).B
- [127] Subtract output 0 -> Multiply.A
- [125] Divide(1,) output 0 -> Multiply.B
- [39] Abs output 0 -> Subtract.A
- [124] Subtract output 0 -> Subtract.B
- [129] Divide(,10) output 0 -> Lerp.A
- [130] Divide(,10) output 0 -> Lerp.B
- [151] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [146] 55 output 0 -> Divide(,10).A
- [147] 4 output 0 -> Divide(,10).A
- [122] Clamp output 0 -> Power.Base
- [132] Lerp output 0 -> Power.Exponent
- [148] 16 output 0 -> Lerp.A
- [149] 9 output 0 -> Lerp.B
- [145] Reroute Node (reroutes wires) output 0 -> Lerp.Alpha
- [137] Saturate output 0 -> Output Front Face Mask.A
- [135] Absolute World Position output 0 -> Subtract.A
- [185] Reroute Node (reroutes wires) output 0 -> Subtract.B
- [134] Subtract output 0 -> Normalize.VectorInput
- [144] Multiply output 0 -> Saturate.Input
- [136] Normalize output 0 -> Dot.A
- [156] Normalized Particle Direction output 0 -> Dot.B
- [155] Normalized Particle Direction output 0 -> Dot.A
- [143] Normalize output 0 -> Dot.B
- [140] Camera Position output 0 -> Subtract.A
- [184] Reroute Node (reroutes wires) output 0 -> Subtract.B
- [141] Subtract output 0 -> Normalize.VectorInput
- [139] Dot output 0 -> Multiply.A
- [138] Dot output 0 -> Multiply.B
- [151] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [133] Output Front Face Mask output 0 -> Reroute Node (reroutes wires).Input
- [7] Input Normalized Particle Direction (Vector3) output 0 -> Normalized Particle Direction.Input
- [138] Dot output 0 -> Subtract(,1).A
- [6] Input Particle Position (Vector3) output 0 -> Particle Position.Input
- [20] Normalize output 0 -> Vector to Camera From Particle.Input
- [9] Input Capsule Diameter (Scalar) output 0 -> Capsule Diameter.Input
- [28] Normalize output 0 -> Up Vector.Input
- [38] Dot output 0 -> Particle Alignment With Camera.Input
- [107] Mask ( G ) output 0 -> Reroute Node (reroutes wires).Input
- [183] Pivot offset output 0 -> Add.A
- [142] Particle Position (Absolute) output 0 -> Add.B
- [79] Multiply output 0 -> Pivot offset.Input
- [181] Add output 0 -> Reroute Node (reroutes wires).Input
- [181] Add output 0 -> Reroute Node (reroutes wires).Input
- [114] Reroute Node (reroutes wires) output 0 -> Use Advanced Features Bool.Input

## Output Trace Roots

- `World Position Offset` is driven by [101] Switch
- `World Space Normals` is driven by [44] Transform3x3Matrix
- `Opacity Mask` is driven by [13] Texture Sample
- `Texture Coordinates` is driven by [16] Append
- `Pixel Depth Offset` is driven by [91] Multiply
- `Scale Factor` is driven by [8] Input Scale Along Velocity Vector (Scalar)
- `Front Face Mask` is driven by [137] Saturate

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
