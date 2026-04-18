# HighPrecisionWorldPosTextureSampling

- Path: `/Engine/Functions/Engine_MaterialFunctions02/HighPrecisionWorldPosTextureSampling.HighPrecisionWorldPosTextureSampling`
- Category: Engine_MaterialFunctions02
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Provides a world position that can be used to map textures to the world without low precision artifacts at large distances.

## Inputs

- `TilingSize` [FunctionInput_Vector3] - Enter a single scalar value to scale all of the coordinates at once.
- `Texture Coord U Vector` [FunctionInput_Vector3] - Enter 1,0,0 to make the first uv channel X or 0,1,0 to make it Y or 0,01 to make it z
- `Texture Coord V Vector` [FunctionInput_Vector3] - Enter 1,0,0 to make the second uv channel X or 0,1,0 to make it Y or 0,01 to make it z
- `Texture Object` [FunctionInput_Texture2D]

## Outputs

- `High Precision World Position`
- `Texture (F4)`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output High Precision World Position | High Precision World Position |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Camera Relative World Position |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionFrac` | Frac |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input TilingSize (Vector3) | TilingSize |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output Texture (F4) | Texture (F4) |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Texture Coord U Vector (Vector3) | Texture Coord U Vector |
| 13 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Texture Coord V Vector (Vector3) | Texture Coord V Vector |
| 15 | expression | `MaterialExpressionAppendVector` | Append |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Texture Object (Texture2D) | Texture Object |
| 17 | expression | `MaterialExpressionTextureObject` | Texture Object |  |

## Exact Input Wiring

### [0] Output High Precision World Position

- `A` <= [6] Add output 0

### [1] Texture Sample

- `Coordinates` <= [15] Append output 0
- `TextureObject` <= [16] Input Texture Object (Texture2D) output 0

### [3] Divide

- `A` <= [2] Camera Relative World Position output 0
- `B` <= [8] Input TilingSize (Vector3) output 0

### [5] Divide

- `A` <= [4] Camera Position output 0
- `B` <= [8] Input TilingSize (Vector3) output 0

### [6] Add

- `A` <= [3] Divide output 0
- `B` <= [7] Frac output 0

### [7] Frac

- `Input` <= [5] Divide output 0

### [9] Output Texture (F4)

- `A` <= [10] Append output 0

### [10] Append

- `A` <= [1] Texture Sample output 0
- `B` <= [1] Texture Sample output 4

### [11] Dot

- `A` <= [6] Add output 0
- `B` <= [12] Input Texture Coord U Vector (Vector3) output 0

### [13] Dot

- `A` <= [6] Add output 0
- `B` <= [14] Input Texture Coord V Vector (Vector3) output 0

### [15] Append

- `A` <= [11] Dot output 0
- `B` <= [13] Dot output 0

### [16] Input Texture Object (Texture2D)

- `Preview` <= [17] Texture Object output 0

## Connection List

- [6] Add output 0 -> Output High Precision World Position.A
- [15] Append output 0 -> Texture Sample.Coordinates
- [16] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [2] Camera Relative World Position output 0 -> Divide.A
- [8] Input TilingSize (Vector3) output 0 -> Divide.B
- [4] Camera Position output 0 -> Divide.A
- [8] Input TilingSize (Vector3) output 0 -> Divide.B
- [3] Divide output 0 -> Add.A
- [7] Frac output 0 -> Add.B
- [5] Divide output 0 -> Frac.Input
- [10] Append output 0 -> Output Texture (F4).A
- [1] Texture Sample output 0 -> Append.A
- [1] Texture Sample output 4 -> Append.B
- [6] Add output 0 -> Dot.A
- [12] Input Texture Coord U Vector (Vector3) output 0 -> Dot.B
- [6] Add output 0 -> Dot.A
- [14] Input Texture Coord V Vector (Vector3) output 0 -> Dot.B
- [11] Dot output 0 -> Append.A
- [13] Dot output 0 -> Append.B
- [17] Texture Object output 0 -> Input Texture Object (Texture2D).Preview

## Output Trace Roots

- `High Precision World Position` is driven by [6] Add
- `Texture (F4)` is driven by [10] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


