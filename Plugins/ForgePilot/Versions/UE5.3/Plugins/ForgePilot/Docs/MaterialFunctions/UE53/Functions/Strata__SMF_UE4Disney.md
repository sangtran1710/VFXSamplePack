# SMF_UE4Disney

- Path: `/Engine/Functions/Strata/SMF_UE4Disney.SMF_UE4Disney`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `Base Color` [FunctionInput_Vector3] - Defines the overall color of the Material.
- `Metallic` [FunctionInput_Scalar] - Controls how "metal-like" your surface looks like. 0 means dielectric, 1 means conductor.
- `Specular` [FunctionInput_Scalar] - Used to scale the current amount of specularity on non-metallic surfaces and is a value between 0 and 1.
- `Roughness` [FunctionInput_Scalar] - Controls how rough the Material is. Roughness of 0 (smooth) is a mirror reflection and 1 (rough) is completely matte or diffuse.
- `Normal` [FunctionInput_Vector3] - The surface normal. (default value assumes normal is specified in tangent space)
- `Emissive Color` [FunctionInput_Vector3] - Emissive Color.
- `Opacity` [FunctionInput_Scalar] - Opacity can be used to alpha fade out a surface when using any of the translucent blending modes.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Simple |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Base Color (Vector3) | Base Color |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Metallic (Scalar) | Metallic |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Specular (Scalar) | Specular |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Roughness (Scalar) | Roughness |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 7 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 8 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Emissive Color (Vector3) | Emissive Color |
| 10 | expression | `MaterialExpressionStrataMetalnessToDiffuseAlbedoF0` | Substrate Metalness-To-DiffuseAlbedo-F0 |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 12 | expression | `MaterialExpressionStrataWeight` | Substrate Coverage Weight |  |
| 13 | expression | `MaterialExpressionNormalize` | Normalize |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [12] Substrate Coverage Weight output 0

### [1] Substrate Slab BSDF - Simple

- `DiffuseAlbedo` <= [10] Substrate Metalness-To-DiffuseAlbedo-F0 output 0
- `F0` <= [10] Substrate Metalness-To-DiffuseAlbedo-F0 output 1
- `Roughness` <= [5] Input Roughness (Scalar) output 0
- `Normal` <= [6] Input Normal (Vector3) output 0
- `EmissiveColor` <= [9] Input Emissive Color (Vector3) output 0

### [6] Input Normal (Vector3)

- `Preview` <= [13] Normalize output 0

### [8] World Space to Tangent Space TransformVector

- `Input` <= [7] VertexNormalWS output 0

### [10] Substrate Metalness-To-DiffuseAlbedo-F0

- `BaseColor` <= [2] Input Base Color (Vector3) output 0
- `Metallic` <= [3] Input Metallic (Scalar) output 0
- `Specular` <= [4] Input Specular (Scalar) output 0

### [12] Substrate Coverage Weight

- `A` <= [1] Substrate Slab BSDF - Simple output 0
- `Weight` <= [11] Input Opacity (Scalar) output 0

### [13] Normalize

- `VectorInput` <= [8] World Space to Tangent Space TransformVector output 0

## Connection List

- [12] Substrate Coverage Weight output 0 -> Output Result.A
- [10] Substrate Metalness-To-DiffuseAlbedo-F0 output 0 -> Substrate Slab BSDF - Simple.DiffuseAlbedo
- [10] Substrate Metalness-To-DiffuseAlbedo-F0 output 1 -> Substrate Slab BSDF - Simple.F0
- [5] Input Roughness (Scalar) output 0 -> Substrate Slab BSDF - Simple.Roughness
- [6] Input Normal (Vector3) output 0 -> Substrate Slab BSDF - Simple.Normal
- [9] Input Emissive Color (Vector3) output 0 -> Substrate Slab BSDF - Simple.EmissiveColor
- [13] Normalize output 0 -> Input Normal (Vector3).Preview
- [7] VertexNormalWS output 0 -> World Space to Tangent Space TransformVector.Input
- [2] Input Base Color (Vector3) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.BaseColor
- [3] Input Metallic (Scalar) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Metallic
- [4] Input Specular (Scalar) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Specular
- [1] Substrate Slab BSDF - Simple output 0 -> Substrate Coverage Weight.A
- [11] Input Opacity (Scalar) output 0 -> Substrate Coverage Weight.Weight
- [8] World Space to Tangent Space TransformVector output 0 -> Normalize.VectorInput

## Output Trace Roots

- `Result` is driven by [12] Substrate Coverage Weight

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
