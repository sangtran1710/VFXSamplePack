# ObjectLocalBounds

- Path: `/Engine/Functions/Engine_MaterialFunctions02/ObjectLocalBounds.ObjectLocalBounds`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- None

## Outputs

- `Local Bounds Minimum`
- `Local Bounds Max`
- `Local Bounds Size`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Local Bounds Minimum | Local Bounds Minimum |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Local Bounds Max | Local Bounds Max |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output Local Bounds Size | Local Bounds Size |
| 3 | custom | `MaterialExpressionCustom` | Local Bounds Minimum |  |
| 4 | custom | `MaterialExpressionCustom` | Local Bounds Max |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |

## Exact Input Wiring

### [0] Output Local Bounds Minimum

- `A` <= [3] Local Bounds Minimum output 0

### [1] Output Local Bounds Max

- `A` <= [4] Local Bounds Max output 0

### [2] Output Local Bounds Size

- `A` <= [5] Subtract output 0

### [5] Subtract

- `A` <= [4] Local Bounds Max output 0
- `B` <= [3] Local Bounds Minimum output 0

## Connection List

- [3] Local Bounds Minimum output 0 -> Output Local Bounds Minimum.A
- [4] Local Bounds Max output 0 -> Output Local Bounds Max.A
- [5] Subtract output 0 -> Output Local Bounds Size.A
- [4] Local Bounds Max output 0 -> Subtract.A
- [3] Local Bounds Minimum output 0 -> Subtract.B

## Output Trace Roots

- `Local Bounds Minimum` is driven by [3] Local Bounds Minimum
- `Local Bounds Max` is driven by [4] Local Bounds Max
- `Local Bounds Size` is driven by [5] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


