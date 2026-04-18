# DistanceFields/Shapes

Engine: Unreal Engine 5.3
Function count: 3

## Agent Use

Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.

Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.

## Functions

### DistanceField_Capsule

- Path: `/Engine/Functions/DistanceFields/Shapes/DistanceField_Capsule.DistanceField_Capsule`
- Deep graph doc: [DistanceField_Capsule](Functions/DistanceFields_Shapes__DistanceField_Capsule.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 19
- Description: Draws a line between 2 points using supplied 3D coordinates. 
- Input pins: Coordinates [FunctionInput_Vector3], P0 [FunctionInput_Vector3], P1 [FunctionInput_Vector3], Width [FunctionInput_Scalar]
- Output pins: Result

### DistanceField_Cylinder

- Path: `/Engine/Functions/DistanceFields/Shapes/DistanceField_Cylinder.DistanceField_Cylinder`
- Deep graph doc: [DistanceField_Cylinder](Functions/DistanceFields_Shapes__DistanceField_Cylinder.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 23
- Description: Generates a distance field shape for a capped cylinder.
- Input pins: Coordinates [FunctionInput_Vector3], h [FunctionInput_Scalar], Center [FunctionInput_Vector3]
- Output pins: Result

### DistanceField_Sphere

- Path: `/Engine/Functions/DistanceFields/Shapes/DistanceField_Sphere.DistanceField_Sphere`
- Deep graph doc: [DistanceField_Sphere](Functions/DistanceFields_Shapes__DistanceField_Sphere.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 11
- Description: Draws a line between 2 points using supplied 3D coordinates. 
- Input pins: Coordinates [FunctionInput_Vector3], Radius [FunctionInput_Scalar], Center [FunctionInput_Vector3]
- Output pins: Result



