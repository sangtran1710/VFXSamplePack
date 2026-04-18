# UVLayoutToWorldSpacePosition

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/UVLayoutToWorldSpacePosition.UVLayoutToWorldSpacePosition`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Use in the world position offset input to visual uv coordinates in 3D. 

## Inputs

- `Texture Coordinate` [FunctionInput_Vector2] - Insert a uv coordinate to view in 3d space.

## Outputs

- `World Positon`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Positon | World Positon |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionObjectRadius` | Object Radius |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionAppendVector` | Append |  |
| 5 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 13 | expression | `MaterialExpressionConstant` | 0 |  |
| 14 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 15 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 16 | expression | `MaterialExpressionConstant2Vector` | 0.5,0 |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Texture Coordinate (Vector2) | Texture Coordinate |

## Exact Input Wiring

### [0] Output World Positon

- `A` <= [12] Subtract output 0

### [1] Add

- `A` <= [2] Object Radius output 0
- `B` <= [2] Object Radius output 0

### [3] Multiply

- `A` <= [1] Add output 0
- `B` <= [9] Subtract output 0

### [4] Append

- `A` <= [3] Multiply output 0
- `B` <= [13] 0 output 0

### [5] View Space to World Space TransformVector

- `Input` <= [4] Append output 0

### [6] Append

- `A` <= [7] BreakOutFloat2Components output 0
- `B` <= [11] 1-x output 0

### [8] Add

- `A` <= [5] View Space to World Space TransformVector output 0
- `B` <= [14] Actor Position (Absolute) output 0

### [9] Subtract

- `A` <= [6] Append output 0
- `B` <= [16] 0.5,0 output 0

### [11] 1-x

- `Input` <= [7] BreakOutFloat2Components output 1

### [12] Subtract

- `A` <= [8] Add output 0
- `B` <= [15] Absolute World Position output 0

### [17] Input Texture Coordinate (Vector2)

- `Preview` <= [10] TexCoord[0] output 0

## Connection List

- [12] Subtract output 0 -> Output World Positon.A
- [2] Object Radius output 0 -> Add.A
- [2] Object Radius output 0 -> Add.B
- [1] Add output 0 -> Multiply.A
- [9] Subtract output 0 -> Multiply.B
- [3] Multiply output 0 -> Append.A
- [13] 0 output 0 -> Append.B
- [4] Append output 0 -> View Space to World Space TransformVector.Input
- [7] BreakOutFloat2Components output 0 -> Append.A
- [11] 1-x output 0 -> Append.B
- [5] View Space to World Space TransformVector output 0 -> Add.A
- [14] Actor Position (Absolute) output 0 -> Add.B
- [6] Append output 0 -> Subtract.A
- [16] 0.5,0 output 0 -> Subtract.B
- [7] BreakOutFloat2Components output 1 -> 1-x.Input
- [8] Add output 0 -> Subtract.A
- [15] Absolute World Position output 0 -> Subtract.B
- [10] TexCoord[0] output 0 -> Input Texture Coordinate (Vector2).Preview

## Output Trace Roots

- `World Positon` is driven by [12] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
