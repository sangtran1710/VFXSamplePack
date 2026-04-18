# CorrectPixelDepthOffsetValue

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/CorrectPixelDepthOffsetValue.CorrectPixelDepthOffsetValue`
- Category: Engine_MaterialFunctions02/Math
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Extends pdo distance as the camera aligns with the surface plane.

## Inputs

- `Pixel Depth Offset Amount` [FunctionInput_Scalar]

## Outputs

- `Angle Correct Depth Value`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Angle Correct Depth Value | Angle Correct Depth Value |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Pixel Depth Offset Amount (Scalar) | Pixel Depth Offset Amount |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraVectorWithWPOOptions |  |
| 4 | expression | `MaterialExpressionVertexNormalWS` | VertexNormalWS |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionTwoSidedSign` | TwoSidedSign |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |

## Exact Input Wiring

### [0] Output Angle Correct Depth Value

- `A` <= [2] Divide output 0

### [2] Divide

- `A` <= [1] Input Pixel Depth Offset Amount (Scalar) output 0
- `B` <= [5] Dot output 0

### [5] Dot

- `A` <= [3] CameraVectorWithWPOOptions output 0
- `B` <= [7] Multiply output 0

### [7] Multiply

- `A` <= [4] VertexNormalWS output 0
- `B` <= [6] TwoSidedSign output 0

## Connection List

- [2] Divide output 0 -> Output Angle Correct Depth Value.A
- [1] Input Pixel Depth Offset Amount (Scalar) output 0 -> Divide.A
- [5] Dot output 0 -> Divide.B
- [3] CameraVectorWithWPOOptions output 0 -> Dot.A
- [7] Multiply output 0 -> Dot.B
- [4] VertexNormalWS output 0 -> Multiply.A
- [6] TwoSidedSign output 0 -> Multiply.B

## Output Trace Roots

- `Angle Correct Depth Value` is driven by [2] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


