# Engine_MaterialFunctions01/AlphaBlend

Engine: Unreal Engine 5.3
Function count: 1

## Agent Use

Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.

Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.

## Functions

### WorldAlignedBlend

- Path: `/Engine/Functions/Engine_MaterialFunctions01/AlphaBlend/WorldAlignedBlend.WorldAlignedBlend`
- Deep graph doc: [WorldAlignedBlend](Functions/Engine_MaterialFunctions01_AlphaBlend__WorldAlignedBlend.md)
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 3
- Internal nodes: 55
- Input pins: In World Vector [FunctionInput_Vector3], Blend Bias [FunctionInput_Scalar], Blend Sharpness [FunctionInput_Scalar], In Explicit Normal [FunctionInput_Vector3], Clamped? [FunctionInput_StaticBool], Alpha [FunctionInput_Scalar]
- Output pins: Alpha, w/ Explicit Normal, w/Vertex Normals

