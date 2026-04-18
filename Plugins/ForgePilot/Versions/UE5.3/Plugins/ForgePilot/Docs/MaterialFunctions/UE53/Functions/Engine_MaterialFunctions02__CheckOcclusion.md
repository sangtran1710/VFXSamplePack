# CheckOcclusion

- Path: `/Engine/Functions/Engine_MaterialFunctions02/CheckOcclusion.CheckOcclusion`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Checks to see if a world position is occluded by an opaque object. For use with translucent materials.

## Inputs

- `Position To check for Occlusion` [FunctionInput_Vector3]
- `Offscreen Falloff Range` [FunctionInput_Scalar]
- `Depth Occlusion Falloff` [FunctionInput_Scalar]

## Outputs

- `Screen Edge And Occlusion`
- `Screen Edge`
- `Occlusion`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Screen Edge And Occlusion | Screen Edge And Occlusion |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | function_call | `MaterialExpressionMaterialFunctionCall` | TransformToClipSpace |  |
| 3 | expression | `MaterialExpressionSceneDepth` | Scene Depth |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | DepthFromWorldPosition |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |
| 7 | expression | `MaterialExpressionClamp` | Clamp |  |
| 8 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 9 | expression | `MaterialExpressionAbs` | Abs |  |
| 10 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 11 | function_call | `MaterialExpressionMaterialFunctionCall` | BreakOutFloat2Components |  |
| 12 | expression | `MaterialExpressionConstant` | 1 |  |
| 13 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionClamp` | Clamp |  |
| 16 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 17 | expression | `MaterialExpressionFloor` | Floor |  |
| 18 | function_call | `MaterialExpressionMaterialFunctionCall` | AddComponents |  |
| 19 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 20 | expression | `MaterialExpressionClamp` | Clamp |  |
| 21 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Position To check for Occlusion (Vector3) | Position To check for Occlusion |
| 23 | function_input | `MaterialExpressionFunctionInput` | Input Offscreen Falloff Range (Scalar) | Offscreen Falloff Range |
| 24 | expression | `MaterialExpressionTransform` | View Space to World Space TransformVector |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Depth Occlusion Falloff (Scalar) | Depth Occlusion Falloff |
| 26 | function_output | `MaterialExpressionFunctionOutput` | Output Screen Edge | Screen Edge |
| 27 | function_output | `MaterialExpressionFunctionOutput` | Output Occlusion | Occlusion |

## Exact Input Wiring

### [0] Output Screen Edge And Occlusion

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [7] Clamp output 0
- `B` <= [19] Multiply output 0

### [3] Scene Depth

- `Input` <= [2] TransformToClipSpace output 0

### [5] Subtract

- `A` <= [3] Scene Depth output 0
- `B` <= [4] DepthFromWorldPosition output 0

### [6] Divide

- `A` <= [5] Subtract output 0
- `B` <= [25] Input Depth Occlusion Falloff (Scalar) output 0

### [7] Clamp

- `Input` <= [6] Divide output 0

### [9] Abs

- `Input` <= [2] TransformToClipSpace output 2

### [10] SphereMask

- `A` <= [11] BreakOutFloat2Components output 0
- `B` <= [12] 1 output 0
- `Radius` <= [23] Input Offscreen Falloff Range (Scalar) output 0

### [13] SphereMask

- `A` <= [11] BreakOutFloat2Components output 1
- `B` <= [12] 1 output 0
- `Radius` <= [23] Input Offscreen Falloff Range (Scalar) output 0

### [14] Add

- `A` <= [10] SphereMask output 0
- `B` <= [13] SphereMask output 0

### [15] Clamp

- `Input` <= [14] Add output 0

### [16] 1-x

- `Input` <= [15] Clamp output 0

### [17] Floor

- `Input` <= [9] Abs output 0

### [19] Multiply

- `A` <= [21] 1-x output 0
- `B` <= [16] 1-x output 0

### [20] Clamp

- `Input` <= [18] AddComponents output 0

### [21] 1-x

- `Input` <= [20] Clamp output 0

### [22] Input Position To check for Occlusion (Vector3)

- `Preview` <= [8] Actor Position (Absolute) output 0

### [26] Output Screen Edge

- `A` <= [19] Multiply output 0

### [27] Output Occlusion

- `A` <= [7] Clamp output 0

## Connection List

- [1] Multiply output 0 -> Output Screen Edge And Occlusion.A
- [7] Clamp output 0 -> Multiply.A
- [19] Multiply output 0 -> Multiply.B
- [2] TransformToClipSpace output 0 -> Scene Depth.Input
- [3] Scene Depth output 0 -> Subtract.A
- [4] DepthFromWorldPosition output 0 -> Subtract.B
- [5] Subtract output 0 -> Divide.A
- [25] Input Depth Occlusion Falloff (Scalar) output 0 -> Divide.B
- [6] Divide output 0 -> Clamp.Input
- [2] TransformToClipSpace output 2 -> Abs.Input
- [11] BreakOutFloat2Components output 0 -> SphereMask.A
- [12] 1 output 0 -> SphereMask.B
- [23] Input Offscreen Falloff Range (Scalar) output 0 -> SphereMask.Radius
- [11] BreakOutFloat2Components output 1 -> SphereMask.A
- [12] 1 output 0 -> SphereMask.B
- [23] Input Offscreen Falloff Range (Scalar) output 0 -> SphereMask.Radius
- [10] SphereMask output 0 -> Add.A
- [13] SphereMask output 0 -> Add.B
- [14] Add output 0 -> Clamp.Input
- [15] Clamp output 0 -> 1-x.Input
- [9] Abs output 0 -> Floor.Input
- [21] 1-x output 0 -> Multiply.A
- [16] 1-x output 0 -> Multiply.B
- [18] AddComponents output 0 -> Clamp.Input
- [20] Clamp output 0 -> 1-x.Input
- [8] Actor Position (Absolute) output 0 -> Input Position To check for Occlusion (Vector3).Preview
- [19] Multiply output 0 -> Output Screen Edge.A
- [7] Clamp output 0 -> Output Occlusion.A

## Output Trace Roots

- `Screen Edge And Occlusion` is driven by [1] Multiply
- `Screen Edge` is driven by [19] Multiply
- `Occlusion` is driven by [7] Clamp

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
