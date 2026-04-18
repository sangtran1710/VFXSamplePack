# Blend_HardLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_HardLight.Blend_HardLight`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Harsher version of Soft Light blend

## Inputs

- `Blend` [FunctionInput_Vector3] - Blend texture input
- `Base` [FunctionInput_Vector3] - Base texture input

## Outputs

- `Result` - Harsher version of Soft Light blend

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Blend (Vector3) | Blend |
| 4 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 6 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 7 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionIf` | If |  |
| 10 | expression | `MaterialExpressionIf` | If |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionConstant` | 2 |  |
| 13 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 14 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 19 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 21 | expression | `MaterialExpressionAbs` | Abs |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionConstant` | 2 |  |
| 26 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 27 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 28 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 29 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 30 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [21] Abs output 0

### [1] Append

- `A` <= [8] Append output 0
- `B` <= [2] If output 0

### [2] If

- `A` <= [20] Mask ( B ) output 0
- `B` <= [7] 0.5 output 0
- `AGreaterThanB` <= [28] Mask ( B ) output 0
- `ALessThanB` <= [31] Mask ( B ) output 0

### [3] Input Blend (Vector3)

- `Preview` <= [4] Texture Sample output 0

### [5] Input Base (Vector3)

- `Preview` <= [6] Texture Sample output 0

### [8] Append

- `A` <= [10] If output 0
- `B` <= [9] If output 0

### [9] If

- `A` <= [19] Mask ( G ) output 0
- `B` <= [7] 0.5 output 0
- `AGreaterThanB` <= [27] Mask ( G ) output 0
- `ALessThanB` <= [30] Mask ( G ) output 0

### [10] If

- `A` <= [18] Mask ( R ) output 0
- `B` <= [7] 0.5 output 0
- `AGreaterThanB` <= [26] Mask ( R ) output 0
- `ALessThanB` <= [29] Mask ( R ) output 0

### [11] Multiply

- `A` <= [3] Input Blend (Vector3) output 0
- `B` <= [12] 2 output 0

### [13] 1-x

- `Input` <= [22] Multiply output 0

### [14] 1-x

- `Input` <= [5] Input Base (Vector3) output 0

### [15] Multiply

- `A` <= [14] 1-x output 0
- `B` <= [13] 1-x output 0

### [16] 1-x

- `Input` <= [15] Multiply output 0

### [17] Multiply

- `A` <= [5] Input Base (Vector3) output 0
- `B` <= [11] Multiply output 0

### [18] Mask ( R )

- `Input` <= [3] Input Blend (Vector3) output 0

### [19] Mask ( G )

- `Input` <= [3] Input Blend (Vector3) output 0

### [20] Mask ( B )

- `Input` <= [3] Input Blend (Vector3) output 0

### [21] Abs

- `Input` <= [1] Append output 0

### [22] Multiply

- `A` <= [24] Add output 0
- `B` <= [25] 2 output 0

### [24] Add

- `A` <= [3] Input Blend (Vector3) output 0
- `B` <= [23] -0.5 output 0

### [26] Mask ( R )

- `Input` <= [16] 1-x output 0

### [27] Mask ( G )

- `Input` <= [16] 1-x output 0

### [28] Mask ( B )

- `Input` <= [16] 1-x output 0

### [29] Mask ( R )

- `Input` <= [17] Multiply output 0

### [30] Mask ( G )

- `Input` <= [17] Multiply output 0

### [31] Mask ( B )

- `Input` <= [17] Multiply output 0

## Connection List

- [21] Abs output 0 -> Output Result.A
- [8] Append output 0 -> Append.A
- [2] If output 0 -> Append.B
- [20] Mask ( B ) output 0 -> If.A
- [7] 0.5 output 0 -> If.B
- [28] Mask ( B ) output 0 -> If.AGreaterThanB
- [31] Mask ( B ) output 0 -> If.ALessThanB
- [4] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [6] Texture Sample output 0 -> Input Base (Vector3).Preview
- [10] If output 0 -> Append.A
- [9] If output 0 -> Append.B
- [19] Mask ( G ) output 0 -> If.A
- [7] 0.5 output 0 -> If.B
- [27] Mask ( G ) output 0 -> If.AGreaterThanB
- [30] Mask ( G ) output 0 -> If.ALessThanB
- [18] Mask ( R ) output 0 -> If.A
- [7] 0.5 output 0 -> If.B
- [26] Mask ( R ) output 0 -> If.AGreaterThanB
- [29] Mask ( R ) output 0 -> If.ALessThanB
- [3] Input Blend (Vector3) output 0 -> Multiply.A
- [12] 2 output 0 -> Multiply.B
- [22] Multiply output 0 -> 1-x.Input
- [5] Input Base (Vector3) output 0 -> 1-x.Input
- [14] 1-x output 0 -> Multiply.A
- [13] 1-x output 0 -> Multiply.B
- [15] Multiply output 0 -> 1-x.Input
- [5] Input Base (Vector3) output 0 -> Multiply.A
- [11] Multiply output 0 -> Multiply.B
- [3] Input Blend (Vector3) output 0 -> Mask ( R ).Input
- [3] Input Blend (Vector3) output 0 -> Mask ( G ).Input
- [3] Input Blend (Vector3) output 0 -> Mask ( B ).Input
- [1] Append output 0 -> Abs.Input
- [24] Add output 0 -> Multiply.A
- [25] 2 output 0 -> Multiply.B
- [3] Input Blend (Vector3) output 0 -> Add.A
- [23] -0.5 output 0 -> Add.B
- [16] 1-x output 0 -> Mask ( R ).Input
- [16] 1-x output 0 -> Mask ( G ).Input
- [16] 1-x output 0 -> Mask ( B ).Input
- [17] Multiply output 0 -> Mask ( R ).Input
- [17] Multiply output 0 -> Mask ( G ).Input
- [17] Multiply output 0 -> Mask ( B ).Input

## Output Trace Roots

- `Result` is driven by [21] Abs

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
