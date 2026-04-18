# Engine_MaterialFunctions01/ImageAdjustment

Engine: Unreal Engine 5.3
Function count: 16

## Agent Use

Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.

Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.

## Functions

### 3ColorBlend

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/3ColorBlend.3ColorBlend`
- Deep graph doc: [3ColorBlend](Functions/Engine_MaterialFunctions01_ImageAdjustment__3ColorBlend.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 14
- Description: Blends between 3 colors based on a greyscale input
- Input pins: Alpha [FunctionInput_Scalar], A [FunctionInput_Vector3], B [FunctionInput_Vector3], C [FunctionInput_Vector3]
- Output pins: Result

### BlurSampleOffsets

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/BlurSampleOffsets.BlurSampleOffsets`
- Deep graph doc: [BlurSampleOffsets](Functions/Engine_MaterialFunctions01_ImageAdjustment__BlurSampleOffsets.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 8
- Internal nodes: 25
- Description: Multiplies an input offset by a number of 2d vectors.
- Input pins: OffsetAmount [FunctionInput_Vector2]
- Output pins: 1,0, -1,0, 0,1, 0,-1, .5,.5, -.5,-.5, -.5,.5, .5,-.5

### CheapContrast

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast.CheapContrast`
- Deep graph doc: [CheapContrast](Functions/Engine_MaterialFunctions01_ImageAdjustment__CheapContrast.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 10
- Description: Cheaply adds contrast similar to pulling in edges in Photoshops Levels tool
- Input pins: In [FunctionInput_Scalar], Contrast [FunctionInput_Scalar]
- Output pins: Result

### CheapContrast_RGB

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast_RGB.CheapContrast_RGB`
- Deep graph doc: [CheapContrast_RGB](Functions/Engine_MaterialFunctions01_ImageAdjustment__CheapContrast_RGB.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 10
- Description: Cheaply adds contrast similar to pulling in edges in Photoshops Levels tool
- Input pins: In [FunctionInput_Vector3], Contrast [FunctionInput_Scalar]
- Output pins: Result

### ColorCorrection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/ColorCorrection.ColorCorrection`
- Deep graph doc: [ColorCorrection](Functions/Engine_MaterialFunctions01_ImageAdjustment__ColorCorrection.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 1
- Internal nodes: 21
- Description: Color correction
- Input pins: Color [FunctionInput_Vector3], Gain [FunctionInput_Vector3], Gamma [FunctionInput_Vector3], Lift [FunctionInput_Vector3], Contrast [FunctionInput_Scalar], Saturation [FunctionInput_Scalar], Hue Shift [FunctionInput_Scalar], Brightness [FunctionInput_Scalar]
- Output pins: Result

### Contrast_Preserve_Color

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/Contrast_Preserve_Color.Contrast_Preserve_Color`
- Deep graph doc: [Contrast_Preserve_Color](Functions/Engine_MaterialFunctions01_ImageAdjustment__Contrast_Preserve_Color.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 8
- Description: This function allows you to modify the contrast of an image without affecting the relative color values.
- Input pins: Color [FunctionInput_Vector3], Contrast [FunctionInput_Scalar]
- Output pins: Result

### DeriveHDRfromLDR

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/DeriveHDRfromLDR.DeriveHDRfromLDR`
- Deep graph doc: [DeriveHDRfromLDR](Functions/Engine_MaterialFunctions01_ImageAdjustment__DeriveHDRfromLDR.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 1
- Internal nodes: 22
- Description: Derive an HDR range from an LDR texture while maintaining chroma and minimizing artifacts
- Input pins: LDR Input [FunctionInput_Vector3], Derive Luminance from Linear [FunctionInput_StaticBool], Desaturate Luminance [FunctionInput_Scalar], Derive HDR Power [FunctionInput_Scalar], HDR Dynamic Range [FunctionInput_Scalar], HDR Tint [FunctionInput_Vector3], HDR Intensity Multiplier [FunctionInput_Scalar]
- Output pins: RemappedRange

### HighPassFunction

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/HighPassFunction.HighPassFunction`
- Deep graph doc: [HighPassFunction](Functions/Engine_MaterialFunctions01_ImageAdjustment__HighPassFunction.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 11
- Outputs: 7
- Internal nodes: 48
- Description: Performs a high pass type operation on the input functions. This filters out low frequency information. Note that you do do not have to hook up all 6 offset function inputs if you do not want full 3d sampling. If you exclude the two Z samples and set the "Number of Samples" to 4, it will be a 2D highpass on the XY plane.
- Input pins: Contrast [FunctionInput_Scalar], Position [FunctionInput_Vector3], Highpass Offset [FunctionInput_Vector3], Center Sample [FunctionInput_Vector3], Offset Sample +X [FunctionInput_Vector3], Offset Sample -X [FunctionInput_Vector3], Offset Sample +Y [FunctionInput_Vector3], Offset Sample -Y [FunctionInput_Vector3], Offset Sample +Z [FunctionInput_Vector3], Offset Sample -Z [FunctionInput_Vector3], Number of Samples [FunctionInput_Scalar]
- Output pins: Result, Offset +X, Offset -X, Offset +Y, Offset -Y, Offset +Z, Offset -Z

### HighPassTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/HighPassTexture.HighPassTexture`
- Deep graph doc: [HighPassTexture](Functions/Engine_MaterialFunctions01_ImageAdjustment__HighPassTexture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 43
- Description: Performs a high pass type operation on an input texture object. This filters out low frequency information.
- Input pins: Texture Object [FunctionInput_Texture2D], Contrast [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Highpass Offset [FunctionInput_Scalar], High Quality [FunctionInput_StaticBool]
- Output pins: Result, Result No Contrast

### LinearTosRGB

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/LinearTosRGB.LinearTosRGB`
- Deep graph doc: [LinearTosRGB](Functions/Engine_MaterialFunctions01_ImageAdjustment__LinearTosRGB.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 12
- Description: Convert Linear to sRGB
- Input pins: Linear [FunctionInput_Scalar]
- Output pins: sRGB

### Luminosity_And_Color

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/Luminosity_And_Color.Luminosity_And_Color`
- Deep graph doc: [Luminosity_And_Color](Functions/Engine_MaterialFunctions01_ImageAdjustment__Luminosity_And_Color.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 5
- Description: This function takes an input color and an input luminosity and merges them.
- Input pins: Color [FunctionInput_Vector3], Luminosity [FunctionInput_Scalar]
- Output pins: Result

### SceneTextureAverage

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/SceneTextureAverage.SceneTextureAverage`
- Deep graph doc: [SceneTextureAverage](Functions/Engine_MaterialFunctions01_ImageAdjustment__SceneTextureAverage.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 19
- Description: Samples and averages the scene texture 8 times in a circular pattern.
- Input pins: OffsetAmount [FunctionInput_Vector2]
- Output pins: 8 Samples

### SCurve

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/SCurve.SCurve`
- Deep graph doc: [SCurve](Functions/Engine_MaterialFunctions01_ImageAdjustment__SCurve.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 27
- Description: Applies an S curve image adjustment
- Input pins: Power [FunctionInput_Scalar], In [FunctionInput_Vector3]
- Output pins: Result

### sRGBToLinear

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/sRGBToLinear.sRGBToLinear`
- Deep graph doc: [sRGBToLinear](Functions/Engine_MaterialFunctions01_ImageAdjustment__sRGBToLinear.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 12
- Description: Convert sRGB to Linear
- Input pins: sRGB [FunctionInput_Scalar]
- Output pins: Linear

### UnSharpMaskFunction

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/UnSharpMaskFunction.UnSharpMaskFunction`
- Deep graph doc: [UnSharpMaskFunction](Functions/Engine_MaterialFunctions01_ImageAdjustment__UnSharpMaskFunction.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 11
- Outputs: 7
- Internal nodes: 48
- Description: Performs a high pass type operation on the input functions. This filters out low frequency information.
- Input pins: Amount [FunctionInput_Scalar], Position [FunctionInput_Vector3], Sample Offset [FunctionInput_Vector3], Center Sample [FunctionInput_Vector3], Offset Sample +X [FunctionInput_Vector3], Offset Sample -X [FunctionInput_Vector3], Offset Sample +Y [FunctionInput_Vector3], Offset Sample -Y [FunctionInput_Vector3], Offset Sample +Z [FunctionInput_Vector3], Offset Sample -Z [FunctionInput_Vector3], Number of Samples [FunctionInput_Scalar]
- Output pins: Result, Offset +X, Offset -X, Offset +Y, Offset -Y, Offset +Z, Offset -Z

### UnSharpMaskTexture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/UnSharpMaskTexture.UnSharpMaskTexture`
- Deep graph doc: [UnSharpMaskTexture](Functions/Engine_MaterialFunctions01_ImageAdjustment__UnSharpMaskTexture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 43
- Description: Performs a high pass type operation on an input texture object. This filters out low frequency information.
- Input pins: Texture Object [FunctionInput_Texture2D], Amount [FunctionInput_Scalar], UVs [FunctionInput_Vector2], Sample Offset [FunctionInput_Scalar], High Quality [FunctionInput_StaticBool], TextureSize [FunctionInput_Scalar]
- Output pins: Result



