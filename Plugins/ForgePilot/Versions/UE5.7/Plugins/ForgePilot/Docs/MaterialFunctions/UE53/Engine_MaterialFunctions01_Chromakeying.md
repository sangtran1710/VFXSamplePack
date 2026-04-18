# Engine_MaterialFunctions01/Chromakeying

Engine: Unreal Engine 5.3
Function count: 6

## Agent Use

Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### DespillByAvg

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DespillByAvg.DespillByAvg`
- Deep graph doc: [DespillByAvg](Functions/Engine_MaterialFunctions01_Chromakeying__DespillByAvg.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Description: Despill using average color of input
- Input pins: Image Color [FunctionInput_Vector3]
- Output pins: Result

### DespillByHue

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DespillByHue.DespillByHue`
- Deep graph doc: [DespillByHue](Functions/Engine_MaterialFunctions01_Chromakeying__DespillByHue.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 15
- Input pins: Texture [FunctionInput_Vector3], Despill Hue [FunctionInput_Vector3], Hue Range [FunctionInput_Scalar], DeSpill Amount [FunctionInput_Scalar]
- Output pins: Result

### DespillManual

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DespillManual.DespillManual`
- Deep graph doc: [DespillManual](Functions/Engine_MaterialFunctions01_Chromakeying__DespillManual.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 1
- Internal nodes: 29
- Description: Manual despill function
- Input pins: Image Color [FunctionInput_Vector3], Raw Comparison [FunctionInput_Vector3], KeyedColor [FunctionInput_Vector3], Despill Minimum [FunctionInput_Scalar], Despill Divisor [FunctionInput_Scalar], Despill Amount [FunctionInput_Scalar], Sky Color [FunctionInput_Vector3], Sky Intensity [FunctionInput_Scalar]
- Output pins: Result

### DiffColorKeyer

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DiffColorKeyer.DiffColorKeyer`
- Deep graph doc: [DiffColorKeyer](Functions/Engine_MaterialFunctions01_Chromakeying__DiffColorKeyer.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 8
- Input pins: GreenScreen [FunctionInput_Vector3], KeyColor [FunctionInput_Vector3], AlphaThresOffset [FunctionInput_Vector2], WeightsRB [FunctionInput_Vector2], ClipBW [FunctionInput_Vector2], UnPremult [FunctionInput_Scalar]
- Output pins: RGBA

### DiffColorKeyerErodeSinglePass

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/DiffColorKeyerErodeSinglePass.DiffColorKeyerErodeSinglePass`
- Deep graph doc: [DiffColorKeyerErodeSinglePass](Functions/Engine_MaterialFunctions01_Chromakeying__DiffColorKeyerErodeSinglePass.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 15
- Outputs: 2
- Internal nodes: 32
- Description: Color difference based keyer
- Input pins: TextureExternal [FunctionInput_TextureExternal], KeyColor [FunctionInput_Vector3], KernalSize [FunctionInput_Scalar], NumSamples [FunctionInput_Scalar], UnPremult [FunctionInput_Scalar], AlphaThreshold [FunctionInput_Scalar], AlphaOffset [FunctionInput_Scalar], WeightRed [FunctionInput_Scalar], WeightBlue [FunctionInput_Scalar], ClipBlack [FunctionInput_Scalar], ClipWhite [FunctionInput_Scalar], ErodeAlpha [FunctionInput_StaticBool]
- Output pins: RGB, Alpha

### RGBtoHSV

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Chromakeying/RGBtoHSV.RGBtoHSV`
- Deep graph doc: [RGBtoHSV](Functions/Engine_MaterialFunctions01_Chromakeying__RGBtoHSV.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Input pins: In [FunctionInput_Vector3]
- Output pins: Result

