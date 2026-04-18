# Posterize

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Posterize.Posterize`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Diffuse` [FunctionInput_Vector3]
- `Number of value steps` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant4Vector` | 1,1,1,1e-05 |  |
| 2 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionConstant` | 1 |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionCeil` | Ceil |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse (Vector3) | Diffuse |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Number of value steps (Scalar) | Number of value steps |
| 12 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 14 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] Multiply output 0

### [2] Dot

- `A` <= [1] 1,1,1,1e-05 output 0
- `B` <= [4] Append output 0

### [3] Divide

- `A` <= [10] Input Diffuse (Vector3) output 0
- `B` <= [2] Dot output 0

### [4] Append

- `A` <= [10] Input Diffuse (Vector3) output 0
- `B` <= [5] 1 output 0

### [6] Multiply

- `A` <= [2] Dot output 0
- `B` <= [11] Input Number of value steps (Scalar) output 0

### [7] Ceil

- `Input` <= [6] Multiply output 0

### [8] Multiply

- `A` <= [3] Divide output 0
- `B` <= [9] Divide output 0

### [9] Divide

- `A` <= [7] Ceil output 0
- `B` <= [11] Input Number of value steps (Scalar) output 0

### [10] Input Diffuse (Vector3)

- `Preview` <= [14] Append output 0

### [13] Mask ( G )

- `Input` <= [12] TexCoord[0] output 0

### [14] Append

- `A` <= [12] TexCoord[0] output 0
- `B` <= [13] Mask ( G ) output 0

## Connection List

- [8] Multiply output 0 -> Output Result.A
- [1] 1,1,1,1e-05 output 0 -> Dot.A
- [4] Append output 0 -> Dot.B
- [10] Input Diffuse (Vector3) output 0 -> Divide.A
- [2] Dot output 0 -> Divide.B
- [10] Input Diffuse (Vector3) output 0 -> Append.A
- [5] 1 output 0 -> Append.B
- [2] Dot output 0 -> Multiply.A
- [11] Input Number of value steps (Scalar) output 0 -> Multiply.B
- [6] Multiply output 0 -> Ceil.Input
- [3] Divide output 0 -> Multiply.A
- [9] Divide output 0 -> Multiply.B
- [7] Ceil output 0 -> Divide.A
- [11] Input Number of value steps (Scalar) output 0 -> Divide.B
- [14] Append output 0 -> Input Diffuse (Vector3).Preview
- [12] TexCoord[0] output 0 -> Mask ( G ).Input
- [12] TexCoord[0] output 0 -> Append.A
- [13] Mask ( G ) output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [8] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
