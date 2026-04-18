# SampleSceneDepth

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SampleSceneDepth.SampleSceneDepth`
- Category: Engine_MaterialFunctions02
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Samples the scene depth texture using multiple methods. 

## Inputs

- `Pixel Offset` [FunctionInput_Vector2]
- `Fraction Offset` [FunctionInput_Vector2]
- `Use Custom Depth` [FunctionInput_StaticBool]

## Outputs

- `Depth Using Pixel Offset`
- `Depth Using Fraction Offset`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Depth Using Pixel Offset | Depth Using Pixel Offset |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionSceneTexelSize` | SceneTexelSize |  |
| 3 | expression | `MaterialExpressionAdd` | Add |  |
| 4 | expression | `MaterialExpressionSceneDepth` | Scene Depth |  |
| 5 | expression | `MaterialExpressionScreenPosition` | ScreenPosition |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Pixel Offset (Vector2) | Pixel Offset |
| 8 | expression | `MaterialExpressionViewSize` | ViewSize |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Fraction Offset (Vector2) | Fraction Offset |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | function_output | `MaterialExpressionFunctionOutput` | Output Depth Using Fraction Offset | Depth Using Fraction Offset |
| 13 | expression | `MaterialExpressionSceneDepth` | Scene Depth |  |
| 14 | expression | `MaterialExpressionSceneTexture` | SceneTexture:CustomDepth |  |
| 15 | expression | `MaterialExpressionSceneTexture` | SceneTexture:CustomDepth |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Use Custom Depth (StaticBool) | Use Custom Depth |
| 17 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 18 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 19 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 20 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 21 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |

## Exact Input Wiring

### [0] Output Depth Using Pixel Offset

- `A` <= [17] Switch output 0

### [1] Multiply

- `A` <= [2] SceneTexelSize output 0
- `B` <= [7] Input Pixel Offset (Vector2) output 0

### [3] Add

- `A` <= [1] Multiply output 0
- `B` <= [6] Mask ( R G ) output 0

### [4] Scene Depth

- `Input` <= [3] Add output 0

### [6] Mask ( R G )

- `Input` <= [5] ScreenPosition output 0

### [9] Multiply

- `A` <= [8] ViewSize output 0
- `B` <= [10] Input Fraction Offset (Vector2) output 0

### [10] Input Fraction Offset (Vector2)

- `Preview` <= [7] Input Pixel Offset (Vector2) output 0

### [11] Add

- `A` <= [9] Multiply output 0
- `B` <= [6] Mask ( R G ) output 0

### [12] Output Depth Using Fraction Offset

- `A` <= [19] Switch output 0

### [13] Scene Depth

- `Input` <= [11] Add output 0

### [14] SceneTexture:CustomDepth

- `Coordinates` <= [3] Add output 0

### [15] SceneTexture:CustomDepth

- `Coordinates` <= [11] Add output 0

### [16] Input Use Custom Depth (StaticBool)

- `Preview` <= [21] Static Bool (False) output 0

### [17] Switch

- `A` <= [18] Mask ( R ) output 0
- `B` <= [4] Scene Depth output 0
- `Value` <= [16] Input Use Custom Depth (StaticBool) output 0

### [18] Mask ( R )

- `Input` <= [14] SceneTexture:CustomDepth output 0

### [19] Switch

- `A` <= [20] Mask ( R ) output 0
- `B` <= [13] Scene Depth output 0
- `Value` <= [16] Input Use Custom Depth (StaticBool) output 0

### [20] Mask ( R )

- `Input` <= [15] SceneTexture:CustomDepth output 0

## Connection List

- [17] Switch output 0 -> Output Depth Using Pixel Offset.A
- [2] SceneTexelSize output 0 -> Multiply.A
- [7] Input Pixel Offset (Vector2) output 0 -> Multiply.B
- [1] Multiply output 0 -> Add.A
- [6] Mask ( R G ) output 0 -> Add.B
- [3] Add output 0 -> Scene Depth.Input
- [5] ScreenPosition output 0 -> Mask ( R G ).Input
- [8] ViewSize output 0 -> Multiply.A
- [10] Input Fraction Offset (Vector2) output 0 -> Multiply.B
- [7] Input Pixel Offset (Vector2) output 0 -> Input Fraction Offset (Vector2).Preview
- [9] Multiply output 0 -> Add.A
- [6] Mask ( R G ) output 0 -> Add.B
- [19] Switch output 0 -> Output Depth Using Fraction Offset.A
- [11] Add output 0 -> Scene Depth.Input
- [3] Add output 0 -> SceneTexture:CustomDepth.Coordinates
- [11] Add output 0 -> SceneTexture:CustomDepth.Coordinates
- [21] Static Bool (False) output 0 -> Input Use Custom Depth (StaticBool).Preview
- [18] Mask ( R ) output 0 -> Switch.A
- [4] Scene Depth output 0 -> Switch.B
- [16] Input Use Custom Depth (StaticBool) output 0 -> Switch.Value
- [14] SceneTexture:CustomDepth output 0 -> Mask ( R ).Input
- [20] Mask ( R ) output 0 -> Switch.A
- [13] Scene Depth output 0 -> Switch.B
- [16] Input Use Custom Depth (StaticBool) output 0 -> Switch.Value
- [15] SceneTexture:CustomDepth output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Depth Using Pixel Offset` is driven by [17] Switch
- `Depth Using Fraction Offset` is driven by [19] Switch

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


