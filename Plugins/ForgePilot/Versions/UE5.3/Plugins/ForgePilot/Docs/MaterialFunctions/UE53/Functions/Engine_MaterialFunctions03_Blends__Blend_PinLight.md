# Blend_PinLight

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_PinLight.Blend_PinLight`
- Category: Engine_MaterialFunctions03/Blends
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Softer version of Overlay, solid white or black as blend will not result in solid color.

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
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionConstant` | 2 |  |
| 14 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 15 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 16 | expression | `MaterialExpressionAppendVector` | Append |  |
| 17 | expression | `MaterialExpressionIf` | If |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 19 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 21 | expression | `MaterialExpressionIf` | If |  |
| 22 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 24 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply(0,) |  |
| 26 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 27 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 28 | expression | `MaterialExpressionAdd` | Add |  |
| 29 | expression | `MaterialExpressionConstant` | -0.5 |  |
| 30 | custom | `MaterialExpressionCustom` | Max |  |
| 31 | custom | `MaterialExpressionCustom` | Min |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Append output 0

### [1] Append

- `A` <= [16] Append output 0
- `B` <= [2] If output 0

### [2] If

- `A` <= [15] Mask ( B ) output 0
- `B` <= [14] 0.5 output 0
- `AGreaterThanB` <= [11] Mask ( B ) output 0
- `ALessThanB` <= [3] Mask ( B ) output 0

### [3] Mask ( B )

- `Input` <= [31] Min output 0

### [4] Multiply

- `A` <= [9] Input Base (Vector3) output 0
- `B` <= [7] Multiply output 0

### [5] Input Blend (Vector3)

- `Preview` <= [6] Texture Sample output 0

### [7] Multiply

- `A` <= [5] Input Blend (Vector3) output 0
- `B` <= [8] 2 output 0

### [9] Input Base (Vector3)

- `Preview` <= [10] Texture Sample output 0

### [11] Mask ( B )

- `Input` <= [30] Max output 0

### [12] Multiply

- `A` <= [28] Add output 0
- `B` <= [13] 2 output 0

### [15] Mask ( B )

- `Input` <= [5] Input Blend (Vector3) output 0

### [16] Append

- `A` <= [21] If output 0
- `B` <= [17] If output 0

### [17] If

- `A` <= [20] Mask ( G ) output 0
- `B` <= [14] 0.5 output 0
- `AGreaterThanB` <= [19] Mask ( G ) output 0
- `ALessThanB` <= [18] Mask ( G ) output 0

### [18] Mask ( G )

- `Input` <= [31] Min output 0

### [19] Mask ( G )

- `Input` <= [30] Max output 0

### [20] Mask ( G )

- `Input` <= [5] Input Blend (Vector3) output 0

### [21] If

- `A` <= [24] Mask ( R ) output 0
- `B` <= [14] 0.5 output 0
- `AGreaterThanB` <= [23] Mask ( R ) output 0
- `ALessThanB` <= [22] Mask ( R ) output 0

### [22] Mask ( R )

- `Input` <= [31] Min output 0

### [23] Mask ( R )

- `Input` <= [30] Max output 0

### [24] Mask ( R )

- `Input` <= [5] Input Blend (Vector3) output 0

### [25] Multiply(0,)

- `B` <= [26] 1-x output 0

### [26] 1-x

- `Input` <= [4] Multiply output 0

### [27] 1-x

- `Input` <= [25] Multiply(0,) output 0

### [28] Add

- `A` <= [5] Input Blend (Vector3) output 0
- `B` <= [29] -0.5 output 0

## Connection List

- [1] Append output 0 -> Output Result.A
- [16] Append output 0 -> Append.A
- [2] If output 0 -> Append.B
- [15] Mask ( B ) output 0 -> If.A
- [14] 0.5 output 0 -> If.B
- [11] Mask ( B ) output 0 -> If.AGreaterThanB
- [3] Mask ( B ) output 0 -> If.ALessThanB
- [31] Min output 0 -> Mask ( B ).Input
- [9] Input Base (Vector3) output 0 -> Multiply.A
- [7] Multiply output 0 -> Multiply.B
- [6] Texture Sample output 0 -> Input Blend (Vector3).Preview
- [5] Input Blend (Vector3) output 0 -> Multiply.A
- [8] 2 output 0 -> Multiply.B
- [10] Texture Sample output 0 -> Input Base (Vector3).Preview
- [30] Max output 0 -> Mask ( B ).Input
- [28] Add output 0 -> Multiply.A
- [13] 2 output 0 -> Multiply.B
- [5] Input Blend (Vector3) output 0 -> Mask ( B ).Input
- [21] If output 0 -> Append.A
- [17] If output 0 -> Append.B
- [20] Mask ( G ) output 0 -> If.A
- [14] 0.5 output 0 -> If.B
- [19] Mask ( G ) output 0 -> If.AGreaterThanB
- [18] Mask ( G ) output 0 -> If.ALessThanB
- [31] Min output 0 -> Mask ( G ).Input
- [30] Max output 0 -> Mask ( G ).Input
- [5] Input Blend (Vector3) output 0 -> Mask ( G ).Input
- [24] Mask ( R ) output 0 -> If.A
- [14] 0.5 output 0 -> If.B
- [23] Mask ( R ) output 0 -> If.AGreaterThanB
- [22] Mask ( R ) output 0 -> If.ALessThanB
- [31] Min output 0 -> Mask ( R ).Input
- [30] Max output 0 -> Mask ( R ).Input
- [5] Input Blend (Vector3) output 0 -> Mask ( R ).Input
- [26] 1-x output 0 -> Multiply(0,).B
- [4] Multiply output 0 -> 1-x.Input
- [25] Multiply(0,) output 0 -> 1-x.Input
- [5] Input Blend (Vector3) output 0 -> Add.A
- [29] -0.5 output 0 -> Add.B

## Output Trace Roots

- `Result` is driven by [1] Append

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
