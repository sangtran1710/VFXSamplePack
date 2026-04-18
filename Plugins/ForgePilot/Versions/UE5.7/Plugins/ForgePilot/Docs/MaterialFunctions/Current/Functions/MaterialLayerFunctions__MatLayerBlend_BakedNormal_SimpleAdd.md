# MatLayerBlend_BakedNormal_SimpleAdd

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BakedNormal_SimpleAdd.MatLayerBlend_BakedNormal_SimpleAdd`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends a Processed Normal Map with layered normals

## Inputs

- `Material` [FunctionInput_MaterialAttributes]
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionConstant3Vector` | 1,1,0.5 |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionConstant3Vector` | 1,1,0.675 |  |
| 8 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 9 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [9] SetMaterialAttributes output 0

### [3] Multiply

- `A` <= [8] GetMaterialAttributes output 1
- `B` <= [4] 1,1,0.5 output 0

### [5] Add

- `A` <= [3] Multiply output 0
- `B` <= [2] Input Normal (Vector3) output 0

### [6] Multiply

- `A` <= [5] Add output 0
- `B` <= [7] 1,1,0.675 output 0

### [9] SetMaterialAttributes

- `Inputs[0]` <= [8] GetMaterialAttributes output 0
- `Inputs[1]` <= [6] Multiply output 0

## Connection List

- [9] SetMaterialAttributes output 0 -> Output Blended Material.A
- [8] GetMaterialAttributes output 1 -> Multiply.A
- [4] 1,1,0.5 output 0 -> Multiply.B
- [3] Multiply output 0 -> Add.A
- [2] Input Normal (Vector3) output 0 -> Add.B
- [5] Add output 0 -> Multiply.A
- [7] 1,1,0.675 output 0 -> Multiply.B
- [8] GetMaterialAttributes output 0 -> SetMaterialAttributes.Inputs[0]
- [6] Multiply output 0 -> SetMaterialAttributes.Inputs[1]

## Output Trace Roots

- `Blended Material` is driven by [9] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


