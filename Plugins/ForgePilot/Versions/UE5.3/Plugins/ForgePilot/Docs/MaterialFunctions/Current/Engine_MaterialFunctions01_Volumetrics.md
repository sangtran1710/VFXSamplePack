# Engine_MaterialFunctions01/Volumetrics

Engine: Unreal Engine 5.3
Function count: 3

## Agent Use

General material function. Inspect pins and internal nodes before using in production automation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### BeersLaw

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Volumetrics/BeersLaw.BeersLaw`
- Deep graph doc: [BeersLaw](Functions/Engine_MaterialFunctions01_Volumetrics__BeersLaw.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Returns e^(-d) which is a standard exponential density function.
- Input pins: Depth Scale [FunctionInput_Scalar], Thickness [FunctionInput_Scalar]
- Output pins: Result

### Henyey-Greenstein-Phase

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Volumetrics/Henyey-Greenstein-Phase.Henyey-Greenstein-Phase`
- Deep graph doc: [Henyey-Greenstein-Phase](Functions/Engine_MaterialFunctions01_Volumetrics__Henyey-Greenstein-Phase.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 22
- Input pins: AngleDegrees [FunctionInput_Scalar], Anisotropy [FunctionInput_Scalar], Light Vector [FunctionInput_Vector3]
- Output pins: Result

### SchlickPhase

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Volumetrics/SchlickPhase.SchlickPhase`
- Deep graph doc: [SchlickPhase](Functions/Engine_MaterialFunctions01_Volumetrics__SchlickPhase.md)
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 4
- Description: The Schlick Phase Function is useful for adding some directionality or anisotropy to volumetric scattering. Useful for fog/water etc.
- Input pins: Cosine [FunctionInput_Scalar], Anisotropy [FunctionInput_Scalar]
- Output pins: Result



