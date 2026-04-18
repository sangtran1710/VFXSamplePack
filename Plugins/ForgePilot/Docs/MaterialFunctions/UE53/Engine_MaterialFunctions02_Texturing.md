# Engine_MaterialFunctions02/Texturing

Engine: Unreal Engine 5.3
Function count: 21

## Agent Use

Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### 3dParticleOpacity

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/3dParticleOpacity.3dParticleOpacity`
- Deep graph doc: [3dParticleOpacity](Functions/Engine_MaterialFunctions02_Texturing__3dParticleOpacity.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 15
- Outputs: 2
- Internal nodes: 49
- Input pins: Explicit Texture Depth (WS) [FunctionInput_Scalar], Depth Texture [FunctionInput_Scalar], Use Near Camera Falloff [FunctionInput_StaticBool], Use Particle Alpha [FunctionInput_StaticBool], Depth Texture Falloff Softness (WS) (1/n) [FunctionInput_Scalar], Near Camera Fade Distance (1/n) [FunctionInput_Scalar], Near Camera Falloff Start Distance [FunctionInput_Scalar], -------Camera Falloff Group------- [FunctionInput_StaticBool], -------Particle Alpha Group------- [FunctionInput_StaticBool], Use Depth In Camera Falloff Calculation [FunctionInput_StaticBool], -------WPO Group------- [FunctionInput_StaticBool], Camera Offset [FunctionInput_Scalar]
- Output pins: Opacity, World Position Offset - Camera Offset

### 3DParticleUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/3DParticleUVs.3DParticleUVs`
- Deep graph doc: [3DParticleUVs](Functions/Engine_MaterialFunctions02_Texturing__3DParticleUVs.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 20
- Description: Moves through a flipbook texture based on the cameras position compared to the object center.
- Input pins: Number of horizontal Images [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Number of texture rotations per world rotation [FunctionInput_Scalar]
- Output pins: Uvs

### 3DSandMayaUVCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/3DSandMayaUVCoordinates.3DSandMayaUVCoordinates`
- Deep graph doc: [3DSandMayaUVCoordinates](Functions/Engine_MaterialFunctions02_Texturing__3DSandMayaUVCoordinates.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 4
- Internal nodes: 24
- Description: Inverts the green channel in each set of model uvs to place 0,0 in the bottom left hand corner to match 3D Studio Max and Mayas UV layouts. Flipping the values are important if you're storing specific values in the uv channels.
- Output pins: UVChannel0, UVChannel1, UVChannel2, UVChannel3

### CustomRotator

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/CustomRotator.CustomRotator`
- Deep graph doc: [CustomRotator](Functions/Engine_MaterialFunctions02_Texturing__CustomRotator.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 17
- Description: Rotator with rotation center exposed and a rotation angle on 0-1 scale. A rotation angle value of 1 is equal to one full rotation.
- Input pins: Rotation Center [FunctionInput_Vector2], Rotation Angle (0-1) [FunctionInput_Scalar], UVs [FunctionInput_Vector2]
- Output pins: Rotated Values

### FlipBook

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook.FlipBook`
- Deep graph doc: [FlipBook](Functions/Engine_MaterialFunctions02_Texturing__FlipBook.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 4
- Internal nodes: 38
- Description: Animates a texture in a flipbook like fashion. 
- Input pins: Texture [FunctionInput_Texture2D], Animation  Phase (0-1) [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Number of Columns [FunctionInput_Scalar], Number of Rows [FunctionInput_Scalar], Clamp Anim (see tooltip) [FunctionInput_StaticBool], MipBias/Level [FunctionInput_Scalar], Use Mip Bias (T) Level (F) [FunctionInput_StaticBool]
- Output pins: Result, UVs, Alpha, UV Center

### FlipBook_MotionVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_MotionVectors.FlipBook_MotionVectors`
- Deep graph doc: [FlipBook_MotionVectors](Functions/Engine_MaterialFunctions02_Texturing__FlipBook_MotionVectors.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Check texture inputs and motion-vector conventions before wiring; wrong pins can compile but look incorrect.
- Study status: inspected through get_material_function_info
- Inputs: 12
- Outputs: 6
- Internal nodes: 98
- Description: Flipbook with additional options for Motion Vectors. Animates a texture in a flipbook like fashion. 
- Input pins: Texture [FunctionInput_Texture2D], Animation  Phase (0-1) [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Number of Columns X [FunctionInput_Scalar], Number of Rows Y [FunctionInput_Scalar], Motion Vector Texture [FunctionInput_Texture2D], Phase to Debug [FunctionInput_Scalar], Motion Vector Intensity [FunctionInput_Scalar], Double Apply Motion Vectors [FunctionInput_StaticBool], Use Custom UVs [FunctionInput_StaticBool], Custom UV Input [FunctionInput_Vector2], Custom UV2 Input [FunctionInput_Vector2]
- Output pins: Result, Alpha, Blend Phase, Forward Motion Vectors, CustomUV Output, CustomUV2 Output

### FlipBook_UniformNonUniform

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_UniformNonUniform.FlipBook_UniformNonUniform`
- Deep graph doc: [FlipBook_UniformNonUniform](Functions/Engine_MaterialFunctions02_Texturing__FlipBook_UniformNonUniform.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 5
- Internal nodes: 44
- Description: This is a special version of the flipbook function only useful to the RenderToTexture blueprint. It allows you to sample Non-Uniform flipbooks from within Uniform Textures. Ie, like a letterboxed flipbook which is how all RenderTarget Previews are set up.
- Input pins: Texture [FunctionInput_Texture2D], Animation  Phase (0-1) [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Number of Columns [FunctionInput_Scalar], Number of Rows [FunctionInput_Scalar], Clamp Anim (see tooltip) [FunctionInput_StaticBool], MipBias [FunctionInput_Scalar]
- Output pins: Result, UVs, Alpha, Blend Phase, Y Row

### FlowMaps

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/Flowmaps.FlowMaps`
- Deep graph doc: [FlowMaps](Functions/Engine_MaterialFunctions02_Texturing__FlowMaps.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 9
- Outputs: 5
- Internal nodes: 93
- Description: Pushes the Diffuse and Normal textures along the flow map vectors. See Material'/Engine/Functions/Engine_MaterialFunctions02/ExampleContent/FlowMaps.FlowMaps' for setting suggestions or additional techniques.
- Input pins: Diffuse [FunctionInput_Texture2D], Flow Vector Map (see tooltip) [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Time (see tooltip) [FunctionInput_Scalar], UV texture 2 offset [FunctionInput_Vector2], Normal Map [FunctionInput_Texture2D], Texture Mip Bias [FunctionInput_Scalar], Texture Size [FunctionInput_Vector2], useMipLevel [FunctionInput_StaticBool]
- Output pins: Diffuse, Diffuse Alpha, Normal, Distortion, UVs

### Flowmaps_2D

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/Flowmaps_2D.Flowmaps_2D`
- Deep graph doc: [Flowmaps_2D](Functions/Engine_MaterialFunctions02_Texturing__Flowmaps_2D.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 28
- Description: This 2D version of flowmaps is designed to be the 2D version of the 3D flowmap function. It is designed to work using World XY Coordinates instead of UVs.
- Input pins: Volume Texture [FunctionInput_Texture2D], UV [FunctionInput_Vector2], Velocity [FunctionInput_Vector2], Override Time [FunctionInput_Scalar], Mip Level [FunctionInput_Scalar]
- Output pins: Result RGB, Result RGBA

### Flowmaps_3D

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/Flowmaps_3D.Flowmaps_3D`
- Deep graph doc: [Flowmaps_3D](Functions/Engine_MaterialFunctions02_Texturing__Flowmaps_3D.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 28
- Description: Performs Flowmap blending using a Volume Texture.
- Input pins: Volume Texture [FunctionInput_VolumeTexture], UVW [FunctionInput_Vector3], Velocity [FunctionInput_Vector3], Override Time [FunctionInput_Scalar], Mip Level [FunctionInput_Scalar]
- Output pins: Result RGB, Result RGBA

### FlowMaps_Simple

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlowMaps_Simple.FlowMaps_Simple`
- Deep graph doc: [FlowMaps_Simple](Functions/Engine_MaterialFunctions02_Texturing__FlowMaps_Simple.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 4
- Internal nodes: 75
- Description: Pushes the Diffuse and Normal textures along the flow map vectors. See Material'/Engine/Functions/Engine_MaterialFunctions02/ExampleContent/FlowMaps.FlowMaps' for setting suggestions or additional techniques.
- Input pins: Diffuse [FunctionInput_Texture2D], Flow Vector Map (see tooltip) [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Time (see tooltip) [FunctionInput_Scalar], UV texture 2 offset [FunctionInput_Vector2], Normal Map [FunctionInput_Texture2D]
- Output pins: Diffuse, Diffuse Alpha, Normal, Distortion

### FlowMaps_UV1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlowMaps_UV1.FlowMaps_UV1`
- Deep graph doc: [FlowMaps_UV1](Functions/Engine_MaterialFunctions02_Texturing__FlowMaps_UV1.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 4
- Internal nodes: 79
- Description: Pushes the Diffuse and Normal textures along the flow map vectors. See Material'/Engine/Functions/Engine_MaterialFunctions02/ExampleContent/FlowMaps.FlowMaps' for setting suggestions or additional techniques.
- Input pins: Diffuse [FunctionInput_Texture2D], Flow Vector Map (see tooltip) [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Time (see tooltip) [FunctionInput_Scalar], UV texture 2 offset [FunctionInput_Vector2], Normal Map [FunctionInput_Texture2D], Texture Mip Bias [FunctionInput_Scalar], Texture Size [FunctionInput_Vector2]
- Output pins: Diffuse, Diffuse Alpha, Normal, Distortion

### HeightLerp

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/HeightLerp.HeightLerp`
- Deep graph doc: [HeightLerp](Functions/Engine_MaterialFunctions02_Texturing__HeightLerp.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 3
- Internal nodes: 18
- Description: Lerp between two values or textures based off of a height map and a transition value.
- Input pins: Transition Phase [FunctionInput_Scalar], Height Texture [FunctionInput_Scalar], A [FunctionInput_Vector3], B [FunctionInput_Vector3], Contrast [FunctionInput_Scalar]
- Output pins: Results, Alpha, Lerp Alpha No Contrast

### HeightLerpWithTwoHeightMaps

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/HeightLerpWithTwoHeightMaps.HeightLerpWithTwoHeightMaps`
- Deep graph doc: [HeightLerpWithTwoHeightMaps](Functions/Engine_MaterialFunctions02_Texturing__HeightLerpWithTwoHeightMaps.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 4
- Internal nodes: 22
- Description: Lerp between two values or textures based off of two height map and a transition value.
- Input pins: Transistion Phase [FunctionInput_Scalar], Height Texture 1 [FunctionInput_Scalar], Diffuse 1 [FunctionInput_Vector3], Diffuse 2 [FunctionInput_Vector3], Contrast [FunctionInput_Scalar], Height Texture 2 [FunctionInput_Scalar]
- Output pins: Results, Alpha, Lerp Alpha No Contrast, Resulting Height Map

### RaiseBlackLevelsByPercentage

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/RaiseBlackLevelsByPercentage.RaiseBlackLevelsByPercentage`
- Deep graph doc: [RaiseBlackLevelsByPercentage](Functions/Engine_MaterialFunctions02_Texturing__RaiseBlackLevelsByPercentage.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Raises the black levels of an input value based on a percentage provided in the "black intensity level" parameter. Note: the image input values should range from 0-1.  
- Input pins: Image [FunctionInput_Vector3], Black intensity level [FunctionInput_Vector3]
- Output pins: Result

### ScreenAlignedPixelToPixelUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/ScreenAlignedPixelToPixelUVs.ScreenAlignedPixelToPixelUVs`
- Deep graph doc: [ScreenAlignedPixelToPixelUVs](Functions/Engine_MaterialFunctions02_Texturing__ScreenAlignedPixelToPixelUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 4
- Description: Maps a texture to the screen without stretching any of the pixels. Each pixel on the screen will line up with a pixel from the sampled texture. Note that this will preview incorrectly in the material editor.
- Input pins: TextureResolution [FunctionInput_Vector2]
- Output pins: Result

### ScreenAlignedUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/ScreenAlignedUVs.ScreenAlignedUVs`
- Deep graph doc: [ScreenAlignedUVs](Functions/Engine_MaterialFunctions02_Texturing__ScreenAlignedUVs.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 3
- Internal nodes: 26
- Description: Maps the 0-1 uv range to the screen. Don't use as UV for SceneTexture material expression
- Input pins: Texture Dimensions [FunctionInput_Vector2]
- Output pins: X 100%, Y 100%, X Scale to Ratio, Y 100%, X 100%, Y Scale to Ratio

### TangentSpaceFlowComplex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/TangentSpaceFlowComplex.TangentSpaceFlowComplex`
- Deep graph doc: [TangentSpaceFlowComplex](Functions/Engine_MaterialFunctions02_Texturing__TangentSpaceFlowComplex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 45
- Description: Pushing the FlowTexture along the direction of the FlowDirection
- Input pins: FlowTexture [FunctionInput_Texture2D], FlowMap [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Time [FunctionInput_Scalar], Anim offset Noise [FunctionInput_Scalar]
- Output pins: Result

### WorldAlignedTextures_Complex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/WorldAlignedTextures_Complex.WorldAlignedTextures_Complex`
- Deep graph doc: [WorldAlignedTextures_Complex](Functions/Engine_MaterialFunctions02_Texturing__WorldAlignedTextures_Complex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 24
- Outputs: 16
- Internal nodes: 136
- Description: Tiles a texture in worldspace
- Input pins: World Space Normal [FunctionInput_Vector3], Diffuse Texture Object [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], Export Float 4 Diffuse [FunctionInput_StaticBool], ProjectionTransitionContrast [FunctionInput_Scalar], Use Texture Alpha Heightmap to Refine the Transistion [FunctionInput_StaticBool], Texture Size Z projection [FunctionInput_Vector3], Use seperate settings for Z projection [FunctionInput_StaticBool], Z Plane Diffuse Texture Object [FunctionInput_Texture2D], Use seperate texture for Z projection [FunctionInput_StaticBool], World Space Normals [FunctionInput_StaticBool]
- Output pins: XYZ Diffuse Texture, XY Diffuse Texture, Z Diffuse Texture, Normal Z Texture, Normal XY Texture, Normal XYZ Texture, Normal XYZ Flat Top, -------------------------- , XYZ Spec, XY Spec, Z Spec,  --------------------------

### WorldAlignedTextures_Complex_OptimizationAttempt

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/WorldAlignedTextures_Complex_OptimizationAttempt.WorldAlignedTextures_Complex_OptimizationAttempt`
- Deep graph doc: [WorldAlignedTextures_Complex_OptimizationAttempt](Functions/Engine_MaterialFunctions02_Texturing__WorldAlignedTextures_Complex_OptimizationAttempt.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 23
- Outputs: 15
- Internal nodes: 148
- Description: Tiles a texture in worldspace
- Input pins: World Space Normal [FunctionInput_Vector3], Diffuse Texture Object [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], Export Float 4 [FunctionInput_StaticBool], ProjectionTransitionContrast [FunctionInput_Scalar], Use Texture Alpha Heightmap to Refine the Transistion [FunctionInput_StaticBool], Texture Size Z projection [FunctionInput_Vector3], Use seperate settings for Z projection [FunctionInput_StaticBool], Z Plane Diffuse Texture Object [FunctionInput_Texture2D], Use seperate texture for Z projection [FunctionInput_StaticBool], World Space Normals [FunctionInput_StaticBool]
- Output pins: XYZ Diffuse Texture, XY Diffuse Texture, Z Diffuse Texture, Normal Z Texture, Normal XY Texture, Normal XYZ Texture, Normal XYZ Flat Top, -------------, Opti Normal - Custom UV 6, Opti Normal - Custom UV 5, Opti Normal - Custom UV 4, -------

### WorldPositionWithScale

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/WorldPositionWithScale.WorldPositionWithScale`
- Deep graph doc: [WorldPositionWithScale](Functions/Engine_MaterialFunctions02_Texturing__WorldPositionWithScale.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 3
- Internal nodes: 9
- Description: WorldPosition divided by a number and split into common configurations.
- Input pins: Divisor [FunctionInput_Vector3]
- Output pins: XYZ, XY, Z

