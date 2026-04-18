# Substrate-StandardSurface-Translucent

- Path: `/Engine/Functions/Strata/Substrate-StandardSurface-Translucent.Substrate-StandardSurface-Translucent`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Translucent Standard Surface

## Inputs

- `BaseColor` [FunctionInput_Vector3]
- `TransmissionColor` [FunctionInput_Vector3]
- `SpecularColor` [FunctionInput_Vector3]
- `SpecularAnisotropy` [FunctionInput_Scalar]
- `AnisotropyRotation` [FunctionInput_Scalar] - Normalized Rotation
- `EmissiveColor` [FunctionInput_Vector3]
- `---- Coat ----` [FunctionInput_StaticBool]
- `TransmissionDepth` [FunctionInput_Scalar]
- `BaseWeight` [FunctionInput_Scalar]
- `TransmissionWeight` [FunctionInput_Scalar]
- `IOR` [FunctionInput_Scalar]
- `Metalness` [FunctionInput_Scalar]
- `TransmissionScatter` [FunctionInput_Vector3]
- `SpecularWeight` [FunctionInput_Scalar]
- `SpecularRoughness` [FunctionInput_Scalar]
- `Normal` [FunctionInput_Vector3]
- `ThinFilmThickness` [FunctionInput_Scalar]
- `SheenColor` [FunctionInput_Vector3]
- `SheenWeight` [FunctionInput_Scalar]
- `---- Sheen ----` [FunctionInput_StaticBool]
- `---- Emission ----` [FunctionInput_StaticBool]
- `---- Transmission ----` [FunctionInput_StaticBool]
- `---- Specular ----` [FunctionInput_StaticBool]
- `---- Thin Film ----` [FunctionInput_StaticBool]
- `---- Geometry ----` [FunctionInput_StaticBool]
- `CoatNormal` [FunctionInput_Vector3]
- `CoatWeight` [FunctionInput_Scalar]
- `CoatIOR` [FunctionInput_Scalar]
- `CoatRoughness` [FunctionInput_Scalar]
- `CoatAnisotropy` [FunctionInput_Scalar]
- `CoatRotation` [FunctionInput_Scalar]
- `CoatColor` [FunctionInput_Vector3]
- `AnisotropyTangent` [FunctionInput_Vector3]
- `---- Base ----` [FunctionInput_StaticBool]
- `ThinFilmIOR` [FunctionInput_Scalar]
- `DiffuseRoughness` [FunctionInput_Scalar]
- `SheenRoughness` [FunctionInput_Scalar]
- `EmissionWeight` [FunctionInput_Scalar]
- `Opacity` [FunctionInput_Scalar]
- `ExtraRoughness` [FunctionInput_Scalar]

## Outputs

