# Engine_MaterialFunctions01/Decal

Engine: Unreal Engine 5.7
Function count: 1

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### ApplyDBuffer

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Decal/ApplyDBuffer.ApplyDBuffer`
- Deep graph doc: [ApplyDBuffer](Functions/Engine_MaterialFunctions01_Decal__ApplyDBuffer.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 27
- Description: Apply DBuffer directly to material attributes. This replicates the logic automatically applied by the Dbuffer Decal Response material setting. Material normal should be in world space.
- Input pins: DBufferA [FunctionInput_Vector4], DBufferB [FunctionInput_Vector4], DBufferC [FunctionInput_Vector4], InMaterial [FunctionInput_MaterialAttributes]
- Output pins: OutMaterial



