# ParticleSizeByPixelUnits

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Particles/ParticleSizeByPixelUnits.ParticleSizeByPixelUnits`
- Category: Engine_MaterialFunctions02/Particles
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function replaces the vertex shader for sprite particles with screen space sprites. The particles XY dimension are interpretted as the number of pixels that the particle should span rather than the number of world space units that the particle should occupy.

## Inputs

- `Normals (Optional)` [FunctionInput_Vector3] - This is an optional input. The function will correctly transform your normals.
- `Size In Pixels` [FunctionInput_Vector2]
- `ScreenSpace Pivot Offset` [FunctionInput_Vector2]
- `ParticlePosition` [FunctionInput_Vector3]

## Outputs

- `World Position Offset` - Takes the particles' dimensions and transforms those values into the world space positions needed to expand particles out to that number of pixels. I.e. a particle with a scale of (1,2) would appear to be 1 pixel wide by 2 pixels tall at any distance.
- `Normals` - This will output transformed tangent space normals.
- `World Position Offset With Pivot Support`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 14 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 15 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 16 | expression | `MaterialExpressionConstant3Vector` | 0,-1,0 |  |
| 17 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 18 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 21 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 22 | expression | `MaterialExpressionAbs` | Abs |  |
| 23 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 24 | custom | `MaterialExpressionCustom` | Custom |  |
| 25 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 26 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 27 | expression | `MaterialExpressionDivide` | Divide |  |
| 28 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 29 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Normals (Optional) (Vector3) | Normals (Optional) |
| 32 | expression | `MaterialExpressionTransform` | View Space to Tangent Space TransformVector |  |
| 33 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 34 | expression | `MaterialExpressionConstant3Vector` | 1,-1,-1 |  |
| 35 | function_output | `MaterialExpressionFunctionOutput` | Output Normals | Normals |
| 36 | function_call | `MaterialExpressionMaterialFunctionCall` | TransformToClipSpace |  |
| 37 | expression | `MaterialExpressionDivide` | Divide |  |
| 38 | expression | `MaterialExpressionSceneTexelSize` | SceneTexelSize |  |
| 39 | expression | `MaterialExpressionCeil` | Ceil |  |
| 40 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 41 | custom | `MaterialExpressionCustom` | Custom |  |
| 42 | expression | `MaterialExpressionAppendVector` | Append |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input Size In Pixels (Vector2) | Size In Pixels |
| 44 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset With Pivot Support | World Position Offset With Pivot Support |
| 45 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 47 | expression | `MaterialExpressionAdd` | Add |  |
| 48 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 49 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 50 | function_input | `MaterialExpressionFunctionInput` | Input ScreenSpace Pivot Offset (Vector2) | ScreenSpace Pivot Offset |
| 51 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 52 | expression | `MaterialExpressionAdd` | Add |  |
| 53 | function_input | `MaterialExpressionFunctionInput` | Input ParticlePosition (Vector3) | ParticlePosition |
| 54 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [12] Add output 0

### [2] Subtract(,0.5)

- `A` <= [1] TexCoord[0] output 0

### [4] Multiply

- `A` <= [3] BreakOutFloat2Components output 0
- `B` <= [14] Camera Space to World Space TransformVector output 0

### [5] Multiply

- `A` <= [3] BreakOutFloat2Components output 1
- `B` <= [13] Camera Space to World Space TransformVector output 0

### [6] Multiply

- `A` <= [4] Multiply output 0
- `B` <= [9] BreakOutFloat2Components output 0

### [7] Multiply

- `A` <= [5] Multiply output 0
- `B` <= [9] BreakOutFloat2Components output 1

### [8] Add

- `A` <= [6] Multiply output 0
- `B` <= [7] Multiply output 0

### [10] Subtract

- `A` <= [8] Add output 0
- `B` <= [11] Absolute World Position output 0

### [12] Add

- `A` <= [10] Subtract output 0
- `B` <= [54] Reroute Node (reroutes wires) output 0

### [13] Camera Space to World Space TransformVector

- `Input` <= [16] 0,-1,0 output 0

### [14] Camera Space to World Space TransformVector

- `Input` <= [15] 1,0,0 output 0

### [18] Dot

- `A` <= [20] Subtract output 0
- `B` <= [19] CameraDirectionVector output 0

### [20] Subtract

- `A` <= [53] Input ParticlePosition (Vector3) output 0
- `B` <= [21] Camera Position output 0

### [22] Abs

- `Input` <= [18] Dot output 0

### [26] Multiply

- `A` <= [24] Custom output 0
- `B` <= [25] ViewSize output 0

### [27] Divide

- `A` <= [22] Abs output 0
- `B` <= [29] Multiply(,0.5) output 0

### [28] Mask ( R )

- `Input` <= [26] Multiply output 0

### [29] Multiply(,0.5)

- `A` <= [28] Mask ( R ) output 0

### [30] Multiply

- `A` <= [43] Input Size In Pixels (Vector2) output 0
- `B` <= [27] Divide output 0

### [32] View Space to Tangent Space TransformVector

- `Input` <= [33] Multiply output 0

### [33] Multiply

- `A` <= [31] Input Normals (Optional) (Vector3) output 0
- `B` <= [34] 1,-1,-1 output 0

### [35] Output Normals

- `A` <= [32] View Space to Tangent Space TransformVector output 0

### [37] Divide

- `A` <= [36] TransformToClipSpace output 2
- `B` <= [38] SceneTexelSize output 0

### [39] Ceil

- `Input` <= [37] Divide output 0

### [40] Multiply

- `A` <= [39] Ceil output 0
- `B` <= [38] SceneTexelSize output 0

### [42] Append

- `A` <= [40] Multiply output 0
- `B` <= [22] Abs output 0

### [43] Input Size In Pixels (Vector2)

- `Preview` <= [23] Particle Size output 0

### [44] Output World Position Offset With Pivot Support

- `A` <= [52] Add output 0

### [45] Multiply

- `A` <= [48] Multiply output 0
- `B` <= [9] BreakOutFloat2Components output 0

### [46] Multiply

- `A` <= [49] Multiply output 0
- `B` <= [9] BreakOutFloat2Components output 1

### [47] Add

- `A` <= [45] Multiply output 0
- `B` <= [46] Multiply output 0

### [48] Multiply

- `A` <= [14] Camera Space to World Space TransformVector output 0
- `B` <= [51] BreakOutFloat2Components output 0

### [49] Multiply

- `A` <= [13] Camera Space to World Space TransformVector output 0
- `B` <= [51] BreakOutFloat2Components output 1

### [52] Add

- `A` <= [47] Add output 0
- `B` <= [12] Add output 0

### [53] Input ParticlePosition (Vector3)

- `Preview` <= [17] Particle Position (Absolute) output 0

### [54] Reroute Node (reroutes wires)

- `Input` <= [53] Input ParticlePosition (Vector3) output 0

## Connection List

- [12] Add output 0 -> Output World Position Offset.A
- [1] TexCoord[0] output 0 -> Subtract(,0.5).A
- [3] BreakOutFloat2Components output 0 -> Multiply.A
- [14] Camera Space to World Space TransformVector output 0 -> Multiply.B
- [3] BreakOutFloat2Components output 1 -> Multiply.A
- [13] Camera Space to World Space TransformVector output 0 -> Multiply.B
- [4] Multiply output 0 -> Multiply.A
- [9] BreakOutFloat2Components output 0 -> Multiply.B
- [5] Multiply output 0 -> Multiply.A
- [9] BreakOutFloat2Components output 1 -> Multiply.B
- [6] Multiply output 0 -> Add.A
- [7] Multiply output 0 -> Add.B
- [8] Add output 0 -> Subtract.A
- [11] Absolute World Position output 0 -> Subtract.B
- [10] Subtract output 0 -> Add.A
- [54] Reroute Node (reroutes wires) output 0 -> Add.B
- [16] 0,-1,0 output 0 -> Camera Space to World Space TransformVector.Input
- [15] 1,0,0 output 0 -> Camera Space to World Space TransformVector.Input
- [20] Subtract output 0 -> Dot.A
- [19] CameraDirectionVector output 0 -> Dot.B
- [53] Input ParticlePosition (Vector3) output 0 -> Subtract.A
- [21] Camera Position output 0 -> Subtract.B
- [18] Dot output 0 -> Abs.Input
- [24] Custom output 0 -> Multiply.A
- [25] ViewSize output 0 -> Multiply.B
- [22] Abs output 0 -> Divide.A
- [29] Multiply(,0.5) output 0 -> Divide.B
- [26] Multiply output 0 -> Mask ( R ).Input
- [28] Mask ( R ) output 0 -> Multiply(,0.5).A
- [43] Input Size In Pixels (Vector2) output 0 -> Multiply.A
- [27] Divide output 0 -> Multiply.B
- [33] Multiply output 0 -> View Space to Tangent Space TransformVector.Input
- [31] Input Normals (Optional) (Vector3) output 0 -> Multiply.A
- [34] 1,-1,-1 output 0 -> Multiply.B
- [32] View Space to Tangent Space TransformVector output 0 -> Output Normals.A
- [36] TransformToClipSpace output 2 -> Divide.A
- [38] SceneTexelSize output 0 -> Divide.B
- [37] Divide output 0 -> Ceil.Input
- [39] Ceil output 0 -> Multiply.A
- [38] SceneTexelSize output 0 -> Multiply.B
- [40] Multiply output 0 -> Append.A
- [22] Abs output 0 -> Append.B
- [23] Particle Size output 0 -> Input Size In Pixels (Vector2).Preview
- [52] Add output 0 -> Output World Position Offset With Pivot Support.A
- [48] Multiply output 0 -> Multiply.A
- [9] BreakOutFloat2Components output 0 -> Multiply.B
- [49] Multiply output 0 -> Multiply.A
- [9] BreakOutFloat2Components output 1 -> Multiply.B
- [45] Multiply output 0 -> Add.A
- [46] Multiply output 0 -> Add.B
- [14] Camera Space to World Space TransformVector output 0 -> Multiply.A
- [51] BreakOutFloat2Components output 0 -> Multiply.B
- [13] Camera Space to World Space TransformVector output 0 -> Multiply.A
- [51] BreakOutFloat2Components output 1 -> Multiply.B
- [47] Add output 0 -> Add.A
- [12] Add output 0 -> Add.B
- [17] Particle Position (Absolute) output 0 -> Input ParticlePosition (Vector3).Preview
- [53] Input ParticlePosition (Vector3) output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `World Position Offset` is driven by [12] Add
- `Normals` is driven by [32] View Space to Tangent Space TransformVector
- `World Position Offset With Pivot Support` is driven by [52] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
