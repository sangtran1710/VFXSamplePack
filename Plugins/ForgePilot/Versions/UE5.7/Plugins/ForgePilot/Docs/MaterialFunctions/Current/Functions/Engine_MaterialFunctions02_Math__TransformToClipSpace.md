# TransformToClipSpace

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TransformToClipSpace.TransformToClipSpace`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Transforms a world position into clip space (render buffer uv space).

## Inputs

- `World Position` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input World Position (Vector3) | World Position |
| 3 | custom | `MaterialExpressionCustom` | Custom |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 6 | expression | `MaterialExpressionDivide` | Divide |  |

## Exact Input Wiring

### [0] Output Result

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

## Connection List

- [6] Divide output 0 -> Output Result.A
- [1] Absolute World Position (Excluding Material Offsets) output 0 -> Input World Position (Vector3).Preview
- [3] Custom output 0 -> Mask ( R G ).Input
- [3] Custom output 0 -> Mask ( A ).Input
- [4] Mask ( R G ) output 0 -> Divide.A
- [5] Mask ( A ) output 0 -> Divide.B

## Output Trace Roots

- `Result` is driven by [6] Divide

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


