# CalcLightsourceAngle

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/CalcLightsourceAngle.CalcLightsourceAngle`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function can be used to calculate the light source angle of a pointlight given a certain radius in world space.

## Inputs

- `Light Position` [FunctionInput_Vector3] - The light position to calculate light source angle for. For a directional light the light source angle is always the same.
- `Source Radius` [FunctionInput_Scalar] - This is the size of the light source radius in world units.

## Outputs

- `Degrees`
- `Radians`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Degrees | Degrees |
| 1 | expression | `MaterialExpressionDistance` | Distance |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 3 | expression | `MaterialExpressionDivide` | Divide |  |
| 4 | custom | `MaterialExpressionCustom` | asin(x) |  |
| 5 | expression | `MaterialExpressionConstant` | 57.3 |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMax` | Max |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Light Position (Vector3) | Light Position |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Source Radius (Scalar) | Source Radius |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Radians | Radians |

## Exact Input Wiring

### [0] Output Degrees

- `A` <= [6] Multiply output 0

### [1] Distance

- `A` <= [8] Input Light Position (Vector3) output 0
- `B` <= [2] Absolute World Position output 0

### [3] Divide

- `A` <= [9] Input Source Radius (Scalar) output 0
- `B` <= [7] Max output 0

### [6] Multiply

- `A` <= [10] Output Radians output 0
- `B` <= [5] 57.3 output 0

### [7] Max

- `A` <= [9] Input Source Radius (Scalar) output 0
- `B` <= [1] Distance output 0

### [10] Output Radians

- `A` <= [4] asin(x) output 0

## Connection List

- [6] Multiply output 0 -> Output Degrees.A
- [8] Input Light Position (Vector3) output 0 -> Distance.A
- [2] Absolute World Position output 0 -> Distance.B
- [9] Input Source Radius (Scalar) output 0 -> Divide.A
- [7] Max output 0 -> Divide.B
- [10] Output Radians output 0 -> Multiply.A
- [5] 57.3 output 0 -> Multiply.B
- [9] Input Source Radius (Scalar) output 0 -> Max.A
- [1] Distance output 0 -> Max.B
- [4] asin(x) output 0 -> Output Radians.A

## Output Trace Roots

- `Degrees` is driven by [6] Multiply
- `Radians` is driven by [4] asin(x)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


