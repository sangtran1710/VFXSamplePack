# Sprite_Ellipsoid

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite_Ellipsoid.Sprite_Ellipsoid`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The material function will align a particle, or plane, to a normalized velocity vector, and scale on that axis while maintaining it's screen-alignment. The distortion along the velocity vector disappears when the vector and the viewer's line of sight align. The module can be used to represent velocity aligned ellipsoidal shapes on quads.

## Inputs

- `World Position to Negate` [FunctionInput_Vector3] - World position must be subtracted from the World Position Offset output before being inserted into the World Position Offset pin. This input allows advanced users to override the world position value that's subtracted from the output.
- `Particle Position` [FunctionInput_Vector3]
- `Particle Direction` [FunctionInput_Vector3]
- `Scale Along Velocity Vector` [FunctionInput_Scalar] - Note: By default, an equation using the "Scale Over Velocity Parameters" sets this value. Plugging an input into this field will allow one to manually control the scale factor.
- `Particle Size` [FunctionInput_Vector2]
- `Normals` [FunctionInput_Vector3] - These normals will be transformed from view space to world space. Please ensure that your normal texture's tangent space isn't rotated or skewed.
- `Opacity Mask` [FunctionInput_Vector3]
- `---------Scale Over Velocity Parameters---------` [FunctionInput_StaticBool]
- `Particle Speed` [FunctionInput_Scalar]
- `Minimum Speed` [FunctionInput_Scalar] - The particle must go this fast before beginning to warp along the velocity vector.
- `Max Speed` [FunctionInput_Scalar] - After reaching the minimum speed, the particle will start to warp until this speed is reached (in addition to the minimum speed).
- `Minimum Scale Factor` [FunctionInput_Scalar]
- `Maximum Scale Factor` [FunctionInput_Scalar]

## Outputs

- `World Position Offset`
- `World Space Normal` - These normals are in world space. Ensure that tangent space normals are unchecked in your material's details panel. 
- `Opacity Mask`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 11 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 12 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 13 | expression | `MaterialExpressionConstant3Vector` | 0,-1,0 |  |
| 14 | expression | `MaterialExpressionTransform` | World Space to Camera Space TransformVector |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionConstant3Vector` | 1,1,0 |  |
| 19 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 22 | expression | `MaterialExpressionAdd` | Add |  |
| 23 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input World Position to Negate (Vector3) | World Position to Negate |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Particle Position (Vector3) | Particle Position |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Particle Direction (Vector3) | Particle Direction |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Scale Along Velocity Vector (Scalar) | Scale Along Velocity Vector |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Particle Size (Vector2) | Particle Size |
| 29 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 30 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normal | World Space Normal |
| 32 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 33 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 34 | function_output | `MaterialExpressionFunctionOutput` | Output Opacity Mask | Opacity Mask |
| 35 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 36 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 37 | function_input | `MaterialExpressionFunctionInput` | Input Normals (Vector3) | Normals |
| 38 | function_input | `MaterialExpressionFunctionInput` | Input Opacity Mask (Vector3) | Opacity Mask |
| 39 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 40 | expression | `MaterialExpressionConstant3Vector` | 1,-1,-1 |  |
| 41 | expression | `MaterialExpressionDivide` | Divide |  |
| 42 | expression | `MaterialExpressionClamp` | Clamp |  |
| 43 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 44 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 45 | function_input | `MaterialExpressionFunctionInput` | Input ---------Scale Over Velocity Parameters--------- (StaticBool) | ---------Scale Over Velocity Parameters--------- |
| 46 | function_input | `MaterialExpressionFunctionInput` | Input Particle Speed (Scalar) | Particle Speed |
| 47 | expression | `MaterialExpressionParticleSpeed` | Particle Speed |  |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input Minimum Speed (Scalar) | Minimum Speed |
| 49 | function_input | `MaterialExpressionFunctionInput` | Input Max Speed (Scalar) | Max Speed |
| 50 | function_input | `MaterialExpressionFunctionInput` | Input Minimum Scale Factor (Scalar) | Minimum Scale Factor |
| 51 | function_input | `MaterialExpressionFunctionInput` | Input Maximum Scale Factor (Scalar) | Maximum Scale Factor |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [7] Subtract output 0

