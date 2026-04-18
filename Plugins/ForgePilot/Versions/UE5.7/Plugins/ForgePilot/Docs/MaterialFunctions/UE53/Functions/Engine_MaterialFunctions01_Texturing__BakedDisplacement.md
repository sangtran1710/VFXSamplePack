# BakedDisplacement

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/BakedDisplacement.BakedDisplacement`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Set up a baked scalar displacement map to be used with WorldDispalcement

## Inputs

- `In` [FunctionInput_Scalar]
- `Baked_Min` [FunctionInput_Scalar]
- `Baked_Max` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Baked_Min (Scalar) | Baked_Min |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Baked_Max (Scalar) | Baked_Max |
| 4 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [5] Multiply output 0

### [4] Lerp

- `A` <= [2] Input Baked_Min (Scalar) output 0
- `B` <= [3] Input Baked_Max (Scalar) output 0
- `Alpha` <= [1] Input In (Scalar) output 0

### [5] Multiply

- `A` <= [4] Lerp output 0
- `B` <= [6] VertexNormalWS output 0

## Connection List

- [5] Multiply output 0 -> Output Result.A
- [2] Input Baked_Min (Scalar) output 0 -> Lerp.A
- [3] Input Baked_Max (Scalar) output 0 -> Lerp.B
- [1] Input In (Scalar) output 0 -> Lerp.Alpha
- [4] Lerp output 0 -> Multiply.A
- [6] VertexNormalWS output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [5] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
