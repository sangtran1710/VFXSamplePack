# DistanceFields/Combiners

Engine: Unreal Engine 5.3
Function count: 3

## Agent Use

Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.

Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.

## Functions

### DistanceField_Intersection

- Path: `/Engine/Functions/DistanceFields/Combiners/DistanceField_Intersection.DistanceField_Intersection`
- Deep graph doc: [DistanceField_Intersection](Functions/DistanceFields_Combiners__DistanceField_Intersection.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 4
- Description: Returns the intersection of two distance field functions.
- Input pins: Distance Field 1 [FunctionInput_Scalar], Distance Field 2 [FunctionInput_Scalar]
- Output pins: Result

### DistanceField_Subtract

- Path: `/Engine/Functions/DistanceFields/Combiners/DistanceField_Subtract.DistanceField_Subtract`
- Deep graph doc: [DistanceField_Subtract](Functions/DistanceFields_Combiners__DistanceField_Subtract.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 5
- Description: Subtracts Distance Field 2 from Distance Field 1
- Input pins: Distance Field 1 [FunctionInput_Scalar], Distance Field 2 [FunctionInput_Scalar]
- Output pins: Result

### DistanceField_Union

- Path: `/Engine/Functions/DistanceFields/Combiners/DistanceField_Union.DistanceField_Union`
- Deep graph doc: [DistanceField_Union](Functions/DistanceFields_Combiners__DistanceField_Union.md)
- Use: Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 4
- Description: Returns the Union of two distance field shapes.
- Input pins: Distance Field 1 [FunctionInput_Scalar], Distance Field 2 [FunctionInput_Scalar]
- Output pins: Result



