# Engine_MaterialFunctions02/Normals

Engine: Unreal Engine 5.3
Function count: 3

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### DynamicNormalFromDistanceField

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Normals/DynamicNormalFromDistanceField.DynamicNormalFromDistanceField`
- Deep graph doc: [DynamicNormalFromDistanceField](Functions/Engine_MaterialFunctions02_Normals__DynamicNormalFromDistanceField.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 4
- Internal nodes: 40
- Description: Generate animated normals from offset distance fields using this material function. A normal lip will form as the distance field errodes away using a user specified alpha value. Note: multiple distance fields can dynamically be combined together using a "min" operation to generate fluid results. If you wish to invert the results invert the distance field before feeding it into the function. 
- Input pins: Normal Lip Curvature [FunctionInput_Scalar], Normal Lip Tightness [FunctionInput_Scalar], Right Shifted Distance Field [FunctionInput_Scalar], Dissolve Alpha [FunctionInput_Scalar], Downward Shifted Distance Field [FunctionInput_Scalar], Unshifted distance field [FunctionInput_Scalar]
- Output pins: Normal, Mask, Normal XY (Advanced), Thresholded Gradients (Advanced)

### RotateNormalWhileMasking

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Normals/RotateNormalWhileMasking.RotateNormalWhileMasking`
- Deep graph doc: [RotateNormalWhileMasking](Functions/Engine_MaterialFunctions02_Normals__RotateNormalWhileMasking.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 14
- Description: This will rotate a normal toward it's slope as the greyscale input increases. This is useful for thresholding a normal map and generating a lip around the newly masked areas.
- Input pins: Hp Normal [FunctionInput_Vector3], LowPolyNormal [FunctionInput_Vector3], Grey Scale Mask Amount [FunctionInput_Scalar]
- Output pins: Normal

### UnpackNormalFromFloat

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Normals/UnpackNormalFromFloat.UnpackNormalFromFloat`
- Deep graph doc: [UnpackNormalFromFloat](Functions/Engine_MaterialFunctions02_Normals__UnpackNormalFromFloat.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 78
- Description: Unpack Normal/Float3 values from a scalar value
- Input pins: Scalar Value [FunctionInput_Scalar]
- Output pins: XXY.YZZ Vector, XXY.YZZ Normalized Vector, ( z sign ) XXX.YYY Normal, ( z sign ) XXYY.0

