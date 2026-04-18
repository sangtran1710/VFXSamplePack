# HeightLerp

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Texturing/HeightLerp.HeightLerp`
- Category: Engine_MaterialFunctions02/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Lerp between two values or textures based off of a height map and a transition value.

## Inputs

- `Transition Phase` [FunctionInput_Scalar] - Enter a value between 0-1.
- `Height Texture` [FunctionInput_Scalar] - Enter a grayscale heightmap with values between 0-1.
- `A` [FunctionInput_Vector3] - Enter a color or texture. This texture Acts as the base.
- `B` [FunctionInput_Vector3] - Enter a color or texture. This texture is exposed as the Transistion Phase increases.
- `Contrast` [FunctionInput_Scalar] - Uses cheap contrast function and adds one instruction.

## Outputs

- `Results` - Lerped result of value a and b based off of the input heightmap and transition value.
- `Alpha` - Value used in lerp function. 
- `Lerp Alpha No Contrast` - Value used in lerp function. 

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Results | Results |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 3 | expression | `MaterialExpressionConstant` | 1 |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionConstant` | 2 |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Transition Phase (Scalar) | Transition Phase |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Height Texture (Scalar) | Height Texture |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input A (Vector3) | A |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input B (Vector3) | B |
| 11 | function_output | `MaterialExpressionFunctionOutput` | Output Alpha | Alpha |
| 12 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Contrast (Scalar) | Contrast |
| 14 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 15 | expression | `MaterialExpressionClamp` | Clamp |  |
| 16 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 17 | function_output | `MaterialExpressionFunctionOutput` | Output Lerp Alpha No Contrast | Lerp Alpha No Contrast |

## Exact Input Wiring

### [0] Output Results

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [9] Input A (Vector3) output 0
- `B` <= [10] Input B (Vector3) output 0
- `Alpha` <= [14] CheapContrast output 0

### [2] Subtract

- `A` <= [7] Input Height Texture (Scalar) output 0
- `B` <= [3] 1 output 0

### [4] Multiply

- `A` <= [6] Input Transition Phase (Scalar) output 0
- `B` <= [5] 2 output 0

### [8] Add

- `A` <= [2] Subtract output 0
- `B` <= [4] Multiply output 0

### [11] Output Alpha

- `A` <= [12] Mask ( R ) output 0

### [12] Mask ( R )

- `Input` <= [14] CheapContrast output 0

### [15] Clamp

- `Input` <= [8] Add output 0

### [16] Mask ( R )

- `Input` <= [15] Clamp output 0

### [17] Output Lerp Alpha No Contrast

- `A` <= [16] Mask ( R ) output 0

## Connection List

- [1] Lerp output 0 -> Output Results.A
- [9] Input A (Vector3) output 0 -> Lerp.A
- [10] Input B (Vector3) output 0 -> Lerp.B
- [14] CheapContrast output 0 -> Lerp.Alpha
- [7] Input Height Texture (Scalar) output 0 -> Subtract.A
- [3] 1 output 0 -> Subtract.B
- [6] Input Transition Phase (Scalar) output 0 -> Multiply.A
- [5] 2 output 0 -> Multiply.B
- [2] Subtract output 0 -> Add.A
- [4] Multiply output 0 -> Add.B
- [12] Mask ( R ) output 0 -> Output Alpha.A
- [14] CheapContrast output 0 -> Mask ( R ).Input
- [8] Add output 0 -> Clamp.Input
- [15] Clamp output 0 -> Mask ( R ).Input
- [16] Mask ( R ) output 0 -> Output Lerp Alpha No Contrast.A

## Output Trace Roots

- `Results` is driven by [1] Lerp
- `Alpha` is driven by [12] Mask ( R )
- `Lerp Alpha No Contrast` is driven by [16] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
