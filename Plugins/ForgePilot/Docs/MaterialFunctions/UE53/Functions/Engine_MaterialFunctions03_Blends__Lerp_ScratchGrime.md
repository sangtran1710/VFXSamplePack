# Lerp_ScratchGrime

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Lerp_ScratchGrime.Lerp_ScratchGrime`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Use to creat Material layers that have scratches and grime

## Inputs

- `Base` [FunctionInput_Vector3]
- `ScratchValue` [FunctionInput_Vector3]
- `GrimeValue` [FunctionInput_Vector3]
- `ScratchMask` [FunctionInput_Scalar]
- `GrimeMask` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input ScratchValue (Vector3) | ScratchValue |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input GrimeValue (Vector3) | GrimeValue |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input ScratchMask (Scalar) | ScratchMask |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input GrimeMask (Scalar) | GrimeMask |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Lerp output 0

### [6] Lerp

- `A` <= [1] Input Base (Vector3) output 0
- `B` <= [2] Input ScratchValue (Vector3) output 0
- `Alpha` <= [4] Input ScratchMask (Scalar) output 0

### [7] Lerp

- `A` <= [6] Lerp output 0
- `B` <= [3] Input GrimeValue (Vector3) output 0
- `Alpha` <= [5] Input GrimeMask (Scalar) output 0

## Connection List

- [7] Lerp output 0 -> Output Result.A
- [1] Input Base (Vector3) output 0 -> Lerp.A
- [2] Input ScratchValue (Vector3) output 0 -> Lerp.B
- [4] Input ScratchMask (Scalar) output 0 -> Lerp.Alpha
- [6] Lerp output 0 -> Lerp.A
- [3] Input GrimeValue (Vector3) output 0 -> Lerp.B
- [5] Input GrimeMask (Scalar) output 0 -> Lerp.Alpha

## Output Trace Roots

- `Result` is driven by [7] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
