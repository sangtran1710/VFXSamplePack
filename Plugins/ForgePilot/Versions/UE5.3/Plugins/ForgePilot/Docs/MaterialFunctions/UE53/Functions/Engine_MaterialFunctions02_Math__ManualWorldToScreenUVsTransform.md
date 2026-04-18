# ManualWorldToScreenUVsTransform

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ManualWorldToScreenUVsTransform.ManualWorldToScreenUVsTransform`
- Category: Engine_MaterialFunctions02/Math
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This function will allow one to manually recreate any perspective viewport without the use of any intrinsic view properties. This is helpful for Render to texture passes.

## Inputs

- `X Camera to World Vector` [FunctionInput_Vector3] - 1,0,0 transformed from camera space to world space.
- `Y Camera to World Vector` [FunctionInput_Vector3] - 0,1,0 transformed from camera space to world space.
- `Z Camera to World Vector` [FunctionInput_Vector3] - 0,0,1 transformed from camera space to world space.
- `Camera Relative Position` [FunctionInput_Vector3] - This will typically be world position - camera position.
- `Tan(FOV/2)*[1,Screen Res Y/X]` [FunctionInput_Vector2] - Tan of FOV/2 for x. For y it's the ratio of the screen's y resolution to the x resolution multiplied by tan of fov/2. See an example within this function. 

## Outputs

- `Screen UVs`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 1 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 4 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionTransform` | Camera Space to World Space TransformVector |  |
| 7 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | FOV |  |
| 15 | expression | `MaterialExpressionDivide` | Divide(,2) |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 19 | expression | `MaterialExpressionTangent` | Tangent |  |
| 20 | function_call | `MaterialExpressionMaterialFunctionCall` | ScreenResolution |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 22 | expression | `MaterialExpressionDivide` | Divide |  |
| 23 | expression | `MaterialExpressionConstant2Vector` | 0.5,0.5 |  |
| 24 | expression | `MaterialExpressionConstant2Vector` | 0.5,-0.5 |  |
| 25 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 26 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 27 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 28 | function_output | `MaterialExpressionFunctionOutput` | Output Screen UVs | Screen UVs |
| 29 | function_input | `MaterialExpressionFunctionInput` | Input X Camera to World Vector (Vector3) | X Camera to World Vector |
| 30 | expression | `MaterialExpressionConstant3Vector` | 1,0,0 |  |
| 31 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 32 | function_input | `MaterialExpressionFunctionInput` | Input Y Camera to World Vector (Vector3) | Y Camera to World Vector |
| 33 | function_input | `MaterialExpressionFunctionInput` | Input Z Camera to World Vector (Vector3) | Z Camera to World Vector |
| 34 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input Camera Relative Position (Vector3) | Camera Relative Position |
| 36 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 37 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 38 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 39 | expression | `MaterialExpressionViewProperty` | Tan(0.5 * Field Of View) |  |
| 40 | function_input | `MaterialExpressionFunctionInput` | Input Tan(FOV/2)*[1,Screen Res Y/X] (Vector2) | Tan(FOV/2)*[1,Screen Res Y/X] |
| 41 | expression | `MaterialExpressionAppendVector` | Append |  |
| 42 | expression | `MaterialExpressionConstant` | 1 |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Camera Space to World Space TransformVector

- `Input` <= [30] 1,0,0 output 0

### [1] Camera Space to World Space TransformVector

- `Input` <= [31] 0,1,0 output 0

### [2] Subtract

- `A` <= [12] Absolute World Position output 0
- `B` <= [3] Camera Position output 0

### [4] Dot

- `A` <= [25] Reroute Node (reroutes wires) output 0
- `B` <= [27] Reroute Node (reroutes wires) output 0

### [5] Dot

- `A` <= [25] Reroute Node (reroutes wires) output 0
- `B` <= [26] Reroute Node (reroutes wires) output 0

### [6] Camera Space to World Space TransformVector

- `Input` <= [34] 0,0,1 output 0

### [7] Dot

- `A` <= [25] Reroute Node (reroutes wires) output 0
- `B` <= [33] Input Z Camera to World Vector (Vector3) output 0

### [8] Divide

- `A` <= [18] Mask ( R G ) output 0
- `B` <= [16] Multiply output 0

### [9] Multiply

- `A` <= [36] Reroute Node (reroutes wires) output 0
- `B` <= [4] Dot output 0

### [10] Add

- `A` <= [9] Multiply output 0
- `B` <= [11] Multiply output 0

### [11] Multiply

- `A` <= [38] Reroute Node (reroutes wires) output 0
- `B` <= [5] Dot output 0

### [13] Add

- `A` <= [17] Multiply output 0
- `B` <= [23] 0.5,0.5 output 0

### [15] Divide(,2)

- `A` <= [14] FOV output 2

### [16] Multiply

- `A` <= [7] Dot output 0
- `B` <= [40] Input Tan(FOV/2)*[1,Screen Res Y/X] (Vector2) output 0

### [17] Multiply

- `A` <= [8] Divide output 0
- `B` <= [24] 0.5,-0.5 output 0

