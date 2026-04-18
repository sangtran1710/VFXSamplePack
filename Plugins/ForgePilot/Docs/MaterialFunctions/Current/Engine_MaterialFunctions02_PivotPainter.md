# Engine_MaterialFunctions02/PivotPainter

Engine: Unreal Engine 5.7
Function count: 6

## Agent Use

Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### PivotPainter_HierarchyData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_HierarchyData.PivotPainter_HierarchyData`
- Deep graph doc: [PivotPainter_HierarchyData](Functions/Engine_MaterialFunctions02_PivotPainter__PivotPainter_HierarchyData.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 10
- Internal nodes: 92
- Description: Processes and organizes world position and angle information stored in the model's uvs by the Pivot Painter MAXScript. 
- Input pins: Max Dist for Parent Piv [FunctionInput_Scalar]
- Output pins: Child Piv Position, Parent Piv Position, Parent X Axis Vector, Child X-Axis Vector, Object Scale, Object  Pivot Point, Object Orientation, ------------------------, Black Mask, ------------------------ 

### PivotPainter_PerObjectData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_PerObjectData.PivotPainter_PerObjectData`
- Deep graph doc: [PivotPainter_PerObjectData](Functions/Engine_MaterialFunctions02_PivotPainter__PivotPainter_PerObjectData.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 6
- Internal nodes: 30
- Description: Processes and organizes world position and angle information stored in the model's uvs by the Pivot Painter MAXScript. 
- Output pins: Pivot Position, Custom Alpha Values, Random Value Per Element, X-Axis Vector, Object Scale, Black Mask

### PivotPainter_PerObjectFoliageData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_PerObjectFoliageData.PivotPainter_PerObjectFoliageData`
- Deep graph doc: [PivotPainter_PerObjectFoliageData](Functions/Engine_MaterialFunctions02_PivotPainter__PivotPainter_PerObjectFoliageData.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 12
- Internal nodes: 38
- Description: Processes and organizes world position and angle information stored in the model's uvs by the Pivot Painter MAXScript. 
- Input pins: Optimized for Foliage Placement [FunctionInput_StaticBool], Optimized Vector [FunctionInput_Vector3], Wind Vector [FunctionInput_Vector4]
- Output pins: Piv Position, Custom Alpha Values, Random Value Per Element, Element X-Axis, Element Rot Axis, Normalized Wind Vector, Wind Accumulator, Wind Strength, Uniform Object Scale, Object Scale XYZ, Wind Speed, Object PIvot Location

### PivotPainter_TreeData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_TreeData.PivotPainter_TreeData`
- Deep graph doc: [PivotPainter_TreeData](Functions/Engine_MaterialFunctions02_PivotPainter__PivotPainter_TreeData.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 25
- Internal nodes: 117
- Description: Processes and organizes world position and angle information stored in the model's uvs by the painter script. The outputs starting with (tree) process the models uv information as it would be stored by the  Pivot Painter MAXScript. The outputs starting with (Leaf) process the uv information as it would be stored by the per object pivot painting section of the script.
- Input pins: Max Dist for Parent Piv [FunctionInput_Scalar], WindVector [FunctionInput_Vector3]
- Output pins: Leaf Rot Axis, Leaf Piv Position, Branch Piv Position, Branch Wind Rot Axis, Branch-X Axis Vector, WindSpeed, WindStrength, Leaf X-Axis Vector, Normalized Wind Vector, Object Scale, Object  Pivot Point, Object Orientation

### ProtectFrom0

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/ProtectFrom0.ProtectFrom0`
- Deep graph doc: [ProtectFrom0](Functions/Engine_MaterialFunctions02_PivotPainter__ProtectFrom0.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 13
- Input pins: In [FunctionInput_Scalar]
- Output pins: RGB, R, G, B

### TreeAnimationSines

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/TreeAnimationSines.TreeAnimationSines`
- Deep graph doc: [TreeAnimationSines](Functions/Engine_MaterialFunctions02_PivotPainter__TreeAnimationSines.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 19
- Description: Add time and a gradient to create an animation used for trees
- Input pins: LS4 [FunctionInput_Scalar], Time and Grad [FunctionInput_Scalar], LS3 [FunctionInput_Scalar], LS2 [FunctionInput_Scalar], LS1 [FunctionInput_Scalar]
- Output pins: Result



