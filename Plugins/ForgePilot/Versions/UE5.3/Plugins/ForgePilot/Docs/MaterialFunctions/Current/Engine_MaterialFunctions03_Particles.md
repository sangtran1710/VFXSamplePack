# Engine_MaterialFunctions03/Particles

Engine: Unreal Engine 5.3
Function count: 3

## Agent Use

Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### Distance_Blend

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Particles/Distance_Blend.Distance_Blend`
- Deep graph doc: [Distance_Blend](Functions/Engine_MaterialFunctions03_Particles__Distance_Blend.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 7
- Description: Sets a distance and range to blend from 0 to 1
- Input pins: Start Offset [FunctionInput_Scalar], Blend Range [FunctionInput_Scalar]
- Output pins: Result

### ParticleDOF

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Particles/ParticleDOF.ParticleDOF`
- Deep graph doc: [ParticleDOF](Functions/Engine_MaterialFunctions03_Particles__ParticleDOF.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 2
- Internal nodes: 22
- Description: Use with with CircleDOF
- Input pins: Opacity [FunctionInput_Scalar]
- Output pins: WorldPositionOffset, Opacity

### RandomUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Particles/RandomUVs.RandomUVs`
- Deep graph doc: [RandomUVs](Functions/Engine_MaterialFunctions03_Particles__RandomUVs.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 6
- Description: Uses a DynamicParameter with U_Offset and V_Offset as parameter names to move UV coordinates around.  Set up as a uniform distribution, at spawn time only, with a 0-1 distribution per-channel.
- Input pins: Coordinate Input [FunctionInput_Vector2]
- Output pins: UV out



