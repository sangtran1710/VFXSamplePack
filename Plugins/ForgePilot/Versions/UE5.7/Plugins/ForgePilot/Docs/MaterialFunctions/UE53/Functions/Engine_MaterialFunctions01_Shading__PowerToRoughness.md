# PowerToRoughness

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/PowerToRoughness.PowerToRoughness`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Power` [FunctionInput_Scalar]

## Outputs

- `Roughness`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Roughness | Roughness |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Power (Scalar) | Power |
| 2 | expression | `MaterialExpressionClamp` | Clamp |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply(,4) |  |
| 4 | expression | `MaterialExpressionAdd` | Add(,2) |  |
| 5 | expression | `MaterialExpressionDivide` | Divide(2,) |  |
| 6 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 7 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |

## Exact Input Wiring

### [0] Output Roughness

- `A` <= [7] Sqrt output 0

### [2] Clamp

- `Input` <= [1] Input Power (Scalar) output 0

### [3] Multiply(,4)

- `A` <= [2] Clamp output 0

### [4] Add(,2)

- `A` <= [3] Multiply(,4) output 0

### [5] Divide(2,)

- `B` <= [4] Add(,2) output 0

### [6] Sqrt

- `Input` <= [5] Divide(2,) output 0

### [7] Sqrt

- `Input` <= [6] Sqrt output 0

## Connection List

- [7] Sqrt output 0 -> Output Roughness.A
- [1] Input Power (Scalar) output 0 -> Clamp.Input
- [2] Clamp output 0 -> Multiply(,4).A
- [3] Multiply(,4) output 0 -> Add(,2).A
- [4] Add(,2) output 0 -> Divide(2,).B
- [5] Divide(2,) output 0 -> Sqrt.Input
- [6] Sqrt output 0 -> Sqrt.Input

## Output Trace Roots

- `Roughness` is driven by [7] Sqrt

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
