# 3ColorBlend

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/3ColorBlend.3ColorBlend`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Blends between 3 colors based on a greyscale input

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `A` [FunctionInput_Vector3]
- `B` [FunctionInput_Vector3]
- `C` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionClamp` | Clamp |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionAdd` | Add(,-0.5) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input A (Vector3) | A |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input B (Vector3) | B |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input C (Vector3) | C |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [2] Lerp output 0
- `B` <= [5] Lerp output 0
- `Alpha` <= [10] Input Alpha (Scalar) output 0

### [2] Lerp

- `A` <= [11] Input A (Vector3) output 0
- `B` <= [12] Input B (Vector3) output 0
- `Alpha` <= [3] Clamp output 0

### [3] Clamp

- `Input` <= [4] Multiply(,2) output 0

### [4] Multiply(,2)

- `A` <= [10] Input Alpha (Scalar) output 0

### [5] Lerp

- `A` <= [12] Input B (Vector3) output 0
- `B` <= [13] Input C (Vector3) output 0
- `Alpha` <= [8] Clamp output 0

### [6] Add(,-0.5)

- `A` <= [10] Input Alpha (Scalar) output 0

### [7] Multiply(,2)

- `A` <= [6] Add(,-0.5) output 0

### [8] Clamp

- `Input` <= [7] Multiply(,2) output 0

### [10] Input Alpha (Scalar)

- `Preview` <= [9] LinearGradient output 0

## Connection List

- [1] Lerp output 0 -> Output Result.A
- [2] Lerp output 0 -> Lerp.A
- [5] Lerp output 0 -> Lerp.B
- [10] Input Alpha (Scalar) output 0 -> Lerp.Alpha
- [11] Input A (Vector3) output 0 -> Lerp.A
- [12] Input B (Vector3) output 0 -> Lerp.B
- [3] Clamp output 0 -> Lerp.Alpha
- [4] Multiply(,2) output 0 -> Clamp.Input
- [10] Input Alpha (Scalar) output 0 -> Multiply(,2).A
- [12] Input B (Vector3) output 0 -> Lerp.A
- [13] Input C (Vector3) output 0 -> Lerp.B
- [8] Clamp output 0 -> Lerp.Alpha
- [10] Input Alpha (Scalar) output 0 -> Add(,-0.5).A
- [6] Add(,-0.5) output 0 -> Multiply(,2).A
- [7] Multiply(,2) output 0 -> Clamp.Input
- [9] LinearGradient output 0 -> Input Alpha (Scalar).Preview

## Output Trace Roots

- `Result` is driven by [1] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
