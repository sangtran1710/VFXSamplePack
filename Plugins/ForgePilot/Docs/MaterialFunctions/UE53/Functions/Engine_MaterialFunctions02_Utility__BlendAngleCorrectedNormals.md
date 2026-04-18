# BlendAngleCorrectedNormals

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Utility/BlendAngleCorrectedNormals.BlendAngleCorrectedNormals`
- Category: Engine_MaterialFunctions02/Utility
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: Wire by target_input_name; wrong pin order can still compile with wrong art direction.
- Inspect status: PASS

## Description

Corrects the normal direction of normal map that is overlayed on other normal maps. (Typically this would be a detail map.) 

## Inputs

- `BaseNormal` [FunctionInput_Vector3]
- `AdditionalNormal` [FunctionInput_Vector3] - Insert a tiling normal map or other normal map to blend with the base normal.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input BaseNormal (Vector3) | BaseNormal |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input AdditionalNormal (Vector3) | AdditionalNormal |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 7 | expression | `MaterialExpressionAdd` | Add(,1) |  |
| 8 | expression | `MaterialExpressionAppendVector` | Append |  |
| 9 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 10 | expression | `MaterialExpressionAppendVector` | Append |  |
| 11 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | expression | `MaterialExpressionSubtract` | Subtract |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [14] Subtract output 0

### [3] Mask ( R G )

- `Input` <= [1] Input BaseNormal (Vector3) output 0

### [4] Mask ( B )

- `Input` <= [1] Input BaseNormal (Vector3) output 0

### [5] Mask ( R G )

- `Input` <= [2] Input AdditionalNormal (Vector3) output 0

### [6] Mask ( B )

- `Input` <= [2] Input AdditionalNormal (Vector3) output 0

### [7] Add(,1)

- `A` <= [4] Mask ( B ) output 0

### [8] Append

- `A` <= [3] Mask ( R G ) output 0
- `B` <= [7] Add(,1) output 0

### [9] Dot

- `A` <= [8] Append output 0
- `B` <= [10] Append output 0

### [10] Append

- `A` <= [11] Multiply(,-1) output 0
- `B` <= [6] Mask ( B ) output 0

### [11] Multiply(,-1)

- `A` <= [5] Mask ( R G ) output 0

### [12] Multiply

- `A` <= [7] Add(,1) output 0
- `B` <= [10] Append output 0

### [13] Multiply

- `A` <= [8] Append output 0
- `B` <= [9] Dot output 0

### [14] Subtract

- `A` <= [13] Multiply output 0
- `B` <= [12] Multiply output 0

## Connection List

- [14] Subtract output 0 -> Output Result.A
- [1] Input BaseNormal (Vector3) output 0 -> Mask ( R G ).Input
- [1] Input BaseNormal (Vector3) output 0 -> Mask ( B ).Input
- [2] Input AdditionalNormal (Vector3) output 0 -> Mask ( R G ).Input
- [2] Input AdditionalNormal (Vector3) output 0 -> Mask ( B ).Input
- [4] Mask ( B ) output 0 -> Add(,1).A
- [3] Mask ( R G ) output 0 -> Append.A
- [7] Add(,1) output 0 -> Append.B
- [8] Append output 0 -> Dot.A
- [10] Append output 0 -> Dot.B
- [11] Multiply(,-1) output 0 -> Append.A
- [6] Mask ( B ) output 0 -> Append.B
- [5] Mask ( R G ) output 0 -> Multiply(,-1).A
- [7] Add(,1) output 0 -> Multiply.A
- [10] Append output 0 -> Multiply.B
- [8] Append output 0 -> Multiply.A
- [9] Dot output 0 -> Multiply.B
- [13] Multiply output 0 -> Subtract.A
- [12] Multiply output 0 -> Subtract.B

## Output Trace Roots

- `Result` is driven by [14] Subtract

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
