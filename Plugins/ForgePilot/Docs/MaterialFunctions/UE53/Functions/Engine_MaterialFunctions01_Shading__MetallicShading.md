# MetallicShading

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/MetallicShading.MetallicShading`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Apply to base color of metals for more interesting shading

## Inputs

- `BaseColor` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input BaseColor (Vector3) | BaseColor |
| 2 | expression | `MaterialExpressionPower` | Power(X, 4) |  |
| 3 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionPixelNormalWS` | PixelNormalWS |  |
| 6 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionPower` | Power(X, 2) |  |
| 10 | expression | `MaterialExpressionLinearInterpolate` | Lerp(0.3,0.9,) |  |
| 11 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Multiply output 0

### [2] Power(X, 4)

- `Base` <= [3] 1-x output 0

### [3] 1-x

- `Input` <= [8] Clamp output 0

### [4] Multiply

- `A` <= [1] Input BaseColor (Vector3) output 0
- `B` <= [7] Add output 0

### [6] Dot

- `A` <= [5] PixelNormalWS output 0
- `B` <= [11] Camera Vector output 0

### [7] Add

- `A` <= [2] Power(X, 4) output 0
- `B` <= [10] Lerp(0.3,0.9,) output 0

### [8] Clamp

- `Input` <= [6] Dot output 0

### [9] Power(X, 2)

- `Base` <= [8] Clamp output 0

### [10] Lerp(0.3,0.9,)

- `Alpha` <= [9] Power(X, 2) output 0

## Connection List

- [4] Multiply output 0 -> Output Result.A
- [3] 1-x output 0 -> Power(X, 4).Base
- [8] Clamp output 0 -> 1-x.Input
- [1] Input BaseColor (Vector3) output 0 -> Multiply.A
- [7] Add output 0 -> Multiply.B
- [5] PixelNormalWS output 0 -> Dot.A
- [11] Camera Vector output 0 -> Dot.B
- [2] Power(X, 4) output 0 -> Add.A
- [10] Lerp(0.3,0.9,) output 0 -> Add.B
- [6] Dot output 0 -> Clamp.Input
- [8] Clamp output 0 -> Power(X, 2).Base
- [9] Power(X, 2) output 0 -> Lerp(0.3,0.9,).Alpha

## Output Trace Roots

- `Result` is driven by [4] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
