# CheapContrast_RGB

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast_RGB.CheapContrast_RGB`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Cheaply adds contrast similar to pulling in edges in Photoshops Levels tool

## Inputs

- `In` [FunctionInput_Vector3]
- `Contrast` [FunctionInput_Scalar] - 0 is default contrast

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In (Vector3) | In |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 4 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract(0,) |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [8] Clamp output 0

### [1] Input In (Vector3)

- `Preview` <= [9] Texture Sample output 0

### [2] Lerp

- `A` <= [7] Append output 0
- `B` <= [4] Add(,1) output 0
- `Alpha` <= [1] Input In (Vector3) output 0

### [4] Add(,1)

- `A` <= [3] Input Contrast (Scalar) output 0

### [5] Subtract(0,)

- `B` <= [3] Input Contrast (Scalar) output 0

### [6] Append

- `A` <= [5] Subtract(0,) output 0
- `B` <= [5] Subtract(0,) output 0

### [7] Append

- `A` <= [6] Append output 0
- `B` <= [5] Subtract(0,) output 0

### [8] Clamp

- `Input` <= [2] Lerp output 0

## Connection List

- [8] Clamp output 0 -> Output Result.A
- [9] Texture Sample output 0 -> Input In (Vector3).Preview
- [7] Append output 0 -> Lerp.A
- [4] Add(,1) output 0 -> Lerp.B
- [1] Input In (Vector3) output 0 -> Lerp.Alpha
- [3] Input Contrast (Scalar) output 0 -> Add(,1).A
- [3] Input Contrast (Scalar) output 0 -> Subtract(0,).B
- [5] Subtract(0,) output 0 -> Append.A
- [5] Subtract(0,) output 0 -> Append.B
- [6] Append output 0 -> Append.A
- [5] Subtract(0,) output 0 -> Append.B
- [2] Lerp output 0 -> Clamp.Input

## Output Trace Roots

- `Result` is driven by [8] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
