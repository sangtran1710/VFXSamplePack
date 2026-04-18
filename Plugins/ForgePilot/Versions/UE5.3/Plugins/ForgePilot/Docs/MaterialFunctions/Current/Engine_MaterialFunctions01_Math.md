# Engine_MaterialFunctions01/Math

Engine: Unreal Engine 5.3
Function count: 6

## Agent Use

Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### BoxIntersection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/BoxIntersection.BoxIntersection`
- Deep graph doc: [BoxIntersection](Functions/Engine_MaterialFunctions01_Math__BoxIntersection.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 5
- Internal nodes: 32
- Description: Returns Entry and Exit points for a Ray intersecting an Axis Aligned Bounding Box. If you need it to be non axis aligned then you will have to Inverse Transform the Ray Origin and Ray End into the local space of the box.
- Input pins: Ray Origin [FunctionInput_Vector3], Ray Direction [FunctionInput_Vector3], Box Min [FunctionInput_Vector3], Box Max [FunctionInput_Vector3]
- Output pins: Ray Entry Position, Ray Exit Position, Box Thickness, T1, T0

### CylinderIntersection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/CylinderIntersection.CylinderIntersection`
- Deep graph doc: [CylinderIntersection](Functions/Engine_MaterialFunctions01_Math__CylinderIntersection.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 7
- Internal nodes: 145
- Description: Returns an analytical intersection for a cylinder of the specified coordinates.
- Input pins: Origin [FunctionInput_Vector3], Z Axis [FunctionInput_Vector3], X Axis [FunctionInput_Vector3], Radius [FunctionInput_Scalar], XY Scale [FunctionInput_Vector2], Capped? [FunctionInput_StaticBool], Z Min-Max [FunctionInput_Vector2]
- Output pins: Ray Entry Position, Normal, Opacity Mask, Cylinder Thickness, T0, T1, Ray Exit Position

### DegreesToRadians

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/DegreesToRadians.DegreesToRadians`
- Deep graph doc: [DegreesToRadians](Functions/Engine_MaterialFunctions01_Math__DegreesToRadians.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Converts from Degrees to Radians.
- Input pins: Degrees [FunctionInput_Scalar]
- Output pins: Radians

### QuadraticFormula

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/QuadraticFormula.QuadraticFormula`
- Deep graph doc: [QuadraticFormula](Functions/Engine_MaterialFunctions01_Math__QuadraticFormula.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 17
- Description: Solved a quadratic equation of the form ax^2 + bx + c = 0.
- Input pins: A [FunctionInput_Scalar], B [FunctionInput_Scalar], C [FunctionInput_Scalar]
- Output pins: T0, T1, Discriminant

### RadiansToDegrees

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/RadiansToDegrees.RadiansToDegrees`
- Deep graph doc: [RadiansToDegrees](Functions/Engine_MaterialFunctions01_Math__RadiansToDegrees.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Converts from Radians to Degrees.
- Input pins: Radians [FunctionInput_Scalar]
- Output pins: Degrees

### SmoothCeil

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Math/SmoothCeil.SmoothCeil`
- Deep graph doc: [SmoothCeil](Functions/Engine_MaterialFunctions01_Math__SmoothCeil.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 18
- Description: Smooth ceil is like ceil but gives a smooth ramp before each integer value.
- Input pins: In [FunctionInput_Scalar], Ramp Width [FunctionInput_Scalar]
- Output pins: Result



