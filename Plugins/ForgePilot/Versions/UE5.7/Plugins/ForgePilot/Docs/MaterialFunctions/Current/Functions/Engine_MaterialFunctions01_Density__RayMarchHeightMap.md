# RayMarchHeightMap

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Density/RayMarchHeightMap.RayMarchHeightMap`
- Category: Engine_MaterialFunctions01/Density
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function Ray Marches a heightmap to return shadow density.

## Inputs

- `Texture Object` [FunctionInput_Texture2D] - The texture containing the heightmap to ray march.
- `UVs` [FunctionInput_Vector2] - The UVs to use. If this is for a flipbook, plug in the flipbook UVs here.
- `Light Vector WorldSpace` [FunctionInput_Vector3] - The Light Vector to trace towards in World Space. The vector will be transformed into tangent space inside of the function.
- `Max Steps` [FunctionInput_Scalar] - The max number of steps to trace. The tracer will exit early if it crosses the UV border.
- `Trace Distance` [FunctionInput_Scalar] - This defaults to 1 which means the function will trace across 1 UV width. Using smaller values can give higher quality results if you do not need to trace the entire UV width, such as for very dense smoke where the shadows would accumulate to black more quickly.
- `Start Bias` [FunctionInput_Scalar] - Starting directly at the surface of the heightmap can cause more artifacts and does not look very volumetric. Starting the trace a bit below the surface improves quality and approximates an integration of multiple nested density/lighting deep much better.
- `Height Scale` [FunctionInput_Scalar] - This setting lets you scale the height of the heightmap.
- `Shadow Density` [FunctionInput_Scalar] - This controls the density of the shadow result.
- `SubUV Frames` [FunctionInput_Vector2] - For non SubUV textures leave this value at 1, otherwise set this to equal the number of X,Y frames.
- `Animation Phase` [FunctionInput_Scalar] - This is the 0-1 animation phase of the SubUV, if any.
- `Temporal Jitter` [FunctionInput_StaticBool]
- `Heightmap Channel` [FunctionInput_Vector4]

## Outputs

- `Light Energy`
- `Step Complexity` - Step Complexity can show how the cost of this function is dependent on viewing angle and lighting direction and number of steps, as well as the heightmap content.
- `Raw Heightmap Value` - This is a single lookup used from the specified heightmap texture and channel.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Light Energy | Light Energy |
| 1 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 2 | expression | `MaterialExpressionVectorParameter` | Param (0,1,0,0) 'LV' |  |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | Unspecified Function |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 7 | expression | `MaterialExpressionTransform` | World Space to Tangent Space TransformVector |  |
| 8 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 9 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 10 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 11 | custom | `MaterialExpressionCustom` | RayMarch Heightmap |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Texture Object (Texture2D) | Texture Object |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input Light Vector WorldSpace (Vector3) | Light Vector WorldSpace |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Max Steps (Scalar) | Max Steps |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Trace Distance (Scalar) | Trace Distance |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Start Bias (Scalar) | Start Bias |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Height Scale (Scalar) | Height Scale |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Shadow Density (Scalar) | Shadow Density |
| 20 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 21 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 22 | expression | `MaterialExpressionConstant` | 1 |  |
| 23 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 24 | expression | `MaterialExpressionConstant` | 0 |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 26 | function_call | `MaterialExpressionMaterialFunctionCall` | BoxIntersection |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input SubUV Frames (Vector2) | SubUV Frames |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input Animation Phase (Scalar) | Animation Phase |
| 29 | function_call | `MaterialExpressionMaterialFunctionCall` | 1Dto2DIndex |  |
| 30 | expression | `MaterialExpressionDivide` | Divide |  |
| 31 | expression | `MaterialExpressionDivide` | Divide |  |
| 32 | expression | `MaterialExpressionAdd` | Add |  |
| 33 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 34 | function_input | `MaterialExpressionFunctionInput` | Input Temporal Jitter (StaticBool) | Temporal Jitter |
| 35 | expression | `MaterialExpressionFrac` | Frac |  |
| 36 | expression | `MaterialExpressionDivide` | Divide |  |
| 37 | expression | `MaterialExpressionAppendVector` | Append |  |
| 38 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 39 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Start Threshold' |  |
| 40 | expression | `MaterialExpressionFloor` | Floor |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 42 | function_output | `MaterialExpressionFunctionOutput` | Output Step Complexity | Step Complexity |
| 43 | custom | `MaterialExpressionCustom` | ShaderComplexity |  |
| 44 | expression | `MaterialExpressionMultiply` | Multiply(,0.499) |  |
| 45 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 47 | expression | `MaterialExpressionConstant` | 1 |  |
| 48 | expression | `MaterialExpressionConstant` | 0 |  |
| 49 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 50 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 51 | expression | `MaterialExpressionAppendVector` | Append |  |
| 52 | function_input | `MaterialExpressionFunctionInput` | Input Heightmap Channel (Vector4) | Heightmap Channel |
| 53 | expression | `MaterialExpressionMin` | Min |  |
| 54 | expression | `MaterialExpressionMax` | Max |  |
| 55 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 56 | expression | `MaterialExpressionDivide` | Divide |  |
| 57 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 58 | function_call | `MaterialExpressionMaterialFunctionCall` | MakeFloat3 |  |
| 59 | expression | `MaterialExpressionConstant` | 1 |  |
| 60 | function_output | `MaterialExpressionFunctionOutput` | Output Raw Heightmap Value | Raw Heightmap Value |
| 61 | expression | `MaterialExpressionCeil` | Ceil |  |
| 62 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 63 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 64 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 65 | expression | `MaterialExpressionFrac` | Frac |  |
| 66 | function_call | `MaterialExpressionMaterialFunctionCall` | BeersLaw |  |

## Exact Input Wiring

### [0] Output Light Energy

- `A` <= [66] BeersLaw output 0

### [1] Multiply(,1)

- `A` <= [8] Multiply output 0

### [5] Multiply(,-1)

- `A` <= [6] Mask ( R G B ) output 0

### [6] Mask ( R G B )

- `Input` <= [2] Param (0,1,0,0) 'LV' output 0

### [7] World Space to Tangent Space TransformVector

- `Input` <= [10] Normalize output 0

### [8] Multiply

- `A` <= [9] Divide(1,) output 0
- `B` <= [46] Multiply output 0

### [9] Divide(1,)

- `B` <= [15] Input Max Steps (Scalar) output 0

### [10] Normalize

- `VectorInput` <= [14] Input Light Vector WorldSpace (Vector3) output 0

### [12] Input Texture Object (Texture2D)

- `Preview` <= [21] Texture Object output 0

### [13] Input UVs (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

### [14] Input Light Vector WorldSpace (Vector3)

- `Preview` <= [5] Multiply(,-1) output 0

### [23] Switch

- `A` <= [22] 1 output 0
- `B` <= [24] 0 output 0
- `Value` <= [34] Input Temporal Jitter (StaticBool) output 0

### [25] Multiply(,1)

- `A` <= [15] Input Max Steps (Scalar) output 0

### [30] Divide

- `A` <= [13] Input UVs (Vector2) output 0
- `B` <= [27] Input SubUV Frames (Vector2) output 0

### [31] Divide

- `A` <= [29] 1Dto2DIndex output 0
- `B` <= [27] Input SubUV Frames (Vector2) output 0

### [32] Add

- `A` <= [30] Divide output 0
- `B` <= [31] Divide output 0

### [33] Divide(1,)

- `B` <= [27] Input SubUV Frames (Vector2) output 0

### [34] Input Temporal Jitter (StaticBool)

- `Preview` <= [20] Static Bool (False) output 0

### [35] Frac

- `Input` <= [13] Input UVs (Vector2) output 0

### [36] Divide

- `A` <= [26] BoxIntersection output 4
- `B` <= [9] Divide(1,) output 0

### [37] Append

- `A` <= [35] Frac output 0
- `B` <= [44] Multiply(,0.499) output 0

### [38] Texture Sample

- `Coordinates` <= [32] Add output 0
- `TextureObject` <= [12] Input Texture Object (Texture2D) output 0

### [40] Floor

- `Input` <= [36] Divide output 0

### [41] Multiply(,1)

- `A` <= [9] Divide(1,) output 0

### [42] Output Step Complexity

- `A` <= [43] ShaderComplexity output 0

### [44] Multiply(,0.499)

- `A` <= [45] Add(,1) output 0

### [45] Add(,1)

- `A` <= [62] Multiply output 0

### [46] Multiply

- `A` <= [10] Normalize output 0
- `B` <= [16] Input Trace Distance (Scalar) output 0

### [49] Multiply

- `A` <= [41] Multiply(,1) output 0
- `B` <= [16] Input Trace Distance (Scalar) output 0

### [50] Dot

- `A` <= [51] Append output 0
- `B` <= [52] Input Heightmap Channel (Vector4) output 0

### [51] Append

- `A` <= [38] Texture Sample output 0
- `B` <= [38] Texture Sample output 4

### [53] Min

- `A` <= [15] Input Max Steps (Scalar) output 0
- `B` <= [40] Floor output 0

### [54] Max

- `A` <= [55] BreakOutFloat2Components output 0
- `B` <= [55] BreakOutFloat2Components output 1

### [56] Divide

- `A` <= [1] Multiply(,1) output 0
- `B` <= [58] MakeFloat3 output 0

### [57] Subtract

- `A` <= [50] Dot output 0
- `B` <= [17] Input Start Bias (Scalar) output 0

### [60] Output Raw Heightmap Value

- `A` <= [50] Dot output 0

### [61] Ceil

- `Input` <= [36] Divide output 0

### [62] Multiply

- `A` <= [50] Dot output 0
- `B` <= [63] 1-x output 0

### [63] 1-x

- `Input` <= [17] Input Start Bias (Scalar) output 0

### [64] Multiply(,1)

- `A` <= [46] Multiply output 0

### [65] Frac

- `Input` <= [32] Add output 0

## Connection List

- [66] BeersLaw output 0 -> Output Light Energy.A
- [8] Multiply output 0 -> Multiply(,1).A
- [6] Mask ( R G B ) output 0 -> Multiply(,-1).A
- [2] Param (0,1,0,0) 'LV' output 0 -> Mask ( R G B ).Input
- [10] Normalize output 0 -> World Space to Tangent Space TransformVector.Input
- [9] Divide(1,) output 0 -> Multiply.A
- [46] Multiply output 0 -> Multiply.B
- [15] Input Max Steps (Scalar) output 0 -> Divide(1,).B
- [14] Input Light Vector WorldSpace (Vector3) output 0 -> Normalize.VectorInput
- [21] Texture Object output 0 -> Input Texture Object (Texture2D).Preview
- [3] TexCoord[0] output 0 -> Input UVs (Vector2).Preview
- [5] Multiply(,-1) output 0 -> Input Light Vector WorldSpace (Vector3).Preview
- [22] 1 output 0 -> Switch.A
- [24] 0 output 0 -> Switch.B
- [34] Input Temporal Jitter (StaticBool) output 0 -> Switch.Value
- [15] Input Max Steps (Scalar) output 0 -> Multiply(,1).A
- [13] Input UVs (Vector2) output 0 -> Divide.A
- [27] Input SubUV Frames (Vector2) output 0 -> Divide.B
- [29] 1Dto2DIndex output 0 -> Divide.A
- [27] Input SubUV Frames (Vector2) output 0 -> Divide.B
- [30] Divide output 0 -> Add.A
- [31] Divide output 0 -> Add.B
- [27] Input SubUV Frames (Vector2) output 0 -> Divide(1,).B
- [20] Static Bool (False) output 0 -> Input Temporal Jitter (StaticBool).Preview
- [13] Input UVs (Vector2) output 0 -> Frac.Input
- [26] BoxIntersection output 4 -> Divide.A
- [9] Divide(1,) output 0 -> Divide.B
- [35] Frac output 0 -> Append.A
- [44] Multiply(,0.499) output 0 -> Append.B
- [32] Add output 0 -> Texture Sample.Coordinates
- [12] Input Texture Object (Texture2D) output 0 -> Texture Sample.TextureObject
- [36] Divide output 0 -> Floor.Input
- [9] Divide(1,) output 0 -> Multiply(,1).A
- [43] ShaderComplexity output 0 -> Output Step Complexity.A
- [45] Add(,1) output 0 -> Multiply(,0.499).A
- [62] Multiply output 0 -> Add(,1).A
- [10] Normalize output 0 -> Multiply.A
- [16] Input Trace Distance (Scalar) output 0 -> Multiply.B
- [41] Multiply(,1) output 0 -> Multiply.A
- [16] Input Trace Distance (Scalar) output 0 -> Multiply.B
- [51] Append output 0 -> Dot.A
- [52] Input Heightmap Channel (Vector4) output 0 -> Dot.B
- [38] Texture Sample output 0 -> Append.A
- [38] Texture Sample output 4 -> Append.B
- [15] Input Max Steps (Scalar) output 0 -> Min.A
- [40] Floor output 0 -> Min.B
- [55] BreakOutFloat2Components output 0 -> Max.A
- [55] BreakOutFloat2Components output 1 -> Max.B
- [1] Multiply(,1) output 0 -> Divide.A
- [58] MakeFloat3 output 0 -> Divide.B
- [50] Dot output 0 -> Subtract.A
- [17] Input Start Bias (Scalar) output 0 -> Subtract.B
- [50] Dot output 0 -> Output Raw Heightmap Value.A
- [36] Divide output 0 -> Ceil.Input
- [50] Dot output 0 -> Multiply.A
- [63] 1-x output 0 -> Multiply.B
- [17] Input Start Bias (Scalar) output 0 -> 1-x.Input
- [46] Multiply output 0 -> Multiply(,1).A
- [32] Add output 0 -> Frac.Input

## Output Trace Roots

- `Light Energy` is driven by [66] BeersLaw
- `Step Complexity` is driven by [43] ShaderComplexity
- `Raw Heightmap Value` is driven by [50] Dot

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


