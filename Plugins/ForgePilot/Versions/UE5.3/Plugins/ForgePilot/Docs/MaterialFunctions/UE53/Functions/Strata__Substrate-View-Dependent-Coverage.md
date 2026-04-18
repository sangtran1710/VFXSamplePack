# Substrate-View-Dependent-Coverage

- Path: `/Engine/Functions/Strata/Substrate-View-Dependent-Coverage.Substrate-View-Dependent-Coverage`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `Thickness` [FunctionInput_Scalar]
- `Coverage` [FunctionInput_Scalar]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Thickness` - The resulting thickness. Should be unchanged compare to the input.
- `Coverage` - The resulting modified coverage.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Thickness | Thickness |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Coverage | Coverage |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Thickness (Scalar) | Thickness |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Coverage (Scalar) | Coverage |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 5 | expression | `MaterialExpressionPixelNormalWS` | PixelNormalWS |  |
| 6 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 7 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 8 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 9 | custom | `MaterialExpressionCustom` | Custom |  |
| 10 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 11 | expression | `MaterialExpressionConstant` | 0.01 |  |
| 12 | expression | `MaterialExpressionConstant` | 0 |  |
| 13 | expression | `MaterialExpressionMax` | Max(,0) |  |

## Exact Input Wiring

### [0] Output Thickness

- `A` <= [13] Max(,0) output 0

### [1] Output Coverage

- `A` <= [9] Custom output 0

### [2] Input Thickness (Scalar)

- `Preview` <= [11] 0.01 output 0

### [3] Input Coverage (Scalar)

- `Preview` <= [12] 0 output 0

### [4] Input Normal (Vector3)

- `Preview` <= [5] PixelNormalWS output 0

### [6] Dot

- `A` <= [7] Camera Vector output 0
- `B` <= [4] Input Normal (Vector3) output 0

### [8] Saturate

- `Input` <= [6] Dot output 0

### [10] Saturate

- `Input` <= [3] Input Coverage (Scalar) output 0

### [13] Max(,0)

- `A` <= [2] Input Thickness (Scalar) output 0

## Connection List

- [13] Max(,0) output 0 -> Output Thickness.A
- [9] Custom output 0 -> Output Coverage.A
- [11] 0.01 output 0 -> Input Thickness (Scalar).Preview
- [12] 0 output 0 -> Input Coverage (Scalar).Preview
- [5] PixelNormalWS output 0 -> Input Normal (Vector3).Preview
- [7] Camera Vector output 0 -> Dot.A
- [4] Input Normal (Vector3) output 0 -> Dot.B
- [6] Dot output 0 -> Saturate.Input
- [3] Input Coverage (Scalar) output 0 -> Saturate.Input
- [2] Input Thickness (Scalar) output 0 -> Max(,0).A

## Output Trace Roots

- `Thickness` is driven by [13] Max(,0)
- `Coverage` is driven by [9] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
