# 2Dto1DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/2Dto1DIndex.2Dto1DIndex`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts a 2D index into a 1D index based on a given total X,Y size. For example an 8x8 grid would return cells 0-63.

## Inputs

- `2D Index` [FunctionInput_Vector2]
- `Columns X` [FunctionInput_Vector2]

## Outputs

- `1D Index` - The 1D index result
- `Centered UV Position` - This is the centered V2 position of the original input, defined as floor(UV) + 0.5.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 1D Index | 1D Index |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input 2D Index (Vector2) | 2D Index |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Columns X (Vector2) | Columns X |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionFloor` | Floor |  |
| 7 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Centered UV Position | Centered UV Position |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |

## Exact Input Wiring

### [0] Output 1D Index

- `A` <= [4] Add output 0

### [4] Add

- `A` <= [2] BreakOutFloat2Components output 0
- `B` <= [5] Multiply output 0

### [5] Multiply

- `A` <= [2] BreakOutFloat2Components output 1
- `B` <= [10] Mask ( R ) output 0

### [6] Floor

- `Input` <= [1] Input 2D Index (Vector2) output 0

### [7] Add(,0.5)

- `A` <= [6] Floor output 0

### [8] Output Centered UV Position

- `A` <= [9] Divide output 0

### [9] Divide

- `A` <= [7] Add(,0.5) output 0
- `B` <= [11] Multiply(,1) output 0

### [10] Mask ( R )

- `Input` <= [3] Input Columns X (Vector2) output 0

### [11] Multiply(,1)

- `A` <= [3] Input Columns X (Vector2) output 0

## Connection List

- [4] Add output 0 -> Output 1D Index.A
- [2] BreakOutFloat2Components output 0 -> Add.A
- [5] Multiply output 0 -> Add.B
- [2] BreakOutFloat2Components output 1 -> Multiply.A
- [10] Mask ( R ) output 0 -> Multiply.B
- [1] Input 2D Index (Vector2) output 0 -> Floor.Input
- [6] Floor output 0 -> Add(,0.5).A
- [9] Divide output 0 -> Output Centered UV Position.A
- [7] Add(,0.5) output 0 -> Divide.A
- [11] Multiply(,1) output 0 -> Divide.B
- [3] Input Columns X (Vector2) output 0 -> Mask ( R ).Input
- [3] Input Columns X (Vector2) output 0 -> Multiply(,1).A

## Output Trace Roots

- `1D Index` is driven by [4] Add
- `Centered UV Position` is driven by [9] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
