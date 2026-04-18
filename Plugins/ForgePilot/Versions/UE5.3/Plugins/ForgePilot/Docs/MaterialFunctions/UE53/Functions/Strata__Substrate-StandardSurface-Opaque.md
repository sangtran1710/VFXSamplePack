# Substrate-StandardSurface-Opaque

- Path: `/Engine/Functions/Strata/Substrate-StandardSurface-Opaque.Substrate-StandardSurface-Opaque`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Opaque Standard Surface

## Inputs

- `BaseColor` [FunctionInput_Vector3]
- `ScatterRadius` [FunctionInput_Vector3]
- `ScatterScale` [FunctionInput_Scalar]
- `SpecularColor` [FunctionInput_Vector3]
- `SpecularAnisotropy` [FunctionInput_Scalar]
- `AnisotropyRotation` [FunctionInput_Scalar]
- `EmissiveColor` [FunctionInput_Vector3]
- `---- Coat ----` [FunctionInput_StaticBool]
- `BaseWeight` [FunctionInput_Scalar]
- `ScatterWeight` [FunctionInput_Scalar]
- `IOR` [FunctionInput_Scalar]
- `Metalness` [FunctionInput_Scalar]
- `SubsurfaceColor` [FunctionInput_Vector3]
- `SpecularWeight` [FunctionInput_Scalar]
- `SpecularRoughness` [FunctionInput_Scalar]
- `Normal` [FunctionInput_Vector3]
- `ThinFilmThickness` [FunctionInput_Scalar]
- `SheenColor` [FunctionInput_Vector3]
- `SheenWeight` [FunctionInput_Scalar]
- `---- Subsurface ----` [FunctionInput_StaticBool]
- `---- Sheen ----` [FunctionInput_StaticBool]
- `---- Emission ----` [FunctionInput_StaticBool]
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
- `ScatterAnisotropy` [FunctionInput_Scalar]

## Outputs

