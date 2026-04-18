# 3Dto1DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/3Dto1DIndex.3Dto1DIndex`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts a 3D index into a 1D index based on a given total X,Y,Z size. For example an 3x3x3 grid would return cells 0-26.

## Inputs

- `3D Index` [FunctionInput_Vector3]
- `Cells XYZ` [FunctionInput_Vector3]

## Outputs

- `1D Index`
- `Centered UVW Position`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 1D Index | 1D Index |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input 3D Index (Vector3) | 3D Index |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Cells XYZ (Vector3) | Cells XYZ |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Centered UVW Position | Centered UVW Position |
| 6 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 7 | expression | `MaterialExpressionFloor` | Floor |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |

## Exact Input Wiring

### [0] Output 1D Index

- `A` <= [11] Add output 0

### [3] Add

- `A` <= [8] BreakOutFloat3Components output 0
- `B` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [8] BreakOutFloat3Components output 1
- `B` <= [12] BreakOutFloat3Components output 0

### [5] Output Centered UVW Position

- `A` <= [13] Divide output 0

### [6] Add(,0.5)

- `A` <= [7] Floor output 0

### [7] Floor

- `Input` <= [1] Input 3D Index (Vector3) output 0

### [9] Multiply

- `A` <= [12] BreakOutFloat3Components output 0
- `B` <= [12] BreakOutFloat3Components output 1

### [10] Multiply

- `A` <= [8] BreakOutFloat3Components output 2
- `B` <= [9] Multiply output 0

### [11] Add

- `A` <= [3] Add output 0
- `B` <= [10] Multiply output 0

### [13] Divide

- `A` <= [6] Add(,0.5) output 0
- `B` <= [14] Multiply(,1) output 0

### [14] Multiply(,1)

- `A` <= [2] Input Cells XYZ (Vector3) output 0

## Connection List

- [11] Add output 0 -> Output 1D Index.A
- [8] BreakOutFloat3Components output 0 -> Add.A
- [4] Multiply output 0 -> Add.B
- [8] BreakOutFloat3Components output 1 -> Multiply.A
- [12] BreakOutFloat3Components output 0 -> Multiply.B
- [13] Divide output 0 -> Output Centered UVW Position.A
- [7] Floor output 0 -> Add(,0.5).A
- [1] Input 3D Index (Vector3) output 0 -> Floor.Input
- [12] BreakOutFloat3Components output 0 -> Multiply.A
- [12] BreakOutFloat3Components output 1 -> Multiply.B
- [8] BreakOutFloat3Components output 2 -> Multiply.A
- [9] Multiply output 0 -> Multiply.B
- [3] Add output 0 -> Add.A
- [10] Multiply output 0 -> Add.B
- [6] Add(,0.5) output 0 -> Divide.A
- [14] Multiply(,1) output 0 -> Divide.B
- [2] Input Cells XYZ (Vector3) output 0 -> Multiply(,1).A

## Output Trace Roots

- `1D Index` is driven by [11] Add
- `Centered UVW Position` is driven by [13] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
