# Strata

Engine: Unreal Engine 5.3
Function count: 17

## Agent Use

Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.

Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.

## Functions

### MF_SchlickApprox

- Path: `/Engine/Functions/Strata/MF_SchlickApprox.MF_SchlickApprox`
- Deep graph doc: [MF_SchlickApprox](Functions/Strata__MF_SchlickApprox.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Input pins: IOR [FunctionInput_Scalar]
- Output pins: Result

### MF_SchlickApprox1

- Path: `/Engine/Functions/Strata/MF_SchlickApprox1.MF_SchlickApprox1`
- Deep graph doc: [MF_SchlickApprox1](Functions/Strata__MF_SchlickApprox1.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Input pins: IOR [FunctionInput_Scalar]
- Output pins: Result

### MF_SchlickInternal

- Path: `/Engine/Functions/Strata/MF_SchlickInternal.MF_SchlickInternal`
- Deep graph doc: [MF_SchlickInternal](Functions/Strata__MF_SchlickInternal.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Input pins: In_IOR [FunctionInput_Scalar], Out_IOR [FunctionInput_Scalar]
- Output pins: Result

### MF_SchlickInternal1

- Path: `/Engine/Functions/Strata/MF_SchlickInternal1.MF_SchlickInternal1`
- Deep graph doc: [MF_SchlickInternal1](Functions/Strata__MF_SchlickInternal1.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Input pins: In_IOR [FunctionInput_Scalar], Out_IOR [FunctionInput_Scalar]
- Output pins: Result

### MF_SmoothMax

- Path: `/Engine/Functions/Strata/MF_SmoothMax.MF_SmoothMax`
- Deep graph doc: [MF_SmoothMax](Functions/Strata__MF_SmoothMax.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 8
- Input pins: A [FunctionInput_Scalar], B [FunctionInput_Scalar], distance [FunctionInput_Scalar]
- Output pins: SmoothMax

### MF_SmoothMax1

- Path: `/Engine/Functions/Strata/MF_SmoothMax1.MF_SmoothMax1`
- Deep graph doc: [MF_SmoothMax1](Functions/Strata__MF_SmoothMax1.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 8
- Input pins: A [FunctionInput_Scalar], B [FunctionInput_Scalar], distance [FunctionInput_Scalar]
- Output pins: SmoothMax

### MF_SmoothMin

- Path: `/Engine/Functions/Strata/MF_SmoothMin.MF_SmoothMin`
- Deep graph doc: [MF_SmoothMin](Functions/Strata__MF_SmoothMin.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 18
- Input pins: A [FunctionInput_Scalar], B [FunctionInput_Scalar], distance [FunctionInput_Scalar]
- Output pins: SmoothMin

### MF_SmoothMin1

- Path: `/Engine/Functions/Strata/MF_SmoothMin1.MF_SmoothMin1`
- Deep graph doc: [MF_SmoothMin1](Functions/Strata__MF_SmoothMin1.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 18
- Input pins: A [FunctionInput_Scalar], B [FunctionInput_Scalar], distance [FunctionInput_Scalar]
- Output pins: SmoothMin

### SMF_Coated

- Path: `/Engine/Functions/Strata/SMF_Coated.SMF_Coated`
- Deep graph doc: [SMF_Coated](Functions/Strata__SMF_Coated.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 13
- Outputs: 1
- Internal nodes: 28
- Input pins: Base Color [FunctionInput_Vector3], Metallic [FunctionInput_Scalar], Specular [FunctionInput_Scalar], Roughness [FunctionInput_Scalar], Normal [FunctionInput_Vector3], Coat Color [FunctionInput_Vector3], Coat Roughness [FunctionInput_Scalar], Coat Specular [FunctionInput_Scalar], Coat Normal [FunctionInput_Vector3], Coat Emissive Color [FunctionInput_Vector3], Emissive Color [FunctionInput_Vector3], Thickness factor [FunctionInput_Scalar]
- Output pins: Result

### SMF_UE4Disney

- Path: `/Engine/Functions/Strata/SMF_UE4Disney.SMF_UE4Disney`
- Deep graph doc: [SMF_UE4Disney](Functions/Strata__SMF_UE4Disney.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 1
- Internal nodes: 14
- Input pins: Base Color [FunctionInput_Vector3], Metallic [FunctionInput_Scalar], Specular [FunctionInput_Scalar], Roughness [FunctionInput_Scalar], Normal [FunctionInput_Vector3], Emissive Color [FunctionInput_Vector3], Opacity [FunctionInput_Scalar]
- Output pins: Result

### SMF_UE4Unlit

- Path: `/Engine/Functions/Strata/SMF_UE4Unlit.SMF_UE4Unlit`
- Deep graph doc: [SMF_UE4Unlit](Functions/Strata__SMF_UE4Unlit.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Unlit material with Opacity input.
- Input pins: Color [FunctionInput_Vector3], Opacity [FunctionInput_Scalar]
- Output pins: Result

### Substrate-FlipFlop

- Path: `/Engine/Functions/Strata/Substrate-FlipFlop.Substrate-FlipFlop`
- Deep graph doc: [Substrate-FlipFlop](Functions/Strata__Substrate-FlipFlop.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 2
- Internal nodes: 16
- Input pins: F0 [FunctionInput_Vector3], F90 [FunctionInput_Vector3], Falloff [FunctionInput_Scalar], WorldNormal [FunctionInput_Vector3]
- Output pins: Result, LerpFactor

### Substrate-IOR-To-F0

- Path: `/Engine/Functions/Strata/Substrate-IOR-To-F0.Substrate-IOR-To-F0`
- Deep graph doc: [Substrate-IOR-To-F0](Functions/Strata__Substrate-IOR-To-F0.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Convert a dieletric IOR into a F0 value.
- Input pins: IOR [FunctionInput_Scalar]
- Output pins: F0

### Substrate-Rotation-To-Tangent

- Path: `/Engine/Functions/Strata/Substrate-Rotation-To-Tangent.Substrate-Rotation-To-Tangent`
- Deep graph doc: [Substrate-Rotation-To-Tangent](Functions/Strata__Substrate-Rotation-To-Tangent.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 6
- Description: Convert a rotation angle into a tangent vector
- Input pins: Rotation [FunctionInput_Scalar]
- Output pins: Tangent

### Substrate-StandardSurface-Opaque

- Path: `/Engine/Functions/Strata/Substrate-StandardSurface-Opaque.Substrate-StandardSurface-Opaque`
- Deep graph doc: [Substrate-StandardSurface-Opaque](Functions/Strata__Substrate-StandardSurface-Opaque.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 40
- Outputs: 2
- Internal nodes: 187
- Description: Opaque Standard Surface
- Input pins: BaseColor [FunctionInput_Vector3], ScatterRadius [FunctionInput_Vector3], ScatterScale [FunctionInput_Scalar], SpecularColor [FunctionInput_Vector3], SpecularAnisotropy [FunctionInput_Scalar], AnisotropyRotation [FunctionInput_Scalar], EmissiveColor [FunctionInput_Vector3], ---- Coat ---- [FunctionInput_StaticBool], BaseWeight [FunctionInput_Scalar], ScatterWeight [FunctionInput_Scalar], IOR [FunctionInput_Scalar], Metalness [FunctionInput_Scalar]
- Output pins: Substrate StandardSurface Opaque, Geometry Opacity

### Substrate-StandardSurface-Translucent

- Path: `/Engine/Functions/Strata/Substrate-StandardSurface-Translucent.Substrate-StandardSurface-Translucent`
- Deep graph doc: [Substrate-StandardSurface-Translucent](Functions/Strata__Substrate-StandardSurface-Translucent.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 40
- Outputs: 1
- Internal nodes: 191
- Description: Translucent Standard Surface
- Input pins: BaseColor [FunctionInput_Vector3], TransmissionColor [FunctionInput_Vector3], SpecularColor [FunctionInput_Vector3], SpecularAnisotropy [FunctionInput_Scalar], AnisotropyRotation [FunctionInput_Scalar], EmissiveColor [FunctionInput_Vector3], ---- Coat ---- [FunctionInput_StaticBool], TransmissionDepth [FunctionInput_Scalar], BaseWeight [FunctionInput_Scalar], TransmissionWeight [FunctionInput_Scalar], IOR [FunctionInput_Scalar], Metalness [FunctionInput_Scalar]
- Output pins: Substrate_StandardTranslucent

### Substrate-View-Dependent-Coverage

- Path: `/Engine/Functions/Strata/Substrate-View-Dependent-Coverage.Substrate-View-Dependent-Coverage`
- Deep graph doc: [Substrate-View-Dependent-Coverage](Functions/Strata__Substrate-View-Dependent-Coverage.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 14
- Input pins: Thickness [FunctionInput_Scalar], Coverage [FunctionInput_Scalar], Normal [FunctionInput_Vector3]
- Output pins: Thickness, Coverage



