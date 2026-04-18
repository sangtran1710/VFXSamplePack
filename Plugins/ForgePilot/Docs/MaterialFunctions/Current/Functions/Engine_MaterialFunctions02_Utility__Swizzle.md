# Swizzle

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Swizzle.Swizzle`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `XY` [FunctionInput_Vector2] - Input the Vector2 you want swizzled
- `XYZ` [FunctionInput_Vector3] - Input Vector3 you want Swizzled

## Outputs

- `YX`
- `YXZ`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output YX | YX |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input XY (Vector2) | XY |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input XYZ (Vector3) | XYZ |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output YXZ | YXZ |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionAppendVector` | Append |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |

## Exact Input Wiring

### [0] Output YX

- `A` <= [4] Append output 0

### [1] Input XY (Vector2)

- `Preview` <= [5] TexCoord[0] output 0

### [2] Mask ( R )

- `Input` <= [1] Input XY (Vector2) output 0

### [3] Mask ( G )

- `Input` <= [1] Input XY (Vector2) output 0

### [4] Append

- `A` <= [3] Mask ( G ) output 0
- `B` <= [2] Mask ( R ) output 0

### [6] Input XYZ (Vector3)

- `Preview` <= [13] 0,0,0 output 0

### [7] Output YXZ

- `A` <= [12] Append output 0

### [8] Mask ( R )

- `Input` <= [6] Input XYZ (Vector3) output 0

### [9] Append

- `A` <= [10] Mask ( G ) output 0
- `B` <= [8] Mask ( R ) output 0

### [10] Mask ( G )

- `Input` <= [6] Input XYZ (Vector3) output 0

### [11] Mask ( B )

- `Input` <= [6] Input XYZ (Vector3) output 0

### [12] Append

- `A` <= [9] Append output 0
- `B` <= [11] Mask ( B ) output 0

## Connection List

- [4] Append output 0 -> Output YX.A
- [5] TexCoord[0] output 0 -> Input XY (Vector2).Preview
- [1] Input XY (Vector2) output 0 -> Mask ( R ).Input
- [1] Input XY (Vector2) output 0 -> Mask ( G ).Input
- [3] Mask ( G ) output 0 -> Append.A
- [2] Mask ( R ) output 0 -> Append.B
- [13] 0,0,0 output 0 -> Input XYZ (Vector3).Preview
- [12] Append output 0 -> Output YXZ.A
- [6] Input XYZ (Vector3) output 0 -> Mask ( R ).Input
- [10] Mask ( G ) output 0 -> Append.A
- [8] Mask ( R ) output 0 -> Append.B
- [6] Input XYZ (Vector3) output 0 -> Mask ( G ).Input
- [6] Input XYZ (Vector3) output 0 -> Mask ( B ).Input
- [9] Append output 0 -> Append.A
- [11] Mask ( B ) output 0 -> Append.B

## Output Trace Roots

- `YX` is driven by [4] Append
- `YXZ` is driven by [12] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


