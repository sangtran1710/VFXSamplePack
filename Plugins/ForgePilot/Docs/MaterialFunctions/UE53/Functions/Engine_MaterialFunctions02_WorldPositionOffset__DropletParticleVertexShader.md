# DropletParticleVertexShader

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/DropletParticleVertexShader.DropletParticleVertexShader`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Make a camera aligned particle distort like a raindrop at different viewing angles. The X particle dimension define the sphere radius and the y particle dimension define the water drop length.

## Inputs

- `Speed Per unit of tail distortion` [FunctionInput_Scalar] - Defines how much speed is nessesary to distort the particle systems tail end by one world unit.
- `Max tail lengthing percentage` [FunctionInput_Scalar] - Maximum about of tail end distortion.
- `Min speed before distortion` [FunctionInput_Scalar]
- `Distortion lerp rate` [FunctionInput_Scalar] - Velocity divisor

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionClamp` | Clamp |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionParticleSpeed` | Particle Speed |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 6 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionAppendVector` | Append |  |
| 16 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 17 | expression | `MaterialExpressionConstant` | 0 |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 21 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionConstant3Vector` | -1,1,1 |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionConstant` | 0 |  |
| 27 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 28 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 29 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 31 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 32 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 33 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeVectorsOrthogonal |  |
| 34 | expression | `MaterialExpressionParticleDirection` | Particle Direction |  |
| 35 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 36 | expression | `MaterialExpressionAdd` | Add |  |
| 37 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 38 | expression | `MaterialExpressionParticlePositionWS` | Particle Position (Absolute) |  |
| 39 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 40 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 41 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 42 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 43 | expression | `MaterialExpressionClamp` | Clamp |  |
| 44 | expression | `MaterialExpressionAdd` | Add |  |
| 45 | expression | `MaterialExpressionClamp` | Clamp |  |
| 46 | expression | `MaterialExpressionDivide` | Divide |  |
| 47 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 48 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 49 | function_input | `MaterialExpressionFunctionInput` | Input Speed Per unit of tail distortion (Scalar) | Speed Per unit of tail distortion |
| 50 | function_input | `MaterialExpressionFunctionInput` | Input Max tail lengthing percentage (Scalar) | Max tail lengthing percentage |
| 51 | function_input | `MaterialExpressionFunctionInput` | Input Min speed before distortion (Scalar) | Min speed before distortion |
| 52 | function_input | `MaterialExpressionFunctionInput` | Input Distortion lerp rate (Scalar) | Distortion lerp rate |
| 53 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 54 | expression | `MaterialExpressionSubtract` | Subtract(1,) |  |
| 55 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 56 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 57 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 58 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 59 | expression | `MaterialExpressionParticleSize` | Particle Size |  |
| 60 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 61 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [32] Subtract output 0

### [1] Clamp

- `Input` <= [2] Divide output 0
- `Max` <= [50] Input Max tail lengthing percentage (Scalar) output 0

### [2] Divide

- `A` <= [3] Particle Speed output 0
- `B` <= [49] Input Speed Per unit of tail distortion (Scalar) output 0

### [4] Multiply

- `A` <= [60] Multiply output 0
- `B` <= [7] Mask ( R G ) output 0

### [5] World Space to Tangent Space TransformVector

- `Input` <= [6] Particle Direction output 0

### [7] Mask ( R G )

- `Input` <= [5] World Space to Tangent Space TransformVector output 0

### [8] Multiply

- `A` <= [4] Multiply output 0
- `B` <= [20] Subtract(,0.5) output 0

### [9] Dot

- `A` <= [7] Mask ( R G ) output 0
- `B` <= [20] Subtract(,0.5) output 0

### [10] Multiply

- `A` <= [12] Particle Size output 0
- `B` <= [19] Add output 0

### [11] Add

- `A` <= [8] Multiply output 0
- `B` <= [10] Multiply output 0

### [13] Multiply(,0.5)

- `A` <= [9] Dot output 0

### [14] Multiply

- `A` <= [13] Multiply(,0.5) output 0
- `B` <= [20] Subtract(,0.5) output 0

### [15] Append

- `A` <= [20] Subtract(,0.5) output 0
- `B` <= [17] 0 output 0

### [18] Multiply

- `A` <= [15] Append output 0
- `B` <= [23] -1,1,1 output 0

### [19] Add

- `A` <= [14] Multiply output 0
- `B` <= [20] Subtract(,0.5) output 0

### [20] Subtract(,0.5)

- `A` <= [16] TexCoord[0] output 0

### [21] View Space to World Space TransformVector

- `Input` <= [18] Multiply output 0

### [22] Append

- `A` <= [11] Add output 0
- `B` <= [26] 0 output 0

### [24] Add

- `A` <= [38] Particle Position (Absolute) output 0
- `B` <= [25] Multiply output 0

### [25] Multiply

- `A` <= [21] View Space to World Space TransformVector output 0
- `B` <= [28] Mask ( R ) output 0

### [28] Mask ( R )

- `Input` <= [27] Particle Size output 0

### [29] Lerp

- `A` <= [36] Add output 0
- `B` <= [24] Add output 0
- `Alpha` <= [43] Clamp output 0

### [31] Cross

- `A` <= [34] Particle Direction output 0
- `B` <= [37] CameraDirectionVector output 0

### [32] Subtract

- `A` <= [29] Lerp output 0
- `B` <= [35] Absolute World Position output 0

### [36] Add

- `A` <= [30] Transform3x3Matrix output 0
- `B` <= [38] Particle Position (Absolute) output 0

