# SCurve

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/SCurve.SCurve`
- Category: Engine_MaterialFunctions01/ImageAdjustment
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Applies an S curve image adjustment

## Inputs

- `Power` [FunctionInput_Scalar]
- `In` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 3 | expression | `MaterialExpressionPower` | Power |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Power (Scalar) | Power |
| 5 | expression | `MaterialExpressionClamp` | Clamp |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input In (Vector3) | In |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 10 | expression | `MaterialExpressionConstant` | 2 |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 14 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 15 | expression | `MaterialExpressionPower` | Power |  |
| 16 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 17 | expression | `MaterialExpressionClamp` | Clamp |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionConstant` | 2 |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 22 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | expression | `MaterialExpressionConstant` | 500 |  |
| 25 | expression | `MaterialExpressionClamp` | Clamp |  |
| 26 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [26] Lerp output 0

### [1] Multiply

- `A` <= [3] Power output 0
- `B` <= [2] 0.5 output 0

### [3] Power

- `Base` <= [5] Clamp output 0
- `Exponent` <= [4] Input Power (Scalar) output 0

### [5] Clamp

- `Input` <= [6] Multiply output 0

### [6] Multiply

- `A` <= [10] 2 output 0
- `B` <= [7] Input In (Vector3) output 0

### [7] Input In (Vector3)

- `Preview` <= [8] Mask ( R ) output 0

### [8] Mask ( R )

- `Input` <= [9] TexCoord[0] output 0

### [11] Add

- `A` <= [22] 0.5 output 0
- `B` <= [12] Multiply output 0

### [12] Multiply

- `A` <= [14] 1-x output 0
- `B` <= [13] 0.5 output 0

### [14] 1-x

- `Input` <= [15] Power output 0

### [15] Power

- `Base` <= [16] 1-x output 0
- `Exponent` <= [4] Input Power (Scalar) output 0

### [16] 1-x

- `Input` <= [17] Clamp output 0

### [17] Clamp

- `Input` <= [18] Multiply output 0

### [18] Multiply

- `A` <= [20] Add output 0
- `B` <= [19] 2 output 0

### [20] Add

- `A` <= [7] Input In (Vector3) output 0
- `B` <= [21] -0.5 output 0

### [23] Multiply

- `A` <= [14] 1-x output 0
- `B` <= [24] 500 output 0

### [25] Clamp

- `Input` <= [23] Multiply output 0

### [26] Lerp

- `A` <= [1] Multiply output 0
- `B` <= [11] Add output 0
- `Alpha` <= [25] Clamp output 0

## Connection List

- [26] Lerp output 0 -> Output Result.A
- [3] Power output 0 -> Multiply.A
- [2] 0.5 output 0 -> Multiply.B
- [5] Clamp output 0 -> Power.Base
- [4] Input Power (Scalar) output 0 -> Power.Exponent
- [6] Multiply output 0 -> Clamp.Input
- [10] 2 output 0 -> Multiply.A
- [7] Input In (Vector3) output 0 -> Multiply.B
- [8] Mask ( R ) output 0 -> Input In (Vector3).Preview
- [9] TexCoord[0] output 0 -> Mask ( R ).Input
- [22] 0.5 output 0 -> Add.A
- [12] Multiply output 0 -> Add.B
- [14] 1-x output 0 -> Multiply.A
- [13] 0.5 output 0 -> Multiply.B
- [15] Power output 0 -> 1-x.Input
- [16] 1-x output 0 -> Power.Base
- [4] Input Power (Scalar) output 0 -> Power.Exponent
- [17] Clamp output 0 -> 1-x.Input
- [18] Multiply output 0 -> Clamp.Input
- [20] Add output 0 -> Multiply.A
- [19] 2 output 0 -> Multiply.B
- [7] Input In (Vector3) output 0 -> Add.A
- [21] -0.5 output 0 -> Add.B
- [14] 1-x output 0 -> Multiply.A
- [24] 500 output 0 -> Multiply.B
- [23] Multiply output 0 -> Clamp.Input
- [1] Multiply output 0 -> Lerp.A
- [11] Add output 0 -> Lerp.B
- [25] Clamp output 0 -> Lerp.Alpha

## Output Trace Roots

- `Result` is driven by [26] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
