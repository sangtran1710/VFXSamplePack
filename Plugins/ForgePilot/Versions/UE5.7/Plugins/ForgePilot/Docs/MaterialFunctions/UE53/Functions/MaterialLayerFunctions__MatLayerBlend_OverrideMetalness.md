# MatLayerBlend_OverrideMetalness

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideMetalness.MatLayerBlend_OverrideMetalness`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Replace the Metalness

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Mask` [FunctionInput_Scalar]
- `NewMetalness` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Mask (Scalar) | Mask |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input NewMetalness (Vector3) | NewMetalness |
| 5 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 7 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 8 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [8] SetMaterialAttributes output 0

### [1] Input Material (MaterialAttributes)

- `Preview` <= [5] MakeMaterialAttributes output 0

### [2] Lerp

- `A` <= [7] GetMaterialAttributes output 1
- `B` <= [4] Input NewMetalness (Vector3) output 0
- `Alpha` <= [3] Input Mask (Scalar) output 0

### [6] Mask ( R )

- `Input` <= [7] GetMaterialAttributes output 2

### [8] SetMaterialAttributes

- `Inputs[0]` <= [7] GetMaterialAttributes output 0
- `Inputs[1]` <= [2] Lerp output 0
- `Inputs[2]` <= [6] Mask ( R ) output 0

## Connection List

- [8] SetMaterialAttributes output 0 -> Output Blended Material.A
- [5] MakeMaterialAttributes output 0 -> Input Material (MaterialAttributes).Preview
- [7] GetMaterialAttributes output 1 -> Lerp.A
- [4] Input NewMetalness (Vector3) output 0 -> Lerp.B
- [3] Input Mask (Scalar) output 0 -> Lerp.Alpha
- [7] GetMaterialAttributes output 2 -> Mask ( R ).Input
- [7] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [2] Lerp output 0 -> SetMaterialAttributes.Inputs[1]
- [6] Mask ( R ) output 0 -> SetMaterialAttributes.Inputs[2]

## Output Trace Roots

- `Blended Material` is driven by [8] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
