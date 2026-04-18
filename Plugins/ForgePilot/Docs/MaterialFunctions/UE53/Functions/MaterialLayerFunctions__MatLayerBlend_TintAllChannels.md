# MatLayerBlend_TintAllChannels

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_TintAllChannels.MatLayerBlend_TintAllChannels`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Simple Tinting of Base Color

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Tint` [FunctionInput_Vector3] - This Color Multiplies against the BaseColor of the provided Material.
- `TintMask` [FunctionInput_Scalar] - Masks where the tint is applied.

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Tint (Vector3) | Tint |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input TintMask (Scalar) | TintMask |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 7 | expression | `MaterialExpressionConstant` | 1 |  |
| 8 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 9 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [9] SetMaterialAttributes output 0

### [2] Multiply

- `A` <= [8] GetMaterialAttributes output 1
- `B` <= [6] Lerp output 0

### [5] Multiply

- `A` <= [8] GetMaterialAttributes output 2
- `B` <= [6] Lerp output 0

### [6] Lerp

- `A` <= [7] 1 output 0
- `B` <= [3] Input Tint (Vector3) output 0
- `Alpha` <= [4] Input TintMask (Scalar) output 0

### [9] SetMaterialAttributes

- `Inputs[0]` <= [8] GetMaterialAttributes output 0
- `Inputs[1]` <= [2] Multiply output 0
- `Inputs[2]` <= [5] Multiply output 0

## Connection List

- [9] SetMaterialAttributes output 0 -> Output Blended Material.A
- [8] GetMaterialAttributes output 1 -> Multiply.A
- [6] Lerp output 0 -> Multiply.B
- [8] GetMaterialAttributes output 2 -> Multiply.A
- [6] Lerp output 0 -> Multiply.B
- [7] 1 output 0 -> Lerp.A
- [3] Input Tint (Vector3) output 0 -> Lerp.B
- [4] Input TintMask (Scalar) output 0 -> Lerp.Alpha
- [8] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [2] Multiply output 0 -> SetMaterialAttributes.Inputs[1]
- [5] Multiply output 0 -> SetMaterialAttributes.Inputs[2]

## Output Trace Roots

- `Blended Material` is driven by [9] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
