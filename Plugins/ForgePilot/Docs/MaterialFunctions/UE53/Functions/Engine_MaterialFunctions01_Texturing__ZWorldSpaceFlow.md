# ZWorldSpaceFlow

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/ZWorldSpaceFlow.ZWorldSpaceFlow`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Pushing the FlowTexture along the direction of the FlowDirection

## Inputs

- `FlowSpeed` [FunctionInput_Scalar]
- `FlowTexture` [FunctionInput_Texture2D]
- `FlowStrength` [FunctionInput_Scalar]
- `FlowDirection` [FunctionInput_Vector2] - Flow Direction in Tangent Space from -1 to 1
- `TextureSize` [FunctionInput_Vector3] - Size of texture in worldspace units
- `WorldPosition` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 9 | expression | `MaterialExpressionConstant` | 1 |  |
| 10 | expression | `MaterialExpressionAbs` | Abs |  |
| 11 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 12 | expression | `MaterialExpressionFrac` | Frac |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input FlowSpeed (Scalar) | FlowSpeed |
| 16 | expression | `MaterialExpressionTime` | Time |  |
| 17 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input FlowTexture (Texture2D) | FlowTexture |
| 19 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 20 | expression | `MaterialExpressionAdd` | Add |  |
| 21 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 22 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 23 | expression | `MaterialExpressionFrac` | Frac |  |
| 24 | expression | `MaterialExpressionAdd` | Add |  |
| 25 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input FlowStrength (Scalar) | FlowStrength |
| 27 | expression | `MaterialExpressionConstant` | 1 |  |
| 28 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 29 | expression | `MaterialExpressionConstant2Vector` | -1,-1 |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input FlowDirection (Vector2) | FlowDirection |
| 31 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 32 | expression | `MaterialExpressionConstant3Vector` | 0,1,0 |  |
| 33 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 34 | expression | `MaterialExpressionDivide` | Divide |  |
| 35 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 36 | function_input | `MaterialExpressionFunctionInput` | Input TextureSize (Vector3) | TextureSize |
| 37 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 38 | expression | `MaterialExpressionWorldPosition` | Absolute World Position |  |
| 39 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 40 | expression | `MaterialExpressionAdd` | Add |  |
| 41 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 42 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 43 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 44 | expression | `MaterialExpressionConstant3Vector` | -1,-1,1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [43] Multiply output 0

### [1] Lerp

- `A` <= [39] Texture Sample output 0
- `B` <= [17] Texture Sample output 0
- `Alpha` <= [2] 1-x output 0

### [2] 1-x

- `Input` <= [3] Multiply output 0

### [3] Multiply

- `A` <= [4] 1-x output 0
- `B` <= [4] 1-x output 0

### [4] 1-x

- `Input` <= [5] Multiply output 0

### [5] Multiply

- `A` <= [6] Divide output 0
- `B` <= [6] Divide output 0

### [6] Divide

- `A` <= [10] Abs output 0
- `B` <= [7] Multiply output 0

### [7] Multiply

- `A` <= [9] 1 output 0
- `B` <= [8] 0.5 output 0

### [10] Abs

- `Input` <= [11] Subtract output 0

### [11] Subtract

- `A` <= [12] Frac output 0
- `B` <= [7] Multiply output 0

### [12] Frac

- `Input` <= [13] Multiply output 0

### [13] Multiply

- `A` <= [14] Multiply output 0
- `B` <= [9] 1 output 0

### [14] Multiply

- `A` <= [16] Time output 0
- `B` <= [15] Input FlowSpeed (Scalar) output 0

### [17] Texture Sample

- `Coordinates` <= [20] Add output 0
- `TextureObject` <= [18] Input FlowTexture (Texture2D) output 0

### [18] Input FlowTexture (Texture2D)

- `Preview` <= [19] Texture Object output 0

### [20] Add

- `A` <= [33] Mask ( R G ) output 0
- `B` <= [21] Multiply output 0

### [21] Multiply

- `A` <= [28] Multiply output 0
- `B` <= [22] Multiply output 0

### [22] Multiply

- `A` <= [25] Multiply output 0
- `B` <= [23] Frac output 0

### [23] Frac

- `Input` <= [24] Add output 0

### [24] Add

- `A` <= [13] Multiply output 0
- `B` <= [7] Multiply output 0

### [25] Multiply

- `A` <= [27] 1 output 0
- `B` <= [26] Input FlowStrength (Scalar) output 0

### [28] Multiply

- `A` <= [30] Input FlowDirection (Vector2) output 0
- `B` <= [29] -1,-1 output 0

### [30] Input FlowDirection (Vector2)

- `Preview` <= [31] Mask ( R G ) output 0

### [31] Mask ( R G )

- `Input` <= [32] 0,1,0 output 0

### [33] Mask ( R G )

- `Input` <= [34] Divide output 0

### [34] Divide

- `A` <= [37] Input WorldPosition (Vector3) output 0
- `B` <= [35] Multiply(,-1) output 0

### [35] Multiply(,-1)

- `A` <= [36] Input TextureSize (Vector3) output 0

### [37] Input WorldPosition (Vector3)

- `Preview` <= [38] Absolute World Position output 0

### [39] Texture Sample

- `Coordinates` <= [40] Add output 0
- `TextureObject` <= [18] Input FlowTexture (Texture2D) output 0

### [40] Add

- `A` <= [33] Mask ( R G ) output 0
- `B` <= [41] Multiply output 0

### [41] Multiply

- `A` <= [28] Multiply output 0
- `B` <= [42] Multiply output 0

### [42] Multiply

- `A` <= [12] Frac output 0
- `B` <= [25] Multiply output 0

### [43] Multiply

- `A` <= [1] Lerp output 0
- `B` <= [44] -1,-1,1 output 0

## Connection List

- [43] Multiply output 0 -> Output Result.A
- [39] Texture Sample output 0 -> Lerp.A
- [17] Texture Sample output 0 -> Lerp.B
- [2] 1-x output 0 -> Lerp.Alpha
- [3] Multiply output 0 -> 1-x.Input
- [4] 1-x output 0 -> Multiply.A
- [4] 1-x output 0 -> Multiply.B
- [5] Multiply output 0 -> 1-x.Input
- [6] Divide output 0 -> Multiply.A
- [6] Divide output 0 -> Multiply.B
- [10] Abs output 0 -> Divide.A
- [7] Multiply output 0 -> Divide.B
- [9] 1 output 0 -> Multiply.A
- [8] 0.5 output 0 -> Multiply.B
- [11] Subtract output 0 -> Abs.Input
- [12] Frac output 0 -> Subtract.A
- [7] Multiply output 0 -> Subtract.B
- [13] Multiply output 0 -> Frac.Input
- [14] Multiply output 0 -> Multiply.A
- [9] 1 output 0 -> Multiply.B
- [16] Time output 0 -> Multiply.A
- [15] Input FlowSpeed (Scalar) output 0 -> Multiply.B
- [20] Add output 0 -> Texture Sample.Coordinates
- [18] Input FlowTexture (Texture2D) output 0 -> Texture Sample.TextureObject
- [19] Texture Object output 0 -> Input FlowTexture (Texture2D).Preview
- [33] Mask ( R G ) output 0 -> Add.A
- [21] Multiply output 0 -> Add.B
- [28] Multiply output 0 -> Multiply.A
- [22] Multiply output 0 -> Multiply.B
- [25] Multiply output 0 -> Multiply.A
- [23] Frac output 0 -> Multiply.B
- [24] Add output 0 -> Frac.Input
- [13] Multiply output 0 -> Add.A
- [7] Multiply output 0 -> Add.B
- [27] 1 output 0 -> Multiply.A
- [26] Input FlowStrength (Scalar) output 0 -> Multiply.B
- [30] Input FlowDirection (Vector2) output 0 -> Multiply.A
- [29] -1,-1 output 0 -> Multiply.B
- [31] Mask ( R G ) output 0 -> Input FlowDirection (Vector2).Preview
- [32] 0,1,0 output 0 -> Mask ( R G ).Input
- [34] Divide output 0 -> Mask ( R G ).Input
- [37] Input WorldPosition (Vector3) output 0 -> Divide.A
- [35] Multiply(,-1) output 0 -> Divide.B
- [36] Input TextureSize (Vector3) output 0 -> Multiply(,-1).A
- [38] Absolute World Position output 0 -> Input WorldPosition (Vector3).Preview
- [40] Add output 0 -> Texture Sample.Coordinates
- [18] Input FlowTexture (Texture2D) output 0 -> Texture Sample.TextureObject
- [33] Mask ( R G ) output 0 -> Add.A
- [41] Multiply output 0 -> Add.B
- [28] Multiply output 0 -> Multiply.A
- [42] Multiply output 0 -> Multiply.B
- [12] Frac output 0 -> Multiply.A
- [25] Multiply output 0 -> Multiply.B
- [1] Lerp output 0 -> Multiply.A
- [44] -1,-1,1 output 0 -> Multiply.B

## Output Trace Roots

- `Result` is driven by [43] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
