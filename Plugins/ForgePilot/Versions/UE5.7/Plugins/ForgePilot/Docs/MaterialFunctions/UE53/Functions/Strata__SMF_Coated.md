# SMF_Coated

- Path: `/Engine/Functions/Strata/SMF_Coated.SMF_Coated`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Inputs

- `Base Color` [FunctionInput_Vector3]
- `Metallic` [FunctionInput_Scalar]
- `Specular` [FunctionInput_Scalar]
- `Roughness` [FunctionInput_Scalar]
- `Normal` [FunctionInput_Vector3]
- `Coat Color` [FunctionInput_Vector3] - Transmittance color for a direction perpendicular to the surface.
- `Coat Roughness` [FunctionInput_Scalar]
- `Coat Specular` [FunctionInput_Scalar]
- `Coat Normal` [FunctionInput_Vector3]
- `Coat Emissive Color` [FunctionInput_Vector3]
- `Emissive Color` [FunctionInput_Vector3]
- `Thickness factor` [FunctionInput_Scalar]
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
| 9 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Single |  |
| 10 | expression | `MaterialExpressionStrataVerticalLayering` | Substrate Vertical Layer |  |
| 11 | expression | `MaterialExpressionStrataTransmittanceToMFP` | Substrate Transmittance-To-MeanFreePath |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Coat Color (Vector3) | Coat Color |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Coat Roughness (Scalar) | Coat Roughness |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Coat Specular (Scalar) | Coat Specular |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Coat Normal (Vector3) | Coat Normal |
| 16 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 17 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Coat Emissive Color (Vector3) | Coat Emissive Color |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Emissive Color (Vector3) | Emissive Color |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | function_input | `MaterialExpressionFunctionInput` | Input Thickness factor (Scalar) | Thickness factor |
| 22 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 23 | expression | `MaterialExpressionStrataMetalnessToDiffuseAlbedoF0` | Substrate Metalness-To-DiffuseAlbedo-F0 |  |
| 24 | expression | `MaterialExpressionStrataMetalnessToDiffuseAlbedoF0` | Substrate Metalness-To-DiffuseAlbedo-F0 |  |
| 25 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 27 | expression | `MaterialExpressionStrataWeight` | Substrate Coverage Weight |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [27] Substrate Coverage Weight output 0

### [1] Substrate Slab BSDF - Simple

- `DiffuseAlbedo` <= [23] Substrate Metalness-To-DiffuseAlbedo-F0 output 0
- `F0` <= [23] Substrate Metalness-To-DiffuseAlbedo-F0 output 1
- `Roughness` <= [5] Input Roughness (Scalar) output 0
- `Normal` <= [6] Input Normal (Vector3) output 0
- `EmissiveColor` <= [19] Input Emissive Color (Vector3) output 0

### [6] Input Normal (Vector3)

- `Preview` <= [8] World Space to Tangent Space TransformVector output 0

### [8] World Space to Tangent Space TransformVector

- `Input` <= [7] VertexNormalWS output 0

### [9] Substrate Slab BSDF - Single

- `DiffuseAlbedo` <= [24] Substrate Metalness-To-DiffuseAlbedo-F0 output 0
- `F0` <= [24] Substrate Metalness-To-DiffuseAlbedo-F0 output 1
- `Roughness` <= [13] Input Coat Roughness (Scalar) output 0
- `Normal` <= [15] Input Coat Normal (Vector3) output 0
- `SSSMFP` <= [11] Substrate Transmittance-To-MeanFreePath output 0
- `EmissiveColor` <= [18] Input Coat Emissive Color (Vector3) output 0

### [10] Substrate Vertical Layer

- `Top` <= [9] Substrate Slab BSDF - Single output 0
- `Base` <= [1] Substrate Slab BSDF - Simple output 0
- `Thickness` <= [20] Multiply output 0

### [11] Substrate Transmittance-To-MeanFreePath

- `TransmittanceColor` <= [12] Input Coat Color (Vector3) output 0

### [15] Input Coat Normal (Vector3)

- `Preview` <= [17] World Space to Tangent Space TransformVector output 0

