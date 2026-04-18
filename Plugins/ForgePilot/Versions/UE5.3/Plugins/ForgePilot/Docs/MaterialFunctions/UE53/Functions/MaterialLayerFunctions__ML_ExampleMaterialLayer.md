# ML_ExampleMaterialLayer

- Path: `/Engine/Functions/MaterialLayerFunctions/ML_ExampleMaterialLayer.ML_ExampleMaterialLayer`
- Category: MaterialLayerFunctions
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `Tint` [FunctionInput_Vector3] - Tint the base Color
- `Tiling` [FunctionInput_Scalar]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Tint (Vector3) | Tint |
| 4 | expression | `MaterialExpressionConstant` | 0 |  |
| 5 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 6 | expression | `MaterialExpressionConstant` | 0 |  |
| 7 | expression | `MaterialExpressionConstant` | 1 |  |
| 8 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Tiling (Scalar) | Tiling |
| 12 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [12] SetMaterialAttributes output 0

### [1] Texture Sample

- `Coordinates` <= [10] Multiply output 0

### [2] Multiply

- `A` <= [3] Input Tint (Vector3) output 0
- `B` <= [1] Texture Sample output 0

### [5] Lerp

- `A` <= [6] 0 output 0
- `B` <= [7] 1 output 0
- `Alpha` <= [1] Texture Sample output 4

### [8] Texture Sample

- `Coordinates` <= [10] Multiply output 0

### [10] Multiply

- `A` <= [9] TexCoord[0] output 0
- `B` <= [11] Input Tiling (Scalar) output 0

### [12] SetMaterialAttributes

- `Inputs[1]` <= [2] Multiply output 0
- `Inputs[2]` <= [4] 0 output 0
- `Inputs[3]` <= [5] Lerp output 0
- `Inputs[4]` <= [8] Texture Sample output 0

## Connection List

- [12] SetMaterialAttributes output 0 -> Output Result.A
- [10] Multiply output 0 -> Texture Sample.Coordinates
- [3] Input Tint (Vector3) output 0 -> Multiply.A
- [1] Texture Sample output 0 -> Multiply.B
- [6] 0 output 0 -> Lerp.A
- [7] 1 output 0 -> Lerp.B
- [1] Texture Sample output 4 -> Lerp.Alpha
- [10] Multiply output 0 -> Texture Sample.Coordinates
- [9] TexCoord[0] output 0 -> Multiply.A
- [11] Input Tiling (Scalar) output 0 -> Multiply.B
- [2] Multiply output 0 -> SetMaterialAttributes.Inputs[1]
- [4] 0 output 0 -> SetMaterialAttributes.Inputs[2]
- [5] Lerp output 0 -> SetMaterialAttributes.Inputs[3]
- [8] Texture Sample output 0 -> SetMaterialAttributes.Inputs[4]

## Output Trace Roots

- `Result` is driven by [12] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
