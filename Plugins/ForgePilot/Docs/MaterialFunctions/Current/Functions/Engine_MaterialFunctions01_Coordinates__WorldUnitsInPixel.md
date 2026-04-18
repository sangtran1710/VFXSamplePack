# WorldUnitsInPixel

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/WorldUnitsInPixel.WorldUnitsInPixel`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Depth` [FunctionInput_Scalar] - from camera e.g. PixelDepth
- `WorldUnits` [FunctionInput_Scalar]

## Outputs

- `PIxels`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | custom | `MaterialExpressionCustom` | Custom |  |
| 1 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 5 | expression | `MaterialExpressionPixelDepth` | PixelDepth |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output PIxels | PIxels |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Depth (Scalar) | Depth |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input WorldUnits (Scalar) | WorldUnits |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [2] Multiply

- `A` <= [0] Custom output 0
- `B` <= [1] ViewSize output 0

### [3] Mask ( R )

- `Input` <= [2] Multiply output 0

### [4] Multiply(,0.5)

- `A` <= [3] Mask ( R ) output 0

### [6] Divide

- `A` <= [4] Multiply(,0.5) output 0
- `B` <= [8] Input Depth (Scalar) output 0

### [7] Output PIxels

- `A` <= [10] Multiply output 0

### [8] Input Depth (Scalar)

- `Preview` <= [5] PixelDepth output 0

### [10] Multiply

- `A` <= [9] Input WorldUnits (Scalar) output 0
- `B` <= [6] Divide output 0

## Connection List

- [0] Custom output 0 -> Multiply.A
- [1] ViewSize output 0 -> Multiply.B
- [2] Multiply output 0 -> Mask ( R ).Input
- [3] Mask ( R ) output 0 -> Multiply(,0.5).A
- [4] Multiply(,0.5) output 0 -> Divide.A
- [8] Input Depth (Scalar) output 0 -> Divide.B
- [10] Multiply output 0 -> Output PIxels.A
- [5] PixelDepth output 0 -> Input Depth (Scalar).Preview
- [9] Input WorldUnits (Scalar) output 0 -> Multiply.A
- [6] Divide output 0 -> Multiply.B

## Output Trace Roots

- `PIxels` is driven by [10] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


