# DeriveTangentBasis

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/DeriveTangentBasis.DeriveTangentBasis`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Derive a secondary tangent basis from another UV channel.

## Inputs

- `UVs` [FunctionInput_Vector2] - Which UV set to use when creating new tangent basis.
- `Normal` [FunctionInput_Vector3]

## Outputs

- `Tangent`
- `World Space Normal`
- `Binormal`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Tangent | Tangent |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input UVs (Vector2) | UVs |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[1] |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 4 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output World Space Normal | World Space Normal |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output Binormal | Binormal |
| 7 | function_call | `MaterialExpressionMaterialFunctionCall` | Transform3x3Matrix |  |
| 8 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 9 | expression | `MaterialExpressionDDY` | DDY |  |
| 10 | expression | `MaterialExpressionDDX` | DDX |  |
| 11 | expression | `MaterialExpressionDDY` | DDY |  |
| 12 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 13 | expression | `MaterialExpressionDDX` | DDX |  |
| 14 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 15 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 16 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 17 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionAdd` | Add |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 26 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 27 | expression | `MaterialExpressionMax` | Max |  |
| 28 | expression | `MaterialExpressionDivide` | Divide(1,) |  |
| 29 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 30 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Tangent

- `A` <= [30] Multiply output 0

### [1] Input UVs (Vector2)

- `Preview` <= [2] TexCoord[1] output 0

### [3] Input Normal (Vector3)

- `Preview` <= [4] 0,0,1 output 0

### [5] Output World Space Normal

- `A` <= [7] Transform3x3Matrix output 0

### [6] Output Binormal

- `A` <= [31] Multiply output 0

### [9] DDY

- `Value` <= [12] Absolute World Position (Excluding Material Offsets) output 0

### [10] DDX

- `Value` <= [1] Input UVs (Vector2) output 0

### [11] DDY

- `Value` <= [1] Input UVs (Vector2) output 0

### [13] DDX

- `Value` <= [12] Absolute World Position (Excluding Material Offsets) output 0

### [14] Cross

- `A` <= [9] DDY output 0
- `B` <= [15] VertexNormalWS output 0

### [16] Cross

- `A` <= [15] VertexNormalWS output 0
- `B` <= [13] DDX output 0

### [19] Multiply

- `A` <= [14] Cross output 0
- `B` <= [17] BreakOutFloat2Components output 0

### [20] Multiply

- `A` <= [16] Cross output 0
- `B` <= [18] BreakOutFloat2Components output 0

### [21] Add

- `A` <= [19] Multiply output 0
- `B` <= [20] Multiply output 0

### [22] Multiply

- `A` <= [14] Cross output 0
- `B` <= [17] BreakOutFloat2Components output 1

### [23] Multiply

- `A` <= [16] Cross output 0
- `B` <= [18] BreakOutFloat2Components output 1

### [24] Add

- `A` <= [22] Multiply output 0
- `B` <= [23] Multiply output 0

### [25] Dot

- `A` <= [21] Add output 0
- `B` <= [21] Add output 0

### [26] Dot

- `A` <= [24] Add output 0
- `B` <= [24] Add output 0

### [27] Max

- `A` <= [25] Dot output 0
- `B` <= [26] Dot output 0

### [28] Divide(1,)

- `B` <= [29] Sqrt output 0

### [29] Sqrt

- `Input` <= [27] Max output 0

### [30] Multiply

- `A` <= [21] Add output 0
- `B` <= [28] Divide(1,) output 0

### [31] Multiply

- `A` <= [28] Divide(1,) output 0
- `B` <= [24] Add output 0

## Connection List

- [30] Multiply output 0 -> Output Tangent.A
- [2] TexCoord[1] output 0 -> Input UVs (Vector2).Preview
- [4] 0,0,1 output 0 -> Input Normal (Vector3).Preview
- [7] Transform3x3Matrix output 0 -> Output World Space Normal.A
- [31] Multiply output 0 -> Output Binormal.A
- [12] Absolute World Position (Excluding Material Offsets) output 0 -> DDY.Value
- [1] Input UVs (Vector2) output 0 -> DDX.Value
- [1] Input UVs (Vector2) output 0 -> DDY.Value
- [12] Absolute World Position (Excluding Material Offsets) output 0 -> DDX.Value
- [9] DDY output 0 -> Cross.A
- [15] VertexNormalWS output 0 -> Cross.B
- [15] VertexNormalWS output 0 -> Cross.A
- [13] DDX output 0 -> Cross.B
- [14] Cross output 0 -> Multiply.A
- [17] BreakOutFloat2Components output 0 -> Multiply.B
- [16] Cross output 0 -> Multiply.A
- [18] BreakOutFloat2Components output 0 -> Multiply.B
- [19] Multiply output 0 -> Add.A
- [20] Multiply output 0 -> Add.B
- [14] Cross output 0 -> Multiply.A
- [17] BreakOutFloat2Components output 1 -> Multiply.B
- [16] Cross output 0 -> Multiply.A
- [18] BreakOutFloat2Components output 1 -> Multiply.B
- [22] Multiply output 0 -> Add.A
- [23] Multiply output 0 -> Add.B
- [21] Add output 0 -> Dot.A
- [21] Add output 0 -> Dot.B
- [24] Add output 0 -> Dot.A
- [24] Add output 0 -> Dot.B
- [25] Dot output 0 -> Max.A
- [26] Dot output 0 -> Max.B
- [29] Sqrt output 0 -> Divide(1,).B
- [27] Max output 0 -> Sqrt.Input
- [21] Add output 0 -> Multiply.A
- [28] Divide(1,) output 0 -> Multiply.B
- [28] Divide(1,) output 0 -> Multiply.A
- [24] Add output 0 -> Multiply.B

## Output Trace Roots

- `Tangent` is driven by [30] Multiply
- `World Space Normal` is driven by [7] Transform3x3Matrix
- `Binormal` is driven by [31] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
