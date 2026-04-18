# Sphere-ConeShadow-Texture

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Sphere-ConeShadow-Texture.Sphere-ConeShadow-Texture`
- Category: Engine_MaterialFunctions01/Shading
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the occlusion value for a pixel given a sphere at a specified position and of a specified radius. The occlusion values are read from a pre-calculated texture so this function is designed to run in realtime.

## Inputs

- `Light Vector` [FunctionInput_Vector3] -  SThe Vector where the directional light is coming from. Should be un-normalized if generated from point light (WorldPosition - LightPosition).
- `Sphere Position` [FunctionInput_Vector3] - The Position of the Sphere in the World.
- `Sphere Radius` [FunctionInput_Scalar] - The Radius of the Sphere in World Units.
- `Light Source Angle` [FunctionInput_Scalar] - This is used when using the static switch option to blend between channel packed light source textures. When using debug analytical mode, this value is a literal light source angle.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 2 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionDistance` | Distance |  |
| 5 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 6 | expression | `MaterialExpressionAppendVector` | Append |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionClamp` | Clamp |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Light Vector (Vector3) | Light Vector |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Position (Vector3) | Sphere Position |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Sphere Radius (Scalar) | Sphere Radius |
| 14 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 15 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 16 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 17 | custom | `MaterialExpressionCustom` | Custom |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 19 | expression | `MaterialExpressionSubtract` | Subtract(,2) |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 21 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 22 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 23 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 24 | expression | `MaterialExpressionMultiply` | Multiply(,3) |  |
| 25 | expression | `MaterialExpressionSubtract` | Subtract(,2) |  |
| 26 | expression | `MaterialExpressionSubtract` | Subtract(,1) |  |
| 27 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 28 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 29 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 30 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 31 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 32 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 33 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 34 | expression | `MaterialExpressionClamp` | Clamp |  |
| 35 | expression | `MaterialExpressionClamp` | Clamp |  |
| 36 | function_call | `MaterialExpressionMaterialFunctionCall` | Spherical-Cap-Intersection |  |
| 37 | custom | `MaterialExpressionCustom` | asin(x) |  |
| 38 | expression | `MaterialExpressionConstant` | 57.3 |  |
| 39 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 40 | custom | `MaterialExpressionCustom` | acos(x) |  |
| 41 | expression | `MaterialExpressionConstant` | 57.3 |  |
| 42 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 43 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 44 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 45 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Use Smoothstep for Area calculation' |  |
| 46 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Debug Using Analytical Calculation' |  |
| 47 | expression | `MaterialExpressionStaticComponentMaskParameter` | Mask Param 'Texture Color Channel' |  |
| 48 | expression | `MaterialExpressionAppendVector` | Append |  |
| 49 | expression | `MaterialExpressionStaticBoolParameter` | Static Bool Param (False) 'Blend Channel Packed Light Source Angles' |  |
| 50 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 51 | expression | `MaterialExpressionTextureObjectParameter` | Param Tex Object 'Shadow' |  |
| 52 | expression | `MaterialExpressionClamp` | Clamp |  |
| 53 | function_input | `MaterialExpressionFunctionInput` | Input Light Source Angle (Scalar) | Light Source Angle |
| 54 | expression | `MaterialExpressionAppendVector` | Append |  |
| 55 | expression | `MaterialExpressionAppendVector` | Append |  |
| 56 | expression | `MaterialExpressionClamp` | Clamp |  |
| 57 | expression | `MaterialExpressionIf` | If |  |
| 58 | expression | `MaterialExpressionConstant` | 0 |  |
| 59 | expression | `MaterialExpressionDivide` | Divide |  |
| 60 | expression | `MaterialExpressionClamp` | Clamp |  |
| 61 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 62 | expression | `MaterialExpressionScalarParameter` | Param (0) 'Min Texture Lightsource Angle' |  |
| 63 | expression | `MaterialExpressionScalarParameter` | Param (0) 'MaxTexture Lightsource Angle' |  |
| 64 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 65 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 66 | expression | `MaterialExpressionConstant` | 0 |  |
| 67 | custom | `MaterialExpressionCustom` | acos(x) |  |
| 68 | custom | `MaterialExpressionCustom` | asin(x) |  |
| 69 | expression | `MaterialExpressionDivide` | Divide(,1.57) |  |
| 70 | expression | `MaterialExpressionDivide` | Divide(,3.14) |  |
| 71 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 72 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [43] Switch output 0

### [1] Texture Sample

- `Coordinates` <= [8] Clamp output 0
- `TextureObject` <= [51] Param Tex Object 'Shadow' output 0

### [3] Subtract

- `A` <= [12] Input Sphere Position (Vector3) output 0
- `B` <= [2] Absolute World Position output 0

### [4] Distance

- `A` <= [3] Subtract output 0
- `B` <= [66] 0 output 0

### [5] Normalize

- `VectorInput` <= [3] Subtract output 0

### [6] Append

- `A` <= [44] ConstantBiasScale output 0
- `B` <= [9] Divide output 0

### [7] Dot

- `A` <= [65] Normalize output 0
- `B` <= [10] Multiply(,-1) output 0

### [8] Clamp

- `Input` <= [6] Append output 0

### [9] Divide

- `A` <= [13] Input Sphere Radius (Scalar) output 0
- `B` <= [4] Distance output 0

### [10] Multiply(,-1)

- `A` <= [5] Normalize output 0

### [14] Lerp

- `A` <= [1] Texture Sample output 1
- `B` <= [1] Texture Sample output 2
- `Alpha` <= [18] Multiply(,3) output 0

### [15] Lerp

- `A` <= [14] Lerp output 0
- `B` <= [1] Texture Sample output 3
- `Alpha` <= [35] Clamp output 0

### [16] Lerp

- `A` <= [15] Lerp output 0
- `B` <= [1] Texture Sample output 4
- `Alpha` <= [52] Clamp output 0

### [18] Multiply(,3)

- `A` <= [60] Clamp output 0

### [19] Subtract(,2)

- `A` <= [18] Multiply(,3) output 0

### [20] Subtract(,1)

- `A` <= [18] Multiply(,3) output 0

### [21] Lerp

- `A` <= [27] 1,0,0 output 0
- `B` <= [28] 0,1,0 output 0
- `Alpha` <= [24] Multiply(,3) output 0

### [22] Lerp

- `A` <= [21] Lerp output 0
- `B` <= [29] 0,0,1 output 0
- `Alpha` <= [34] Clamp output 0

### [23] Lerp

- `A` <= [22] Lerp output 0
- `B` <= [30] 0,0,0 output 0
- `Alpha` <= [33] Max(,0) output 0

### [24] Multiply(,3)

- `A` <= [32] Mask ( G ) output 0

### [25] Subtract(,2)

- `A` <= [24] Multiply(,3) output 0

### [26] Subtract(,1)

- `A` <= [24] Multiply(,3) output 0

### [32] Mask ( G )

- `Input` <= [31] TexCoord[0] output 0

### [33] Max(,0)

- `A` <= [25] Subtract(,2) output 0

### [34] Clamp

- `Input` <= [26] Subtract(,1) output 0

### [35] Clamp

- `Input` <= [20] Subtract(,1) output 0

### [39] Multiply

- `A` <= [37] asin(x) output 0
- `B` <= [38] 57.3 output 0

### [42] Multiply

- `A` <= [40] acos(x) output 0
- `B` <= [41] 57.3 output 0

### [43] Switch

- `A` <= [57] If output 0
- `B` <= [50] Switch output 0
- `Value` <= [46] Static Bool Param (False) 'Debug Using Analytical Calculation' output 0

### [44] ConstantBiasScale

- `Input` <= [7] Dot output 0

### [47] Mask Param 'Texture Color Channel'

- `Input` <= [55] Append output 0

### [48] Append

- `A` <= [1] Texture Sample output 1
- `B` <= [1] Texture Sample output 2

### [50] Switch

- `A` <= [16] Lerp output 0
- `B` <= [47] Mask Param 'Texture Color Channel' output 0
- `Value` <= [49] Static Bool Param (False) 'Blend Channel Packed Light Source Angles' output 0

### [52] Clamp

- `Input` <= [19] Subtract(,2) output 0

### [54] Append

- `A` <= [48] Append output 0
- `B` <= [1] Texture Sample output 3

### [55] Append

- `A` <= [54] Append output 0
- `B` <= [1] Texture Sample output 4

### [56] Clamp

- `Input` <= [39] Multiply output 0

### [57] If

- `A` <= [13] Input Sphere Radius (Scalar) output 0
- `B` <= [4] Distance output 0
- `AGreaterThanB` <= [58] 0 output 0
- `AEqualsB` <= [58] 0 output 0
- `ALessThanB` <= [36] Spherical-Cap-Intersection output 0

### [59] Divide

- `A` <= [61] Subtract output 0
- `B` <= [64] Subtract output 0

### [60] Clamp

- `Input` <= [59] Divide output 0

### [61] Subtract

- `A` <= [53] Input Light Source Angle (Scalar) output 0
- `B` <= [62] Param (0) 'Min Texture Lightsource Angle' output 0

### [64] Subtract

- `A` <= [63] Param (0) 'MaxTexture Lightsource Angle' output 0
- `B` <= [62] Param (0) 'Min Texture Lightsource Angle' output 0

### [65] Normalize

- `VectorInput` <= [11] Input Light Vector (Vector3) output 0

### [69] Divide(,1.57)

- `A` <= [68] asin(x) output 0

### [70] Divide(,3.14)

- `A` <= [67] acos(x) output 0

### [71] 1-x

- `Input` <= [70] Divide(,3.14) output 0

### [72] Append

- `A` <= [71] 1-x output 0
- `B` <= [69] Divide(,1.57) output 0

## Connection List

- [43] Switch output 0 -> Output Result.A
- [8] Clamp output 0 -> Texture Sample.Coordinates
- [51] Param Tex Object 'Shadow' output 0 -> Texture Sample.TextureObject
- [12] Input Sphere Position (Vector3) output 0 -> Subtract.A
- [2] Absolute World Position output 0 -> Subtract.B
- [3] Subtract output 0 -> Distance.A
- [66] 0 output 0 -> Distance.B
- [3] Subtract output 0 -> Normalize.VectorInput
- [44] ConstantBiasScale output 0 -> Append.A
- [9] Divide output 0 -> Append.B
- [65] Normalize output 0 -> Dot.A
- [10] Multiply(,-1) output 0 -> Dot.B
- [6] Append output 0 -> Clamp.Input
- [13] Input Sphere Radius (Scalar) output 0 -> Divide.A
- [4] Distance output 0 -> Divide.B
- [5] Normalize output 0 -> Multiply(,-1).A
- [1] Texture Sample output 1 -> Lerp.A
- [1] Texture Sample output 2 -> Lerp.B
- [18] Multiply(,3) output 0 -> Lerp.Alpha
- [14] Lerp output 0 -> Lerp.A
- [1] Texture Sample output 3 -> Lerp.B
- [35] Clamp output 0 -> Lerp.Alpha
- [15] Lerp output 0 -> Lerp.A
- [1] Texture Sample output 4 -> Lerp.B
- [52] Clamp output 0 -> Lerp.Alpha
- [60] Clamp output 0 -> Multiply(,3).A
- [18] Multiply(,3) output 0 -> Subtract(,2).A
- [18] Multiply(,3) output 0 -> Subtract(,1).A
- [27] 1,0,0 output 0 -> Lerp.A
- [28] 0,1,0 output 0 -> Lerp.B
- [24] Multiply(,3) output 0 -> Lerp.Alpha
- [21] Lerp output 0 -> Lerp.A
- [29] 0,0,1 output 0 -> Lerp.B
- [34] Clamp output 0 -> Lerp.Alpha
- [22] Lerp output 0 -> Lerp.A
- [30] 0,0,0 output 0 -> Lerp.B
- [33] Max(,0) output 0 -> Lerp.Alpha
- [32] Mask ( G ) output 0 -> Multiply(,3).A
- [24] Multiply(,3) output 0 -> Subtract(,2).A
- [24] Multiply(,3) output 0 -> Subtract(,1).A
- [31] TexCoord[0] output 0 -> Mask ( G ).Input
- [25] Subtract(,2) output 0 -> Max(,0).A
- [26] Subtract(,1) output 0 -> Clamp.Input
- [20] Subtract(,1) output 0 -> Clamp.Input
- [37] asin(x) output 0 -> Multiply.A
- [38] 57.3 output 0 -> Multiply.B
- [40] acos(x) output 0 -> Multiply.A
- [41] 57.3 output 0 -> Multiply.B
- [57] If output 0 -> Switch.A
- [50] Switch output 0 -> Switch.B
- [46] Static Bool Param (False) 'Debug Using Analytical Calculation' output 0 -> Switch.Value
- [7] Dot output 0 -> ConstantBiasScale.Input
- [55] Append output 0 -> Mask Param 'Texture Color Channel'.Input
- [1] Texture Sample output 1 -> Append.A
- [1] Texture Sample output 2 -> Append.B
- [16] Lerp output 0 -> Switch.A
- [47] Mask Param 'Texture Color Channel' output 0 -> Switch.B
- [49] Static Bool Param (False) 'Blend Channel Packed Light Source Angles' output 0 -> Switch.Value
- [19] Subtract(,2) output 0 -> Clamp.Input
- [48] Append output 0 -> Append.A
- [1] Texture Sample output 3 -> Append.B
- [54] Append output 0 -> Append.A
- [1] Texture Sample output 4 -> Append.B
- [39] Multiply output 0 -> Clamp.Input
- [13] Input Sphere Radius (Scalar) output 0 -> If.A
- [4] Distance output 0 -> If.B
- [58] 0 output 0 -> If.AGreaterThanB
- [58] 0 output 0 -> If.AEqualsB
- [36] Spherical-Cap-Intersection output 0 -> If.ALessThanB
- [61] Subtract output 0 -> Divide.A
- [64] Subtract output 0 -> Divide.B
- [59] Divide output 0 -> Clamp.Input
- [53] Input Light Source Angle (Scalar) output 0 -> Subtract.A
- [62] Param (0) 'Min Texture Lightsource Angle' output 0 -> Subtract.B
- [63] Param (0) 'MaxTexture Lightsource Angle' output 0 -> Subtract.A
- [62] Param (0) 'Min Texture Lightsource Angle' output 0 -> Subtract.B
- [11] Input Light Vector (Vector3) output 0 -> Normalize.VectorInput
- [68] asin(x) output 0 -> Divide(,1.57).A
- [67] acos(x) output 0 -> Divide(,3.14).A
- [70] Divide(,3.14) output 0 -> 1-x.Input
- [71] 1-x output 0 -> Append.A
- [69] Divide(,1.57) output 0 -> Append.B

## Output Trace Roots

- `Result` is driven by [43] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


