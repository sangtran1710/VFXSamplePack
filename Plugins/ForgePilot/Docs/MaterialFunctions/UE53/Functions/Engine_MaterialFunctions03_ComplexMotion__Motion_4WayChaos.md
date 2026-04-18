# Motion_4WayChaos

- Path: `/Engine/Functions/Engine_MaterialFunctions03/ComplexMotion/Motion_4WayChaos.Motion_4WayChaos`
- Category: Engine_MaterialFunctions03/ComplexMotion
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

4-way diagonal motion

## Inputs

- `Texture` [FunctionInput_Texture2D]
- `Coordinates` [FunctionInput_Vector2]
- `Speed` [FunctionInput_Scalar]
- `Divisor` [FunctionInput_Scalar] - How much to reduce the overall combined results of the 4 way add.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 3 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Texture2D) | Texture |
| 6 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 7 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector2) | Coordinates |
| 9 | expression | `MaterialExpressionPanner` | Panner |  |
| 10 | expression | `MaterialExpressionPanner` | Panner |  |
| 11 | expression | `MaterialExpressionPanner` | Panner |  |
| 12 | expression | `MaterialExpressionPanner` | Panner |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionConstant2Vector` | 0.418,0.355 |  |
| 17 | expression | `MaterialExpressionConstant2Vector` | 0.865,0.148 |  |
| 18 | expression | `MaterialExpressionConstant2Vector` | 0.651,0.752 |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Speed (Scalar) | Speed |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionTime` | Time |  |
| 22 | expression | `MaterialExpressionAdd` | Add |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Divisor (Scalar) | Divisor |
| 27 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 28 | expression | `MaterialExpressionQualitySwitch` | Quality Switch |  |
| 29 | expression | `MaterialExpressionConstant` | 0 |  |
| 30 | expression | `MaterialExpressionConstant` | 0 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [25] Multiply output 0

### [1] Texture Sample

- `Coordinates` <= [9] Panner output 0
- `TextureObject` <= [5] Input Texture (Texture2D) output 0

### [2] Texture Sample

- `Coordinates` <= [10] Panner output 0
- `TextureObject` <= [5] Input Texture (Texture2D) output 0

### [3] Texture Sample

- `Coordinates` <= [11] Panner output 0
- `TextureObject` <= [5] Input Texture (Texture2D) output 0

### [4] Texture Sample

- `Coordinates` <= [12] Panner output 0
- `TextureObject` <= [5] Input Texture (Texture2D) output 0

### [5] Input Texture (Texture2D)

- `Preview` <= [6] Texture Object output 0

### [8] Input Coordinates (Vector2)

- `Preview` <= [7] TexCoord[0] output 0

### [9] Panner

- `Coordinate` <= [8] Input Coordinates (Vector2) output 0
- `Time` <= [20] Multiply output 0

### [10] Panner

- `Coordinate` <= [13] Add output 0
- `Time` <= [20] Multiply output 0

### [11] Panner

- `Coordinate` <= [14] Add output 0
- `Time` <= [20] Multiply output 0

### [12] Panner

- `Coordinate` <= [15] Add output 0
- `Time` <= [20] Multiply output 0

### [13] Add

- `A` <= [8] Input Coordinates (Vector2) output 0
- `B` <= [16] 0.418,0.355 output 0

### [14] Add

- `A` <= [8] Input Coordinates (Vector2) output 0
- `B` <= [17] 0.865,0.148 output 0

### [15] Add

- `A` <= [8] Input Coordinates (Vector2) output 0
- `B` <= [18] 0.651,0.752 output 0

### [20] Multiply

- `A` <= [19] Input Speed (Scalar) output 0
- `B` <= [21] Time output 0

### [22] Add

- `A` <= [27] Quality Switch output 0
- `B` <= [2] Texture Sample output 0

### [23] Add

- `A` <= [3] Texture Sample output 0
- `B` <= [4] Texture Sample output 0

### [24] Add

- `A` <= [22] Add output 0
- `B` <= [28] Quality Switch output 0

### [25] Multiply

- `A` <= [24] Add output 0
- `B` <= [26] Input Divisor (Scalar) output 0

### [27] Quality Switch

- `Default` <= [1] Texture Sample output 0
- `Inputs` <= [30] 0 output 0

### [28] Quality Switch

- `Default` <= [23] Add output 0
- `Inputs` <= [29] 0 output 0

## Connection List

- [25] Multiply output 0 -> Output Result.A
- [9] Panner output 0 -> Texture Sample.Coordinates
- [5] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [10] Panner output 0 -> Texture Sample.Coordinates
- [5] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [11] Panner output 0 -> Texture Sample.Coordinates
- [5] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [12] Panner output 0 -> Texture Sample.Coordinates
- [5] Input Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [6] Texture Object output 0 -> Input Texture (Texture2D).Preview
- [7] TexCoord[0] output 0 -> Input Coordinates (Vector2).Preview
- [8] Input Coordinates (Vector2) output 0 -> Panner.Coordinate
- [20] Multiply output 0 -> Panner.Time
- [13] Add output 0 -> Panner.Coordinate
- [20] Multiply output 0 -> Panner.Time
- [14] Add output 0 -> Panner.Coordinate
- [20] Multiply output 0 -> Panner.Time
- [15] Add output 0 -> Panner.Coordinate
- [20] Multiply output 0 -> Panner.Time
- [8] Input Coordinates (Vector2) output 0 -> Add.A
- [16] 0.418,0.355 output 0 -> Add.B
- [8] Input Coordinates (Vector2) output 0 -> Add.A
- [17] 0.865,0.148 output 0 -> Add.B
- [8] Input Coordinates (Vector2) output 0 -> Add.A
- [18] 0.651,0.752 output 0 -> Add.B
- [19] Input Speed (Scalar) output 0 -> Multiply.A
- [21] Time output 0 -> Multiply.B
- [27] Quality Switch output 0 -> Add.A
- [2] Texture Sample output 0 -> Add.B
- [3] Texture Sample output 0 -> Add.A
- [4] Texture Sample output 0 -> Add.B
- [22] Add output 0 -> Add.A
- [28] Quality Switch output 0 -> Add.B
- [24] Add output 0 -> Multiply.A
- [26] Input Divisor (Scalar) output 0 -> Multiply.B
- [1] Texture Sample output 0 -> Quality Switch.Default
- [30] 0 output 0 -> Quality Switch.Inputs
- [23] Add output 0 -> Quality Switch.Default
- [29] 0 output 0 -> Quality Switch.Inputs

## Output Trace Roots

- `Result` is driven by [25] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
