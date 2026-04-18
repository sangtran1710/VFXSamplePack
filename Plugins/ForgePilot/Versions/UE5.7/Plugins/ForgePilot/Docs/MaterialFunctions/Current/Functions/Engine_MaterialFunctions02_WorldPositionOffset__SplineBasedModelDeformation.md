# SplineBasedModelDeformation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/WorldPositionOffset/SplineBasedModelDeformation.SplineBasedModelDeformation`
- Category: Engine_MaterialFunctions02/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Deform a model around a spline that is created with the four control point inputs. P0 through P3. P0 and P3 are tangent handles.

## Inputs

- `Model height along deformation vector` [FunctionInput_Scalar] - This is the models local space height along the spline tangent. If a mesh is being deformed along it's z axis and the distance from the pivot point of the model to the top of the mesh, in local space, is 1 then you would enter 1 in this field. If the model's height were 10 then 10 should be entered.
- `P0` [FunctionInput_Vector3] - Control point. 3D world space position. 
- `P1` [FunctionInput_Vector3] - Control point. 3D world space position. 
- `P2` [FunctionInput_Vector3] - Control point. 3D world space position. 
- `P3` [FunctionInput_Vector3] - Control point. 3D world space position. 
- `LocalDeformationVector` [FunctionInput_Vector3] - Local axis that the model will use to map to the length of the spline.
- `Thickness` [FunctionInput_Vector2] - Automatically retains the model's intial scale. If desired enter a new value to scale the spline deformed models x and y local axes positions.
- `Local Position` [FunctionInput_Vector3] - Automatically supplied.
- `Optional Tangent Space Normal Map` [FunctionInput_Vector3]
- `Optional Greyscale Height Map` [FunctionInput_Scalar] - The intensity will determine amount of offset.

## Outputs

- `World Position Offset`
- `Debug Gradient` - This returns the spline length values being used to drive the spline deformation (t). The values should stat at 0 (at the pivot location) and gradually become 1 near the furthest extent of the object from the pivot point in the direction of the deformation vector.
- `BiNormal (X)` - local x axis
- `Normal (Y)` - local Y axis
- `Tangent (Z)` - local z
- `World T Position`
- `Pixel normal (WS Cylindrical)` - In world and does not include transform the original normals
- `Local Position Offset`
- `Pixel normal (WS Texture)` - In world and does not include transform the original normals

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position Offset | World Position Offset |
| 1 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 3 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | function_call | `MaterialExpressionMaterialFunctionCall` | GenerateASpline |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Model height along deformation vector (Scalar) | Model height along deformation vector |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input P0 (Vector3) | P0 |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input P1 (Vector3) | P1 |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input P2 (Vector3) | P2 |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input P3 (Vector3) | P3 |
| 15 | expression | `MaterialExpressionTransformPosition` | Instance & Particle Space to Absolute World Space TransformPosition |  |
| 16 | expression | `MaterialExpressionClamp` | Clamp |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Debug Gradient | Debug Gradient |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input LocalDeformationVector (Vector3) | LocalDeformationVector |
| 19 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 20 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 21 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 22 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 23 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 24 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 25 | expression | `MaterialExpressionTransformPosition` | Absolute World Space to Instance & Particle Space TransformPosition |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input Thickness (Vector2) | Thickness |
| 27 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 28 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output BiNormal (X) | BiNormal (X) |
| 30 | function_output | `MaterialExpressionFunctionOutput` | Output Normal (Y) | Normal (Y) |
| 31 | function_output | `MaterialExpressionFunctionOutput` | Output Tangent (Z) | Tangent (Z) |
| 32 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 33 | function_input | `MaterialExpressionFunctionInput` | Input Local Position (Vector3) | Local Position |
| 34 | function_output | `MaterialExpressionFunctionOutput` | Output World T Position | World T Position |
| 35 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel normal (WS Cylindrical) | Pixel normal (WS Cylindrical) |
| 36 | function_output | `MaterialExpressionFunctionOutput` | Output Local Position Offset | Local Position Offset |
| 37 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 38 | expression | `MaterialExpressionTransform` | Instance & Particle Space to World Space TransformVector |  |
| 39 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 40 | function_input | `MaterialExpressionFunctionInput` | Input Optional Tangent Space Normal Map (Vector3) | Optional Tangent Space Normal Map |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output Pixel normal (WS Texture) | Pixel normal (WS Texture) |
| 42 | function_input | `MaterialExpressionFunctionInput` | Input Optional Greyscale Height Map (Scalar) | Optional Greyscale Height Map |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | expression | `MaterialExpressionAdd` | Add |  |
| 45 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 46 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 47 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 48 | expression | `MaterialExpressionAdd` | Add |  |
| 49 | function_call | `MaterialExpressionMaterialFunctionCall` | SafeNormalize |  |

