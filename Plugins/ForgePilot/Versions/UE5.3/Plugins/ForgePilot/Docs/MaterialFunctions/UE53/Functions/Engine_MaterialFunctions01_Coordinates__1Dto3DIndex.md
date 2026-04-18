# 1Dto3DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/1Dto3DIndex.1Dto3DIndex`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts a 2D index into a 1D index based on a given total X,Y size. For example an 8x8 grid would return cells 0-63.

## Inputs

- `1D Index` [FunctionInput_Scalar]
- `Cells XYZ` [FunctionInput_Vector3]

## Outputs

- `3D Index`
- `Centered UV Position`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 3D Index | 3D Index |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input 1D Index (Scalar) | 1D Index |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Cells XYZ (Vector3) | Cells XYZ |
| 3 | expression | `MaterialExpressionCeil` | Ceil |  |
| 4 | expression | `MaterialExpressionFmod` | Fmod |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 6 | expression | `MaterialExpressionCeil` | Ceil |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 8 | expression | `MaterialExpressionCeil` | Ceil |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionFmod` | Fmod |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | expression | `MaterialExpressionDivide` | Divide |  |
| 13 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Centered UV Position | Centered UV Position |
| 15 | expression | `MaterialExpressionFloor` | Floor |  |
| 16 | expression | `MaterialExpressionFloor` | Floor |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 18 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output 3D Index

- `A` <= [7] MakeFloat3 output 0

### [3] Ceil

- `Input` <= [1] Input 1D Index (Scalar) output 0

### [4] Fmod

- `A` <= [3] Ceil output 0
- `B` <= [6] Ceil output 0

### [6] Ceil

- `Input` <= [5] BreakOutFloat3Components output 0

### [8] Ceil

- `Input` <= [5] BreakOutFloat3Components output 1

### [9] Multiply

- `A` <= [6] Ceil output 0
- `B` <= [8] Ceil output 0

### [10] Fmod

- `A` <= [3] Ceil output 0
- `B` <= [9] Multiply output 0

### [11] Divide

- `A` <= [10] Fmod output 0
- `B` <= [6] Ceil output 0

### [12] Divide

- `A` <= [3] Ceil output 0
- `B` <= [9] Multiply output 0

### [13] Add(,0.5)

- `A` <= [7] MakeFloat3 output 0

### [14] Output Centered UV Position

- `A` <= [18] Divide output 0

### [15] Floor

- `Input` <= [11] Divide output 0

### [16] Floor

- `Input` <= [12] Divide output 0

### [17] Multiply(,1)

- `A` <= [2] Input Cells XYZ (Vector3) output 0

### [18] Divide

- `A` <= [13] Add(,0.5) output 0
- `B` <= [17] Multiply(,1) output 0

## Connection List

- [7] MakeFloat3 output 0 -> Output 3D Index.A
- [1] Input 1D Index (Scalar) output 0 -> Ceil.Input
- [3] Ceil output 0 -> Fmod.A
- [6] Ceil output 0 -> Fmod.B
- [5] BreakOutFloat3Components output 0 -> Ceil.Input
- [5] BreakOutFloat3Components output 1 -> Ceil.Input
- [6] Ceil output 0 -> Multiply.A
- [8] Ceil output 0 -> Multiply.B
- [3] Ceil output 0 -> Fmod.A
- [9] Multiply output 0 -> Fmod.B
- [10] Fmod output 0 -> Divide.A
- [6] Ceil output 0 -> Divide.B
- [3] Ceil output 0 -> Divide.A
- [9] Multiply output 0 -> Divide.B
- [7] MakeFloat3 output 0 -> Add(,0.5).A
- [18] Divide output 0 -> Output Centered UV Position.A
- [11] Divide output 0 -> Floor.Input
- [12] Divide output 0 -> Floor.Input
- [2] Input Cells XYZ (Vector3) output 0 -> Multiply(,1).A
- [13] Add(,0.5) output 0 -> Divide.A
- [17] Multiply(,1) output 0 -> Divide.B

## Output Trace Roots

- `3D Index` is driven by [7] MakeFloat3
- `Centered UV Position` is driven by [18] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
