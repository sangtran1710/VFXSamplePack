# ViewAlignedReflection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/ViewAlignedReflection.ViewAlignedReflection`
- Category: Engine_MaterialFunctions01/Reflections
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Alignes a Reflection texture to your view.  Reflection should be a sphere.

## Inputs

- `ReflectionTexture` [FunctionInput_Texture2D] - Reflection should look like a sphere
- `ReflectionVector` [FunctionInput_Vector3]

## Outputs

- `Texture`
- `UVs` - Reflection UVs

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Texture | Texture |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input ReflectionTexture (Texture2D) | ReflectionTexture |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input ReflectionVector (Vector3) | ReflectionVector |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 5 | expression | `MaterialExpressionAdd` | Add |  |
| 6 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 7 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 8 | function_output | `MaterialExpressionFunctionOutput` | Output UVs | UVs |
| 9 | expression | `MaterialExpressionTransform` | World Space to View Space TransformVector |  |
| 10 | expression | `MaterialExpressionReflectionVectorWS` | Reflection Vector |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat3Components |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 13 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output Texture

- `A` <= [6] Texture Sample output 0

### [1] Input ReflectionTexture (Texture2D)

- `Preview` <= [7] Texture Object output 0

### [2] Input ReflectionVector (Vector3)

- `Preview` <= [9] World Space to View Space TransformVector output 0

### [3] Multiply

- `A` <= [13] Append output 0
- `B` <= [4] 0.5 output 0

### [5] Add

- `A` <= [3] Multiply output 0
- `B` <= [4] 0.5 output 0

### [6] Texture Sample

- `Coordinates` <= [5] Add output 0
- `TextureObject` <= [1] Input ReflectionTexture (Texture2D) output 0

### [8] Output UVs

- `A` <= [5] Add output 0

### [9] World Space to View Space TransformVector

- `Input` <= [10] Reflection Vector output 0

### [12] Multiply(,-1)

- `A` <= [11] BreakOutFloat3Components output 1

### [13] Append

- `A` <= [11] BreakOutFloat3Components output 0
- `B` <= [12] Multiply(,-1) output 0

## Connection List

- [6] Texture Sample output 0 -> Output Texture.A
- [7] Texture Object output 0 -> Input ReflectionTexture (Texture2D).Preview
- [9] World Space to View Space TransformVector output 0 -> Input ReflectionVector (Vector3).Preview
- [13] Append output 0 -> Multiply.A
- [4] 0.5 output 0 -> Multiply.B
- [3] Multiply output 0 -> Add.A
- [4] 0.5 output 0 -> Add.B
- [5] Add output 0 -> Texture Sample.Coordinates
- [1] Input ReflectionTexture (Texture2D) output 0 -> Texture Sample.TextureObject
- [5] Add output 0 -> Output UVs.A
- [10] Reflection Vector output 0 -> World Space to View Space TransformVector.Input
- [11] BreakOutFloat3Components output 1 -> Multiply(,-1).A
- [11] BreakOutFloat3Components output 0 -> Append.A
- [12] Multiply(,-1) output 0 -> Append.B

## Output Trace Roots

- `Texture` is driven by [6] Texture Sample
- `UVs` is driven by [5] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


