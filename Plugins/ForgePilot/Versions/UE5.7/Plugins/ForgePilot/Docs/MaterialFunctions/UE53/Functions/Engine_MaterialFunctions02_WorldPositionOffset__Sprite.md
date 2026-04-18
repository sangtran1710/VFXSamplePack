# Sprite

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite.Sprite`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Use this function within the world position offset input to align plane meshes with the screen at any location.

## Inputs

- `XY scale` [FunctionInput_Vector2]
- `Center Location` [FunctionInput_Vector3] - The world position of the center of the sprite
- `0-1 UVs` [FunctionInput_Vector2] - The quad should supply uvs that are mapped from the 0 to 1 uv range.
- `Normals (Optional)` [FunctionInput_Vector3] - This is an optional input. The function will correctly transform your normals.
- `OverridePivot` [FunctionInput_StaticBool]

## Outputs

- `World position offset` - Plug this into the world position offset input. Please note that world position is already subtracted from the output.
- `Normals` - This will output transformed tangent space normals.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World position offset | World position offset |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input XY scale (Vector2) | XY scale |
| 10 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Center Location (Vector3) | Center Location |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input 0-1 UVs (Vector2) | 0-1 UVs |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Normals (Optional) (Vector3) | Normals (Optional) |
| 17 | expression | `MaterialExpressionTransform` | View Space to Tangent Space TransformVector |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionConstant3Vector` | 1,-1,-1 |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Normals | Normals |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | ComponentPivotLocation |  |
| 22 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 23 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 24 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 25 | expression | `MaterialExpressionConstant3Vector` | 0,-1,0 |  |
| 26 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input OverridePivot (StaticBool) | OverridePivot |
| 28 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 29 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 30 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output World position offset

- `A` <= [26] Switch output 0

### [2] Subtract(,0.5)

- `A` <= [15] Input 0-1 UVs (Vector2) output 0

### [4] Multiply

- `A` <= [3] BreakOutFloat2Components output 0
- `B` <= [23] Camera Space to World Space TransformVector output 0

### [5] Multiply

- `A` <= [3] BreakOutFloat2Components output 1
- `B` <= [22] Camera Space to World Space TransformVector output 0

### [6] Multiply

- `A` <= [4] Multiply output 0
- `B` <= [10] BreakOutFloat2Components output 0

### [7] Multiply

- `A` <= [5] Multiply output 0
- `B` <= [10] BreakOutFloat2Components output 1

### [8] Add

- `A` <= [6] Multiply output 0
- `B` <= [7] Multiply output 0

### [11] Subtract

- `A` <= [30] Reroute Node (reroutes wires) output 0
- `B` <= [12] Absolute World Position output 0

### [13] Add

- `A` <= [11] Subtract output 0
- `B` <= [14] Input Center Location (Vector3) output 0

### [14] Input Center Location (Vector3)

- `Preview` <= [21] ComponentPivotLocation output 0

### [15] Input 0-1 UVs (Vector2)

- `Preview` <= [1] TexCoord[0] output 0

### [17] View Space to Tangent Space TransformVector

- `Input` <= [18] Multiply output 0

### [18] Multiply

- `A` <= [16] Input Normals (Optional) (Vector3) output 0
- `B` <= [19] 1,-1,-1 output 0

### [20] Output Normals

- `A` <= [17] View Space to Tangent Space TransformVector output 0

### [22] Camera Space to World Space TransformVector

- `Input` <= [25] 0,-1,0 output 0

### [23] Camera Space to World Space TransformVector

- `Input` <= [24] 1,0,0 output 0

### [26] Switch

- `A` <= [13] Add output 0
- `B` <= [29] Reroute Node (reroutes wires) output 0
- `Value` <= [27] Input OverridePivot (StaticBool) output 0

### [27] Input OverridePivot (StaticBool)

- `Preview` <= [28] Static Bool (True) output 0

### [29] Reroute Node (reroutes wires)

- `Input` <= [8] Add output 0

### [30] Reroute Node (reroutes wires)

- `Input` <= [8] Add output 0

## Connection List

- [26] Switch output 0 -> Output World position offset.A
- [15] Input 0-1 UVs (Vector2) output 0 -> Subtract(,0.5).A
- [3] BreakOutFloat2Components output 0 -> Multiply.A
- [23] Camera Space to World Space TransformVector output 0 -> Multiply.B
- [3] BreakOutFloat2Components output 1 -> Multiply.A
- [22] Camera Space to World Space TransformVector output 0 -> Multiply.B
- [4] Multiply output 0 -> Multiply.A
- [10] BreakOutFloat2Components output 0 -> Multiply.B
- [5] Multiply output 0 -> Multiply.A
- [10] BreakOutFloat2Components output 1 -> Multiply.B
- [6] Multiply output 0 -> Add.A
- [7] Multiply output 0 -> Add.B
- [30] Reroute Node (reroutes wires) output 0 -> Subtract.A
- [12] Absolute World Position output 0 -> Subtract.B
- [11] Subtract output 0 -> Add.A
- [14] Input Center Location (Vector3) output 0 -> Add.B
- [21] ComponentPivotLocation output 0 -> Input Center Location (Vector3).Preview
- [1] TexCoord[0] output 0 -> Input 0-1 UVs (Vector2).Preview
- [18] Multiply output 0 -> View Space to Tangent Space TransformVector.Input
- [16] Input Normals (Optional) (Vector3) output 0 -> Multiply.A
- [19] 1,-1,-1 output 0 -> Multiply.B
- [17] View Space to Tangent Space TransformVector output 0 -> Output Normals.A
- [25] 0,-1,0 output 0 -> Camera Space to World Space TransformVector.Input
- [24] 1,0,0 output 0 -> Camera Space to World Space TransformVector.Input
- [13] Add output 0 -> Switch.A
- [29] Reroute Node (reroutes wires) output 0 -> Switch.B
- [27] Input OverridePivot (StaticBool) output 0 -> Switch.Value
- [28] Static Bool (True) output 0 -> Input OverridePivot (StaticBool).Preview
- [8] Add output 0 -> Reroute Node (reroutes wires).Input
- [8] Add output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `World position offset` is driven by [26] Switch
- `Normals` is driven by [17] View Space to Tangent Space TransformVector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
