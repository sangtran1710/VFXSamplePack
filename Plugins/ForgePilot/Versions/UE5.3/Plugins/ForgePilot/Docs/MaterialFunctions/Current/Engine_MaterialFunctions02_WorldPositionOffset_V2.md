# Engine_MaterialFunctions02/WorldPositionOffset/V2

Engine: Unreal Engine 5.3
Function count: 2

## Agent Use

Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### ComponentBasisVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/V2/ComponentBasisVectors.ComponentBasisVectors`
- Deep graph doc: [ComponentBasisVectors](Functions/Engine_MaterialFunctions02_WorldPositionOffset_V2__ComponentBasisVectors.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 3
- Internal nodes: 9
- Description: The local XYZ component transform vectors in world space.
- Output pins: X, Y, Z

### ObjectScale

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/V2/ObjectScale.ObjectScale`
- Deep graph doc: [ObjectScale](Functions/Engine_MaterialFunctions02_WorldPositionOffset_V2__ObjectScale.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 4
- Internal nodes: 15
- Description: Returns the object's XYZ scale together and seperately.This is not compatible with the pixel shader. 
- Output pins: Scale Z, Scale X, Scale Y, Scale XYZ



