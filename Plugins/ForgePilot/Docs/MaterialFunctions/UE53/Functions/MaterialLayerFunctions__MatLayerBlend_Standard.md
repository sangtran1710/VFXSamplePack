# MatLayerBlend_Standard

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Standard.MatLayerBlend_Standard`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends all attributes of 2 Materials

## Inputs

- `Alpha` [FunctionInput_Scalar]
- `Top Material` [FunctionInput_MaterialAttributes]
- `Base Material` [FunctionInput_MaterialAttributes]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Alpha (Scalar) | Alpha |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Top Material (MaterialAttributes) | Top Material |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Base Material (MaterialAttributes) | Base Material |
| 4 | expression | `MaterialExpressionBlendMaterialAttributes` | BlendMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [4] BlendMaterialAttributes output 0

### [4] BlendMaterialAttributes

- `Alpha` <= [1] Input Alpha (Scalar) output 0

## Connection List

- [4] BlendMaterialAttributes output 0 -> Output Blended Material.A
- [1] Input Alpha (Scalar) output 0 -> BlendMaterialAttributes.Alpha

## Output Trace Roots

- `Blended Material` is driven by [4] BlendMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
