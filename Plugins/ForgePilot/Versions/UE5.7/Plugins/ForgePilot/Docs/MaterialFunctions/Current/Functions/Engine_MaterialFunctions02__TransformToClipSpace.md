# TransformToClipSpace

- Path: `/Engine/Functions/Engine_MaterialFunctions02/TransformToClipSpace.TransformToClipSpace`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `World Position` [FunctionInput_Vector3]
- `Buffer UV Pixel Offset` [FunctionInput_Vector2]

## Outputs

- `Clip Space XY` - A clip space -1 to 1 value.
- `Screen aligned 0-1 UV` - 0-1 (if the object is visible).
- `Buffer UVs for Scene Textures`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Clip Space XY | Clip Space XY |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 3 | custom | `MaterialExpressionCustom` | Custom |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply(,0.5) |  |
| 8 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,-0.5) |  |
| 10 | expression | `MaterialExpressionAdd` | Add(,0.5) |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 12 | expression | `MaterialExpressionAppendVector` | Append |  |
| 13 | function_output | `MaterialExpressionFunctionOutput` | Output Screen aligned 0-1 UV | Screen aligned 0-1 UV |
| 14 | expression | `MaterialExpressionSceneTexelSize` | SceneTexelSize |  |
| 15 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | function_output | `MaterialExpressionFunctionOutput` | Output Buffer UVs for Scene Textures | Buffer UVs for Scene Textures |
| 19 | expression | `MaterialExpressionAdd` | Add |  |
| 20 | function_input | `MaterialExpressionFunctionInput` | Input Buffer UV Pixel Offset (Vector2) | Buffer UV Pixel Offset |

## Exact Input Wiring

### [0] Output Clip Space XY

- `A` <= [6] Divide output 0

### [2] Input World Position (Vector3)

- `Preview` <= [1] Absolute World Position (Excluding Material Offsets) output 0

### [4] Mask ( R G )

- `Input` <= [3] Custom output 0

### [5] Mask ( A )

- `Input` <= [3] Custom output 0

### [6] Divide

- `A` <= [4] Mask ( R G ) output 0
- `B` <= [5] Mask ( A ) output 0

### [7] Multiply(,0.5)

- `A` <= [11] BreakOutFloat2Components output 0

### [8] Add(,0.5)

- `A` <= [7] Multiply(,0.5) output 0

### [9] Multiply(,-0.5)

- `A` <= [11] BreakOutFloat2Components output 1

### [10] Add(,0.5)

- `A` <= [9] Multiply(,-0.5) output 0

### [12] Append

- `A` <= [8] Add(,0.5) output 0
- `B` <= [10] Add(,0.5) output 0

### [13] Output Screen aligned 0-1 UV

- `A` <= [12] Append output 0

### [16] Multiply

- `A` <= [13] Output Screen aligned 0-1 UV output 0
- `B` <= [15] ViewSize output 0

### [17] Multiply

- `A` <= [19] Add output 0
- `B` <= [14] SceneTexelSize output 0

### [18] Output Buffer UVs for Scene Textures

- `A` <= [17] Multiply output 0

### [19] Add

- `A` <= [16] Multiply output 0
- `B` <= [20] Input Buffer UV Pixel Offset (Vector2) output 0

## Connection List

- [6] Divide output 0 -> Output Clip Space XY.A
- [1] Absolute World Position (Excluding Material Offsets) output 0 -> Input World Position (Vector3).Preview
- [3] Custom output 0 -> Mask ( R G ).Input
- [3] Custom output 0 -> Mask ( A ).Input
- [4] Mask ( R G ) output 0 -> Divide.A
- [5] Mask ( A ) output 0 -> Divide.B
- [11] BreakOutFloat2Components output 0 -> Multiply(,0.5).A
- [7] Multiply(,0.5) output 0 -> Add(,0.5).A
- [11] BreakOutFloat2Components output 1 -> Multiply(,-0.5).A
- [9] Multiply(,-0.5) output 0 -> Add(,0.5).A
- [8] Add(,0.5) output 0 -> Append.A
- [10] Add(,0.5) output 0 -> Append.B
- [12] Append output 0 -> Output Screen aligned 0-1 UV.A
- [13] Output Screen aligned 0-1 UV output 0 -> Multiply.A
- [15] ViewSize output 0 -> Multiply.B
- [19] Add output 0 -> Multiply.A
- [14] SceneTexelSize output 0 -> Multiply.B
- [17] Multiply output 0 -> Output Buffer UVs for Scene Textures.A
- [16] Multiply output 0 -> Add.A
- [20] Input Buffer UV Pixel Offset (Vector2) output 0 -> Add.B

## Output Trace Roots

- `Clip Space XY` is driven by [6] Divide
- `Screen aligned 0-1 UV` is driven by [12] Append
- `Buffer UVs for Scene Textures` is driven by [17] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


