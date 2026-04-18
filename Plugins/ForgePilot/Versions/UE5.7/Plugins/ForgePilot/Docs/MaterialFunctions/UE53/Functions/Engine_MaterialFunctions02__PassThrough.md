# PassThrough

- Path: `/Engine/Functions/Engine_MaterialFunctions02/PassThrough.PassThrough`
- Category: Engine_MaterialFunctions02
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

This node simply passes any inserted values through itself. Suggested uses include, using this node to form a closer connection point for nodes on opposite sides of the graph or bufferering math operations and the nodes they connect to with this connection hub node. 

## Inputs

- `V3` [FunctionInput_Vector3] - Insert a value to be passed through the node.
- `V2` [FunctionInput_Vector2] - Insert a value to be passed through the node.
- `V4` [FunctionInput_Vector4] - Insert a value to be passed through the node.
- `S` [FunctionInput_Scalar] - Insert a value to be passed through the node.

## Outputs

- `V3`
- `V2`
- `S`
- `V4`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output V3 | V3 |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input V3 (Vector3) | V3 |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input V2 (Vector2) | V2 |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input V4 (Vector4) | V4 |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input S (Scalar) | S |
| 5 | function_output | `MaterialExpressionFunctionOutput` | Output V2 | V2 |
| 6 | function_output | `MaterialExpressionFunctionOutput` | Output S | S |
| 7 | function_output | `MaterialExpressionFunctionOutput` | Output V4 | V4 |

## Exact Input Wiring

### [0] Output V3

- `A` <= [1] Input V3 (Vector3) output 0

### [5] Output V2

- `A` <= [2] Input V2 (Vector2) output 0

### [6] Output S

- `A` <= [4] Input S (Scalar) output 0

### [7] Output V4

- `A` <= [3] Input V4 (Vector4) output 0

## Connection List

- [1] Input V3 (Vector3) output 0 -> Output V3.A
- [2] Input V2 (Vector2) output 0 -> Output V2.A
- [4] Input S (Scalar) output 0 -> Output S.A
- [3] Input V4 (Vector4) output 0 -> Output V4.A

## Output Trace Roots

- `V3` is driven by [1] Input V3 (Vector3)
- `V2` is driven by [2] Input V2 (Vector2)
- `S` is driven by [4] Input S (Scalar)
- `V4` is driven by [3] Input V4 (Vector4)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
