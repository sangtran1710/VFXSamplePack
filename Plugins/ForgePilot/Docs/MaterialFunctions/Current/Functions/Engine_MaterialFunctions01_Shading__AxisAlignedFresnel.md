# AxisAlignedFresnel

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/AxisAlignedFresnel.AxisAlignedFresnel`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Gives a fresnel type falloff that is only perpendicular to the specified axis. Useful for fading the edges of a cylinder even when looking straight down along the axis of the cylinder.

## Inputs

- `Axis` [FunctionInput_Vector3] - The axis to align to. By default this will be the local Z vector of the mesh which means it works with the vertically oriented Engine\Content cylinder meshes.
- `Normal` [FunctionInput_Vector3] - Defaults to Vertex Normal but this allows you to specify a custom normal. Normal should be in world space.
- `Exponent` [FunctionInput_Scalar] - The exponent will be applied as a Power to the result.

## Outputs

- `Fresnel` - The resulting fresnel with Power Exponent applied.
- `Cylinder Thickness` - This returns the value for how far a ray would travel through a cylinder, where a value of 1 equals 1 diameter of the cylinder. It does not handle end caps, only the sides of the cylinder.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Fresnel | Fresnel |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Axis (Vector3) | Axis |
| 2 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 3 | expression | `MaterialExpressionTransform` | Local Space to World Space TransformVector |  |
| 4 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 7 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 11 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionConstant3Vector` | -1,-1,-1 |  |
| 14 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 15 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 19 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 20 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 21 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 22 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Exponent (Scalar) | Exponent |
| 24 | expression | `MaterialExpressionPower` | Power |  |
| 25 | function_output | `MaterialExpressionFunctionOutput` | Output Cylinder Thickness | Cylinder Thickness |
| 26 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 27 | expression | `MaterialExpressionTwoSidedSign` | TwoSidedSign |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Fresnel

- `A` <= [24] Power output 0

### [1] Input Axis (Vector3)

- `Preview` <= [3] Local Space to World Space TransformVector output 0

### [3] Local Space to World Space TransformVector

- `Input` <= [2] 0,0,1 output 0

### [5] Subtract

- `A` <= [4] Absolute World Position output 0
- `B` <= [6] Camera Position output 0

### [7] Normalize

- `VectorInput` <= [5] Subtract output 0

### [8] Divide

- `A` <= [9] Dot output 0
- `B` <= [10] Dot output 0

### [9] Dot

- `A` <= [15] Normalize output 0
- `B` <= [18] Input Normal (Vector3) output 0

### [10] Dot

- `A` <= [15] Normalize output 0
- `B` <= [19] Normalize output 0

### [12] Multiply

- `A` <= [14] Cross output 0
- `B` <= [13] -1,-1,-1 output 0

### [14] Cross

- `A` <= [16] Normalize output 0
- `B` <= [1] Input Axis (Vector3) output 0

### [15] Normalize

- `VectorInput` <= [14] Cross output 0

### [16] Normalize

- `VectorInput` <= [17] Cross output 0

### [17] Cross

- `A` <= [7] Normalize output 0
- `B` <= [1] Input Axis (Vector3) output 0

### [18] Input Normal (Vector3)

- `Preview` <= [11] VertexNormalWS output 0

### [19] Normalize

- `VectorInput` <= [21] Subtract output 0

### [21] Subtract

- `A` <= [22] Absolute World Position output 0
- `B` <= [20] Camera Position output 0

### [24] Power

- `Base` <= [26] Max(,0) output 0
- `Exponent` <= [23] Input Exponent (Scalar) output 0

### [25] Output Cylinder Thickness

- `A` <= [8] Divide output 0

### [26] Max(,0)

- `A` <= [28] Multiply output 0

### [28] Multiply

- `A` <= [27] TwoSidedSign output 0
- `B` <= [9] Dot output 0

## Connection List

- [24] Power output 0 -> Output Fresnel.A
- [3] Local Space to World Space TransformVector output 0 -> Input Axis (Vector3).Preview
- [2] 0,0,1 output 0 -> Local Space to World Space TransformVector.Input
- [4] Absolute World Position output 0 -> Subtract.A
- [6] Camera Position output 0 -> Subtract.B
- [5] Subtract output 0 -> Normalize.VectorInput
- [9] Dot output 0 -> Divide.A
- [10] Dot output 0 -> Divide.B
- [15] Normalize output 0 -> Dot.A
- [18] Input Normal (Vector3) output 0 -> Dot.B
- [15] Normalize output 0 -> Dot.A
- [19] Normalize output 0 -> Dot.B
- [14] Cross output 0 -> Multiply.A
- [13] -1,-1,-1 output 0 -> Multiply.B
- [16] Normalize output 0 -> Cross.A
- [1] Input Axis (Vector3) output 0 -> Cross.B
- [14] Cross output 0 -> Normalize.VectorInput
- [17] Cross output 0 -> Normalize.VectorInput
- [7] Normalize output 0 -> Cross.A
- [1] Input Axis (Vector3) output 0 -> Cross.B
- [11] VertexNormalWS output 0 -> Input Normal (Vector3).Preview
- [21] Subtract output 0 -> Normalize.VectorInput
- [22] Absolute World Position output 0 -> Subtract.A
- [20] Camera Position output 0 -> Subtract.B
- [26] Max(,0) output 0 -> Power.Base
- [23] Input Exponent (Scalar) output 0 -> Power.Exponent
- [8] Divide output 0 -> Output Cylinder Thickness.A
- [28] Multiply output 0 -> Max(,0).A
- [27] TwoSidedSign output 0 -> Multiply.A
- [9] Dot output 0 -> Multiply.B

## Output Trace Roots

- `Fresnel` is driven by [24] Power
- `Cylinder Thickness` is driven by [8] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


