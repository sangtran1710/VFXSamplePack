# MS_CanopyCreatorMeshExpansion

- Path: `/Engine/Functions/Engine_MaterialFunctions02/MAXScripts/MS_CanopyCreatorMeshExpansion.MS_CanopyCreatorMeshExpansion`
- Category: Engine_MaterialFunctions02/MAXScripts
- Use: Example/support content. Treat as reference first; validate before production use.
- Risk: Reference-only by default; verify visual output and packaging before production use.
- Inspect status: PASS

## Description

This function  widens strips of polies along the U of their UVs.  These strips shouuld be extremely thin and imported with remove degenerate triangles disabled. Useful for Hair, Grass, Cables etc.

## Inputs

- `WorldPosition` [FunctionInput_Vector3]
- `Normal Map` [FunctionInput_Vector3] - Adds a texture normal to the normalmap transform

## Outputs

- `Normal` - Add this result to your other WPO calculations
- `WorldPositionOffset` - Add this result to your other WPO calculations
- `Black=Spline Thicken White=Normal` - Effect Alpha
- `Branch Radius`
- `white = knots black = everything else` - Effect Alpha

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Normal | Normal |
| 1 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 3 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 4 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 10 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 11 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 12 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output WorldPositionOffset | WorldPositionOffset |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | CreateThirdOrthogonalVector |  |
| 15 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 18 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 20 | expression | `MaterialExpressionConstant` | -1 |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | TangentBasis |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Normal Map (Vector3) | Normal Map |
| 23 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[2] |  |
| 24 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 25 | expression | `MaterialExpressionVertexColor` | Vertex Color |  |
| 26 | function_output | `MaterialExpressionFunctionOutput` | Output Black=Spline Thicken White=Normal | Black=Spline Thicken White=Normal |
| 27 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 28 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 29 | function_output | `MaterialExpressionFunctionOutput` | Output Branch Radius | Branch Radius |
| 30 | expression | `MaterialExpressionScalarParameter` | Param (1.923) 'Branch Thickening Amount' |  |
| 31 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 32 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 33 | function_output | `MaterialExpressionFunctionOutput` | Output white = knots black = everything else | white = knots black = everything else |
| 34 | expression | `MaterialExpressionConstant` | 1 |  |
| 35 | expression | `MaterialExpressionIf` | If |  |
| 36 | expression | `MaterialExpressionConstant` | 0 |  |
| 37 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 38 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 39 | expression | `MaterialExpressionAdd` | Add |  |

## Exact Input Wiring

### [0] Output Normal

- `A` <= [15] Transform3x3Matrix output 0

### [1] Subtract

- `A` <= [4] Camera Position output 0
- `B` <= [2] Input WorldPosition (Vector3) output 0

### [2] Input WorldPosition (Vector3)

- `Preview` <= [3] Absolute World Position output 0

### [5] Multiply

- `A` <= [11] Normalize output 0
- `B` <= [38] Multiply output 0

### [6] Multiply

- `A` <= [39] Add output 0
- `B` <= [8] Add output 0

### [7] Mask ( R )

- `Input` <= [9] TexCoord[0] output 0

### [8] Add

- `A` <= [10] -0.5 output 0
- `B` <= [7] Mask ( R ) output 0

### [11] Normalize

- `VectorInput` <= [12] Cross output 0

### [12] Cross

- `A` <= [1] Subtract output 0
- `B` <= [21] TangentBasis output 1

### [13] Output WorldPositionOffset

- `A` <= [27] Multiply output 0

### [16] Normalize

- `VectorInput` <= [14] CreateThirdOrthogonalVector output 0

### [17] Normalize

- `VectorInput` <= [14] CreateThirdOrthogonalVector output 1

### [18] Normalize

- `VectorInput` <= [14] CreateThirdOrthogonalVector output 2

### [19] Multiply

- `A` <= [21] TangentBasis output 1
- `B` <= [20] -1 output 0

### [26] Output Black=Spline Thicken White=Normal

