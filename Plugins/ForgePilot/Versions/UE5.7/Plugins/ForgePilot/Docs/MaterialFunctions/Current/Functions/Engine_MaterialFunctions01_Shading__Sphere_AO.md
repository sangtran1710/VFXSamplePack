# Sphere_AO

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Sphere_AO.Sphere_AO`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function can be used to calculate the Ambient Occlusion for a sphere affecting the world at any point. It is computed by getting the area of the hemisphere occluded by the occluder.

## Inputs

- `Sphere Position` [FunctionInput_Vector3] - The light position to calculate light source angle for. For a directional light the light source angle is always the same.
- `Source Radius` [FunctionInput_Scalar] - This is the size of the light source radius in world units.

## Outputs

- `AO Value`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output AO Value | AO Value |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Position (Vector3) | Sphere Position |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Source Radius (Scalar) | Source Radius |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 4 | expression | `MaterialExpressionConstant` | 2 |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionClamp` | Clamp |  |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionDistance` | Distance |  |
| 10 | expression | `MaterialExpressionDivide` | Divide |  |
| 11 | expression | `MaterialExpressionMax` | Max |  |
| 12 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | pythagoreanTheorum |  |
| 14 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output AO Value

- `A` <= [14] 1-x output 0

### [5] Subtract

- `A` <= [3] Pi output 0
- `B` <= [8] Multiply output 0

### [6] Clamp

- `Input` <= [7] Divide output 0

### [7] Divide

- `A` <= [5] Subtract output 0
- `B` <= [3] Pi output 0

### [8] Multiply

- `A` <= [3] Pi output 0
- `B` <= [13] pythagoreanTheorum output 0

### [9] Distance

- `A` <= [1] Input Sphere Position (Vector3) output 0
- `B` <= [12] Absolute World Position output 0

### [10] Divide

- `A` <= [2] Input Source Radius (Scalar) output 0
- `B` <= [11] Max output 0

### [11] Max

- `A` <= [2] Input Source Radius (Scalar) output 0
- `B` <= [9] Distance output 0

### [14] 1-x

- `Input` <= [6] Clamp output 0

## Connection List

- [14] 1-x output 0 -> Output AO Value.A
- [3] Pi output 0 -> Subtract.A
- [8] Multiply output 0 -> Subtract.B
- [7] Divide output 0 -> Clamp.Input
- [5] Subtract output 0 -> Divide.A
- [3] Pi output 0 -> Divide.B
- [3] Pi output 0 -> Multiply.A
- [13] pythagoreanTheorum output 0 -> Multiply.B
- [1] Input Sphere Position (Vector3) output 0 -> Distance.A
- [12] Absolute World Position output 0 -> Distance.B
- [2] Input Source Radius (Scalar) output 0 -> Divide.A
- [11] Max output 0 -> Divide.B
- [2] Input Source Radius (Scalar) output 0 -> Max.A
- [9] Distance output 0 -> Max.B
- [6] Clamp output 0 -> 1-x.Input

## Output Trace Roots

- `AO Value` is driven by [14] 1-x

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


