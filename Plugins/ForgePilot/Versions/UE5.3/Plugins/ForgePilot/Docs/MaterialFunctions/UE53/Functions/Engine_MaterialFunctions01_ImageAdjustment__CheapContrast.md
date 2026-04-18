# CheapContrast

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast.CheapContrast`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Cheaply adds contrast similar to pulling in edges in Photoshops Levels tool

## Inputs

- `In` [FunctionInput_Scalar]
- `Contrast` [FunctionInput_Scalar] - 0 is default contrast

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 3 | expression | `MaterialExpressionClamp` | Clamp |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | LinearGradient |  |
| 6 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 7 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 8 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract(0,) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Mask ( R ) output 0

### [1] Input In (Scalar)

- `Preview` <= [6] 1-x output 0

### [3] Clamp

- `Input` <= [7] Lerp output 0

### [4] Mask ( R )

- `Input` <= [3] Clamp output 0

### [6] 1-x

- `Input` <= [5] LinearGradient output 1

### [7] Lerp

- `A` <= [9] Subtract(0,) output 0
- `B` <= [8] Add(,1) output 0
- `Alpha` <= [1] Input In (Scalar) output 0

### [8] Add(,1)

- `A` <= [2] Input Contrast (Scalar) output 0

### [9] Subtract(0,)

- `B` <= [2] Input Contrast (Scalar) output 0

## Connection List

- [4] Mask ( R ) output 0 -> Output Result.A
- [6] 1-x output 0 -> Input In (Scalar).Preview
- [7] Lerp output 0 -> Clamp.Input
- [3] Clamp output 0 -> Mask ( R ).Input
- [5] LinearGradient output 1 -> 1-x.Input
- [9] Subtract(0,) output 0 -> Lerp.A
- [8] Add(,1) output 0 -> Lerp.B
- [1] Input In (Scalar) output 0 -> Lerp.Alpha
- [2] Input Contrast (Scalar) output 0 -> Add(,1).A
- [2] Input Contrast (Scalar) output 0 -> Subtract(0,).B

## Output Trace Roots

- `Result` is driven by [4] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
