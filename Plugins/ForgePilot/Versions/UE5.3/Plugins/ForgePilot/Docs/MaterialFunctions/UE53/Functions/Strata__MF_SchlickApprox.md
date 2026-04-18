# MF_SchlickApprox

- Path: `/Engine/Functions/Strata/MF_SchlickApprox.MF_SchlickApprox`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `IOR` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input IOR (Scalar) | IOR |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | expression | `MaterialExpressionConstant` | 1 |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [6] Multiply output 0

### [2] 1-x

- `Input` <= [1] Input IOR (Scalar) output 0

### [4] Add

- `A` <= [1] Input IOR (Scalar) output 0
- `B` <= [3] 1 output 0

### [5] Divide

- `A` <= [2] 1-x output 0
- `B` <= [4] Add output 0

### [6] Multiply

- `A` <= [5] Divide output 0
- `B` <= [5] Divide output 0

## Connection List

- [6] Multiply output 0 -> Output Result.A
- [1] Input IOR (Scalar) output 0 -> 1-x.Input
- [1] Input IOR (Scalar) output 0 -> Add.A
- [3] 1 output 0 -> Add.B
- [2] 1-x output 0 -> Divide.A
- [4] Add output 0 -> Divide.B
- [5] Divide output 0 -> Multiply.A
- [5] Divide output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [6] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
