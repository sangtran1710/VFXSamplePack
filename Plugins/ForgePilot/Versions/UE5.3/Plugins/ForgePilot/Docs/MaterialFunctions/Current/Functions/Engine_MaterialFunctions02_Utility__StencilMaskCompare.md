# StencilMaskCompare

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/StencilMaskCompare.StencilMaskCompare`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Compares a float to the stencil buffer. White if true - black if false.

## Inputs

- `ID` [FunctionInput_Scalar]

## Outputs

- `Result` - 1.0 if this float aligns with the stencil buffer value and 0.0 if it does not.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionSceneTexture` | SceneTexture:CustomStencil |  |
| 2 | expression | `MaterialExpressionIf` | If |  |
| 3 | expression | `MaterialExpressionConstant` | 0 |  |
| 4 | expression | `MaterialExpressionConstant` | 1 |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input ID (Scalar) | ID |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [2] If output 0

### [2] If

- `A` <= [6] Mask ( R ) output 0
- `B` <= [5] Input ID (Scalar) output 0
- `AGreaterThanB` <= [3] 0 output 0
- `AEqualsB` <= [4] 1 output 0
- `ALessThanB` <= [3] 0 output 0

### [6] Mask ( R )

- `Input` <= [1] SceneTexture:CustomStencil output 0

## Connection List

- [2] If output 0 -> Output Result.A
- [6] Mask ( R ) output 0 -> If.A
- [5] Input ID (Scalar) output 0 -> If.B
- [3] 0 output 0 -> If.AGreaterThanB
- [4] 1 output 0 -> If.AEqualsB
- [3] 0 output 0 -> If.ALessThanB
- [1] SceneTexture:CustomStencil output 0 -> Mask ( R ).Input

## Output Trace Roots

- `Result` is driven by [2] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


