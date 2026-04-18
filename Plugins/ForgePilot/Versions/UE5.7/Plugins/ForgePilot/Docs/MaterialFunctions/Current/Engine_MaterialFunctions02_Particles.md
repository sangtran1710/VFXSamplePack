# Engine_MaterialFunctions02/Particles

Engine: Unreal Engine 5.7
Function count: 1

## Agent Use

Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### ParticleSizeByPixelUnits

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Particles/ParticleSizeByPixelUnits.ParticleSizeByPixelUnits`
- Deep graph doc: [ParticleSizeByPixelUnits](Functions/Engine_MaterialFunctions02_Particles__ParticleSizeByPixelUnits.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 3
- Internal nodes: 55
- Description: This function replaces the vertex shader for sprite particles with screen space sprites. The particles XY dimension are interpretted as the number of pixels that the particle should span rather than the number of world space units that the particle should occupy.
- Input pins: Normals (Optional) [FunctionInput_Vector3], Size In Pixels [FunctionInput_Vector2], ScreenSpace Pivot Offset [FunctionInput_Vector2], ParticlePosition [FunctionInput_Vector3]
- Output pins: World Position Offset, Normals, World Position Offset With Pivot Support



