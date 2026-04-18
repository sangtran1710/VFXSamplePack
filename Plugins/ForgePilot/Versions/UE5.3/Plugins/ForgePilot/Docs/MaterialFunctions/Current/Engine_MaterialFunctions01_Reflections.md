# Engine_MaterialFunctions01/Reflections

Engine: Unreal Engine 5.3
Function count: 4

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.

## Functions

### DistanceLimitedReflections

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/DistanceLimitedReflections.DistanceLimitedReflections`
- Deep graph doc: [DistanceLimitedReflections](Functions/Engine_MaterialFunctions01_Reflections__DistanceLimitedReflections.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 36
- Description: Distance Limited Reflections give some parallax to cubemaps by fixing them in a point in space and performing a ray sphere intersection. Can be used to help anchor manually placed cubemaps in the world.
- Input pins: CapturePosition [FunctionInput_Vector3], Radius [FunctionInput_Scalar], Cubemap [FunctionInput_TextureCube]
- Output pins: Result, Sphere Hit Mask

### ReflectionVectorSoft

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/ReflectionVectorSoft.ReflectionVectorSoft`
- Deep graph doc: [ReflectionVectorSoft](Functions/Engine_MaterialFunctions01_Reflections__ReflectionVectorSoft.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Softens the normal for a smoother reflection
- Input pins: Normal [FunctionInput_Vector3], Softness [FunctionInput_Scalar]
- Output pins: Result

### ViewAlignedReflection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/ViewAlignedReflection.ViewAlignedReflection`
- Deep graph doc: [ViewAlignedReflection](Functions/Engine_MaterialFunctions01_Reflections__ViewAlignedReflection.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 14
- Description: Alignes a Reflection texture to your view.  Reflection should be a sphere.
- Input pins: ReflectionTexture [FunctionInput_Texture2D], ReflectionVector [FunctionInput_Vector3]
- Output pins: Texture, UVs

### WorldAlignedReflection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/WorldAlignedReflection.WorldAlignedReflection`
- Deep graph doc: [WorldAlignedReflection](Functions/Engine_MaterialFunctions01_Reflections__WorldAlignedReflection.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 17
- Description: Alignes a Reflection texture to your view.  Reflection should be a sphere.
- Input pins: ReflectionTexture [FunctionInput_Texture2D], ReflectionVector [FunctionInput_Vector3]
- Output pins: WorldReflection, WorldReflectionShadowed



