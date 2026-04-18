# CylindricalUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Texturing/CylindricalUVs.CylindricalUVs`
- Category: Engine_MaterialFunctions01/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Tiles a texture using Cylindrical UVS centered around Object Center

## Inputs

- `Normal` [FunctionInput_Vector3]
- `TextureObject` [FunctionInput_Texture2D]
- `In` [FunctionInput_Scalar] - UVHeight

## Outputs

- `Cylinder Projection w Top`
- `Cylinder Projection`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Cylinder Projection w Top | Cylinder Projection w Top |
| 1 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | CheapContrast |  |
| 3 | expression | `MaterialExpressionConstant` | 1 |  |
| 4 | expression | `MaterialExpressionAbs` | Abs |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 6 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 8 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 9 | function_input | `MaterialExpressionFunctionInput` | Input TextureObject (Texture2D) | TextureObject |
| 10 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 12 | expression | `MaterialExpressionDivide` | Divide |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input In (Scalar) | In |
| 14 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 15 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 16 | expression | `MaterialExpressionAppendVector` | Append |  |
| 17 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | VectorToRadialValue |  |
| 19 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 21 | function_output | `MaterialExpressionFunctionOutput` | Output Cylinder Projection | Cylinder Projection |
| 22 | expression | `MaterialExpressionObjectPositionWS` | Object Position (Absolute) |  |

## Exact Input Wiring

### [0] Output Cylinder Projection w Top

- `A` <= [1] Lerp output 0

### [1] Lerp

- `A` <= [15] Texture Sample output 0
- `B` <= [8] Texture Sample output 0
- `Alpha` <= [2] CheapContrast output 0

### [4] Abs

- `Input` <= [5] Mask ( B ) output 0

### [5] Mask ( B )

- `Input` <= [6] Tangent Space to World Space TransformVector output 0

### [6] Tangent Space to World Space TransformVector

- `Input` <= [7] Input Normal (Vector3) output 0

### [8] Texture Sample

- `Coordinates` <= [11] Mask ( R G ) output 0
- `TextureObject` <= [9] Input TextureObject (Texture2D) output 0

### [9] Input TextureObject (Texture2D)

- `Preview` <= [10] Texture Object output 0

### [11] Mask ( R G )

- `Input` <= [12] Divide output 0

### [12] Divide

- `A` <= [14] Absolute World Position (Excluding Material Offsets) output 0
- `B` <= [13] Input In (Scalar) output 0

### [15] Texture Sample

- `Coordinates` <= [16] Append output 0
- `TextureObject` <= [9] Input TextureObject (Texture2D) output 0

### [16] Append

- `A` <= [18] VectorToRadialValue output 1
- `B` <= [17] Mask ( B ) output 0

### [17] Mask ( B )

- `Input` <= [12] Divide output 0

### [19] Mask ( R G )

- `Input` <= [20] Subtract output 0

### [20] Subtract

- `A` <= [14] Absolute World Position (Excluding Material Offsets) output 0
- `B` <= [22] Object Position (Absolute) output 0

### [21] Output Cylinder Projection

- `A` <= [15] Texture Sample output 0

## Connection List

- [1] Lerp output 0 -> Output Cylinder Projection w Top.A
- [15] Texture Sample output 0 -> Lerp.A
- [8] Texture Sample output 0 -> Lerp.B
- [2] CheapContrast output 0 -> Lerp.Alpha
- [5] Mask ( B ) output 0 -> Abs.Input
- [6] Tangent Space to World Space TransformVector output 0 -> Mask ( B ).Input
- [7] Input Normal (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [11] Mask ( R G ) output 0 -> Texture Sample.Coordinates
- [9] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [10] Texture Object output 0 -> Input TextureObject (Texture2D).Preview
- [12] Divide output 0 -> Mask ( R G ).Input
- [14] Absolute World Position (Excluding Material Offsets) output 0 -> Divide.A
- [13] Input In (Scalar) output 0 -> Divide.B
- [16] Append output 0 -> Texture Sample.Coordinates
- [9] Input TextureObject (Texture2D) output 0 -> Texture Sample.TextureObject
- [18] VectorToRadialValue output 1 -> Append.A
- [17] Mask ( B ) output 0 -> Append.B
- [12] Divide output 0 -> Mask ( B ).Input
- [20] Subtract output 0 -> Mask ( R G ).Input
- [14] Absolute World Position (Excluding Material Offsets) output 0 -> Subtract.A
- [22] Object Position (Absolute) output 0 -> Subtract.B
- [15] Texture Sample output 0 -> Output Cylinder Projection.A

## Output Trace Roots

- `Cylinder Projection w Top` is driven by [1] Lerp
- `Cylinder Projection` is driven by [15] Texture Sample

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
