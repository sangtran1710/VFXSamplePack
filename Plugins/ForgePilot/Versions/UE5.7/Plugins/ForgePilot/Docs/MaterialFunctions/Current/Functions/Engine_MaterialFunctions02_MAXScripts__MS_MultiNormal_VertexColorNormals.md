# MS_MultiNormal_VertexColorNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_MultiNormal_VertexColorNormals.MS_MultiNormal_VertexColorNormals`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This material function was designed to be used in conjunction with the MAXScript located here "\UE4\Engine\Extras\3dsMaxScripts\MultiNormal.ms". This particular function outputs vertex normals stored in the Vertex Colors. 

## Inputs

- None

## Outputs

- `Vertex Color Based Tangent Normals`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Vertex Color Based Tangent Normals | Vertex Color Based Tangent Normals |
| 1 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 2 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 3 | expression | `MaterialExpressionTransform` | Local Space to Tangent Space TransformVector |  |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Vertex Color Based Tangent Normals

- `A` <= [4] Normalize output 0

### [2] ConstantBiasScale

- `Input` <= [1] Vertex Color output 0

### [3] Local Space to Tangent Space TransformVector

- `Input` <= [2] ConstantBiasScale output 0

### [4] Normalize

- `VectorInput` <= [3] Local Space to Tangent Space TransformVector output 0

## Connection List

- [4] Normalize output 0 -> Output Vertex Color Based Tangent Normals.A
- [1] Vertex Color output 0 -> ConstantBiasScale.Input
- [2] ConstantBiasScale output 0 -> Local Space to Tangent Space TransformVector.Input
- [3] Local Space to Tangent Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Vertex Color Based Tangent Normals` is driven by [4] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


