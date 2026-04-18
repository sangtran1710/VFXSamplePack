# Engine_MaterialFunctions02/UVs

Engine: Unreal Engine 5.3
Function count: 8

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### BoundingBoxBased_0-1_UVW

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/BoundingBoxBased_0-1_UVW.BoundingBoxBased_0-1_UVW`
- Deep graph doc: [BoundingBoxBased_0-1_UVW](Functions/Engine_MaterialFunctions02_UVs__BoundingBoxBased_0-1_UVW.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 4
- Internal nodes: 10
- Description: Creates a locally aligned 0-1 value on XY and Z.
- Output pins: RGB, R, G, B

### ObjectAlignedVirtualPlaneCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/ObjectAlignedVirtualPlaneCoordinates.ObjectAlignedVirtualPlaneCoordinates`
- Deep graph doc: [ObjectAlignedVirtualPlaneCoordinates](Functions/Engine_MaterialFunctions02_UVs__ObjectAlignedVirtualPlaneCoordinates.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 2
- Internal nodes: 50
- Description: Draws 2d coordinate at a specified alignment and position.
- Input pins: Coordinate Scale (WS) [FunctionInput_Scalar], World Space Projection Normal [FunctionInput_Vector3], World Space Projection Tangent [FunctionInput_Vector3], World Space Projection Binormal [FunctionInput_Vector3], Projection Pivot Position [FunctionInput_Vector3], Plane Depth Offset [FunctionInput_Scalar]
- Output pins: Center Aligned UV Plane, Non Biased Coordinates

### PanTextureCoordinateChannelfrom-1ton+1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/PanTextureCoordinateChannelfrom-1toN+1.PanTextureCoordinateChannelfrom-1ton+1`
- Deep graph doc: [PanTextureCoordinateChannelfrom-1ton+1](Functions/Engine_MaterialFunctions02_UVs__PanTextureCoordinateChannelfrom-1ton+1.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 12
- Description: This function pans a 0-1 texture coordinate channel from -1 to the number of tiled units +1. This is useful when used in conjunction with textures with clamped coordinates.
- Input pins: UV Channel [FunctionInput_Scalar], Tiling Amount [FunctionInput_Scalar], Time (0-1) [FunctionInput_Scalar]
- Output pins: UV Channel output

### PanTextureCoordinateFrom-1toN+1

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/PanTextureCoordinateFrom-1toN+1.PanTextureCoordinateFrom-1toN+1`
- Deep graph doc: [PanTextureCoordinateFrom-1toN+1](Functions/Engine_MaterialFunctions02_UVs__PanTextureCoordinateFrom-1toN+1.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 12
- Description: This function pans 0-1 uv coordinates from -1 to the number of tiled units +1. This is useful when used in conjunction with textures with clamped coordinates.
- Input pins: UV Tiling [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Time (0-1) [FunctionInput_Vector2]
- Output pins: UVs

### UVBrickPatterns

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/UVBrickPatterns.UVBrickPatterns`
- Deep graph doc: [UVBrickPatterns](Functions/Engine_MaterialFunctions02_UVs__UVBrickPatterns.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 21
- Description: Takes coordinates and manipulates thems to form an offset grid patten much like bricks. 
- Input pins: Float 2 Coordinates [FunctionInput_Vector2], Offset X (True) Or Offset Y (False) [FunctionInput_StaticBool], Offset Percentage [FunctionInput_Scalar]
- Output pins: Non-frac, Frac

### WorldPositionBehindFromDepth_experimental

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/WorldPositionBehindFromDepth_experimental.WorldPositionBehindFromDepth_experimental`
- Deep graph doc: [WorldPositionBehindFromDepth_experimental](Functions/Engine_MaterialFunctions02_UVs__WorldPositionBehindFromDepth_experimental.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 19
- Description: This will recreate the scenes world position behind a translucent surface. 
- Input pins: Exclude WPO Offsets [FunctionInput_StaticBool]
- Output pins: Result

### WorldPositionBehindTranslucency

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/WorldPositionBehindTranslucency.WorldPositionBehindTranslucency`
- Deep graph doc: [WorldPositionBehindTranslucency](Functions/Engine_MaterialFunctions02_UVs__WorldPositionBehindTranslucency.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 20
- Description: This will recreate the scenes world position behind a translucent surface. 
- Input pins: Exclude WPO Offsets [FunctionInput_StaticBool], SceneDepth [FunctionInput_Scalar]
- Output pins: Result

### WorldSpaceAlignedScreenCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions02/UVs/WorldSpaceAlignedScreenCoordinates.WorldSpaceAlignedScreenCoordinates`
- Deep graph doc: [WorldSpaceAlignedScreenCoordinates](Functions/Engine_MaterialFunctions02_UVs__WorldSpaceAlignedScreenCoordinates.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 21
- Description: Moves the center of screen aligned coordinates to the center of a world position. A second output scales the results based on the users fov and view size to maintain a consistent look across multiple resolutions.
- Input pins: Center Point World Position [FunctionInput_Vector3]
- Output pins: Offset UVs, Offset and Scaled UVS

