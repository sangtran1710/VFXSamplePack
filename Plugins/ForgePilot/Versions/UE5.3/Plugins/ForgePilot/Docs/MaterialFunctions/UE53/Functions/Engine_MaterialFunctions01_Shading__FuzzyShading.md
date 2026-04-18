# FuzzyShading

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/FuzzyShading.FuzzyShading`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function emulates shading simmilar to velvet or moss

## Inputs

- `Normal` [FunctionInput_Vector3]
- `CoreDarkness` [FunctionInput_Scalar]
- `EdgeBrightness` [FunctionInput_Scalar]
- `BaseColor` [FunctionInput_Vector3]
- `Power` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | expression | `MaterialExpressionConstant` | 0 |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input CoreDarkness (Scalar) | CoreDarkness |
| 8 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 9 | expression | `MaterialExpressionPower` | Power |  |
| 10 | expression | `MaterialExpressionConstant` | 6 |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input EdgeBrightness (Scalar) | EdgeBrightness |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input BaseColor (Vector3) | BaseColor |
| 16 | expression | `MaterialExpressionClamp` | Clamp |  |
| 17 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 18 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Power (Scalar) | Power |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionConstant2Vector` | 1,1 |  |
| 22 | expression | `MaterialExpressionAppendVector` | Append |  |
| 23 | expression | `MaterialExpressionTwoSidedSign` | TwoSidedSign |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [14] Multiply output 0

### [1] Dot

- `A` <= [17] Camera Vector output 0
- `B` <= [18] Tangent Space to World Space TransformVector output 0

### [2] 1-x

- `Input` <= [6] Multiply output 0

### [4] Add

- `A` <= [2] 1-x output 0
- `B` <= [3] 0 output 0

### [6] Multiply

- `A` <= [16] Clamp output 0
- `B` <= [7] Input CoreDarkness (Scalar) output 0

### [8] 1-x

- `Input` <= [16] Clamp output 0

### [9] Power

- `Base` <= [8] 1-x output 0
- `Exponent` <= [19] Input Power (Scalar) output 0

### [11] Multiply

- `A` <= [9] Power output 0
- `B` <= [12] Input EdgeBrightness (Scalar) output 0

### [13] Add

- `A` <= [11] Multiply output 0
- `B` <= [4] Add output 0

### [14] Multiply

- `A` <= [13] Add output 0
- `B` <= [15] Input BaseColor (Vector3) output 0

### [16] Clamp

- `Input` <= [1] Dot output 0

### [18] Tangent Space to World Space TransformVector

- `Input` <= [20] Multiply output 0

### [20] Multiply

- `A` <= [5] Input Normal (Vector3) output 0
- `B` <= [22] Append output 0

### [22] Append

- `A` <= [21] 1,1 output 0
- `B` <= [23] TwoSidedSign output 0

## Connection List

- [14] Multiply output 0 -> Output Result.A
- [17] Camera Vector output 0 -> Dot.A
- [18] Tangent Space to World Space TransformVector output 0 -> Dot.B
- [6] Multiply output 0 -> 1-x.Input
- [2] 1-x output 0 -> Add.A
- [3] 0 output 0 -> Add.B
- [16] Clamp output 0 -> Multiply.A
- [7] Input CoreDarkness (Scalar) output 0 -> Multiply.B
- [16] Clamp output 0 -> 1-x.Input
- [8] 1-x output 0 -> Power.Base
- [19] Input Power (Scalar) output 0 -> Power.Exponent
- [9] Power output 0 -> Multiply.A
- [12] Input EdgeBrightness (Scalar) output 0 -> Multiply.B
- [11] Multiply output 0 -> Add.A
- [4] Add output 0 -> Add.B
- [13] Add output 0 -> Multiply.A
- [15] Input BaseColor (Vector3) output 0 -> Multiply.B
- [1] Dot output 0 -> Clamp.Input
- [20] Multiply output 0 -> Tangent Space to World Space TransformVector.Input
- [5] Input Normal (Vector3) output 0 -> Multiply.A
- [22] Append output 0 -> Multiply.B
- [21] 1,1 output 0 -> Append.A
- [23] TwoSidedSign output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [14] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