### [18] Mask ( R G )

- `Input` <= [10] Add output 0

### [19] Tangent

- `Input` <= [15] Divide(,2) output 0

### [22] Divide

- `A` <= [21] BreakOutFloat2Components output 1
- `B` <= [21] BreakOutFloat2Components output 0

### [25] Reroute Node (reroutes wires)

- `Input` <= [35] Input Camera Relative Position (Vector3) output 0

### [26] Reroute Node (reroutes wires)

- `Input` <= [32] Input Y Camera to World Vector (Vector3) output 0

### [27] Reroute Node (reroutes wires)

- `Input` <= [29] Input X Camera to World Vector (Vector3) output 0

### [28] Output Screen UVs

- `A` <= [13] Add output 0

### [29] Input X Camera to World Vector (Vector3)

- `Preview` <= [0] Camera Space to World Space TransformVector output 0

### [32] Input Y Camera to World Vector (Vector3)

- `Preview` <= [1] Camera Space to World Space TransformVector output 0

### [33] Input Z Camera to World Vector (Vector3)

- `Preview` <= [6] Camera Space to World Space TransformVector output 0

### [35] Input Camera Relative Position (Vector3)

- `Preview` <= [2] Subtract output 0

### [36] Reroute Node (reroutes wires)

- `Input` <= [30] 1,0,0 output 0

### [37] Reroute Node (reroutes wires)

- `Input` <= [31] 0,1,0 output 0

### [38] Reroute Node (reroutes wires)

- `Input` <= [37] Reroute Node (reroutes wires) output 0

### [40] Input Tan(FOV/2)*[1,Screen Res Y/X] (Vector2)

- `Preview` <= [39] Tan(0.5 * Field Of View) output 0

### [41] Append

- `A` <= [42] 1 output 0
- `B` <= [22] Divide output 0

### [43] Multiply

- `A` <= [19] Tangent output 0
- `B` <= [41] Append output 0

## Connection List

- [30] 1,0,0 output 0 -> Camera Space to World Space TransformVector.Input
- [31] 0,1,0 output 0 -> Camera Space to World Space TransformVector.Input
- [12] Absolute World Position output 0 -> Subtract.A
- [3] Camera Position output 0 -> Subtract.B
- [25] Reroute Node (reroutes wires) output 0 -> Dot.A
- [27] Reroute Node (reroutes wires) output 0 -> Dot.B
- [25] Reroute Node (reroutes wires) output 0 -> Dot.A
- [26] Reroute Node (reroutes wires) output 0 -> Dot.B
- [34] 0,0,1 output 0 -> Camera Space to World Space TransformVector.Input
- [25] Reroute Node (reroutes wires) output 0 -> Dot.A
- [33] Input Z Camera to World Vector (Vector3) output 0 -> Dot.B
- [18] Mask ( R G ) output 0 -> Divide.A
- [16] Multiply output 0 -> Divide.B
- [36] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [4] Dot output 0 -> Multiply.B
- [9] Multiply output 0 -> Add.A
- [11] Multiply output 0 -> Add.B
- [38] Reroute Node (reroutes wires) output 0 -> Multiply.A
- [5] Dot output 0 -> Multiply.B
- [17] Multiply output 0 -> Add.A
- [23] 0.5,0.5 output 0 -> Add.B
- [14] FOV output 2 -> Divide(,2).A
- [7] Dot output 0 -> Multiply.A
- [40] Input Tan(FOV/2)*[1,Screen Res Y/X] (Vector2) output 0 -> Multiply.B
- [8] Divide output 0 -> Multiply.A
- [24] 0.5,-0.5 output 0 -> Multiply.B
- [10] Add output 0 -> Mask ( R G ).Input
- [15] Divide(,2) output 0 -> Tangent.Input
- [21] BreakOutFloat2Components output 1 -> Divide.A
- [21] BreakOutFloat2Components output 0 -> Divide.B
- [35] Input Camera Relative Position (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [32] Input Y Camera to World Vector (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [29] Input X Camera to World Vector (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [13] Add output 0 -> Output Screen UVs.A
- [0] Camera Space to World Space TransformVector output 0 -> Input X Camera to World Vector (Vector3).Preview
- [1] Camera Space to World Space TransformVector output 0 -> Input Y Camera to World Vector (Vector3).Preview
- [6] Camera Space to World Space TransformVector output 0 -> Input Z Camera to World Vector (Vector3).Preview
- [2] Subtract output 0 -> Input Camera Relative Position (Vector3).Preview
- [30] 1,0,0 output 0 -> Reroute Node (reroutes wires).Input
- [31] 0,1,0 output 0 -> Reroute Node (reroutes wires).Input
- [37] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [39] Tan(0.5 * Field Of View) output 0 -> Input Tan(FOV/2)*[1,Screen Res Y/X] (Vector2).Preview
- [42] 1 output 0 -> Append.A
- [22] Divide output 0 -> Append.B
- [19] Tangent output 0 -> Multiply.A
- [41] Append output 0 -> Multiply.B

## Output Trace Roots

- `Screen UVs` is driven by [13] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