- `Substrate_StandardTranslucent`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Substrate_StandardTranslucent | Substrate_StandardTranslucent |
| 1 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Complex |  |
| 2 | expression | `MaterialExpressionStrataVerticalLayering` | Substrate Vertical Layer |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input BaseColor (Vector3) | BaseColor |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input TransmissionColor (Vector3) | TransmissionColor |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input SpecularColor (Vector3) | SpecularColor |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input SpecularAnisotropy (Scalar) | SpecularAnisotropy |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input AnisotropyRotation (Scalar) | AnisotropyRotation |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input EmissiveColor (Vector3) | EmissiveColor |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input ---- Coat ---- (StaticBool) | ---- Coat ---- |
| 10 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input TransmissionDepth (Scalar) | TransmissionDepth |
| 12 | expression | `MaterialExpressionConstant` | 1 |  |
| 13 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input BaseWeight (Scalar) | BaseWeight |
| 15 | expression | `MaterialExpressionConstant` | 1 |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionConstant3Vector` | 0.18,0.18,0.18 |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input TransmissionWeight (Scalar) | TransmissionWeight |
| 19 | expression | `MaterialExpressionConstant` | 0 |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input IOR (Scalar) | IOR |
| 21 | expression | `MaterialExpressionConstant` | 1.5 |  |
| 22 | expression | `MaterialExpressionInverseLinearInterpolate` | InvLerp |  |
| 23 | expression | `MaterialExpressionConstant` | 0 |  |
| 24 | expression | `MaterialExpressionConstant` | 0.08 |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Metalness (Scalar) | Metalness |
| 26 | expression | `MaterialExpressionConstant` | 0 |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input TransmissionScatter (Vector3) | TransmissionScatter |
| 28 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 29 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Complex |  |
| 30 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 31 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 32 | function_input | `MaterialExpressionFunctionInput` | Input SpecularWeight (Scalar) | SpecularWeight |
| 33 | expression | `MaterialExpressionConstant` | 1 |  |
| 34 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input SpecularRoughness (Scalar) | SpecularRoughness |
| 36 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 37 | expression | `MaterialExpressionConstant` | 0 |  |
| 38 | expression | `MaterialExpressionConstant` | 0 |  |
| 39 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 40 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input ThinFilmThickness (Scalar) | ThinFilmThickness |
| 42 | expression | `MaterialExpressionConstant` | 0 |  |
| 43 | function_input | `MaterialExpressionFunctionInput` | Input SheenColor (Vector3) | SheenColor |
| 44 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 45 | function_input | `MaterialExpressionFunctionInput` | Input SheenWeight (Scalar) | SheenWeight |
| 46 | expression | `MaterialExpressionConstant` | 0 |  |
| 47 | function_input | `MaterialExpressionFunctionInput` | Input ---- Sheen ---- (StaticBool) | ---- Sheen ---- |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input ---- Emission ---- (StaticBool) | ---- Emission ---- |
| 49 | function_input | `MaterialExpressionFunctionInput` | Input ---- Transmission ---- (StaticBool) | ---- Transmission ---- |
| 50 | function_input | `MaterialExpressionFunctionInput` | Input ---- Specular ---- (StaticBool) | ---- Specular ---- |
| 51 | function_input | `MaterialExpressionFunctionInput` | Input ---- Thin Film ---- (StaticBool) | ---- Thin Film ---- |
| 52 | function_input | `MaterialExpressionFunctionInput` | Input ---- Geometry ---- (StaticBool) | ---- Geometry ---- |
| 53 | function_input | `MaterialExpressionFunctionInput` | Input CoatNormal (Vector3) | CoatNormal |
| 54 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 55 | function_input | `MaterialExpressionFunctionInput` | Input CoatWeight (Scalar) | CoatWeight |
| 56 | expression | `MaterialExpressionConstant` | 0 |  |
| 57 | expression | `MaterialExpressionStrataWeight` | Substrate Coverage Weight |  |
| 58 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 59 | function_input | `MaterialExpressionFunctionInput` | Input CoatIOR (Scalar) | CoatIOR |
| 60 | expression | `MaterialExpressionConstant` | 1.5 |  |
| 61 | expression | `MaterialExpressionConstant` | 1 |  |
| 62 | function_input | `MaterialExpressionFunctionInput` | Input CoatRoughness (Scalar) | CoatRoughness |
| 63 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 64 | function_input | `MaterialExpressionFunctionInput` | Input CoatAnisotropy (Scalar) | CoatAnisotropy |
| 65 | function_input | `MaterialExpressionFunctionInput` | Input CoatRotation (Scalar) | CoatRotation |
| 66 | expression | `MaterialExpressionConstant` | 0 |  |
| 67 | expression | `MaterialExpressionConstant` | 0 |  |
| 68 | function_input | `MaterialExpressionFunctionInput` | Input CoatColor (Vector3) | CoatColor |
| 69 | expression | `MaterialExpressionConstant3Vector` | 0.999,0.999,0.999 |  |
| 70 | function_input | `MaterialExpressionFunctionInput` | Input AnisotropyTangent (Vector3) | AnisotropyTangent |
| 71 | expression | `MaterialExpressionVertexTangentWS` | VertexTangentWS |  |
| 72 | function_input | `MaterialExpressionFunctionInput` | Input ---- Base ---- (StaticBool) | ---- Base ---- |
| 73 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 74 | expression | `MaterialExpressionStrataThinFilm` | Substrate Thin-Film |  |
| 75 | expression | `MaterialExpressionNamedRerouteDeclaration` | AnisotropyTangent |  |
| 76 | expression | `MaterialExpressionNamedRerouteUsage` | AnisotropyTangent |  |
| 77 | expression | `MaterialExpressionNamedRerouteUsage` | AnisotropyTangent |  |
| 78 | expression | `MaterialExpressionNamedRerouteDeclaration` | CoatIOR |  |
| 79 | function_input | `MaterialExpressionFunctionInput` | Input ThinFilmIOR (Scalar) | ThinFilmIOR |
| 80 | expression | `MaterialExpressionConstant` | 1.5 |  |
| 81 | expression | `MaterialExpressionNamedRerouteUsage` | CoatIOR |  |
| 82 | expression | `MaterialExpressionNamedRerouteDeclaration` | Normal |  |
| 83 | expression | `MaterialExpressionNamedRerouteUsage` | Normal |  |
| 84 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 85 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Complex |  |
| 86 | expression | `MaterialExpressionNamedRerouteDeclaration` | WeightedBase |  |
| 87 | expression | `MaterialExpressionNamedRerouteDeclaration` | SlabIOR |  |
| 88 | expression | `MaterialExpressionNamedRerouteDeclaration` | Metalness |  |
| 89 | expression | `MaterialExpressionNamedRerouteDeclaration` | WeightedSpec |  |
| 90 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedSpec |  |
| 91 | expression | `MaterialExpressionNamedRerouteDeclaration` | SpecRoughness |  |
| 92 | expression | `MaterialExpressionNamedRerouteDeclaration` | SpecAnisotropy |  |
| 93 | expression | `MaterialExpressionNamedRerouteDeclaration` | SpecAnisoRot |  |
| 94 | expression | `MaterialExpressionNamedRerouteUsage` | SpecAnisotropy |  |
| 95 | expression | `MaterialExpressionNamedRerouteUsage` | SpecAnisoRot |  |
| 96 | expression | `MaterialExpressionNamedRerouteUsage` | SpecRoughness |  |
| 97 | expression | `MaterialExpressionNamedRerouteUsage` | SpecAnisotropy |  |
| 98 | expression | `MaterialExpressionNamedRerouteUsage` | Normal |  |
| 99 | expression | `MaterialExpressionNamedRerouteUsage` | SpecRoughness |  |
| 100 | expression | `MaterialExpressionNamedRerouteUsage` | SpecAnisoRot |  |
| 101 | expression | `MaterialExpressionNamedRerouteDeclaration` | EmissiveColor |  |
| 102 | expression | `MaterialExpressionNamedRerouteUsage` | EmissiveColor |  |
| 103 | expression | `MaterialExpressionNamedRerouteUsage` | EmissiveColor |  |
| 104 | expression | `MaterialExpressionNamedRerouteDeclaration` | BaseWeight |  |
| 105 | expression | `MaterialExpressionNamedRerouteDeclaration` | SheenWeight |  |
| 106 | expression | `MaterialExpressionNamedRerouteDeclaration` | SheenColor |  |
| 107 | expression | `MaterialExpressionNamedRerouteUsage` | SheenWeight |  |
| 108 | expression | `MaterialExpressionNamedRerouteUsage` | SheenColor |  |
| 109 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedSpec |  |
| 110 | expression | `MaterialExpressionNamedRerouteDeclaration` | SpecF0 |  |
| 111 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 112 | expression | `MaterialExpressionNamedRerouteUsage` | SpecF0 |  |
| 113 | expression | `MaterialExpressionNamedRerouteDeclaration` | MFP |  |
| 114 | expression | `MaterialExpressionConstant` | 1 |  |
| 115 | expression | `MaterialExpressionStrataWeight` | Substrate Coverage Weight |  |
| 116 | function_input | `MaterialExpressionFunctionInput` | Input DiffuseRoughness (Scalar) | DiffuseRoughness |
| 117 | expression | `MaterialExpressionConstant` | 0 |  |
| 118 | expression | `MaterialExpressionNamedRerouteDeclaration` | DiffuseRoughness |  |
| 119 | function_input | `MaterialExpressionFunctionInput` | Input SheenRoughness (Scalar) | SheenRoughness |
| 120 | expression | `MaterialExpressionNamedRerouteDeclaration` | SheenRoughness |  |
| 121 | expression | `MaterialExpressionConstant` | 0 |  |
| 122 | expression | `MaterialExpressionNamedRerouteUsage` | DiffuseRoughness |  |
| 123 | expression | `MaterialExpressionNamedRerouteUsage` | SheenRoughness |  |
| 124 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedBase |  |
| 125 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 126 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 127 | expression | `MaterialExpressionStrataHorizontalMixing` | Substrate Horizontal Blend (Parameter Blend) |  |
| 128 | expression | `MaterialExpressionConstant` | 1 |  |
| 129 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedBase |  |
| 130 | expression | `MaterialExpressionNamedRerouteUsage` | Metalness |  |
| 131 | expression | `MaterialExpressionNamedRerouteUsage` | Metalness |  |
| 132 | expression | `MaterialExpressionNamedRerouteUsage` | SlabIOR |  |
| 133 | expression | `MaterialExpressionLinearInterpolate` | Lerp(,0,) |  |
| 134 | function_input | `MaterialExpressionFunctionInput` | Input EmissionWeight (Scalar) | EmissionWeight |
| 135 | expression | `MaterialExpressionConstant` | 0 |  |
| 136 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 137 | expression | `MaterialExpressionStrataTransmittanceToMFP` | Substrate Transmittance-To-MeanFreePath |  |
| 138 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 139 | expression | `MaterialExpressionConstant` | 1 |  |
| 140 | expression | `MaterialExpressionNamedRerouteDeclaration` | GeometryOpacity |  |
| 141 | expression | `MaterialExpressionNamedRerouteUsage` | GeometryOpacity |  |
| 142 | function_input | `MaterialExpressionFunctionInput` | Input ExtraRoughness (Scalar) | ExtraRoughness |
| 143 | expression | `MaterialExpressionConstant` | 0 |  |
| 144 | expression | `MaterialExpressionNamedRerouteDeclaration` | TransmissionRough |  |
| 145 | expression | `MaterialExpressionNamedRerouteUsage` | TransmissionRough |  |
| 146 | expression | `MaterialExpressionNamedRerouteUsage` | MFP |  |
| 147 | expression | `MaterialExpressionNamedRerouteUsage` | SpecRoughness |  |
| 148 | expression | `MaterialExpressionAdd` | Add |  |
| 149 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 150 | expression | `MaterialExpressionStrataMetalnessToDiffuseAlbedoF0` | Substrate Metalness-To-DiffuseAlbedo-F0 |  |
| 151 | expression | `MaterialExpressionNamedRerouteDeclaration` | FilmThickness |  |
| 152 | expression | `MaterialExpressionNamedRerouteDeclaration` | FilmIOR |  |
| 153 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 154 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 155 | expression | `MaterialExpressionNamedRerouteUsage` | FilmThickness |  |
| 156 | expression | `MaterialExpressionNamedRerouteUsage` | FilmIOR |  |
| 157 | expression | `MaterialExpressionStrataThinFilm` | Substrate Thin-Film |  |
| 158 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 159 | expression | `MaterialExpressionNamedRerouteUsage` | FilmIOR |  |
| 160 | expression | `MaterialExpressionNamedRerouteUsage` | FilmThickness |  |
| 161 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 162 | expression | `MaterialExpressionStep` | Step |  |
| 163 | expression | `MaterialExpressionConstant` | 1e-06 |  |
| 164 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 165 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 166 | expression | `MaterialExpressionSaturate` | Saturate |  |
| 167 | expression | `MaterialExpressionNamedRerouteDeclaration` | TransmissionWeight |  |
| 168 | expression | `MaterialExpressionNamedRerouteUsage` | TransmissionWeight |  |
| 169 | expression | `MaterialExpressionStrataTransmittanceToMFP` | Substrate Transmittance-To-MeanFreePath |  |
| 170 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 171 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 172 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 173 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 174 | expression | `MaterialExpressionCosine` | Cosine |  |
| 175 | expression | `MaterialExpressionSine` | Sine |  |
| 176 | expression | `MaterialExpressionAdd` | Add |  |
| 177 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 178 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 179 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 180 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 181 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 182 | expression | `MaterialExpressionSine` | Sine |  |
| 183 | expression | `MaterialExpressionAdd` | Add |  |
| 184 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 185 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 186 | expression | `MaterialExpressionCosine` | Cosine |  |
| 187 | expression | `MaterialExpressionNamedRerouteDeclaration` | TransmissionScatter |  |
| 188 | expression | `MaterialExpressionNamedRerouteUsage` | TransmissionScatter |  |
| 189 | function_call | `MaterialExpressionMaterialFunctionCall` | MF_SchlickApprox |  |
| 190 | function_call | `MaterialExpressionMaterialFunctionCall` | MF_SchlickInternal |  |

## Exact Input Wiring

### [0] Output Substrate_StandardTranslucent

- `A` <= [115] Substrate Coverage Weight output 0

### [1] Substrate Slab BSDF - Complex

- `DiffuseAlbedo` <= [158] Reroute Node (reroutes wires) output 0
- `F0` <= [157] Substrate Thin-Film output 0
- `F90` <= [157] Substrate Thin-Film output 1
- `Roughness` <= [145] TransmissionRough output 0
- `Anisotropy` <= [94] SpecAnisotropy output 0
- `Normal` <= [83] Normal output 0
- `Tangent` <= [95] SpecAnisoRot output 0
- `SSSMFP` <= [146] MFP output 0
- `EmissiveColor` <= [102] EmissiveColor output 0
- `SecondRoughness` <= [99] SpecRoughness output 0
- `SecondRoughnessWeight` <= [114] 1 output 0
- `FuzzRoughness` <= [123] SheenRoughness output 0
- `FuzzAmount` <= [107] SheenWeight output 0
- `FuzzColor` <= [108] SheenColor output 0

### [2] Substrate Vertical Layer

- `Top` <= [57] Substrate Coverage Weight output 0
- `Base` <= [127] Substrate Horizontal Blend (Parameter Blend) output 0
- `Thickness` <= [137] Substrate Transmittance-To-MeanFreePath output 1

### [3] Input BaseColor (Vector3)

- `Preview` <= [17] 0.18,0.18,0.18 output 0

### [4] Input TransmissionColor (Vector3)

- `Preview` <= [13] 0,0,0 output 0

### [5] Input SpecularColor (Vector3)

- `Preview` <= [31] 1,1,1 output 0

### [6] Input SpecularAnisotropy (Scalar)

- `Preview` <= [37] 0 output 0

### [7] Input AnisotropyRotation (Scalar)

- `Preview` <= [38] 0 output 0

### [8] Input EmissiveColor (Vector3)

- `Preview` <= [10] 0,0,0 output 0

### [11] Input TransmissionDepth (Scalar)

- `Preview` <= [12] 1 output 0

### [14] Input BaseWeight (Scalar)

- `Preview` <= [15] 1 output 0

### [16] Multiply

- `A` <= [14] Input BaseWeight (Scalar) output 0
- `B` <= [3] Input BaseColor (Vector3) output 0

### [18] Input TransmissionWeight (Scalar)

- `Preview` <= [19] 0 output 0

### [20] Input IOR (Scalar)

- `Preview` <= [21] 1.5 output 0

### [22] InvLerp

- `A` <= [23] 0 output 0
- `B` <= [24] 0.08 output 0
- `Value` <= [190] MF_SchlickInternal output 0

### [25] Input Metalness (Scalar)

- `Preview` <= [26] 0 output 0

### [27] Input TransmissionScatter (Vector3)

- `Preview` <= [28] 1,1,1 output 0

### [29] Substrate Slab BSDF - Complex

- `DiffuseAlbedo` <= [111] 0,0,0 output 0
- `F0` <= [189] MF_SchlickApprox output 0
- `F90` <= [58] 1,1,1 output 0
- `Roughness` <= [62] Input CoatRoughness (Scalar) output 0
- `Anisotropy` <= [64] Input CoatAnisotropy (Scalar) output 0
- `Normal` <= [53] Input CoatNormal (Vector3) output 0
- `Tangent` <= [176] Add output 0
- `SSSMFP` <= [137] Substrate Transmittance-To-MeanFreePath output 0

### [30] Saturate

- `Input` <= [22] InvLerp output 0

### [32] Input SpecularWeight (Scalar)

- `Preview` <= [33] 1 output 0

### [34] Multiply

- `A` <= [5] Input SpecularColor (Vector3) output 0
- `B` <= [32] Input SpecularWeight (Scalar) output 0

### [35] Input SpecularRoughness (Scalar)

- `Preview` <= [36] 0.5 output 0

### [39] Input Normal (Vector3)

- `Preview` <= [40] 0,0,1 output 0

### [41] Input ThinFilmThickness (Scalar)

- `Preview` <= [42] 0 output 0

### [43] Input SheenColor (Vector3)

- `Preview` <= [44] 1,1,1 output 0

### [45] Input SheenWeight (Scalar)

- `Preview` <= [46] 0 output 0

### [53] Input CoatNormal (Vector3)

- `Preview` <= [54] 0,0,1 output 0

### [55] Input CoatWeight (Scalar)

- `Preview` <= [56] 0 output 0

### [57] Substrate Coverage Weight

- `A` <= [29] Substrate Slab BSDF - Complex output 0
- `Weight` <= [84] Reroute Node (reroutes wires) output 0

### [59] Input CoatIOR (Scalar)

- `Preview` <= [60] 1.5 output 0

### [62] Input CoatRoughness (Scalar)

- `Preview` <= [63] 0.5 output 0

### [64] Input CoatAnisotropy (Scalar)

- `Preview` <= [66] 0 output 0

### [65] Input CoatRotation (Scalar)

- `Preview` <= [67] 0 output 0

### [68] Input CoatColor (Vector3)

- `Preview` <= [69] 0.999,0.999,0.999 output 0

### [70] Input AnisotropyTangent (Vector3)

- `Preview` <= [149] World Space to Tangent Space TransformVector output 0

### [73] Lerp

- `A` <= [61] 1 output 0
- `B` <= [59] Input CoatIOR (Scalar) output 0
- `Alpha` <= [162] Step output 0

### [74] Substrate Thin-Film

- `Normal` <= [98] Normal output 0
- `F0` <= [150] Substrate Metalness-To-DiffuseAlbedo-F0 output 1
- `F90` <= [90] WeightedSpec output 0
- `Thickness` <= [155] FilmThickness output 0
- `IOR` <= [156] FilmIOR output 0

### [75] AnisotropyTangent

- `Input` <= [171] Normalize output 0

### [78] CoatIOR

- `Input` <= [73] Lerp output 0

### [79] Input ThinFilmIOR (Scalar)

- `Preview` <= [80] 1.5 output 0

### [82] Normal

- `Input` <= [39] Input Normal (Vector3) output 0

### [84] Reroute Node (reroutes wires)

- `Input` <= [55] Input CoatWeight (Scalar) output 0

### [85] Substrate Slab BSDF - Complex

- `DiffuseAlbedo` <= [153] Reroute Node (reroutes wires) output 0
- `F0` <= [74] Substrate Thin-Film output 0
- `F90` <= [74] Substrate Thin-Film output 1
- `Roughness` <= [122] DiffuseRoughness output 0
- `Anisotropy` <= [97] SpecAnisotropy output 0
- `Normal` <= [154] Reroute Node (reroutes wires) output 0
- `Tangent` <= [100] SpecAnisoRot output 0
- `EmissiveColor` <= [103] EmissiveColor output 0
- `SecondRoughness` <= [96] SpecRoughness output 0
- `SecondRoughnessWeight` <= [128] 1 output 0

### [86] WeightedBase

- `Input` <= [16] Multiply output 0

### [87] SlabIOR

- `Input` <= [30] Saturate output 0

### [88] Metalness

- `Input` <= [25] Input Metalness (Scalar) output 0

### [89] WeightedSpec

- `Input` <= [34] Multiply output 0

### [91] SpecRoughness

- `Input` <= [35] Input SpecularRoughness (Scalar) output 0

### [92] SpecAnisotropy

- `Input` <= [6] Input SpecularAnisotropy (Scalar) output 0

### [93] SpecAnisoRot

- `Input` <= [183] Add output 0

### [101] EmissiveColor

- `Input` <= [136] Multiply output 0

### [104] BaseWeight

- `Input` <= [14] Input BaseWeight (Scalar) output 0

### [105] SheenWeight

- `Input` <= [45] Input SheenWeight (Scalar) output 0

### [106] SheenColor

- `Input` <= [43] Input SheenColor (Vector3) output 0

### [110] SpecF0

- `Input` <= [126] Multiply output 0

### [113] MFP

- `Input` <= [164] Lerp output 0

### [115] Substrate Coverage Weight

- `A` <= [2] Substrate Vertical Layer output 0
- `Weight` <= [141] GeometryOpacity output 0

### [116] Input DiffuseRoughness (Scalar)

- `Preview` <= [117] 0 output 0

### [118] DiffuseRoughness

- `Input` <= [133] Lerp(,0,) output 0

### [119] Input SheenRoughness (Scalar)

- `Preview` <= [121] 0 output 0

### [120] SheenRoughness

- `Input` <= [119] Input SheenRoughness (Scalar) output 0

### [125] Lerp

- `A` <= [124] WeightedBase output 0
- `B` <= [188] TransmissionScatter output 0
- `Alpha` <= [168] TransmissionWeight output 0

### [126] Multiply

- `A` <= [190] MF_SchlickInternal output 0
- `B` <= [34] Multiply output 0

### [127] Substrate Horizontal Blend (Parameter Blend)

- `Background` <= [1] Substrate Slab BSDF - Complex output 0
- `Foreground` <= [85] Substrate Slab BSDF - Complex output 0
- `Mix` <= [131] Metalness output 0

### [133] Lerp(,0,)

- `A` <= [116] Input DiffuseRoughness (Scalar) output 0
- `Alpha` <= [25] Input Metalness (Scalar) output 0

### [134] Input EmissionWeight (Scalar)

- `Preview` <= [135] 0 output 0

### [136] Multiply

- `A` <= [8] Input EmissiveColor (Vector3) output 0
- `B` <= [134] Input EmissionWeight (Scalar) output 0

### [137] Substrate Transmittance-To-MeanFreePath

- `TransmittanceColor` <= [68] Input CoatColor (Vector3) output 0

### [138] Input Opacity (Scalar)

- `Preview` <= [139] 1 output 0

### [140] GeometryOpacity

- `Input` <= [138] Input Opacity (Scalar) output 0

### [142] Input ExtraRoughness (Scalar)

- `Preview` <= [143] 0 output 0

### [144] TransmissionRough

- `Input` <= [148] Add output 0

### [148] Add

- `A` <= [142] Input ExtraRoughness (Scalar) output 0
- `B` <= [147] SpecRoughness output 0

### [149] World Space to Tangent Space TransformVector

- `Input` <= [71] VertexTangentWS output 0

### [150] Substrate Metalness-To-DiffuseAlbedo-F0

- `BaseColor` <= [129] WeightedBase output 0
- `Metallic` <= [130] Metalness output 0
- `Specular` <= [132] SlabIOR output 0

### [151] FilmThickness

- `Input` <= [41] Input ThinFilmThickness (Scalar) output 0

### [152] FilmIOR

- `Input` <= [79] Input ThinFilmIOR (Scalar) output 0

### [153] Reroute Node (reroutes wires)

- `Input` <= [150] Substrate Metalness-To-DiffuseAlbedo-F0 output 0

### [154] Reroute Node (reroutes wires)

- `Input` <= [98] Normal output 0

### [157] Substrate Thin-Film

- `Normal` <= [83] Normal output 0
- `F0` <= [112] SpecF0 output 0
- `F90` <= [109] WeightedSpec output 0
- `Thickness` <= [160] FilmThickness output 0
- `IOR` <= [159] FilmIOR output 0

### [158] Reroute Node (reroutes wires)

- `Input` <= [125] Lerp output 0

### [161] Multiply

- `A` <= [169] Substrate Transmittance-To-MeanFreePath output 0
- `B` <= [11] Input TransmissionDepth (Scalar) output 0

### [162] Step

- `Y` <= [163] 1e-06 output 0
- `X` <= [55] Input CoatWeight (Scalar) output 0

### [164] Lerp

- `A` <= [165] 0,0,0 output 0
- `B` <= [161] Multiply output 0
- `Alpha` <= [166] Saturate output 0

### [166] Saturate

- `Input` <= [18] Input TransmissionWeight (Scalar) output 0

### [167] TransmissionWeight

- `Input` <= [166] Saturate output 0

### [169] Substrate Transmittance-To-MeanFreePath

- `TransmittanceColor` <= [4] Input TransmissionColor (Vector3) output 0

### [171] Normalize

- `VectorInput` <= [70] Input AnisotropyTangent (Vector3) output 0

### [172] Cross

- `A` <= [170] VertexNormalWS output 0
- `B` <= [76] AnisotropyTangent output 0

### [173] Normalize

- `VectorInput` <= [172] Cross output 0

### [174] Cosine

- `Input` <= [65] Input CoatRotation (Scalar) output 0

### [175] Sine

- `Input` <= [65] Input CoatRotation (Scalar) output 0

### [176] Add

- `A` <= [178] Multiply output 0
- `B` <= [177] Multiply output 0

### [177] Multiply

- `A` <= [76] AnisotropyTangent output 0
- `B` <= [174] Cosine output 0

### [178] Multiply

- `A` <= [175] Sine output 0
- `B` <= [173] Normalize output 0

### [180] Cross

- `A` <= [179] VertexNormalWS output 0
- `B` <= [77] AnisotropyTangent output 0

### [181] Normalize

- `VectorInput` <= [180] Cross output 0

### [182] Sine

- `Input` <= [7] Input AnisotropyRotation (Scalar) output 0

### [183] Add

- `A` <= [185] Multiply output 0
- `B` <= [184] Multiply output 0

### [184] Multiply

- `A` <= [77] AnisotropyTangent output 0
- `B` <= [186] Cosine output 0

### [185] Multiply

- `A` <= [182] Sine output 0
- `B` <= [181] Normalize output 0

### [186] Cosine

- `Input` <= [7] Input AnisotropyRotation (Scalar) output 0

### [187] TransmissionScatter

- `Input` <= [27] Input TransmissionScatter (Vector3) output 0

## Connection List

- [115] Substrate Coverage Weight output 0 -> Output Substrate_StandardTranslucent.A
- [158] Reroute Node (reroutes wires) output 0 -> Substrate Slab BSDF - Complex.DiffuseAlbedo
- [157] Substrate Thin-Film output 0 -> Substrate Slab BSDF - Complex.F0
- [157] Substrate Thin-Film output 1 -> Substrate Slab BSDF - Complex.F90
- [145] TransmissionRough output 0 -> Substrate Slab BSDF - Complex.Roughness
- [94] SpecAnisotropy output 0 -> Substrate Slab BSDF - Complex.Anisotropy
- [83] Normal output 0 -> Substrate Slab BSDF - Complex.Normal
- [95] SpecAnisoRot output 0 -> Substrate Slab BSDF - Complex.Tangent
- [146] MFP output 0 -> Substrate Slab BSDF - Complex.SSSMFP
- [102] EmissiveColor output 0 -> Substrate Slab BSDF - Complex.EmissiveColor
- [99] SpecRoughness output 0 -> Substrate Slab BSDF - Complex.SecondRoughness
- [114] 1 output 0 -> Substrate Slab BSDF - Complex.SecondRoughnessWeight
- [123] SheenRoughness output 0 -> Substrate Slab BSDF - Complex.FuzzRoughness
- [107] SheenWeight output 0 -> Substrate Slab BSDF - Complex.FuzzAmount
- [108] SheenColor output 0 -> Substrate Slab BSDF - Complex.FuzzColor
- [57] Substrate Coverage Weight output 0 -> Substrate Vertical Layer.Top
- [127] Substrate Horizontal Blend (Parameter Blend) output 0 -> Substrate Vertical Layer.Base
- [137] Substrate Transmittance-To-MeanFreePath output 1 -> Substrate Vertical Layer.Thickness
- [17] 0.18,0.18,0.18 output 0 -> Input BaseColor (Vector3).Preview
- [13] 0,0,0 output 0 -> Input TransmissionColor (Vector3).Preview
- [31] 1,1,1 output 0 -> Input SpecularColor (Vector3).Preview
- [37] 0 output 0 -> Input SpecularAnisotropy (Scalar).Preview
- [38] 0 output 0 -> Input AnisotropyRotation (Scalar).Preview
- [10] 0,0,0 output 0 -> Input EmissiveColor (Vector3).Preview
- [12] 1 output 0 -> Input TransmissionDepth (Scalar).Preview
- [15] 1 output 0 -> Input BaseWeight (Scalar).Preview
- [14] Input BaseWeight (Scalar) output 0 -> Multiply.A
- [3] Input BaseColor (Vector3) output 0 -> Multiply.B
- [19] 0 output 0 -> Input TransmissionWeight (Scalar).Preview
- [21] 1.5 output 0 -> Input IOR (Scalar).Preview
- [23] 0 output 0 -> InvLerp.A
- [24] 0.08 output 0 -> InvLerp.B
- [190] MF_SchlickInternal output 0 -> InvLerp.Value
- [26] 0 output 0 -> Input Metalness (Scalar).Preview
- [28] 1,1,1 output 0 -> Input TransmissionScatter (Vector3).Preview
- [111] 0,0,0 output 0 -> Substrate Slab BSDF - Complex.DiffuseAlbedo
- [189] MF_SchlickApprox output 0 -> Substrate Slab BSDF - Complex.F0
- [58] 1,1,1 output 0 -> Substrate Slab BSDF - Complex.F90
- [62] Input CoatRoughness (Scalar) output 0 -> Substrate Slab BSDF - Complex.Roughness
- [64] Input CoatAnisotropy (Scalar) output 0 -> Substrate Slab BSDF - Complex.Anisotropy
- [53] Input CoatNormal (Vector3) output 0 -> Substrate Slab BSDF - Complex.Normal
- [176] Add output 0 -> Substrate Slab BSDF - Complex.Tangent
- [137] Substrate Transmittance-To-MeanFreePath output 0 -> Substrate Slab BSDF - Complex.SSSMFP
- [22] InvLerp output 0 -> Saturate.Input
- [33] 1 output 0 -> Input SpecularWeight (Scalar).Preview
- [5] Input SpecularColor (Vector3) output 0 -> Multiply.A
- [32] Input SpecularWeight (Scalar) output 0 -> Multiply.B
- [36] 0.5 output 0 -> Input SpecularRoughness (Scalar).Preview
- [40] 0,0,1 output 0 -> Input Normal (Vector3).Preview
- [42] 0 output 0 -> Input ThinFilmThickness (Scalar).Preview
- [44] 1,1,1 output 0 -> Input SheenColor (Vector3).Preview
- [46] 0 output 0 -> Input SheenWeight (Scalar).Preview
- [54] 0,0,1 output 0 -> Input CoatNormal (Vector3).Preview
- [56] 0 output 0 -> Input CoatWeight (Scalar).Preview
- [29] Substrate Slab BSDF - Complex output 0 -> Substrate Coverage Weight.A
- [84] Reroute Node (reroutes wires) output 0 -> Substrate Coverage Weight.Weight
- [60] 1.5 output 0 -> Input CoatIOR (Scalar).Preview
- [63] 0.5 output 0 -> Input CoatRoughness (Scalar).Preview
- [66] 0 output 0 -> Input CoatAnisotropy (Scalar).Preview
- [67] 0 output 0 -> Input CoatRotation (Scalar).Preview
- [69] 0.999,0.999,0.999 output 0 -> Input CoatColor (Vector3).Preview
- [149] World Space to Tangent Space TransformVector output 0 -> Input AnisotropyTangent (Vector3).Preview
- [61] 1 output 0 -> Lerp.A
- [59] Input CoatIOR (Scalar) output 0 -> Lerp.B
- [162] Step output 0 -> Lerp.Alpha
- [98] Normal output 0 -> Substrate Thin-Film.Normal
- [150] Substrate Metalness-To-DiffuseAlbedo-F0 output 1 -> Substrate Thin-Film.F0
- [90] WeightedSpec output 0 -> Substrate Thin-Film.F90
- [155] FilmThickness output 0 -> Substrate Thin-Film.Thickness
- [156] FilmIOR output 0 -> Substrate Thin-Film.IOR
- [171] Normalize output 0 -> AnisotropyTangent.Input
- [73] Lerp output 0 -> CoatIOR.Input
- [80] 1.5 output 0 -> Input ThinFilmIOR (Scalar).Preview
- [39] Input Normal (Vector3) output 0 -> Normal.Input
- [55] Input CoatWeight (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [153] Reroute Node (reroutes wires) output 0 -> Substrate Slab BSDF - Complex.DiffuseAlbedo
- [74] Substrate Thin-Film output 0 -> Substrate Slab BSDF - Complex.F0
- [74] Substrate Thin-Film output 1 -> Substrate Slab BSDF - Complex.F90
- [122] DiffuseRoughness output 0 -> Substrate Slab BSDF - Complex.Roughness
- [97] SpecAnisotropy output 0 -> Substrate Slab BSDF - Complex.Anisotropy
- [154] Reroute Node (reroutes wires) output 0 -> Substrate Slab BSDF - Complex.Normal
- [100] SpecAnisoRot output 0 -> Substrate Slab BSDF - Complex.Tangent
- [103] EmissiveColor output 0 -> Substrate Slab BSDF - Complex.EmissiveColor
- [96] SpecRoughness output 0 -> Substrate Slab BSDF - Complex.SecondRoughness
- [128] 1 output 0 -> Substrate Slab BSDF - Complex.SecondRoughnessWeight
- [16] Multiply output 0 -> WeightedBase.Input
- [30] Saturate output 0 -> SlabIOR.Input
- [25] Input Metalness (Scalar) output 0 -> Metalness.Input
- [34] Multiply output 0 -> WeightedSpec.Input
- [35] Input SpecularRoughness (Scalar) output 0 -> SpecRoughness.Input
- [6] Input SpecularAnisotropy (Scalar) output 0 -> SpecAnisotropy.Input
- [183] Add output 0 -> SpecAnisoRot.Input
- [136] Multiply output 0 -> EmissiveColor.Input
- [14] Input BaseWeight (Scalar) output 0 -> BaseWeight.Input
- [45] Input SheenWeight (Scalar) output 0 -> SheenWeight.Input
- [43] Input SheenColor (Vector3) output 0 -> SheenColor.Input
- [126] Multiply output 0 -> SpecF0.Input
- [164] Lerp output 0 -> MFP.Input
- [2] Substrate Vertical Layer output 0 -> Substrate Coverage Weight.A
- [141] GeometryOpacity output 0 -> Substrate Coverage Weight.Weight
- [117] 0 output 0 -> Input DiffuseRoughness (Scalar).Preview
- [133] Lerp(,0,) output 0 -> DiffuseRoughness.Input
- [121] 0 output 0 -> Input SheenRoughness (Scalar).Preview
- [119] Input SheenRoughness (Scalar) output 0 -> SheenRoughness.Input
- [124] WeightedBase output 0 -> Lerp.A
- [188] TransmissionScatter output 0 -> Lerp.B
- [168] TransmissionWeight output 0 -> Lerp.Alpha
- [190] MF_SchlickInternal output 0 -> Multiply.A
- [34] Multiply output 0 -> Multiply.B
- [1] Substrate Slab BSDF - Complex output 0 -> Substrate Horizontal Blend (Parameter Blend).Background
- [85] Substrate Slab BSDF - Complex output 0 -> Substrate Horizontal Blend (Parameter Blend).Foreground
- [131] Metalness output 0 -> Substrate Horizontal Blend (Parameter Blend).Mix
- [116] Input DiffuseRoughness (Scalar) output 0 -> Lerp(,0,).A
- [25] Input Metalness (Scalar) output 0 -> Lerp(,0,).Alpha
- [135] 0 output 0 -> Input EmissionWeight (Scalar).Preview
- [8] Input EmissiveColor (Vector3) output 0 -> Multiply.A
- [134] Input EmissionWeight (Scalar) output 0 -> Multiply.B
- [68] Input CoatColor (Vector3) output 0 -> Substrate Transmittance-To-MeanFreePath.TransmittanceColor
- [139] 1 output 0 -> Input Opacity (Scalar).Preview
- [138] Input Opacity (Scalar) output 0 -> GeometryOpacity.Input
- [143] 0 output 0 -> Input ExtraRoughness (Scalar).Preview
- [148] Add output 0 -> TransmissionRough.Input
- [142] Input ExtraRoughness (Scalar) output 0 -> Add.A
- [147] SpecRoughness output 0 -> Add.B
- [71] VertexTangentWS output 0 -> World Space to Tangent Space TransformVector.Input
- [129] WeightedBase output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.BaseColor
- [130] Metalness output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Metallic
- [132] SlabIOR output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Specular
- [41] Input ThinFilmThickness (Scalar) output 0 -> FilmThickness.Input
- [79] Input ThinFilmIOR (Scalar) output 0 -> FilmIOR.Input
- [150] Substrate Metalness-To-DiffuseAlbedo-F0 output 0 -> Reroute Node (reroutes wires).Input
- [98] Normal output 0 -> Reroute Node (reroutes wires).Input
- [83] Normal output 0 -> Substrate Thin-Film.Normal
- [112] SpecF0 output 0 -> Substrate Thin-Film.F0
- [109] WeightedSpec output 0 -> Substrate Thin-Film.F90
- [160] FilmThickness output 0 -> Substrate Thin-Film.Thickness
- [159] FilmIOR output 0 -> Substrate Thin-Film.IOR
- [125] Lerp output 0 -> Reroute Node (reroutes wires).Input
- [169] Substrate Transmittance-To-MeanFreePath output 0 -> Multiply.A
- [11] Input TransmissionDepth (Scalar) output 0 -> Multiply.B
- [163] 1e-06 output 0 -> Step.Y
- [55] Input CoatWeight (Scalar) output 0 -> Step.X
- [165] 0,0,0 output 0 -> Lerp.A
- [161] Multiply output 0 -> Lerp.B
- [166] Saturate output 0 -> Lerp.Alpha
- [18] Input TransmissionWeight (Scalar) output 0 -> Saturate.Input
- [166] Saturate output 0 -> TransmissionWeight.Input
- [4] Input TransmissionColor (Vector3) output 0 -> Substrate Transmittance-To-MeanFreePath.TransmittanceColor
- [70] Input AnisotropyTangent (Vector3) output 0 -> Normalize.VectorInput
- [170] VertexNormalWS output 0 -> Cross.A
- [76] AnisotropyTangent output 0 -> Cross.B
- [172] Cross output 0 -> Normalize.VectorInput
- [65] Input CoatRotation (Scalar) output 0 -> Cosine.Input
- [65] Input CoatRotation (Scalar) output 0 -> Sine.Input
- [178] Multiply output 0 -> Add.A
- [177] Multiply output 0 -> Add.B
- [76] AnisotropyTangent output 0 -> Multiply.A
- [174] Cosine output 0 -> Multiply.B
- [175] Sine output 0 -> Multiply.A
- [173] Normalize output 0 -> Multiply.B
- [179] VertexNormalWS output 0 -> Cross.A
- [77] AnisotropyTangent output 0 -> Cross.B
- [180] Cross output 0 -> Normalize.VectorInput
- [7] Input AnisotropyRotation (Scalar) output 0 -> Sine.Input
- [185] Multiply output 0 -> Add.A
- [184] Multiply output 0 -> Add.B
- [77] AnisotropyTangent output 0 -> Multiply.A
- [186] Cosine output 0 -> Multiply.B
- [182] Sine output 0 -> Multiply.A
- [181] Normalize output 0 -> Multiply.B
- [7] Input AnisotropyRotation (Scalar) output 0 -> Cosine.Input
- [27] Input TransmissionScatter (Vector3) output 0 -> TransmissionScatter.Input

## Output Trace Roots

- `Substrate_StandardTranslucent` is driven by [115] Substrate Coverage Weight

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


