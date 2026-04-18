# TangentSpaceFlowComplex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/TangentSpaceFlowComplex.TangentSpaceFlowComplex`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Pushing the FlowTexture along the direction of the FlowDirection

## Inputs

- `FlowTexture` [FunctionInput_Texture2D]
- `FlowMap` [FunctionInput_Vector2] - Flow Direction in Tangent Space from -1 to 1
- `UVs` [FunctionInput_Vector2]
- `Time` [FunctionInput_Scalar]
- `Anim offset Noise` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input FlowTexture (Texture2D) | FlowTexture |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input FlowMap (Vector2) | FlowMap |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 6 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 7 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Time (Scalar) | Time |
| 10 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 12 | expression | `MaterialExpressionConstant` | -0.25 |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 14 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionFrac` | Frac |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearSine |  |
| 20 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 21 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 22 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 23 | expression | `MaterialExpressionFrac` | Frac |  |
| 24 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 27 | expression | `MaterialExpressionFrac` | Frac |  |
| 28 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 29 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 30 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 32 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 33 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 34 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0.75,1.25,) |  |
| 35 | expression | `MaterialExpressionAdd` | Add |  |
| 36 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 37 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 38 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 39 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 40 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 41 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 42 | function_input | `MaterialExpressionFunctionInput` | Input Anim offset Noise (Scalar) | Anim offset Noise |
| 43 | expression | `MaterialExpressionAdd` | Add |  |
| 44 | expression | `MaterialExpressionTextureObject` | Texture Object |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [30] Lerp output 0

### [1] Texture Sample

- `Coordinates` <= [29] Add(,0.5) output 0
- `TextureObject` <= [2] Input FlowTexture (Texture2D) output 0

### [2] Input FlowTexture (Texture2D)

- `Preview` <= [44] Texture Object output 0

### [3] Add

- `A` <= [5] Input UVs (Vector2) output 0
- `B` <= [24] Multiply output 0

### [4] Input FlowMap (Vector2)

- `Preview` <= [31] Multiply(,3) output 0

### [5] Input UVs (Vector2)

- `Preview` <= [6] TexCoord[0] output 0

### [7] Texture Sample

- `Coordinates` <= [8] Add output 0
- `TextureObject` <= [2] Input FlowTexture (Texture2D) output 0

### [8] Add

- `A` <= [25] Multiply output 0
- `B` <= [5] Input UVs (Vector2) output 0

### [9] Input Time (Scalar)

- `Preview` <= [11] TimeWithSpeedVariable output 1

### [13] Mask ( R G )

- `Input` <= [20] Texture Sample output 0

### [14] 1-x

- `Input` <= [19] LinearSine output 0

### [15] Multiply

- `A` <= [19] LinearSine output 0
- `B` <= [7] Texture Sample output 0

### [16] Add

- `A` <= [15] Multiply output 0
- `B` <= [17] Multiply output 0

### [17] Multiply

- `A` <= [14] 1-x output 0
- `B` <= [1] Texture Sample output 0

### [18] Frac

- `Input` <= [3] Add output 0

### [21] Subtract(,0.5)

- `A` <= [13] Mask ( R G ) output 0

### [22] Texture Sample

- `Coordinates` <= [33] TexCoord[0] output 0

### [23] Frac

- `Input` <= [43] Add output 0

### [24] Multiply

- `A` <= [27] Frac output 0
- `B` <= [32] Multiply output 0

### [25] Multiply

- `A` <= [32] Multiply output 0
- `B` <= [23] Frac output 0

### [26] Subtract(,0.5)

- `A` <= [43] Add output 0

### [27] Frac

- `Input` <= [26] Subtract(,0.5) output 0

### [29] Add(,0.5)

- `A` <= [18] Frac output 0

### [30] Lerp

- `A` <= [1] Texture Sample output 0
- `B` <= [7] Texture Sample output 0
- `Alpha` <= [19] LinearSine output 0

### [31] Multiply(,3)

- `A` <= [21] Subtract(,0.5) output 0

### [32] Multiply

- `A` <= [4] Input FlowMap (Vector2) output 0
- `B` <= [22] Texture Sample output 1

### [34] Lerp(0.75,1.25,)

- `Alpha` <= [22] Texture Sample output 1

### [35] Add

- `A` <= [32] Multiply output 0
- `B` <= [32] Multiply output 0

### [36] Texture Sample

- `Coordinates` <= [37] TexCoord[0] output 0

### [38] Texture Sample

- `Coordinates` <= [39] TexCoord[0] output 0

### [40] Texture Sample

- `Coordinates` <= [41] TexCoord[0] output 0

### [42] Input Anim offset Noise (Scalar)

- `Preview` <= [40] Texture Sample output 1

### [43] Add

- `A` <= [42] Input Anim offset Noise (Scalar) output 0
- `B` <= [9] Input Time (Scalar) output 0

## Connection List

- [30] Lerp output 0 -> Output Result.A
- [29] Add(,0.5) output 0 -> Texture Sample.Coordinates
- [2] Input FlowTexture (Texture2D) output 0 -> Texture Sample.TextureObject
- [44] Texture Object output 0 -> Input FlowTexture (Texture2D).Preview
- [5] Input UVs (Vector2) output 0 -> Add.A
- [24] Multiply output 0 -> Add.B
- [31] Multiply(,3) output 0 -> Input FlowMap (Vector2).Preview
- [6] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [8] Add output 0 -> Texture Sample.Coordinates
- [2] Input FlowTexture (Texture2D) output 0 -> Texture Sample.TextureObject
- [25] Multiply output 0 -> Add.A
- [5] Input UVs (Vector2) output 0 -> Add.B
- [11] TimeWithSpeedVariable output 1 -> Input Time (Scalar).Preview
- [20] Texture Sample output 0 -> Mask ( R G ).Input
- [19] LinearSine output 0 -> 1-x.Input
- [19] LinearSine output 0 -> Multiply.A
- [7] Texture Sample output 0 -> Multiply.B
- [15] Multiply output 0 -> Add.A
- [17] Multiply output 0 -> Add.B
- [14] 1-x output 0 -> Multiply.A
- [1] Texture Sample output 0 -> Multiply.B
- [3] Add output 0 -> Frac.Input
- [13] Mask ( R G ) output 0 -> Subtract(,0.5).A
- [33] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [43] Add output 0 -> Frac.Input
- [27] Frac output 0 -> Multiply.A
- [32] Multiply output 0 -> Multiply.B
- [32] Multiply output 0 -> Multiply.A
- [23] Frac output 0 -> Multiply.B
- [43] Add output 0 -> Subtract(,0.5).A
- [26] Subtract(,0.5) output 0 -> Frac.Input
- [18] Frac output 0 -> Add(,0.5).A
- [1] Texture Sample output 0 -> Lerp.A
- [7] Texture Sample output 0 -> Lerp.B
- [19] LinearSine output 0 -> Lerp.Alpha
- [21] Subtract(,0.5) output 0 -> Multiply(,3).A
- [4] Input FlowMap (Vector2) output 0 -> Multiply.A
- [22] Texture Sample output 1 -> Multiply.B
- [22] Texture Sample output 1 -> Lerp(0.75,1.25,).Alpha
- [32] Multiply output 0 -> Add.A
- [32] Multiply output 0 -> Add.B
- [37] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [39] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [41] TexCoord[0] output 0 -> Texture Sample.Coordinates
- [40] Texture Sample output 1 -> Input Anim offset Noise (Scalar).Preview
- [42] Input Anim offset Noise (Scalar) output 0 -> Add.A
- [9] Input Time (Scalar) output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [30] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
