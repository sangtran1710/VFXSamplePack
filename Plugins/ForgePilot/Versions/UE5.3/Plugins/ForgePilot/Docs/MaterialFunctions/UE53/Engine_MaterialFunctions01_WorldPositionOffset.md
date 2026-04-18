# Engine_MaterialFunctions01/WorldPositionOffset

Engine: Unreal Engine 5.3
Function count: 5

## Agent Use

Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### CanopyCreator_Branches

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/CanopyCreator_Branches.CanopyCreator_Branches`
- Deep graph doc: [CanopyCreator_Branches](Functions/Engine_MaterialFunctions01_WorldPositionOffset__CanopyCreator_Branches.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 12
- Outputs: 3
- Internal nodes: 109
- Description: This function  widens strips of polies along the U of their UVs.  These strips shouuld be extremely thin and imported with remove degenerate triangles disabled. Useful for Hair, Grass, Cables etc.
- Input pins: WorldPosition [FunctionInput_Vector3], UVs for Projection [FunctionInput_Vector2], WidthBase [FunctionInput_Scalar], WidthTip [FunctionInput_Scalar], Expand U or V UV Channel [FunctionInput_StaticBool], AdditionalNormal [FunctionInput_Vector3], UVs for Thickness [FunctionInput_Vector2], DeriveNormalZ [FunctionInput_StaticBool], AngleCorrectedNormals [FunctionInput_StaticBool], UVs for texturing [FunctionInput_Vector2], FlattenPixelNormal [FunctionInput_StaticBool], UVs For NormalShading [FunctionInput_Vector2]
- Output pins: Normal, WorldPositionOffset, UVs with Parallax

### ConstantScalebyDistance

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/ConstantScalebyDistance.ConstantScalebyDistance`
- Deep graph doc: [ConstantScalebyDistance](Functions/Engine_MaterialFunctions01_WorldPositionOffset__ConstantScalebyDistance.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 18
- Description: Keeps a quad a constant scale on the screen, falls apart at close distances.
- Input pins: SizeScale [FunctionInput_Scalar]
- Output pins: Result

### FoliageScaleFactor

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/FoliageScaleFactor.FoliageScaleFactor`
- Deep graph doc: [FoliageScaleFactor](Functions/Engine_MaterialFunctions01_WorldPositionOffset__FoliageScaleFactor.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 8
- Description: A way to get object scale for Foliage meshes where easier solutions like "ObjectScale" do not work.
- Input pins: Initial Radius [FunctionInput_Scalar]
- Output pins: Result

### SimpleGrassWind

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/SimpleGrassWind.SimpleGrassWind`
- Deep graph doc: [SimpleGrassWind](Functions/Engine_MaterialFunctions01_WorldPositionOffset__SimpleGrassWind.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 64
- Description: Simple Waving grass wind, Grass must be textured to full sheet.  This should be the last WPO node applied.
- Input pins: AdditionalWPO [FunctionInput_Vector3], WindIntensity [FunctionInput_Scalar], WindWeight [FunctionInput_Scalar], WindSpeed [FunctionInput_Scalar]
- Output pins: Result

### SplineThicken

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/SplineThicken.SplineThicken`
- Deep graph doc: [SplineThicken](Functions/Engine_MaterialFunctions01_WorldPositionOffset__SplineThicken.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 12
- Outputs: 3
- Internal nodes: 109
- Description: This function  widens strips of polies along the U of their UVs.  These strips shouuld be extremely thin and imported with remove degenerate triangles disabled. Useful for Hair, Grass, Cables etc.
- Input pins: WorldPosition [FunctionInput_Vector3], UVs for Projection [FunctionInput_Vector2], WidthBase [FunctionInput_Scalar], WidthTip [FunctionInput_Scalar], Expand U or V UV Channel [FunctionInput_StaticBool], AdditionalNormal [FunctionInput_Vector3], UVs for Thickness [FunctionInput_Vector2], DeriveNormalZ [FunctionInput_StaticBool], AngleCorrectedNormals [FunctionInput_StaticBool], UVs for texturing [FunctionInput_Vector2], FlattenPixelNormal [FunctionInput_StaticBool], UVs For NormalShading [FunctionInput_Vector2]
- Output pins: Normal, WorldPositionOffset, UVs with Parallax