## Exact Input Wiring

### [0] Output World Position Offset

- `A` <= [44] Add output 0

### [1] Dot

- `A` <= [32] Normalize output 0
- `B` <= [33] Input Local Position (Vector3) output 0

### [2] Multiply

- `A` <= [32] Normalize output 0
- `B` <= [1] Dot output 0

### [3] Subtract

- `A` <= [33] Input Local Position (Vector3) output 0
- `B` <= [2] Multiply output 0

### [4] Multiply

- `A` <= [8] GenerateASpline output 1
- `B` <= [21] Dot output 0

### [5] Multiply

- `A` <= [23] Dot output 0
- `B` <= [8] GenerateASpline output 2

### [6] Add

- `A` <= [4] Multiply output 0
- `B` <= [5] Multiply output 0

### [7] Add

- `A` <= [48] Add output 0
- `B` <= [25] Absolute World Space to Instance & Particle Space TransformPosition output 0

### [9] Divide

- `A` <= [1] Dot output 0
- `B` <= [10] Input Model height along deformation vector (Scalar) output 0

### [15] Instance & Particle Space to Absolute World Space TransformPosition

- `Input` <= [28] Subtract output 0

### [16] Clamp

- `Input` <= [9] Divide output 0

### [17] Output Debug Gradient

- `A` <= [16] Clamp output 0

### [19] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [20] Absolute World Position (Excluding Material Offsets) output 0

### [21] Dot

- `A` <= [3] Subtract output 0
- `B` <= [22] 1,0,0 output 0

### [23] Dot

- `A` <= [3] Subtract output 0
- `B` <= [24] 0,1,0 output 0

### [25] Absolute World Space to Instance & Particle Space TransformPosition

- `Input` <= [8] GenerateASpline output 4

### [26] Input Thickness (Vector2)

- `Preview` <= [6] Add output 0

### [27] Multiply

- `A` <= [1] Dot output 0
- `B` <= [32] Normalize output 0

### [28] Subtract

- `A` <= [7] Add output 0
- `B` <= [27] Multiply output 0

### [29] Output BiNormal (X)

- `A` <= [8] GenerateASpline output 1

### [30] Output Normal (Y)

- `A` <= [8] GenerateASpline output 2

### [31] Output Tangent (Z)

- `A` <= [8] GenerateASpline output 3

### [32] Normalize

- `VectorInput` <= [18] Input LocalDeformationVector (Vector3) output 0

### [33] Input Local Position (Vector3)

- `Preview` <= [19] Absolute World Space to Instance & Particle Space TransformPosition output 0

### [34] Output World T Position

- `A` <= [8] GenerateASpline output 4

### [35] Output Pixel normal (WS Cylindrical)

- `A` <= [49] SafeNormalize output 0

### [36] Output Local Position Offset

- `A` <= [7] Add output 0

### [38] Instance & Particle Space to World Space TransformVector

- `Input` <= [8] GenerateASpline output 3

### [41] Output Pixel normal (WS Texture)

- `A` <= [37] Transform3x3Matrix output 0

### [43] Multiply

- `A` <= [49] SafeNormalize output 0
- `B` <= [42] Input Optional Greyscale Height Map (Scalar) output 0

### [44] Add

- `A` <= [15] Instance & Particle Space to Absolute World Space TransformPosition output 0
- `B` <= [43] Multiply output 0

### [46] Multiply

- `A` <= [45] BreakOutFloat2Components output 0
- `B` <= [4] Multiply output 0

