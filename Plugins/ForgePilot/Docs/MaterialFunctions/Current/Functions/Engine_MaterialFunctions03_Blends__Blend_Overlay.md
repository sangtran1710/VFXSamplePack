# Blend_Overlay

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Overlay.Blend_Overlay`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Emulates Photoshop's overlay blend mode.  Lighter pixels will become lighter, darker will become darker.

## Inputs

- `Blend` [FunctionInput_Vector3] - Blend texture input
- `Base` [FunctionInput_Vector3] - Base texture input

## Outputs

- `Result` - Emulates Photoshop's Overlay blend.  The order of inputs does change the result.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Blend (Vector3) | Blend |
| 6 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant` | 2 |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 10 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 12 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionConstant` | 2 |  |
| 17 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 19 | expression | `MaterialExpressionAppendVector` | Append |  |
| 20 | expression | `MaterialExpressionIf` | If |  |
| 21 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 22 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 24 | expression | `MaterialExpressionIf` | If |  |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 26 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 27 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 28 | expression | `MaterialExpressionOneMinus` | 1-x |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Append output 0

### [1] Append

- `A` <= [19] Append output 0
- `B` <= [2] If output 0

### [2] If

- `A` <= [18] Mask ( B ) output 0
- `B` <= [17] 0.5 output 0
- `AGreaterThanB` <= [11] Mask ( B ) output 0
- `ALessThanB` <= [3] Mask ( B ) output 0

### [3] Mask ( B )

- `Input` <= [4] Multiply output 0

### [4] Multiply

- `A` <= [7] Multiply output 0
- `B` <= [5] Input Blend (Vector3) output 0

### [5] Input Blend (Vector3)

- `Preview` <= [6] Texture Sample output 0

### [7] Multiply

- `A` <= [9] Input Base (Vector3) output 0
- `B` <= [8] 2 output 0

### [9] Input Base (Vector3)

- `Preview` <= [10] Texture Sample output 0

### [11] Mask ( B )

- `Input` <= [12] 1-x output 0

### [12] 1-x

- `Input` <= [13] Multiply output 0

### [13] Multiply

- `A` <= [15] Multiply output 0
- `B` <= [14] 1-x output 0

### [14] 1-x

- `Input` <= [5] Input Blend (Vector3) output 0

### [15] Multiply

- `A` <= [28] 1-x output 0
- `B` <= [16] 2 output 0

### [18] Mask ( B )

- `Input` <= [9] Input Base (Vector3) output 0

### [19] Append

- `A` <= [24] If output 0
- `B` <= [20] If output 0

### [20] If

- `A` <= [23] Mask ( G ) output 0
- `B` <= [17] 0.5 output 0
- `AGreaterThanB` <= [22] Mask ( G ) output 0
- `ALessThanB` <= [21] Mask ( G ) output 0

### [21] Mask ( G )

- `Input` <= [4] Multiply output 0

### [22] Mask ( G )

- `Input` <= [12] 1-x output 0

### [23] Mask ( G )

- `Input` <= [9] Input Base (Vector3) output 0

### [24] If

- `A` <= [27] Mask ( R ) output 0
- `B` <= [17] 0.5 output 0
- `AGreaterThanB` <= [26] Mask ( R ) output 0
- `ALessThanB` <= [25] Mask ( R ) output 0

### [25] Mask ( R )

- `Input` <= [4] Multiply output 0

### [26] Mask ( R )

- `Input` <= [12] 1-x output 0

### [27] Mask ( R )

- `Input` <= [9] Input Base (Vector3) output 0

### [28] 1-x

- `Input` <= [9] Input Base (Vector3) output 0

## Connection List

- [1] Append output 0 -> Output Result.A
- [19] Append output 0 -> Append.A
- [2] If output 0 -> Append.B
- [18] Mask ( B ) output 0 -> If.A
- [17] 0.5 output 0 -> If.B
- [11] Mask ( B ) output 0 -> If.AGreaterThanB
- [3] Mask ( B ) output 0 -> If.ALessThanB
- [4] Multiply output 0 -> Mask ( B ).Input
- [7] Multiply output 0 -> Multiply.A
- [5] Input Blend (Vector3) output 0 -> Multiply.B
- [6] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [9] Input Base (Vector3) output 0 -> Multiply.A
- [8] 2 output 0 -> Multiply.B
- [10] Texture Sample output 0 -> Input Base (Vector3).Preview
- [12] 1-x output 0 -> Mask ( B ).Input
- [13] Multiply output 0 -> 1-x.Input
- [15] Multiply output 0 -> Multiply.A
- [14] 1-x output 0 -> Multiply.B
- [5] Input Blend (Vector3) output 0 -> 1-x.Input
- [28] 1-x output 0 -> Multiply.A
- [16] 2 output 0 -> Multiply.B
- [9] Input Base (Vector3) output 0 -> Mask ( B ).Input
- [24] If output 0 -> Append.A
- [20] If output 0 -> Append.B
- [23] Mask ( G ) output 0 -> If.A
- [17] 0.5 output 0 -> If.B
- [22] Mask ( G ) output 0 -> If.AGreaterThanB
- [21] Mask ( G ) output 0 -> If.ALessThanB
- [4] Multiply output 0 -> Mask ( G ).Input
- [12] 1-x output 0 -> Mask ( G ).Input
- [9] Input Base (Vector3) output 0 -> Mask ( G ).Input
- [27] Mask ( R ) output 0 -> If.A
- [17] 0.5 output 0 -> If.B
- [26] Mask ( R ) output 0 -> If.AGreaterThanB
- [25] Mask ( R ) output 0 -> If.ALessThanB
- [4] Multiply output 0 -> Mask ( R ).Input
- [12] 1-x output 0 -> Mask ( R ).Input
- [9] Input Base (Vector3) output 0 -> Mask ( R ).Input
- [9] Input Base (Vector3) output 0 -> 1-x.Input

## Output Trace Roots

- `Result` is driven by [1] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