### [2] Subtract(,0.5)

- `A` <= [1] TexCoord[0] output 0

### [4] Multiply

- `A` <= [3] BreakOutFloat2Components output 0
- `B` <= [11] Camera Space to World Space TransformVector output 0

### [5] Multiply

- `A` <= [3] BreakOutFloat2Components output 1
- `B` <= [10] Camera Space to World Space TransformVector output 0

### [6] Add

- `A` <= [4] Multiply output 0
- `B` <= [5] Multiply output 0

### [7] Subtract

- `A` <= [9] Add output 0
- `B` <= [24] Input World Position to Negate (Vector3) output 0

### [9] Add

- `A` <= [22] Add output 0
- `B` <= [25] Input Particle Position (Vector3) output 0

### [10] Camera Space to World Space TransformVector

- `Input` <= [13] 0,-1,0 output 0

### [11] Camera Space to World Space TransformVector

- `Input` <= [12] 1,0,0 output 0

### [14] World Space to Camera Space TransformVector

- `Input` <= [15] Multiply output 0

### [15] Multiply

- `A` <= [26] Input Particle Direction (Vector3) output 0
- `B` <= [27] Input Scale Along Velocity Vector (Scalar) output 0

### [16] Multiply

- `A` <= [2] Subtract(,0.5) output 0
- `B` <= [28] Input Particle Size (Vector2) output 0

### [17] Multiply

- `A` <= [18] 1,1,0 output 0
- `B` <= [14] World Space to Camera Space TransformVector output 0

### [19] Camera Space to World Space TransformVector

- `Input` <= [17] Multiply output 0

### [20] Multiply

- `A` <= [19] Camera Space to World Space TransformVector output 0
- `B` <= [21] Dot output 0

### [21] Dot

- `A` <= [23] Reroute Node (reroutes wires) output 0
- `B` <= [19] Camera Space to World Space TransformVector output 0

### [22] Add

- `A` <= [23] Reroute Node (reroutes wires) output 0
- `B` <= [20] Multiply output 0

### [23] Reroute Node (reroutes wires)

- `Input` <= [6] Add output 0

### [24] Input World Position to Negate (Vector3)

- `Preview` <= [8] Absolute World Position output 0

### [25] Input Particle Position (Vector3)

- `Preview` <= [30] Particle Position (Absolute) output 0

### [26] Input Particle Direction (Vector3)

- `Preview` <= [29] Particle Direction output 0

### [27] Input Scale Along Velocity Vector (Scalar)

- `Preview` <= [43] Lerp output 0

### [28] Input Particle Size (Vector2)

- `Preview` <= [36] Particle Size output 0

### [31] Output World Space Normal

- `A` <= [33] View Space to World Space TransformVector output 0

### [33] View Space to World Space TransformVector

- `Input` <= [39] Multiply output 0

### [34] Output Opacity Mask

- `A` <= [38] Input Opacity Mask (Vector3) output 0

### [37] Input Normals (Vector3)

- `Preview` <= [32] Texture Sample output 0

### [38] Input Opacity Mask (Vector3)

- `Preview` <= [35] Texture Sample output 1

### [39] Multiply

- `A` <= [37] Input Normals (Vector3) output 0
- `B` <= [40] 1,-1,-1 output 0

### [41] Divide

- `A` <= [44] Subtract output 0
- `B` <= [49] Input Max Speed (Scalar) output 0

### [42] Clamp

- `Input` <= [41] Divide output 0

### [43] Lerp

- `A` <= [50] Input Minimum Scale Factor (Scalar) output 0
- `B` <= [51] Input Maximum Scale Factor (Scalar) output 0
- `Alpha` <= [42] Clamp output 0

### [44] Subtract

- `A` <= [46] Input Particle Speed (Scalar) output 0
- `B` <= [48] Input Minimum Speed (Scalar) output 0

