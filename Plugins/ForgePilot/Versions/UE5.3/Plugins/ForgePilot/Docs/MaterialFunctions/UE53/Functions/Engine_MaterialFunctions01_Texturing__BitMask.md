# BitMask

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/BitMask.BitMask`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Provides a 1bit mask from a supplied Grayscale image and a value between 0-255

## Inputs

- `BitMask` [FunctionInput_Scalar] - Should be texture using TC_Grayscale
- `Bit` [FunctionInput_Scalar] - Value between 0-255

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionIf` | If |  |
| 2 | expression | `MaterialExpressionConstant` | 1 |  |
| 3 | expression | `MaterialExpressionConstant` | 0 |  |
| 4 | expression | `MaterialExpressionDivide` | Divide |  |
| 5 | expression | `MaterialExpressionConstant` | 255 |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input BitMask (Scalar) | BitMask |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Bit (Scalar) | Bit |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] If output 0

### [1] If

- `A` <= [6] Input BitMask (Scalar) output 0
- `B` <= [4] Divide output 0
- `AGreaterThanB` <= [3] 0 output 0
- `AEqualsB` <= [2] 1 output 0
- `ALessThanB` <= [3] 0 output 0

### [4] Divide

- `A` <= [7] Input Bit (Scalar) output 0
- `B` <= [5] 255 output 0

## Connection List

- [1] If output 0 -> Output Result.A
- [6] Input BitMask (Scalar) output 0 -> If.A
- [4] Divide output 0 -> If.B
- [3] 0 output 0 -> If.AGreaterThanB
- [2] 1 output 0 -> If.AEqualsB
- [3] 0 output 0 -> If.ALessThanB
- [7] Input Bit (Scalar) output 0 -> Divide.A
- [5] 255 output 0 -> Divide.B

## Output Trace Roots

- `Result` is driven by [1] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
