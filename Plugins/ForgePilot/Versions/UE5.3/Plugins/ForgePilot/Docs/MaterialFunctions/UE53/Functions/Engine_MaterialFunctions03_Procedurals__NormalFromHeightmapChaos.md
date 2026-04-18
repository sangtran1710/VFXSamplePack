# NormalFromHeightmapChaos

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/NormalFromHeightmapChaos.NormalFromHeightmapChaos`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

EXPENSIVE - Takes a heightmap input, pans it 4 ways and generates a normal map from it.

## Inputs

- `Coordinates` [FunctionInput_Vector2]
- `Height Bias` [FunctionInput_Scalar] - Spread used to derive the height from a scalar input. Default is .005
- `Height` [FunctionInput_Scalar] - How strong the normal map will be.  Default is 8.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | Motion_4WayChaos |  |
| 7 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector2) | Coordinates |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | Motion_4WayChaos |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Height Bias (Scalar) | Height Bias |
| 15 | expression | `MaterialExpressionConstant` | 0.005 |  |
| 16 | expression | `MaterialExpressionConstant` | 0 |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Height (Scalar) | Height |
| 18 | expression | `MaterialExpressionConstant` | 8 |  |
| 19 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 20 | expression | `MaterialExpressionAppendVector` | Append |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | Motion_4WayChaos |  |
| 25 | expression | `MaterialExpressionAdd` | Add |  |
| 26 | expression | `MaterialExpressionAppendVector` | Append |  |
| 27 | expression | `MaterialExpressionConstant2Vector` | 1,0 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Cross output 0

### [1] Cross

- `A` <= [20] Append output 0
- `B` <= [2] Append output 0

### [2] Append

- `A` <= [19] 0,1 output 0
- `B` <= [3] Multiply output 0

### [3] Multiply

- `A` <= [17] Input Height (Scalar) output 0
- `B` <= [4] Subtract output 0

### [4] Subtract

- `A` <= [10] Mask ( R ) output 0
- `B` <= [5] Mask ( R ) output 0

### [5] Mask ( R )

- `Input` <= [6] Motion_4WayChaos output 0

### [8] Input Coordinates (Vector2)

- `Preview` <= [9] TexCoord[0] output 0

### [10] Mask ( R )

- `Input` <= [11] Motion_4WayChaos output 0

### [12] Add

- `A` <= [8] Input Coordinates (Vector2) output 0
- `B` <= [13] Append output 0

### [13] Append

- `A` <= [16] 0 output 0
- `B` <= [14] Input Height Bias (Scalar) output 0

### [14] Input Height Bias (Scalar)

- `Preview` <= [15] 0.005 output 0

### [17] Input Height (Scalar)

- `Preview` <= [18] 8 output 0

### [20] Append

- `A` <= [27] 1,0 output 0
- `B` <= [21] Multiply output 0

### [21] Multiply

- `A` <= [22] Subtract output 0
- `B` <= [17] Input Height (Scalar) output 0

### [22] Subtract

- `A` <= [23] Mask ( R ) output 0
- `B` <= [5] Mask ( R ) output 0

### [23] Mask ( R )

- `Input` <= [24] Motion_4WayChaos output 0

### [25] Add

- `A` <= [26] Append output 0
- `B` <= [8] Input Coordinates (Vector2) output 0

### [26] Append

- `A` <= [14] Input Height Bias (Scalar) output 0
- `B` <= [16] 0 output 0

## Connection List

- [1] Cross output 0 -> Output Result.A
- [20] Append output 0 -> Cross.A
- [2] Append output 0 -> Cross.B
- [19] 0,1 output 0 -> Append.A
- [3] Multiply output 0 -> Append.B
- [17] Input Height (Scalar) output 0 -> Multiply.A
- [4] Subtract output 0 -> Multiply.B
- [10] Mask ( R ) output 0 -> Subtract.A
- [5] Mask ( R ) output 0 -> Subtract.B
- [6] Motion_4WayChaos output 0 -> Mask ( R ).Input
- [9] TexCoord[0] output 0 -> Input Coordinates (Vector2).Preview
- [11] Motion_4WayChaos output 0 -> Mask ( R ).Input
- [8] Input Coordinates (Vector2) output 0 -> Add.A
- [13] Append output 0 -> Add.B
- [16] 0 output 0 -> Append.A
- [14] Input Height Bias (Scalar) output 0 -> Append.B
- [15] 0.005 output 0 -> Input Height Bias (Scalar).Preview
- [18] 8 output 0 -> Input Height (Scalar).Preview
- [27] 1,0 output 0 -> Append.A
- [21] Multiply output 0 -> Append.B
- [22] Subtract output 0 -> Multiply.A
- [17] Input Height (Scalar) output 0 -> Multiply.B
- [23] Mask ( R ) output 0 -> Subtract.A
- [5] Mask ( R ) output 0 -> Subtract.B
- [24] Motion_4WayChaos output 0 -> Mask ( R ).Input
- [26] Append output 0 -> Add.A
- [8] Input Coordinates (Vector2) output 0 -> Add.B
- [14] Input Height Bias (Scalar) output 0 -> Append.A
- [16] 0 output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [1] Cross

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