### [46] Input Particle Speed (Scalar)

- `Preview` <= [47] Particle Speed output 0

## Connection List

- [7] Subtract output 0 -> Output World Position Offset.A
- [1] TexCoord[0] output 0 -> Subtract(,0.5).A
- [3] BreakOutFloat2Components output 0 -> Multiply.A
- [11] Camera Space to World Space TransformVector output 0 -> Multiply.B
- [3] BreakOutFloat2Components output 1 -> Multiply.A
- [10] Camera Space to World Space TransformVector output 0 -> Multiply.B
- [4] Multiply output 0 -> Add.A
- [5] Multiply output 0 -> Add.B
- [9] Add output 0 -> Subtract.A
- [24] Input World Position to Negate (Vector3) output 0 -> Subtract.B
- [22] Add output 0 -> Add.A
- [25] Input Particle Position (Vector3) output 0 -> Add.B
- [13] 0,-1,0 output 0 -> Camera Space to World Space TransformVector.Input
- [12] 1,0,0 output 0 -> Camera Space to World Space TransformVector.Input
- [15] Multiply output 0 -> World Space to Camera Space TransformVector.Input
- [26] Input Particle Direction (Vector3) output 0 -> Multiply.A
- [27] Input Scale Along Velocity Vector (Scalar) output 0 -> Multiply.B
- [2] Subtract(,0.5) output 0 -> Multiply.A
- [28] Input Particle Size (Vector2) output 0 -> Multiply.B
- [18] 1,1,0 output 0 -> Multiply.A
- [14] World Space to Camera Space TransformVector output 0 -> Multiply.B
- [17] Multiply output 0 -> Camera Space to World Space TransformVector.Input
- [19] Camera Space to World Space TransformVector output 0 -> Multiply.A
- [21] Dot output 0 -> Multiply.B
- [23] Reroute Node (reroutes wires) output 0 -> Dot.A
- [19] Camera Space to World Space TransformVector output 0 -> Dot.B
- [23] Reroute Node (reroutes wires) output 0 -> Add.A
- [20] Multiply output 0 -> Add.B
- [6] Add output 0 -> Reroute Node (reroutes wires).Input
- [8] Absolute World Position output 0 -> Input World Position to Negate (Vector3).Preview
- [30] Particle Position (Absolute) output 0 -> Input Particle Position (Vector3).Preview
- [29] Particle Direction output 0 -> Input Particle Direction (Vector3).Preview
- [43] Lerp output 0 -> Input Scale Along Velocity Vector (Scalar).Preview
- [36] Particle Size output 0 -> Input Particle Size (Vector2).Preview
- [33] View Space to World Space TransformVector output 0 -> Output World Space Normal.A
- [39] Multiply output 0 -> View Space to World Space TransformVector.Input
- [38] Input Opacity Mask (Vector3) output 0 -> Output Opacity Mask.A
- [32] Texture Sample output 0 -> Input Normals (Vector3).Preview
- [35] Texture Sample output 1 -> Input Opacity Mask (Vector3).Preview
- [37] Input Normals (Vector3) output 0 -> Multiply.A
- [40] 1,-1,-1 output 0 -> Multiply.B
- [44] Subtract output 0 -> Divide.A
- [49] Input Max Speed (Scalar) output 0 -> Divide.B
- [41] Divide output 0 -> Clamp.Input
- [50] Input Minimum Scale Factor (Scalar) output 0 -> Lerp.A
- [51] Input Maximum Scale Factor (Scalar) output 0 -> Lerp.B
- [42] Clamp output 0 -> Lerp.Alpha
- [46] Input Particle Speed (Scalar) output 0 -> Subtract.A
- [48] Input Minimum Speed (Scalar) output 0 -> Subtract.B
- [47] Particle Speed output 0 -> Input Particle Speed (Scalar).Preview

## Output Trace Roots

- `World Position Offset` is driven by [7] Subtract
- `World Space Normal` is driven by [33] View Space to World Space TransformVector
- `Opacity Mask` is driven by [38] Input Opacity Mask (Vector3)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
