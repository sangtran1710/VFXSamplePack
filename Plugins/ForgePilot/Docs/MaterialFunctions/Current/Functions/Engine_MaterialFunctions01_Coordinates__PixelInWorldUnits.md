# PixelInWorldUnits

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/PixelInWorldUnits.PixelInWorldUnits`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Depth` [FunctionInput_Scalar] - from camera e.g. PixelDepth
- `Pixel` [FunctionInput_Vector2] - offset

## Outputs

- `PixelInWorldUnits`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | custom | `MaterialExpressionCustom` | Custom |  |
| 1 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 6 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Depth (Scalar) | Depth |
| 8 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 9 | expression | `MaterialExpressionAppendVector` | Append |  |
| 10 | expression | `MaterialExpressionConstant` | 0 |  |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output PixelInWorldUnits | PixelInWorldUnits |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Pixel (Vector2) | Pixel |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [2] Multiply

- `A` <= [0] Custom output 0
- `B` <= [1] ViewSize output 0

### [3] Divide

- `A` <= [7] Input Depth (Scalar) output 0
- `B` <= [5] Multiply(,0.5) output 0

### [4] Mask ( R )

- `Input` <= [2] Multiply output 0

### [5] Multiply(,0.5)

- `A` <= [4] Mask ( R ) output 0

### [7] Input Depth (Scalar)

- `Preview` <= [6] PixelDepth output 0

### [8] View Space to World Space TransformVector

- `Input` <= [9] Append output 0

### [9] Append

- `A` <= [13] Multiply output 0
- `B` <= [10] 0 output 0

### [11] Output PixelInWorldUnits

- `A` <= [8] View Space to World Space TransformVector output 0

### [13] Multiply

- `A` <= [12] Input Pixel (Vector2) output 0
- `B` <= [3] Divide output 0

## Connection List

- [0] Custom output 0 -> Multiply.A
- [1] ViewSize output 0 -> Multiply.B
- [7] Input Depth (Scalar) output 0 -> Divide.A
- [5] Multiply(,0.5) output 0 -> Divide.B
- [2] Multiply output 0 -> Mask ( R ).Input
- [4] Mask ( R ) output 0 -> Multiply(,0.5).A
- [6] PixelDepth output 0 -> Input Depth (Scalar).Preview
- [9] Append output 0 -> View Space to World Space TransformVector.Input
- [13] Multiply output 0 -> Append.A
- [10] 0 output 0 -> Append.B
- [8] View Space to World Space TransformVector output 0 -> Output PixelInWorldUnits.A
- [12] Input Pixel (Vector2) output 0 -> Multiply.A
- [3] Divide output 0 -> Multiply.B

## Output Trace Roots

- `PixelInWorldUnits` is driven by [8] View Space to World Space TransformVector

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


