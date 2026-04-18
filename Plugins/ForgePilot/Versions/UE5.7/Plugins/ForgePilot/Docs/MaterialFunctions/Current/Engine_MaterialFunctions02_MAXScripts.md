# Engine_MaterialFunctions02/MAXScripts

Engine: Unreal Engine 5.7
Function count: 8

## Agent Use

Example/support content. Treat as reference first; validate before production use.

Risk: Reference-only by default; verify visual output and packaging before production use.

## Functions

### MS_CanopyCreatorMeshExpansion

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_CanopyCreatorMeshExpansion.MS_CanopyCreatorMeshExpansion`
- Deep graph doc: [MS_CanopyCreatorMeshExpansion](Functions/Engine_MaterialFunctions02_MAXScripts__MS_CanopyCreatorMeshExpansion.md)
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 5
- Internal nodes: 40
- Description: This function  widens strips of polies along the U of their UVs.  These strips shouuld be extremely thin and imported with remove degenerate triangles disabled. Useful for Hair, Grass, Cables etc.
- Input pins: WorldPosition [FunctionInput_Vector3], Normal Map [FunctionInput_Vector3]
- Output pins: Normal, WorldPositionOffset, Black=Spline Thicken White=Normal, Branch Radius, white = knots black = everything else

### MS_MultiNormal_UVnormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_MultiNormal_UVnormals.MS_MultiNormal_UVnormals`
- Deep graph doc: [MS_MultiNormal_UVnormals](Functions/Engine_MaterialFunctions02_MAXScripts__MS_MultiNormal_UVnormals.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 3
- Internal nodes: 23
- Description: This material function was designed to be used in conjunction with the MAXScript located here "\UE4\Engine\Extras\3dsMaxScripts\MultiNormal.ms". This particular function outputs vertex normals stored in the UVs. 
- Output pins: Tangent Space UVs, Custom UV 4, Custom UV 5

### MS_MultiNormal_VertexColorNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_MultiNormal_VertexColorNormals.MS_MultiNormal_VertexColorNormals`
- Deep graph doc: [MS_MultiNormal_VertexColorNormals](Functions/Engine_MaterialFunctions02_MAXScripts__MS_MultiNormal_VertexColorNormals.md)
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 1
- Internal nodes: 5
- Description: This material function was designed to be used in conjunction with the MAXScript located here "\UE4\Engine\Extras\3dsMaxScripts\MultiNormal.ms". This particular function outputs vertex normals stored in the Vertex Colors. 
- Output pins: Vertex Color Based Tangent Normals

### MS_SequencePainter_Sequence

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_SequencePainter_Sequence.MS_SequencePainter_Sequence`
- Deep graph doc: [MS_SequencePainter_Sequence](Functions/Engine_MaterialFunctions02_MAXScripts__MS_SequencePainter_Sequence.md)
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 0
- Outputs: 1
- Internal nodes: 4
- Output pins: Result

### MS_SequencePainter_SequenceFlipbook

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_SequencePainter_SequenceFlipbook.MS_SequencePainter_SequenceFlipbook`
- Deep graph doc: [MS_SequencePainter_SequenceFlipbook](Functions/Engine_MaterialFunctions02_MAXScripts__MS_SequencePainter_SequenceFlipbook.md)
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 16
- Description: This function will create a mesh flipbook from assets processed with the "Sequence Painter" section of the "Texture Morph"  MAXScript.
- Input pins: Number Of Frames [FunctionInput_Scalar], 0-1 Animation [FunctionInput_Scalar]
- Output pins: World Position Offset

### ms_StaticMeshSkeletalAnimation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/ms_StaticMeshSkeletalAnimation.ms_StaticMeshSkeletalAnimation`
- Deep graph doc: [ms_StaticMeshSkeletalAnimation](Functions/Engine_MaterialFunctions02_MAXScripts__ms_StaticMeshSkeletalAnimation.md)
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 8
- Outputs: 3
- Internal nodes: 34
- Description: This material function decodes the skeletal mesh animation textures and bone index data packed by the 3DS MAXScript located here: ...Engine\Extras\3dsMaxScripts\StaticMeshSkeletalAnimation.ms. For more information please refer to the guide button with the script and online documentation.
- Input pins: Vertex Shader Bone Index [FunctionInput_Scalar], Pixel Shader Bone Index [FunctionInput_Scalar], Animation Phase [FunctionInput_Scalar], Rotation Animation [FunctionInput_Texture2D], World Space Normals [FunctionInput_Vector3], Position Animation [FunctionInput_Texture2D], Rest Pose Texture [FunctionInput_Texture2D], World Position [FunctionInput_Vector3]
- Output pins: World Position Offset, World Space Normals, Advanced: Mesh Rotation Axis And Angle

### ms_StaticMeshSkeletalAnimationHighQuality

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/ms_StaticMeshSkeletalAnimationHighQuality.ms_StaticMeshSkeletalAnimationHighQuality`
- Deep graph doc: [ms_StaticMeshSkeletalAnimationHighQuality](Functions/Engine_MaterialFunctions02_MAXScripts__ms_StaticMeshSkeletalAnimationHighQuality.md)
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 10
- Outputs: 13
- Internal nodes: 70
- Description: This material function decodes the skeletal mesh animation textures and bone index data packed by the 3DS MAXScript located here: ...Engine\Extras\3dsMaxScripts\StaticMeshSkeletalAnimation.ms. When using this material function in a chain, feed the output vectors to the subsequent material function. You'll only need to pipe the normal texture input into the last material function. The last instance of the function should be connected to the base materials custom uv inputs.  For more information please refer to the guide button with the script and online documentation. 
- Input pins: Vertex Shader Bone Index [FunctionInput_Scalar], Animation Phase [FunctionInput_Scalar], Rotation Animation [FunctionInput_Texture2D], Tangent Space Normal Map [FunctionInput_Vector3], Position Animation [FunctionInput_Texture2D], Rest Pose Texture [FunctionInput_Texture2D], Tangent Basis X In WS [FunctionInput_Vector3], Tangent Basis Y In WS [FunctionInput_Vector3], Tangent Basis Z In WS [FunctionInput_Vector3], World Position [FunctionInput_Vector3]
- Output pins: World Position Offset, World Space Normals, Updated Vertex Shader X Vector, Updated Vertex Shader Y Vector, Updated Vertex Shader Z Vector, Custom UV 2, Custom UV 3, Custom UV 4, Custom UV 5, Custom UV 6, Updated Pixel Shader X Vector, Updated Pixel Shader Y Vector

### MS_VertexAnimationTools_MorphTargets

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_VertexAnimationTools_MorphTargets.MS_VertexAnimationTools_MorphTargets`
- Deep graph doc: [MS_VertexAnimationTools_MorphTargets](Functions/Engine_MaterialFunctions02_MAXScripts__MS_VertexAnimationTools_MorphTargets.md)
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 6
- Internal nodes: 51
- Description: This material function will apply the VertexAnimationTools morph target to an appropriate static mesh. 
- Input pins: Morph Texture [FunctionInput_Texture2D], Number of Morph Targets [FunctionInput_Scalar], Morph Animation [FunctionInput_Scalar], 0-1 Animation Value? [FunctionInput_StaticBool], Morph Normal [FunctionInput_Texture2D], Morph Target UV [FunctionInput_Vector2]
- Output pins: Pixel Shader Tangent Vertex Normal (See tooltip), World Position Offset, Custom UV 2 (Needed), Custom UV 3 (Needed), Vert shader Normal (Advanced), Pixel Shader World Vertex Normal (See tooltip)



