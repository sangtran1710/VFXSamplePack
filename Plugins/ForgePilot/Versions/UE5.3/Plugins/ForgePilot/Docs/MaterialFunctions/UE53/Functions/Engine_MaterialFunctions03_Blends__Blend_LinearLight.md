# Blend_LinearLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_LinearLight.Blend_LinearLight`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Linear version of Overlay blend mode, harsh results.

## Inputs

- `Blend` [FunctionInput_Vector3] - Blend texture input
- `Base` [FunctionInput_Vector3] - Base texture input

## Outputs

- `Result` - Linear version of Overlay blend mode, harsh results.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionAppendVector` | Append |  |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionConstant` | -0.25 |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Blend (Vector3) | Blend |
| 9 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 10 | expression | `MaterialExpressionConstant` | 2 |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Base (Vector3) | Base |
| 12 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 13 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionAdd` | Add |  |
| 17 | expression | `MaterialExpressionConstant` | -0.125 |  |
| 18 | expression | `MaterialExpressionConstant` | 1.25 |  |
| 19 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 21 | expression | `MaterialExpressionAppendVector` | Append |  |
| 22 | expression | `MaterialExpressionIf` | If |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 24 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 25 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 26 | expression | `MaterialExpressionIf` | If |  |
| 27 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 28 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 29 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Append output 0

### [1] Append

- `A` <= [21] Append output 0
- `B` <= [2] If output 0

### [2] If

- `A` <= [20] Mask ( B ) output 0
- `B` <= [19] 0.5 output 0
- `AGreaterThanB` <= [13] Mask ( B ) output 0
- `ALessThanB` <= [3] Mask ( B ) output 0

### [3] Mask ( B )

- `Input` <= [4] Add output 0

### [4] Add

- `A` <= [11] Input Base (Vector3) output 0
- `B` <= [5] Multiply output 0

### [5] Multiply

- `A` <= [10] 2 output 0
- `B` <= [6] Add output 0

### [6] Add

- `A` <= [8] Input Blend (Vector3) output 0
- `B` <= [7] -0.25 output 0

### [8] Input Blend (Vector3)

- `Preview` <= [9] Texture Sample output 0

### [11] Input Base (Vector3)

- `Preview` <= [12] Texture Sample output 0

### [13] Mask ( B )

- `Input` <= [14] Add output 0

### [14] Add

- `A` <= [11] Input Base (Vector3) output 0
- `B` <= [15] Multiply output 0

### [15] Multiply

- `A` <= [18] 1.25 output 0
- `B` <= [16] Add output 0

### [16] Add

- `A` <= [17] -0.125 output 0
- `B` <= [8] Input Blend (Vector3) output 0

### [20] Mask ( B )

- `Input` <= [8] Input Blend (Vector3) output 0

### [21] Append

- `A` <= [26] If output 0
- `B` <= [22] If output 0

### [22] If

- `A` <= [25] Mask ( G ) output 0
- `B` <= [19] 0.5 output 0
- `AGreaterThanB` <= [24] Mask ( G ) output 0
- `ALessThanB` <= [23] Mask ( G ) output 0

### [23] Mask ( G )

- `Input` <= [4] Add output 0

### [24] Mask ( G )

- `Input` <= [14] Add output 0

### [25] Mask ( G )

- `Input` <= [8] Input Blend (Vector3) output 0

### [26] If

- `A` <= [29] Mask ( R ) output 0
- `B` <= [19] 0.5 output 0
- `AGreaterThanB` <= [28] Mask ( R ) output 0
- `ALessThanB` <= [27] Mask ( R ) output 0

### [27] Mask ( R )

- `Input` <= [4] Add output 0

### [28] Mask ( R )

- `Input` <= [14] Add output 0

### [29] Mask ( R )

- `Input` <= [8] Input Blend (Vector3) output 0

## Connection List

- [1] Append output 0 -> Output Result.A
- [21] Append output 0 -> Append.A
- [2] If output 0 -> Append.B
- [20] Mask ( B ) output 0 -> If.A
- [19] 0.5 output 0 -> If.B
- [13] Mask ( B ) output 0 -> If.AGreaterThanB
- [3] Mask ( B ) output 0 -> If.ALessThanB
- [4] Add output 0 -> Mask ( B ).Input
- [11] Input Base (Vector3) output 0 -> Add.A
- [5] Multiply output 0 -> Add.B
- [10] 2 output 0 -> Multiply.A
- [6] Add output 0 -> Multiply.B
- [8] Input Blend (Vector3) output 0 -> Add.A
- [7] -0.25 output 0 -> Add.B
- [9] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [12] Texture Sample output 0 -> Input Base (Vector3).Preview
- [14] Add output 0 -> Mask ( B ).Input
- [11] Input Base (Vector3) output 0 -> Add.A
- [15] Multiply output 0 -> Add.B
- [18] 1.25 output 0 -> Multiply.A
- [16] Add output 0 -> Multiply.B
- [17] -0.125 output 0 -> Add.A
- [8] Input Blend (Vector3) output 0 -> Add.B
- [8] Input Blend (Vector3) output 0 -> Mask ( B ).Input
- [26] If output 0 -> Append.A
- [22] If output 0 -> Append.B
- [25] Mask ( G ) output 0 -> If.A
- [19] 0.5 output 0 -> If.B
- [24] Mask ( G ) output 0 -> If.AGreaterThanB
- [23] Mask ( G ) output 0 -> If.ALessThanB
- [4] Add output 0 -> Mask ( G ).Input
- [14] Add output 0 -> Mask ( G ).Input
- [8] Input Blend (Vector3) output 0 -> Mask ( G ).Input
- [29] Mask ( R ) output 0 -> If.A
- [19] 0.5 output 0 -> If.B
- [28] Mask ( R ) output 0 -> If.AGreaterThanB
- [27] Mask ( R ) output 0 -> If.ALessThanB
- [4] Add output 0 -> Mask ( R ).Input
- [14] Add output 0 -> Mask ( R ).Input
- [8] Input Blend (Vector3) output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Result` is driven by [1] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
