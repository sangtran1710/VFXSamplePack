# 1Dto2DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/1Dto2DIndex.1Dto2DIndex`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Converts a 2D index into a 1D index based on a given total X,Y size. For example an 8x8 grid would return cells 0-63.

## Inputs

- `1D Index` [FunctionInput_Scalar]
- `Cells XY` [FunctionInput_Vector2]

## Outputs

- `2D Index`
- `Centered UV Position`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output 2D Index | 2D Index |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input 1D Index (Scalar) | 1D Index |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Cells XY (Vector2) | Cells XY |
| 3 | expression | `MaterialExpressionCeil` | Ceil |  |
| 4 | expression | `MaterialExpressionFmod` | Fmod |  |
| 5 | expression | `MaterialExpressionCeil` | Ceil |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionFloor` | Floor |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output Centered UV Position | Centered UV Position |
| 10 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output 2D Index

- `A` <= [6] MakeFloat2 output 0

### [3] Ceil

- `Input` <= [11] Mask ( R ) output 0

### [4] Fmod

- `A` <= [5] Ceil output 0
- `B` <= [3] Ceil output 0

### [5] Ceil

- `Input` <= [1] Input 1D Index (Scalar) output 0

### [7] Divide

- `A` <= [5] Ceil output 0
- `B` <= [3] Ceil output 0

### [8] Floor

- `Input` <= [7] Divide output 0

### [9] Output Centered UV Position

- `A` <= [13] Divide output 0

### [10] Add(,0.5)

- `A` <= [6] MakeFloat2 output 0

### [11] Mask ( R )

- `Input` <= [2] Input Cells XY (Vector2) output 0

### [12] Multiply(,1)

- `A` <= [2] Input Cells XY (Vector2) output 0

### [13] Divide

- `A` <= [10] Add(,0.5) output 0
- `B` <= [12] Multiply(,1) output 0

## Connection List

- [6] MakeFloat2 output 0 -> Output 2D Index.A
- [11] Mask ( R ) output 0 -> Ceil.Input
- [5] Ceil output 0 -> Fmod.A
- [3] Ceil output 0 -> Fmod.B
- [1] Input 1D Index (Scalar) output 0 -> Ceil.Input
- [5] Ceil output 0 -> Divide.A
- [3] Ceil output 0 -> Divide.B
- [7] Divide output 0 -> Floor.Input
- [13] Divide output 0 -> Output Centered UV Position.A
- [6] MakeFloat2 output 0 -> Add(,0.5).A
- [2] Input Cells XY (Vector2) output 0 -> Mask ( R ).Input
- [2] Input Cells XY (Vector2) output 0 -> Multiply(,1).A
- [10] Add(,0.5) output 0 -> Divide.A
- [12] Multiply(,1) output 0 -> Divide.B

## Output Trace Roots

- `2D Index` is driven by [6] MakeFloat2
- `Centered UV Position` is driven by [13] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


