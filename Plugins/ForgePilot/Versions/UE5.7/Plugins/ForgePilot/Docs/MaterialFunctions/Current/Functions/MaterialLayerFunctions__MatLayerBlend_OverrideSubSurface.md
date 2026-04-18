# MatLayerBlend_OverrideSubSurface

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideSubSurface.MatLayerBlend_OverrideSubSurface`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Replace the Subsurface Color and Opacity

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Mask` [FunctionInput_Scalar]
- `NewSubSurfaceColor` [FunctionInput_Vector3]
- `NewOpacity` [FunctionInput_Scalar]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Mask (Scalar) | Mask |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input NewSubSurfaceColor (Vector3) | NewSubSurfaceColor |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input NewOpacity (Scalar) | NewOpacity |
| 7 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 8 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [8] SetMaterialAttributes output 0

### [2] Lerp

- `A` <= [7] GetMaterialAttributes output 2
- `B` <= [4] Input NewSubSurfaceColor (Vector3) output 0
- `Alpha` <= [3] Input Mask (Scalar) output 0

### [5] Lerp

- `A` <= [7] GetMaterialAttributes output 1
- `B` <= [6] Input NewOpacity (Scalar) output 0
- `Alpha` <= [3] Input Mask (Scalar) output 0

### [8] SetMaterialAttributes

- `Inputs[0]` <= [7] GetMaterialAttributes output 0
- `Inputs[1]` <= [5] Lerp output 0
- `Inputs[2]` <= [2] Lerp output 0

## Connection List

- [8] SetMaterialAttributes output 0 -> Output Blended Material.A
- [7] GetMaterialAttributes output 2 -> Lerp.A
- [4] Input NewSubSurfaceColor (Vector3) output 0 -> Lerp.B
- [3] Input Mask (Scalar) output 0 -> Lerp.Alpha
- [7] GetMaterialAttributes output 1 -> Lerp.A
- [6] Input NewOpacity (Scalar) output 0 -> Lerp.B
- [3] Input Mask (Scalar) output 0 -> Lerp.Alpha
- [7] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [2] Lerp output 0 -> SetMaterialAttributes.Inputs[2]

## Output Trace Roots

- `Blended Material` is driven by [8] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