### [17] World Space to Tangent Space TransformVector

- `Input` <= [16] VertexNormalWS output 0

### [20] Multiply

- `A` <= [11] Substrate Transmittance-To-MeanFreePath output 1
- `B` <= [22] Saturate output 0

### [22] Saturate

- `Input` <= [21] Input Thickness factor (Scalar) output 0

### [23] Substrate Metalness-To-DiffuseAlbedo-F0

- `BaseColor` <= [2] Input Base Color (Vector3) output 0
- `Metallic` <= [3] Input Metallic (Scalar) output 0
- `Specular` <= [4] Input Specular (Scalar) output 0

### [24] Substrate Metalness-To-DiffuseAlbedo-F0

- `BaseColor` <= [25] 0,0,0 output 0
- `Specular` <= [14] Input Coat Specular (Scalar) output 0

### [27] Substrate Coverage Weight

- `A` <= [10] Substrate Vertical Layer output 0
- `Weight` <= [26] Input Opacity (Scalar) output 0

## Connection List

- [27] Substrate Coverage Weight output 0 -> Output Result.A
- [23] Substrate Metalness-To-DiffuseAlbedo-F0 output 0 -> Substrate Slab BSDF - Simple.DiffuseAlbedo
- [23] Substrate Metalness-To-DiffuseAlbedo-F0 output 1 -> Substrate Slab BSDF - Simple.F0
- [5] Input Roughness (Scalar) output 0 -> Substrate Slab BSDF - Simple.Roughness
- [6] Input Normal (Vector3) output 0 -> Substrate Slab BSDF - Simple.Normal
- [19] Input Emissive Color (Vector3) output 0 -> Substrate Slab BSDF - Simple.EmissiveColor
- [8] World Space to Tangent Space TransformVector output 0 -> Input Normal (Vector3).Preview
- [7] VertexNormalWS output 0 -> World Space to Tangent Space TransformVector.Input
- [24] Substrate Metalness-To-DiffuseAlbedo-F0 output 0 -> Substrate Slab BSDF - Single.DiffuseAlbedo
- [24] Substrate Metalness-To-DiffuseAlbedo-F0 output 1 -> Substrate Slab BSDF - Single.F0
- [13] Input Coat Roughness (Scalar) output 0 -> Substrate Slab BSDF - Single.Roughness
- [15] Input Coat Normal (Vector3) output 0 -> Substrate Slab BSDF - Single.Normal
- [11] Substrate Transmittance-To-MeanFreePath output 0 -> Substrate Slab BSDF - Single.SSSMFP
- [18] Input Coat Emissive Color (Vector3) output 0 -> Substrate Slab BSDF - Single.EmissiveColor
- [9] Substrate Slab BSDF - Single output 0 -> Substrate Vertical Layer.Top
- [1] Substrate Slab BSDF - Simple output 0 -> Substrate Vertical Layer.Base
- [20] Multiply output 0 -> Substrate Vertical Layer.Thickness
- [12] Input Coat Color (Vector3) output 0 -> Substrate Transmittance-To-MeanFreePath.TransmittanceColor
- [17] World Space to Tangent Space TransformVector output 0 -> Input Coat Normal (Vector3).Preview
- [16] VertexNormalWS output 0 -> World Space to Tangent Space TransformVector.Input
- [11] Substrate Transmittance-To-MeanFreePath output 1 -> Multiply.A
- [22] Saturate output 0 -> Multiply.B
- [21] Input Thickness factor (Scalar) output 0 -> Saturate.Input
- [2] Input Base Color (Vector3) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.BaseColor
- [3] Input Metallic (Scalar) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Metallic
- [4] Input Specular (Scalar) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Specular
- [25] 0,0,0 output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.BaseColor
- [14] Input Coat Specular (Scalar) output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Specular
- [10] Substrate Vertical Layer output 0 -> Substrate Coverage Weight.A
- [26] Input Opacity (Scalar) output 0 -> Substrate Coverage Weight.Weight

## Output Trace Roots

- `Result` is driven by [27] Substrate Coverage Weight

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
