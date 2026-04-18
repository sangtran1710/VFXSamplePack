# MS_VertexAnimationTools_MorphTargets

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_VertexAnimationTools_MorphTargets.MS_VertexAnimationTools_MorphTargets`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This material function will apply the VertexAnimationTools morph target to an appropriate static mesh. 

## Inputs

- `Morph Texture` [FunctionInput_Texture2D] - Insert the .EXR texture here.
- `Number of Morph Targets` [FunctionInput_Scalar] - 1-based number indicating the number of morph targets in the morph texture.
- `Morph Animation` [FunctionInput_Scalar] - 0-based number that is used to indicate what stage of the morph target animation is being used. If "0-1 Animation Value?" is set to true then 1 will represent the last frame of the animation otherwise the last frame will be equal to the number of frames -1.
- `0-1 Animation Value?` [FunctionInput_StaticBool] - Defaults to true. Alternative is that each full number value will transition 1 morph target
- `Morph Normal` [FunctionInput_Texture2D] - Input the morph texture ending with _Normal.
- `Morph Target UV` [FunctionInput_Vector2] - This value should remain UV 1 unless the mesh was processed with another uv coordinate index. This feature was added after 4.13.

## Outputs

- `Pixel Shader Tangent Vertex Normal (See tooltip)` - Note: Tangent Space normals may create surface artifacts. Ideally, one should use the world space  normal output above and plug it directly into the normals material input. Tangent Space Normals should be unchecked in the shader when using world space normals. 

This value only puts out the meshes deformed vertex normal if the custom uv inputs are attached. 

Also, one mustn't use vertex normal in their base shader when applying this technique. The vertex normals do not get updated and therefore that node will produce incorrect results.


- `World Position Offset` - Outputs morph target offsets
- `Custom UV 2 (Needed)` - Connecting this Output pin to Custom UV 2 will enable the normal output pin to work correctly.

This will allow you to sample the noraml texture in the vertex shader and then pass it to the normal input in the pixel shader. 
- `Custom UV 3 (Needed)`
- `Vert shader Normal (Advanced)` - This feature won't be needed for most uses. If you do however need the deformed normal in the vertex shader, then you should use this value instead of the other normal output. This cannot be used in the pixel shader. 
- `Pixel Shader World Vertex Normal (See tooltip)` - Note: Due to certain limitations, this output pin should be used instead of tangent space normals. Transforming the normals to tangent space should be avoided. Plug this output pin directly into the materials normal input and UNCHECK Tangent Space  Normals in the materials properties. 

This value only puts out the meshes deformed vertex normal if the custom uv inputs are attached.

Also, one mustn't use vertex normal in their base shader when applying this technique. The vertex normals do not get updated and therefore that node will produce incorrect results.


## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Shader Tangent Vertex Normal (See tooltip) | Pixel Shader Tangent Vertex Normal (See tooltip) |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionAppendVector` | Append |  |
| 3 | expression | `MaterialExpressionAppendVector` | Append |  |
| 4 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 5 | function_call | `MaterialExpressionMaterialFunctionCall` | GradientMap_Multi |  |
| 6 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 7 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 9 | expression | `MaterialExpressionConstant` | 0 |  |
| 10 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 11 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 12 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 13 | function_call | `MaterialExpressionMaterialFunctionCall` | GradientMap_Multi |  |
| 14 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 15 | expression | `MaterialExpressionFloor` | Floor |  |
| 16 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 17 | expression | `MaterialExpressionFrac` | Frac |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 2 (Needed) | Custom UV 2 (Needed) |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Custom UV 3 (Needed) | Custom UV 3 (Needed) |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Morph Texture (Texture2D) | Morph Texture |
| 24 | function_input | `MaterialExpressionFunctionInput` | Input Number of Morph Targets (Scalar) | Number of Morph Targets |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Morph Animation (Scalar) | Morph Animation |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input 0-1 Animation Value? (StaticBool) | 0-1 Animation Value? |
| 27 | expression | `MaterialExpressionStaticBool` | Static Bool (True) |  |
| 28 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 29 | function_call | `MaterialExpressionMaterialFunctionCall` | TimeWithSpeedVariable |  |
| 30 | expression | `MaterialExpressionDivide` | Divide(4,) |  |
| 31 | function_call | `MaterialExpressionMaterialFunctionCall` | UnpackNormalFromFloat |  |
| 32 | expression | `MaterialExpressionAppendVector` | Append |  |
| 33 | expression | `MaterialExpressionAppendVector` | Append |  |
| 34 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 35 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 36 | function_call | `MaterialExpressionMaterialFunctionCall` | GradientMap_Multi |  |
| 37 | function_call | `MaterialExpressionMaterialFunctionCall` | GradientMap_Multi |  |
| 38 | function_input | `MaterialExpressionFunctionInput` | Input Morph Normal (Texture2D) | Morph Normal |
| 39 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 40 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output Vert shader Normal (Advanced) | Vert shader Normal (Advanced) |
| 42 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 43 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 44 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 45 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 46 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 47 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 48 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel Shader World Vertex Normal (See tooltip) | Pixel Shader World Vertex Normal (See tooltip) |
| 49 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 50 | function_input | `MaterialExpressionFunctionInput` | Input Morph Target UV (Vector2) | Morph Target UV |

