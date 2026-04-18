# Engine_MaterialFunctions01/Density

Engine: Unreal Engine 5.3
Function count: 2

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### ExponentialDensity

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Density/ExponentialDensity.ExponentialDensity`
- Deep graph doc: [ExponentialDensity](Functions/Engine_MaterialFunctions01_Density__ExponentialDensity.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 15
- Description: Uses the D3DFOG_EXP function to map exponential density. The result is inverted so that 0 is returned when depth = 0 and approaches 1 as depth increases.
- Input pins: Depth [FunctionInput_Scalar], Density [FunctionInput_Scalar], Use Exp2 [FunctionInput_StaticBool]
- Output pins: Result

### RayMarchHeightMap

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Density/RayMarchHeightMap.RayMarchHeightMap`
- Deep graph doc: [RayMarchHeightMap](Functions/Engine_MaterialFunctions01_Density__RayMarchHeightMap.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 12
- Outputs: 3
- Internal nodes: 67
- Description: This function Ray Marches a heightmap to return shadow density.
- Input pins: Texture Object [FunctionInput_Texture2D], UVs [FunctionInput_Vector2], Light Vector WorldSpace [FunctionInput_Vector3], Max Steps [FunctionInput_Scalar], Trace Distance [FunctionInput_Scalar], Start Bias [FunctionInput_Scalar], Height Scale [FunctionInput_Scalar], Shadow Density [FunctionInput_Scalar], SubUV Frames [FunctionInput_Vector2], Animation Phase [FunctionInput_Scalar], Temporal Jitter [FunctionInput_StaticBool], Heightmap Channel [FunctionInput_Vector4]
- Output pins: Light Energy, Step Complexity, Raw Heightmap Value

