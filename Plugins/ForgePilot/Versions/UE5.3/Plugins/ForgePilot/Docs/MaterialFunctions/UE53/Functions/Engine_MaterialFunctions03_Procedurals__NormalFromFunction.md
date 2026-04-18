# NormalFromFunction

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/NormalFromFunction.NormalFromFunction`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Takes a heightmap input and generates a normal map from it.

## Inputs

- `Coordinates` [FunctionInput_Vector2]
- `Height Map UV Offset` [FunctionInput_Scalar] - Spread used to derive the height from a scalar input. Default is .005
- `Normal Map Intensity` [FunctionInput_Scalar] - How strong the normal map will be.  Default is 8.
- `Function(UV1)` [FunctionInput_Vector3]
- `Function(UV2)` [FunctionInput_Vector3]
- `Function(UV3)` [FunctionInput_Vector3]

## Outputs

- `Normal`
- `UV1`
- `UV2`
- `UV3`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 1 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Coordinates (Vector2) | Coordinates |
| 6 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Height Map UV Offset (Scalar) | Height Map UV Offset |
| 10 | expression | `MaterialExpressionConstant` | 0.005 |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Normal Map Intensity (Scalar) | Normal Map Intensity |
| 13 | expression | `MaterialExpressionConstant2Vector` | 0,1 |  |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |
| 15 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionAppendVector` | Append |  |
| 18 | expression | `MaterialExpressionConstant2Vector` | 1,0 |  |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output UV1 | UV1 |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output UV2 | UV2 |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output UV3 | UV3 |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Function(UV1) (Vector3) | Function(UV1) |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Function(UV2) (Vector3) | Function(UV2) |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Function(UV3) (Vector3) | Function(UV3) |
| 25 | expression | `MaterialExpressionDivide` | Divide |  |
| 26 | expression | `MaterialExpressionDivide` | Divide |  |
| 27 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [1] Cross output 0

### [1] Cross

- `A` <= [14] Append output 0
- `B` <= [2] Append output 0

### [2] Append

- `A` <= [13] 0,1 output 0
- `B` <= [26] Divide output 0

### [3] Multiply

- `A` <= [12] Input Normal Map Intensity (Scalar) output 0
- `B` <= [9] Input Height Map UV Offset (Scalar) output 0

### [4] Subtract

- `A` <= [24] Input Function(UV3) (Vector3) output 0
- `B` <= [22] Input Function(UV1) (Vector3) output 0

### [5] Input Coordinates (Vector2)

- `Preview` <= [6] TexCoord[0] output 0

### [7] Add

- `A` <= [5] Input Coordinates (Vector2) output 0
- `B` <= [8] Append output 0

### [8] Append

- `A` <= [11] 0 output 0
- `B` <= [9] Input Height Map UV Offset (Scalar) output 0

### [9] Input Height Map UV Offset (Scalar)

- `Preview` <= [10] 0.005 output 0

### [14] Append

- `A` <= [18] 1,0 output 0
- `B` <= [25] Divide output 0

### [15] Subtract

- `A` <= [23] Input Function(UV2) (Vector3) output 0
- `B` <= [22] Input Function(UV1) (Vector3) output 0

### [16] Add

- `A` <= [17] Append output 0
- `B` <= [5] Input Coordinates (Vector2) output 0

### [17] Append

- `A` <= [9] Input Height Map UV Offset (Scalar) output 0
- `B` <= [11] 0 output 0

### [19] Output UV1

- `A` <= [5] Input Coordinates (Vector2) output 0

### [20] Output UV2

- `A` <= [16] Add output 0

### [21] Output UV3

- `A` <= [7] Add output 0

### [25] Divide

- `A` <= [15] Subtract output 0
- `B` <= [27] Divide output 0

### [26] Divide

- `A` <= [4] Subtract output 0
- `B` <= [27] Divide output 0

### [27] Divide

- `A` <= [9] Input Height Map UV Offset (Scalar) output 0
- `B` <= [12] Input Normal Map Intensity (Scalar) output 0

## Connection List

- [1] Cross output 0 -> Output Normal.A
- [14] Append output 0 -> Cross.A
- [2] Append output 0 -> Cross.B
- [13] 0,1 output 0 -> Append.A
- [26] Divide output 0 -> Append.B
- [12] Input Normal Map Intensity (Scalar) output 0 -> Multiply.A
- [9] Input Height Map UV Offset (Scalar) output 0 -> Multiply.B
- [24] Input Function(UV3) (Vector3) output 0 -> Subtract.A
- [22] Input Function(UV1) (Vector3) output 0 -> Subtract.B
- [6] TexCoord[0] output 0 -> Input Coordinates (Vector2).Preview
- [5] Input Coordinates (Vector2) output 0 -> Add.A
- [8] Append output 0 -> Add.B
- [11] 0 output 0 -> Append.A
- [9] Input Height Map UV Offset (Scalar) output 0 -> Append.B
- [10] 0.005 output 0 -> Input Height Map UV Offset (Scalar).Preview
- [18] 1,0 output 0 -> Append.A
- [25] Divide output 0 -> Append.B
- [23] Input Function(UV2) (Vector3) output 0 -> Subtract.A
- [22] Input Function(UV1) (Vector3) output 0 -> Subtract.B
- [17] Append output 0 -> Add.A
- [5] Input Coordinates (Vector2) output 0 -> Add.B
- [9] Input Height Map UV Offset (Scalar) output 0 -> Append.A
- [11] 0 output 0 -> Append.B
- [5] Input Coordinates (Vector2) output 0 -> Output UV1.A
- [16] Add output 0 -> Output UV2.A
- [7] Add output 0 -> Output UV3.A
- [15] Subtract output 0 -> Divide.A
- [27] Divide output 0 -> Divide.B
- [4] Subtract output 0 -> Divide.A
- [27] Divide output 0 -> Divide.B
- [9] Input Height Map UV Offset (Scalar) output 0 -> Divide.A
- [12] Input Normal Map Intensity (Scalar) output 0 -> Divide.B

## Output Trace Roots

- `Normal` is driven by [1] Cross
- `UV1` is driven by [5] Input Coordinates (Vector2)
- `UV2` is driven by [16] Add
- `UV3` is driven by [7] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
