# Engine_MaterialFunctions01/Landscape

Engine: Unreal Engine 5.3
Function count: 1

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### Landscape_Manual_UVW

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Landscape/Landscape_Manual_UVW.Landscape_Manual_UVW`
- Deep graph doc: [Landscape_Manual_UVW](Functions/Engine_MaterialFunctions01_Landscape__Landscape_Manual_UVW.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 9
- Description: Landscape Manual UVW Defined by Min and Max positions
- Input pins: Max Pos [FunctionInput_Vector3], Min Pos [FunctionInput_Vector3], Position [FunctionInput_Vector3]
- Output pins: UVW