### [39] Subtract

- `A` <= [40] Camera Position output 0
- `B` <= [36] Add output 0

### [41] Normalize

- `VectorInput` <= [39] Subtract output 0

### [42] Mask ( B )

- `Input` <= [41] Normalize output 0

### [43] Clamp

- `Input` <= [44] Add output 0

### [44] Add

- `A` <= [53] Multiply output 0
- `B` <= [47] 1-x output 0

### [45] Clamp

- `Input` <= [46] Divide output 0

### [46] Divide

- `A` <= [48] Subtract output 0
- `B` <= [52] Input Distortion lerp rate (Scalar) output 0

### [47] 1-x

- `Input` <= [45] Clamp output 0

### [48] Subtract

- `A` <= [3] Particle Speed output 0
- `B` <= [51] Input Min speed before distortion (Scalar) output 0

### [53] Multiply

- `A` <= [42] Mask ( B ) output 0
- `B` <= [42] Mask ( B ) output 0

### [54] Subtract(1,)

- `B` <= [55] Camera Position output 0

### [56] Normalize

- `VectorInput` <= [54] Subtract(1,) output 0

### [57] Multiply(,1)

- `A` <= [56] Normalize output 0

### [58] Add(,1)

- `A` <= [57] Multiply(,1) output 0

### [60] Multiply

- `A` <= [1] Clamp output 0
- `B` <= [61] Mask ( G ) output 0

### [61] Mask ( G )

- `Input` <= [59] Particle Size output 0

## Connection List

- [32] Subtract output 0 -> Output Result.A
- [2] Divide output 0 -> Clamp.Input
- [50] Input Max tail lengthing percentage (Scalar) output 0 -> Clamp.Max
- [3] Particle Speed output 0 -> Divide.A
- [49] Input Speed Per unit of tail distortion (Scalar) output 0 -> Divide.B
- [60] Multiply output 0 -> Multiply.A
- [7] Mask ( R G ) output 0 -> Multiply.B
- [6] Particle Direction output 0 -> World Space to Tangent Space TransformVector.Input
- [5] World Space to Tangent Space TransformVector output 0 -> Mask ( R G ).Input
- [4] Multiply output 0 -> Multiply.A
- [20] Subtract(,0.5) output 0 -> Multiply.B
- [7] Mask ( R G ) output 0 -> Dot.A
- [20] Subtract(,0.5) output 0 -> Dot.B
- [12] Particle Size output 0 -> Multiply.A
- [19] Add output 0 -> Multiply.B
- [8] Multiply output 0 -> Add.A
- [10] Multiply output 0 -> Add.B
- [9] Dot output 0 -> Multiply(,0.5).A
- [13] Multiply(,0.5) output 0 -> Multiply.A
- [20] Subtract(,0.5) output 0 -> Multiply.B
- [20] Subtract(,0.5) output 0 -> Append.A
- [17] 0 output 0 -> Append.B
- [15] Append output 0 -> Multiply.A
- [23] -1,1,1 output 0 -> Multiply.B
- [14] Multiply output 0 -> Add.A
- [20] Subtract(,0.5) output 0 -> Add.B
- [16] TexCoord[0] output 0 -> Subtract(,0.5).A
- [18] Multiply output 0 -> View Space to World Space TransformVector.Input
- [11] Add output 0 -> Append.A
- [26] 0 output 0 -> Append.B
- [38] Particle Position (Absolute) output 0 -> Add.A
- [25] Multiply output 0 -> Add.B
- [21] View Space to World Space TransformVector output 0 -> Multiply.A
- [28] Mask ( R ) output 0 -> Multiply.B
- [27] Particle Size output 0 -> Mask ( R ).Input
- [36] Add output 0 -> Lerp.A
- [24] Add output 0 -> Lerp.B
- [43] Clamp output 0 -> Lerp.Alpha
- [34] Particle Direction output 0 -> Cross.A
- [37] CameraDirectionVector output 0 -> Cross.B
- [29] Lerp output 0 -> Subtract.A
- [35] Absolute World Position output 0 -> Subtract.B
- [30] Transform3x3Matrix output 0 -> Add.A
- [38] Particle Position (Absolute) output 0 -> Add.B
- [40] Camera Position output 0 -> Subtract.A
- [36] Add output 0 -> Subtract.B
- [39] Subtract output 0 -> Normalize.VectorInput
- [41] Normalize output 0 -> Mask ( B ).Input
- [44] Add output 0 -> Clamp.Input
- [53] Multiply output 0 -> Add.A
- [47] 1-x output 0 -> Add.B
- [46] Divide output 0 -> Clamp.Input
- [48] Subtract output 0 -> Divide.A
- [52] Input Distortion lerp rate (Scalar) output 0 -> Divide.B
- [45] Clamp output 0 -> 1-x.Input
- [3] Particle Speed output 0 -> Subtract.A
- [51] Input Min speed before distortion (Scalar) output 0 -> Subtract.B
- [42] Mask ( B ) output 0 -> Multiply.A
- [42] Mask ( B ) output 0 -> Multiply.B
- [55] Camera Position output 0 -> Subtract(1,).B
- [54] Subtract(1,) output 0 -> Normalize.VectorInput
- [56] Normalize output 0 -> Multiply(,1).A
- [57] Multiply(,1) output 0 -> Add(,1).A
- [1] Clamp output 0 -> Multiply.A
- [61] Mask ( G ) output 0 -> Multiply.B
- [59] Particle Size output 0 -> Mask ( G ).Input

## Output Trace Roots

- `Result` is driven by [32] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
