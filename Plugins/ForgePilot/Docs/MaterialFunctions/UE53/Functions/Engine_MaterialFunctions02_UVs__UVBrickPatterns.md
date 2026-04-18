# UVBrickPatterns

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/UVBrickPatterns.UVBrickPatterns`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Takes coordinates and manipulates thems to form an offset grid patten much like bricks. 

## Inputs

- `Float 2 Coordinates` [FunctionInput_Vector2] - Enter world position channels, uvs or any other data. 
- `Offset X (True) Or Offset Y (False)` [FunctionInput_StaticBool] - Enter world position channels, uvs or any other data. 
- `Offset Percentage` [FunctionInput_Scalar] - Amount each cell will be offset

## Outputs

- `Non-frac`
- `Frac`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionFrac` | Frac |  |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionCeil` | Ceil |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Float 2 Coordinates (Vector2) | Float 2 Coordinates |
| 9 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Offset X (True) Or Offset Y (False) (StaticBool) | Offset X (True) Or Offset Y (False) |
| 13 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 14 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 15 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 16 | expression | `MaterialExpressionAppendVector` | Append |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Offset Percentage (Scalar) | Offset Percentage |
| 18 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 19 | function_output | `MaterialExpressionFunctionOutput` | Output Non-frac | Non-frac |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Frac | Frac |

## Exact Input Wiring

### [0] Frac

- `Input` <= [10] Subtract output 0

### [1] Append

- `A` <= [14] Switch output 0
- `B` <= [6] Add output 0

### [2] Mask ( R )

- `Input` <= [8] Input Float 2 Coordinates (Vector2) output 0

### [3] Multiply

- `A` <= [5] Ceil output 0
- `B` <= [17] Input Offset Percentage (Scalar) output 0

### [4] Add

- `A` <= [14] Switch output 0
- `B` <= [17] Input Offset Percentage (Scalar) output 0

### [5] Ceil

- `Input` <= [4] Add output 0

### [6] Add

- `A` <= [3] Multiply output 0
- `B` <= [13] Switch output 0

### [7] Mask ( G )

- `Input` <= [8] Input Float 2 Coordinates (Vector2) output 0

### [8] Input Float 2 Coordinates (Vector2)

- `Preview` <= [18] TexCoord[0] output 0

### [10] Subtract

- `A` <= [15] Switch output 0
- `B` <= [11] 1-x output 0

### [11] 1-x

- `Input` <= [17] Input Offset Percentage (Scalar) output 0

### [12] Input Offset X (True) Or Offset Y (False) (StaticBool)

- `Preview` <= [9] Static Bool (False) output 0

### [13] Switch

- `A` <= [2] Mask ( R ) output 0
- `B` <= [7] Mask ( G ) output 0
- `Value` <= [12] Input Offset X (True) Or Offset Y (False) (StaticBool) output 0

### [14] Switch

- `A` <= [7] Mask ( G ) output 0
- `B` <= [2] Mask ( R ) output 0
- `Value` <= [12] Input Offset X (True) Or Offset Y (False) (StaticBool) output 0

### [15] Switch

- `A` <= [16] Append output 0
- `B` <= [1] Append output 0
- `Value` <= [12] Input Offset X (True) Or Offset Y (False) (StaticBool) output 0

### [16] Append

- `A` <= [6] Add output 0
- `B` <= [14] Switch output 0

### [19] Output Non-frac

- `A` <= [10] Subtract output 0

### [20] Output Frac

- `A` <= [0] Frac output 0

## Connection List

- [10] Subtract output 0 -> Frac.Input
- [14] Switch output 0 -> Append.A
- [6] Add output 0 -> Append.B
- [8] Input Float 2 Coordinates (Vector2) output 0 -> Mask ( R ).Input
- [5] Ceil output 0 -> Multiply.A
- [17] Input Offset Percentage (Scalar) output 0 -> Multiply.B
- [14] Switch output 0 -> Add.A
- [17] Input Offset Percentage (Scalar) output 0 -> Add.B
- [4] Add output 0 -> Ceil.Input
- [3] Multiply output 0 -> Add.A
- [13] Switch output 0 -> Add.B
- [8] Input Float 2 Coordinates (Vector2) output 0 -> Mask ( G ).Input
- [18] TexCoord[0] output 0 -> Input Float 2 Coordinates (Vector2).Preview
- [15] Switch output 0 -> Subtract.A
- [11] 1-x output 0 -> Subtract.B
- [17] Input Offset Percentage (Scalar) output 0 -> 1-x.Input
- [9] Static Bool (False) output 0 -> Input Offset X (True) Or Offset Y (False) (StaticBool).Preview
- [2] Mask ( R ) output 0 -> Switch.A
- [7] Mask ( G ) output 0 -> Switch.B
- [12] Input Offset X (True) Or Offset Y (False) (StaticBool) output 0 -> Switch.Value
- [7] Mask ( G ) output 0 -> Switch.A
- [2] Mask ( R ) output 0 -> Switch.B
- [12] Input Offset X (True) Or Offset Y (False) (StaticBool) output 0 -> Switch.Value
- [16] Append output 0 -> Switch.A
- [1] Append output 0 -> Switch.B
- [12] Input Offset X (True) Or Offset Y (False) (StaticBool) output 0 -> Switch.Value
- [6] Add output 0 -> Append.A
- [14] Switch output 0 -> Append.B
- [10] Subtract output 0 -> Output Non-frac.A
- [0] Frac output 0 -> Output Frac.A

## Output Trace Roots

- `Non-frac` is driven by [10] Subtract
- `Frac` is driven by [0] Frac

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