- `Substrate StandardSurface Opaque`
- `Geometry Opacity`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Substrate StandardSurface Opaque | Substrate StandardSurface Opaque |
| 1 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Complex |  |
| 2 | expression | `MaterialExpressionStrataVerticalLayering` | Substrate Vertical Layer |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input BaseColor (Vector3) | BaseColor |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input ScatterRadius (Vector3) | ScatterRadius |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input ScatterScale (Scalar) | ScatterScale |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input SpecularColor (Vector3) | SpecularColor |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input SpecularAnisotropy (Scalar) | SpecularAnisotropy |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input AnisotropyRotation (Scalar) | AnisotropyRotation |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input EmissiveColor (Vector3) | EmissiveColor |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input ---- Coat ---- (StaticBool) | ---- Coat ---- |
| 11 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 12 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 13 | expression | `MaterialExpressionConstant` | 1 |  |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input BaseWeight (Scalar) | BaseWeight |
| 15 | expression | `MaterialExpressionConstant` | 1 |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionConstant3Vector` | 0.18,0.18,0.18 |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input ScatterWeight (Scalar) | ScatterWeight |
| 19 | expression | `MaterialExpressionConstant` | 0 |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input IOR (Scalar) | IOR |
| 21 | expression | `MaterialExpressionConstant` | 1.5 |  |
| 22 | expression | `MaterialExpressionInverseLinearInterpolate` | InvLerp |  |
| 23 | expression | `MaterialExpressionConstant` | 0 |  |
| 24 | expression | `MaterialExpressionConstant` | 0.08 |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Metalness (Scalar) | Metalness |
| 26 | expression | `MaterialExpressionConstant` | 0 |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input SubsurfaceColor (Vector3) | SubsurfaceColor |
| 28 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
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
| 47 | function_input | `MaterialExpressionFunctionInput` | Input ---- Subsurface ---- (StaticBool) | ---- Subsurface ---- |
| 48 | function_input | `MaterialExpressionFunctionInput` | Input ---- Sheen ---- (StaticBool) | ---- Sheen ---- |
| 49 | function_input | `MaterialExpressionFunctionInput` | Input ---- Emission ---- (StaticBool) | ---- Emission ---- |
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
| 69 | expression | `MaterialExpressionConstant3Vector` | 1,1,1 |  |
| 70 | function_input | `MaterialExpressionFunctionInput` | Input AnisotropyTangent (Vector3) | AnisotropyTangent |
| 71 | expression | `MaterialExpressionVertexTangentWS` | VertexTangentWS |  |
| 72 | function_input | `MaterialExpressionFunctionInput` | Input ---- Base ---- (StaticBool) | ---- Base ---- |
| 73 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 74 | function_call | `MaterialExpressionMaterialFunctionCall` | MF_SchlickApprox |  |
| 75 | function_call | `MaterialExpressionMaterialFunctionCall` | MF_SchlickInternal |  |
| 76 | expression | `MaterialExpressionNamedRerouteDeclaration` | AnisotropyTangent |  |
| 77 | expression | `MaterialExpressionNamedRerouteUsage` | AnisotropyTangent |  |
| 78 | expression | `MaterialExpressionNamedRerouteUsage` | AnisotropyTangent |  |
| 79 | expression | `MaterialExpressionNamedRerouteDeclaration` | CoatIOR |  |
| 80 | function_input | `MaterialExpressionFunctionInput` | Input ThinFilmIOR (Scalar) | ThinFilmIOR |
| 81 | expression | `MaterialExpressionConstant` | 1.5 |  |
| 82 | expression | `MaterialExpressionNamedRerouteUsage` | CoatIOR |  |
| 83 | expression | `MaterialExpressionNamedRerouteDeclaration` | Normal |  |
| 84 | expression | `MaterialExpressionNamedRerouteUsage` | Normal |  |
| 85 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 86 | expression | `MaterialExpressionStrataSlabBSDF` | Substrate Slab BSDF - Complex |  |
| 87 | expression | `MaterialExpressionNamedRerouteDeclaration` | WeightedBase |  |
| 88 | expression | `MaterialExpressionNamedRerouteDeclaration` | SlabIOR |  |
| 89 | expression | `MaterialExpressionNamedRerouteDeclaration` | Metalness |  |
| 90 | expression | `MaterialExpressionNamedRerouteDeclaration` | WeightedSpec |  |
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
| 109 | expression | `MaterialExpressionNamedRerouteDeclaration` | ScatterWeight |  |
| 110 | expression | `MaterialExpressionNamedRerouteDeclaration` | WeightedScatter |  |
| 111 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedSpec |  |
| 112 | expression | `MaterialExpressionNamedRerouteDeclaration` | SpecF0 |  |
| 113 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 114 | expression | `MaterialExpressionNamedRerouteUsage` | SpecF0 |  |
| 115 | expression | `MaterialExpressionNamedRerouteDeclaration` | ScatterRadius |  |
| 116 | expression | `MaterialExpressionNamedRerouteDeclaration` | ScatterScale |  |
| 117 | expression | `MaterialExpressionNamedRerouteUsage` | ScatterScale |  |
| 118 | expression | `MaterialExpressionConstant` | 1 |  |
| 119 | expression | `MaterialExpressionStrataWeight` | Substrate Coverage Weight |  |
| 120 | function_input | `MaterialExpressionFunctionInput` | Input DiffuseRoughness (Scalar) | DiffuseRoughness |
| 121 | expression | `MaterialExpressionConstant` | 0 |  |
| 122 | expression | `MaterialExpressionNamedRerouteDeclaration` | DiffuseRoughness |  |
| 123 | function_input | `MaterialExpressionFunctionInput` | Input SheenRoughness (Scalar) | SheenRoughness |
| 124 | expression | `MaterialExpressionNamedRerouteDeclaration` | SheenRoughness |  |
| 125 | expression | `MaterialExpressionConstant` | 0 |  |
| 126 | expression | `MaterialExpressionNamedRerouteUsage` | DiffuseRoughness |  |
| 127 | expression | `MaterialExpressionNamedRerouteUsage` | SheenRoughness |  |
| 128 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedBase |  |
| 129 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 130 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedScatter |  |
| 131 | expression | `MaterialExpressionNamedRerouteUsage` | ScatterWeight |  |
| 132 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 133 | expression | `MaterialExpressionConstant` | 1 |  |
| 134 | expression | `MaterialExpressionNamedRerouteUsage` | Metalness |  |
| 135 | expression | `MaterialExpressionNamedRerouteUsage` | DiffuseRoughness |  |
| 136 | expression | `MaterialExpressionLinearInterpolate` | Lerp(,0,) |  |
| 137 | function_input | `MaterialExpressionFunctionInput` | Input EmissionWeight (Scalar) | EmissionWeight |
| 138 | expression | `MaterialExpressionConstant` | 0 |  |
| 139 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 140 | expression | `MaterialExpressionNamedRerouteUsage` | ScatterRadius |  |
| 141 | expression | `MaterialExpressionStrataTransmittanceToMFP` | Substrate Transmittance-To-MeanFreePath |  |
| 142 | function_input | `MaterialExpressionFunctionInput` | Input Opacity (Scalar) | Opacity |
| 143 | expression | `MaterialExpressionConstant` | 1 |  |
| 144 | expression | `MaterialExpressionNamedRerouteDeclaration` | GeometryOpacity |  |
| 145 | expression | `MaterialExpressionNamedRerouteUsage` | GeometryOpacity |  |
| 146 | function_input | `MaterialExpressionFunctionInput` | Input ScatterAnisotropy (Scalar) | ScatterAnisotropy |
| 147 | expression | `MaterialExpressionConstant` | 0 |  |
| 148 | expression | `MaterialExpressionNamedRerouteDeclaration` | ScatterAniso |  |
| 149 | expression | `MaterialExpressionNamedRerouteUsage` | ScatterAniso |  |
| 150 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 151 | expression | `MaterialExpressionStrataThinFilm` | Substrate Thin-Film |  |
| 152 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedSpec |  |
| 153 | expression | `MaterialExpressionNamedRerouteUsage` | WeightedBase |  |
| 154 | expression | `MaterialExpressionNamedRerouteUsage` | Metalness |  |
| 155 | expression | `MaterialExpressionNamedRerouteUsage` | SlabIOR |  |
| 156 | expression | `MaterialExpressionStrataMetalnessToDiffuseAlbedoF0` | Substrate Metalness-To-DiffuseAlbedo-F0 |  |
| 157 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 158 | expression | `MaterialExpressionNamedRerouteDeclaration` | FilmThickness |  |
| 159 | expression | `MaterialExpressionNamedRerouteDeclaration` | FilmIOR |  |
| 160 | expression | `MaterialExpressionNamedRerouteUsage` | FilmIOR |  |
| 161 | expression | `MaterialExpressionNamedRerouteUsage` | FilmThickness |  |
| 162 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 163 | expression | `MaterialExpressionNamedRerouteUsage` | FilmThickness |  |
| 164 | expression | `MaterialExpressionNamedRerouteUsage` | FilmIOR |  |
| 165 | expression | `MaterialExpressionStrataThinFilm` | Substrate Thin-Film |  |
| 166 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 167 | expression | `MaterialExpressionStep` | Step |  |
| 168 | expression | `MaterialExpressionConstant` | 1e-06 |  |
| 169 | expression | `MaterialExpressionStrataHorizontalMixing` | Substrate Horizontal Blend (Parameter Blend) |  |
| 170 | expression | `MaterialExpressionCosine` | Cosine |  |
| 171 | expression | `MaterialExpressionSine` | Sine |  |
| 172 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 173 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 174 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 175 | expression | `MaterialExpressionAdd` | Add |  |
| 176 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 177 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 178 | expression | `MaterialExpressionCosine` | Cosine |  |
| 179 | expression | `MaterialExpressionSine` | Sine |  |
| 180 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 181 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 182 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 183 | expression | `MaterialExpressionAdd` | Add |  |
| 184 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 185 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 186 | function_output | `MaterialExpressionFunctionOutput` | Output Geometry Opacity | Geometry Opacity |

## Exact Input Wiring

### [0] Output Substrate StandardSurface Opaque

- `A` <= [119] Substrate Coverage Weight output 0

### [1] Substrate Slab BSDF - Complex

- `DiffuseAlbedo` <= [166] Reroute Node (reroutes wires) output 0
- `F0` <= [165] Substrate Thin-Film output 0
- `F90` <= [165] Substrate Thin-Film output 1
- `Roughness` <= [126] DiffuseRoughness output 0
- `Anisotropy` <= [94] SpecAnisotropy output 0
- `Normal` <= [84] Normal output 0
- `Tangent` <= [95] SpecAnisoRot output 0
- `SSSMFP` <= [140] ScatterRadius output 0
- `SSSMFPScale` <= [117] ScatterScale output 0
- `SSSPhaseAnisotropy` <= [149] ScatterAniso output 0
- `EmissiveColor` <= [102] EmissiveColor output 0
- `SecondRoughness` <= [99] SpecRoughness output 0
- `SecondRoughnessWeight` <= [118] 1 output 0
- `FuzzRoughness` <= [127] SheenRoughness output 0
- `FuzzAmount` <= [107] SheenWeight output 0
- `FuzzColor` <= [108] SheenColor output 0

### [2] Substrate Vertical Layer

- `Top` <= [57] Substrate Coverage Weight output 0
- `Base` <= [169] Substrate Horizontal Blend (Parameter Blend) output 0
- `Thickness` <= [141] Substrate Transmittance-To-MeanFreePath output 1

### [3] Input BaseColor (Vector3)

- `Preview` <= [17] 0.18,0.18,0.18 output 0

### [4] Input ScatterRadius (Vector3)

- `Preview` <= [11] 0,0,0 output 0

### [5] Input ScatterScale (Scalar)

- `Preview` <= [13] 1 output 0

### [6] Input SpecularColor (Vector3)

- `Preview` <= [31] 1,1,1 output 0

### [7] Input SpecularAnisotropy (Scalar)

- `Preview` <= [37] 0 output 0

### [8] Input AnisotropyRotation (Scalar)

- `Preview` <= [38] 0 output 0

### [9] Input EmissiveColor (Vector3)

- `Preview` <= [12] 0,0,0 output 0

### [14] Input BaseWeight (Scalar)

- `Preview` <= [15] 1 output 0

### [16] Multiply

- `A` <= [14] Input BaseWeight (Scalar) output 0
- `B` <= [3] Input BaseColor (Vector3) output 0

### [18] Input ScatterWeight (Scalar)

- `Preview` <= [19] 0 output 0

### [20] Input IOR (Scalar)

- `Preview` <= [21] 1.5 output 0

### [22] InvLerp

- `A` <= [23] 0 output 0
- `B` <= [24] 0.08 output 0
- `Value` <= [75] MF_SchlickInternal output 0

### [25] Input Metalness (Scalar)

- `Preview` <= [26] 0 output 0

### [27] Input SubsurfaceColor (Vector3)

- `Preview` <= [28] 0,0,0 output 0

### [29] Substrate Slab BSDF - Complex

- `DiffuseAlbedo` <= [113] 0,0,0 output 0
- `F0` <= [74] MF_SchlickApprox output 0
- `F90` <= [58] 1,1,1 output 0
- `Roughness` <= [62] Input CoatRoughness (Scalar) output 0
- `Anisotropy` <= [64] Input CoatAnisotropy (Scalar) output 0
- `Normal` <= [53] Input CoatNormal (Vector3) output 0
- `Tangent` <= [175] Add output 0
- `SSSMFP` <= [141] Substrate Transmittance-To-MeanFreePath output 0

### [30] Saturate

- `Input` <= [22] InvLerp output 0

### [32] Input SpecularWeight (Scalar)

- `Preview` <= [33] 1 output 0

### [34] Multiply

- `A` <= [6] Input SpecularColor (Vector3) output 0
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
- `Weight` <= [85] Reroute Node (reroutes wires) output 0

### [59] Input CoatIOR (Scalar)

- `Preview` <= [60] 1.5 output 0

### [62] Input CoatRoughness (Scalar)

- `Preview` <= [63] 0.5 output 0

### [64] Input CoatAnisotropy (Scalar)

- `Preview` <= [66] 0 output 0

### [65] Input CoatRotation (Scalar)

- `Preview` <= [67] 0 output 0

### [68] Input CoatColor (Vector3)

- `Preview` <= [69] 1,1,1 output 0

### [70] Input AnisotropyTangent (Vector3)

- `Preview` <= [150] World Space to Tangent Space TransformVector output 0

### [73] Lerp

- `A` <= [61] 1 output 0
- `B` <= [59] Input CoatIOR (Scalar) output 0
- `Alpha` <= [167] Step output 0

### [76] AnisotropyTangent

- `Input` <= [70] Input AnisotropyTangent (Vector3) output 0

### [79] CoatIOR

- `Input` <= [73] Lerp output 0

### [80] Input ThinFilmIOR (Scalar)

- `Preview` <= [81] 1.5 output 0

### [83] Normal

- `Input` <= [39] Input Normal (Vector3) output 0

### [85] Reroute Node (reroutes wires)

- `Input` <= [55] Input CoatWeight (Scalar) output 0

### [86] Substrate Slab BSDF - Complex

- `DiffuseAlbedo` <= [157] Reroute Node (reroutes wires) output 0
- `F0` <= [151] Substrate Thin-Film output 0
- `F90` <= [151] Substrate Thin-Film output 1
- `Roughness` <= [135] DiffuseRoughness output 0
- `Anisotropy` <= [97] SpecAnisotropy output 0
- `Normal` <= [162] Reroute Node (reroutes wires) output 0
- `Tangent` <= [100] SpecAnisoRot output 0
- `EmissiveColor` <= [103] EmissiveColor output 0
- `SecondRoughness` <= [96] SpecRoughness output 0
- `SecondRoughnessWeight` <= [133] 1 output 0

### [87] WeightedBase

- `Input` <= [16] Multiply output 0

### [88] SlabIOR

- `Input` <= [30] Saturate output 0

### [89] Metalness

- `Input` <= [25] Input Metalness (Scalar) output 0

### [90] WeightedSpec

- `Input` <= [34] Multiply output 0

### [91] SpecRoughness

- `Input` <= [35] Input SpecularRoughness (Scalar) output 0

### [92] SpecAnisotropy

- `Input` <= [7] Input SpecularAnisotropy (Scalar) output 0

### [93] SpecAnisoRot

- `Input` <= [183] Add output 0

### [101] EmissiveColor

- `Input` <= [139] Multiply output 0

### [104] BaseWeight

- `Input` <= [14] Input BaseWeight (Scalar) output 0

### [105] SheenWeight

- `Input` <= [45] Input SheenWeight (Scalar) output 0

### [106] SheenColor

- `Input` <= [43] Input SheenColor (Vector3) output 0

### [109] ScatterWeight

- `Input` <= [18] Input ScatterWeight (Scalar) output 0

### [110] WeightedScatter

- `Input` <= [27] Input SubsurfaceColor (Vector3) output 0

### [112] SpecF0

- `Input` <= [132] Multiply output 0

### [115] ScatterRadius

- `Input` <= [4] Input ScatterRadius (Vector3) output 0

### [116] ScatterScale

- `Input` <= [5] Input ScatterScale (Scalar) output 0

### [119] Substrate Coverage Weight

- `A` <= [2] Substrate Vertical Layer output 0
- `Weight` <= [145] GeometryOpacity output 0

### [120] Input DiffuseRoughness (Scalar)

- `Preview` <= [121] 0 output 0

### [122] DiffuseRoughness

- `Input` <= [136] Lerp(,0,) output 0

### [123] Input SheenRoughness (Scalar)

- `Preview` <= [125] 0 output 0

### [124] SheenRoughness

- `Input` <= [123] Input SheenRoughness (Scalar) output 0

### [129] Lerp

- `A` <= [128] WeightedBase output 0
- `B` <= [130] WeightedScatter output 0
- `Alpha` <= [131] ScatterWeight output 0

### [132] Multiply

- `A` <= [75] MF_SchlickInternal output 0
- `B` <= [34] Multiply output 0

### [136] Lerp(,0,)

- `A` <= [120] Input DiffuseRoughness (Scalar) output 0
- `Alpha` <= [25] Input Metalness (Scalar) output 0

### [137] Input EmissionWeight (Scalar)

- `Preview` <= [138] 0 output 0

### [139] Multiply

- `A` <= [9] Input EmissiveColor (Vector3) output 0
- `B` <= [137] Input EmissionWeight (Scalar) output 0

### [141] Substrate Transmittance-To-MeanFreePath

- `TransmittanceColor` <= [68] Input CoatColor (Vector3) output 0

### [142] Input Opacity (Scalar)

- `Preview` <= [143] 1 output 0

### [144] GeometryOpacity

- `Input` <= [142] Input Opacity (Scalar) output 0

### [146] Input ScatterAnisotropy (Scalar)

- `Preview` <= [147] 0 output 0

### [148] ScatterAniso

- `Input` <= [146] Input ScatterAnisotropy (Scalar) output 0

### [150] World Space to Tangent Space TransformVector

- `Input` <= [71] VertexTangentWS output 0

### [151] Substrate Thin-Film

- `Normal` <= [98] Normal output 0
- `F0` <= [156] Substrate Metalness-To-DiffuseAlbedo-F0 output 1
- `F90` <= [152] WeightedSpec output 0
- `Thickness` <= [161] FilmThickness output 0
- `IOR` <= [160] FilmIOR output 0

### [156] Substrate Metalness-To-DiffuseAlbedo-F0

- `BaseColor` <= [153] WeightedBase output 0
- `Metallic` <= [154] Metalness output 0
- `Specular` <= [155] SlabIOR output 0

### [157] Reroute Node (reroutes wires)

- `Input` <= [156] Substrate Metalness-To-DiffuseAlbedo-F0 output 0

### [158] FilmThickness

- `Input` <= [41] Input ThinFilmThickness (Scalar) output 0

### [159] FilmIOR

- `Input` <= [80] Input ThinFilmIOR (Scalar) output 0

### [162] Reroute Node (reroutes wires)

- `Input` <= [98] Normal output 0

### [165] Substrate Thin-Film

- `Normal` <= [84] Normal output 0
- `F0` <= [114] SpecF0 output 0
- `F90` <= [111] WeightedSpec output 0
- `Thickness` <= [163] FilmThickness output 0
- `IOR` <= [164] FilmIOR output 0

### [166] Reroute Node (reroutes wires)

- `Input` <= [129] Lerp output 0

### [167] Step

- `Y` <= [168] 1e-06 output 0
- `X` <= [55] Input CoatWeight (Scalar) output 0

### [169] Substrate Horizontal Blend (Parameter Blend)

- `Background` <= [1] Substrate Slab BSDF - Complex output 0
- `Foreground` <= [86] Substrate Slab BSDF - Complex output 0
- `Mix` <= [134] Metalness output 0

### [170] Cosine

- `Input` <= [65] Input CoatRotation (Scalar) output 0

### [171] Sine

- `Input` <= [65] Input CoatRotation (Scalar) output 0

### [173] Cross

- `A` <= [172] VertexNormalWS output 0
- `B` <= [77] AnisotropyTangent output 0

### [174] Normalize

- `VectorInput` <= [173] Cross output 0

### [175] Add

- `A` <= [177] Multiply output 0
- `B` <= [176] Multiply output 0

### [176] Multiply

- `A` <= [77] AnisotropyTangent output 0
- `B` <= [170] Cosine output 0

### [177] Multiply

- `A` <= [171] Sine output 0
- `B` <= [174] Normalize output 0

### [178] Cosine

- `Input` <= [8] Input AnisotropyRotation (Scalar) output 0

### [179] Sine

- `Input` <= [8] Input AnisotropyRotation (Scalar) output 0

### [181] Cross

- `A` <= [180] VertexNormalWS output 0
- `B` <= [78] AnisotropyTangent output 0

### [182] Normalize

- `VectorInput` <= [181] Cross output 0

### [183] Add

- `A` <= [185] Multiply output 0
- `B` <= [184] Multiply output 0

### [184] Multiply

- `A` <= [78] AnisotropyTangent output 0
- `B` <= [178] Cosine output 0

### [185] Multiply

- `A` <= [179] Sine output 0
- `B` <= [182] Normalize output 0

### [186] Output Geometry Opacity

- `A` <= [145] GeometryOpacity output 0

## Connection List

- [119] Substrate Coverage Weight output 0 -> Output Substrate StandardSurface Opaque.A
- [166] Reroute Node (reroutes wires) output 0 -> Substrate Slab BSDF - Complex.DiffuseAlbedo
- [165] Substrate Thin-Film output 0 -> Substrate Slab BSDF - Complex.F0
- [165] Substrate Thin-Film output 1 -> Substrate Slab BSDF - Complex.F90
- [126] DiffuseRoughness output 0 -> Substrate Slab BSDF - Complex.Roughness
- [94] SpecAnisotropy output 0 -> Substrate Slab BSDF - Complex.Anisotropy
- [84] Normal output 0 -> Substrate Slab BSDF - Complex.Normal
- [95] SpecAnisoRot output 0 -> Substrate Slab BSDF - Complex.Tangent
- [140] ScatterRadius output 0 -> Substrate Slab BSDF - Complex.SSSMFP
- [117] ScatterScale output 0 -> Substrate Slab BSDF - Complex.SSSMFPScale
- [149] ScatterAniso output 0 -> Substrate Slab BSDF - Complex.SSSPhaseAnisotropy
- [102] EmissiveColor output 0 -> Substrate Slab BSDF - Complex.EmissiveColor
- [99] SpecRoughness output 0 -> Substrate Slab BSDF - Complex.SecondRoughness
- [118] 1 output 0 -> Substrate Slab BSDF - Complex.SecondRoughnessWeight
- [127] SheenRoughness output 0 -> Substrate Slab BSDF - Complex.FuzzRoughness
- [107] SheenWeight output 0 -> Substrate Slab BSDF - Complex.FuzzAmount
- [108] SheenColor output 0 -> Substrate Slab BSDF - Complex.FuzzColor
- [57] Substrate Coverage Weight output 0 -> Substrate Vertical Layer.Top
- [169] Substrate Horizontal Blend (Parameter Blend) output 0 -> Substrate Vertical Layer.Base
- [141] Substrate Transmittance-To-MeanFreePath output 1 -> Substrate Vertical Layer.Thickness
- [17] 0.18,0.18,0.18 output 0 -> Input BaseColor (Vector3).Preview
- [11] 0,0,0 output 0 -> Input ScatterRadius (Vector3).Preview
- [13] 1 output 0 -> Input ScatterScale (Scalar).Preview
- [31] 1,1,1 output 0 -> Input SpecularColor (Vector3).Preview
- [37] 0 output 0 -> Input SpecularAnisotropy (Scalar).Preview
- [38] 0 output 0 -> Input AnisotropyRotation (Scalar).Preview
- [12] 0,0,0 output 0 -> Input EmissiveColor (Vector3).Preview
- [15] 1 output 0 -> Input BaseWeight (Scalar).Preview
- [14] Input BaseWeight (Scalar) output 0 -> Multiply.A
- [3] Input BaseColor (Vector3) output 0 -> Multiply.B
- [19] 0 output 0 -> Input ScatterWeight (Scalar).Preview
- [21] 1.5 output 0 -> Input IOR (Scalar).Preview
- [23] 0 output 0 -> InvLerp.A
- [24] 0.08 output 0 -> InvLerp.B
- [75] MF_SchlickInternal output 0 -> InvLerp.Value
- [26] 0 output 0 -> Input Metalness (Scalar).Preview
- [28] 0,0,0 output 0 -> Input SubsurfaceColor (Vector3).Preview
- [113] 0,0,0 output 0 -> Substrate Slab BSDF - Complex.DiffuseAlbedo
- [74] MF_SchlickApprox output 0 -> Substrate Slab BSDF - Complex.F0
- [58] 1,1,1 output 0 -> Substrate Slab BSDF - Complex.F90
- [62] Input CoatRoughness (Scalar) output 0 -> Substrate Slab BSDF - Complex.Roughness
- [64] Input CoatAnisotropy (Scalar) output 0 -> Substrate Slab BSDF - Complex.Anisotropy
- [53] Input CoatNormal (Vector3) output 0 -> Substrate Slab BSDF - Complex.Normal
- [175] Add output 0 -> Substrate Slab BSDF - Complex.Tangent
- [141] Substrate Transmittance-To-MeanFreePath output 0 -> Substrate Slab BSDF - Complex.SSSMFP
- [22] InvLerp output 0 -> Saturate.Input
- [33] 1 output 0 -> Input SpecularWeight (Scalar).Preview
- [6] Input SpecularColor (Vector3) output 0 -> Multiply.A
- [32] Input SpecularWeight (Scalar) output 0 -> Multiply.B
- [36] 0.5 output 0 -> Input SpecularRoughness (Scalar).Preview
- [40] 0,0,1 output 0 -> Input Normal (Vector3).Preview
- [42] 0 output 0 -> Input ThinFilmThickness (Scalar).Preview
- [44] 1,1,1 output 0 -> Input SheenColor (Vector3).Preview
- [46] 0 output 0 -> Input SheenWeight (Scalar).Preview
- [54] 0,0,1 output 0 -> Input CoatNormal (Vector3).Preview
- [56] 0 output 0 -> Input CoatWeight (Scalar).Preview
- [29] Substrate Slab BSDF - Complex output 0 -> Substrate Coverage Weight.A
- [85] Reroute Node (reroutes wires) output 0 -> Substrate Coverage Weight.Weight
- [60] 1.5 output 0 -> Input CoatIOR (Scalar).Preview
- [63] 0.5 output 0 -> Input CoatRoughness (Scalar).Preview
- [66] 0 output 0 -> Input CoatAnisotropy (Scalar).Preview
- [67] 0 output 0 -> Input CoatRotation (Scalar).Preview
- [69] 1,1,1 output 0 -> Input CoatColor (Vector3).Preview
- [150] World Space to Tangent Space TransformVector output 0 -> Input AnisotropyTangent (Vector3).Preview
- [61] 1 output 0 -> Lerp.A
- [59] Input CoatIOR (Scalar) output 0 -> Lerp.B
- [167] Step output 0 -> Lerp.Alpha
- [70] Input AnisotropyTangent (Vector3) output 0 -> AnisotropyTangent.Input
- [73] Lerp output 0 -> CoatIOR.Input
- [81] 1.5 output 0 -> Input ThinFilmIOR (Scalar).Preview
- [39] Input Normal (Vector3) output 0 -> Normal.Input
- [55] Input CoatWeight (Scalar) output 0 -> Reroute Node (reroutes wires).Input
- [157] Reroute Node (reroutes wires) output 0 -> Substrate Slab BSDF - Complex.DiffuseAlbedo
- [151] Substrate Thin-Film output 0 -> Substrate Slab BSDF - Complex.F0
- [151] Substrate Thin-Film output 1 -> Substrate Slab BSDF - Complex.F90
- [135] DiffuseRoughness output 0 -> Substrate Slab BSDF - Complex.Roughness
- [97] SpecAnisotropy output 0 -> Substrate Slab BSDF - Complex.Anisotropy
- [162] Reroute Node (reroutes wires) output 0 -> Substrate Slab BSDF - Complex.Normal
- [100] SpecAnisoRot output 0 -> Substrate Slab BSDF - Complex.Tangent
- [103] EmissiveColor output 0 -> Substrate Slab BSDF - Complex.EmissiveColor
- [96] SpecRoughness output 0 -> Substrate Slab BSDF - Complex.SecondRoughness
- [133] 1 output 0 -> Substrate Slab BSDF - Complex.SecondRoughnessWeight
- [16] Multiply output 0 -> WeightedBase.Input
- [30] Saturate output 0 -> SlabIOR.Input
- [25] Input Metalness (Scalar) output 0 -> Metalness.Input
- [34] Multiply output 0 -> WeightedSpec.Input
- [35] Input SpecularRoughness (Scalar) output 0 -> SpecRoughness.Input
- [7] Input SpecularAnisotropy (Scalar) output 0 -> SpecAnisotropy.Input
- [183] Add output 0 -> SpecAnisoRot.Input
- [139] Multiply output 0 -> EmissiveColor.Input
- [14] Input BaseWeight (Scalar) output 0 -> BaseWeight.Input
- [45] Input SheenWeight (Scalar) output 0 -> SheenWeight.Input
- [43] Input SheenColor (Vector3) output 0 -> SheenColor.Input
- [18] Input ScatterWeight (Scalar) output 0 -> ScatterWeight.Input
- [27] Input SubsurfaceColor (Vector3) output 0 -> WeightedScatter.Input
- [132] Multiply output 0 -> SpecF0.Input
- [4] Input ScatterRadius (Vector3) output 0 -> ScatterRadius.Input
- [5] Input ScatterScale (Scalar) output 0 -> ScatterScale.Input
- [2] Substrate Vertical Layer output 0 -> Substrate Coverage Weight.A
- [145] GeometryOpacity output 0 -> Substrate Coverage Weight.Weight
- [121] 0 output 0 -> Input DiffuseRoughness (Scalar).Preview
- [136] Lerp(,0,) output 0 -> DiffuseRoughness.Input
- [125] 0 output 0 -> Input SheenRoughness (Scalar).Preview
- [123] Input SheenRoughness (Scalar) output 0 -> SheenRoughness.Input
- [128] WeightedBase output 0 -> Lerp.A
- [130] WeightedScatter output 0 -> Lerp.B
- [131] ScatterWeight output 0 -> Lerp.Alpha
- [75] MF_SchlickInternal output 0 -> Multiply.A
- [34] Multiply output 0 -> Multiply.B
- [120] Input DiffuseRoughness (Scalar) output 0 -> Lerp(,0,).A
- [25] Input Metalness (Scalar) output 0 -> Lerp(,0,).Alpha
- [138] 0 output 0 -> Input EmissionWeight (Scalar).Preview
- [9] Input EmissiveColor (Vector3) output 0 -> Multiply.A
- [137] Input EmissionWeight (Scalar) output 0 -> Multiply.B
- [68] Input CoatColor (Vector3) output 0 -> Substrate Transmittance-To-MeanFreePath.TransmittanceColor
- [143] 1 output 0 -> Input Opacity (Scalar).Preview
- [142] Input Opacity (Scalar) output 0 -> GeometryOpacity.Input
- [147] 0 output 0 -> Input ScatterAnisotropy (Scalar).Preview
- [146] Input ScatterAnisotropy (Scalar) output 0 -> ScatterAniso.Input
- [71] VertexTangentWS output 0 -> World Space to Tangent Space TransformVector.Input
- [98] Normal output 0 -> Substrate Thin-Film.Normal
- [156] Substrate Metalness-To-DiffuseAlbedo-F0 output 1 -> Substrate Thin-Film.F0
- [152] WeightedSpec output 0 -> Substrate Thin-Film.F90
- [161] FilmThickness output 0 -> Substrate Thin-Film.Thickness
- [160] FilmIOR output 0 -> Substrate Thin-Film.IOR
- [153] WeightedBase output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.BaseColor
- [154] Metalness output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Metallic
- [155] SlabIOR output 0 -> Substrate Metalness-To-DiffuseAlbedo-F0.Specular
- [156] Substrate Metalness-To-DiffuseAlbedo-F0 output 0 -> Reroute Node (reroutes wires).Input
- [41] Input ThinFilmThickness (Scalar) output 0 -> FilmThickness.Input
- [80] Input ThinFilmIOR (Scalar) output 0 -> FilmIOR.Input
- [98] Normal output 0 -> Reroute Node (reroutes wires).Input
- [84] Normal output 0 -> Substrate Thin-Film.Normal
- [114] SpecF0 output 0 -> Substrate Thin-Film.F0
- [111] WeightedSpec output 0 -> Substrate Thin-Film.F90
- [163] FilmThickness output 0 -> Substrate Thin-Film.Thickness
- [164] FilmIOR output 0 -> Substrate Thin-Film.IOR
- [129] Lerp output 0 -> Reroute Node (reroutes wires).Input
- [168] 1e-06 output 0 -> Step.Y
- [55] Input CoatWeight (Scalar) output 0 -> Step.X
- [1] Substrate Slab BSDF - Complex output 0 -> Substrate Horizontal Blend (Parameter Blend).Background
- [86] Substrate Slab BSDF - Complex output 0 -> Substrate Horizontal Blend (Parameter Blend).Foreground
- [134] Metalness output 0 -> Substrate Horizontal Blend (Parameter Blend).Mix
- [65] Input CoatRotation (Scalar) output 0 -> Cosine.Input
- [65] Input CoatRotation (Scalar) output 0 -> Sine.Input
- [172] VertexNormalWS output 0 -> Cross.A
- [77] AnisotropyTangent output 0 -> Cross.B
- [173] Cross output 0 -> Normalize.VectorInput
- [177] Multiply output 0 -> Add.A
- [176] Multiply output 0 -> Add.B
- [77] AnisotropyTangent output 0 -> Multiply.A
- [170] Cosine output 0 -> Multiply.B
- [171] Sine output 0 -> Multiply.A
- [174] Normalize output 0 -> Multiply.B
- [8] Input AnisotropyRotation (Scalar) output 0 -> Cosine.Input
- [8] Input AnisotropyRotation (Scalar) output 0 -> Sine.Input
- [180] VertexNormalWS output 0 -> Cross.A
- [78] AnisotropyTangent output 0 -> Cross.B
- [181] Cross output 0 -> Normalize.VectorInput
- [185] Multiply output 0 -> Add.A
- [184] Multiply output 0 -> Add.B
- [78] AnisotropyTangent output 0 -> Multiply.A
- [178] Cosine output 0 -> Multiply.B
- [179] Sine output 0 -> Multiply.A
- [182] Normalize output 0 -> Multiply.B
- [145] GeometryOpacity output 0 -> Output Geometry Opacity.A

## Output Trace Roots

- `Substrate StandardSurface Opaque` is driven by [119] Substrate Coverage Weight
- `Geometry Opacity` is driven by [145] GeometryOpacity

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
