# Engine_MaterialFunctions03/Texturing

Engine: Unreal Engine 5.3
Function count: 9

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### CameraWorldBlend

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/CameraWorldBlend.CameraWorldBlend`
- Deep graph doc: [CameraWorldBlend](Functions/Engine_MaterialFunctions03_Texturing__CameraWorldBlend.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 31
- Description: Outputs falloff results for the 3 primary world vectors based on the camera angle.  Can be used to blend between textures based on direction the camera is looking.
- Input pins: Blend Power [FunctionInput_Scalar], Use Reflection Vector [FunctionInput_StaticBool], Use Smoothable Normals [FunctionInput_StaticBool], Smooth Reflection Percentage [FunctionInput_Scalar]
- Output pins: YZ True, XZ True, XY True

### LocalSpaceSurfaceMirroring

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/LocalSpaceSurfaceMirroring.LocalSpaceSurfaceMirroring`
- Deep graph doc: [LocalSpaceSurfaceMirroring](Functions/Engine_MaterialFunctions03_Texturing__LocalSpaceSurfaceMirroring.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 19
- Description: Generates mask results based on the local orientation of surfaces.  Useful for mirroring texture information based on the object's topology.
- Input pins: In [FunctionInput_Scalar], Use NormalMap [FunctionInput_StaticBool], Normal Map [FunctionInput_Vector3]
- Output pins: Local Z, Local Y, Local X

### SubUV_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/SubUV_Function.SubUV_Function`
- Deep graph doc: [SubUV_Function](Functions/Engine_MaterialFunctions03_Texturing__SubUV_Function.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 32
- Description: Allows SubUV blending of multiple texture frames in a single texture
- Input pins: Texture [FunctionInput_Texture2D], SubImages [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Frame [FunctionInput_Scalar]
- Output pins: RGB, Alpha, UVs

### SubUV_Function_MipDerivative

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/SubUV_Function_MipDerivative.SubUV_Function_MipDerivative`
- Deep graph doc: [SubUV_Function_MipDerivative](Functions/Engine_MaterialFunctions03_Texturing__SubUV_Function_MipDerivative.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Check texture inputs and motion-vector conventions before wiring; wrong pins can compile but look incorrect.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 37
- Description: Allows SubUV blending of multiple texture frames in a single texture. This MipDerivative version is used to fix mipmap artifacts coming from tiling textures.
- Input pins: Texture [FunctionInput_Texture2D], SubImages [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Frame [FunctionInput_Scalar]
- Output pins: RGB, Alpha, UVs

### TextureVariation

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation.TextureVariation`
- Deep graph doc: [TextureVariation](Functions/Engine_MaterialFunctions03_Texturing__TextureVariation.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 9
- Outputs: 5
- Internal nodes: 119
- Description: Gives Texture UV variation that can be used to break up tiling of repetitive textures. It is designed to use displacement maps to create an intersection based on height, and then does a temporal dither. This avoids needing to pass in every texture to be randomized as a texture object and instead the "Shifted UVs" output can be used for regular texture samples. Make sure to set texture samples to MipValueMode: Derivative. Additional Parameters such as Dithering and Scale Variance can be adjusted by creating a material instance of this material.
- Input pins: Heightmap [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], Variation Scale [FunctionInput_Scalar], Variation Levels [FunctionInput_Scalar], Heightmap Influence [FunctionInput_Scalar], Random Rotation and Scale [FunctionInput_StaticBool], Use Dither [FunctionInput_StaticBool], HQ Edge Comparison [FunctionInput_StaticBool], Mask Channel [FunctionInput_Vector4]
- Output pins: Shifted UVs, Raw UVs, DDX, Random Offset, DDY

### TextureVariation_RotateNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation_RotateNormals.TextureVariation_RotateNormals`
- Deep graph doc: [TextureVariation_RotateNormals](Functions/Engine_MaterialFunctions03_Texturing__TextureVariation_RotateNormals.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 12
- Input pins: Vector [FunctionInput_Vector2], Normal [FunctionInput_Vector3]
- Output pins: Result

### TextureVariation_RotateUV

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation_RotateUV.TextureVariation_RotateUV`
- Deep graph doc: [TextureVariation_RotateUV](Functions/Engine_MaterialFunctions03_Texturing__TextureVariation_RotateUV.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 14
- Input pins: UVs [FunctionInput_Vector2], Vector [FunctionInput_Vector2], Offset [FunctionInput_Vector2]
- Output pins: Result

### TwoSidedTexturing

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/TwoSidedTexturing.TwoSidedTexturing`
- Deep graph doc: [TwoSidedTexturing](Functions/Engine_MaterialFunctions03_Texturing__TwoSidedTexturing.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 9
- Description: Gives independent texturing control for both sides of a two-sided material.  Will have no effect on non-two sided materials.
- Input pins: Texture Side B [FunctionInput_Vector3], Texture Side A [FunctionInput_Vector3]
- Output pins: Texture Blend Output, Mask

### WorldCoordinate3Way

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/WorldCoordinate3Way.WorldCoordinate3Way`
- Deep graph doc: [WorldCoordinate3Way](Functions/Engine_MaterialFunctions03_Texturing__WorldCoordinate3Way.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 13
- Outputs: 4
- Internal nodes: 44
- Description: Takes textures and projects them in world-space onto the surface.
- Input pins: XY Scale [FunctionInput_Scalar], XZ Scale [FunctionInput_Scalar], YZ Scale [FunctionInput_Scalar], XY Texture [FunctionInput_Texture2D], XZ Texture [FunctionInput_Texture2D], YZ Texture [FunctionInput_Texture2D], Normal [FunctionInput_Vector3], Blend Exponent Y Z [FunctionInput_Scalar], Single Texture [FunctionInput_StaticBool], Blend Exponent X [FunctionInput_Scalar], Blend Mult X [FunctionInput_Scalar], Blend Mult Y Z [FunctionInput_Scalar]
- Output pins: XYZ Output, XY, XZ, YZ

