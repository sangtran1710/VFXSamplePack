# Engine_MaterialFunctions01/Texturing

Engine: Unreal Engine 5.3
Function count: 42

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### AbberatedBlur-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/AbberatedBlur-Texture.AbberatedBlur-Texture`
- Deep graph doc: [AbberatedBlur-Texture](Functions/Engine_MaterialFunctions01_Texturing__AbberatedBlur-Texture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 8
- Description: Blurs a Texture along a specified 2D Axis with chromatic abberation.
- Input pins: TextureObject [FunctionInput_Texture2D], UV [FunctionInput_Vector2], Steps [FunctionInput_Scalar], Distance [FunctionInput_Scalar], Vector [FunctionInput_Vector2]
- Output pins: Result With Abberation

### BakedDisplacement

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/BakedDisplacement.BakedDisplacement`
- Deep graph doc: [BakedDisplacement](Functions/Engine_MaterialFunctions01_Texturing__BakedDisplacement.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: Set up a baked scalar displacement map to be used with WorldDispalcement
- Input pins: In [FunctionInput_Scalar], Baked_Min [FunctionInput_Scalar], Baked_Max [FunctionInput_Scalar]
- Output pins: Result

### BitMask

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/BitMask.BitMask`
- Deep graph doc: [BitMask](Functions/Engine_MaterialFunctions01_Texturing__BitMask.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: Provides a 1bit mask from a supplied Grayscale image and a value between 0-255
- Input pins: BitMask [FunctionInput_Scalar], Bit [FunctionInput_Scalar]
- Output pins: Result

### BrickAndTileUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/BrickAndTileUVs.BrickAndTileUVs`
- Deep graph doc: [BrickAndTileUVs](Functions/Engine_MaterialFunctions01_Texturing__BrickAndTileUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 4
- Internal nodes: 58
- Description: Bricks and Tile UVs create a semi procedural UV offset pattern useful for the creation of patterned textures.
- Input pins: Tiles X [FunctionInput_Scalar], Tiles Y [FunctionInput_Scalar], Tile Shift [FunctionInput_Scalar], Texture Tiling [FunctionInput_Scalar], Rotate 90 [FunctionInput_StaticBool], Macro Tiling [FunctionInput_Scalar], Pattern Distortion [FunctionInput_Vector2], UV [FunctionInput_Vector2]
- Output pins: DiffuseUVs, MacroUVs, TileUVs, Macro Offsets only

### CameraVectorToLatLongUV

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/CameraVectorToLatLongUV.CameraVectorToLatLongUV`
- Deep graph doc: [CameraVectorToLatLongUV](Functions/Engine_MaterialFunctions01_Texturing__CameraVectorToLatLongUV.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 9
- Description: Converts camera vector to latlong UVs. Helpful for skybox textures
- Input pins: CameraVector [FunctionInput_Vector3]
- Output pins: Uvs

### ComputeMipLevel

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ComputeMipLevel.ComputeMipLevel`
- Deep graph doc: [ComputeMipLevel](Functions/Engine_MaterialFunctions01_Texturing__ComputeMipLevel.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 15
- Input pins: Texture Size [FunctionInput_Vector2], UVs [FunctionInput_Vector2]
- Output pins: MipLevel

### CylindricalUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/CylindricalUVs.CylindricalUVs`
- Deep graph doc: [CylindricalUVs](Functions/Engine_MaterialFunctions01_Texturing__CylindricalUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 23
- Description: Tiles a texture using Cylindrical UVS centered around Object Center
- Input pins: Normal [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], In [FunctionInput_Scalar]
- Output pins: Cylinder Projection w Top, Cylinder Projection

### DeriveTangentBasis

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/DeriveTangentBasis.DeriveTangentBasis`
- Deep graph doc: [DeriveTangentBasis](Functions/Engine_MaterialFunctions01_Texturing__DeriveTangentBasis.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 32
- Description: Derive a secondary tangent basis from another UV channel.
- Input pins: UVs [FunctionInput_Vector2], Normal [FunctionInput_Vector3]
- Output pins: Tangent, World Space Normal, Binormal

### DetailTexturing

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/DetailTexturing.DetailTexturing`
- Deep graph doc: [DetailTexturing](Functions/Engine_MaterialFunctions01_Texturing__DetailTexturing.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 2
- Internal nodes: 26
- Input pins: Diffuse [FunctionInput_Vector3], Normal [FunctionInput_Vector3], Scale [FunctionInput_Scalar], NormalIntensity [FunctionInput_Scalar], DiffuseIntensity [FunctionInput_Scalar], DetailDiffuse [FunctionInput_Texture2D], DetailNormal [FunctionInput_Texture2D]
- Output pins: Diffuse, Normal

### DistanceField

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/DistanceField.DistanceField`
- Deep graph doc: [DistanceField](Functions/Engine_MaterialFunctions01_Texturing__DistanceField.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: Reads a Distance field texture
- Input pins: EdgeSoftness [FunctionInput_Scalar], DistanceField [FunctionInput_Scalar]
- Output pins: Result

### FlattenNormal

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/FlattenNormal.FlattenNormal`
- Deep graph doc: [FlattenNormal](Functions/Engine_MaterialFunctions01_Texturing__FlattenNormal.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 5
- Description: Lerps a NormalMap with 0,0,1
- Input pins: Normal [FunctionInput_Vector3], Flatness [FunctionInput_Scalar]
- Output pins: Result

### LocalAlignedTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/LocalAlignedTexture.LocalAlignedTexture`
- Deep graph doc: [LocalAlignedTexture](Functions/Engine_MaterialFunctions01_Texturing__LocalAlignedTexture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 23
- Description: Tiles a texture in worldspace
- Input pins: TextureObject [FunctionInput_Texture2D], Normal [FunctionInput_Vector3]
- Output pins: Z Texture, XY Texture, XYZ Texture

### LocalAlignedTexture_TransformedWorldSpace

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/LocalAlignedTexture_TransformedWorldSpace.LocalAlignedTexture_TransformedWorldSpace`
- Deep graph doc: [LocalAlignedTexture_TransformedWorldSpace](Functions/Engine_MaterialFunctions01_Texturing__LocalAlignedTexture_TransformedWorldSpace.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 23
- Description: Tiles a texture in worldspace
- Input pins: TextureObject [FunctionInput_Texture2D], Normal [FunctionInput_Vector3]
- Output pins: Z Texture, XY Texture, XYZ Texture

### MosaicUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/MosaicUVs.MosaicUVs`
- Deep graph doc: [MosaicUVs](Functions/Engine_MaterialFunctions01_Texturing__MosaicUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Creates a mosaic style effect where the texture will be blocks, or large pixels
- Input pins: UVs [FunctionInput_Vector2], Tiles [FunctionInput_Scalar]
- Output pins: Result

### MotionBlur-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/MotionBlur-Texture.MotionBlur-Texture`
- Deep graph doc: [MotionBlur-Texture](Functions/Engine_MaterialFunctions01_Texturing__MotionBlur-Texture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 10
- Description: Blurs a Texture along a specified 2D Axis
- Input pins: TextureObject [FunctionInput_Texture2D], UV [FunctionInput_Vector2], Steps [FunctionInput_Scalar], Distance [FunctionInput_Scalar], Vector [FunctionInput_Vector2]
- Output pins: Result, Results with Chromatic Shift

### PackedDistanceField

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/PackedDistanceField.PackedDistanceField`
- Deep graph doc: [PackedDistanceField](Functions/Engine_MaterialFunctions01_Texturing__PackedDistanceField.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 10
- Description: Reads Distance Field textures packed as a subUV
- Input pins: ImageCount [FunctionInput_Vector2], ImageNumber [FunctionInput_Scalar], PackedDistanceField [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], EdgeSoftness [FunctionInput_Scalar]
- Output pins: Result

### Parallax_For_Bomb

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/Parallax_For_Bomb.Parallax_For_Bomb`
- Deep graph doc: [Parallax_For_Bomb](Functions/Engine_MaterialFunctions01_Texturing__Parallax_For_Bomb.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 16
- Outputs: 7
- Internal nodes: 121
- Description: Parallax Occlusion Mapping uses Ray Tracing to find the correct offsets at each point. Keep in mind that the material cost will not be easily trackable using the instruction count or shader complexity view due to the nature of the loops involved. Use the debug emissive colors to get an idea of expense based on your specified Min and Max Step sizes.
- Input pins: Min Steps [FunctionInput_Scalar], Max Steps [FunctionInput_Scalar], Height Ratio [FunctionInput_Scalar], Heightmap Texture [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], ----------------------------- [FunctionInput_StaticBool], Specify Manual Texture Size [FunctionInput_StaticBool], Manual Texture Size [FunctionInput_Scalar], ------------------------------ [FunctionInput_StaticBool], Render Shadows (Occlusion Mapping) [FunctionInput_StaticBool], Shadow Steps [FunctionInput_Scalar], Shadow Penumbra [FunctionInput_Scalar]
- Output pins: Parallax UVs, Pixel Depth Offset, Offset Only, Shadow, Material Complexity - Steps Debug, Tangent Light Vector, World Position

### ParallaxOcclusionMapping

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ParallaxOcclusionMapping.ParallaxOcclusionMapping`
- Deep graph doc: [ParallaxOcclusionMapping](Functions/Engine_MaterialFunctions01_Texturing__ParallaxOcclusionMapping.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 17
- Outputs: 7
- Internal nodes: 127
- Description: Parallax Occlusion Mapping uses Ray Tracing to find the correct offsets at each point. Keep in mind that the material cost will not be easily trackable using the instruction count or shader complexity view due to the nature of the loops involved. Use the debug emissive colors to get an idea of expense based on your specified Min and Max Step sizes.
- Input pins: Min Steps [FunctionInput_Scalar], Max Steps [FunctionInput_Scalar], Height Ratio [FunctionInput_Scalar], Heightmap Texture [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], ----------------------------- [FunctionInput_StaticBool], Specify Manual Texture Size [FunctionInput_StaticBool], Manual Texture Size [FunctionInput_Scalar], ------------------------------ [FunctionInput_StaticBool], Render Shadows (Occlusion Mapping) [FunctionInput_StaticBool], Shadow Steps [FunctionInput_Scalar], Shadow Penumbra [FunctionInput_Scalar]
- Output pins: Parallax UVs, Pixel Depth Offset, Offset Only, Shadow, Material Complexity - Steps Debug, Tangent Light Vector, World Position

### PointSampledUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/PointSampledUVs.PointSampledUVs`
- Deep graph doc: [PointSampledUVs](Functions/Engine_MaterialFunctions01_Texturing__PointSampledUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: Returns UVs that are point sampled which is a way to force nearest neighbor sampling. This should only be used with a forced mip level and only in situations that require it (such as writing render targets in non power of 2 sizes).
- Input pins: UVs [FunctionInput_Vector2], Texture Size [FunctionInput_Vector2]
- Output pins: Result

### ScaleUVsAroundPoint

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ScaleUVsAroundPoint.ScaleUVsAroundPoint`
- Deep graph doc: [ScaleUVsAroundPoint](Functions/Engine_MaterialFunctions01_Texturing__ScaleUVsAroundPoint.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 20
- Input pins: Offset XY Texturesize Z [FunctionInput_Vector3], Texture Scale [FunctionInput_Vector2], UVs [FunctionInput_Vector2]
- Output pins: UVs, 0-1 mask

### ScaleUVsByCenter

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ScaleUVsByCenter.ScaleUVsByCenter`
- Deep graph doc: [ScaleUVsByCenter](Functions/Engine_MaterialFunctions01_Texturing__ScaleUVsByCenter.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 16
- Input pins: Texture Scale [FunctionInput_Vector2], UVs [FunctionInput_Vector2]
- Output pins: UVs, 0-1 mask

### SkyAtmosphereImage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SkyAtmosphereImage.SkyAtmosphereImage`
- Deep graph doc: [SkyAtmosphereImage](Functions/Engine_MaterialFunctions01_Texturing__SkyAtmosphereImage.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 12
- Input pins: Rotation (radian) [FunctionInput_Scalar], Direction [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], Scale [FunctionInput_Vector2]
- Output pins: Result, Alpha, Mask

### SkyboxImage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SkyboxImage.SkyboxImage`
- Deep graph doc: [SkyboxImage](Functions/Engine_MaterialFunctions01_Texturing__SkyboxImage.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 36
- Description: Projects an image onto an infinite sphere, useful for putting images in the skybox
- Input pins: Rotation 0-1 [FunctionInput_Scalar], ImageVector [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], Scale [FunctionInput_Vector2]
- Output pins: RGB, Mask, Alpha

### SlopeMask

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SlopeMask.SlopeMask`
- Deep graph doc: [SlopeMask](Functions/Engine_MaterialFunctions01_Texturing__SlopeMask.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 10
- Description: Slopemask, useful for stuff like snow, moss
- Input pins: TangentNormal [FunctionInput_Vector3], CheapContrast [FunctionInput_Scalar], FalloffPower [FunctionInput_Scalar], SlopAngle [FunctionInput_Vector3]
- Output pins: Result

### SpiralBlur-SceneTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SpiralBlur-SceneTexture.SpiralBlur-SceneTexture`
- Deep graph doc: [SpiralBlur-SceneTexture](Functions/Engine_MaterialFunctions01_Texturing__SpiralBlur-SceneTexture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 2
- Internal nodes: 13
- Description: Performs a Spiral Blur with controllable number of steps and offests.
- Input pins: Distance Steps [FunctionInput_Scalar], Distance [FunctionInput_Scalar], Radial Steps [FunctionInput_Scalar], Radial Offset [FunctionInput_Scalar], TempAA Radial Blur [FunctionInput_Scalar], TempAA Distance Blur [FunctionInput_Scalar], Distance Mask [FunctionInput_Scalar]
- Output pins: Result, SceneColor clamp to 0

### SpiralBlur-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/SpiralBlur-Texture.SpiralBlur-Texture`
- Deep graph doc: [SpiralBlur-Texture](Functions/Engine_MaterialFunctions01_Texturing__SpiralBlur-Texture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 1
- Internal nodes: 11
- Description: Performs a Spiral Blur with controllable number of steps and offests.
- Input pins: TextureObject [FunctionInput_Texture2D], UV [FunctionInput_Vector2], Distance Steps [FunctionInput_Scalar], Distance [FunctionInput_Scalar], Radial Steps [FunctionInput_Scalar], Radial Offset [FunctionInput_Scalar], Kernel Power [FunctionInput_Scalar]
- Output pins: Result

### TangentSpaceFlow

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TangentSpaceFlow.TangentSpaceFlow`
- Deep graph doc: [TangentSpaceFlow](Functions/Engine_MaterialFunctions01_Texturing__TangentSpaceFlow.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 46
- Description: Pushing the FlowTexture along the direction of the FlowDirection
- Input pins: FlowSpeed [FunctionInput_Scalar], FlowTexture [FunctionInput_Texture2D], FlowStrength [FunctionInput_Scalar], FlowDirection [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Export Float 4 [FunctionInput_StaticBool]
- Output pins: Result

### TangentSpaceFlowComplex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TangentSpaceFlowComplex.TangentSpaceFlowComplex`
- Deep graph doc: [TangentSpaceFlowComplex](Functions/Engine_MaterialFunctions01_Texturing__TangentSpaceFlowComplex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 4
- Internal nodes: 47
- Description: Pushing the FlowTexture along the direction of the FlowDirection
- Input pins: FlowSpeed [FunctionInput_Scalar], FlowTexture [FunctionInput_Texture2D], FlowStrength [FunctionInput_Scalar], FlowDirection [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Time [FunctionInput_Scalar], MipBias [FunctionInput_Scalar]
- Output pins: Result, Uv1, Uv2, LerpAlpha

### Texture_Bombing

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/Texture_Bombing.Texture_Bombing`
- Deep graph doc: [Texture_Bombing](Functions/Engine_MaterialFunctions01_Texturing__Texture_Bombing.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 1
- Internal nodes: 93
- Description: Texture bombing uses multiple offset texture samples to break up tiling artifacts.
- Input pins: Texture Object [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], Tiling [FunctionInput_Scalar], Offset [FunctionInput_Scalar], Optional Heightmap [FunctionInput_Texture2D], Contrast [FunctionInput_Scalar], Enable Height Lerp [FunctionInput_StaticBool], Is Normalmap [FunctionInput_StaticBool]
- Output pins: Result

### Texture_Bombing_POM

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/Texture_Bombing_POM.Texture_Bombing_POM`
- Deep graph doc: [Texture_Bombing_POM](Functions/Engine_MaterialFunctions01_Texturing__Texture_Bombing_POM.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 9
- Outputs: 1
- Internal nodes: 83
- Description: Texture bombing uses multiple offset texture samples to break up tiling artifacts.
- Input pins: Texture Object [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], Tiling [FunctionInput_Scalar], Offset [FunctionInput_Scalar], Optional Heightmap [FunctionInput_Texture2D], Contrast [FunctionInput_Scalar], Enable Height Lerp [FunctionInput_StaticBool], Is Normalmap [FunctionInput_StaticBool], HeightRatio [FunctionInput_Scalar]
- Output pins: Result

### TextureBomb_SingleSample

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TextureBomb_SingleSample.TextureBomb_SingleSample`
- Deep graph doc: [TextureBomb_SingleSample](Functions/Engine_MaterialFunctions01_Texturing__TextureBomb_SingleSample.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 4
- Internal nodes: 57
- Input pins: UVs [FunctionInput_Vector2], Cell Index [FunctionInput_Vector2], Offset Strength [FunctionInput_Scalar], TexObject [FunctionInput_Texture2D], IsNormal? [FunctionInput_StaticBool]
- Output pins: UVs, Texture Sample, Tangent Space Camera Vector, Normal Flip

### TextureBomb_SingleSample_POM

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TextureBomb_SingleSample_POM.TextureBomb_SingleSample_POM`
- Deep graph doc: [TextureBomb_SingleSample_POM](Functions/Engine_MaterialFunctions01_Texturing__TextureBomb_SingleSample_POM.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 2
- Internal nodes: 61
- Input pins: UVs [FunctionInput_Vector2], Cell Index [FunctionInput_Vector2], Offset Strength [FunctionInput_Scalar], TexObject [FunctionInput_Texture2D], IsNormal? [FunctionInput_StaticBool], HeightTex [FunctionInput_Texture2D], HeightRatio [FunctionInput_Scalar]
- Output pins: UVs, Texture Sample

### TextureCropping

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TextureCropping.TextureCropping`
- Deep graph doc: [TextureCropping](Functions/Engine_MaterialFunctions01_Texturing__TextureCropping.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 4
- Internal nodes: 27
- Description: Crops a texture, useful for Emissives.  Supply the top left and bottom right corners of the crop  in 0-1 range.
- Input pins: UpperLeftCorner [FunctionInput_Vector2], LowerRightCorner [FunctionInput_Vector2], TextureIn [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], ExportFloat4 [FunctionInput_StaticBool]
- Output pins: CroppedMasked, Cropped, Crop UVs, Crop Mask

### TriplanarCameraVector

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/TriplanarCameraVector.TriplanarCameraVector`
- Deep graph doc: [TriplanarCameraVector](Functions/Engine_MaterialFunctions01_Texturing__TriplanarCameraVector.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 26
- Description: Applies a texture to the background, similar to a cubemap but uses the same 2d texture for all sides
- Input pins: Texture [FunctionInput_Texture2D], Tiling [FunctionInput_Vector3], AxisFadeContrast [FunctionInput_Scalar], Offset [FunctionInput_Vector3], CameraVector [FunctionInput_Vector3]
- Output pins: Result

### UVCropping

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/UVCropping.UVCropping`
- Deep graph doc: [UVCropping](Functions/Engine_MaterialFunctions01_Texturing__UVCropping.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 22
- Description: Creates a cropped region to be multiplied to texture
- Input pins: Crop Left [FunctionInput_Scalar], Crop Right [FunctionInput_Scalar], Crop Bottom [FunctionInput_Scalar], Crop Top [FunctionInput_Scalar]
- Output pins: Result

### VectorDisplacement

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/VectorDisplacement.VectorDisplacement`
- Deep graph doc: [VectorDisplacement](Functions/Engine_MaterialFunctions01_Texturing__VectorDisplacement.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 15
- Description: Use a Baked Vector displacement map from Xnormal
- Input pins: Vector [FunctionInput_Vector3], ScalarDisplacement [FunctionInput_Vector3], Min [FunctionInput_Scalar], Max [FunctionInput_Scalar]
- Output pins: Result

### WorldAlignedNormal

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal.WorldAlignedNormal`
- Deep graph doc: [WorldAlignedNormal](Functions/Engine_MaterialFunctions01_Texturing__WorldAlignedNormal.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 4
- Internal nodes: 72
- Description: Tiles a texture in worldspace
- Input pins: Normal [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], WorldSpace [FunctionInput_StaticBool], Use High Quality Normals [FunctionInput_StaticBool], ProjectionTransitionContrast [FunctionInput_Scalar]
- Output pins: Z Texture, XY Texture, XYZ Texture, XYZFlatTop

### WorldAlignedNormal2

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal2.WorldAlignedNormal2`
- Deep graph doc: [WorldAlignedNormal2](Functions/Engine_MaterialFunctions01_Texturing__WorldAlignedNormal2.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 4
- Internal nodes: 56
- Description: Tiles a texture in worldspace
- Input pins: Normal [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3]
- Output pins: Z Texture, XY Texture, XYZ Texture, XYZFlatTop

### WorldAlignedTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture.WorldAlignedTexture`
- Deep graph doc: [WorldAlignedTexture](Functions/Engine_MaterialFunctions01_Texturing__WorldAlignedTexture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 3
- Internal nodes: 38
- Description: Tiles a texture in worldspace
- Input pins: World Space Normal [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], Export Float 4 [FunctionInput_StaticBool], ProjectionTransitionContrast [FunctionInput_Scalar]
- Output pins: XYZ Texture, XY Texture, Z Texture

### WorldAlignedTexture_MipBias

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture_MipBias.WorldAlignedTexture_MipBias`
- Deep graph doc: [WorldAlignedTexture_MipBias](Functions/Engine_MaterialFunctions01_Texturing__WorldAlignedTexture_MipBias.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 3
- Internal nodes: 39
- Description: Tiles a texture in worldspace
- Input pins: World Space Normal [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], Export Float 4 [FunctionInput_StaticBool], ProjectionTransitionContrast [FunctionInput_Scalar], MipBias [FunctionInput_Scalar]
- Output pins: XYZ Texture, XY Texture, Z Texture

### WorldAlignedTexture_SeperateChannels

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture_SeperateChannels.WorldAlignedTexture_SeperateChannels`
- Deep graph doc: [WorldAlignedTexture_SeperateChannels](Functions/Engine_MaterialFunctions01_Texturing__WorldAlignedTexture_SeperateChannels.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 9
- Internal nodes: 60
- Description: Tiles a texture in worldspace
- Input pins: World Space Normal [FunctionInput_Vector3], TextureObject [FunctionInput_Texture2D], TextureSize (R) [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], ProjectionTransitionContrast [FunctionInput_Scalar], TextureSize (G) [FunctionInput_Vector3], TextureSize (B) [FunctionInput_Vector3]
- Output pins: XYZ Texture (R), XY Texture (R), Z Texture (R), XYZ Texture (G), XY Texture (G), Z Texture (G), XYZ Texture (B), XY Texture (B), Z Texture (B)

### ZWorldSpaceFlow

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ZWorldSpaceFlow.ZWorldSpaceFlow`
- Deep graph doc: [ZWorldSpaceFlow](Functions/Engine_MaterialFunctions01_Texturing__ZWorldSpaceFlow.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 45
- Description: Pushing the FlowTexture along the direction of the FlowDirection
- Input pins: FlowSpeed [FunctionInput_Scalar], FlowTexture [FunctionInput_Texture2D], FlowStrength [FunctionInput_Scalar], FlowDirection [FunctionInput_Vector2], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3]
- Output pins: Result



