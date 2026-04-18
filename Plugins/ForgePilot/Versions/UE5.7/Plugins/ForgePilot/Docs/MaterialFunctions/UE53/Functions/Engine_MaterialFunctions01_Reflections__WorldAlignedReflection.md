# WorldAlignedReflection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Reflections/WorldAlignedReflection.WorldAlignedReflection`
- Category: Engine_MaterialFunctions01/Reflections
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Alignes a Reflection texture to your view.  Reflection should be a sphere.

## Inputs

- `ReflectionTexture` [FunctionInput_Texture2D] - Reflection should look like a sphere
- `ReflectionVector` [FunctionInput_Vector3]

## Outputs

- `WorldReflection`
- `WorldReflectionShadowed`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output WorldReflection | WorldReflection |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input ReflectionTexture (Texture2D) | ReflectionTexture |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input ReflectionVector (Vector3) | ReflectionVector |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 6 | expression | `MaterialExpressionAdd` | Add |  |
| 7 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 8 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 10 | expression | `MaterialExpressionConstantBiasScale` | ConstantBiasScale |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 12 | expression | `MaterialExpressionConstant` | 2 |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output WorldReflectionShadowed | WorldReflectionShadowed |
| 16 | expression | `MaterialExpressionTextureObject` | Texture Object |  |

## Exact Input Wiring

### [0] Output WorldReflection

- `A` <= [7] Texture Sample output 0

### [1] Input ReflectionTexture (Texture2D)

- `Preview` <= [16] Texture Object output 0

### [3] Mask ( R G )

- `Input` <= [8] Tangent Space to World Space TransformVector output 0

### [4] Multiply

- `A` <= [3] Mask ( R G ) output 0
- `B` <= [5] 0.5 output 0

### [6] Add

- `A` <= [4] Multiply output 0
- `B` <= [5] 0.5 output 0

### [7] Texture Sample

- `Coordinates` <= [6] Add output 0
- `TextureObject` <= [1] Input ReflectionTexture (Texture2D) output 0

### [8] Tangent Space to World Space TransformVector

- `Input` <= [2] Input ReflectionVector (Vector3) output 0

### [9] Mask ( B )

- `Input` <= [8] Tangent Space to World Space TransformVector output 0

### [10] ConstantBiasScale

- `Input` <= [9] Mask ( B ) output 0

### [13] Multiply

- `A` <= [11] CheapContrast output 0
- `B` <= [11] CheapContrast output 0

### [14] Multiply

- `A` <= [7] Texture Sample output 0
- `B` <= [13] Multiply output 0

### [15] Output WorldReflectionShadowed

- `A` <= [14] Multiply output 0

## Connection List

- [7] Texture Sample output 0 -> Output WorldReflection.A
- [16] Texture Object output 0 -> Input ReflectionTexture (Texture2D).Preview
- [8] Tangent Space to World Space TransformVector output 0 -> Mask ( R G ).Input
- [3] Mask ( R G ) output 0 -> Multiply.A
- [5] 0.5 output 0 -> Multiply.B
- [4] Multiply output 0 -> Add.A
- [5] 0.5 output 0 -> Add.B
- [6] Add output 0 -> Texture Sample.Coordinates
- [1] Input ReflectionTexture (Texture2D) output 0 -> Texture Sample.TextureObject
- [2] Input ReflectionVector (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [8] Tangent Space to World Space TransformVector output 0 -> Mask ( B ).Input
- [9] Mask ( B ) output 0 -> ConstantBiasScale.Input
- [11] CheapContrast output 0 -> Multiply.A
- [11] CheapContrast output 0 -> Multiply.B
- [7] Texture Sample output 0 -> Multiply.A
- [13] Multiply output 0 -> Multiply.B
- [14] Multiply output 0 -> Output WorldReflectionShadowed.A

## Output Trace Roots

- `WorldReflection` is driven by [7] Texture Sample
- `WorldReflectionShadowed` is driven by [14] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
