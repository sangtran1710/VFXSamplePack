# UnpackTwoNormalizedFloats

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/UnpackTwoNormalizedFloats.UnpackTwoNormalizedFloats`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Unpacks a float value encoded by the function PackTwoNormalizedFloats into two normalized floats (0-1) onto one single float. Precision is good to 3 decimal places, starts to lose precision at 4.

## Inputs

- `PackedFloat` [FunctionInput_Scalar]

## Outputs

- `NormalizedFloatA`
- `NormalizedFloatB`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output NormalizedFloatA | NormalizedFloatA |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input PackedFloat (Scalar) | PackedFloat |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output NormalizedFloatB | NormalizedFloatB |
| 5 | custom | `MaterialExpressionCustom` | Custom |  |

## Exact Input Wiring

### [0] Output NormalizedFloatA

- `A` <= [3] BreakOutFloat2Components output 0

### [4] Output NormalizedFloatB

- `A` <= [3] BreakOutFloat2Components output 1

## Connection List

- [3] BreakOutFloat2Components output 0 -> Output NormalizedFloatA.A
- [3] BreakOutFloat2Components output 1 -> Output NormalizedFloatB.A

## Output Trace Roots

- `NormalizedFloatA` is driven by [3] BreakOutFloat2Components
- `NormalizedFloatB` is driven by [3] BreakOutFloat2Components

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
