# ProjectVectorOntoPlane

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ProjectVectorOntoPlane.ProjectVectorOntoPlane`
- Category: Engine_MaterialFunctions02/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Projects a vector onto a plane by removing it's deviation along the plane's normal. 

## Inputs

- `Vector` [FunctionInput_Vector3]
- `Plane Normal` [FunctionInput_Vector3]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Plane Normal (Vector3) | Plane Normal |
| 3 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 4 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 5 | expression | `MaterialExpressionSubtract` | Subtract |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [5] Subtract output 0

### [3] Dot

- `A` <= [1] Input Vector (Vector3) output 0
- `B` <= [2] Input Plane Normal (Vector3) output 0

### [4] Multiply

- `A` <= [3] Dot output 0
- `B` <= [2] Input Plane Normal (Vector3) output 0

### [5] Subtract

- `A` <= [1] Input Vector (Vector3) output 0
- `B` <= [4] Multiply output 0

## Connection List

- [5] Subtract output 0 -> Output Result.A
- [1] Input Vector (Vector3) output 0 -> Dot.A
- [2] Input Plane Normal (Vector3) output 0 -> Dot.B
- [3] Dot output 0 -> Multiply.A
- [2] Input Plane Normal (Vector3) output 0 -> Multiply.B
- [1] Input Vector (Vector3) output 0 -> Subtract.A
- [4] Multiply output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [5] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
