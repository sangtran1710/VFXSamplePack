# ApplyDBuffer

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Decal/ApplyDBuffer.ApplyDBuffer`
- Category: Engine_MaterialFunctions01/Decal
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Apply DBuffer directly to material attributes. This replicates the logic automatically applied by the Dbuffer Decal Response material setting. Material normal should be in world space.

## Inputs

- `DBufferA` [FunctionInput_Vector4]
- `DBufferB` [FunctionInput_Vector4]
- `DBufferC` [FunctionInput_Vector4]
- `InMaterial` [FunctionInput_MaterialAttributes]

## Outputs

- `OutMaterial`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output OutMaterial | OutMaterial |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input DBufferA (Vector4) | DBufferA |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input DBufferB (Vector4) | DBufferB |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input DBufferC (Vector4) | DBufferC |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input InMaterial (MaterialAttributes) | InMaterial |
| 5 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 6 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 10 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 12 | expression | `MaterialExpressionAdd` | Add |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | expression | `MaterialExpressionAdd` | Add |  |
| 16 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat4Components |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 21 | expression | `MaterialExpressionAdd` | Add |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionAdd` | Add |  |
| 24 | expression | `MaterialExpressionDBufferTexture` | DBufferA (BaseColor) |  |
| 25 | expression | `MaterialExpressionDBufferTexture` | DBufferB (WorldNormal) |  |
| 26 | expression | `MaterialExpressionDBufferTexture` | DBufferC (Roughness) |  |

## Exact Input Wiring

### [0] Output OutMaterial

- `A` <= [6] SetMaterialAttributes output 0

### [1] Input DBufferA (Vector4)

- `Preview` <= [24] DBufferA (BaseColor) output 0

### [2] Input DBufferB (Vector4)

- `Preview` <= [25] DBufferB (WorldNormal) output 0

### [3] Input DBufferC (Vector4)

- `Preview` <= [26] DBufferC (Roughness) output 0

### [6] SetMaterialAttributes

- `Inputs[0]` <= [4] Input InMaterial (MaterialAttributes) output 0
- `Inputs[1]` <= [12] Add output 0
- `Inputs[2]` <= [13] Multiply output 0
- `Inputs[3]` <= [16] Normalize output 0
- `Inputs[4]` <= [18] Add output 0
- `Inputs[5]` <= [23] Add output 0
- `Inputs[6]` <= [21] Add output 0

### [7] Mask ( A )

- `Input` <= [1] Input DBufferA (Vector4) output 0

### [8] Mask ( R G B )

- `Input` <= [1] Input DBufferA (Vector4) output 0

### [9] Mask ( R G B )

- `Input` <= [2] Input DBufferB (Vector4) output 0

### [10] Mask ( A )

- `Input` <= [2] Input DBufferB (Vector4) output 0

### [11] Multiply

- `A` <= [5] GetMaterialAttributes output 1
- `B` <= [7] Mask ( A ) output 0

### [12] Add

- `A` <= [11] Multiply output 0
- `B` <= [8] Mask ( R G B ) output 0

### [13] Multiply

- `A` <= [5] GetMaterialAttributes output 2
- `B` <= [7] Mask ( A ) output 0

### [14] Multiply

- `A` <= [5] GetMaterialAttributes output 3
- `B` <= [10] Mask ( A ) output 0

### [15] Add

- `A` <= [14] Multiply output 0
- `B` <= [9] Mask ( R G B ) output 0

### [16] Normalize

- `VectorInput` <= [15] Add output 0

### [17] Multiply

- `A` <= [5] GetMaterialAttributes output 4
- `B` <= [19] BreakOutFloat4Components output 3

### [18] Add

- `A` <= [17] Multiply output 0
- `B` <= [19] BreakOutFloat4Components output 1

### [20] Multiply

- `A` <= [5] GetMaterialAttributes output 6
- `B` <= [19] BreakOutFloat4Components output 3

### [21] Add

- `A` <= [20] Multiply output 0
- `B` <= [19] BreakOutFloat4Components output 0

### [22] Multiply

- `A` <= [5] GetMaterialAttributes output 5
- `B` <= [19] BreakOutFloat4Components output 3

### [23] Add

- `A` <= [22] Multiply output 0
- `B` <= [19] BreakOutFloat4Components output 2

## Connection List

- [6] SetMaterialAttributes output 0 -> Output OutMaterial.A
- [24] DBufferA (BaseColor) output 0 -> Input DBufferA (Vector4).Preview
- [25] DBufferB (WorldNormal) output 0 -> Input DBufferB (Vector4).Preview
- [26] DBufferC (Roughness) output 0 -> Input DBufferC (Vector4).Preview
- [4] Input InMaterial (MaterialAttributes) output 0 -> SetMaterialAttributes.Inputs[0]
- [12] Add output 0 -> SetMaterialAttributes.Inputs[1]
- [13] Multiply output 0 -> SetMaterialAttributes.Inputs[2]
- [16] Normalize output 0 -> SetMaterialAttributes.Inputs[3]
- [18] Add output 0 -> SetMaterialAttributes.Inputs[4]
- [23] Add output 0 -> SetMaterialAttributes.Inputs[5]
- [21] Add output 0 -> SetMaterialAttributes.Inputs[6]
- [1] Input DBufferA (Vector4) output 0 -> Mask ( A ).Input
- [1] Input DBufferA (Vector4) output 0 -> Mask ( R G B ).Input
- [2] Input DBufferB (Vector4) output 0 -> Mask ( R G B ).Input
- [2] Input DBufferB (Vector4) output 0 -> Mask ( A ).Input
- [5] GetMaterialAttributes output 1 -> Multiply.A
- [7] Mask ( A ) output 0 -> Multiply.B
- [11] Multiply output 0 -> Add.A
- [8] Mask ( R G B ) output 0 -> Add.B
- [5] GetMaterialAttributes output 2 -> Multiply.A
- [7] Mask ( A ) output 0 -> Multiply.B
- [5] GetMaterialAttributes output 3 -> Multiply.A
- [10] Mask ( A ) output 0 -> Multiply.B
- [14] Multiply output 0 -> Add.A
- [9] Mask ( R G B ) output 0 -> Add.B
- [15] Add output 0 -> Normalize.VectorInput
- [5] GetMaterialAttributes output 4 -> Multiply.A
- [19] BreakOutFloat4Components output 3 -> Multiply.B
- [17] Multiply output 0 -> Add.A
- [19] BreakOutFloat4Components output 1 -> Add.B
- [5] GetMaterialAttributes output 6 -> Multiply.A
- [19] BreakOutFloat4Components output 3 -> Multiply.B
- [20] Multiply output 0 -> Add.A
- [19] BreakOutFloat4Components output 0 -> Add.B
- [5] GetMaterialAttributes output 5 -> Multiply.A
- [19] BreakOutFloat4Components output 3 -> Multiply.B
- [22] Multiply output 0 -> Add.A
- [19] BreakOutFloat4Components output 2 -> Add.B

## Output Trace Roots

- `OutMaterial` is driven by [6] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


