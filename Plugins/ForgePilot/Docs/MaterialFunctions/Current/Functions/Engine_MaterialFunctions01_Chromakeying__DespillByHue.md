# DespillByHue

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DespillByHue.DespillByHue`
- Category: Engine_MaterialFunctions01/Chromakeying
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Inputs

- `Texture` [FunctionInput_Vector3] - Green Screen Input
- `Despill Hue` [FunctionInput_Vector3]
- `Hue Range` [FunctionInput_Scalar] - Hue Range
- `DeSpill Amount` [FunctionInput_Scalar] - DeSpill Amount

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Texture (Vector3) | Texture |
| 2 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 3 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | SmoothStep |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | RGBtoHSV |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | RGBtoHSV |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionAbs` | Abs |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Despill Hue (Vector3) | Despill Hue |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Hue Range (Scalar) | Hue Range |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input DeSpill Amount (Scalar) | DeSpill Amount |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] Lerp output 0

### [2] Desaturation

- `Input` <= [1] Input Texture (Vector3) output 0
- `Fraction` <= [14] Input DeSpill Amount (Scalar) output 0

### [3] Lerp

- `A` <= [1] Input Texture (Vector3) output 0
- `B` <= [2] Desaturation output 0
- `Alpha` <= [4] SmoothStep output 0

### [6] Mask ( R )

- `Input` <= [5] RGBtoHSV output 0

### [8] Mask ( R )

- `Input` <= [7] RGBtoHSV output 0

### [9] Subtract

- `A` <= [6] Mask ( R ) output 0
- `B` <= [8] Mask ( R ) output 0

### [10] Abs

- `Input` <= [9] Subtract output 0

## Connection List

- [3] Lerp output 0 -> Output Result.A
- [1] Input Texture (Vector3) output 0 -> Desaturation.Input
- [14] Input DeSpill Amount (Scalar) output 0 -> Desaturation.Fraction
- [1] Input Texture (Vector3) output 0 -> Lerp.A
- [2] Desaturation output 0 -> Lerp.B
- [4] SmoothStep output 0 -> Lerp.Alpha
- [5] RGBtoHSV output 0 -> Mask ( R ).Input
- [7] RGBtoHSV output 0 -> Mask ( R ).Input
- [6] Mask ( R ) output 0 -> Subtract.A
- [8] Mask ( R ) output 0 -> Subtract.B
- [9] Subtract output 0 -> Abs.Input

## Output Trace Roots

- `Result` is driven by [3] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


