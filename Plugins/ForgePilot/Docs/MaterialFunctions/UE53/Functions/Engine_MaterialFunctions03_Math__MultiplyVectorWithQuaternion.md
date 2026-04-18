# MultiplyVectorWithQuaternion

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Math/MultiplyVectorWithQuaternion.MultiplyVectorWithQuaternion`
- Category: Engine_MaterialFunctions03/Math
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `Quat` [FunctionInput_Vector4]
- `Vector` [FunctionInput_Vector3]
- `Invert Quaternion` [FunctionInput_StaticBool]

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Quat (Vector4) | Quat |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Vector (Vector3) | Vector |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( R G B ) |  |
| 4 | expression | `MaterialExpressionComponentMask` | Mask ( A ) |  |
| 5 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionAdd` | Add |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 10 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 11 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 13 | expression | `MaterialExpressionAdd` | Add |  |
| 14 | expression | `MaterialExpressionCrossProduct` | Cross |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 16 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Invert Quaternion (StaticBool) | Invert Quaternion |
| 18 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 19 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 20 | expression | `MaterialExpressionMultiply` | Multiply(,-1) |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [13] Add output 0

### [3] Mask ( R G B )

- `Input` <= [1] Input Quat (Vector4) output 0

### [4] Mask ( A )

- `Input` <= [1] Input Quat (Vector4) output 0

### [5] Dot

- `A` <= [18] Switch output 0
- `B` <= [2] Input Vector (Vector3) output 0

### [6] Multiply(,2)

- `A` <= [5] Dot output 0

### [7] Multiply

- `A` <= [18] Switch output 0
- `B` <= [6] Multiply(,2) output 0

### [8] Add

- `A` <= [7] Multiply output 0
- `B` <= [12] Multiply output 0

### [9] Multiply

- `A` <= [4] Mask ( A ) output 0
- `B` <= [4] Mask ( A ) output 0

### [10] Subtract

- `A` <= [9] Multiply output 0
- `B` <= [11] Dot output 0

### [11] Dot

- `A` <= [18] Switch output 0
- `B` <= [18] Switch output 0

### [12] Multiply

- `A` <= [10] Subtract output 0
- `B` <= [2] Input Vector (Vector3) output 0

### [13] Add

- `A` <= [8] Add output 0
- `B` <= [16] Multiply output 0

### [14] Cross

- `A` <= [18] Switch output 0
- `B` <= [2] Input Vector (Vector3) output 0

### [15] Multiply(,2)

- `A` <= [4] Mask ( A ) output 0

### [16] Multiply

- `A` <= [14] Cross output 0
- `B` <= [15] Multiply(,2) output 0

### [17] Input Invert Quaternion (StaticBool)

- `Preview` <= [19] Static Bool (False) output 0

### [18] Switch

- `A` <= [20] Multiply(,-1) output 0
- `B` <= [3] Mask ( R G B ) output 0
- `Value` <= [17] Input Invert Quaternion (StaticBool) output 0

### [20] Multiply(,-1)

- `A` <= [3] Mask ( R G B ) output 0

## Connection List

- [13] Add output 0 -> Output Result.A
- [1] Input Quat (Vector4) output 0 -> Mask ( R G B ).Input
- [1] Input Quat (Vector4) output 0 -> Mask ( A ).Input
- [18] Switch output 0 -> Dot.A
- [2] Input Vector (Vector3) output 0 -> Dot.B
- [5] Dot output 0 -> Multiply(,2).A
- [18] Switch output 0 -> Multiply.A
- [6] Multiply(,2) output 0 -> Multiply.B
- [7] Multiply output 0 -> Add.A
- [12] Multiply output 0 -> Add.B
- [4] Mask ( A ) output 0 -> Multiply.A
- [4] Mask ( A ) output 0 -> Multiply.B
- [9] Multiply output 0 -> Subtract.A
- [11] Dot output 0 -> Subtract.B
- [18] Switch output 0 -> Dot.A
- [18] Switch output 0 -> Dot.B
- [10] Subtract output 0 -> Multiply.A
- [2] Input Vector (Vector3) output 0 -> Multiply.B
- [8] Add output 0 -> Add.A
- [16] Multiply output 0 -> Add.B
- [18] Switch output 0 -> Cross.A
- [2] Input Vector (Vector3) output 0 -> Cross.B
- [4] Mask ( A ) output 0 -> Multiply(,2).A
- [14] Cross output 0 -> Multiply.A
- [15] Multiply(,2) output 0 -> Multiply.B
- [19] Static Bool (False) output 0 -> Input Invert Quaternion (StaticBool).Preview
- [20] Multiply(,-1) output 0 -> Switch.A
- [3] Mask ( R G B ) output 0 -> Switch.B
- [17] Input Invert Quaternion (StaticBool) output 0 -> Switch.Value
- [3] Mask ( R G B ) output 0 -> Multiply(,-1).A

## Output Trace Roots

- `Result` is driven by [13] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
