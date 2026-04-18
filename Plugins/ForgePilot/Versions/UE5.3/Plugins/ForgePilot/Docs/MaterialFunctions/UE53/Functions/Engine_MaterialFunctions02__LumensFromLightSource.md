# LumensFromLightSource

- Path: `/Engine/Functions/Engine_MaterialFunctions02/LumensFromLightSource.LumensFromLightSource`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the per pixel brightness of a spherical light at a given radius.

## Inputs

- `Light Color` [FunctionInput_Vector3]
- `Light Source Lumens` [FunctionInput_Scalar]
- `Light Radius` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionConstant` | 16 |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Light Color (Vector3) | Light Color |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Light Source Lumens (Scalar) | Light Source Lumens |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Light Radius (Scalar) | Light Radius |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [10] Divide output 0

### [2] Multiply

- `A` <= [1] 16 output 0
- `B` <= [4] Input Light Color (Vector3) output 0

### [3] Multiply

- `A` <= [2] Multiply output 0
- `B` <= [5] Input Light Source Lumens (Scalar) output 0

### [7] Multiply

- `A` <= [6] Input Light Radius (Scalar) output 0
- `B` <= [6] Input Light Radius (Scalar) output 0

### [9] Multiply

- `A` <= [7] Multiply output 0
- `B` <= [8] Pi output 0

### [10] Divide

- `A` <= [3] Multiply output 0
- `B` <= [9] Multiply output 0

## Connection List

- [10] Divide output 0 -> Output Result.A
- [1] 16 output 0 -> Multiply.A
- [4] Input Light Color (Vector3) output 0 -> Multiply.B
- [2] Multiply output 0 -> Multiply.A
- [5] Input Light Source Lumens (Scalar) output 0 -> Multiply.B
- [6] Input Light Radius (Scalar) output 0 -> Multiply.A
- [6] Input Light Radius (Scalar) output 0 -> Multiply.B
- [7] Multiply output 0 -> Multiply.A
- [8] Pi output 0 -> Multiply.B
- [3] Multiply output 0 -> Divide.A
- [9] Multiply output 0 -> Divide.B

## Output Trace Roots

- `Result` is driven by [10] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
