# MF_SchlickInternal1

- Path: `/Engine/Functions/Strata/MF_SchlickInternal1.MF_SchlickInternal1`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `In_IOR` [FunctionInput_Scalar]
- `Out_IOR` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In_IOR (Scalar) | In_IOR |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Out_IOR (Scalar) | Out_IOR |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Multiply output 0

### [2] Add

- `A` <= [6] Input Out_IOR (Scalar) output 0
- `B` <= [1] Input In_IOR (Scalar) output 0

### [3] Divide

- `A` <= [5] Subtract output 0
- `B` <= [2] Add output 0

### [4] Multiply

- `A` <= [3] Divide output 0
- `B` <= [3] Divide output 0

### [5] Subtract

- `A` <= [6] Input Out_IOR (Scalar) output 0
- `B` <= [1] Input In_IOR (Scalar) output 0

## Connection List

- [4] Multiply output 0 -> Output Result.A
- [6] Input Out_IOR (Scalar) output 0 -> Add.A
- [1] Input In_IOR (Scalar) output 0 -> Add.B
- [5] Subtract output 0 -> Divide.A
- [2] Add output 0 -> Divide.B
- [3] Divide output 0 -> Multiply.A
- [3] Divide output 0 -> Multiply.B
- [6] Input Out_IOR (Scalar) output 0 -> Subtract.A
- [1] Input In_IOR (Scalar) output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [4] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


