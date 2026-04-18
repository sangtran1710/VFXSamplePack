# BoundingBoxBased_0-1_UVW

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/BoundingBoxBased_0-1_UVW.BoundingBoxBased_0-1_UVW`
- Category: Engine_MaterialFunctions02/UVs
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Creates a locally aligned 0-1 value on XY and Z.

## Inputs

- None

## Outputs

- `RGB`
- `R`
- `G`
- `B`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 1 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectLocalBounds |  |
| 5 | expression | `MaterialExpressionDivide` | Divide |  |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output R | R |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output G | G |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output B | B |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |

## Exact Input Wiring

### [0] Output RGB

- `A` <= [5] Divide output 0

### [1] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [2] Absolute World Position (Excluding Material Offsets) output 0

### [3] Subtract

- `A` <= [1] Absolute World Space to Instance & Particle Space TransformPosition output 0
- `B` <= [4] ObjectLocalBounds output 0

### [5] Divide

- `A` <= [3] Subtract output 0
- `B` <= [4] ObjectLocalBounds output 2

### [6] Output R

- `A` <= [9] BreakOutFloat3Components output 0

### [7] Output G

- `A` <= [9] BreakOutFloat3Components output 1

### [8] Output B

- `A` <= [9] BreakOutFloat3Components output 2

## Connection List

- [5] Divide output 0 -> Output RGB.A
- [2] Absolute World Position (Excluding Material Offsets) output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [1] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Subtract.A
- [4] ObjectLocalBounds output 0 -> Subtract.B
- [3] Subtract output 0 -> Divide.A
- [4] ObjectLocalBounds output 2 -> Divide.B
- [9] BreakOutFloat3Components output 0 -> Output R.A
- [9] BreakOutFloat3Components output 1 -> Output G.A
- [9] BreakOutFloat3Components output 2 -> Output B.A

## Output Trace Roots

- `RGB` is driven by [5] Divide
- `R` is driven by [9] BreakOutFloat3Components
- `G` is driven by [9] BreakOutFloat3Components
- `B` is driven by [9] BreakOutFloat3Components

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


