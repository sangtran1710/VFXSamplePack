# Engine_MaterialFunctions02/Gradients

Engine: Unreal Engine 5.3
Function count: 6

## Agent Use

Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### GetGradientMapRow

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/GetGradientMapRow.GetGradientMapRow`
- Deep graph doc: [GetGradientMapRow](Functions/Engine_MaterialFunctions02_Gradients__GetGradientMapRow.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 9
- Description: Gets the row needed for a given gradient index in a gradient atlas map of a given size
- Input pins: Index (0 Based) [FunctionInput_Scalar], Atlas Height [FunctionInput_Scalar], CurveTime [FunctionInput_Scalar]
- Output pins: Result

### GradientMap_Multi

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/GradientMap_Multi.GradientMap_Multi`
- Deep graph doc: [GradientMap_Multi](Functions/Engine_MaterialFunctions02_Gradients__GradientMap_Multi.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 2
- Internal nodes: 17
- Description: Maps a 0-1 value to a color gradient slice. See the default textures for example content.
- Input pins: Number Of Gradients In the Gradient Map [FunctionInput_Scalar], Greyscale Value To Gradient Map [FunctionInput_Scalar], Index (0 Based) [FunctionInput_Scalar], Gradient Texture [FunctionInput_Texture2D]
- Output pins: RGB, A

### GradientMap_Multi_TexObjSamplerType

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/GradientMap_Multi_TexObjSamplerType.GradientMap_Multi_TexObjSamplerType`
- Deep graph doc: [GradientMap_Multi_TexObjSamplerType](Functions/Engine_MaterialFunctions02_Gradients__GradientMap_Multi_TexObjSamplerType.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 2
- Internal nodes: 17
- Description: Maps a 0-1 value to a color gradient slice. See the default textures for example content. (This material function differs from GradientMap_Multi by using the texture's sampler type rather than utilizing shared:wrap)
- Input pins: Number Of Gradients In the Gradient Map [FunctionInput_Scalar], Greyscale Value To Gradient Map [FunctionInput_Scalar], Index (0 Based) [FunctionInput_Scalar], Gradient Texture [FunctionInput_Texture2D]
- Output pins: RGB, A

### RayTraceSphereFalloff

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/RayTraceSphereFalloff.RayTraceSphereFalloff`
- Deep graph doc: [RayTraceSphereFalloff](Functions/Engine_MaterialFunctions02_Gradients__RayTraceSphereFalloff.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 36
- Description: finds the distance to the sphere
- Input pins: Sphere Radius [FunctionInput_Scalar], Sphere Falloff [FunctionInput_Scalar], Ray Origin [FunctionInput_Vector3], Sphere Center [FunctionInput_Vector3], Unit Ray Direction [FunctionInput_Vector3]
- Output pins: Clamped Falloff, Unclamped Results

### SmoothCurve

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/SmoothCurve.SmoothCurve`
- Deep graph doc: [SmoothCurve](Functions/Engine_MaterialFunctions02_Gradients__SmoothCurve.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 21
- Description: Adjust the tangents at x0 and x1 to adjust x's curve.
- Input pins: Tangent 0 [FunctionInput_Scalar], Tangent 1 [FunctionInput_Scalar], X [FunctionInput_Scalar]
- Output pins: Result

### StretchGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/StretchGradient.StretchGradient`
- Deep graph doc: [StretchGradient](Functions/Engine_MaterialFunctions02_Gradients__StretchGradient.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 20
- Description: A 0-1 uv gradient is scaled by a desired amount while retaining the uvs 0 and 1 value locations. It does this by stretching out the gradient at a point of the user's choosing.
- Input pins: Scale Factor [FunctionInput_Scalar], Center Point [FunctionInput_Scalar], 0-1 Gradient [FunctionInput_Scalar]
- Output pins: Resulting Gradient, Clamped area

