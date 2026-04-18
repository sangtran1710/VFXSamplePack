# Engine_MaterialFunctions01/Lighting

Engine: Unreal Engine 5.3
Function count: 2

## Agent Use

Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### GGXSpecular

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Lighting/GGXSpecular.GGXSpecular`
- Deep graph doc: [GGXSpecular](Functions/Engine_MaterialFunctions01_Lighting__GGXSpecular.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 5
- Description: Returns a specular lobe using GGX.
- Input pins: Normal [FunctionInput_Vector3], Light Vector [FunctionInput_Vector3], Roughness [FunctionInput_Scalar]
- Output pins: Result

### PixelDepthOffset_Foliage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Lighting/PixelDepthOffset_Foliage.PixelDepthOffset_Foliage`
- Deep graph doc: [PixelDepthOffset_Foliage](Functions/Engine_MaterialFunctions01_Lighting__PixelDepthOffset_Foliage.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 3
- Internal nodes: 28
- Description: This function helps set up PixelDepthOffset for foliage meshes.
- Input pins: WorldPositionOffset [FunctionInput_Vector3], Dynamic Shadow Distance [FunctionInput_Scalar], Dynamic Shadow Fade Length [FunctionInput_Scalar], Initial Radius [FunctionInput_Scalar], CustomUV Input [FunctionInput_Vector2], Depth Texture [FunctionInput_Scalar]
- Output pins: World Position Offset, Custom UVs, Pixel Depth Offset



