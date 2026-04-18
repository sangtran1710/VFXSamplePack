# MatLayerBlend_TenLayerBlend

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_TenLayerBlend.MatLayerBlend_TenLayerBlend`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Fixed function blending of 10 layers.  Only uses the layers hooked up.  Compiler should optimize out unused layers.

## Inputs

- `Background` [FunctionInput_MaterialAttributes]
- `Layer1` [FunctionInput_MaterialAttributes]
- `Layer2` [FunctionInput_MaterialAttributes]
- `Layer4` [FunctionInput_MaterialAttributes]
- `Layer3` [FunctionInput_MaterialAttributes]
- `Layer5` [FunctionInput_MaterialAttributes]
- `Layer6` [FunctionInput_MaterialAttributes]
- `Layer7` [FunctionInput_MaterialAttributes]
- `Layer10` [FunctionInput_MaterialAttributes]
- `Layer9` [FunctionInput_MaterialAttributes]
- `Layer8` [FunctionInput_MaterialAttributes]
- `Layer1_Mask` [FunctionInput_Scalar]
- `Layer2_Mask` [FunctionInput_Scalar]
- `Layer3_Mask` [FunctionInput_Scalar]
- `Layer4_Mask` [FunctionInput_Scalar]
- `Layer8_Mask
` [FunctionInput_Scalar]
- `Layer7_Mask` [FunctionInput_Scalar]
- `Layer6_Mask
` [FunctionInput_Scalar]
- `Layer5_Mask` [FunctionInput_Scalar]
- `Layer9_Mask` [FunctionInput_Scalar]
- `Layer10_Mask` [FunctionInput_Scalar]
- `<BakedNormal>` [FunctionInput_Vector3]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Background (MaterialAttributes) | Background |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Layer1 (MaterialAttributes) | Layer1 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Layer2 (MaterialAttributes) | Layer2 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Layer4 (MaterialAttributes) | Layer4 |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Layer3 (MaterialAttributes) | Layer3 |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Layer5 (MaterialAttributes) | Layer5 |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Layer6 (MaterialAttributes) | Layer6 |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Layer7 (MaterialAttributes) | Layer7 |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Layer10 (MaterialAttributes) | Layer10 |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Layer9 (MaterialAttributes) | Layer9 |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Layer8 (MaterialAttributes) | Layer8 |
| 12 | expression | `MaterialExpressionMakeMaterialAttributes` | MakeMaterialAttributes |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Layer1_Mask (Scalar) | Layer1_Mask |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Layer2_Mask (Scalar) | Layer2_Mask |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Layer3_Mask (Scalar) | Layer3_Mask |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Layer4_Mask (Scalar) | Layer4_Mask |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Layer8_Mask
 (Scalar) | Layer8_Mask
 |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Layer7_Mask (Scalar) | Layer7_Mask |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Layer6_Mask
 (Scalar) | Layer6_Mask
 |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Layer5_Mask (Scalar) | Layer5_Mask |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Layer9_Mask (Scalar) | Layer9_Mask |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Layer10_Mask (Scalar) | Layer10_Mask |
| 23 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 25 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 26 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 27 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 28 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 29 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 30 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 31 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_Standard |  |
| 33 | function_input | `MaterialExpressionFunctionInput` | Input <BakedNormal> (Vector3) | <BakedNormal> |
| 34 | function_call | `MaterialExpressionMaterialFunctionCall` | MatLayerBlend_BakedNormal |  |

## Exact Input Wiring

### [0] Output Blended Material

- `A` <= [34] MatLayerBlend_BakedNormal output 0

### [1] Input Background (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [2] Input Layer1 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [3] Input Layer2 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [4] Input Layer4 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [5] Input Layer3 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [6] Input Layer5 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [7] Input Layer6 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [8] Input Layer7 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [9] Input Layer10 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [10] Input Layer9 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

### [11] Input Layer8 (MaterialAttributes)

- `Preview` <= [12] MakeMaterialAttributes output 0

## Connection List

- [34] MatLayerBlend_BakedNormal output 0 -> Output Blended Material.A
- [12] MakeMaterialAttributes output 0 -> Input Background (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer1 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer2 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer4 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer3 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer5 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer6 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer7 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer10 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer9 (MaterialAttributes).Preview
- [12] MakeMaterialAttributes output 0 -> Input Layer8 (MaterialAttributes).Preview

## Output Trace Roots

- `Blended Material` is driven by [34] MatLayerBlend_BakedNormal

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