- `A` <= [35] If output 0

### [27] Multiply

- `A` <= [5] Multiply output 0
- `B` <= [28] 1-x output 0

### [28] 1-x

- `Input` <= [35] If output 0

### [29] Output Branch Radius

- `A` <= [24] BreakOutFloat2Components output 0

### [31] SphereMask

- `A` <= [25] Vertex Color output 4
- `B` <= [32] 0.5 output 0

### [33] Output white = knots black = everything else

- `A` <= [31] SphereMask output 0

### [35] If

- `A` <= [25] Vertex Color output 4
- `B` <= [36] 0 output 0
- `AGreaterThanB` <= [34] 1 output 0
- `AEqualsB` <= [36] 0 output 0
- `ALessThanB` <= [34] 1 output 0

### [37] 1-x

- `Input` <= [31] SphereMask output 0

### [38] Multiply

- `A` <= [6] Multiply output 0
- `B` <= [30] Param (1.923) 'Branch Thickening Amount' output 0

### [39] Add

- `A` <= [24] BreakOutFloat2Components output 0
- `B` <= [30] Param (1.923) 'Branch Thickening Amount' output 0

## Connection List

- [15] Transform3x3Matrix output 0 -> Output Normal.A
- [4] Camera Position output 0 -> Subtract.A
- [2] Input WorldPosition (Vector3) output 0 -> Subtract.B
- [3] Absolute World Position output 0 -> Input WorldPosition (Vector3).Preview
- [11] Normalize output 0 -> Multiply.A
- [38] Multiply output 0 -> Multiply.B
- [39] Add output 0 -> Multiply.A
- [8] Add output 0 -> Multiply.B
- [9] TexCoord[0] output 0 -> Mask ( R ).Input
- [10] -0.5 output 0 -> Add.A
- [7] Mask ( R ) output 0 -> Add.B
- [12] Cross output 0 -> Normalize.VectorInput
- [1] Subtract output 0 -> Cross.A
- [21] TangentBasis output 1 -> Cross.B
- [27] Multiply output 0 -> Output WorldPositionOffset.A
- [14] CreateThirdOrthogonalVector output 0 -> Normalize.VectorInput
- [14] CreateThirdOrthogonalVector output 1 -> Normalize.VectorInput
- [14] CreateThirdOrthogonalVector output 2 -> Normalize.VectorInput
- [21] TangentBasis output 1 -> Multiply.A
- [20] -1 output 0 -> Multiply.B
- [35] If output 0 -> Output Black=Spline Thicken White=Normal.A
- [5] Multiply output 0 -> Multiply.A
- [28] 1-x output 0 -> Multiply.B
- [35] If output 0 -> 1-x.Input
- [24] BreakOutFloat2Components output 0 -> Output Branch Radius.A
- [25] Vertex Color output 4 -> SphereMask.A
- [32] 0.5 output 0 -> SphereMask.B
- [31] SphereMask output 0 -> Output white = knots black = everything else.A
- [25] Vertex Color output 4 -> If.A
- [36] 0 output 0 -> If.B
- [34] 1 output 0 -> If.AGreaterThanB
- [36] 0 output 0 -> If.AEqualsB
- [34] 1 output 0 -> If.ALessThanB
- [31] SphereMask output 0 -> 1-x.Input
- [6] Multiply output 0 -> Multiply.A
- [30] Param (1.923) 'Branch Thickening Amount' output 0 -> Multiply.B
- [24] BreakOutFloat2Components output 0 -> Add.A
- [30] Param (1.923) 'Branch Thickening Amount' output 0 -> Add.B

## Output Trace Roots

- `Normal` is driven by [15] Transform3x3Matrix
- `WorldPositionOffset` is driven by [27] Multiply
- `Black=Spline Thicken White=Normal` is driven by [35] If
- `Branch Radius` is driven by [24] BreakOutFloat2Components
- `white = knots black = everything else` is driven by [31] SphereMask

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
