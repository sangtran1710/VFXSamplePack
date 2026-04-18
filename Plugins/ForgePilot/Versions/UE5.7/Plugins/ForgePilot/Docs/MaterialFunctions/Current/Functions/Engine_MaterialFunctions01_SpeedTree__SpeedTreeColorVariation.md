# SpeedTreeColorVariation

- Path: `/Engine/Functions/Engine_MaterialFunctions01/SpeedTree/SpeedTreeColorVariation.SpeedTreeColorVariation`
- Category: Engine_MaterialFunctions01/SpeedTree
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Adds a color variation per object/instance to break up instances in a large forest

## Inputs

- `BaseColor` [FunctionInput_Vector3]
- `Amount` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input BaseColor (Vector3) | BaseColor |
| 2 | expression | `MaterialExpressionObjectPositionWS` | Object Position (Absolute) |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionConstant` | 0.01 |  |
| 5 | expression | `MaterialExpressionFrac` | Frac |  |
| 6 | expression | `MaterialExpressionPerInstanceRandom` | PerInstanceRandom |  |
| 7 | expression | `MaterialExpressionConstant3Vector` | 100,10,1 |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Amount (Scalar) | Amount |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 16 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 18 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [9] Add output 0

### [3] Multiply

- `A` <= [2] Object Position (Absolute) output 0
- `B` <= [4] 0.01 output 0

### [5] Frac

- `Input` <= [14] Add output 0

### [8] Multiply

- `A` <= [7] 100,10,1 output 0
- `B` <= [6] PerInstanceRandom output 0

### [9] Add

- `A` <= [10] Multiply output 0
- `B` <= [1] Input BaseColor (Vector3) output 0

### [10] Multiply

- `A` <= [13] Multiply output 0
- `B` <= [18] Normalize output 0

### [11] Mask ( R G )

- `Input` <= [5] Frac output 0

### [13] Multiply

- `A` <= [12] Input Amount (Scalar) output 0
- `B` <= [15] ConstantBiasScale output 0

### [14] Add

- `A` <= [8] Multiply output 0
- `B` <= [3] Multiply output 0

### [15] ConstantBiasScale

- `Input` <= [16] Dot output 0

### [16] Dot

- `A` <= [11] Mask ( R G ) output 0
- `B` <= [17] Mask ( G B ) output 0

### [17] Mask ( G B )

- `Input` <= [5] Frac output 0

### [18] Normalize

- `VectorInput` <= [5] Frac output 0

## Connection List

- [9] Add output 0 -> Output Result.A
- [2] Object Position (Absolute) output 0 -> Multiply.A
- [4] 0.01 output 0 -> Multiply.B
- [14] Add output 0 -> Frac.Input
- [7] 100,10,1 output 0 -> Multiply.A
- [6] PerInstanceRandom output 0 -> Multiply.B
- [10] Multiply output 0 -> Add.A
- [1] Input BaseColor (Vector3) output 0 -> Add.B
- [13] Multiply output 0 -> Multiply.A
- [18] Normalize output 0 -> Multiply.B
- [5] Frac output 0 -> Mask ( R G ).Input
- [12] Input Amount (Scalar) output 0 -> Multiply.A
- [15] ConstantBiasScale output 0 -> Multiply.B
- [8] Multiply output 0 -> Add.A
- [3] Multiply output 0 -> Add.B
- [16] Dot output 0 -> ConstantBiasScale.Input
- [11] Mask ( R G ) output 0 -> Dot.A
- [17] Mask ( G B ) output 0 -> Dot.B
- [5] Frac output 0 -> Mask ( G B ).Input
- [5] Frac output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Result` is driven by [9] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


