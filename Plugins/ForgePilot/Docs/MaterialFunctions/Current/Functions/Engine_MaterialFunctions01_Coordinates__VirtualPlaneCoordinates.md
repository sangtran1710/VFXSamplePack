# VirtualPlaneCoordinates

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Coordinates/VirtualPlaneCoordinates.VirtualPlaneCoordinates`
- Category: Engine_MaterialFunctions01/Coordinates
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Virtual coordinates for a raytraced plane, in world units

## Inputs

- `Plane Normal Axis` [FunctionInput_Vector3]
- `Plane Center` [FunctionInput_Vector3] - The center location of the virtual Plane
- `UV Scale` [FunctionInput_Vector2] - The size of the virtual plane
- `Plane X Axis` [FunctionInput_Vector3] - By default, the x axis or tangent vector is defined as the cross product of the Normal and 0,0.00015,1. You can specify a manual tangent vector here.

## Outputs

- `World Position` - The world position at each point on the virtual plane.
- `UVs` - The 2D UVs of the virtual plane coordinates.
- `World Position - Centered` - The world position centered on the specified position. This means position will be 0,0,0 at the specified center.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output World Position | World Position |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Plane Normal Axis (Vector3) | Plane Normal Axis |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 3 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 6 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Plane Center (Vector3) | Plane Center |
| 9 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 10 | expression | `MaterialExpressionConstant3Vector` | 0,0.00015,-1 |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 12 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 13 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | expression | `MaterialExpressionIf` | If |  |
| 18 | expression | `MaterialExpressionConstant` | 0 |  |
| 19 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 23 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 24 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 25 | expression | `MaterialExpressionAppendVector` | Append |  |
| 26 | expression | `MaterialExpressionConstant` | 0 |  |
| 27 | function_output | `MaterialExpressionFunctionOutput` | Output World Position - Centered | World Position - Centered |
| 28 | function_input | `MaterialExpressionFunctionInput` | Input UV Scale (Vector2) | UV Scale |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input Plane X Axis (Vector3) | Plane X Axis |
| 30 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 31 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 32 | expression | `MaterialExpressionDivide` | Divide |  |
| 33 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |

## Exact Input Wiring

### [0] Output World Position

- `A` <= [20] Add output 0

### [2] Output UVs

- `A` <= [33] Add(,0.5) output 0

### [4] Subtract

- `A` <= [3] Absolute World Position output 0
- `B` <= [5] Camera Position output 0

### [6] Dot

- `A` <= [1] Input Plane Normal Axis (Vector3) output 0
- `B` <= [8] Input Plane Center (Vector3) output 0

### [7] Dot

- `A` <= [1] Input Plane Normal Axis (Vector3) output 0
- `B` <= [4] Subtract output 0

### [11] Multiply(,1)

- `A` <= [9] CreateThirdOrthogonalVector output 2

### [13] Dot

- `A` <= [1] Input Plane Normal Axis (Vector3) output 0
- `B` <= [12] Camera Position output 0

### [14] Subtract

- `A` <= [13] Dot output 0
- `B` <= [6] Dot output 0

### [15] Multiply(,-1)

- `A` <= [14] Subtract output 0

### [16] Divide

- `A` <= [15] Multiply(,-1) output 0
- `B` <= [7] Dot output 0

### [17] If

- `A` <= [16] Divide output 0
- `B` <= [18] 0 output 0
- `AGreaterThanB` <= [25] Append output 0
- `AEqualsB` <= [26] 0 output 0
- `ALessThanB` <= [26] 0 output 0

### [20] Add

- `A` <= [19] Camera Position output 0
- `B` <= [21] Multiply output 0

### [21] Multiply

- `A` <= [16] Divide output 0
- `B` <= [4] Subtract output 0

### [22] Dot

- `A` <= [29] Input Plane X Axis (Vector3) output 0
- `B` <= [24] Subtract output 0

### [23] Dot

- `A` <= [31] Normalize output 0
- `B` <= [24] Subtract output 0

### [24] Subtract

- `A` <= [20] Add output 0
- `B` <= [8] Input Plane Center (Vector3) output 0

### [25] Append

- `A` <= [22] Dot output 0
- `B` <= [23] Dot output 0

### [27] Output World Position - Centered

- `A` <= [24] Subtract output 0

### [29] Input Plane X Axis (Vector3)

- `Preview` <= [11] Multiply(,1) output 0

### [30] Cross

- `A` <= [29] Input Plane X Axis (Vector3) output 0
- `B` <= [9] CreateThirdOrthogonalVector output 0

### [31] Normalize

- `VectorInput` <= [30] Cross output 0

### [32] Divide

- `A` <= [17] If output 0
- `B` <= [28] Input UV Scale (Vector2) output 0

### [33] Add(,0.5)

- `A` <= [32] Divide output 0

## Connection List

- [20] Add output 0 -> Output World Position.A
- [33] Add(,0.5) output 0 -> Output UVs.A
- [3] Absolute World Position output 0 -> Subtract.A
- [5] Camera Position output 0 -> Subtract.B
- [1] Input Plane Normal Axis (Vector3) output 0 -> Dot.A
- [8] Input Plane Center (Vector3) output 0 -> Dot.B
- [1] Input Plane Normal Axis (Vector3) output 0 -> Dot.A
- [4] Subtract output 0 -> Dot.B
- [9] CreateThirdOrthogonalVector output 2 -> Multiply(,1).A
- [1] Input Plane Normal Axis (Vector3) output 0 -> Dot.A
- [12] Camera Position output 0 -> Dot.B
- [13] Dot output 0 -> Subtract.A
- [6] Dot output 0 -> Subtract.B
- [14] Subtract output 0 -> Multiply(,-1).A
- [15] Multiply(,-1) output 0 -> Divide.A
- [7] Dot output 0 -> Divide.B
- [16] Divide output 0 -> If.A
- [18] 0 output 0 -> If.B
- [25] Append output 0 -> If.AGreaterThanB
- [26] 0 output 0 -> If.AEqualsB
- [26] 0 output 0 -> If.ALessThanB
- [19] Camera Position output 0 -> Add.A
- [21] Multiply output 0 -> Add.B
- [16] Divide output 0 -> Multiply.A
- [4] Subtract output 0 -> Multiply.B
- [29] Input Plane X Axis (Vector3) output 0 -> Dot.A
- [24] Subtract output 0 -> Dot.B
- [31] Normalize output 0 -> Dot.A
- [24] Subtract output 0 -> Dot.B
- [20] Add output 0 -> Subtract.A
- [8] Input Plane Center (Vector3) output 0 -> Subtract.B
- [22] Dot output 0 -> Append.A
- [23] Dot output 0 -> Append.B
- [24] Subtract output 0 -> Output World Position - Centered.A
- [11] Multiply(,1) output 0 -> Input Plane X Axis (Vector3).Preview
- [29] Input Plane X Axis (Vector3) output 0 -> Cross.A
- [9] CreateThirdOrthogonalVector output 0 -> Cross.B
- [30] Cross output 0 -> Normalize.VectorInput
- [17] If output 0 -> Divide.A
- [28] Input UV Scale (Vector2) output 0 -> Divide.B
- [32] Divide output 0 -> Add(,0.5).A

## Output Trace Roots

- `World Position` is driven by [20] Add
- `UVs` is driven by [33] Add(,0.5)
- `World Position - Centered` is driven by [24] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


