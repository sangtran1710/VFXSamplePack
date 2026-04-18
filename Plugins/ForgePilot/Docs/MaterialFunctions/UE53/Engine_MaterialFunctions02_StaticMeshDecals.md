# Engine_MaterialFunctions02/StaticMeshDecals

Engine: Unreal Engine 5.3
Function count: 1

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### StaticMeshDecal_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions02/StaticMeshDecals/StaticMeshDecal_Function.StaticMeshDecal_Function`
- Deep graph doc: [StaticMeshDecal_Function](Functions/Engine_MaterialFunctions02_StaticMeshDecals__StaticMeshDecal_Function.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 5
- Internal nodes: 49
- Description: Use this function with "1x1x1_Box_Pivot_-XYZ" to draw decal like projections on to surfaces. The material should use shader domains other than the decal domain.
- Input pins: Local Projection Angle Mask Vector And Falloff Power (V4) [FunctionInput_Vector4], XYZ Opacity Falloff Hardness [FunctionInput_Vector3], Camera Clip Mask Distance [FunctionInput_Scalar]
- Output pins: 0-1 RGB UVW, Extent bounds masks, Camera Clip Mask, Projection Angle Mask, Combined Mask

