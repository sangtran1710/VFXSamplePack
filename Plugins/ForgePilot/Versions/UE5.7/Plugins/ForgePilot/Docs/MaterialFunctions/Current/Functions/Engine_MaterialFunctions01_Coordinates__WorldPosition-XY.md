# WorldPosition-XY

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/WorldPosition-XY.WorldPosition-XY`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

WorldPosition XY Coordinates with Scale input.

## Inputs

- `Scale` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Scale (Scalar) | Scale |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] Mask ( R G ) output 0

### [2] Divide

- `A` <= [1] Absolute World Position output 0
- `B` <= [3] Input Scale (Scalar) output 0

### [4] Mask ( R G )

- `Input` <= [2] Divide output 0

## Connection List

- [4] Mask ( R G ) output 0 -> Output Result.A
- [1] Absolute World Position output 0 -> Divide.A
- [3] Input Scale (Scalar) output 0 -> Divide.B
- [2] Divide output 0 -> Mask ( R G ).Input

## Output Trace Roots

- `Result` is driven by [4] Mask ( R G )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


