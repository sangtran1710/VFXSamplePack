# ConcatenateMatrices

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ConcatenateMatrices.ConcatenateMatrices`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Concatenate 2 matrices

## Inputs

- `BasisX` [FunctionInput_Vector4]
- `BasisY` [FunctionInput_Vector4]
- `BasisZ` [FunctionInput_Vector4]
- `BasisW` [FunctionInput_Vector4]
- `BasisX2` [FunctionInput_Vector4]
- `BasisY2` [FunctionInput_Vector4]
- `BasisZ2` [FunctionInput_Vector4]
- `BasisW2` [FunctionInput_Vector4]
- `----------` [FunctionInput_StaticBool]

## Outputs

- `X`
- `Y`
- `Z`
- `W`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output X | X |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input BasisX (Vector4) | BasisX |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input BasisY (Vector4) | BasisY |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input BasisZ (Vector4) | BasisZ |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input BasisW (Vector4) | BasisW |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input BasisX2 (Vector4) | BasisX2 |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input BasisY2 (Vector4) | BasisY2 |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input BasisZ2 (Vector4) | BasisZ2 |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input BasisW2 (Vector4) | BasisW2 |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output Y | Y |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Z | Z |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output W | W |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 20 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 22 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 23 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat4 |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat4 |  |
| 25 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat4 |  |
| 26 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat4 |  |
| 27 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 28 | expression | `MaterialExpressionAppendVector` | Append |  |
| 29 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 30 | expression | `MaterialExpressionAppendVector` | Append |  |
| 31 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 32 | expression | `MaterialExpressionAppendVector` | Append |  |
| 33 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 34 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 35 | expression | `MaterialExpressionAppendVector` | Append |  |
| 36 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 37 | expression | `MaterialExpressionAppendVector` | Append |  |
| 38 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 39 | expression | `MaterialExpressionAppendVector` | Append |  |
| 40 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 41 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 42 | expression | `MaterialExpressionAppendVector` | Append |  |
| 43 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 44 | expression | `MaterialExpressionAppendVector` | Append |  |
| 45 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 46 | expression | `MaterialExpressionAppendVector` | Append |  |
| 47 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input ---------- (StaticBool) | ---------- |

## Exact Input Wiring

### [0] Output X

- `A` <= [10] Append output 0

### [3] Dot

- `A` <= [1] Input BasisX (Vector4) output 0
- `B` <= [23] MakeFloat4 output 0

### [4] Append

- `A` <= [3] Dot output 0
- `B` <= [5] Dot output 0

### [5] Dot

- `A` <= [2] Input BasisY (Vector4) output 0
- `B` <= [23] MakeFloat4 output 0

### [7] Append

- `A` <= [4] Append output 0
- `B` <= [8] Dot output 0

### [8] Dot

- `A` <= [6] Input BasisZ (Vector4) output 0
- `B` <= [23] MakeFloat4 output 0

### [10] Append

- `A` <= [7] Append output 0
- `B` <= [11] Dot output 0

### [11] Dot

- `A` <= [9] Input BasisW (Vector4) output 0
- `B` <= [23] MakeFloat4 output 0

### [16] Output Y

- `A` <= [32] Append output 0

### [17] Output Z

- `A` <= [39] Append output 0

### [18] Output W

- `A` <= [46] Append output 0

### [27] Dot

- `A` <= [1] Input BasisX (Vector4) output 0
- `B` <= [24] MakeFloat4 output 0

### [28] Append

- `A` <= [27] Dot output 0
- `B` <= [29] Dot output 0

### [29] Dot

- `A` <= [2] Input BasisY (Vector4) output 0
- `B` <= [24] MakeFloat4 output 0

### [30] Append

- `A` <= [28] Append output 0
- `B` <= [31] Dot output 0

### [31] Dot

- `A` <= [6] Input BasisZ (Vector4) output 0
- `B` <= [24] MakeFloat4 output 0

### [32] Append

- `A` <= [30] Append output 0
- `B` <= [33] Dot output 0

### [33] Dot

- `A` <= [9] Input BasisW (Vector4) output 0
- `B` <= [24] MakeFloat4 output 0

### [34] Dot

- `A` <= [1] Input BasisX (Vector4) output 0
- `B` <= [25] MakeFloat4 output 0

### [35] Append

- `A` <= [34] Dot output 0
- `B` <= [36] Dot output 0

### [36] Dot

- `A` <= [2] Input BasisY (Vector4) output 0
- `B` <= [25] MakeFloat4 output 0

### [37] Append

- `A` <= [35] Append output 0
- `B` <= [38] Dot output 0

### [38] Dot

- `A` <= [6] Input BasisZ (Vector4) output 0
- `B` <= [25] MakeFloat4 output 0

### [39] Append

- `A` <= [37] Append output 0
- `B` <= [40] Dot output 0

### [40] Dot

- `A` <= [9] Input BasisW (Vector4) output 0
- `B` <= [25] MakeFloat4 output 0

### [41] Dot

- `A` <= [1] Input BasisX (Vector4) output 0
- `B` <= [26] MakeFloat4 output 0

### [42] Append

- `A` <= [41] Dot output 0
- `B` <= [43] Dot output 0

### [43] Dot

- `A` <= [2] Input BasisY (Vector4) output 0
- `B` <= [26] MakeFloat4 output 0

### [44] Append

- `A` <= [42] Append output 0
- `B` <= [45] Dot output 0

### [45] Dot

- `A` <= [6] Input BasisZ (Vector4) output 0
- `B` <= [26] MakeFloat4 output 0

### [46] Append

- `A` <= [44] Append output 0
- `B` <= [47] Dot output 0

### [47] Dot

- `A` <= [9] Input BasisW (Vector4) output 0
- `B` <= [26] MakeFloat4 output 0

## Connection List

- [10] Append output 0 -> Output X.A
- [1] Input BasisX (Vector4) output 0 -> Dot.A
- [23] MakeFloat4 output 0 -> Dot.B
- [3] Dot output 0 -> Append.A
- [5] Dot output 0 -> Append.B
- [2] Input BasisY (Vector4) output 0 -> Dot.A
- [23] MakeFloat4 output 0 -> Dot.B
- [4] Append output 0 -> Append.A
- [8] Dot output 0 -> Append.B
- [6] Input BasisZ (Vector4) output 0 -> Dot.A
- [23] MakeFloat4 output 0 -> Dot.B
- [7] Append output 0 -> Append.A
- [11] Dot output 0 -> Append.B
- [9] Input BasisW (Vector4) output 0 -> Dot.A
- [23] MakeFloat4 output 0 -> Dot.B
- [32] Append output 0 -> Output Y.A
- [39] Append output 0 -> Output Z.A
- [46] Append output 0 -> Output W.A
- [1] Input BasisX (Vector4) output 0 -> Dot.A
- [24] MakeFloat4 output 0 -> Dot.B
- [27] Dot output 0 -> Append.A
- [29] Dot output 0 -> Append.B
- [2] Input BasisY (Vector4) output 0 -> Dot.A
- [24] MakeFloat4 output 0 -> Dot.B
- [28] Append output 0 -> Append.A
- [31] Dot output 0 -> Append.B
- [6] Input BasisZ (Vector4) output 0 -> Dot.A
- [24] MakeFloat4 output 0 -> Dot.B
- [30] Append output 0 -> Append.A
- [33] Dot output 0 -> Append.B
- [9] Input BasisW (Vector4) output 0 -> Dot.A
- [24] MakeFloat4 output 0 -> Dot.B
- [1] Input BasisX (Vector4) output 0 -> Dot.A
- [25] MakeFloat4 output 0 -> Dot.B
- [34] Dot output 0 -> Append.A
- [36] Dot output 0 -> Append.B
- [2] Input BasisY (Vector4) output 0 -> Dot.A
- [25] MakeFloat4 output 0 -> Dot.B
- [35] Append output 0 -> Append.A
- [38] Dot output 0 -> Append.B
- [6] Input BasisZ (Vector4) output 0 -> Dot.A
- [25] MakeFloat4 output 0 -> Dot.B
- [37] Append output 0 -> Append.A
- [40] Dot output 0 -> Append.B
- [9] Input BasisW (Vector4) output 0 -> Dot.A
- [25] MakeFloat4 output 0 -> Dot.B
- [1] Input BasisX (Vector4) output 0 -> Dot.A
- [26] MakeFloat4 output 0 -> Dot.B
- [41] Dot output 0 -> Append.A
- [43] Dot output 0 -> Append.B
- [2] Input BasisY (Vector4) output 0 -> Dot.A
- [26] MakeFloat4 output 0 -> Dot.B
- [42] Append output 0 -> Append.A
- [45] Dot output 0 -> Append.B
- [6] Input BasisZ (Vector4) output 0 -> Dot.A
- [26] MakeFloat4 output 0 -> Dot.B
- [44] Append output 0 -> Append.A
- [47] Dot output 0 -> Append.B
- [9] Input BasisW (Vector4) output 0 -> Dot.A
- [26] MakeFloat4 output 0 -> Dot.B

## Output Trace Roots

- `X` is driven by [10] Append
- `Y` is driven by [32] Append
- `Z` is driven by [39] Append
- `W` is driven by [46] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