## Exact Input Wiring

### [0] Output Pixel Shader Tangent Vertex Normal (See tooltip)

- `A` <= [47] Normalize output 0

### [1] Append

- `A` <= [10] TexCoord[2] output 0
- `B` <= [12] BreakOutFloat2Components output 0

### [2] Append

- `A` <= [8] BreakOutFloat3Components output 0
- `B` <= [8] BreakOutFloat3Components output 1

### [3] Append

- `A` <= [8] BreakOutFloat3Components output 2
- `B` <= [9] 0 output 0

### [7] Instance & Particle Space to World Space TransformVector

- `Input` <= [34] Mask ( R G B ) output 0

### [14] Add(,1)

- `A` <= [15] Floor output 0

### [15] Floor

- `Input` <= [28] Switch output 0

### [16] Lerp

- `A` <= [33] Append output 0
- `B` <= [32] Append output 0
- `Alpha` <= [17] Frac output 0

### [17] Frac

- `Input` <= [28] Switch output 0

### [18] Multiply

- `A` <= [25] Input Morph Animation (Scalar) output 0
- `B` <= [19] Subtract(,1) output 0

### [19] Subtract(,1)

- `A` <= [24] Input Number of Morph Targets (Scalar) output 0

### [20] Output World Position Offset

- `A` <= [7] Instance & Particle Space to World Space TransformVector output 0

### [21] Output Custom UV 2 (Needed)

- `A` <= [2] Append output 0

### [22] Output Custom UV 3 (Needed)

- `A` <= [3] Append output 0

### [23] Input Morph Texture (Texture2D)

- `Preview` <= [42] Texture Object output 0

### [25] Input Morph Animation (Scalar)

- `Preview` <= [29] TimeWithSpeedVariable output 0

### [26] Input 0-1 Animation Value? (StaticBool)

- `Preview` <= [27] Static Bool (True) output 0

### [28] Switch

- `A` <= [18] Multiply output 0
- `B` <= [25] Input Morph Animation (Scalar) output 0
- `Value` <= [26] Input 0-1 Animation Value? (StaticBool) output 0

### [30] Divide(4,)

- `B` <= [24] Input Number of Morph Targets (Scalar) output 0

### [32] Append

- `A` <= [13] GradientMap_Multi output 0
- `B` <= [13] GradientMap_Multi output 1

### [33] Append

- `A` <= [5] GradientMap_Multi output 0
- `B` <= [5] GradientMap_Multi output 1

### [34] Mask ( R G B )

- `Input` <= [16] Lerp output 0

### [35] Mask ( A )

- `Input` <= [16] Lerp output 0

### [38] Input Morph Normal (Texture2D)

- `Preview` <= [43] Texture Object output 0

### [39] Lerp

- `A` <= [36] GradientMap_Multi output 0
- `B` <= [37] GradientMap_Multi output 0
- `Alpha` <= [17] Frac output 0

### [40] ConstantBiasScale

- `Input` <= [39] Lerp output 0

### [41] Output Vert shader Normal (Advanced)

- `A` <= [45] Normalize output 0

### [44] Instance & Particle Space to World Space TransformVector

- `Input` <= [40] ConstantBiasScale output 0

### [45] Normalize

