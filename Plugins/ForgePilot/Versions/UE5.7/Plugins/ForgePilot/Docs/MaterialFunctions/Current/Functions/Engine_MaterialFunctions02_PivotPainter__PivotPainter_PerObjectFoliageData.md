# PivotPainter_PerObjectFoliageData

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PivotPainter/PivotPainter_PerObjectFoliageData.PivotPainter_PerObjectFoliageData`
- Category: Engine_MaterialFunctions02/PivotPainter
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Processes and organizes world position and angle information stored in the model's uvs by the Pivot Painter MAXScript. 

## Inputs

- `Optimized for Foliage Placement` [FunctionInput_StaticBool] - Default value = false. Check if you processed the mesh using Pivot Painter with the optimize for Foliage Placement option checked.
- `Optimized Vector` [FunctionInput_Vector3] - Enter a local vector to be used as the elements rotation axis. If the optimized for foliage placement boolean is true. 
- `Wind Vector` [FunctionInput_Vector4] - Needs to be replaced with wind vector.

## Outputs

- `Piv Position` - Returns the pivot point information for each element in world space.
- `Custom Alpha Values` - Returns the custom falloff values stored in the vertex alpha channel of the model. 
- `Random Value Per Element` - Returns a random value per element in the 0-1 range.
- `Element X-Axis` - Returns a normalized vector pointing down the elements X-Axis. Which points from the pivot point toward the averaged center of the mesh.
- `Element Rot Axis` - Returns a rotation axis for branches to be used with the "RotateAboutAxis" node. Leaves return the same information. Note: The angle is found by transforming a vector along the branch's x-axis from local to world space. Then the cross product is found between the wind direction and the transformed vector.
- `Normalized Wind Vector`
- `Wind Accumulator` - Wind speed multiplied by time multiplied by -1.
- `Wind Strength` - Returns wind strength. The magnitude of the wind vector found by calculating distance from the wind vector to 0. 
- `Uniform Object Scale` - Returns a scalar value that is equal to the objects uniform scale.
- `Object Scale XYZ` - Returns a scalar value that is equal to the objects uniform scale.
- `Wind Speed`
- `Object PIvot Location` - Returns a scalar value that is equal to the objects uniform scale.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Piv Position | Piv Position |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Custom Alpha Values | Custom Alpha Values |
| 7 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[3] |  |
| 10 | function_output | `MaterialExpressionFunctionOutput` | Output Random Value Per Element | Random Value Per Element |
| 11 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 14 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Element X-Axis | Element X-Axis |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 18 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 19 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Element Rot Axis | Element Rot Axis |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | Wind |  |
| 22 | function_output | `MaterialExpressionFunctionOutput` | Output Normalized Wind Vector | Normalized Wind Vector |
| 23 | function_output | `MaterialExpressionFunctionOutput` | Output Wind Accumulator | Wind Accumulator |
| 24 | function_output | `MaterialExpressionFunctionOutput` | Output Wind Strength | Wind Strength |
| 25 | function_output | `MaterialExpressionFunctionOutput` | Output Uniform Object Scale | Uniform Object Scale |
| 26 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 27 | function_output | `MaterialExpressionFunctionOutput` | Output Object Scale XYZ | Object Scale XYZ |
| 28 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input Optimized for Foliage Placement (StaticBool) | Optimized for Foliage Placement |
| 30 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Optimized Vector (Vector3) | Optimized Vector |
| 32 | function_call | `MaterialExpressionMaterialFunctionCall` | ObjectLocalToWorldData |  |
| 33 | function_input | `MaterialExpressionFunctionInput` | Input Wind Vector (Vector4) | Wind Vector |
| 34 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 35 | function_output | `MaterialExpressionFunctionOutput` | Output Wind Speed | Wind Speed |
| 36 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 37 | function_output | `MaterialExpressionFunctionOutput` | Output Object PIvot Location | Object PIvot Location |

## Exact Input Wiring

### [1] Append

- `A` <= [0] TexCoord[2] output 0
- `B` <= [4] Mask ( R ) output 0

### [2] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [1] Append output 0

### [4] Mask ( R )

- `Input` <= [3] TexCoord[3] output 0

### [5] Output Piv Position

- `A` <= [2] Instance & Particle Space to Absolute World Space TransformPosition output 0

### [6] Output Custom Alpha Values

- `A` <= [26] Mask ( R ) output 0

### [8] Mask ( G )

- `Input` <= [9] TexCoord[3] output 0

### [10] Output Random Value Per Element

- `A` <= [8] Mask ( G ) output 0

### [11] Instance & Particle Space to World Space TransformVector

- `Input` <= [12] Normalize output 0

### [12] Normalize

- `VectorInput` <= [28] Switch output 0

### [13] ConstantBiasScale

- `Input` <= [14] Vertex Color output 0

### [15] Output Element X-Axis

- `A` <= [16] Normalize output 0

### [16] Normalize

- `VectorInput` <= [11] Instance & Particle Space to World Space TransformVector output 0

### [17] Normalize

- `VectorInput` <= [34] Mask ( R G B ) output 0

### [18] Normalize

- `VectorInput` <= [19] Cross output 0

### [19] Cross

- `A` <= [17] Normalize output 0
- `B` <= [16] Normalize output 0

### [20] Output Element Rot Axis

- `A` <= [18] Normalize output 0

### [22] Output Normalized Wind Vector

- `A` <= [21] Wind output 0

### [23] Output Wind Accumulator

- `A` <= [21] Wind output 2

### [24] Output Wind Strength

- `A` <= [21] Wind output 1

### [25] Output Uniform Object Scale

- `A` <= [32] ObjectLocalToWorldData output 2

### [26] Mask ( R )

- `Input` <= [7] Vertex Color output 4

### [27] Output Object Scale XYZ

- `A` <= [32] ObjectLocalToWorldData output 1

### [28] Switch

- `A` <= [31] Input Optimized Vector (Vector3) output 0
- `B` <= [13] ConstantBiasScale output 0
- `Value` <= [29] Input Optimized for Foliage Placement (StaticBool) output 0

### [29] Input Optimized for Foliage Placement (StaticBool)

- `Preview` <= [30] Static Bool (False) output 0

### [34] Mask ( R G B )

- `Input` <= [33] Input Wind Vector (Vector4) output 0

### [35] Output Wind Speed

- `A` <= [36] Mask ( A ) output 0

### [36] Mask ( A )

- `Input` <= [33] Input Wind Vector (Vector4) output 0

### [37] Output Object PIvot Location

- `A` <= [32] ObjectLocalToWorldData output 0

## Connection List

- [0] TexCoord[2] output 0 -> Append.A
- [4] Mask ( R ) output 0 -> Append.B
- [1] Append output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [3] TexCoord[3] output 0 -> Mask ( R ).Input
- [2] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Output Piv Position.A
- [26] Mask ( R ) output 0 -> Output Custom Alpha Values.A
- [9] TexCoord[3] output 0 -> Mask ( G ).Input
- [8] Mask ( G ) output 0 -> Output Random Value Per Element.A
- [12] Normalize output 0 -> Instance & Particle Space to World Space TransformVector.Input
- [28] Switch output 0 -> Normalize.VectorInput
- [14] Vertex Color output 0 -> ConstantBiasScale.Input
- [16] Normalize output 0 -> Output Element X-Axis.A
- [11] Instance & Particle Space to World Space TransformVector output 0 -> Normalize.VectorInput
- [34] Mask ( R G B ) output 0 -> Normalize.VectorInput
- [19] Cross output 0 -> Normalize.VectorInput
- [17] Normalize output 0 -> Cross.A
- [16] Normalize output 0 -> Cross.B
- [18] Normalize output 0 -> Output Element Rot Axis.A
- [21] Wind output 0 -> Output Normalized Wind Vector.A
- [21] Wind output 2 -> Output Wind Accumulator.A
- [21] Wind output 1 -> Output Wind Strength.A
- [32] ObjectLocalToWorldData output 2 -> Output Uniform Object Scale.A
- [7] Vertex Color output 4 -> Mask ( R ).Input
- [32] ObjectLocalToWorldData output 1 -> Output Object Scale XYZ.A
- [31] Input Optimized Vector (Vector3) output 0 -> Switch.A
- [13] ConstantBiasScale output 0 -> Switch.B
- [29] Input Optimized for Foliage Placement (StaticBool) output 0 -> Switch.Value
- [30] Static Bool (False) output 0 -> Input Optimized for Foliage Placement (StaticBool).Preview
- [33] Input Wind Vector (Vector4) output 0 -> Mask ( R G B ).Input
- [36] Mask ( A ) output 0 -> Output Wind Speed.A
- [33] Input Wind Vector (Vector4) output 0 -> Mask ( A ).Input
- [32] ObjectLocalToWorldData output 0 -> Output Object PIvot Location.A

## Output Trace Roots

- `Piv Position` is driven by [2] Instance & Particle Space to Absolute World Space TransformPosition
- `Custom Alpha Values` is driven by [26] Mask ( R )
- `Random Value Per Element` is driven by [8] Mask ( G )
- `Element X-Axis` is driven by [16] Normalize
- `Element Rot Axis` is driven by [18] Normalize
- `Normalized Wind Vector` is driven by [21] Wind
- `Wind Accumulator` is driven by [21] Wind
- `Wind Strength` is driven by [21] Wind
- `Uniform Object Scale` is driven by [32] ObjectLocalToWorldData
- `Object Scale XYZ` is driven by [32] ObjectLocalToWorldData
- `Wind Speed` is driven by [36] Mask ( A )
- `Object PIvot Location` is driven by [32] ObjectLocalToWorldData

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


