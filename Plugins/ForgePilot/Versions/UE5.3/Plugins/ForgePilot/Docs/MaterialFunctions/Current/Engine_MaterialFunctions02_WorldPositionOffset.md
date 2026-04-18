# Engine_MaterialFunctions02/WorldPositionOffset

Engine: Unreal Engine 5.3
Function count: 22

## Agent Use

Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### AlignMeshToTheCamera

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/AlignMeshToTheCamera.AlignMeshToTheCamera`
- Deep graph doc: [AlignMeshToTheCamera](Functions/Engine_MaterialFunctions02_WorldPositionOffset__AlignMeshToTheCamera.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 5
- Internal nodes: 38
- Description: Aligns a static mesh to the camera and allows the user to rotate the mesh by altering the objects basis vectors.
- Input pins: Custom Object Basis 1 [FunctionInput_Vector3], Custom Object Basis 2 [FunctionInput_Vector3], Custom Object Basis 3 [FunctionInput_Vector3], WS Normals [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], PivotLocation [FunctionInput_Vector3]
- Output pins: World Position Offset, Rotated Normals, Undeformed Pixel Shader World Position, test WPO, CameraXVector

### AttachMeshToTheCamera

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/AttachMeshToTheCamera.AttachMeshToTheCamera`
- Deep graph doc: [AttachMeshToTheCamera](Functions/Engine_MaterialFunctions02_WorldPositionOffset__AttachMeshToTheCamera.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 17
- Description: Attaches a static mesh to the camera and allows the user to rotate the mesh by altering the object bases and also provides a camera space offset variable.
- Input pins: Camera Space Position Offset [FunctionInput_Vector3], Object Basis 1 [FunctionInput_Vector3], Object Basis 2 [FunctionInput_Vector3], Object Basis 3 [FunctionInput_Vector3]
- Output pins: World Position Offset

### CameraOffset

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/CameraOffset.CameraOffset`
- Deep graph doc: [CameraOffset](Functions/Engine_MaterialFunctions02_WorldPositionOffset__CameraOffset.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 15
- Description: Offset objects in the direction of the camera to either make them appear in front or behind other objects. 
- Input pins: Offset Amount [FunctionInput_Scalar], World Position [FunctionInput_Vector3], Clamp Padding [FunctionInput_Scalar]
- Output pins: Camera Offset, Clamped Camera Offset

### ComponentBasisVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ComponentBasisVectors.ComponentBasisVectors`
- Deep graph doc: [ComponentBasisVectors](Functions/Engine_MaterialFunctions02_WorldPositionOffset__ComponentBasisVectors.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 3
- Internal nodes: 9
- Description: The local XYZ component transform vectors in world space.
- Output pins: X, Y, Z

### ComponentPivotLocation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ComponentPivotLocation.ComponentPivotLocation`
- Deep graph doc: [ComponentPivotLocation](Functions/Engine_MaterialFunctions02_WorldPositionOffset__ComponentPivotLocation.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 1
- Internal nodes: 3
- Description: Returns the world position of the components origin.
- Output pins: Result

### DropletParticleVertexShader

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/DropletParticleVertexShader.DropletParticleVertexShader`
- Deep graph doc: [DropletParticleVertexShader](Functions/Engine_MaterialFunctions02_WorldPositionOffset__DropletParticleVertexShader.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 62
- Description: Make a camera aligned particle distort like a raindrop at different viewing angles. The X particle dimension define the sphere radius and the y particle dimension define the water drop length.
- Input pins: Speed Per unit of tail distortion [FunctionInput_Scalar], Max tail lengthing percentage [FunctionInput_Scalar], Min speed before distortion [FunctionInput_Scalar], Distortion lerp rate [FunctionInput_Scalar]
- Output pins: Result

### GenerateASpline

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/GenerateASpline.GenerateASpline`
- Deep graph doc: [GenerateASpline](Functions/Engine_MaterialFunctions02_WorldPositionOffset__GenerateASpline.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 5
- Internal nodes: 64
- Description: Generate a Catmull-Rom spline.
- Input pins: P0 [FunctionInput_Vector3], P1 [FunctionInput_Vector3], P2 [FunctionInput_Vector3], t [FunctionInput_Scalar], P3 [FunctionInput_Vector3], Normal/Bi-Normal Cross product vector [FunctionInput_Vector3]
- Output pins: T position, T Tangent, T Bi-normal, T Normal, UnnormalizedTangent

### Gravity_WPO

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Gravity_WPO.Gravity_WPO`
- Deep graph doc: [Gravity_WPO](Functions/Engine_MaterialFunctions02_WorldPositionOffset__Gravity_WPO.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 2
- Internal nodes: 14
- Description: Find the velocity and updated position of an object that's falling on earth. 
- Input pins: Current Time [FunctionInput_Scalar], Fall Start Time [FunctionInput_Scalar], Gravitational Acceleration [FunctionInput_Vector3]
- Output pins: Offset From Start Position, Instantaneous Velocity

### LocalPosition

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/LocalPosition.LocalPosition`
- Deep graph doc: [LocalPosition](Functions/Engine_MaterialFunctions02_WorldPositionOffset__LocalPosition.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 4
- Internal nodes: 10
- Output pins: Local Position, Local Position (Excluding Offsets), Instance Local Position (Excluding Offsets), Instance Local Position

### ObjectPivotPoint

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ObjectPivotPoint.ObjectPivotPoint`
- Deep graph doc: [ObjectPivotPoint](Functions/Engine_MaterialFunctions02_WorldPositionOffset__ObjectPivotPoint.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 2
- Internal nodes: 5
- Description: Returns the object's pivot point in world space. This is not compatible with the pixel shader.
- Output pins: Object Pivot Location, Mesh Particle Pivot Location

### ObjectScale

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/ObjectScale.ObjectScale`
- Deep graph doc: [ObjectScale](Functions/Engine_MaterialFunctions02_WorldPositionOffset__ObjectScale.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 4
- Internal nodes: 15
- Description: Returns the object's XYZ scale together and seperately.This is not compatible with the pixel shader.
- Output pins: Scale Z, Scale X, Scale Y, Scale XYZ

### PivotAxis

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/PivotAxis.PivotAxis`
- Deep graph doc: [PivotAxis](Functions/Engine_MaterialFunctions02_WorldPositionOffset__PivotAxis.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 13
- Description: Creates a common pivot location on arbitrary axes. This is helpful for creating flag motion. Instead of using a single pivot point near the top of the flag, use a shared z point and unique xy location data to create a more realistic connection along the object's width. Note: this shader node only supports uniform scaling. Also, the rotate about axis axis and the pivot axis/pos should not coincide.
- Input pins: Pivot Axis/Pos [FunctionInput_Vector3]
- Output pins: Pivot

### RotateAboutWorldAxis_cheap

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/RotateAboutWorldAxis_cheap.RotateAboutWorldAxis_cheap`
- Deep graph doc: [RotateAboutWorldAxis_cheap](Functions/Engine_MaterialFunctions02_WorldPositionOffset__RotateAboutWorldAxis_cheap.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 76
- Description: This function cheaply rotates objects around world axes. Input the angle you would like to use and attach the output to world position offset.
- Input pins: PivotPoint [FunctionInput_Vector3], WorldPosition [FunctionInput_Vector3], Rotation Amount [FunctionInput_Scalar]
- Output pins: Z-Axis, Y-Axis, X-Axis

### SineWithNormalSupport

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/SineWithNormalSupport.SineWithNormalSupport`
- Deep graph doc: [SineWithNormalSupport](Functions/Engine_MaterialFunctions02_WorldPositionOffset__SineWithNormalSupport.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 32
- Description: Very specific material function that projects into world space and then maps a normal map in that direction and bends its normals to face in the new direction.  For test purposes only. (May remove eventually)  Outputs worls space normals
- Input pins: X world dot [FunctionInput_Scalar], Y world dot [FunctionInput_Scalar], World Position [FunctionInput_Vector3], time [FunctionInput_Scalar], Rotation fudge factor [FunctionInput_Scalar]
- Output pins: Sine Wave World Normal, SineWave

### SplineBasedModelDeformation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/SplineBasedModelDeformation.SplineBasedModelDeformation`
- Deep graph doc: [SplineBasedModelDeformation](Functions/Engine_MaterialFunctions02_WorldPositionOffset__SplineBasedModelDeformation.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 10
- Outputs: 9
- Internal nodes: 50
- Description: Deform a model around a spline that is created with the four control point inputs. P0 through P3. P0 and P3 are tangent handles.
- Input pins: Model height along deformation vector [FunctionInput_Scalar], P0 [FunctionInput_Vector3], P1 [FunctionInput_Vector3], P2 [FunctionInput_Vector3], P3 [FunctionInput_Vector3], LocalDeformationVector [FunctionInput_Vector3], Thickness [FunctionInput_Vector2], Local Position [FunctionInput_Vector3], Optional Tangent Space Normal Map [FunctionInput_Vector3], Optional Greyscale Height Map [FunctionInput_Scalar]
- Output pins: World Position Offset, Debug Gradient, BiNormal (X), Normal (Y), Tangent (Z), World T Position, Pixel normal (WS Cylindrical), Local Position Offset, Pixel normal (WS Texture)

### Sprite

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite.Sprite`
- Deep graph doc: [Sprite](Functions/Engine_MaterialFunctions02_WorldPositionOffset__Sprite.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 2
- Internal nodes: 31
- Description: Use this function within the world position offset input to align plane meshes with the screen at any location.
- Input pins: XY scale [FunctionInput_Vector2], Center Location [FunctionInput_Vector3], 0-1 UVs [FunctionInput_Vector2], Normals (Optional) [FunctionInput_Vector3], OverridePivot [FunctionInput_StaticBool]
- Output pins: World position offset, Normals

### Sprite_Capsule

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite_Capsule.Sprite_Capsule`
- Deep graph doc: [Sprite_Capsule](Functions/Engine_MaterialFunctions02_WorldPositionOffset__Sprite_Capsule.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 16
- Outputs: 7
- Internal nodes: 187
- Description: The material function performs a number of functions to turn a particle sprite, or plane, into a velocity aligned capsule. 
- Input pins: World Position to Negate [FunctionInput_Vector3], Particle Position [FunctionInput_Vector3], Normalized Particle Direction [FunctionInput_Vector3], Scale Along Velocity Vector [FunctionInput_Scalar], Capsule Diameter [FunctionInput_Scalar], ---------Scale Over Velocity Parameters--------- [FunctionInput_StaticBool], Particle Speed [FunctionInput_Scalar], Minimum Speed [FunctionInput_Scalar], Max Speed [FunctionInput_Scalar], Minimum Scale Factor [FunctionInput_Scalar], Maximum Scale Factor [FunctionInput_Scalar], Use Advanced Features [FunctionInput_StaticBool]
- Output pins: World Position Offset, World Space Normals, Opacity Mask, Texture Coordinates, Pixel Depth Offset, Scale Factor, Front Face Mask

### Sprite_Ellipsoid

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite_Ellipsoid.Sprite_Ellipsoid`
- Deep graph doc: [Sprite_Ellipsoid](Functions/Engine_MaterialFunctions02_WorldPositionOffset__Sprite_Ellipsoid.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 13
- Outputs: 3
- Internal nodes: 52
- Description: The material function will align a particle, or plane, to a normalized velocity vector, and scale on that axis while maintaining it's screen-alignment. The distortion along the velocity vector disappears when the vector and the viewer's line of sight align. The module can be used to represent velocity aligned ellipsoidal shapes on quads.
- Input pins: World Position to Negate [FunctionInput_Vector3], Particle Position [FunctionInput_Vector3], Particle Direction [FunctionInput_Vector3], Scale Along Velocity Vector [FunctionInput_Scalar], Particle Size [FunctionInput_Vector2], Normals [FunctionInput_Vector3], Opacity Mask [FunctionInput_Vector3], ---------Scale Over Velocity Parameters--------- [FunctionInput_StaticBool], Particle Speed [FunctionInput_Scalar], Minimum Speed [FunctionInput_Scalar], Max Speed [FunctionInput_Scalar], Minimum Scale Factor [FunctionInput_Scalar]
- Output pins: World Position Offset, World Space Normal, Opacity Mask

### Sprite_TearDrop

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Sprite_TearDrop.Sprite_TearDrop`
- Deep graph doc: [Sprite_TearDrop](Functions/Engine_MaterialFunctions02_WorldPositionOffset__Sprite_TearDrop.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 10
- Outputs: 4
- Internal nodes: 102
- Description: Stretch a particle out along the velocity vector when it's velocity is perpendicular to the view vector.
- Input pins: Min Tear Drop Width [FunctionInput_Scalar], Max Stretch Factor [FunctionInput_Scalar], Particle Position [FunctionInput_Vector3], ------ Particle Defaults Below ------ [FunctionInput_StaticBool], Particle Direction [FunctionInput_Vector3], Particle Size [FunctionInput_Vector2], Particle Speed [FunctionInput_Scalar], Minimum Speed [FunctionInput_Scalar], Max Speed [FunctionInput_Scalar], UV Add [FunctionInput_Vector2]
- Output pins: World Position Offset, World Space Normals, Opacity Mask, Texture Coordinates

### StaticMeshMorphTargets

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/StaticMeshMorphTargets.StaticMeshMorphTargets`
- Deep graph doc: [StaticMeshMorphTargets](Functions/Engine_MaterialFunctions02_WorldPositionOffset__StaticMeshMorphTargets.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 5
- Internal nodes: 23
- Description: Unpacks Morph Data From the 3ds Max Morph Packer Script
- Output pins: Morph Target 1 WPO, Morph Target 2 WPO, Morph Target 1 Normals, ---------------, Pivot Position WS

### UVLayoutToWorldSpacePosition

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/UVLayoutToWorldSpacePosition.UVLayoutToWorldSpacePosition`
- Deep graph doc: [UVLayoutToWorldSpacePosition](Functions/Engine_MaterialFunctions02_WorldPositionOffset__UVLayoutToWorldSpacePosition.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 18
- Description: Use in the world position offset input to visual uv coordinates in 3D. 
- Input pins: Texture Coordinate [FunctionInput_Vector2]
- Output pins: World Positon

### Wind

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/Wind.Wind`
- Deep graph doc: [Wind](Functions/Engine_MaterialFunctions02_WorldPositionOffset__Wind.md)
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 4
- Internal nodes: 12
- Description: Seperate outputs for wind strength, speed multiplied by time and a normalized wind vector.
- Input pins: WindActor [FunctionInput_Vector4]
- Output pins: WindSpeed, Wind Strength, Normalized Wind Vector, WindActor



