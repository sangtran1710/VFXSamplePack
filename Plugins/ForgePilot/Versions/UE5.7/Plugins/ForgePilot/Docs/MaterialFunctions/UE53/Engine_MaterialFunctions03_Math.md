# Engine_MaterialFunctions03/Math

Engine: Unreal Engine 5.3
Function count: 13

## Agent Use

Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.

Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.

## Functions

### MultiplyVectorWithQuaternion

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/MultiplyVectorWithQuaternion.MultiplyVectorWithQuaternion`
- Deep graph doc: [MultiplyVectorWithQuaternion](Functions/Engine_MaterialFunctions03_Math__MultiplyVectorWithQuaternion.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 21
- Input pins: Quat [FunctionInput_Vector4], Vector [FunctionInput_Vector3], Invert Quaternion [FunctionInput_StaticBool]
- Output pins: Result

### PackTwoNormalizedFloats

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/PackTwoNormalizedFloats.PackTwoNormalizedFloats`
- Deep graph doc: [PackTwoNormalizedFloats](Functions/Engine_MaterialFunctions03_Math__PackTwoNormalizedFloats.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Packs two normalized floats (0-1) onto one single float. Precision is good to 3 decimal places, starts to lose precision at 4.
- Input pins: NormalizedFloatA [FunctionInput_Scalar], NormalizedFloatB [FunctionInput_Scalar]
- Output pins: PackedFloat

### RemapValueRange

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRange.RemapValueRange`
- Deep graph doc: [RemapValueRange](Functions/Engine_MaterialFunctions03_Math__RemapValueRange.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 12
- Description: Generalized remapping function to explicitly remap an input range to an output range.
- Input pins: Input [FunctionInput_Scalar], Input Low [FunctionInput_Scalar], Input High [FunctionInput_Scalar], Target Low [FunctionInput_Scalar], Target High [FunctionInput_Scalar]
- Output pins: Result

### RemapValueRangeNormalized

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRangeNormalized.RemapValueRangeNormalized`
- Deep graph doc: [RemapValueRangeNormalized](Functions/Engine_MaterialFunctions03_Math__RemapValueRangeNormalized.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 8
- Description: Generalized remapping function to explicitly remap an input range to 0 - 1.
- Input pins: Input [FunctionInput_Scalar], Input Low [FunctionInput_Scalar], Input High [FunctionInput_Scalar]
- Output pins: Result

### UnpackTwoNormalizedFloats

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/UnpackTwoNormalizedFloats.UnpackTwoNormalizedFloats`
- Deep graph doc: [UnpackTwoNormalizedFloats](Functions/Engine_MaterialFunctions03_Math__UnpackTwoNormalizedFloats.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 6
- Description: Unpacks a float value encoded by the function PackTwoNormalizedFloats into two normalized floats (0-1) onto one single float. Precision is good to 3 decimal places, starts to lose precision at 4.
- Input pins: PackedFloat [FunctionInput_Scalar]
- Output pins: NormalizedFloatA, NormalizedFloatB

### WithinRange

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRange.WithinRange`
- Deep graph doc: [WithinRange](Functions/Engine_MaterialFunctions03_Math__WithinRange.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 10
- Description: Returns 1 if a value is within the provided range limits. 0 otherwise.
- Input pins: Value [FunctionInput_Scalar], RangeMax [FunctionInput_Scalar], RangeMin [FunctionInput_Scalar]
- Output pins: AllComponentsWithinRange

### WithinRangeFloat2

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRangeFloat2.WithinRangeFloat2`
- Deep graph doc: [WithinRangeFloat2](Functions/Engine_MaterialFunctions03_Math__WithinRangeFloat2.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 13
- Description: Returns 1 if a value is within the provided range limits. 0 otherwise.
- Input pins: Value [FunctionInput_Vector2], RangeMax [FunctionInput_Vector2], RangeMin [FunctionInput_Vector2]
- Output pins: AnyComponentsWithinRange, AllComponentsWithinRange

### WithinRangeFloat3

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRangeFloat3.WithinRangeFloat3`
- Deep graph doc: [WithinRangeFloat3](Functions/Engine_MaterialFunctions03_Math__WithinRangeFloat3.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 14
- Description: Returns 1 if a value is within the provided range limits. 0 otherwise.
- Input pins: Value [FunctionInput_Vector3], RangeMax [FunctionInput_Vector3], RangeMin [FunctionInput_Vector3]
- Output pins: AnyComponentsWithinRange, AllComponentsWithinRange

### WithinRangeFloat4

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WithinRangeFloat4.WithinRangeFloat4`
- Deep graph doc: [WithinRangeFloat4](Functions/Engine_MaterialFunctions03_Math__WithinRangeFloat4.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 19
- Description: Returns 1 if a value is within the provided range limits. 0 otherwise.
- Input pins: Value [FunctionInput_Vector4], RangeMax [FunctionInput_Vector4], RangeMin [FunctionInput_Vector4]
- Output pins: AnyComponentsWithinRange, AllComponentsWithinRange

### WrapFloat

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WrapFloat.WrapFloat`
- Deep graph doc: [WrapFloat](Functions/Engine_MaterialFunctions03_Math__WrapFloat.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Similar to fmod with support for negative floats : returns a value between 0 and Wrap
- Input pins: Value [FunctionInput_Scalar], Wrap [FunctionInput_Scalar]
- Output pins: Result

### WrapFloat2

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WrapFloat2.WrapFloat2`
- Deep graph doc: [WrapFloat2](Functions/Engine_MaterialFunctions03_Math__WrapFloat2.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Similar to fmod with support for negative floats : returns a value between 0 and Wrap
- Input pins: Value [FunctionInput_Vector2], Wrap [FunctionInput_Vector2]
- Output pins: Result

### WrapFloat3

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WrapFloat3.WrapFloat3`
- Deep graph doc: [WrapFloat3](Functions/Engine_MaterialFunctions03_Math__WrapFloat3.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Similar to fmod with support for negative floats : returns a value between 0 and Wrap
- Input pins: Value [FunctionInput_Vector3], Wrap [FunctionInput_Vector3]
- Output pins: Result

### WrapFloat4

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/WrapFloat4.WrapFloat4`
- Deep graph doc: [WrapFloat4](Functions/Engine_MaterialFunctions03_Math__WrapFloat4.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Similar to fmod with support for negative floats : returns a value between 0 and Wrap
- Input pins: Value [FunctionInput_Vector4], Wrap [FunctionInput_Vector4]
- Output pins: Result

