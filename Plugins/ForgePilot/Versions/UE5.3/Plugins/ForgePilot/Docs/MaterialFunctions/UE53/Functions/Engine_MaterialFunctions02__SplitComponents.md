# SplitComponents

- Path: `/Engine/Functions/Engine_MaterialFunctions02/SplitComponents.SplitComponents`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Color` [FunctionInput_Vector3]

## Outputs

- `R`
- `G`
- `B`
- `RGB` - Passthrough

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output R | R |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output G | G |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output B | B |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Color (Vector3) | Color |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output RGB | RGB |

## Exact Input Wiring

### [0] Output R

- `A` <= [4] Mask ( R ) output 0

### [1] Output G

- `A` <= [5] Mask ( G ) output 0

### [2] Output B

- `A` <= [6] Mask ( B ) output 0

### [4] Mask ( R )

- `Input` <= [3] Input Color (Vector3) output 0

### [5] Mask ( G )

- `Input` <= [3] Input Color (Vector3) output 0

### [6] Mask ( B )

- `Input` <= [3] Input Color (Vector3) output 0

### [7] Output RGB

- `A` <= [3] Input Color (Vector3) output 0

## Connection List

- [4] Mask ( R ) output 0 -> Output R.A
- [5] Mask ( G ) output 0 -> Output G.A
- [6] Mask ( B ) output 0 -> Output B.A
- [3] Input Color (Vector3) output 0 -> Mask ( R ).Input
- [3] Input Color (Vector3) output 0 -> Mask ( G ).Input
- [3] Input Color (Vector3) output 0 -> Mask ( B ).Input
- [3] Input Color (Vector3) output 0 -> Output RGB.A

## Output Trace Roots

- `R` is driven by [4] Mask ( R )
- `G` is driven by [5] Mask ( G )
- `B` is driven by [6] Mask ( B )
- `RGB` is driven by [3] Input Color (Vector3)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
