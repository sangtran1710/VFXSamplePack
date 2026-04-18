# MS_MultiNormal_UVnormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_MultiNormal_UVnormals.MS_MultiNormal_UVnormals`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This material function was designed to be used in conjunction with the MAXScript located here "\UE4\Engine\Extras\3dsMaxScripts\MultiNormal.ms". This particular function outputs vertex normals stored in the UVs. 

## Inputs

- None

## Outputs

- `Tangent Space UVs`
- `Custom UV 4`
- `Custom UV 5`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Tangent Space UVs | Tangent Space UVs |
| 1 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 3 | expression | `MaterialExpressionIf` | If |  |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionAbs` | Abs |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | expression | `MaterialExpressionDeriveNormalZ` | DeriveNormalZ |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionTransform` | Local Space to Tangent Space TransformVector |  |
| 10 | expression | `MaterialExpressionConstant` | 1 |  |
| 11 | expression | `MaterialExpressionConstant` | -1 |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 14 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 16 | expression | `MaterialExpressionAppendVector` | Append |  |
| 17 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[4] |  |
| 18 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[5] |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 4 | Custom UV 4 |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 5 | Custom UV 5 |
| 22 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Tangent Space UVs

- `A` <= [22] Normalize output 0

### [3] If

- `A` <= [2] BreakOutFloat2Components output 0
- `B` <= [4] 0 output 0
- `AGreaterThanB` <= [10] 1 output 0
- `AEqualsB` <= [11] -1 output 0
- `ALessThanB` <= [11] -1 output 0

### [5] Abs

- `Input` <= [2] BreakOutFloat2Components output 0

### [6] Append

- `A` <= [5] Abs output 0
- `B` <= [2] BreakOutFloat2Components output 1

### [7] DeriveNormalZ

- `InXY` <= [14] ConstantBiasScale output 0

### [8] Multiply

- `A` <= [3] If output 0
- `B` <= [12] BreakOutFloat3Components output 2

### [9] Local Space to Tangent Space TransformVector

- `Input` <= [16] Append output 0

### [14] ConstantBiasScale

- `Input` <= [6] Append output 0

### [16] Append

- `A` <= [17] TexCoord[4] output 0
- `B` <= [19] BreakOutFloat2Components output 0

### [20] Output Custom UV 4

- `A` <= [13] MakeFloat3 output 0

### [21] Output Custom UV 5

- `A` <= [15] BreakOutFloat3Components output 2

### [22] Normalize

- `VectorInput` <= [9] Local Space to Tangent Space TransformVector output 0

## Connection List

- [22] Normalize output 0 -> Output Tangent Space UVs.A
- [2] BreakOutFloat2Components output 0 -> If.A
- [4] 0 output 0 -> If.B
- [10] 1 output 0 -> If.AGreaterThanB
- [11] -1 output 0 -> If.AEqualsB
- [11] -1 output 0 -> If.ALessThanB
- [2] BreakOutFloat2Components output 0 -> Abs.Input
- [5] Abs output 0 -> Append.A
- [2] BreakOutFloat2Components output 1 -> Append.B
- [14] ConstantBiasScale output 0 -> DeriveNormalZ.InXY
- [3] If output 0 -> Multiply.A
- [12] BreakOutFloat3Components output 2 -> Multiply.B
- [16] Append output 0 -> Local Space to Tangent Space TransformVector.Input
- [6] Append output 0 -> ConstantBiasScale.Input
- [17] TexCoord[4] output 0 -> Append.A
- [19] BreakOutFloat2Components output 0 -> Append.B
- [13] MakeFloat3 output 0 -> Output Custom UV 4.A
- [15] BreakOutFloat3Components output 2 -> Output Custom UV 5.A
- [9] Local Space to Tangent Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Tangent Space UVs` is driven by [22] Normalize
- `Custom UV 4` is driven by [13] MakeFloat3
- `Custom UV 5` is driven by [15] BreakOutFloat3Components

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


