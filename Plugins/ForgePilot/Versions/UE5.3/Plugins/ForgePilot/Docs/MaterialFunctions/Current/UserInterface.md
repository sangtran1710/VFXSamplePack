# UserInterface

Engine: Unreal Engine 5.3
Function count: 1

## Agent Use

UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### GetUserInterfaceUV

- Path: `/Engine/Functions/UserInterface/GetUserInterfaceUV.GetUserInterfaceUV`
- Deep graph doc: [GetUserInterfaceUV](Functions/UserInterface__GetUserInterfaceUV.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 5
- Internal nodes: 10
- Description: Provides access to various built in UV sets for UI materials rendered with Slate or UMG
- Output pins: 9-Slice UV, Tiling, 9-Slice UV (No Tiling), Pixel Size, Normalized UV



