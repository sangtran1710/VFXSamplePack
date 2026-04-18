# MaterialLayerFunctions

Engine: Unreal Engine 5.3
Function count: 38

## Agent Use

Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.

Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.

## Functions

### MatLayerBlend_AddWorldPositionOffset

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_AddWorldPositionOffset.MatLayerBlend_AddWorldPositionOffset`
- Deep graph doc: [MatLayerBlend_AddWorldPositionOffset](Functions/MaterialLayerFunctions__MatLayerBlend_AddWorldPositionOffset.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Adds WorldPositionOffset
- Input pins: Material [FunctionInput_MaterialAttributes], WorldPositionOffset [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_AO

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_AO.MatLayerBlend_AO`
- Deep graph doc: [MatLayerBlend_AO](Functions/MaterialLayerFunctions__MatLayerBlend_AO.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 15
- Description: Blends an AO map to remove reflection.
- Input pins: AO [FunctionInput_Scalar], Material [FunctionInput_MaterialAttributes]
- Output pins: Blended Material

### MatLayerBlend_BakedNormal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BakedNormal.MatLayerBlend_BakedNormal`
- Deep graph doc: [MatLayerBlend_BakedNormal](Functions/MaterialLayerFunctions__MatLayerBlend_BakedNormal.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 11
- Description: Blends a Processed Normal Map with layered normals
- Input pins: Material [FunctionInput_MaterialAttributes], Normal [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_BakedNormal_SimpleAdd

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BakedNormal_SimpleAdd.MatLayerBlend_BakedNormal_SimpleAdd`
- Deep graph doc: [MatLayerBlend_BakedNormal_SimpleAdd](Functions/MaterialLayerFunctions__MatLayerBlend_BakedNormal_SimpleAdd.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 10
- Description: Blends a Processed Normal Map with layered normals
- Input pins: Material [FunctionInput_MaterialAttributes], Normal [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_BlendAngleCorrectedNormals

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BlendAngleCorrectedNormals.MatLayerBlend_BlendAngleCorrectedNormals`
- Deep graph doc: [MatLayerBlend_BlendAngleCorrectedNormals](Functions/MaterialLayerFunctions__MatLayerBlend_BlendAngleCorrectedNormals.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 8
- Description: Replaces WorldPositionDisplacement
- Input pins: Material [FunctionInput_MaterialAttributes], Additional Normal [FunctionInput_Vector3], Mask [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_BreakBaseColor

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BreakBaseColor.MatLayerBlend_BreakBaseColor`
- Deep graph doc: [MatLayerBlend_BreakBaseColor](Functions/MaterialLayerFunctions__MatLayerBlend_BreakBaseColor.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Replace the Base Color
- Input pins: Material [FunctionInput_MaterialAttributes]
- Output pins: BaseColor

### MatLayerBlend_BreakNormal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_BreakNormal.MatLayerBlend_BreakNormal`
- Deep graph doc: [MatLayerBlend_BreakNormal](Functions/MaterialLayerFunctions__MatLayerBlend_BreakNormal.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Break out the Normal
- Input pins: Material [FunctionInput_MaterialAttributes]
- Output pins: Normal

### MatLayerBlend_Decal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Decal.MatLayerBlend_Decal`
- Deep graph doc: [MatLayerBlend_Decal](Functions/MaterialLayerFunctions__MatLayerBlend_Decal.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 17
- Description: Blends in a decal sheet using the 2nd uv channel
- Input pins: Base Material [FunctionInput_MaterialAttributes], DecalRoughness [FunctionInput_Scalar], DecalSheet [FunctionInput_Texture2D], DecalNormalFlatness [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_Decal_UV3

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Decal_UV3.MatLayerBlend_Decal_UV3`
- Deep graph doc: [MatLayerBlend_Decal_UV3](Functions/MaterialLayerFunctions__MatLayerBlend_Decal_UV3.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 16
- Description: Blends in a decal sheet using the 2nd uv channel
- Input pins: Base Material [FunctionInput_MaterialAttributes], DecalRoughness [FunctionInput_Scalar], DecalSheet [FunctionInput_Texture2D], DecalNormalFlatness [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_Displacement

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Displacement.MatLayerBlend_Displacement`
- Deep graph doc: [MatLayerBlend_Displacement](Functions/MaterialLayerFunctions__MatLayerBlend_Displacement.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Adds Displacement
- Input pins: Material [FunctionInput_MaterialAttributes], Displacement [FunctionInput_Vector3], Tessellation Multiplier [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_Emissive

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Emissive.MatLayerBlend_Emissive`
- Deep graph doc: [MatLayerBlend_Emissive](Functions/MaterialLayerFunctions__MatLayerBlend_Emissive.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Adds Emissive
- Input pins: Material [FunctionInput_MaterialAttributes], Emissive [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_LightmassReplace

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_LightmassReplace.MatLayerBlend_LightmassReplace`
- Deep graph doc: [MatLayerBlend_LightmassReplace](Functions/MaterialLayerFunctions__MatLayerBlend_LightmassReplace.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Replace the Base Color in Lightmass
- Input pins: Material [FunctionInput_MaterialAttributes], LightmassReplaceColor [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_ModulateRoughness

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_ModulateRoughness.MatLayerBlend_ModulateRoughness`
- Deep graph doc: [MatLayerBlend_ModulateRoughness](Functions/MaterialLayerFunctions__MatLayerBlend_ModulateRoughness.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Modulates the roughness value, useful for greasy look.
- Input pins: Base Material [FunctionInput_MaterialAttributes], RoughnessMultiplier [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_ModulateSpecular

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_ModulateSpecular.MatLayerBlend_ModulateSpecular`
- Deep graph doc: [MatLayerBlend_ModulateSpecular](Functions/MaterialLayerFunctions__MatLayerBlend_ModulateSpecular.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Modulates the Specular value
- Input pins: Base Material [FunctionInput_MaterialAttributes], SpecularMultiplier [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_Multiply

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Multiply.MatLayerBlend_Multiply`
- Deep graph doc: [MatLayerBlend_Multiply](Functions/MaterialLayerFunctions__MatLayerBlend_Multiply.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 19
- Description: Multiplies all elements of Material Attributes by Multiplier
- Input pins: Base Material [FunctionInput_MaterialAttributes], Multiplier [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_MultiplyBaseColor

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_MultiplyBaseColor.MatLayerBlend_MultiplyBaseColor`
- Deep graph doc: [MatLayerBlend_MultiplyBaseColor](Functions/MaterialLayerFunctions__MatLayerBlend_MultiplyBaseColor.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 10
- Description: Multiply the Base Color by a Color
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewBaseColor [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_NormalBlend

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_NormalBlend.MatLayerBlend_NormalBlend`
- Deep graph doc: [MatLayerBlend_NormalBlend](Functions/MaterialLayerFunctions__MatLayerBlend_NormalBlend.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 10
- Description: Blends a Normalmap on top of a material using a mask.
- Input pins: Material [FunctionInput_MaterialAttributes], NormalMask [FunctionInput_Scalar], Normal [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_NormalFlatten

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_NormalFlatten.MatLayerBlend_NormalFlatten`
- Deep graph doc: [MatLayerBlend_NormalFlatten](Functions/MaterialLayerFunctions__MatLayerBlend_NormalFlatten.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Diminishes the effect of a normalmap.
- Input pins: NormalFlatness [FunctionInput_Scalar], Material [FunctionInput_MaterialAttributes], Normal [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_OverrideBaseColor

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideBaseColor.MatLayerBlend_OverrideBaseColor`
- Deep graph doc: [MatLayerBlend_OverrideBaseColor](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideBaseColor.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 9
- Description: Replace the Base Color
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewBaseColor [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_OverrideDisplacement

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideDisplacement.MatLayerBlend_OverrideDisplacement`
- Deep graph doc: [MatLayerBlend_OverrideDisplacement](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideDisplacement.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 9
- Description: Replaces WorldPositionDisplacement
- Input pins: Material [FunctionInput_MaterialAttributes], WorldPositionDisplacement [FunctionInput_Vector3], Mask [FunctionInput_Scalar], TessellationMultiplier [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_OverrideMetalness

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideMetalness.MatLayerBlend_OverrideMetalness`
- Deep graph doc: [MatLayerBlend_OverrideMetalness](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideMetalness.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 9
- Description: Replace the Metalness
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewMetalness [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_OverrideOpacity

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideOpacity.MatLayerBlend_OverrideOpacity`
- Deep graph doc: [MatLayerBlend_OverrideOpacity](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideOpacity.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Replaced the Opacity
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewOpacity [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_OverrideOpacityMask

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideOpacityMask.MatLayerBlend_OverrideOpacityMask`
- Deep graph doc: [MatLayerBlend_OverrideOpacityMask](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideOpacityMask.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Replaced the Opacity Mask
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewOpacityMask [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_OverrideSubSurface

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideSubSurface.MatLayerBlend_OverrideSubSurface`
- Deep graph doc: [MatLayerBlend_OverrideSubSurface](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideSubSurface.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 9
- Description: Replace the Subsurface Color and Opacity
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewSubSurfaceColor [FunctionInput_Vector3], NewOpacity [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_OverrideWorldPositionOffset

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_OverrideWorldPositionOffset.MatLayerBlend_OverrideWorldPositionOffset`
- Deep graph doc: [MatLayerBlend_OverrideWorldPositionOffset](Functions/MaterialLayerFunctions__MatLayerBlend_OverrideWorldPositionOffset.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Replaces WorldPositionOffset
- Input pins: Material [FunctionInput_MaterialAttributes], WorldPositionOffset [FunctionInput_Vector3], Mask [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_ReplaceNormals

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_ReplaceNormals.MatLayerBlend_ReplaceNormals`
- Deep graph doc: [MatLayerBlend_ReplaceNormals](Functions/MaterialLayerFunctions__MatLayerBlend_ReplaceNormals.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 5
- Input pins: Material [FunctionInput_MaterialAttributes], Normal [FunctionInput_Vector3]
- Output pins: Result

### MatLayerBlend_RoughnessOverride

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_RoughnessOverride.MatLayerBlend_RoughnessOverride`
- Deep graph doc: [MatLayerBlend_RoughnessOverride](Functions/MaterialLayerFunctions__MatLayerBlend_RoughnessOverride.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Replace the Roughness
- Input pins: Material [FunctionInput_MaterialAttributes], Mask [FunctionInput_Scalar], NewRoughness [FunctionInput_Vector3]
- Output pins: Blended Material

### MatLayerBlend_SeparateNormalandColorClamps

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_SeparateNormalandColorClamps.MatLayerBlend_SeparateNormalandColorClamps`
- Deep graph doc: [MatLayerBlend_SeparateNormalandColorClamps](Functions/MaterialLayerFunctions__MatLayerBlend_SeparateNormalandColorClamps.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 2
- Internal nodes: 26
- Description: Blends all attributes of 2 Materials
- Input pins: Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes], Base Material [FunctionInput_MaterialAttributes], Color Blend Min [FunctionInput_Scalar], Color Blend Max [FunctionInput_Scalar], Normal Blend Min [FunctionInput_Scalar], Normal Blend Max [FunctionInput_Scalar]
- Output pins: Blended Material, Displacement

### MatLayerBlend_Simple

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Simple.MatLayerBlend_Simple`
- Deep graph doc: [MatLayerBlend_Simple](Functions/MaterialLayerFunctions__MatLayerBlend_Simple.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 11
- Description: Blends all attributes of 2 Materials except Normal.
- Input pins: Base Material [FunctionInput_MaterialAttributes], Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes]
- Output pins: Blended Material

### MatLayerBlend_Stain

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Stain.MatLayerBlend_Stain`
- Deep graph doc: [MatLayerBlend_Stain](Functions/MaterialLayerFunctions__MatLayerBlend_Stain.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 12
- Description: Blends the top layer as a stain, only the color and roughness from the top layer are used.
- Input pins: Base Material [FunctionInput_MaterialAttributes], Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes], StainPremultiply [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_Standard

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Standard.MatLayerBlend_Standard`
- Deep graph doc: [MatLayerBlend_Standard](Functions/MaterialLayerFunctions__MatLayerBlend_Standard.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 5
- Description: Blends all attributes of 2 Materials
- Input pins: Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes], Base Material [FunctionInput_MaterialAttributes]
- Output pins: Blended Material

### MatLayerBlend_StandardWithDisplacement

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_StandardWithDisplacement.MatLayerBlend_StandardWithDisplacement`
- Deep graph doc: [MatLayerBlend_StandardWithDisplacement](Functions/MaterialLayerFunctions__MatLayerBlend_StandardWithDisplacement.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 24
- Description: Blends all attributes of 2 Materials
- Input pins: Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes], Base Material [FunctionInput_MaterialAttributes]
- Output pins: Blended Material, Displacement

### MatLayerBlend_StandardWithMaskEdgeTint

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_StandardWithMaskEdgeTint.MatLayerBlend_StandardWithMaskEdgeTint`
- Deep graph doc: [MatLayerBlend_StandardWithMaskEdgeTint](Functions/MaterialLayerFunctions__MatLayerBlend_StandardWithMaskEdgeTint.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 2
- Internal nodes: 23
- Description: Blends all attributes of 2 Materials
- Input pins: Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes], Base Material [FunctionInput_MaterialAttributes], Tint Mask [FunctionInput_Scalar]
- Output pins: Blended Material, Displacement

### MatLayerBlend_TenLayerBlend

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_TenLayerBlend.MatLayerBlend_TenLayerBlend`
- Deep graph doc: [MatLayerBlend_TenLayerBlend](Functions/MaterialLayerFunctions__MatLayerBlend_TenLayerBlend.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 22
- Outputs: 1
- Internal nodes: 35
- Description: Fixed function blending of 10 layers.  Only uses the layers hooked up.  Compiler should optimize out unused layers.
- Input pins: Background [FunctionInput_MaterialAttributes], Layer1 [FunctionInput_MaterialAttributes], Layer2 [FunctionInput_MaterialAttributes], Layer4 [FunctionInput_MaterialAttributes], Layer3 [FunctionInput_MaterialAttributes], Layer5 [FunctionInput_MaterialAttributes], Layer6 [FunctionInput_MaterialAttributes], Layer7 [FunctionInput_MaterialAttributes], Layer10 [FunctionInput_MaterialAttributes], Layer9 [FunctionInput_MaterialAttributes], Layer8 [FunctionInput_MaterialAttributes], Layer1_Mask [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_Tint

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_Tint.MatLayerBlend_Tint`
- Deep graph doc: [MatLayerBlend_Tint](Functions/MaterialLayerFunctions__MatLayerBlend_Tint.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 8
- Description: Simple Tinting of Base Color
- Input pins: Material [FunctionInput_MaterialAttributes], Tint [FunctionInput_Vector3], TintMask [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_TintAllChannels

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_TintAllChannels.MatLayerBlend_TintAllChannels`
- Deep graph doc: [MatLayerBlend_TintAllChannels](Functions/MaterialLayerFunctions__MatLayerBlend_TintAllChannels.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 10
- Description: Simple Tinting of Base Color
- Input pins: Material [FunctionInput_MaterialAttributes], Tint [FunctionInput_Vector3], TintMask [FunctionInput_Scalar]
- Output pins: Blended Material

### MatLayerBlend_TopNormal

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_TopNormal.MatLayerBlend_TopNormal`
- Deep graph doc: [MatLayerBlend_TopNormal](Functions/MaterialLayerFunctions__MatLayerBlend_TopNormal.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 19
- Description: Blends Material Attributes but only uses the Top Normal Map
- Input pins: Alpha [FunctionInput_Scalar], Top Material [FunctionInput_MaterialAttributes], Base Material [FunctionInput_MaterialAttributes]
- Output pins: Blended Material

### ML_ExampleMaterialLayer

- Path: `/Engine/Functions/MaterialLayerFunctions/ML_ExampleMaterialLayer.ML_ExampleMaterialLayer`
- Deep graph doc: [ML_ExampleMaterialLayer](Functions/MaterialLayerFunctions__ML_ExampleMaterialLayer.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 13
- Input pins: Tint [FunctionInput_Vector3], Tiling [FunctionInput_Scalar]
- Output pins: Result

