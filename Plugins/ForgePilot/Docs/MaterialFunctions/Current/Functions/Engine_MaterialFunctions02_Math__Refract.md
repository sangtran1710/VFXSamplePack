# Refract

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/Refract.Refract`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `RatioOfIndicesOfRefraction` [FunctionInput_Scalar]
- `IncidenceVector` [FunctionInput_Vector3]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input RatioOfIndicesOfRefraction (Scalar) | RatioOfIndicesOfRefraction |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionConstant3Vector` | -1,-1,-1 |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input IncidenceVector (Vector3) | IncidenceVector |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 18 | expression | `MaterialExpressionAbs` | Abs |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [13] Add output 0
- `B` <= [2] 1-x output 0

### [2] 1-x

- `Input` <= [3] Multiply output 0

### [3] Multiply

- `A` <= [6] 1-x output 0
- `B` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [5] Input RatioOfIndicesOfRefraction (Scalar) output 0
- `B` <= [5] Input RatioOfIndicesOfRefraction (Scalar) output 0

### [6] 1-x

- `Input` <= [7] Multiply output 0

### [7] Multiply

- `A` <= [8] Dot output 0
- `B` <= [8] Dot output 0

### [8] Dot

- `A` <= [12] Input Normal (Vector3) output 0
- `B` <= [9] Multiply output 0

### [9] Multiply

- `A` <= [11] Input IncidenceVector (Vector3) output 0
- `B` <= [10] -1,-1,-1 output 0

### [13] Add

- `A` <= [15] Multiply output 0
- `B` <= [14] Multiply output 0

### [14] Multiply

- `A` <= [11] Input IncidenceVector (Vector3) output 0
- `B` <= [5] Input RatioOfIndicesOfRefraction (Scalar) output 0

### [15] Multiply

- `A` <= [12] Input Normal (Vector3) output 0
- `B` <= [16] Subtract output 0

### [16] Subtract

- `A` <= [19] Multiply output 0
- `B` <= [17] Sqrt output 0

### [17] Sqrt

- `Input` <= [18] Abs output 0

### [18] Abs

- `Input` <= [2] 1-x output 0

### [19] Multiply

- `A` <= [8] Dot output 0
- `B` <= [5] Input RatioOfIndicesOfRefraction (Scalar) output 0

## Connection List

- [1] Multiply output 0 -> Output Result.A
- [13] Add output 0 -> Multiply.A
- [2] 1-x output 0 -> Multiply.B
- [3] Multiply output 0 -> 1-x.Input
- [6] 1-x output 0 -> Multiply.A
- [4] Multiply output 0 -> Multiply.B
- [5] Input RatioOfIndicesOfRefraction (Scalar) output 0 -> Multiply.A
- [5] Input RatioOfIndicesOfRefraction (Scalar) output 0 -> Multiply.B
- [7] Multiply output 0 -> 1-x.Input
- [8] Dot output 0 -> Multiply.A
- [8] Dot output 0 -> Multiply.B
- [12] Input Normal (Vector3) output 0 -> Dot.A
- [9] Multiply output 0 -> Dot.B
- [11] Input IncidenceVector (Vector3) output 0 -> Multiply.A
- [10] -1,-1,-1 output 0 -> Multiply.B
- [15] Multiply output 0 -> Add.A
- [14] Multiply output 0 -> Add.B
- [11] Input IncidenceVector (Vector3) output 0 -> Multiply.A
- [5] Input RatioOfIndicesOfRefraction (Scalar) output 0 -> Multiply.B
- [12] Input Normal (Vector3) output 0 -> Multiply.A
- [16] Subtract output 0 -> Multiply.B
- [19] Multiply output 0 -> Subtract.A
- [17] Sqrt output 0 -> Subtract.B
- [18] Abs output 0 -> Sqrt.Input
- [2] 1-x output 0 -> Abs.Input
- [8] Dot output 0 -> Multiply.A
- [5] Input RatioOfIndicesOfRefraction (Scalar) output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [1] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


