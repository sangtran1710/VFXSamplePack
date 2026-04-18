# Engine_MaterialFunctions02/Masking

Engine: Unreal Engine 5.7
Function count: 2

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### ComponentWise_SphereMask

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Masking/ComponentWise_SphereMask.ComponentWise_SphereMask`
- Deep graph doc: [ComponentWise_SphereMask](Functions/Engine_MaterialFunctions02_Masking__ComponentWise_SphereMask.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 13
- Description: Apply a sphere mask for each channel of a vector independently.
- Input pins: A [FunctionInput_Vector3], B [FunctionInput_Vector3], Radius [FunctionInput_Vector3], Hardness [FunctionInput_Vector3]
- Output pins: Result

### ThresholdWithRange

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Masking/ThresholdWithRange.ThresholdWithRange`
- Deep graph doc: [ThresholdWithRange](Functions/Engine_MaterialFunctions02_Masking__ThresholdWithRange.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 26
- Input pins: Alpha [FunctionInput_Scalar], ThresholdValue [FunctionInput_Scalar], ThresholdRange [FunctionInput_Scalar], TwoSidedThreshold [FunctionInput_StaticBool]
- Output pins: Result



