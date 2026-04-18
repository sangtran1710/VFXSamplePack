# ObjectSpaceFalloff

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/ObjectSpaceFalloff.ObjectSpaceFalloff`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Derives a falloff based on object's location.

## Inputs

- `Falloff Hardness` [FunctionInput_Scalar] - 0 - 100 defines sharpness of falloff.
- `Aspect Ratio - V` [FunctionInput_Scalar] - Bias V shape of results.
- `Aspect Ratio - U` [FunctionInput_Scalar] - Bias U shape of results.
- `Invert 2nd channel` [FunctionInput_StaticBool] - Swap between using B and G for 2nd coordinate for spheremask.
- `Falloff Scale` [FunctionInput_Scalar]
- `Offset - V` [FunctionInput_Scalar]
- `Offset - U` [FunctionInput_Scalar]
- `Invert 1st Channel` [FunctionInput_StaticBool] - Swap between using R and G for 2nd coordinate for spheremask.
- `Projection Transform Switch` [FunctionInput_StaticBool] - If true, ignores object's local transform when generating results.
- `Offset Transform Switch` [FunctionInput_StaticBool] - If true, ignores object's local transform when applying offsets.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionConstant` | 100 |  |
| 4 | expression | `MaterialExpressionClamp` | Clamp |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Falloff Hardness (Scalar) | Falloff Hardness |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | expression | `MaterialExpressionAppendVector` | Append |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Aspect Ratio - V (Scalar) | Aspect Ratio - V |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input Aspect Ratio - U (Scalar) | Aspect Ratio - U |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 12 | function_input | `MaterialExpressionFunctionInput` | Input Invert 2nd channel (StaticBool) | Invert 2nd channel |
| 13 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 14 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 15 | expression | `MaterialExpressionTransform` | World Space to Local Space TransformVector |  |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Falloff Scale (Scalar) | Falloff Scale |
| 18 | expression | `MaterialExpressionConstant` | 64 |  |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | expression | `MaterialExpressionTransform` | Local Space to World Space TransformVector |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionConstant` | 2 |  |
| 24 | expression | `MaterialExpressionAppendVector` | Append |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Offset - V (Scalar) | Offset - V |
| 26 | expression | `MaterialExpressionAppendVector` | Append |  |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Offset - U (Scalar) | Offset - U |
| 28 | expression | `MaterialExpressionConstant` | 0 |  |
| 29 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 30 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 31 | expression | `MaterialExpressionObjectPositionWS` | Object Position (Absolute) |  |
| 32 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 33 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 34 | function_input | `MaterialExpressionFunctionInput` | Input Invert 1st Channel (StaticBool) | Invert 1st Channel |
| 35 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 36 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 37 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 38 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 39 | function_input | `MaterialExpressionFunctionInput` | Input Projection Transform Switch (StaticBool) | Projection Transform Switch |
| 40 | expression | `MaterialExpressionConstant` | 0 |  |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input Offset Transform Switch (StaticBool) | Offset Transform Switch |
| 42 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 43 | expression | `MaterialExpressionStaticSwitch` | Switch |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] SphereMask output 0

### [1] SphereMask

- `A` <= [6] Multiply output 0
- `B` <= [40] 0 output 0
- `Hardness` <= [2] Divide output 0

### [2] Divide

- `A` <= [4] Clamp output 0
- `B` <= [3] 100 output 0

### [4] Clamp

- `Input` <= [5] Input Falloff Hardness (Scalar) output 0

### [6] Multiply

- `A` <= [10] Append output 0
- `B` <= [7] Append output 0

### [7] Append

- `A` <= [9] Input Aspect Ratio - U (Scalar) output 0
- `B` <= [8] Input Aspect Ratio - V (Scalar) output 0

### [10] Append

- `A` <= [33] Switch output 0
- `B` <= [11] Switch output 0

### [11] Switch

- `A` <= [32] Mask ( G ) output 0
- `B` <= [14] Mask ( B ) output 0
- `Value` <= [12] Input Invert 2nd channel (StaticBool) output 0

### [12] Input Invert 2nd channel (StaticBool)

- `Preview` <= [13] Static Bool (False) output 0

### [14] Mask ( B )

- `Input` <= [37] Switch output 0

### [15] World Space to Local Space TransformVector

- `Input` <= [16] Divide output 0

### [16] Divide

- `A` <= [19] Add output 0
- `B` <= [17] Input Falloff Scale (Scalar) output 0

### [17] Input Falloff Scale (Scalar)

- `Preview` <= [18] 64 output 0

### [19] Add

- `A` <= [29] Subtract output 0
- `B` <= [43] Switch output 0

### [20] Local Space to World Space TransformVector

- `Input` <= [21] Multiply output 0

### [21] Multiply

- `A` <= [22] Multiply output 0
- `B` <= [17] Input Falloff Scale (Scalar) output 0

### [22] Multiply

- `A` <= [24] Append output 0
- `B` <= [23] 2 output 0

### [24] Append

- `A` <= [26] Append output 0
- `B` <= [25] Input Offset - V (Scalar) output 0

### [26] Append

- `A` <= [28] 0 output 0
- `B` <= [27] Input Offset - U (Scalar) output 0

### [29] Subtract

- `A` <= [31] Object Position (Absolute) output 0
- `B` <= [30] Absolute World Position output 0

### [32] Mask ( G )

- `Input` <= [37] Switch output 0

### [33] Switch

- `A` <= [36] Mask ( R ) output 0
- `B` <= [35] Mask ( G ) output 0
- `Value` <= [34] Input Invert 1st Channel (StaticBool) output 0

### [34] Input Invert 1st Channel (StaticBool)

- `Preview` <= [13] Static Bool (False) output 0

### [35] Mask ( G )

- `Input` <= [37] Switch output 0

### [36] Mask ( R )

- `Input` <= [37] Switch output 0

### [37] Switch

- `A` <= [16] Divide output 0
- `B` <= [15] World Space to Local Space TransformVector output 0
- `Value` <= [39] Input Projection Transform Switch (StaticBool) output 0

### [39] Input Projection Transform Switch (StaticBool)

- `Preview` <= [38] Static Bool (False) output 0

### [41] Input Offset Transform Switch (StaticBool)

- `Preview` <= [42] Static Bool (False) output 0

### [43] Switch

- `A` <= [21] Multiply output 0
- `B` <= [20] Local Space to World Space TransformVector output 0
- `Value` <= [41] Input Offset Transform Switch (StaticBool) output 0

## Connection List

- [1] SphereMask output 0 -> Output Result.A
- [6] Multiply output 0 -> SphereMask.A
- [40] 0 output 0 -> SphereMask.B
- [2] Divide output 0 -> SphereMask.Hardness
- [4] Clamp output 0 -> Divide.A
- [3] 100 output 0 -> Divide.B
- [5] Input Falloff Hardness (Scalar) output 0 -> Clamp.Input
- [10] Append output 0 -> Multiply.A
- [7] Append output 0 -> Multiply.B
- [9] Input Aspect Ratio - U (Scalar) output 0 -> Append.A
- [8] Input Aspect Ratio - V (Scalar) output 0 -> Append.B
- [33] Switch output 0 -> Append.A
- [11] Switch output 0 -> Append.B
- [32] Mask ( G ) output 0 -> Switch.A
- [14] Mask ( B ) output 0 -> Switch.B
- [12] Input Invert 2nd channel (StaticBool) output 0 -> Switch.Value
- [13] Static Bool (False) output 0 -> Input Invert 2nd channel (StaticBool).Preview
- [37] Switch output 0 -> Mask ( B ).Input
- [16] Divide output 0 -> World Space to Local Space TransformVector.Input
- [19] Add output 0 -> Divide.A
- [17] Input Falloff Scale (Scalar) output 0 -> Divide.B
- [18] 64 output 0 -> Input Falloff Scale (Scalar).Preview
- [29] Subtract output 0 -> Add.A
- [43] Switch output 0 -> Add.B
- [21] Multiply output 0 -> Local Space to World Space TransformVector.Input
- [22] Multiply output 0 -> Multiply.A
- [17] Input Falloff Scale (Scalar) output 0 -> Multiply.B
- [24] Append output 0 -> Multiply.A
- [23] 2 output 0 -> Multiply.B
- [26] Append output 0 -> Append.A
- [25] Input Offset - V (Scalar) output 0 -> Append.B
- [28] 0 output 0 -> Append.A
- [27] Input Offset - U (Scalar) output 0 -> Append.B
- [31] Object Position (Absolute) output 0 -> Subtract.A
- [30] Absolute World Position output 0 -> Subtract.B
- [37] Switch output 0 -> Mask ( G ).Input
- [36] Mask ( R ) output 0 -> Switch.A
- [35] Mask ( G ) output 0 -> Switch.B
- [34] Input Invert 1st Channel (StaticBool) output 0 -> Switch.Value
- [13] Static Bool (False) output 0 -> Input Invert 1st Channel (StaticBool).Preview
- [37] Switch output 0 -> Mask ( G ).Input
- [37] Switch output 0 -> Mask ( R ).Input
- [16] Divide output 0 -> Switch.A
- [15] World Space to Local Space TransformVector output 0 -> Switch.B
- [39] Input Projection Transform Switch (StaticBool) output 0 -> Switch.Value
- [38] Static Bool (False) output 0 -> Input Projection Transform Switch (StaticBool).Preview
- [42] Static Bool (False) output 0 -> Input Offset Transform Switch (StaticBool).Preview
- [21] Multiply output 0 -> Switch.A
- [20] Local Space to World Space TransformVector output 0 -> Switch.B
- [41] Input Offset Transform Switch (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `Result` is driven by [1] SphereMask

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
