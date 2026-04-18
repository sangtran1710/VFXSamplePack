# Engine_MaterialFunctions02

Engine: Unreal Engine 5.7
Function count: 37

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### 3PointLevels

- Path: `/Engine/Functions/Engine_MaterialFunctions02/3PointLevels.3PointLevels`
- Deep graph doc: [3PointLevels](Functions/Engine_MaterialFunctions02__3PointLevels.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 10
- Outputs: 1
- Internal nodes: 37
- Description: Remaps 0-1 values by linearly interpolating through 3 new values.
- Input pins: New Black Value [FunctionInput_Scalar], New Middle Value [FunctionInput_Scalar], New White Value [FunctionInput_Scalar], Middle Point [FunctionInput_Scalar], Texture [FunctionInput_Scalar], --------------- [FunctionInput_StaticBool], ---------------  [FunctionInput_StaticBool], Define Interpolation Curve [FunctionInput_StaticBool], Interpolation Power [FunctionInput_Scalar], Invert Interpolation Power [FunctionInput_StaticBool]
- Output pins: Result

### AlignFacingParticlesByVelocity2D

- Path: `/Engine/Functions/Engine_MaterialFunctions02/AlignFacingParticlesByVelocity2D.AlignFacingParticlesByVelocity2D`
- Deep graph doc: [AlignFacingParticlesByVelocity2D](Functions/Engine_MaterialFunctions02__AlignFacingParticlesByVelocity2D.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 1
- Internal nodes: 33
- Output pins: Result

### CenterPivotAroundVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/CenterPivotAroundVector.CenterPivotAroundVector`
- Deep graph doc: [CenterPivotAroundVector](Functions/Engine_MaterialFunctions02__CenterPivotAroundVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 11
- Description: Returns world position for every vertex as it would be projected against a vector defined by the vector user input. This can be used as the pivot point in a rotate about axis node.
- Input pins: Vector [FunctionInput_Vector3]
- Output pins: WorldPosition, LocalPosition

### CheckOcclusion

- Path: `/Engine/Functions/Engine_MaterialFunctions02/CheckOcclusion.CheckOcclusion`
- Deep graph doc: [CheckOcclusion](Functions/Engine_MaterialFunctions02__CheckOcclusion.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 28
- Description: Checks to see if a world position is occluded by an opaque object. For use with translucent materials.
- Input pins: Position To check for Occlusion [FunctionInput_Vector3], Offscreen Falloff Range [FunctionInput_Scalar], Depth Occlusion Falloff [FunctionInput_Scalar]
- Output pins: Screen Edge And Occlusion, Screen Edge, Occlusion

### FixRotateAboutAxisNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/FixRotateAboutAxisNormals.FixRotateAboutAxisNormals`
- Deep graph doc: [FixRotateAboutAxisNormals](Functions/Engine_MaterialFunctions02__FixRotateAboutAxisNormals.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 4
- Internal nodes: 18
- Description: Use this material function in conjunction with a rotate about axis based vertex shader to update the vertex normals so they rotate with and match the vertex shaders motion.
- Input pins: Rotation Axis [FunctionInput_Vector3], Rotation Angle [FunctionInput_Vector3], Movement Mask [FunctionInput_Scalar], World Space Vertex Normals [FunctionInput_Vector3], PivotPoint [FunctionInput_Vector3]
- Output pins: New Tangent Space Vertex Normal, New Tangent Space Vertex Normal RG, New Tangent Space Vertex Normal B, New World Space Vertex Normal

### FixRotateAboutAxisNormals2

- Path: `/Engine/Functions/Engine_MaterialFunctions02/FixRotateAboutAxisNormals2.FixRotateAboutAxisNormals2`
- Deep graph doc: [FixRotateAboutAxisNormals2](Functions/Engine_MaterialFunctions02__FixRotateAboutAxisNormals2.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 4
- Internal nodes: 17
- Description: Use this material function in conjunction with a rotate about axis based vertex shader to update the vertex normals so they rotate with and match the vertex shaders motion.
- Input pins: Rotation Axis [FunctionInput_Vector3], Rotation Angle [FunctionInput_Vector3], Movement Mask [FunctionInput_Scalar], World Space Vertex Normals [FunctionInput_Vector3]
- Output pins: New Tangent Space Vertex Normal, New Tangent Space Vertex Normal RG, New Tangent Space Vertex Normal B, New World Space Vertex Normal

### Fresnel_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Fresnel_Function.Fresnel_Function`
- Deep graph doc: [Fresnel_Function](Functions/Engine_MaterialFunctions02__Fresnel_Function.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 1
- Internal nodes: 22
- Description: Fresnel with more properties exposed, less instructions and a cheap alternative falloff.
- Input pins: Normal Vector [FunctionInput_Vector3], Camera Vector [FunctionInput_Vector3], Power [FunctionInput_Scalar], Invert Fresnel [FunctionInput_StaticBool], Use Cheap contrast [FunctionInput_StaticBool], Cheap contrast dark [FunctionInput_Scalar], Cheap contrast bright [FunctionInput_Scalar], Clamp Fresnel Dot Product [FunctionInput_StaticBool]
- Output pins: Result

### HighPrecisionWorldPosTextureSampling

- Path: `/Engine/Functions/Engine_MaterialFunctions02/HighPrecisionWorldPosTextureSampling.HighPrecisionWorldPosTextureSampling`
- Deep graph doc: [HighPrecisionWorldPosTextureSampling](Functions/Engine_MaterialFunctions02__HighPrecisionWorldPosTextureSampling.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 2
- Internal nodes: 18
- Description: Provides a world position that can be used to map textures to the world without low precision artifacts at large distances.
- Input pins: TilingSize [FunctionInput_Vector3], Texture Coord U Vector [FunctionInput_Vector3], Texture Coord V Vector [FunctionInput_Vector3], Texture Object [FunctionInput_Texture2D]
- Output pins: High Precision World Position, Texture (F4)

### HueShift

- Path: `/Engine/Functions/Engine_MaterialFunctions02/HueShift.HueShift`
- Deep graph doc: [HueShift](Functions/Engine_MaterialFunctions02__HueShift.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 9
- Description: Adjusts the hue of an image.
- Input pins: Texture [FunctionInput_Vector3], Hue Shift Percentage [FunctionInput_Scalar]
- Output pins: Result

### Lerp_3Color

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_3Color.Lerp_3Color`
- Deep graph doc: [Lerp_3Color](Functions/Engine_MaterialFunctions02__Lerp_3Color.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 11
- Description: Lerps 3 inputs based on a single value.
- Input pins: A [FunctionInput_Vector3], B [FunctionInput_Vector3], C [FunctionInput_Vector3], Alpha [FunctionInput_Scalar]
- Output pins: Result

### Lerp_Multiple_Float

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_Multiple_Float.Lerp_Multiple_Float`
- Deep graph doc: [Lerp_Multiple_Float](Functions/Engine_MaterialFunctions02__Lerp_Multiple_Float.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 23
- Description: Lerps multiple inputs based on a single value.
- Input pins: 1 [FunctionInput_Scalar], 2 [FunctionInput_Scalar], 3 [FunctionInput_Scalar], 4 [FunctionInput_Scalar], A [FunctionInput_Scalar]
- Output pins: Lerp 3 Inputs, Lerp 4 Inputs

### Lerp_Multiple_Float2

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_Multiple_Float2.Lerp_Multiple_Float2`
- Deep graph doc: [Lerp_Multiple_Float2](Functions/Engine_MaterialFunctions02__Lerp_Multiple_Float2.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 23
- Description: Lerps multiple inputs based on a single value.
- Input pins: 1 [FunctionInput_Vector2], 2 [FunctionInput_Vector2], 3 [FunctionInput_Vector2], 4 [FunctionInput_Vector2], A [FunctionInput_Scalar]
- Output pins: Lerp 3 Inputs, Lerp 4 Inputs

### Lerp_Multiple_Float3

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_Multiple_Float3.Lerp_Multiple_Float3`
- Deep graph doc: [Lerp_Multiple_Float3](Functions/Engine_MaterialFunctions02__Lerp_Multiple_Float3.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 23
- Description: Lerps multiple inputs based on a single value.
- Input pins: 1 [FunctionInput_Vector3], 2 [FunctionInput_Vector3], 3 [FunctionInput_Vector3], 4 [FunctionInput_Vector3], A [FunctionInput_Scalar]
- Output pins: Lerp 3 Inputs, Lerp 4 Inputs

### Lerp_Multiple_Float4

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Lerp_Multiple_Float4.Lerp_Multiple_Float4`
- Deep graph doc: [Lerp_Multiple_Float4](Functions/Engine_MaterialFunctions02__Lerp_Multiple_Float4.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 23
- Description: Lerps multiple inputs based on a single value.
- Input pins: 1 [FunctionInput_Vector4], 2 [FunctionInput_Vector4], 3 [FunctionInput_Vector4], 4 [FunctionInput_Vector4], A [FunctionInput_Scalar]
- Output pins: Lerp 3 Inputs, Lerp 4 Inputs

### LumensFromLightSource

- Path: `/Engine/Functions/Engine_MaterialFunctions02/LumensFromLightSource.LumensFromLightSource`
- Deep graph doc: [LumensFromLightSource](Functions/Engine_MaterialFunctions02__LumensFromLightSource.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 11
- Description: Returns the per pixel brightness of a spherical light at a given radius.
- Input pins: Light Color [FunctionInput_Vector3], Light Source Lumens [FunctionInput_Scalar], Light Radius [FunctionInput_Scalar]
- Output pins: Result

### MacroUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MacroUVs.MacroUVs`
- Deep graph doc: [MacroUVs](Functions/Engine_MaterialFunctions02__MacroUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 0
- Internal nodes: 0

### MatLayerBlend_BreakOpacity

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MatLayerBlend_BreakOpacity.MatLayerBlend_BreakOpacity`
- Deep graph doc: [MatLayerBlend_BreakOpacity](Functions/Engine_MaterialFunctions02__MatLayerBlend_BreakOpacity.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Input pins: Mat attributes [FunctionInput_MaterialAttributes]
- Output pins: Result

### ms_PivotPainter2_UnpackIntegerAsFloat

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ms_PivotPainter2_UnpackIntegerAsFloat.ms_PivotPainter2_UnpackIntegerAsFloat`
- Deep graph doc: [ms_PivotPainter2_UnpackIntegerAsFloat](Functions/Engine_MaterialFunctions02__ms_PivotPainter2_UnpackIntegerAsFloat.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Decodes Pivot Painter Integer As Float Data
- Input pins: Integer as Float [FunctionInput_Scalar]
- Output pins: Result

### ObjectLocalBounds

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ObjectLocalBounds.ObjectLocalBounds`
- Deep graph doc: [ObjectLocalBounds](Functions/Engine_MaterialFunctions02__ObjectLocalBounds.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 3
- Internal nodes: 6
- Output pins: Local Bounds Minimum, Local Bounds Max, Local Bounds Size

### ObjectLocalToWorldData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ObjectLocalToWorldData.ObjectLocalToWorldData`
- Deep graph doc: [ObjectLocalToWorldData](Functions/Engine_MaterialFunctions02__ObjectLocalToWorldData.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 16
- Description: Returns object level information.
- Input pins: Local Vector [FunctionInput_Vector3]
- Output pins: Object Scale, World Vector, Object Pivot Location, Object Scale XYZ

### OffsetAndScaleTo1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/OffsetAndScaleTo1.OffsetAndScaleTo1`
- Deep graph doc: [OffsetAndScaleTo1](Functions/Engine_MaterialFunctions02__OffsetAndScaleTo1.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: The value entered in the original value input is offset by the offset value and the values are then scaled back into the 0-1 range. 
- Input pins: Offset Amount [FunctionInput_Scalar], Original Value [FunctionInput_Scalar]
- Output pins: Result

### PassThrough

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PassThrough.PassThrough`
- Deep graph doc: [PassThrough](Functions/Engine_MaterialFunctions02__PassThrough.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 4
- Internal nodes: 8
- Description: This node simply passes any inserted values through itself. Suggested uses include, using this node to form a closer connection point for nodes on opposite sides of the graph or bufferering math operations and the nodes they connect to with this connection hub node. 
- Input pins: V3 [FunctionInput_Vector3], V2 [FunctionInput_Vector2], V4 [FunctionInput_Vector4], S [FunctionInput_Scalar]
- Output pins: V3, V2, S, V4

### Posterize

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Posterize.Posterize`
- Deep graph doc: [Posterize](Functions/Engine_MaterialFunctions02__Posterize.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 15
- Input pins: Diffuse [FunctionInput_Vector3], Number of value steps [FunctionInput_Scalar]
- Output pins: Result

### RayTracedSphere

- Path: `/Engine/Functions/Engine_MaterialFunctions02/RayTracedSphere.RayTracedSphere`
- Deep graph doc: [RayTracedSphere](Functions/Engine_MaterialFunctions02__RayTracedSphere.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 7
- Internal nodes: 48
- Input pins: Sphere Position [FunctionInput_Vector3], Sphere Radius [FunctionInput_Scalar], RayDirection [FunctionInput_Vector3], World position [FunctionInput_Vector3]
- Output pins: Ray Intersection Position , Intersection End Depth , 0-1 Sphere density, Projected Capture Vector , Intersection End World Position, Ray Hit Bool, Intersection Surface Normal

### SafeNormalize

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SafeNormalize.SafeNormalize`
- Deep graph doc: [SafeNormalize](Functions/Engine_MaterialFunctions02__SafeNormalize.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 14
- Description: If the input vectors length == 0 then return 0 else return the normalized vector by default. This avoids returning NAN or NULL. Optionally, one can pass in a default vector that should be used when the vector 
- Input pins: Vector [FunctionInput_Vector3], Default [FunctionInput_Vector3]
- Output pins: Result, Length==0

### SampleSceneDepth

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SampleSceneDepth.SampleSceneDepth`
- Deep graph doc: [SampleSceneDepth](Functions/Engine_MaterialFunctions02__SampleSceneDepth.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 22
- Description: Samples the scene depth texture using multiple methods. 
- Input pins: Pixel Offset [FunctionInput_Vector2], Fraction Offset [FunctionInput_Vector2], Use Custom Depth [FunctionInput_StaticBool]
- Output pins: Depth Using Pixel Offset, Depth Using Fraction Offset

### ScreenResolution

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ScreenResolution.ScreenResolution`
- Deep graph doc: [ScreenResolution](Functions/Engine_MaterialFunctions02__ScreenResolution.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 2
- Internal nodes: 4
- Description: Retrieves the visible screen resolution and the render target size.
- Output pins: Visible Resolution, Buffer Resolution

### SmoothStep

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SmoothStep.SmoothStep`
- Deep graph doc: [SmoothStep](Functions/Engine_MaterialFunctions02__SmoothStep.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 5
- Description: HLSL smoothstep function. a custom node)
- Input pins: Min [FunctionInput_Scalar], Max [FunctionInput_Scalar], Alpha [FunctionInput_Scalar]
- Output pins: Result

### SmoothThreshold

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SmoothThreshold.SmoothThreshold`
- Deep graph doc: [SmoothThreshold](Functions/Engine_MaterialFunctions02__SmoothThreshold.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 15
- Description: Smooth contrast applied after a threshold value is passed
- Input pins: Gradient [FunctionInput_Scalar], Cutoff Value [FunctionInput_Scalar], Lerp Value [FunctionInput_Scalar]
- Output pins: Result

### SplitComponents

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SplitComponents.SplitComponents`
- Deep graph doc: [SplitComponents](Functions/Engine_MaterialFunctions02__SplitComponents.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 8
- Input pins: Color [FunctionInput_Vector3]
- Output pins: R, G, B, RGB

### Step

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Step.Step`
- Deep graph doc: [Step](Functions/Engine_MaterialFunctions02__Step.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 9
- Input pins: Low [FunctionInput_Scalar], High [FunctionInput_Scalar]
- Output pins: Result

### SteppingPannerTime

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SteppingPannerTime.SteppingPannerTime`
- Deep graph doc: [SteppingPannerTime](Functions/Engine_MaterialFunctions02__SteppingPannerTime.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 15
- Description: Add to UV coordinates to make them jump by the "Jump Amount Per Step" value for the number of times, per scaled second, set by the "Number of Steps Per Unit of Time". This will make noise maps seem to be more random than they are. 
- Input pins: Number of Steps Per Unit of Time [FunctionInput_Vector2], TimeSpeed [FunctionInput_Scalar], Jump Amount Per Step [FunctionInput_Vector2], Time Offset [FunctionInput_Scalar]
- Output pins: Result

### TangentBasis

- Path: `/Engine/Functions/Engine_MaterialFunctions02/TangentBasis.TangentBasis`
- Deep graph doc: [TangentBasis](Functions/Engine_MaterialFunctions02__TangentBasis.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 3
- Internal nodes: 12
- Description: Retrieves the tangent basis in world space
- Output pins: X U, Y V, Z W

### TransformToClipSpace

- Path: `/Engine/Functions/Engine_MaterialFunctions02/TransformToClipSpace.TransformToClipSpace`
- Deep graph doc: [TransformToClipSpace](Functions/Engine_MaterialFunctions02__TransformToClipSpace.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 21
- Input pins: World Position [FunctionInput_Vector3], Buffer UV Pixel Offset [FunctionInput_Vector2]
- Output pins: Clip Space XY, Screen aligned 0-1 UV, Buffer UVs for Scene Textures

### ValueStep

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ValueStep.ValueStep`
- Deep graph doc: [ValueStep](Functions/Engine_MaterialFunctions02__ValueStep.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 17
- Description: Takes a gradient with values over one and masks out a portion of the gradient based on input variables.
- Input pins: Gradient [FunctionInput_Scalar], Mask Offset Value [FunctionInput_Scalar], Number Before White Result [FunctionInput_Scalar]
- Output pins: Result

### WorldAlignedNormals_HighQuality

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldAlignedNormals_HighQuality.WorldAlignedNormals_HighQuality`
- Deep graph doc: [WorldAlignedNormals_HighQuality](Functions/Engine_MaterialFunctions02__WorldAlignedNormals_HighQuality.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 4
- Internal nodes: 82
- Description: Applies normal maps to objects and correctly orients them. See WorldAlignedTexture_Complex for a more thorough implementation.  This version of world aligned normals is ~20 instructions more. The Normals are in worldspace by default. Transform them from world to tangent. 
- Input pins: Texture [FunctionInput_Texture2D], Negative World Position Divisor [FunctionInput_Vector3], World Position [FunctionInput_Vector3], X lerp alpha [FunctionInput_Scalar], Z lerp Alpha [FunctionInput_Scalar], WorldNormal [FunctionInput_Vector3]
- Output pins: World Space Normals XYZ, Z Projection World Space Normals, XY Projection World Space Normals, XY Flat Top Projection World Space Normals

### WorldAlignedNormals_HighQuality_optimizationAttempt

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldAlignedNormals_HighQuality_optimizationAttempt.WorldAlignedNormals_HighQuality_optimizationAttempt`
- Deep graph doc: [WorldAlignedNormals_HighQuality_optimizationAttempt](Functions/Engine_MaterialFunctions02__WorldAlignedNormals_HighQuality_optimizationAttempt.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 10
- Internal nodes: 119
- Description: Applies normal maps to objects and correctly orients them. See WorldAlignedTexture_Complex for a more thorough implementation.  This version of world aligned normals is ~20 instructions more. The Normals are in worldspace by default. Transform them from world to tangent. 
- Input pins: Texture [FunctionInput_Texture2D], Negative World Position Divisor [FunctionInput_Vector3], World Position [FunctionInput_Vector3], X lerp alpha [FunctionInput_Scalar], Z lerp Alpha [FunctionInput_Scalar], WorldNormal [FunctionInput_Vector3], Optimize (See Comment) [FunctionInput_StaticBool]
- Output pins: World Space Normals XYZ, Z Projection World Space Normals, XY Projection World Space Normals, XY Flat Top Projection World Space Normals, UV3, UV4, UV2, UV6, UV7, UV5



