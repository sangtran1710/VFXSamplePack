# Engine_MaterialFunctions01/Debug

Engine: Unreal Engine 5.3
Function count: 7

## Agent Use

Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### DebugBinaryValues-Float

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/DebugBinaryValues-Float.DebugBinaryValues-Float`
- Deep graph doc: [DebugBinaryValues-Float](Functions/Engine_MaterialFunctions01_Debug__DebugBinaryValues-Float.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 59
- Description: Displays the bit pattern of the input value as a floating point number.
- Input pins: Number of Bits [FunctionInput_Scalar], Number To Convert [FunctionInput_Scalar], UVs [FunctionInput_Vector2]
- Output pins: Result

### DebugBinaryValues-Int

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/DebugBinaryValues-Int.DebugBinaryValues-Int`
- Deep graph doc: [DebugBinaryValues-Int](Functions/Engine_MaterialFunctions01_Debug__DebugBinaryValues-Int.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 60
- Description: Displays the bit pattern of the input value as a floating point number.
- Input pins: Number of Bits [FunctionInput_Scalar], Number To Convert [FunctionInput_Scalar], UVs [FunctionInput_Vector2]
- Output pins: Result

### DebugIntValues

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/DebugIntValues.DebugIntValues`
- Deep graph doc: [DebugIntValues](Functions/Engine_MaterialFunctions01_Debug__DebugIntValues.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 65
- Description: Enter a value into the material function and read its return value as a number in uv space.
- Input pins: Number [FunctionInput_Scalar], MaximumNumberOfDigits [FunctionInput_Scalar], UVs [FunctionInput_Vector2], DebugTextLocation RG_UpperRight BA_LowerLeft [FunctionInput_Vector4]
- Output pins: Result

### FunctionGraphSetup

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/FunctionGraphSetup.FunctionGraphSetup`
- Deep graph doc: [FunctionGraphSetup](Functions/Engine_MaterialFunctions01_Debug__FunctionGraphSetup.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 6
- Internal nodes: 35
- Input pins: X range Y range [FunctionInput_Vector4], Gradient Height [FunctionInput_Scalar], UVs [FunctionInput_Vector2]
- Output pins: UV, x, y, Gradient Height, Corrected Y, Range

### PlotFunctionOnGraph

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/PlotFunctionOnGraph.PlotFunctionOnGraph`
- Deep graph doc: [PlotFunctionOnGraph](Functions/Engine_MaterialFunctions01_Debug__PlotFunctionOnGraph.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 2
- Internal nodes: 100
- Description: Plots f(x) = y to visualize the output of another function.
- Input pins: f(x) [FunctionInput_Scalar], Color [FunctionInput_Vector3], Thickness [FunctionInput_Scalar], ShowGrid? [FunctionInput_StaticBool], X range Y range [FunctionInput_Vector4], Gradient Height [FunctionInput_Scalar]
- Output pins: f(x) = y, x

### PlotFunctionOnGraph_Derivative

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/PlotFunctionOnGraph_Derivative.PlotFunctionOnGraph_Derivative`
- Deep graph doc: [PlotFunctionOnGraph_Derivative](Functions/Engine_MaterialFunctions01_Debug__PlotFunctionOnGraph_Derivative.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 4
- Internal nodes: 142
- Description: Plots f(x) = y to visualize the output of another function. This version includes the derivative which requires running the function twice, once with  x + Δx as the input.
- Input pins: f(x) [FunctionInput_Scalar], Color [FunctionInput_Vector3], Thickness [FunctionInput_Scalar], ShowGrid? [FunctionInput_StaticBool], X range Y range [FunctionInput_Vector4], f(x + Δx) [FunctionInput_Scalar], Gradient Height [FunctionInput_Scalar]
- Output pins: f(x) = y, x, x + Δx, d(x)

### PlotFunctionOnGraph_Setup_Input

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Debug/PlotFunctionOnGraph_Setup_Input.PlotFunctionOnGraph_Setup_Input`
- Deep graph doc: [PlotFunctionOnGraph_Setup_Input](Functions/Engine_MaterialFunctions01_Debug__PlotFunctionOnGraph_Setup_Input.md)
- Use: Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 1
- Internal nodes: 99
- Description: Plots f(x) = y to visualize the output of another function.
- Input pins: f(x) [FunctionInput_Scalar], Color [FunctionInput_Vector3], Thickness [FunctionInput_Scalar], ShowGrid? [FunctionInput_StaticBool], X range Y range [FunctionInput_Vector4], Gradient Height [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Corrected Y-size [FunctionInput_Scalar]
- Output pins: f(x) = y

