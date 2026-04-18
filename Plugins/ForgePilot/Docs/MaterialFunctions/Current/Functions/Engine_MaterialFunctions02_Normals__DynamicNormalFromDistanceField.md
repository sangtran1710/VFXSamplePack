# DynamicNormalFromDistanceField

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Normals/DynamicNormalFromDistanceField.DynamicNormalFromDistanceField`
- Category: Engine_MaterialFunctions02/Normals
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Generate animated normals from offset distance fields using this material function. A normal lip will form as the distance field errodes away using a user specified alpha value. Note: multiple distance fields can dynamically be combined together using a "min" operation to generate fluid results. If you wish to invert the results invert the distance field before feeding it into the function. 

## Inputs

- `Normal Lip Curvature` [FunctionInput_Scalar]
- `Normal Lip Tightness` [FunctionInput_Scalar]
- `Right Shifted Distance Field` [FunctionInput_Scalar] - Create a texture that is a distance field, offset the red channel x units to the right and the green channel x units upward.
- `Dissolve Alpha` [FunctionInput_Scalar]
- `Downward Shifted Distance Field` [FunctionInput_Scalar] - Create a texture that is a distance field, offset the red channel x units to the right and the green channel x units upward.
- `Unshifted distance field` [FunctionInput_Scalar] - Create a texture that is a distance field, offset the red channel x units to the right and the green channel x units upward.

## Outputs

- `Normal`
- `Mask`
- `Normal XY (Advanced)` - This output can be added with similar outputs before Derive Normal Z is run on the composited value. This is exposed to save instructions in that scenario.
- `Thresholded Gradients (Advanced)` - This output can be used to generate custom normals or other operations. The blue channel can be used to represent a psuedo thickness value.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionConstant` | 1 |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionConstant` | 2 |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionAppendVector` | Append |  |
| 12 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 16 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 18 | expression | `MaterialExpressionPower` | Power |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 20 | function_call | `MaterialExpressionMaterialFunctionCall` | HeightLerp |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Normal Lip Curvature (Scalar) | Normal Lip Curvature |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Normal Lip Tightness (Scalar) | Normal Lip Tightness |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Right Shifted Distance Field (Scalar) | Right Shifted Distance Field |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Dissolve Alpha (Scalar) | Dissolve Alpha |
| 26 | function_output | `MaterialExpressionFunctionOutput` | Output Mask | Mask |
| 27 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 28 | expression | `MaterialExpressionConstant` | -0.25 |  |
| 29 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Downward Shifted Distance Field (Scalar) | Downward Shifted Distance Field |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Unshifted distance field (Scalar) | Unshifted distance field |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 33 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 34 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 35 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 36 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 37 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 38 | function_output | `MaterialExpressionFunctionOutput` | Output Normal XY (Advanced) | Normal XY (Advanced) |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output Thresholded Gradients (Advanced) | Thresholded Gradients (Advanced) |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [12] DeriveNormalZ output 0

### [1] Texture Sample

- `Coordinates` <= [35] TexCoord[0] output 0

### [3] Subtract

- `A` <= [34] Reroute Node (reroutes wires) output 0
- `B` <= [4] 1 output 0

### [5] Multiply

- `A` <= [27] 1-x output 0
- `B` <= [6] 2 output 0

### [7] Add

- `A` <= [3] Subtract output 0
- `B` <= [5] Multiply output 0

### [8] Clamp

- `Input` <= [7] Add output 0

### [9] Subtract

- `A` <= [19] BreakOutFloat3Components output 0
- `B` <= [19] BreakOutFloat3Components output 2

### [10] Subtract

- `A` <= [19] BreakOutFloat3Components output 1
- `B` <= [19] BreakOutFloat3Components output 2

### [11] Append

- `A` <= [9] Subtract output 0
- `B` <= [10] Subtract output 0

### [12] DeriveNormalZ

- `InXY` <= [11] Append output 0

### [18] Power

- `Base` <= [17] MakeFloat3 output 0
- `Exponent` <= [22] Input Normal Lip Curvature (Scalar) output 0

### [24] Input Right Shifted Distance Field (Scalar)

- `Preview` <= [1] Texture Sample output 1

### [25] Input Dissolve Alpha (Scalar)

- `Preview` <= [20] HeightLerp output 1

### [26] Output Mask

- `A` <= [37] Reroute Node (reroutes wires) output 0

### [27] 1-x

- `Input` <= [25] Input Dissolve Alpha (Scalar) output 0

### [29] 1-x

- `Input` <= [2] LinearGradient output 1

### [30] Input Downward Shifted Distance Field (Scalar)

- `Preview` <= [1] Texture Sample output 2

### [31] Input Unshifted distance field (Scalar)

- `Preview` <= [1] Texture Sample output 3

### [33] Reroute Node (reroutes wires)

- `Input` <= [32] MakeFloat3 output 0

### [34] Reroute Node (reroutes wires)

- `Input` <= [33] Reroute Node (reroutes wires) output 0

### [36] 1-x

- `Input` <= [18] Power output 0

### [37] Reroute Node (reroutes wires)

- `Input` <= [19] BreakOutFloat3Components output 2

### [38] Output Normal XY (Advanced)

- `A` <= [11] Append output 0

### [39] Output Thresholded Gradients (Advanced)

- `A` <= [8] Clamp output 0

## Connection List

- [12] DeriveNormalZ output 0 -> Output Normal.A
- [35] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [34] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [4] 1 output 0 -> Subtract.B
- [27] 1-x output 0 -> Multiply.A
- [6] 2 output 0 -> Multiply.B
- [3] Subtract output 0 -> Add.A
- [5] Multiply output 0 -> Add.B
- [7] Add output 0 -> Clamp.Input
- [19] BreakOutFloat3Components output 0 -> Subtract.A
- [19] BreakOutFloat3Components output 2 -> Subtract.B
- [19] BreakOutFloat3Components output 1 -> Subtract.A
- [19] BreakOutFloat3Components output 2 -> Subtract.B
- [9] Subtract output 0 -> Append.A
- [10] Subtract output 0 -> Append.B
- [11] Append output 0 -> DeriveNormalZ.InXY
- [17] MakeFloat3 output 0 -> Power.Base
- [22] Input Normal Lip Curvature (Scalar) output 0 -> Power.Exponent
- [1] Texture Sample output 1 -> Input Right Shifted Distance Field (Scalar).Preview
- [20] HeightLerp output 1 -> Input Dissolve Alpha (Scalar).Preview
- [37] Reroute Node (reroutes wires) output 0 -> Output Mask.A
- [25] Input Dissolve Alpha (Scalar) output 0 -> 1-x.Input
- [2] LinearGradient output 1 -> 1-x.Input
- [1] Texture Sample output 2 -> Input Downward Shifted Distance Field (Scalar).Preview
- [1] Texture Sample output 3 -> Input Unshifted distance field (Scalar).Preview
- [32] MakeFloat3 output 0 -> Reroute Node (reroutes wires).Input
- [33] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [18] Power output 0 -> 1-x.Input
- [19] BreakOutFloat3Components output 2 -> Reroute Node (reroutes wires).Input
- [11] Append output 0 -> Output Normal XY (Advanced).A
- [8] Clamp output 0 -> Output Thresholded Gradients (Advanced).A

## Output Trace Roots

- `Normal` is driven by [12] DeriveNormalZ
- `Mask` is driven by [37] Reroute Node (reroutes wires)
- `Normal XY (Advanced)` is driven by [11] Append
- `Thresholded Gradients (Advanced)` is driven by [8] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


