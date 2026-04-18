# Engine_MaterialFunctions03/Blends

Engine: Unreal Engine 5.7
Function count: 16

## Agent Use

Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.

Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.

## Functions

### Blend_ColorBurn

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_ColorBurn.Blend_ColorBurn`
- Deep graph doc: [Blend_ColorBurn](Functions/Engine_MaterialFunctions03_Blends__Blend_ColorBurn.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: The darker the Blend texture, the more color in the final result.  If the Blend is white, there will be no change.
- Input pins: Blend [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Blend_ColorDodge

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_ColorDodge.Blend_ColorDodge`
- Deep graph doc: [Blend_ColorDodge](Functions/Engine_MaterialFunctions03_Blends__Blend_ColorDodge.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Divides the Base texture by the inverted Blend texture.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_Darken

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Darken.Blend_Darken`
- Deep graph doc: [Blend_Darken](Functions/Engine_MaterialFunctions03_Blends__Blend_Darken.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Returns the darker of the two values for each pixel.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_Difference

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Difference.Blend_Difference`
- Deep graph doc: [Blend_Difference](Functions/Engine_MaterialFunctions03_Blends__Blend_Difference.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Subtracts the Blend from the Base, Abs the results to ensure the results are positive.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_Exclusion

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Exclusion.Blend_Exclusion`
- Deep graph doc: [Blend_Exclusion](Functions/Engine_MaterialFunctions03_Blends__Blend_Exclusion.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 13
- Description: Halves the Blend and Base, combines them together, then partially inverts the results.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_HardLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_HardLight.Blend_HardLight`
- Deep graph doc: [Blend_HardLight](Functions/Engine_MaterialFunctions03_Blends__Blend_HardLight.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 32
- Description: Harsher version of Soft Light blend
- Input pins: Blend [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Blend_Lighten

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Lighten.Blend_Lighten`
- Deep graph doc: [Blend_Lighten](Functions/Engine_MaterialFunctions03_Blends__Blend_Lighten.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Returns the lighter of the two values for each pixel.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_LinearBurn

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_LinearBurn.Blend_LinearBurn`
- Deep graph doc: [Blend_LinearBurn](Functions/Engine_MaterialFunctions03_Blends__Blend_LinearBurn.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: Adds the two textures then subtracts one from the result
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_LinearDodge

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_LinearDodge.Blend_LinearDodge`
- Deep graph doc: [Blend_LinearDodge](Functions/Engine_MaterialFunctions03_Blends__Blend_LinearDodge.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Adds the base and blend together.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_LinearLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_LinearLight.Blend_LinearLight`
- Deep graph doc: [Blend_LinearLight](Functions/Engine_MaterialFunctions03_Blends__Blend_LinearLight.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 30
- Description: Linear version of Overlay blend mode, harsh results.
- Input pins: Blend [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Blend_Overlay

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Overlay.Blend_Overlay`
- Deep graph doc: [Blend_Overlay](Functions/Engine_MaterialFunctions03_Blends__Blend_Overlay.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 29
- Description: Emulates Photoshop's overlay blend mode.  Lighter pixels will become lighter, darker will become darker.
- Input pins: Blend [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Blend_PinLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_PinLight.Blend_PinLight`
- Deep graph doc: [Blend_PinLight](Functions/Engine_MaterialFunctions03_Blends__Blend_PinLight.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 32
- Description: Softer version of Overlay, solid white or black as blend will not result in solid color.
- Input pins: Blend [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Blend_Screen

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Screen.Blend_Screen`
- Deep graph doc: [Blend_Screen](Functions/Engine_MaterialFunctions03_Blends__Blend_Screen.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 9
- Description: Inverts each texture, multiplies them together, then re-inverts the results.
- Input pins: Base [FunctionInput_Vector3], Blend [FunctionInput_Vector3]
- Output pins: Result

### Blend_SoftLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_SoftLight.Blend_SoftLight`
- Deep graph doc: [Blend_SoftLight](Functions/Engine_MaterialFunctions03_Blends__Blend_SoftLight.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 33
- Description: Softer version of Overlay, solid white or black as blend will not result in solid color.
- Input pins: Blend [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Lerp_ScratchGrime

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Lerp_ScratchGrime.Lerp_ScratchGrime`
- Deep graph doc: [Lerp_ScratchGrime](Functions/Engine_MaterialFunctions03_Blends__Lerp_ScratchGrime.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 8
- Description: Use to creat Material layers that have scratches and grime
- Input pins: Base [FunctionInput_Vector3], ScratchValue [FunctionInput_Vector3], GrimeValue [FunctionInput_Vector3], ScratchMask [FunctionInput_Scalar], GrimeMask [FunctionInput_Scalar]
- Output pins: Result

### Lerp_ScratchGrime2

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Lerp_ScratchGrime2.Lerp_ScratchGrime2`
- Deep graph doc: [Lerp_ScratchGrime2](Functions/Engine_MaterialFunctions03_Blends__Lerp_ScratchGrime2.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 10
- Description: Use to creat Material layers that have scratches and grime
- Input pins: Base [FunctionInput_Vector3], ScratchValue [FunctionInput_Vector3], GrimeValue [FunctionInput_Vector3], ScratchMask [FunctionInput_Scalar], GrimeMask [FunctionInput_Scalar]
- Output pins: Result



