# PackTwoNormalizedFloats

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/PackTwoNormalizedFloats.PackTwoNormalizedFloats`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Packs two normalized floats (0-1) onto one single float. Precision is good to 3 decimal places, starts to lose precision at 4.

## Inputs

- `NormalizedFloatA` [FunctionInput_Scalar] - A float in the range of 0-1. 
- `NormalizedFloatB` [FunctionInput_Scalar] - A float in the range of 0-1. 

## Outputs

- `PackedFloat`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output PackedFloat | PackedFloat |
| 1 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 2 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 3 | custom | `MaterialExpressionCustom` | Custom |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input NormalizedFloatA (Scalar) | NormalizedFloatA |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input NormalizedFloatB (Scalar) | NormalizedFloatB |
| 6 | custom | `MaterialExpressionCustom` | Custom |  |

## Exact Input Wiring

### [0] Output PackedFloat

- `A` <= [3] Custom output 0

### [1] Saturate

- `Input` <= [4] Input NormalizedFloatA (Scalar) output 0

### [2] Saturate

- `Input` <= [5] Input NormalizedFloatB (Scalar) output 0

## Connection List

- [3] Custom output 0 -> Output PackedFloat.A
- [4] Input NormalizedFloatA (Scalar) output 0 -> Saturate.Input
- [5] Input NormalizedFloatB (Scalar) output 0 -> Saturate.Input

## Output Trace Roots

- `PackedFloat` is driven by [3] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


