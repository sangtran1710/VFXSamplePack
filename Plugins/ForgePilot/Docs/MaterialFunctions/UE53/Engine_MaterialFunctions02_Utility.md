# Engine_MaterialFunctions02/Utility

Engine: Unreal Engine 5.3
Function count: 32

## Agent Use

Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### AppendMany

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/AppendMany.AppendMany`
- Deep graph doc: [AppendMany](Functions/Engine_MaterialFunctions02_Utility__AppendMany.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 10
- Description: Allows you to quickly append up to 4 scalars in one function
- Input pins: R [FunctionInput_Scalar], G [FunctionInput_Scalar], B [FunctionInput_Scalar], A [FunctionInput_Scalar]
- Output pins: RG, RGB, RGBA

### BlendAngleCorrectedNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BlendAngleCorrectedNormals.BlendAngleCorrectedNormals`
- Deep graph doc: [BlendAngleCorrectedNormals](Functions/Engine_MaterialFunctions02_Utility__BlendAngleCorrectedNormals.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 15
- Description: Corrects the normal direction of normal map that is overlayed on other normal maps. (Typically this would be a detail map.) 
- Input pins: BaseNormal [FunctionInput_Vector3], AdditionalNormal [FunctionInput_Vector3]
- Output pins: Result

### BreakOutFloat2Components

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat2Components.BreakOutFloat2Components`
- Deep graph doc: [BreakOutFloat2Components](Functions/Engine_MaterialFunctions02_Utility__BreakOutFloat2Components.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 5
- Description: Enter a float 2 vector into the function and retrieve individual channels back through the scalar outputs.
- Input pins: Float2 [FunctionInput_Vector2]
- Output pins: R, G

### BreakOutFloat3Components

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat3Components.BreakOutFloat3Components`
- Deep graph doc: [BreakOutFloat3Components](Functions/Engine_MaterialFunctions02_Utility__BreakOutFloat3Components.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 3
- Internal nodes: 7
- Description: Enter a float 3 vector into the function and retrieve individual channels back through the scalar outputs.
- Input pins: Float3 [FunctionInput_Vector3]
- Output pins: R, G, B

### BreakOutFloat4Components

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat4Components.BreakOutFloat4Components`
- Deep graph doc: [BreakOutFloat4Components](Functions/Engine_MaterialFunctions02_Utility__BreakOutFloat4Components.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 9
- Description: Enter a float 4 vector into the function and retrieve individual channels back through the scalar outputs.
- Input pins: Float4 [FunctionInput_Vector4]
- Output pins: R, G, B, A

### BumpOffset_advanced

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BumpOffset_Advanced.BumpOffset_advanced`
- Deep graph doc: [BumpOffset_advanced](Functions/Engine_MaterialFunctions02_Utility__BumpOffset_advanced.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 3
- Internal nodes: 16
- Input pins: HeightTexture [FunctionInput_Scalar], ReferencePlane [FunctionInput_Scalar], HeightRatioInput [FunctionInput_Scalar], TextureCoordinates [FunctionInput_Vector2], CameraVector [FunctionInput_Vector3]
- Output pins: UVs, Distortion, Undeformed UVs

### CameraDirectionVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/CameraDirectionVector.CameraDirectionVector`
- Deep graph doc: [CameraDirectionVector](Functions/Engine_MaterialFunctions02_Utility__CameraDirectionVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 1
- Internal nodes: 3
- Description: World space vector of the camera direction. 
- Output pins: Result

### CameraVectorWithWPOOptions

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/CameraVectorWithWPOOptions.CameraVectorWithWPOOptions`
- Deep graph doc: [CameraVectorWithWPOOptions](Functions/Engine_MaterialFunctions02_Utility__CameraVectorWithWPOOptions.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 10
- Description: Retrieves the camera vector with or without vertex shader offsets.
- Input pins: Use World Position Offsets [FunctionInput_StaticBool]
- Output pins: Result

### CustomReflectionVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/CustomReflectionVector.CustomReflectionVector`
- Deep graph doc: [CustomReflectionVector](Functions/Engine_MaterialFunctions02_Utility__CustomReflectionVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 12
- Description: Input a normal to generate a reflection vector independant of the default reflection vector and the normals input on the base shader.
- Input pins: Normal [FunctionInput_Vector3], CameraVector [FunctionInput_Vector3]
- Output pins: Reflection Vector

### DebugFloat2Values

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugFloat2Values.DebugFloat2Values`
- Deep graph doc: [DebugFloat2Values](Functions/Engine_MaterialFunctions02_Utility__DebugFloat2Values.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 21
- Description: Plug a vector into the function and then preview the output to view the numeric values of the input vector.
- Input pins: UVs [FunctionInput_Vector2], DebugTextLocation RG_UpperRight BA_LowerLeft [FunctionInput_Vector4], MaximumNumberOfDigits [FunctionInput_Scalar], Vector2 [FunctionInput_Vector2], Component Spacing [FunctionInput_Vector4]
- Output pins: ColorCodedOutput, GreyScaleOutput

### DebugFloat3Values

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugFloat3Values.DebugFloat3Values`
- Deep graph doc: [DebugFloat3Values](Functions/Engine_MaterialFunctions02_Utility__DebugFloat3Values.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 25
- Description: Plug a vector into the function and then preview the output to view the numeric values of the input vector.
- Input pins: UVs [FunctionInput_Vector2], DebugTextLocation RG_UpperRight BA_LowerLeft [FunctionInput_Vector4], MaximumNumberOfDigits [FunctionInput_Scalar], Vector3 [FunctionInput_Vector3], Component Spacing [FunctionInput_Vector4]
- Output pins: ColorCodedOutput, GreyScaleOutput

### DebugFloat4Values

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugFloat4Values.DebugFloat4Values`
- Deep graph doc: [DebugFloat4Values](Functions/Engine_MaterialFunctions02_Utility__DebugFloat4Values.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 31
- Description: Plug a vector into the function and then preview the output to view the numeric values of the input vector.
- Input pins: UVs [FunctionInput_Vector2], DebugTextLocation RG_UpperRight BA_LowerLeft [FunctionInput_Vector4], MaximumNumberOfDigits [FunctionInput_Scalar], Vector4 [FunctionInput_Vector4], Component Spacing [FunctionInput_Vector4]
- Output pins: ColorCodedOutput, GreyScaleOutput

### DebugOnOff

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugOnOff.DebugOnOff`
- Deep graph doc: [DebugOnOff](Functions/Engine_MaterialFunctions02_Utility__DebugOnOff.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 1
- Internal nodes: 5
- Description: Useful for doing AB comparrison in material
- Output pins: Result

### DebugScalarValues

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugScalarValues.DebugScalarValues`
- Deep graph doc: [DebugScalarValues](Functions/Engine_MaterialFunctions02_Utility__DebugScalarValues.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 127
- Description: Enter a value into the material function and read its return value as a number in uv space.
- Input pins: Number [FunctionInput_Scalar], MaximumNumberOfDigits [FunctionInput_Scalar], UVs [FunctionInput_Vector2], DebugTextLocation RG_UpperRight BA_LowerLeft [FunctionInput_Vector4]
- Output pins: Result

### DebugTimeSine

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DebugTimeSine.DebugTimeSine`
- Deep graph doc: [DebugTimeSine](Functions/Engine_MaterialFunctions02_Utility__DebugTimeSine.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 6
- Description: useful for seeing what a scalar value does between 0 and 1
- Input pins: Speed [FunctionInput_Scalar]
- Output pins: Result

### DepthFromWorldPosition

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DepthFromWorldPosition.DepthFromWorldPosition`
- Deep graph doc: [DepthFromWorldPosition](Functions/Engine_MaterialFunctions02_Utility__DepthFromWorldPosition.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Description: Returns the same value that Pixel Depth or Scene Depth Return but using an arbitrary world position.
- Input pins: World Position [FunctionInput_Vector3]
- Output pins: Result

### DitherTemporalAA

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DitherTemporalAA.DitherTemporalAA`
- Deep graph doc: [DitherTemporalAA](Functions/Engine_MaterialFunctions02_Utility__DitherTemporalAA.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 16
- Description: Stipple pattern/Dither  in screen space and time to work well with TemporalAA for things like masked translucency or anisotropic materials
- Input pins: Alpha Threshold [FunctionInput_Scalar], Random [FunctionInput_Scalar]
- Output pins: Result

### DynamicBranch

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/DynamicBranch.DynamicBranch`
- Deep graph doc: [DynamicBranch](Functions/Engine_MaterialFunctions02_Utility__DynamicBranch.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 6
- Description: The code connected to the "Color Input 1" will be executed if the alpha is greater than 0 and the "Color Input 0" input code will be evaluated  if the alpha is 0. Dynamic branches are best used when masking 20 or more instructions or a texture sample.
- Input pins: Color Input 0 [FunctionInput_Vector3], Alpha [FunctionInput_Scalar], Color Input 1 [FunctionInput_Vector3]
- Output pins: Result

### FOV

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/FOV.FOV`
- Deep graph doc: [FOV](Functions/Engine_MaterialFunctions02_Utility__FOV.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 3
- Internal nodes: 10
- Description: Returns the current cameras FOV setting
- Output pins: FOV angle in degrees, FOV in radians, FOV 

### IsFloatValid

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/IsFloatValid.IsFloatValid`
- Deep graph doc: [IsFloatValid](Functions/Engine_MaterialFunctions02_Utility__IsFloatValid.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 3
- Description: Returns 1 if the float is a real number and 0 if it is either infinite or a NaN.
- Input pins: In [FunctionInput_Scalar]
- Output pins: Result

### LinearSine

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/LinearSine.LinearSine`
- Deep graph doc: [LinearSine](Functions/Engine_MaterialFunctions02_Utility__LinearSine.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 34
- Description: A sine-like function that transitions linearly in the 0-1 range using a period of 1.
- Input pins: Value [FunctionInput_Scalar], Period [FunctionInput_Scalar], -1 to 1 [FunctionInput_StaticBool], Sine Phase [FunctionInput_StaticBool]
- Output pins: Linear Sine, Rounded Linear Sine , Direction

### MakeFloat2

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat2.MakeFloat2`
- Deep graph doc: [MakeFloat2](Functions/Engine_MaterialFunctions02_Utility__MakeFloat2.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 4
- Description: Creates a float 2 vector from a series of scalar inputs.
- Input pins: X [FunctionInput_Scalar], Y [FunctionInput_Scalar]
- Output pins: Result

### MakeFloat3

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat3.MakeFloat3`
- Deep graph doc: [MakeFloat3](Functions/Engine_MaterialFunctions02_Utility__MakeFloat3.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 6
- Description: Creates a float 3 vector from a series of scalar inputs.
- Input pins: X [FunctionInput_Scalar], Y [FunctionInput_Scalar], Z [FunctionInput_Scalar]
- Output pins: Result

### MakeFloat4

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat4.MakeFloat4`
- Deep graph doc: [MakeFloat4](Functions/Engine_MaterialFunctions02_Utility__MakeFloat4.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 8
- Description: Creates a float 4 vector from a series of scalar inputs.
- Input pins: X [FunctionInput_Scalar], Y [FunctionInput_Scalar], Z [FunctionInput_Scalar], A [FunctionInput_Scalar]
- Output pins: Result

### Round

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Round.Round`
- Deep graph doc: [Round](Functions/Engine_MaterialFunctions02_Utility__Round.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 11
- Description: Rounds a number to the next closest whole number
- Input pins: In [FunctionInput_Scalar]
- Output pins: Result

### Sign

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Sign.Sign`
- Deep graph doc: [Sign](Functions/Engine_MaterialFunctions02_Utility__Sign.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Description: Returns a -1 for negative numbers, a 1 for positive numbers and 0 for 0.
- Input pins: In [FunctionInput_Scalar]
- Output pins: Result

### Sine_Remapped

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Sine_Remapped.Sine_Remapped`
- Deep graph doc: [Sine_Remapped](Functions/Engine_MaterialFunctions02_Utility__Sine_Remapped.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 7
- Description: remaps the output of a sine wave to two colors/vectors specified by the user.
- Input pins: Value 2 [FunctionInput_Vector3], Sine Phase [FunctionInput_Scalar], Value 1 [FunctionInput_Vector3]
- Output pins: Remapped Sine

### StencilMaskCompare

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/StencilMaskCompare.StencilMaskCompare`
- Deep graph doc: [StencilMaskCompare](Functions/Engine_MaterialFunctions02_Utility__StencilMaskCompare.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Description: Compares a float to the stencil buffer. White if true - black if false.
- Input pins: ID [FunctionInput_Scalar]
- Output pins: Result

### SumOfAConsecutiveNumberSequence

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/SumOfAConsecutiveNumberSequence.SumOfAConsecutiveNumberSequence`
- Deep graph doc: [SumOfAConsecutiveNumberSequence](Functions/Engine_MaterialFunctions02_Utility__SumOfAConsecutiveNumberSequence.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 6
- Description: This will find the sum of numbers up to the integer inserted. I.e. [ 1+2+3...+ n ].
- Input pins: Number of integers [FunctionInput_Scalar]
- Output pins: Result

### Swizzle

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/Swizzle.Swizzle`
- Deep graph doc: [Swizzle](Functions/Engine_MaterialFunctions02_Utility__Swizzle.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 14
- Input pins: XY [FunctionInput_Vector2], XYZ [FunctionInput_Vector3]
- Output pins: YX, YXZ

### VectorLength

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/VectorLength.VectorLength`
- Deep graph doc: [VectorLength](Functions/Engine_MaterialFunctions02_Utility__VectorLength.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 7
- Description: Returns a vectors length.
- Input pins: Vector 3 [FunctionInput_Vector3], Vector 2 [FunctionInput_Vector2]
- Output pins: V3 Length, V2 Length

### VectorToRadialValue

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/VectorToRadialValue.VectorToRadialValue`
- Deep graph doc: [VectorToRadialValue](Functions/Engine_MaterialFunctions02_Utility__VectorToRadialValue.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 28
- Description: The input coordinates or vector will be transformed into radial coordinates. In the case of a vector the angle will be output in one channel and the length of the vector in the other.
- Input pins: Swizzle Coordinate Output [FunctionInput_StaticBool], Vector or UVs [FunctionInput_Vector2]
- Output pins: Radial Coordinates, Vector Converted to Angle, Linear Distance

