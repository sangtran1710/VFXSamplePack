# AddComponents

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/AddComponents.AddComponents`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Input a float 2, 3 or 4 and retrieve the sum of the component values.

## Inputs

- `f2` [FunctionInput_Vector2]
- `f3` [FunctionInput_Vector3]
- `f4` [FunctionInput_Vector4]

## Outputs

- `f2`
- `f3`
- `f4`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output f2 | f2 |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input f2 (Vector2) | f2 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input f3 (Vector3) | f3 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input f4 (Vector4) | f4 |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output f3 | f3 |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output f4 | f4 |

## Exact Input Wiring

### [0] Output f2

- `A` <= [6] Add output 0

### [4] Mask ( R )

- `Input` <= [1] Input f2 (Vector2) output 0

### [5] Mask ( G )

- `Input` <= [1] Input f2 (Vector2) output 0

### [6] Add

- `A` <= [4] Mask ( R ) output 0
- `B` <= [5] Mask ( G ) output 0

### [7] Mask ( R )

- `Input` <= [2] Input f3 (Vector3) output 0

### [8] Add

- `A` <= [7] Mask ( R ) output 0
- `B` <= [9] Mask ( G ) output 0

### [9] Mask ( G )

- `Input` <= [2] Input f3 (Vector3) output 0

### [10] Mask ( B )

- `Input` <= [2] Input f3 (Vector3) output 0

### [11] Add

- `A` <= [8] Add output 0
- `B` <= [10] Mask ( B ) output 0

### [12] Add

- `A` <= [13] Mask ( R ) output 0
- `B` <= [14] Mask ( G ) output 0

### [13] Mask ( R )

- `Input` <= [3] Input f4 (Vector4) output 0

### [14] Mask ( G )

- `Input` <= [3] Input f4 (Vector4) output 0

### [15] Add

- `A` <= [12] Add output 0
- `B` <= [16] Mask ( B ) output 0

### [16] Mask ( B )

- `Input` <= [3] Input f4 (Vector4) output 0

### [17] Mask ( A )

- `Input` <= [3] Input f4 (Vector4) output 0

### [18] Add

- `A` <= [15] Add output 0
- `B` <= [17] Mask ( A ) output 0

### [19] Output f3

- `A` <= [11] Add output 0

### [20] Output f4

- `A` <= [18] Add output 0

## Connection List

- [6] Add output 0 -> Output f2.A
- [1] Input f2 (Vector2) output 0 -> Mask ( R ).Input
- [1] Input f2 (Vector2) output 0 -> Mask ( G ).Input
- [4] Mask ( R ) output 0 -> Add.A
- [5] Mask ( G ) output 0 -> Add.B
- [2] Input f3 (Vector3) output 0 -> Mask ( R ).Input
- [7] Mask ( R ) output 0 -> Add.A
- [9] Mask ( G ) output 0 -> Add.B
- [2] Input f3 (Vector3) output 0 -> Mask ( G ).Input
- [2] Input f3 (Vector3) output 0 -> Mask ( B ).Input
- [8] Add output 0 -> Add.A
- [10] Mask ( B ) output 0 -> Add.B
- [13] Mask ( R ) output 0 -> Add.A
- [14] Mask ( G ) output 0 -> Add.B
- [3] Input f4 (Vector4) output 0 -> Mask ( R ).Input
- [3] Input f4 (Vector4) output 0 -> Mask ( G ).Input
- [12] Add output 0 -> Add.A
- [16] Mask ( B ) output 0 -> Add.B
- [3] Input f4 (Vector4) output 0 -> Mask ( B ).Input
- [3] Input f4 (Vector4) output 0 -> Mask ( A ).Input
- [15] Add output 0 -> Add.A
- [17] Mask ( A ) output 0 -> Add.B
- [11] Add output 0 -> Output f3.A
- [18] Add output 0 -> Output f4.A

## Output Trace Roots

- `f2` is driven by [6] Add
- `f3` is driven by [11] Add
- `f4` is driven by [18] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