- `VectorInput` <= [44] Instance & Particle Space to World Space TransformVector output 0

### [46] World Space to Tangent Space TransformVector

- `Input` <= [1] Append output 0

### [47] Normalize

- `VectorInput` <= [46] World Space to Tangent Space TransformVector output 0

### [48] Output Pixel Shader World Vertex Normal (See tooltip)

- `A` <= [49] Normalize output 0

### [49] Normalize

- `VectorInput` <= [1] Append output 0

### [50] Input Morph Target UV (Vector2)

- `Preview` <= [4] TexCoord[1] output 0

## Connection List

- [47] Normalize output 0 -> Output Pixel Shader Tangent Vertex Normal (See tooltip).A
- [10] TexCoord[2] output 0 -> Append.A
- [12] BreakOutFloat2Components output 0 -> Append.B
- [8] BreakOutFloat3Components output 0 -> Append.A
- [8] BreakOutFloat3Components output 1 -> Append.B
- [8] BreakOutFloat3Components output 2 -> Append.A
- [9] 0 output 0 -> Append.B
- [34] Mask ( R G B ) output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [15] Floor output 0 -> Add(,1).A
- [28] Switch output 0 -> Floor.Input
- [33] Append output 0 -> Lerp.A
- [32] Append output 0 -> Lerp.B
- [17] Frac output 0 -> Lerp.Alpha
- [28] Switch output 0 -> Frac.Input
- [25] Input Morph Animation (Scalar) output 0 -> Multiply.A
- [19] Subtract(,1) output 0 -> Multiply.B
- [24] Input Number of Morph Targets (Scalar) output 0 -> Subtract(,1).A
- [7] Instance & Particle Space to World Space TransformVector output 0 -> Output World Position Offset.A
- [2] Append output 0 -> Output Custom UV 2 (Needed).A
- [3] Append output 0 -> Output Custom UV 3 (Needed).A
- [42] Texture Object output 0 -> Input Morph Texture (Texture2D).Preview
- [29] TimeWithSpeedVariable output 0 -> Input Morph Animation (Scalar).Preview
- [27] Static Bool (True) output 0 -> Input 0-1 Animation Value? (StaticBool).Preview
- [18] Multiply output 0 -> Switch.A
- [25] Input Morph Animation (Scalar) output 0 -> Switch.B
- [26] Input 0-1 Animation Value? (StaticBool) output 0 -> Switch.Value
- [24] Input Number of Morph Targets (Scalar) output 0 -> Divide(4,).B
- [13] GradientMap_Multi output 0 -> Append.A
- [13] GradientMap_Multi output 1 -> Append.B
- [5] GradientMap_Multi output 0 -> Append.A
- [5] GradientMap_Multi output 1 -> Append.B
- [16] Lerp output 0 -> Mask ( R G B ).Input
- [16] Lerp output 0 -> Mask ( A ).Input
- [43] Texture Object output 0 -> Input Morph Normal (Texture2D).Preview
- [36] GradientMap_Multi output 0 -> Lerp.A
- [37] GradientMap_Multi output 0 -> Lerp.B
- [17] Frac output 0 -> Lerp.Alpha
- [39] Lerp output 0 -> ConstantBiasScale.Input
- [45] Normalize output 0 -> Output Vert shader Normal (Advanced).A
- [40] ConstantBiasScale output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [44] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [1] Append output 0 -> World Space to Tangent Space TransformVector.Input
- [46] World Space to Tangent Space TransformVector output 0 -> Normalize.VectorInput
- [49] Normalize output 0 -> Output Pixel Shader World Vertex Normal (See tooltip).A
- [1] Append output 0 -> Normalize.VectorInput
- [4] TexCoord[1] output 0 -> Input Morph Target UV (Vector2).Preview

## Output Trace Roots

- `Pixel Shader Tangent Vertex Normal (See tooltip)` is driven by [47] Normalize
- `World Position Offset` is driven by [7] Instance & Particle Space to World Space TransformVector
- `Custom UV 2 (Needed)` is driven by [2] Append
- `Custom UV 3 (Needed)` is driven by [3] Append
- `Vert shader Normal (Advanced)` is driven by [45] Normalize
- `Pixel Shader World Vertex Normal (See tooltip)` is driven by [49] Normalize

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