### [47] Multiply

- `A` <= [45] BreakOutFloat2Components output 1
- `B` <= [5] Multiply output 0

### [48] Add

- `A` <= [46] Multiply output 0
- `B` <= [47] Multiply output 0

## Connection List

- [44] Add output 0 -> Output World Position Offset.A
- [32] Normalize output 0 -> Dot.A
- [33] Input Local Position (Vector3) output 0 -> Dot.B
- [32] Normalize output 0 -> Multiply.A
- [1] Dot output 0 -> Multiply.B
- [33] Input Local Position (Vector3) output 0 -> Subtract.A
- [2] Multiply output 0 -> Subtract.B
- [8] GenerateASpline output 1 -> Multiply.A
- [21] Dot output 0 -> Multiply.B
- [23] Dot output 0 -> Multiply.A
- [8] GenerateASpline output 2 -> Multiply.B
- [4] Multiply output 0 -> Add.A
- [5] Multiply output 0 -> Add.B
- [48] Add output 0 -> Add.A
- [25] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Add.B
- [1] Dot output 0 -> Divide.A
- [10] Input Model height along deformation vector (Scalar) output 0 -> Divide.B
- [28] Subtract output 0 -> Instance & Particle Space to Absolute World Space TransformPosition.Input
- [9] Divide output 0 -> Clamp.Input
- [16] Clamp output 0 -> Output Debug Gradient.A
- [20] Absolute World Position (Excluding Material Offsets) output 0 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [3] Subtract output 0 -> Dot.A
- [22] 1,0,0 output 0 -> Dot.B
- [3] Subtract output 0 -> Dot.A
- [24] 0,1,0 output 0 -> Dot.B
- [8] GenerateASpline output 4 -> Absolute World Space to Instance & Particle Space TransformPosition.Input
- [6] Add output 0 -> Input Thickness (Vector2).Preview
- [1] Dot output 0 -> Multiply.A
- [32] Normalize output 0 -> Multiply.B
- [7] Add output 0 -> Subtract.A
- [27] Multiply output 0 -> Subtract.B
- [8] GenerateASpline output 1 -> Output BiNormal (X).A
- [8] GenerateASpline output 2 -> Output Normal (Y).A
- [8] GenerateASpline output 3 -> Output Tangent (Z).A
- [18] Input LocalDeformationVector (Vector3) output 0 -> Normalize.VectorInput
- [19] Absolute World Space to Instance & Particle Space TransformPosition output 0 -> Input Local Position (Vector3).Preview
- [8] GenerateASpline output 4 -> Output World T Position.A
- [49] SafeNormalize output 0 -> Output Pixel normal (WS Cylindrical).A
- [7] Add output 0 -> Output Local Position Offset.A
- [8] GenerateASpline output 3 -> Instance & Particle Space to World Space TransformVector.Input
- [37] Transform3x3Matrix output 0 -> Output Pixel normal (WS Texture).A
- [49] SafeNormalize output 0 -> Multiply.A
- [42] Input Optional Greyscale Height Map (Scalar) output 0 -> Multiply.B
- [15] Instance & Particle Space to Absolute World Space TransformPosition output 0 -> Add.A
- [43] Multiply output 0 -> Add.B
- [45] BreakOutFloat2Components output 0 -> Multiply.A
- [4] Multiply output 0 -> Multiply.B
- [45] BreakOutFloat2Components output 1 -> Multiply.A
- [5] Multiply output 0 -> Multiply.B
- [46] Multiply output 0 -> Add.A
- [47] Multiply output 0 -> Add.B

## Output Trace Roots

- `World Position Offset` is driven by [44] Add
- `Debug Gradient` is driven by [16] Clamp
- `BiNormal (X)` is driven by [8] GenerateASpline
- `Normal (Y)` is driven by [8] GenerateASpline
- `Tangent (Z)` is driven by [8] GenerateASpline
- `World T Position` is driven by [8] GenerateASpline
- `Pixel normal (WS Cylindrical)` is driven by [49] SafeNormalize
- `Local Position Offset` is driven by [7] Add
- `Pixel normal (WS Texture)` is driven by [37] Transform3x3Matrix

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


