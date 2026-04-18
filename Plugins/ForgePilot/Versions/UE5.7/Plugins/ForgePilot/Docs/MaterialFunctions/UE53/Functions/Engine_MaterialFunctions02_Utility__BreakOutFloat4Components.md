# BreakOutFloat4Components

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat4Components.BreakOutFloat4Components`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Enter a float 4 vector into the function and retrieve individual channels back through the scalar outputs.

## Inputs

- `Float4` [FunctionInput_Vector4]

## Outputs

- `R`
- `G`
- `B`
- `A`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output R | R |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Float4 (Vector4) | Float4 |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output G | G |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output B | B |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output A | A |

## Exact Input Wiring

### [0] Output R

- `A` <= [2] Mask ( R ) output 0

### [2] Mask ( R )

- `Input` <= [1] Input Float4 (Vector4) output 0

### [3] Mask ( G )

- `Input` <= [1] Input Float4 (Vector4) output 0

### [4] Mask ( B )

- `Input` <= [1] Input Float4 (Vector4) output 0

### [5] Mask ( A )

- `Input` <= [1] Input Float4 (Vector4) output 0

### [6] Output G

- `A` <= [3] Mask ( G ) output 0

### [7] Output B

- `A` <= [4] Mask ( B ) output 0

### [8] Output A

- `A` <= [5] Mask ( A ) output 0

## Connection List

- [2] Mask ( R ) output 0 -> Output R.A
- [1] Input Float4 (Vector4) output 0 -> Mask ( R ).Input
- [1] Input Float4 (Vector4) output 0 -> Mask ( G ).Input
- [1] Input Float4 (Vector4) output 0 -> Mask ( B ).Input
- [1] Input Float4 (Vector4) output 0 -> Mask ( A ).Input
- [3] Mask ( G ) output 0 -> Output G.A
- [4] Mask ( B ) output 0 -> Output B.A
- [5] Mask ( A ) output 0 -> Output A.A

## Output Trace Roots

- `R` is driven by [2] Mask ( R )
- `G` is driven by [3] Mask ( G )
- `B` is driven by [4] Mask ( B )
- `A` is driven by [5] Mask ( A )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
