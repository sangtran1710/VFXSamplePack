# WorldPositionWithScale

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/WorldPositionWithScale.WorldPositionWithScale`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

WorldPosition divided by a number and split into common configurations.

## Inputs

- `Divisor` [FunctionInput_Vector3]

## Outputs

- `XYZ`
- `XY`
- `Z`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ | XYZ |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output XY | XY |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Z | Z |
| 6 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Divisor (Vector3) | Divisor |

## Exact Input Wiring

### [0] Output XYZ

- `A` <= [1] Mask ( R G B ) output 0

### [1] Mask ( R G B )

- `Input` <= [7] Divide output 0

### [2] Mask ( R G )

- `Input` <= [7] Divide output 0

### [3] Mask ( B )

- `Input` <= [7] Divide output 0

### [4] Output XY

- `A` <= [2] Mask ( R G ) output 0

### [5] Output Z

- `A` <= [3] Mask ( B ) output 0

### [7] Divide

- `A` <= [6] Absolute World Position output 0
- `B` <= [8] Input Divisor (Vector3) output 0

## Connection List

- [1] Mask ( R G B ) output 0 -> Output XYZ.A
- [7] Divide output 0 -> Mask ( R G B ).Input
- [7] Divide output 0 -> Mask ( R G ).Input
- [7] Divide output 0 -> Mask ( B ).Input
- [2] Mask ( R G ) output 0 -> Output XY.A
- [3] Mask ( B ) output 0 -> Output Z.A
- [6] Absolute World Position output 0 -> Divide.A
- [8] Input Divisor (Vector3) output 0 -> Divide.B

## Output Trace Roots

- `XYZ` is driven by [1] Mask ( R G B )
- `XY` is driven by [2] Mask ( R G )
- `Z` is driven by [3] Mask ( B )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
