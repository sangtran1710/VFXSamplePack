# Engine_MaterialFunctions01/Coordinates

Engine: Unreal Engine 5.3
Function count: 13

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### 1Dto2DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/1Dto2DIndex.1Dto2DIndex`
- Deep graph doc: [1Dto2DIndex](Functions/Engine_MaterialFunctions01_Coordinates__1Dto2DIndex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 14
- Description: Converts a 2D index into a 1D index based on a given total X,Y size. For example an 8x8 grid would return cells 0-63.
- Input pins: 1D Index [FunctionInput_Scalar], Cells XY [FunctionInput_Vector2]
- Output pins: 2D Index, Centered UV Position

### 1Dto3DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/1Dto3DIndex.1Dto3DIndex`
- Deep graph doc: [1Dto3DIndex](Functions/Engine_MaterialFunctions01_Coordinates__1Dto3DIndex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 19
- Description: Converts a 2D index into a 1D index based on a given total X,Y size. For example an 8x8 grid would return cells 0-63.
- Input pins: 1D Index [FunctionInput_Scalar], Cells XYZ [FunctionInput_Vector3]
- Output pins: 3D Index, Centered UV Position

### 2Dto1DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/2Dto1DIndex.2Dto1DIndex`
- Deep graph doc: [2Dto1DIndex](Functions/Engine_MaterialFunctions01_Coordinates__2Dto1DIndex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 12
- Description: Converts a 2D index into a 1D index based on a given total X,Y size. For example an 8x8 grid would return cells 0-63.
- Input pins: 2D Index [FunctionInput_Vector2], Columns X [FunctionInput_Vector2]
- Output pins: 1D Index, Centered UV Position

### 3Dto1DIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/3Dto1DIndex.3Dto1DIndex`
- Deep graph doc: [3Dto1DIndex](Functions/Engine_MaterialFunctions01_Coordinates__3Dto1DIndex.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 15
- Description: Converts a 3D index into a 1D index based on a given total X,Y,Z size. For example an 3x3x3 grid would return cells 0-26.
- Input pins: 3D Index [FunctionInput_Vector3], Cells XYZ [FunctionInput_Vector3]
- Output pins: 1D Index, Centered UVW Position

### DrawLine-2D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/DrawLine-2D.DrawLine-2D`
- Deep graph doc: [DrawLine-2D](Functions/Engine_MaterialFunctions01_Coordinates__DrawLine-2D.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 57
- Description: Draws a line between 2 points using supplied 2D coordinates. 
- Input pins: Coordinates [FunctionInput_Vector2], P0 [FunctionInput_Vector2], P1 [FunctionInput_Vector2], Line Width [FunctionInput_Scalar], Edge Falloff [FunctionInput_Scalar], Square Corners? [FunctionInput_StaticBool]
- Output pins: Result

### DrawLine-3D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/DrawLine-3D.DrawLine-3D`
- Deep graph doc: [DrawLine-3D](Functions/Engine_MaterialFunctions01_Coordinates__DrawLine-3D.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 62
- Description: Draws a line between 2 points using supplied 3D coordinates. 
- Input pins: Coordinates [FunctionInput_Vector3], P0 [FunctionInput_Vector3], P1 [FunctionInput_Vector3], Line Width [FunctionInput_Scalar], Edge Falloff [FunctionInput_Scalar], Square Corners? [FunctionInput_StaticBool]
- Output pins: Result

### LongLatToUV

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/LongLatToUV.LongLatToUV`
- Deep graph doc: [LongLatToUV](Functions/Engine_MaterialFunctions01_Coordinates__LongLatToUV.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 4
- Description: This node transforms a directional vector into a UV value for use in Long/Lat coordinates
- Input pins: Vector [FunctionInput_Vector3]
- Output pins: Result

### PixelInWorldUnits

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/PixelInWorldUnits.PixelInWorldUnits`
- Deep graph doc: [PixelInWorldUnits](Functions/Engine_MaterialFunctions01_Coordinates__PixelInWorldUnits.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 14
- Input pins: Depth [FunctionInput_Scalar], Pixel [FunctionInput_Vector2]
- Output pins: PixelInWorldUnits

### UVRemap_0-1_ToRange

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/UVRemap_0-1_ToRange.UVRemap_0-1_ToRange`
- Deep graph doc: [UVRemap_0-1_ToRange](Functions/Engine_MaterialFunctions01_Coordinates__UVRemap_0-1_ToRange.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 4
- Internal nodes: 20
- Description: Remaps UVs from 0-1 to the specified range. Useful for when you want control of range rather than tiling.
- Input pins: X range Y range [FunctionInput_Vector4], UVs [FunctionInput_Vector2]
- Output pins: UV, x, y, Range

### UVToLongLat

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/UVToLongLat.UVToLongLat`
- Deep graph doc: [UVToLongLat](Functions/Engine_MaterialFunctions01_Coordinates__UVToLongLat.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 8
- Description: This node transforms a UV value into a directional vector which can be used to sample a Cubemap texture as a regular 2D texture.
- Input pins: UV [FunctionInput_Vector2]
- Output pins: Result

### VirtualPlaneCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/VirtualPlaneCoordinates.VirtualPlaneCoordinates`
- Deep graph doc: [VirtualPlaneCoordinates](Functions/Engine_MaterialFunctions01_Coordinates__VirtualPlaneCoordinates.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 34
- Description: Virtual coordinates for a raytraced plane, in world units
- Input pins: Plane Normal Axis [FunctionInput_Vector3], Plane Center [FunctionInput_Vector3], UV Scale [FunctionInput_Vector2], Plane X Axis [FunctionInput_Vector3]
- Output pins: World Position, UVs, World Position - Centered

### WorldPosition-XY

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/WorldPosition-XY.WorldPosition-XY`
- Deep graph doc: [WorldPosition-XY](Functions/Engine_MaterialFunctions01_Coordinates__WorldPosition-XY.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 5
- Description: WorldPosition XY Coordinates with Scale input.
- Input pins: Scale [FunctionInput_Scalar]
- Output pins: Result

### WorldUnitsInPixel

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/WorldUnitsInPixel.WorldUnitsInPixel`
- Deep graph doc: [WorldUnitsInPixel](Functions/Engine_MaterialFunctions01_Coordinates__WorldUnitsInPixel.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 11
- Input pins: Depth [FunctionInput_Scalar], WorldUnits [FunctionInput_Scalar]
- Output pins: PIxels



