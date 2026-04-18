# Engine_MaterialFunctions01/Gradient

Engine: Unreal Engine 5.7
Function count: 10

## Agent Use

Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### BellCurve

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/BellCurve.BellCurve`
- Deep graph doc: [BellCurve](Functions/Engine_MaterialFunctions01_Gradient__BellCurve.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 16
- Description: Gives a Bell Curve also known as a Normal Distribution
- Input pins: x [FunctionInput_Scalar]
- Output pins: 0-1, Standard

### BoxMask-2D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/BoxMask-2D.BoxMask-2D`
- Deep graph doc: [BoxMask-2D](Functions/Engine_MaterialFunctions01_Gradient__BoxMask-2D.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 15
- Description: Draws a box in 2D Space. Controls for size and falloff.
- Input pins: A [FunctionInput_Vector2], Bounds [FunctionInput_Vector2], B [FunctionInput_Vector2], Edge Falloff [FunctionInput_Scalar]
- Output pins: Result

### BoxMask-3D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/BoxMask-3D.BoxMask-3D`
- Deep graph doc: [BoxMask-3D](Functions/Engine_MaterialFunctions01_Gradient__BoxMask-3D.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 15
- Description: Draws a box in 3D Space. Controls for size and falloff.
- Input pins: A [FunctionInput_Vector3], Bounds [FunctionInput_Vector3], B [FunctionInput_Vector3], Edge Falloff [FunctionInput_Scalar]
- Output pins: Result

### DiamondGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/DiamondGradient.DiamondGradient`
- Deep graph doc: [DiamondGradient](Functions/Engine_MaterialFunctions01_Gradient__DiamondGradient.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 15
- Description: Uses UV Channel 0 to generate a Diamond Gradient
- Input pins: Falloff [FunctionInput_Scalar]
- Output pins: DiamondGradient

### GeneratedRoundRect

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/GeneratedRoundRect.GeneratedRoundRect`
- Deep graph doc: [GeneratedRoundRect](Functions/Engine_MaterialFunctions01_Gradient__GeneratedRoundRect.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 20
- Description: Draws a box in 2D Space. Controls for size and falloff.
- Input pins: UV Coords [FunctionInput_Vector2], Box Dimensions [FunctionInput_Vector2], Center [FunctionInput_Vector2], CornerRadius [FunctionInput_Scalar], Sharpness [FunctionInput_Scalar]
- Output pins: Result

### LinearGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/LinearGradient.LinearGradient`
- Deep graph doc: [LinearGradient](Functions/Engine_MaterialFunctions01_Gradient__LinearGradient.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 6
- Description: Uses UV Channel 0 to generate a linear gradient along the U or V axis
- Input pins: UV Channel [FunctionInput_Vector2]
- Output pins: VGradient, UGradient

### RadialGradient

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/RadialGradient.RadialGradient`
- Deep graph doc: [RadialGradient](Functions/Engine_MaterialFunctions01_Gradient__RadialGradient.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 5
- Description: Uses UV Channel 0 to create a radial gradient
- Input pins: CenterPosition [FunctionInput_Vector2], Radius [FunctionInput_Scalar]
- Output pins: RadialGradient

### RadialGradientExponential

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/RadialGradientExponential.RadialGradientExponential`
- Deep graph doc: [RadialGradientExponential](Functions/Engine_MaterialFunctions01_Gradient__RadialGradientExponential.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 24
- Description: Creates a radial gradient using the input UV coordinates and applies exponential density with specified density.
- Input pins: CenterPosition [FunctionInput_Vector2], Radius [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Density [FunctionInput_Scalar], Invert Density [FunctionInput_StaticBool]
- Output pins: RadialGradientExponential

### SphereGradient-2D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/SphereGradient-2D.SphereGradient-2D`
- Deep graph doc: [SphereGradient-2D](Functions/Engine_MaterialFunctions01_Gradient__SphereGradient-2D.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 21
- Description: Generates a gradient representing the depth of a shere centered on the desired point. 
- Input pins: CenterPosition [FunctionInput_Vector2], Radius [FunctionInput_Scalar], UVs [FunctionInput_Vector2]
- Output pins: Result 0-1, Result Diameter, Result Radius

### SphereGradient-3D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/SphereGradient-3D.SphereGradient-3D`
- Deep graph doc: [SphereGradient-3D](Functions/Engine_MaterialFunctions01_Gradient__SphereGradient-3D.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 3
- Internal nodes: 82
- Description: Generates a gradient representing the depth of a shere centered on the desired point. 
- Input pins: Radius [FunctionInput_Scalar], Location [FunctionInput_Vector3], Offset [FunctionInput_Vector3], Calculate Camera Inside [FunctionInput_StaticBool], Depth Biased Alpha [FunctionInput_StaticBool], FadeDistance [FunctionInput_Scalar]
- Output pins: Result 0-1, Result Diameter, Result Radius



