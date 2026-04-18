# GradientMap_Multi_TexObjSamplerType

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Gradients/GradientMap_Multi_TexObjSamplerType.GradientMap_Multi_TexObjSamplerType`
- Category: Engine_MaterialFunctions02/Gradients
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Maps a 0-1 value to a color gradient slice. See the default textures for example content. (This material function differs from GradientMap_Multi by using the texture's sampler type rather than utilizing shared:wrap)

## Inputs

- `Number Of Gradients In the Gradient Map` [FunctionInput_Scalar]
- `Greyscale Value To Gradient Map` [FunctionInput_Scalar]
- `Index (0 Based)` [FunctionInput_Scalar] - The first index is 0 and the last index is (number of gradients in the gradient map) -1.
- `Gradient Texture` [FunctionInput_Texture2D]

## Outputs

- `RGB`
- `A`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Number Of Gradients In the Gradient Map (Scalar) | Number Of Gradients In the Gradient Map |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Greyscale Value To Gradient Map (Scalar) | Greyscale Value To Gradient Map |
| 3 | expression | `MaterialExpressionConstant` | 1 |  |
| 4 | expression | `MaterialExpressionDivide` | Divide |  |
| 5 | expression | `MaterialExpressionCeil` | Ceil |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Index (0 Based) (Scalar) | Index (0 Based) |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Gradient Texture (Texture2D) | Gradient Texture |
| 12 | expression | `MaterialExpressionCeil` | Ceil |  |
| 13 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 14 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 15 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 16 | function_output | `MaterialExpressionFunctionOutput` | Output A | A |

## Exact Input Wiring

### [0] Output RGB

- `A` <= [13] Texture Sample output 0

### [2] Input Greyscale Value To Gradient Map (Scalar)

- `Preview` <= [15] Texture Sample output 1

### [4] Divide

- `A` <= [3] 1 output 0
- `B` <= [5] Ceil output 0

### [5] Ceil

- `Input` <= [1] Input Number Of Gradients In the Gradient Map (Scalar) output 0

### [6] Multiply(,0.5)

- `A` <= [4] Divide output 0

### [7] Add

- `A` <= [6] Multiply(,0.5) output 0
- `B` <= [9] Multiply output 0

### [9] Multiply

- `A` <= [4] Divide output 0
- `B` <= [12] Ceil output 0

### [10] Append

- `A` <= [2] Input Greyscale Value To Gradient Map (Scalar) output 0
- `B` <= [7] Add output 0

### [11] Input Gradient Texture (Texture2D)

- `Preview` <= [14] Texture Object output 0

### [12] Ceil

- `Input` <= [8] Input Index (0 Based) (Scalar) output 0

### [13] Texture Sample

- `Coordinates` <= [10] Append output 0
- `TextureObject` <= [11] Input Gradient Texture (Texture2D) output 0

### [16] Output A

- `A` <= [13] Texture Sample output 4

## Connection List

- [13] Texture Sample output 0 -> Output RGB.A
- [15] Texture Sample output 1 -> Input Greyscale Value To Gradient Map (Scalar).Preview
- [3] 1 output 0 -> Divide.A
- [5] Ceil output 0 -> Divide.B
- [1] Input Number Of Gradients In the Gradient Map (Scalar) output 0 -> Ceil.Input
- [4] Divide output 0 -> Multiply(,0.5).A
- [6] Multiply(,0.5) output 0 -> Add.A
- [9] Multiply output 0 -> Add.B
- [4] Divide output 0 -> Multiply.A
- [12] Ceil output 0 -> Multiply.B
- [2] Input Greyscale Value To Gradient Map (Scalar) output 0 -> Append.A
- [7] Add output 0 -> Append.B
- [14] Texture Object output 0 -> Input Gradient Texture (Texture2D).Preview
- [8] Input Index (0 Based) (Scalar) output 0 -> Ceil.Input
- [10] Append output 0 -> Texture Sample.Coordinates
- [11] Input Gradient Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [13] Texture Sample output 4 -> Output A.A

## Output Trace Roots

- `RGB` is driven by [13] Texture Sample
- `A` is driven by [13] Texture Sample

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
