# Engine_MaterialFunctions01/Vectors

Engine: Unreal Engine 5.3
Function count: 5

## Agent Use

Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### MF_RotateVector_90

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/MF_RotateVector_90.MF_RotateVector_90`
- Deep graph doc: [MF_RotateVector_90](Functions/Engine_MaterialFunctions01_Vectors__MF_RotateVector_90.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 6
- Internal nodes: 25
- Description: Rotates vectors along any cardinal axis (Right hand coordinates)
- Input pins: Vector [FunctionInput_Vector3]
- Output pins: CW X, CCW X, CW Y, CCW Y, CW Z, CCW Z

### OctahedronToUnitVector

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/OctahedronToUnitVector.OctahedronToUnitVector`
- Deep graph doc: [OctahedronToUnitVector](Functions/Engine_MaterialFunctions01_Vectors__OctahedronToUnitVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Unpacks a v2 Octahedron to a v3 vector.
- Input pins: Octahedron [FunctionInput_Vector2]
- Output pins: Unit Vector

### Refract

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/Refract.Refract`
- Deep graph doc: [Refract](Functions/Engine_MaterialFunctions01_Vectors__Refract.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 7
- Description: Calculates vector based Refraction using the hlsl intrinsic function.
- Input pins: Ray Direction [FunctionInput_Vector3], Surface Normal [FunctionInput_Vector3], Refractive Index Origin [FunctionInput_Scalar], Refractive Index Target [FunctionInput_Scalar]
- Output pins: Vector

### RotateVector

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/RotateVector.RotateVector`
- Deep graph doc: [RotateVector](Functions/Engine_MaterialFunctions01_Vectors__RotateVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 14
- Description: Rotates an input vector to point toward a given direction.
- Input pins: LookAtVector [FunctionInput_Vector3], VectorToRotate [FunctionInput_Vector3], RestingVector [FunctionInput_Vector3]
- Output pins: Result

### UnitVectorToOctahedron

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Vectors/UnitVectorToOctahedron.UnitVectorToOctahedron`
- Deep graph doc: [UnitVectorToOctahedron](Functions/Engine_MaterialFunctions01_Vectors__UnitVectorToOctahedron.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Packs a v3 vector to a v2 Octahedron.
- Input pins: Vector [FunctionInput_Vector3]
- Output pins: Octahedron



