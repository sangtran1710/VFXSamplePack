# DetailTexturing

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/DetailTexturing.DetailTexturing`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Diffuse` [FunctionInput_Vector3]
- `Normal` [FunctionInput_Vector3]
- `Scale` [FunctionInput_Scalar]
- `NormalIntensity` [FunctionInput_Scalar]
- `DiffuseIntensity` [FunctionInput_Scalar]
- `DetailDiffuse` [FunctionInput_Texture2D] - Should have highpass run on it and have sRGB unchecked
- `DetailNormal` [FunctionInput_Texture2D]

## Outputs

- `Diffuse` - Multiply against your diffuse
- `Normal` - Add to  your normal

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Diffuse | Diffuse |
| 1 | expression | `MaterialExpressionConstant` | 1 |  |
| 2 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 3 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 9 | expression | `MaterialExpressionAppendVector` | Append |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionConstant` | 0 |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse (Vector3) | Diffuse |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Scale (Scalar) | Scale |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input NormalIntensity (Scalar) | NormalIntensity |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input DiffuseIntensity (Scalar) | DiffuseIntensity |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input DetailDiffuse (Texture2D) | DetailDiffuse |
| 21 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 22 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 23 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input DetailNormal (Texture2D) | DetailNormal |
| 25 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output Diffuse

- `A` <= [25] Clamp output 0

### [2] Lerp

- `A` <= [1] 1 output 0
- `B` <= [4] Add output 0
- `Alpha` <= [17] Input DiffuseIntensity (Scalar) output 0

### [4] Add

- `A` <= [3] 0.5 output 0
- `B` <= [22] Texture Sample output 0

### [5] Texture Sample

- `Coordinates` <= [7] Multiply output 0
- `TextureObject` <= [24] Input DetailNormal (Texture2D) output 0

### [6] Multiply

- `A` <= [5] Texture Sample output 0
- `B` <= [10] Append output 0

### [7] Multiply

- `A` <= [8] TexCoord[0] output 0
- `B` <= [15] Input Scale (Scalar) output 0

### [9] Append

- `A` <= [16] Input NormalIntensity (Scalar) output 0
- `B` <= [16] Input NormalIntensity (Scalar) output 0

### [10] Append

- `A` <= [9] Append output 0
- `B` <= [11] 0 output 0

### [12] Output Normal

- `A` <= [19] Add output 0

### [18] Multiply

- `A` <= [13] Input Diffuse (Vector3) output 0
- `B` <= [2] Lerp output 0

### [19] Add

- `A` <= [6] Multiply output 0
- `B` <= [14] Input Normal (Vector3) output 0

### [20] Input DetailDiffuse (Texture2D)

- `Preview` <= [21] Texture Object output 0

### [22] Texture Sample

- `Coordinates` <= [7] Multiply output 0
- `TextureObject` <= [20] Input DetailDiffuse (Texture2D) output 0

### [24] Input DetailNormal (Texture2D)

- `Preview` <= [23] Texture Object output 0

### [25] Clamp

- `Input` <= [18] Multiply output 0

## Connection List

- [25] Clamp output 0 -> Output Diffuse.A
- [1] 1 output 0 -> Lerp.A
- [4] Add output 0 -> Lerp.B
- [17] Input DiffuseIntensity (Scalar) output 0 -> Lerp.Alpha
- [3] 0.5 output 0 -> Add.A
- [22] Texture Sample output 0 -> Add.B
- [7] Multiply output 0 -> Texture Sample.Coordinates
- [24] Input DetailNormal (Texture2D) output 0 -> Texture Sample.TextureObject
- [5] Texture Sample output 0 -> Multiply.A
- [10] Append output 0 -> Multiply.B
- [8] TexCoord[0] output 0 -> Multiply.A
- [15] Input Scale (Scalar) output 0 -> Multiply.B
- [16] Input NormalIntensity (Scalar) output 0 -> Append.A
- [16] Input NormalIntensity (Scalar) output 0 -> Append.B
- [9] Append output 0 -> Append.A
- [11] 0 output 0 -> Append.B
- [19] Add output 0 -> Output Normal.A
- [13] Input Diffuse (Vector3) output 0 -> Multiply.A
- [2] Lerp output 0 -> Multiply.B
- [6] Multiply output 0 -> Add.A
- [14] Input Normal (Vector3) output 0 -> Add.B
- [21] Texture Object output 0 -> Input DetailDiffuse (Texture2D).Preview
- [7] Multiply output 0 -> Texture Sample.Coordinates
- [20] Input DetailDiffuse (Texture2D) output 0 -> Texture Sample.TextureObject
- [23] Texture Object output 0 -> Input DetailNormal (Texture2D).Preview
- [18] Multiply output 0 -> Clamp.Input

## Output Trace Roots

- `Diffuse` is driven by [25] Clamp
- `Normal` is driven by [19] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


