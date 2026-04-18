# ParticleDOF

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Particles/ParticleDOF.ParticleDOF`
- Category: Engine_MaterialFunctions03/Particles
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Use with with CircleDOF

## Inputs

- `Opacity` [FunctionInput_Scalar]

## Outputs

- `WorldPositionOffset`
- `Opacity`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output WorldPositionOffset | WorldPositionOffset |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Opacity | Opacity |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionDepthOfFieldFunction` | DepthOfFieldFunction |  |
| 6 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract(,0.5) |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 10 | expression | `MaterialExpressionAbs` | Abs |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionDivide` | Divide |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionConstant2Vector` | 2,-2 |  |
| 17 | expression | `MaterialExpressionScalarParameter` | Param (1) 'MinPixelRadius' |  |
| 18 | expression | `MaterialExpressionParticleRadius` | Particle Radius |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | function_call | `MaterialExpressionMaterialFunctionCall` | PixelInWorldUnits |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldUnitsInPixel |  |

## Exact Input Wiring

### [0] Output WorldPositionOffset

- `A` <= [4] Multiply output 0

### [1] Output Opacity

- `A` <= [3] Multiply output 0

### [3] Multiply

- `A` <= [13] Divide output 0
- `B` <= [2] Input Opacity (Scalar) output 0

### [4] Multiply

- `A` <= [20] PixelInWorldUnits output 0
- `B` <= [19] Add output 0

### [7] Subtract(,0.5)

- `A` <= [6] TexCoord[0] output 0

### [8] Multiply

- `A` <= [7] Subtract(,0.5) output 0
- `B` <= [16] 2,-2 output 0

### [9] Multiply(,1)

- `A` <= [5] DepthOfFieldFunction output 0

### [10] Abs

- `Input` <= [9] Multiply(,1) output 0

### [11] Add

- `A` <= [17] Param (1) 'MinPixelRadius' output 0
- `B` <= [21] WorldUnitsInPixel output 0

### [12] Multiply

- `A` <= [21] WorldUnitsInPixel output 0
- `B` <= [21] WorldUnitsInPixel output 0

### [13] Divide

- `A` <= [12] Multiply output 0
- `B` <= [14] Multiply output 0

### [14] Multiply

- `A` <= [15] Add output 0
- `B` <= [15] Add output 0

### [15] Add

- `A` <= [11] Add output 0
- `B` <= [10] Abs output 0

### [19] Add

- `A` <= [17] Param (1) 'MinPixelRadius' output 0
- `B` <= [10] Abs output 0

## Connection List

- [4] Multiply output 0 -> Output WorldPositionOffset.A
- [3] Multiply output 0 -> Output Opacity.A
- [13] Divide output 0 -> Multiply.A
- [2] Input Opacity (Scalar) output 0 -> Multiply.B
- [20] PixelInWorldUnits output 0 -> Multiply.A
- [19] Add output 0 -> Multiply.B
- [6] TexCoord[0] output 0 -> Subtract(,0.5).A
- [7] Subtract(,0.5) output 0 -> Multiply.A
- [16] 2,-2 output 0 -> Multiply.B
- [5] DepthOfFieldFunction output 0 -> Multiply(,1).A
- [9] Multiply(,1) output 0 -> Abs.Input
- [17] Param (1) 'MinPixelRadius' output 0 -> Add.A
- [21] WorldUnitsInPixel output 0 -> Add.B
- [21] WorldUnitsInPixel output 0 -> Multiply.A
- [21] WorldUnitsInPixel output 0 -> Multiply.B
- [12] Multiply output 0 -> Divide.A
- [14] Multiply output 0 -> Divide.B
- [15] Add output 0 -> Multiply.A
- [15] Add output 0 -> Multiply.B
- [11] Add output 0 -> Add.A
- [10] Abs output 0 -> Add.B
- [17] Param (1) 'MinPixelRadius' output 0 -> Add.A
- [10] Abs output 0 -> Add.B

## Output Trace Roots

- `WorldPositionOffset` is driven by [4] Multiply
- `Opacity` is driven by [3] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
