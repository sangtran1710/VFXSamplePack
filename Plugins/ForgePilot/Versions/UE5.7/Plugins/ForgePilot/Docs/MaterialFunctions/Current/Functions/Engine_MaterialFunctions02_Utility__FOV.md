# FOV

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/FOV.FOV`
- Category: Engine_MaterialFunctions02/Utility
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Returns the current cameras FOV setting

## Inputs

- None

## Outputs

- `FOV angle in degrees`
- `FOV in radians`
- `FOV ` - Returns the fov before expensive conversions are run.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output FOV angle in degrees | FOV angle in degrees |
| 1 | custom | `MaterialExpressionCustom` | GrabFOV |  |
| 2 | expression | `MaterialExpressionMultiply` | Multiply(,57.3) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 4 | function_output | `MaterialExpressionFunctionOutput` | Output FOV in radians | FOV in radians |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 6 | custom | `MaterialExpressionCustom` | GrabFOV |  |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output FOV  | FOV  |
| 8 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 9 | custom | `MaterialExpressionCustom` | GrabFOV |  |

## Exact Input Wiring

### [0] Output FOV angle in degrees

- `A` <= [3] Mask ( R ) output 0

### [2] Multiply(,57.3)

- `A` <= [1] GrabFOV output 0

### [3] Mask ( R )

- `Input` <= [2] Multiply(,57.3) output 0

### [4] Output FOV in radians

- `A` <= [5] Mask ( R ) output 0

### [5] Mask ( R )

- `Input` <= [1] GrabFOV output 0

### [7] Output FOV 

- `A` <= [8] Mask ( R ) output 0

### [8] Mask ( R )

- `Input` <= [6] GrabFOV output 0

## Connection List

- [3] Mask ( R ) output 0 -> Output FOV angle in degrees.A
- [1] GrabFOV output 0 -> Multiply(,57.3).A
- [2] Multiply(,57.3) output 0 -> Mask ( R ).Input
- [5] Mask ( R ) output 0 -> Output FOV in radians.A
- [1] GrabFOV output 0 -> Mask ( R ).Input
- [8] Mask ( R ) output 0 -> Output FOV .A
- [6] GrabFOV output 0 -> Mask ( R ).Input

## Output Trace Roots

- `FOV angle in degrees` is driven by [3] Mask ( R )
- `FOV in radians` is driven by [5] Mask ( R )
- `FOV ` is driven by [8] Mask ( R )

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


