# AlignFacingParticlesByVelocity2D

- Path: `/Engine/Functions/Engine_MaterialFunctions02/AlignFacingParticlesByVelocity2D.AlignFacingParticlesByVelocity2D`
- Category: Engine_MaterialFunctions02
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- None

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 2 | expression | `MaterialExpressionClamp` | Clamp |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 8 | expression | `MaterialExpressionRotateAboutAxis` | RotateAboutAxis |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorToRadialValue |  |
| 10 | expression | `MaterialExpressionTransform` | World Space to View Space TransformVector |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 12 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 13 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 14 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 15 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 16 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 17 | expression | `MaterialExpressionAppendVector` | Append |  |
| 18 | expression | `MaterialExpressionConstant` | 0 |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 20 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeVectorsOrthogonal |  |
| 21 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 22 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 23 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 24 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 25 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 26 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 27 | expression | `MaterialExpressionAdd` | Add |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 31 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 32 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] RotateAboutAxis output 0

### [2] Clamp

- `Input` <= [4] Multiply(,2) output 0

### [4] Multiply(,2)

- `A` <= [5] Mask ( R ) output 0

### [5] Mask ( R )

- `Input` <= [3] CameraDirectionVector output 0

### [6] Multiply

- `A` <= [1] CameraDirectionVector output 0
- `B` <= [2] Clamp output 0

### [7] Normalize

- `VectorInput` <= [6] Multiply output 0

### [8] RotateAboutAxis

- `NormalizedRotationAxis` <= [15] VertexNormalWS output 0
- `RotationAngle` <= [9] VectorToRadialValue output 1
- `PivotPoint` <= [13] Particle Position (Absolute) output 0
- `Position` <= [14] Absolute World Position output 0

### [10] World Space to View Space TransformVector

- `Input` <= [12] Particle Direction output 0

### [11] Mask ( G B )

- `Input` <= [10] World Space to View Space TransformVector output 0

### [17] Append

- `A` <= [28] Multiply output 0
- `B` <= [18] 0 output 0

### [21] Cross

- `A` <= [22] Particle Direction output 0
- `B` <= [23] CameraDirectionVector output 0

### [24] Subtract

- `A` <= [27] Add output 0
- `B` <= [25] Absolute World Position output 0

### [27] Add

- `A` <= [19] Transform3x3Matrix output 0
- `B` <= [26] Particle Position (Absolute) output 0

### [28] Multiply

- `A` <= [32] Append output 0
- `B` <= [29] Particle Size output 0

### [31] 1-x

- `Input` <= [30] BreakOutFloat2Components output 1

### [32] Append

- `A` <= [30] BreakOutFloat2Components output 0
- `B` <= [31] 1-x output 0

## Connection List

- [8] RotateAboutAxis output 0 -> Output Result.A
- [4] Multiply(,2) output 0 -> Clamp.Input
- [5] Mask ( R ) output 0 -> Multiply(,2).A
- [3] CameraDirectionVector output 0 -> Mask ( R ).Input
- [1] CameraDirectionVector output 0 -> Multiply.A
- [2] Clamp output 0 -> Multiply.B
- [6] Multiply output 0 -> Normalize.VectorInput
- [15] VertexNormalWS output 0 -> RotateAboutAxis.NormalizedRotationAxis
- [9] VectorToRadialValue output 1 -> RotateAboutAxis.RotationAngle
- [13] Particle Position (Absolute) output 0 -> RotateAboutAxis.PivotPoint
- [14] Absolute World Position output 0 -> RotateAboutAxis.Position
- [12] Particle Direction output 0 -> World Space to View Space TransformVector.Input
- [10] World Space to View Space TransformVector output 0 -> Mask ( G B ).Input
- [28] Multiply output 0 -> Append.A
- [18] 0 output 0 -> Append.B
- [22] Particle Direction output 0 -> Cross.A
- [23] CameraDirectionVector output 0 -> Cross.B
- [27] Add output 0 -> Subtract.A
- [25] Absolute World Position output 0 -> Subtract.B
- [19] Transform3x3Matrix output 0 -> Add.A
- [26] Particle Position (Absolute) output 0 -> Add.B
- [32] Append output 0 -> Multiply.A
- [29] Particle Size output 0 -> Multiply.B
- [30] BreakOutFloat2Components output 1 -> 1-x.Input
- [30] BreakOutFloat2Components output 0 -> Append.A
- [31] 1-x output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [8] RotateAboutAxis

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


