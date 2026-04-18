# Engine_MaterialFunctions02/NotInUse

Engine: Unreal Engine 5.7
Function count: 2

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Avoid by default; category indicates retired or unsupported content.

## Functions

### WorldAlignedNormals_HighQuality_optimizationAttempt

- Path: `/Engine/Functions/Engine_MaterialFunctions02/NotInUse/WorldAlignedNormals_HighQuality_optimizationAttempt.WorldAlignedNormals_HighQuality_optimizationAttempt`
- Deep graph doc: [WorldAlignedNormals_HighQuality_optimizationAttempt](Functions/Engine_MaterialFunctions02_NotInUse__WorldAlignedNormals_HighQuality_optimizationAttempt.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Avoid by default; category indicates retired or unsupported content.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 10
- Internal nodes: 119
- Description: Applies normal maps to objects and correctly orients them. See WorldAlignedTexture_Complex for a more thorough implementation.  This version of world aligned normals is ~20 instructions more. The Normals are in worldspace by default. Transform them from world to tangent. 
- Input pins: Texture [FunctionInput_Texture2D], Negative World Position Divisor [FunctionInput_Vector3], World Position [FunctionInput_Vector3], X lerp alpha [FunctionInput_Scalar], Z lerp Alpha [FunctionInput_Scalar], WorldNormal [FunctionInput_Vector3], Optimize (See Comment) [FunctionInput_StaticBool]
- Output pins: World Space Normals XYZ, Z Projection World Space Normals, XY Projection World Space Normals, XY Flat Top Projection World Space Normals, UV3, UV4, UV2, UV6, UV7, UV5

### WorldAlignedTextures_Complex_OptimizationAttempt

- Path: `/Engine/Functions/Engine_MaterialFunctions02/NotInUse/WorldAlignedTextures_Complex_OptimizationAttempt.WorldAlignedTextures_Complex_OptimizationAttempt`
- Deep graph doc: [WorldAlignedTextures_Complex_OptimizationAttempt](Functions/Engine_MaterialFunctions02_NotInUse__WorldAlignedTextures_Complex_OptimizationAttempt.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Avoid by default; category indicates retired or unsupported content.
- Study status: inspected through get_material_function_info
- Inputs: 23
- Outputs: 15
- Internal nodes: 148
- Description: Tiles a texture in worldspace
- Input pins: World Space Normal [FunctionInput_Vector3], Diffuse Texture Object [FunctionInput_Texture2D], TextureSize [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], Export Float 4 [FunctionInput_StaticBool], ProjectionTransitionContrast [FunctionInput_Scalar], Use Texture Alpha Heightmap to Refine the Transistion [FunctionInput_StaticBool], Texture Size Z projection [FunctionInput_Vector3], Use seperate settings for Z projection [FunctionInput_StaticBool], Z Plane Diffuse Texture Object [FunctionInput_Texture2D], Use seperate texture for Z projection [FunctionInput_StaticBool], World Space Normals [FunctionInput_StaticBool]
- Output pins: XYZ Diffuse Texture, XY Diffuse Texture, Z Diffuse Texture, Normal Z Texture, Normal XY Texture, Normal XYZ Texture, Normal XYZ Flat Top, -------------, Opti Normal - Custom UV 6, Opti Normal - Custom UV 5, Opti Normal - Custom UV 4, -------



