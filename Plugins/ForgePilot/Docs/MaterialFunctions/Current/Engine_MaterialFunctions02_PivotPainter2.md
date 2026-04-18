# Engine_MaterialFunctions02/PivotPainter2

Engine: Unreal Engine 5.7
Function count: 9

## Agent Use

Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### 2dArrayLookupByIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/2dArrayLookupByIndex.2dArrayLookupByIndex`
- Deep graph doc: [2dArrayLookupByIndex](Functions/Engine_MaterialFunctions02_PivotPainter2__2dArrayLookupByIndex.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 12
- Description: Finds an index's location in a 2d Array.
- Input pins: Array Dimensions [FunctionInput_Vector2], Index [FunctionInput_Scalar]
- Output pins: UVs

### ms_PivotPainter2_CalculateMeshElementIndex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_CalculateMeshElementIndex.ms_PivotPainter2_CalculateMeshElementIndex`
- Deep graph doc: [ms_PivotPainter2_CalculateMeshElementIndex](Functions/Engine_MaterialFunctions02_PivotPainter2__ms_PivotPainter2_CalculateMeshElementIndex.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 10
- Description: Pulls the model elements element ID from the models uvs.
- Input pins: Pivot Painter UV Coordinates [FunctionInput_Vector2], Data Texture Dimensions [FunctionInput_Vector2]
- Output pins: Index

### ms_PivotPainter2_Decode8BitAlphaAxisExtent

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_Decode8BitAlphaAxisExtent.ms_PivotPainter2_Decode8BitAlphaAxisExtent`
- Deep graph doc: [ms_PivotPainter2_Decode8BitAlphaAxisExtent](Functions/Engine_MaterialFunctions02_PivotPainter2__ms_PivotPainter2_Decode8BitAlphaAxisExtent.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 4
- Description: Rescale 8 bit axis extent texture data information from Pivot Painter 2 Into world space data.
- Input pins: 8 Bit Alpha Extent Value [FunctionInput_Scalar]
- Output pins: Rescaled Extent

### ms_PivotPainter2_DecodeAxisVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_DecodeAxisVector.ms_PivotPainter2_DecodeAxisVector`
- Deep graph doc: [ms_PivotPainter2_DecodeAxisVector](Functions/Engine_MaterialFunctions02_PivotPainter2__ms_PivotPainter2_DecodeAxisVector.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 5
- Description: Transforms Pivot Painter 2.0's local space vector info into world space vectors.
- Input pins: Axis Vector RGB [FunctionInput_Vector3]
- Output pins: Result

### ms_PivotPainter2_DecodePostion

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_DecodePostion.ms_PivotPainter2_DecodePostion`
- Deep graph doc: [ms_PivotPainter2_DecodePostion](Functions/Engine_MaterialFunctions02_PivotPainter2__ms_PivotPainter2_DecodePostion.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Transforms Pivot Painter 2.0's local space position information into world position info.
- Input pins: Position RGB [FunctionInput_Vector3]
- Output pins: Result

### ms_PivotPainter2_ReturnParentTextureInfo

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_ReturnParentTextureInfo.ms_PivotPainter2_ReturnParentTextureInfo`
- Deep graph doc: [ms_PivotPainter2_ReturnParentTextureInfo](Functions/Engine_MaterialFunctions02_PivotPainter2__ms_PivotPainter2_ReturnParentTextureInfo.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 10
- Description: Read a parent sub object's texture data using Pivot Painter 2.0 parent index integer as float texture data.
- Input pins: Parent Index As Float (See note) [FunctionInput_Scalar], Texture Dimensions [FunctionInput_Vector2], Current Index (see note) [FunctionInput_Vector3]
- Output pins: Parent UVs, Is Child? (See note)

### ms_PivotPainter2_SampleLayerData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/ms_PivotPainter2_SampleLayerData.ms_PivotPainter2_SampleLayerData`
- Deep graph doc: [ms_PivotPainter2_SampleLayerData](Functions/Engine_MaterialFunctions02_PivotPainter2__ms_PivotPainter2_SampleLayerData.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 5
- Internal nodes: 19
- Input pins: UVs [FunctionInput_Vector2], Texture resolution (ParentUVs) [FunctionInput_Vector2], Current Index (ParentUVs) [FunctionInput_Scalar]
- Output pins: Parent UVs, X axis vector and X extent, Position and Parent Index, Parent Index, Is Child?

### PivotPainter2FoliageAnimation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/PivotPainter2FoliageAnimation.PivotPainter2FoliageAnimation`
- Deep graph doc: [PivotPainter2FoliageAnimation](Functions/Engine_MaterialFunctions02_PivotPainter2__PivotPainter2FoliageAnimation.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 22
- Outputs: 6
- Internal nodes: 139
- Input pins: Texture Dimensions To Gather Parent UVs [FunctionInput_Vector2], Wind Gust Offset [FunctionInput_Scalar], Wind Gust Angle Rotation [FunctionInput_Scalar], Motion Dampening Falloff Radius [FunctionInput_Scalar], Pivot Painter Texture Coordinate [FunctionInput_Vector2], Wind Speed Down Wind Vector [FunctionInput_Scalar], Optional - world space pixel normal [FunctionInput_Vector3], Layer Mask [FunctionInput_Scalar], Enable [FunctionInput_StaticBool], Enable Normal Rotation [FunctionInput_StaticBool], Wind Shelter Settings [FunctionInput_Vector2], Random Rotation Influence [FunctionInput_Scalar]
- Output pins: Parent Index Integer, World Position Offset, Parent UVs, Rotated World Space Pixel Normal, Final World Position, Advanced - Rotation Angle Animation

### PivotPainter2FoliageShader

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter2/PivotPainter2FoliageShader.PivotPainter2FoliageShader`
- Deep graph doc: [PivotPainter2FoliageShader](Functions/Engine_MaterialFunctions02_PivotPainter2__PivotPainter2FoliageShader.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 175
- Description: This material function contains texture and numeric parameters that should be altered to fit your particular asset. It is suggested that this be done at the material instance level. 
- Input pins: Material Attributes (See Note) [FunctionInput_MaterialAttributes], Pivot Painter Texture Coordinate [FunctionInput_Vector2]
- Output pins: World Position Offset Component, Modified World Space Normal Component, Final Material with World Space Normals



