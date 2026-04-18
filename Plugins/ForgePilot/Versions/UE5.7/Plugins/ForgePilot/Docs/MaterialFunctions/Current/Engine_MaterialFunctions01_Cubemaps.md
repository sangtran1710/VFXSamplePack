# Engine_MaterialFunctions01/Cubemaps

Engine: Unreal Engine 5.7
Function count: 1

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### InteriorCubemap

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Cubemaps/InteriorCubemap.InteriorCubemap`
- Deep graph doc: [InteriorCubemap](Functions/Engine_MaterialFunctions01_Cubemaps__InteriorCubemap.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 50
- Description: Interior Cubemap conforms a cubemap to be in the form of a box shape inset from the surface UVs. It is useful for giving parallax to rooms visible through windows on city buildings.
- Input pins: Tiling [FunctionInput_Vector2], UVs [FunctionInput_Vector2], Randomize Rotation [FunctionInput_StaticBool]
- Output pins: UVW



