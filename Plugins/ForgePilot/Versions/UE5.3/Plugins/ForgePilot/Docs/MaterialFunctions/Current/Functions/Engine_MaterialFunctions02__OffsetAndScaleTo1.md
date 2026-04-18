# OffsetAndScaleTo1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/OffsetAndScaleTo1.OffsetAndScaleTo1`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The value entered in the original value input is offset by the offset value and the values are then scaled back into the 0-1 range. 

## Inputs

- `Offset Amount` [FunctionInput_Scalar]
- `Original Value` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Offset Amount (Scalar) | Offset Amount |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Original Value (Scalar) | Original Value |
| 6 | expression | `MaterialExpressionConstant` | 1 |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [7] Mask ( R ) output 0

### [2] Divide

- `A` <= [6] 1 output 0
- `B` <= [3] Add(,1) output 0

### [3] Add(,1)

- `A` <= [1] Input Offset Amount (Scalar) output 0

### [4] Multiply

- `A` <= [5] Input Original Value (Scalar) output 0
- `B` <= [2] Divide output 0

### [7] Mask ( R )

- `Input` <= [4] Multiply output 0

## Connection List

- [7] Mask ( R ) output 0 -> Output Result.A
- [6] 1 output 0 -> Divide.A
- [3] Add(,1) output 0 -> Divide.B
- [1] Input Offset Amount (Scalar) output 0 -> Add(,1).A
- [5] Input Original Value (Scalar) output 0 -> Multiply.A
- [2] Divide output 0 -> Multiply.B
- [4] Multiply output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Result` is driven by [7] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


