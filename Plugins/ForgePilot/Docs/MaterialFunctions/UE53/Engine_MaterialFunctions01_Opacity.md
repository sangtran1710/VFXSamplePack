# Engine_MaterialFunctions01/Opacity

Engine: Unreal Engine 5.3
Function count: 4

## Agent Use

Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### AlphaOffset

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/AlphaOffset.AlphaOffset`
- Deep graph doc: [AlphaOffset](Functions/Engine_MaterialFunctions01_Opacity__AlphaOffset.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Applies an offset to alpha while keeping the range 0 to 1. Useful for blurred alphas that need to shrink.
- Input pins: Alpha [FunctionInput_Scalar], Offset [FunctionInput_Scalar]
- Output pins: Result

### CameraDepthFade

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/CameraDepthFade.CameraDepthFade`
- Deep graph doc: [CameraDepthFade](Functions/Engine_MaterialFunctions01_Opacity__CameraDepthFade.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 21
- Description: Creates a gradient of 0 near the camera to white at Fade Length. Useful for preventing particles from camera clipping.0
- Input pins: Fade Length [FunctionInput_Scalar], Fade Offset [FunctionInput_Scalar], For Vertex Shader [FunctionInput_StaticBool]
- Output pins: Result

### Chroma_Key_Alpha

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/Chroma_Key_Alpha.Chroma_Key_Alpha`
- Deep graph doc: [Chroma_Key_Alpha](Functions/Engine_MaterialFunctions01_Opacity__Chroma_Key_Alpha.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 3
- Internal nodes: 29
- Description: Extracts an alpha mask from a chroma key aka green screen input image.
- Input pins: Image Color [FunctionInput_Vector3], Chroma Color [FunctionInput_Vector3], Alpha Cutoff Min [FunctionInput_Scalar], Alpha Cutoff Max [FunctionInput_Scalar], Alpha Exponent [FunctionInput_Scalar], Despill Cuttoff Max [FunctionInput_Scalar], Despill Exponent [FunctionInput_Scalar], LumaMask [FunctionInput_Scalar]
- Output pins: Alpha, Raw Comparison, Despill Alpha

### SoftOpacity

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Opacity/SoftOpacity.SoftOpacity`
- Deep graph doc: [SoftOpacity](Functions/Engine_MaterialFunctions01_Opacity__SoftOpacity.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 15
- Description: Softens the Opacity input by multiplying against Fresnel, DepthBiasedAlpha and Pixel Depth
- Input pins: OpacityIn [FunctionInput_Scalar], FadeDistance [FunctionInput_Scalar], DepthFadeDistance [FunctionInput_Scalar]
- Output pins: OutputUsesDepthBias, OutputNoDepthBias

