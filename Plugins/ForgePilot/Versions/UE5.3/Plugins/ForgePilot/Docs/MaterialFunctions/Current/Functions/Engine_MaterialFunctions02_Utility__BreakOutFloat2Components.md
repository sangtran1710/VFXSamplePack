# BreakOutFloat2Components

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat2Components.BreakOutFloat2Components`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Enter a float 2 vector into the function and retrieve individual channels back through the scalar outputs.

## Inputs

- `Float2` [FunctionInput_Vector2]

## Outputs

- `R`
- `G`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output R | R |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Float2 (Vector2) | Float2 |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output G | G |

## Exact Input Wiring

### [0] Output R

- `A` <= [2] Mask ( R ) output 0

### [2] Mask ( R )

- `Input` <= [1] Input Float2 (Vector2) output 0

### [3] Mask ( G )

- `Input` <= [1] Input Float2 (Vector2) output 0

### [4] Output G

- `A` <= [3] Mask ( G ) output 0

## Connection List

- [2] Mask ( R ) output 0 -> Output R.A
- [1] Input Float2 (Vector2) output 0 -> Mask ( R ).Input
- [1] Input Float2 (Vector2) output 0 -> Mask ( G ).Input
- [3] Mask ( G ) output 0 -> Output G.A

## Output Trace Roots

- `R` is driven by [2] Mask ( R )
- `G` is driven by [3] Mask ( G )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


