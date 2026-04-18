# Engine_MaterialFunctions01/Shading

Engine: Unreal Engine 5.3
Function count: 12

## Agent Use

Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### AxisAlignedFresnel

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/AxisAlignedFresnel.AxisAlignedFresnel`
- Deep graph doc: [AxisAlignedFresnel](Functions/Engine_MaterialFunctions01_Shading__AxisAlignedFresnel.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 29
- Description: Gives a fresnel type falloff that is only perpendicular to the specified axis. Useful for fading the edges of a cylinder even when looking straight down along the axis of the cylinder.
- Input pins: Axis [FunctionInput_Vector3], Normal [FunctionInput_Vector3], Exponent [FunctionInput_Scalar]
- Output pins: Fresnel, Cylinder Thickness

### CalcLightsourceAngle

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/CalcLightsourceAngle.CalcLightsourceAngle`
- Deep graph doc: [CalcLightsourceAngle](Functions/Engine_MaterialFunctions01_Shading__CalcLightsourceAngle.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 11
- Description: This function can be used to calculate the light source angle of a pointlight given a certain radius in world space.
- Input pins: Light Position [FunctionInput_Vector3], Source Radius [FunctionInput_Scalar]
- Output pins: Degrees, Radians

### ConvertFromDiffSpec

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/ConvertFromDiffSpec.ConvertFromDiffSpec`
- Deep graph doc: [ConvertFromDiffSpec](Functions/Engine_MaterialFunctions01_Shading__ConvertFromDiffSpec.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 21
- Input pins: DiffuseColor [FunctionInput_Vector3], SpecularColor [FunctionInput_Vector3]
- Output pins: BaseColor, Metallic, Specular

### Ellipsoid-ConeShadow-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Ellipsoid-ConeShadow-Texture.Ellipsoid-ConeShadow-Texture`
- Deep graph doc: [Ellipsoid-ConeShadow-Texture](Functions/Engine_MaterialFunctions01_Shading__Ellipsoid-ConeShadow-Texture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 9
- Outputs: 1
- Internal nodes: 86
- Description: Returns the occlusion value for a pixel given a sphere at a specified position and of a specified radius. The occlusion values are read from a pre-calculated texture so this function is designed to run in realtime.
- Input pins: Light Vector [FunctionInput_Vector3], Ellipsoid Position [FunctionInput_Vector3], Ellipsoid XY size [FunctionInput_Scalar], Light Source Angle [FunctionInput_Scalar], Ellipsoid Z Scale [FunctionInput_Scalar], Ellipsoid X Axis [FunctionInput_Vector3], Ellipsoid Y Axis [FunctionInput_Vector3], Ellipsoid Z Axis [FunctionInput_Vector3], Is Light Directional [FunctionInput_StaticBool]
- Output pins: Result

### FuzzyShading

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/FuzzyShading.FuzzyShading`
- Deep graph doc: [FuzzyShading](Functions/Engine_MaterialFunctions01_Shading__FuzzyShading.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 24
- Description: This function emulates shading simmilar to velvet or moss
- Input pins: Normal [FunctionInput_Vector3], CoreDarkness [FunctionInput_Scalar], EdgeBrightness [FunctionInput_Scalar], BaseColor [FunctionInput_Vector3], Power [FunctionInput_Scalar]
- Output pins: Result

### FuzzyShadingGrass

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/FuzzyShadingGrass.FuzzyShadingGrass`
- Deep graph doc: [FuzzyShadingGrass](Functions/Engine_MaterialFunctions01_Shading__FuzzyShadingGrass.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 1
- Internal nodes: 30
- Description: This function is the diffuse portion of grass shading.
- Input pins: Diffuse [FunctionInput_Vector3], CoreDarkness [FunctionInput_Scalar], Normal [FunctionInput_Vector3], EdgeBrightness [FunctionInput_Scalar], Power [FunctionInput_Scalar], EdgeDesat [FunctionInput_Scalar], EdgeColor [FunctionInput_Vector3]
- Output pins: Result

### MetallicShading

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/MetallicShading.MetallicShading`
- Deep graph doc: [MetallicShading](Functions/Engine_MaterialFunctions01_Shading__MetallicShading.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 12
- Description: Apply to base color of metals for more interesting shading
- Input pins: BaseColor [FunctionInput_Vector3]
- Output pins: Result

### PowerToRoughness

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/PowerToRoughness.PowerToRoughness`
- Deep graph doc: [PowerToRoughness](Functions/Engine_MaterialFunctions01_Shading__PowerToRoughness.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 8
- Input pins: Power [FunctionInput_Scalar]
- Output pins: Roughness

### Sphere_AO

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Sphere_AO.Sphere_AO`
- Deep graph doc: [Sphere_AO](Functions/Engine_MaterialFunctions01_Shading__Sphere_AO.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 15
- Description: This function can be used to calculate the Ambient Occlusion for a sphere affecting the world at any point. It is computed by getting the area of the hemisphere occluded by the occluder.
- Input pins: Sphere Position [FunctionInput_Vector3], Source Radius [FunctionInput_Scalar]
- Output pins: AO Value

### Sphere-ConeShadow-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Sphere-ConeShadow-Texture.Sphere-ConeShadow-Texture`
- Deep graph doc: [Sphere-ConeShadow-Texture](Functions/Engine_MaterialFunctions01_Shading__Sphere-ConeShadow-Texture.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 73
- Description: Returns the occlusion value for a pixel given a sphere at a specified position and of a specified radius. The occlusion values are read from a pre-calculated texture so this function is designed to run in realtime.
- Input pins: Light Vector [FunctionInput_Vector3], Sphere Position [FunctionInput_Vector3], Sphere Radius [FunctionInput_Scalar], Light Source Angle [FunctionInput_Scalar]
- Output pins: Result

### Spherical-Cap-Intersection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Spherical-Cap-Intersection.Spherical-Cap-Intersection`
- Deep graph doc: [Spherical-Cap-Intersection](Functions/Engine_MaterialFunctions01_Shading__Spherical-Cap-Intersection.md)
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 36
- Description: Mathematically computes the area of two spherecap intersections. This will return the occlusion ratio given a sphere occluder and a light angle. Returns the inverse, so it returns 1 when there was no occlusion of the light cone and 0 when there is full occlusion of the light cone. Units are specified in degrees. Note that this function is used to generate static textures for use in final shading, not meant to be used in realtime.
- Input pins: Angle Between Cones [FunctionInput_Scalar], View Occlusion Angle [FunctionInput_Scalar], Light Source Angle [FunctionInput_Scalar], Use Smoothstep [FunctionInput_StaticBool], Use Radians [FunctionInput_StaticBool]
- Output pins: Illumination, Area of intersection

### TextureDefinedSpecularShape

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/TextureDefinedSpecularShape.TextureDefinedSpecularShape`
- Deep graph doc: [TextureDefinedSpecularShape](Functions/Engine_MaterialFunctions01_Shading__TextureDefinedSpecularShape.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 28
- Description: Aligns the Highlight shape to the reflected light
- Input pins: HighlightShape [FunctionInput_Texture2D], SpecularSharpness [FunctionInput_Scalar], Normal [FunctionInput_Vector3], SpecularIntensity [FunctionInput_Vector3]
- Output pins: Result

