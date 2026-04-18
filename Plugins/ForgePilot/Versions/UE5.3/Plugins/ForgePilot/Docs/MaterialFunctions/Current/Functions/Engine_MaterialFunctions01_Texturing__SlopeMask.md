# SlopeMask

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SlopeMask.SlopeMask`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Slopemask, useful for stuff like snow, moss

## Inputs

- `TangentNormal` [FunctionInput_Vector3]
- `CheapContrast` [FunctionInput_Scalar]
- `FalloffPower` [FunctionInput_Scalar]
- `SlopAngle` [FunctionInput_Vector3] - World direction for slop, typically points down

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input TangentNormal (Vector3) | TangentNormal |
| 2 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input CheapContrast (Scalar) | CheapContrast |
| 7 | expression | `MaterialExpressionPower` | Power |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input FalloffPower (Scalar) | FalloffPower |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input SlopAngle (Vector3) | SlopAngle |

## Exact Input Wiring

### [0] Output Result

- `A` <= [5] CheapContrast output 0

### [2] Tangent Space to World Space TransformVector

- `Input` <= [1] Input TangentNormal (Vector3) output 0

### [3] Dot

- `A` <= [2] Tangent Space to World Space TransformVector output 0
- `B` <= [9] Input SlopAngle (Vector3) output 0

### [4] Clamp

- `Input` <= [3] Dot output 0

### [7] Power

- `Base` <= [4] Clamp output 0
- `Exponent` <= [8] Input FalloffPower (Scalar) output 0

## Connection List

- [5] CheapContrast output 0 -> Output Result.A
- [1] Input TangentNormal (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [2] Tangent Space to World Space TransformVector output 0 -> Dot.A
- [9] Input SlopAngle (Vector3) output 0 -> Dot.B
- [3] Dot output 0 -> Clamp.Input
- [4] Clamp output 0 -> Power.Base
- [8] Input FalloffPower (Scalar) output 0 -> Power.Exponent

## Output Trace Roots

- `Result` is driven by [5] CheapContrast

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


