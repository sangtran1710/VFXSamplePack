# Engine_MaterialFunctions01/SpeedTree

Engine: Unreal Engine 5.3
Function count: 3

## Agent Use

Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### SpeedTreeBillboardNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions01/SpeedTree/SpeedTreeBillboardNormals.SpeedTreeBillboardNormals`
- Deep graph doc: [SpeedTreeBillboardNormals](Functions/Engine_MaterialFunctions01_SpeedTree__SpeedTreeBillboardNormals.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 12
- Description: Move normals into screen space for smoother billboard lighting
- Input pins: Normal [FunctionInput_Vector3]
- Output pins: NormalWS

### SpeedTreeColorVariation

- Path: `/Engine/Functions/Engine_MaterialFunctions01/SpeedTree/SpeedTreeColorVariation.SpeedTreeColorVariation`
- Deep graph doc: [SpeedTreeColorVariation](Functions/Engine_MaterialFunctions01_SpeedTree__SpeedTreeColorVariation.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 19
- Description: Adds a color variation per object/instance to break up instances in a large forest
- Input pins: BaseColor [FunctionInput_Vector3], Amount [FunctionInput_Scalar]
- Output pins: Result

### SpeedTreeCrossfadeBillboard

- Path: `/Engine/Functions/Engine_MaterialFunctions01/SpeedTree/SpeedTreeCrossfadeBillboard.SpeedTreeCrossfadeBillboard`
- Deep graph doc: [SpeedTreeCrossfadeBillboard](Functions/Engine_MaterialFunctions01_SpeedTree__SpeedTreeCrossfadeBillboard.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 25
- Description: Handles crossfading between billboard faces
- Input pins: Opacity [FunctionInput_Scalar]
- Output pins: OpacityMask, CustomUV



