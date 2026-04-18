# UE 5.3 Material Function Study

Generated for ForgePilot so Codex, Claude, and other AI agents can choose Material Functions instead of rebuilding large expression chains by hand.

## Rules For Agents

- Treat Material Functions as reusable graph blocks made from Material Expressions.
- Prefer functions for complex reusable logic: flipbooks, radial UVs, world aligned mapping, blend modes, remap/contrast, procedural helpers, and packed channel helpers.
- Keep simple math inline with expressions when it is only one or two nodes.
- Before wiring a MaterialFunctionCall, call `get_material_function_info` and connect by `target_input_name` rather than guessed pin index.
- A function is not automatically cheaper at shader runtime; it mainly improves graph readability, reuse, and consistency.

## Category Docs

- [DistanceFields/Combiners](DistanceFields_Combiners.md) - 3 functions
- [DistanceFields/Shapes](DistanceFields_Shapes.md) - 3 functions
- [Engine_MaterialFunctions01/AlphaBlend](Engine_MaterialFunctions01_AlphaBlend.md) - 1 functions
- [Engine_MaterialFunctions01/Chromakeying](Engine_MaterialFunctions01_Chromakeying.md) - 6 functions
- [Engine_MaterialFunctions01/Coordinates](Engine_MaterialFunctions01_Coordinates.md) - 13 functions
- [Engine_MaterialFunctions01/Cubemaps](Engine_MaterialFunctions01_Cubemaps.md) - 1 functions
- [Engine_MaterialFunctions01/Debug](Engine_MaterialFunctions01_Debug.md) - 7 functions
- [Engine_MaterialFunctions01/Decal](Engine_MaterialFunctions01_Decal.md) - 1 functions
- [Engine_MaterialFunctions01/Density](Engine_MaterialFunctions01_Density.md) - 2 functions
- [Engine_MaterialFunctions01/Gradient](Engine_MaterialFunctions01_Gradient.md) - 10 functions
- [Engine_MaterialFunctions01/ImageAdjustment](Engine_MaterialFunctions01_ImageAdjustment.md) - 16 functions
- [Engine_MaterialFunctions01/Landscape](Engine_MaterialFunctions01_Landscape.md) - 1 functions
- [Engine_MaterialFunctions01/Lighting](Engine_MaterialFunctions01_Lighting.md) - 2 functions
- [Engine_MaterialFunctions01/Math](Engine_MaterialFunctions01_Math.md) - 6 functions
- [Engine_MaterialFunctions01/Opacity](Engine_MaterialFunctions01_Opacity.md) - 4 functions
- [Engine_MaterialFunctions01/Reflections](Engine_MaterialFunctions01_Reflections.md) - 4 functions
- [Engine_MaterialFunctions01/Shading](Engine_MaterialFunctions01_Shading.md) - 12 functions
- [Engine_MaterialFunctions01/SpeedTree](Engine_MaterialFunctions01_SpeedTree.md) - 3 functions
- [Engine_MaterialFunctions01/Texturing](Engine_MaterialFunctions01_Texturing.md) - 42 functions
- [Engine_MaterialFunctions01/Units](Engine_MaterialFunctions01_Units.md) - 2 functions
- [Engine_MaterialFunctions01/Vectors](Engine_MaterialFunctions01_Vectors.md) - 5 functions
- [Engine_MaterialFunctions01/Volumetrics](Engine_MaterialFunctions01_Volumetrics.md) - 3 functions
- [Engine_MaterialFunctions01/WorldPositionOffset](Engine_MaterialFunctions01_WorldPositionOffset.md) - 5 functions
- [Engine_MaterialFunctions02](Engine_MaterialFunctions02.md) - 37 functions
- [Engine_MaterialFunctions02/Gradients](Engine_MaterialFunctions02_Gradients.md) - 6 functions
- [Engine_MaterialFunctions02/Masking](Engine_MaterialFunctions02_Masking.md) - 2 functions
- [Engine_MaterialFunctions02/Math](Engine_MaterialFunctions02_Math.md) - 26 functions
- [Engine_MaterialFunctions02/MAXScripts](Engine_MaterialFunctions02_MAXScripts.md) - 8 functions
- [Engine_MaterialFunctions02/Normals](Engine_MaterialFunctions02_Normals.md) - 3 functions
- [Engine_MaterialFunctions02/NotInUse](Engine_MaterialFunctions02_NotInUse.md) - 2 functions
- [Engine_MaterialFunctions02/Particles](Engine_MaterialFunctions02_Particles.md) - 1 functions
- [Engine_MaterialFunctions02/PivotPainter](Engine_MaterialFunctions02_PivotPainter.md) - 6 functions
- [Engine_MaterialFunctions02/PivotPainter2](Engine_MaterialFunctions02_PivotPainter2.md) - 9 functions
- [Engine_MaterialFunctions02/StaticMeshDecals](Engine_MaterialFunctions02_StaticMeshDecals.md) - 1 functions
- [Engine_MaterialFunctions02/Texturing](Engine_MaterialFunctions02_Texturing.md) - 21 functions
- [Engine_MaterialFunctions02/Utility](Engine_MaterialFunctions02_Utility.md) - 32 functions
- [Engine_MaterialFunctions02/UVs](Engine_MaterialFunctions02_UVs.md) - 8 functions
- [Engine_MaterialFunctions02/WorldPositionOffset](Engine_MaterialFunctions02_WorldPositionOffset.md) - 22 functions
- [Engine_MaterialFunctions02/WorldPositionOffset/V2](Engine_MaterialFunctions02_WorldPositionOffset_V2.md) - 2 functions
- [Engine_MaterialFunctions03/Blends](Engine_MaterialFunctions03_Blends.md) - 16 functions
- [Engine_MaterialFunctions03/ComplexMotion](Engine_MaterialFunctions03_ComplexMotion.md) - 2 functions
- [Engine_MaterialFunctions03/Math](Engine_MaterialFunctions03_Math.md) - 13 functions
- [Engine_MaterialFunctions03/Particles](Engine_MaterialFunctions03_Particles.md) - 3 functions
- [Engine_MaterialFunctions03/Procedurals](Engine_MaterialFunctions03_Procedurals.md) - 15 functions
- [Engine_MaterialFunctions03/Texturing](Engine_MaterialFunctions03_Texturing.md) - 9 functions
- [GetPostProcessFunctions](GetPostProcessFunctions.md) - 2 functions
- [MaterialLayerFunctions](MaterialLayerFunctions.md) - 38 functions
- [Strata](Strata.md) - 17 functions
- [UserInterface](UserInterface.md) - 1 functions

## Source

- Catalog: `Knowledge/Materials/Reference/material_node_catalog_ue53.json`
- Exporter: `Tools/Validation/export_material_function_docs.ps1`
- IncludeFunctionInfo: True
