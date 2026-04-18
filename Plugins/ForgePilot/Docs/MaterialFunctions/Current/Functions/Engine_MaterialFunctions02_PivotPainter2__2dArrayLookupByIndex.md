# 2dArrayLookupByIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/2dArrayLookupByIndex.2dArrayLookupByIndex`
- Category: Engine_MaterialFunctions02/PivotPainter2
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Finds an index's location in a 2d Array.

## Inputs

- `Array Dimensions` [FunctionInput_Vector2]
- `Index` [FunctionInput_Scalar]

## Outputs

- `UVs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 1 | expression | `MaterialExpressionFmod` | Fmod |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | expression | `MaterialExpressionDivide` | Divide |  |
| 5 | expression | `MaterialExpressionFloor` | Floor |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat2 |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Array Dimensions (Vector2) | Array Dimensions |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Index (Scalar) | Index |

## Exact Input Wiring

### [0] Output UVs

- `A` <= [9] MakeFloat2 output 0

### [1] Fmod

- `A` <= [11] Input Index (Scalar) output 0
- `B` <= [3] BreakOutFloat2Components output 0

### [2] Append

- `A` <= [1] Fmod output 0
- `B` <= [5] Floor output 0

### [4] Divide

- `A` <= [11] Input Index (Scalar) output 0
- `B` <= [3] BreakOutFloat2Components output 0

### [5] Floor

- `Input` <= [4] Divide output 0

### [6] Divide

- `A` <= [7] Add(,0.5) output 0
- `B` <= [10] Input Array Dimensions (Vector2) output 0

### [7] Add(,0.5)

- `A` <= [2] Append output 0

## Connection List

- [9] MakeFloat2 output 0 -> Output UVs.A
- [11] Input Index (Scalar) output 0 -> Fmod.A
- [3] BreakOutFloat2Components output 0 -> Fmod.B
- [1] Fmod output 0 -> Append.A
- [5] Floor output 0 -> Append.B
- [11] Input Index (Scalar) output 0 -> Divide.A
- [3] BreakOutFloat2Components output 0 -> Divide.B
- [4] Divide output 0 -> Floor.Input
- [7] Add(,0.5) output 0 -> Divide.A
- [10] Input Array Dimensions (Vector2) output 0 -> Divide.B
- [2] Append output 0 -> Add(,0.5).A

## Output Trace Roots

- `UVs` is driven by [9] MakeFloat2

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


