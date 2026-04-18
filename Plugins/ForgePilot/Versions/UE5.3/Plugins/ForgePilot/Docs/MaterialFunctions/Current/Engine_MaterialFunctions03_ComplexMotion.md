# Engine_MaterialFunctions03/ComplexMotion

Engine: Unreal Engine 5.3
Function count: 2

## Agent Use

Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### Motion_4WayChaos

- Path: `/Engine/Functions/Engine_MaterialFunctions03/ComplexMotion/Motion_4WayChaos.Motion_4WayChaos`
- Deep graph doc: [Motion_4WayChaos](Functions/Engine_MaterialFunctions03_ComplexMotion__Motion_4WayChaos.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 31
- Description: 4-way diagonal motion
- Input pins: Texture [FunctionInput_Texture2D], Coordinates [FunctionInput_Vector2], Speed [FunctionInput_Scalar], Divisor [FunctionInput_Scalar]
- Output pins: Result

### Motion_4WayChaos_Normal

- Path: `/Engine/Functions/Engine_MaterialFunctions03/ComplexMotion/Motion_4WayChaos_Normal.Motion_4WayChaos_Normal`
- Deep graph doc: [Motion_4WayChaos_Normal](Functions/Engine_MaterialFunctions03_ComplexMotion__Motion_4WayChaos_Normal.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 27
- Description: 4-way diagonal motion, normal maps only
- Input pins: Texture [FunctionInput_Texture2D], Coordinates [FunctionInput_Vector2], Speed [FunctionInput_Scalar], Divisor [FunctionInput_Scalar]
- Output pins: Result



