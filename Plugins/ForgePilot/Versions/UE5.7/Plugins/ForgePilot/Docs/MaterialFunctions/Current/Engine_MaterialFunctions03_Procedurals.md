# Engine_MaterialFunctions03/Procedurals

Engine: Unreal Engine 5.7
Function count: 15

## Agent Use

Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.

Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.

## Functions

### Compute3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/Compute3DDeriv.Compute3DDeriv`
- Deep graph doc: [Compute3DDeriv](Functions/Engine_MaterialFunctions03_Procedurals__Compute3DDeriv.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 4
- Internal nodes: 22
- Description: Uses positions offset in a tetrahedral pattern to compute 3D derivatives. Use with Prepare3DDeriv.
- Input pins: Value1 [FunctionInput_Vector3], Value2 [FunctionInput_Vector3], Value3 [FunctionInput_Vector3], Value4 [FunctionInput_Vector3], Tetrahedral Offset [FunctionInput_Scalar]
- Output pins: dx, dy, dz, Value

### ComputeFilterWidth

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/ComputeFilterWidth.ComputeFilterWidth`
- Deep graph doc: [ComputeFilterWidth](Functions/Engine_MaterialFunctions03_Procedurals__ComputeFilterWidth.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 8
- Input pins: In [FunctionInput_Vector3]
- Output pins: Result

### CurlFrom3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/CurlFrom3DDeriv.CurlFrom3DDeriv`
- Deep graph doc: [CurlFrom3DDeriv](Functions/Engine_MaterialFunctions03_Procedurals__CurlFrom3DDeriv.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 11
- Description: Compute curl of at 3D vector field from result of Prepare3DDeriv/Compute3DDeriv
- Input pins: dx [FunctionInput_Vector3], dy [FunctionInput_Vector3], dz [FunctionInput_Vector3]
- Output pins: Curl

### GeneratedBand

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/GeneratedBand.GeneratedBand`
- Deep graph doc: [GeneratedBand](Functions/Engine_MaterialFunctions03_Procedurals__GeneratedBand.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 1
- Internal nodes: 16
- Description: Procedurally generates a horizontal or vertical band from the default TextureCoordinates.
- Input pins: Input Coordinates [FunctionInput_Vector2], Width [FunctionInput_Scalar], Sharpness [FunctionInput_Scalar], Offset [FunctionInput_Scalar], Direction Switch [FunctionInput_StaticBool], Compare [FunctionInput_Scalar]
- Output pins: Result

### GeneratedOffsetBands

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/GeneratedOffsetBands.GeneratedOffsetBands`
- Deep graph doc: [GeneratedOffsetBands](Functions/Engine_MaterialFunctions03_Procedurals__GeneratedOffsetBands.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 1
- Internal nodes: 19
- Description: Procedurally generates a horizontal or vertical band from the default TextureCoordinates.
- Input pins: Input Coordinates [FunctionInput_Vector2], Width [FunctionInput_Scalar], Sharpness [FunctionInput_Scalar], Offset [FunctionInput_Scalar], Direction Switch [FunctionInput_StaticBool], Compare [FunctionInput_Scalar], Bands [FunctionInput_Scalar]
- Output pins: Result

### GradFrom3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/GradFrom3DDeriv.GradFrom3DDeriv`
- Deep graph doc: [GradFrom3DDeriv](Functions/Engine_MaterialFunctions03_Procedurals__GradFrom3DDeriv.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 8
- Description: Compute 3D gradient vector from result of Prepare3DDeriv/Compute3DDeriv
- Input pins: dx [FunctionInput_Vector3], dy [FunctionInput_Vector3], dz [FunctionInput_Vector3]
- Output pins: Gradient

### HeightToNormalSmooth

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/HeightToNormalSmooth.HeightToNormalSmooth`
- Deep graph doc: [HeightToNormalSmooth](Functions/Engine_MaterialFunctions03_Procedurals__HeightToNormalSmooth.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 20
- Description: Provide perpixel height data and return a partially smooth normal (requires AA for smooth details)
- Input pins: Height [FunctionInput_Scalar], World Space Vertex Normals [FunctionInput_Vector3], Absolute World Position [FunctionInput_Vector3]
- Output pins: WorldSpaceNormal

### NormalFromFunction

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/NormalFromFunction.NormalFromFunction`
- Deep graph doc: [NormalFromFunction](Functions/Engine_MaterialFunctions03_Procedurals__NormalFromFunction.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 4
- Internal nodes: 28
- Description: Takes a heightmap input and generates a normal map from it.
- Input pins: Coordinates [FunctionInput_Vector2], Height Map UV Offset [FunctionInput_Scalar], Normal Map Intensity [FunctionInput_Scalar], Function(UV1) [FunctionInput_Vector3], Function(UV2) [FunctionInput_Vector3], Function(UV3) [FunctionInput_Vector3]
- Output pins: Normal, UV1, UV2, UV3

### NormalFromHeightmap

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/NormalFromHeightmap.NormalFromHeightmap`
- Deep graph doc: [NormalFromHeightmap](Functions/Engine_MaterialFunctions03_Procedurals__NormalFromHeightmap.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 33
- Description: Takes a heightmap input and generates a normal map from it.
- Input pins: Height Map [FunctionInput_Texture2D], Coordinates [FunctionInput_Vector2], Height Map UV Offset [FunctionInput_Scalar], Normal Map Intensity [FunctionInput_Scalar], Height Map Channel Selector [FunctionInput_Vector4]
- Output pins: Result

### NormalFromHeightmapChaos

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/NormalFromHeightmapChaos.NormalFromHeightmapChaos`
- Deep graph doc: [NormalFromHeightmapChaos](Functions/Engine_MaterialFunctions03_Procedurals__NormalFromHeightmapChaos.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 28
- Description: EXPENSIVE - Takes a heightmap input, pans it 4 ways and generates a normal map from it.
- Input pins: Coordinates [FunctionInput_Vector2], Height Bias [FunctionInput_Scalar], Height [FunctionInput_Scalar]
- Output pins: Result

### ObjectSpaceFalloff

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/ObjectSpaceFalloff.ObjectSpaceFalloff`
- Deep graph doc: [ObjectSpaceFalloff](Functions/Engine_MaterialFunctions03_Procedurals__ObjectSpaceFalloff.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 10
- Outputs: 1
- Internal nodes: 44
- Description: Derives a falloff based on object's location.
- Input pins: Falloff Hardness [FunctionInput_Scalar], Aspect Ratio - V [FunctionInput_Scalar], Aspect Ratio - U [FunctionInput_Scalar], Invert 2nd channel [FunctionInput_StaticBool], Falloff Scale [FunctionInput_Scalar], Offset - V [FunctionInput_Scalar], Offset - U [FunctionInput_Scalar], Invert 1st Channel [FunctionInput_StaticBool], Projection Transform Switch [FunctionInput_StaticBool], Offset Transform Switch [FunctionInput_StaticBool]
- Output pins: Result

### PerturbNormalHQ

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/PerturbNormalHQ.PerturbNormalHQ`
- Deep graph doc: [PerturbNormalHQ](Functions/Engine_MaterialFunctions03_Procedurals__PerturbNormalHQ.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 26
- Input pins: Bump center [FunctionInput_Scalar], Bump one pixel down [FunctionInput_Scalar], Bump one pixel right [FunctionInput_Scalar], WS Normal [FunctionInput_Vector3]
- Output pins: WS Normal

### PerturbNormalLQ

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/PerturbNormalLQ.PerturbNormalLQ`
- Deep graph doc: [PerturbNormalLQ](Functions/Engine_MaterialFunctions03_Procedurals__PerturbNormalLQ.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 23
- Input pins: Bump [FunctionInput_Scalar]
- Output pins: WS Normal

### Prepare3DDeriv

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/Prepare3DDeriv.Prepare3DDeriv`
- Deep graph doc: [Prepare3DDeriv](Functions/Engine_MaterialFunctions03_Procedurals__Prepare3DDeriv.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 4
- Internal nodes: 18
- Description: Uses positions offset in a tetrahedral pattern to compute 3D derivatives. Evaluate the same 3D function at each offset position produced by this function, then feed the resulting values into Compute3DDeriv.
- Input pins: Position [FunctionInput_Vector3], Tetrahedral Offset [FunctionInput_Scalar]
- Output pins: Offset1, Offset2, Offset3, Offset4

### PreparePerturbNormalHQ

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/PreparePerturbNormalHQ.PreparePerturbNormalHQ`
- Deep graph doc: [PreparePerturbNormalHQ](Functions/Engine_MaterialFunctions03_Procedurals__PreparePerturbNormalHQ.md)
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 13
- Input pins: Value [FunctionInput_Vector3]
- Output pins: Value one pixel right, Value, Value one pixel down, FilterWidth



