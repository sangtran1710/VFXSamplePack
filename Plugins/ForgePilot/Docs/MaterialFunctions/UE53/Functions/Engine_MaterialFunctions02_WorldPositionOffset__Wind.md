# Wind

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Wind.Wind`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Seperate outputs for wind strength, speed multiplied by time and a normalized wind vector.

## Inputs

- `WindActor` [FunctionInput_Vector4] - Needs tp be replaced with the wind actor when it becomes available

## Outputs

- `WindSpeed` - Wind speed multiplied by time.
- `Wind Strength` - Returns wind strength. The magnitude of the wind vector found by calculating distance from the wind vector to 0. 
- `Normalized Wind Vector` - Normalize Wind Vector
- `WindActor` - Standard WindActor node.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionTime` | Time |  |
| 3 | function_output | `MaterialExpressionFunctionOutput` | Output WindSpeed | WindSpeed |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Wind Strength | Wind Strength |
| 6 | expression | `MaterialExpressionDistance` | Distance |  |
| 7 | expression | `MaterialExpressionConstant` | 0 |  |
| 8 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 9 | function_output | `MaterialExpressionFunctionOutput` | Output Normalized Wind Vector | Normalized Wind Vector |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output WindActor | WindActor |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input WindActor (Vector4) | WindActor |

## Exact Input Wiring

### [0] Mask ( A )

- `Input` <= [11] Input WindActor (Vector4) output 0

### [1] Multiply

- `A` <= [0] Mask ( A ) output 0
- `B` <= [2] Time output 0

### [3] Output WindSpeed

- `A` <= [1] Multiply output 0

### [4] Mask ( R G B )

- `Input` <= [11] Input WindActor (Vector4) output 0

### [5] Output Wind Strength

- `A` <= [6] Distance output 0

### [6] Distance

- `A` <= [4] Mask ( R G B ) output 0
- `B` <= [7] 0 output 0

### [8] Normalize

- `VectorInput` <= [4] Mask ( R G B ) output 0

### [9] Output Normalized Wind Vector

- `A` <= [8] Normalize output 0

### [10] Output WindActor

- `A` <= [11] Input WindActor (Vector4) output 0

## Connection List

- [11] Input WindActor (Vector4) output 0 -> Mask ( A ).Input
- [0] Mask ( A ) output 0 -> Multiply.A
- [2] Time output 0 -> Multiply.B
- [1] Multiply output 0 -> Output WindSpeed.A
- [11] Input WindActor (Vector4) output 0 -> Mask ( R G B ).Input
- [6] Distance output 0 -> Output Wind Strength.A
- [4] Mask ( R G B ) output 0 -> Distance.A
- [7] 0 output 0 -> Distance.B
- [4] Mask ( R G B ) output 0 -> Normalize.VectorInput
- [8] Normalize output 0 -> Output Normalized Wind Vector.A
- [11] Input WindActor (Vector4) output 0 -> Output WindActor.A

## Output Trace Roots

- `WindSpeed` is driven by [1] Multiply
- `Wind Strength` is driven by [6] Distance
- `Normalized Wind Vector` is driven by [8] Normalize
- `WindActor` is driven by [11] Input WindActor (Vector4)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
