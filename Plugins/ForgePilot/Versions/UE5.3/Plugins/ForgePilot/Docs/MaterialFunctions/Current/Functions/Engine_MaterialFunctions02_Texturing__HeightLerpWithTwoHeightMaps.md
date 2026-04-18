# HeightLerpWithTwoHeightMaps

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/HeightLerpWithTwoHeightMaps.HeightLerpWithTwoHeightMaps`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Lerp between two values or textures based off of two height map and a transition value.

## Inputs

- `Transistion Phase` [FunctionInput_Scalar] - Enter a value between 0-1.
- `Height Texture 1` [FunctionInput_Scalar] - Enter a grayscale heightmap with values between 0-1.
- `Diffuse 1` [FunctionInput_Vector3] - Enter a color or texture. This texture Acts as the base.
- `Diffuse 2` [FunctionInput_Vector3] - Enter a color or texture. This texture is exposed as the Transistion Phase increases.
- `Contrast` [FunctionInput_Scalar] - Uses cheap contrast function and adds one instruction.
- `Height Texture 2` [FunctionInput_Scalar] - Enter a grayscale heightmap with values between 0-1.

## Outputs

- `Results` - Lerped result of value a and b based off of the input heightmap and transition value.
- `Alpha` - Value used in lerp function. Positive 1 lerps to texture 1
- `Lerp Alpha No Contrast` - Value used in lerp function. Positive 1 lerps to texture 1.
- `Resulting Height Map` - Value used in lerp function. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Results | Results |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Transistion Phase (Scalar) | Transistion Phase |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Height Texture 1 (Scalar) | Height Texture 1 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse 1 (Vector3) | Diffuse 1 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input Diffuse 2 (Vector3) | Diffuse 2 |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 7 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output Lerp Alpha No Contrast | Lerp Alpha No Contrast |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 11 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 12 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply(2,) |  |
| 15 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 16 | expression | `MaterialExpressionClamp` | Clamp |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Height Texture 2 (Scalar) | Height Texture 2 |
| 20 | function_output | `MaterialExpressionFunctionOutput` | Output Resulting Height Map | Resulting Height Map |
| 21 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |

## Exact Input Wiring

### [0] Output Results

- `A` <= [12] Lerp output 0

### [5] Output Alpha

- `A` <= [17] Mask ( R ) output 0

### [7] Mask ( R )

- `Input` <= [16] Clamp output 0

### [8] Output Lerp Alpha No Contrast

- `A` <= [7] Mask ( R ) output 0

### [9] Add

- `A` <= [2] Input Height Texture 1 (Scalar) output 0
- `B` <= [11] 1-x output 0

### [10] Add(,1)

- `A` <= [9] Add output 0

### [11] 1-x

- `Input` <= [1] Input Transistion Phase (Scalar) output 0

### [12] Lerp

- `A` <= [4] Input Diffuse 2 (Vector3) output 0
- `B` <= [3] Input Diffuse 1 (Vector3) output 0
- `Alpha` <= [17] Mask ( R ) output 0

### [13] Add

- `A` <= [14] Multiply(2,) output 0
- `B` <= [19] Input Height Texture 2 (Scalar) output 0

### [14] Multiply(2,)

- `B` <= [1] Input Transistion Phase (Scalar) output 0

### [15] Subtract

- `A` <= [10] Add(,1) output 0
- `B` <= [13] Add output 0

### [16] Clamp

- `Input` <= [15] Subtract output 0

### [17] Mask ( R )

- `Input` <= [18] CheapContrast output 0

### [20] Output Resulting Height Map

- `A` <= [21] Lerp output 0

### [21] Lerp

- `A` <= [19] Input Height Texture 2 (Scalar) output 0
- `B` <= [2] Input Height Texture 1 (Scalar) output 0
- `Alpha` <= [17] Mask ( R ) output 0

## Connection List

- [12] Lerp output 0 -> Output Results.A
- [17] Mask ( R ) output 0 -> Output Alpha.A
- [16] Clamp output 0 -> Mask ( R ).Input
- [7] Mask ( R ) output 0 -> Output Lerp Alpha No Contrast.A
- [2] Input Height Texture 1 (Scalar) output 0 -> Add.A
- [11] 1-x output 0 -> Add.B
- [9] Add output 0 -> Add(,1).A
- [1] Input Transistion Phase (Scalar) output 0 -> 1-x.Input
- [4] Input Diffuse 2 (Vector3) output 0 -> Lerp.A
- [3] Input Diffuse 1 (Vector3) output 0 -> Lerp.B
- [17] Mask ( R ) output 0 -> Lerp.Alpha
- [14] Multiply(2,) output 0 -> Add.A
- [19] Input Height Texture 2 (Scalar) output 0 -> Add.B
- [1] Input Transistion Phase (Scalar) output 0 -> Multiply(2,).B
- [10] Add(,1) output 0 -> Subtract.A
- [13] Add output 0 -> Subtract.B
- [15] Subtract output 0 -> Clamp.Input
- [18] CheapContrast output 0 -> Mask ( R ).Input
- [21] Lerp output 0 -> Output Resulting Height Map.A
- [19] Input Height Texture 2 (Scalar) output 0 -> Lerp.A
- [2] Input Height Texture 1 (Scalar) output 0 -> Lerp.B
- [17] Mask ( R ) output 0 -> Lerp.Alpha

## Output Trace Roots

- `Results` is driven by [12] Lerp
- `Alpha` is driven by [17] Mask ( R )
- `Lerp Alpha No Contrast` is driven by [7] Mask ( R )
- `Resulting Height Map` is driven by [21] Lerp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


