# HeightToNormalSmooth

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/HeightToNormalSmooth.HeightToNormalSmooth`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Provide perpixel height data and return a partially smooth normal (requires AA for smooth details)

## Inputs

- `Height` [FunctionInput_Scalar] - Height in world units
- `World Space Vertex Normals` [FunctionInput_Vector3]
- `Absolute World Position` [FunctionInput_Vector3]

## Outputs

- `WorldSpaceNormal`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output WorldSpaceNormal | WorldSpaceNormal |
| 1 | expression | `MaterialExpressionAdd` | Add |  |
| 2 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Height (Scalar) | Height |
| 4 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 5 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 6 | expression | `MaterialExpressionDDX` | DDX |  |
| 7 | expression | `MaterialExpressionDDY` | DDY |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 10 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 11 | expression | `MaterialExpressionDDX` | DDX |  |
| 12 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 13 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 14 | expression | `MaterialExpressionDDY` | DDY |  |
| 15 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input World Space Vertex Normals (Vector3) | World Space Vertex Normals |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Absolute World Position (Vector3) | Absolute World Position |
| 18 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 19 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [0] Output WorldSpaceNormal

- `A` <= [1] Add output 0

### [1] Add

- `A` <= [15] Subtract output 0
- `B` <= [18] Reroute Node (reroutes wires) output 0

### [4] Normalize

- `VectorInput` <= [5] Cross output 0

### [5] Cross

- `A` <= [6] DDX output 0
- `B` <= [7] DDY output 0

### [6] DDX

- `Value` <= [8] Add output 0

### [7] DDY

- `Value` <= [8] Add output 0

### [8] Add

- `A` <= [10] Multiply output 0
- `B` <= [19] Reroute Node (reroutes wires) output 0

### [10] Multiply

- `A` <= [3] Input Height (Scalar) output 0
- `B` <= [18] Reroute Node (reroutes wires) output 0

### [11] DDX

- `Value` <= [19] Reroute Node (reroutes wires) output 0

### [12] Normalize

- `VectorInput` <= [13] Cross output 0

### [13] Cross

- `A` <= [11] DDX output 0
- `B` <= [14] DDY output 0

### [14] DDY

- `Value` <= [19] Reroute Node (reroutes wires) output 0

### [15] Subtract

- `A` <= [4] Normalize output 0
- `B` <= [12] Normalize output 0

### [16] Input World Space Vertex Normals (Vector3)

- `Preview` <= [2] VertexNormalWS output 0

### [17] Input Absolute World Position (Vector3)

- `Preview` <= [9] Absolute World Position output 0

### [18] Reroute Node (reroutes wires)

- `Input` <= [16] Input World Space Vertex Normals (Vector3) output 0

### [19] Reroute Node (reroutes wires)

- `Input` <= [17] Input Absolute World Position (Vector3) output 0

## Connection List

- [1] Add output 0 -> Output WorldSpaceNormal.A
- [15] Subtract output 0 -> Add.A
- [18] Reroute Node (reroutes wires) output 0 -> Add.B
- [5] Cross output 0 -> Normalize.VectorInput
- [6] DDX output 0 -> Cross.A
- [7] DDY output 0 -> Cross.B
- [8] Add output 0 -> DDX.Value
- [8] Add output 0 -> DDY.Value
- [10] Multiply output 0 -> Add.A
- [19] Reroute Node (reroutes wires) output 0 -> Add.B
- [3] Input Height (Scalar) output 0 -> Multiply.A
- [18] Reroute Node (reroutes wires) output 0 -> Multiply.B
- [19] Reroute Node (reroutes wires) output 0 -> DDX.Value
- [13] Cross output 0 -> Normalize.VectorInput
- [11] DDX output 0 -> Cross.A
- [14] DDY output 0 -> Cross.B
- [19] Reroute Node (reroutes wires) output 0 -> DDY.Value
- [4] Normalize output 0 -> Subtract.A
- [12] Normalize output 0 -> Subtract.B
- [2] VertexNormalWS output 0 -> Input World Space Vertex Normals (Vector3).Preview
- [9] Absolute World Position output 0 -> Input Absolute World Position (Vector3).Preview
- [16] Input World Space Vertex Normals (Vector3) output 0 -> Reroute Node (reroutes wires).Input
- [17] Input Absolute World Position (Vector3) output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `WorldSpaceNormal` is driven by [1] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


