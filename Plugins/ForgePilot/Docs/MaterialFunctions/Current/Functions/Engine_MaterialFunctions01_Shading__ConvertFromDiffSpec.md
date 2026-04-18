# ConvertFromDiffSpec

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/ConvertFromDiffSpec.ConvertFromDiffSpec`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Inputs

- `DiffuseColor` [FunctionInput_Vector3]
- `SpecularColor` [FunctionInput_Vector3]

## Outputs

- `BaseColor`
- `Metallic`
- `Specular`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output BaseColor | BaseColor |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Metallic | Metallic |
| 2 | function_output | `MaterialExpressionFunctionOutput` | Output Specular | Specular |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input DiffuseColor (Vector3) | DiffuseColor |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input SpecularColor (Vector3) | SpecularColor |
| 5 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 6 | expression | `MaterialExpressionDesaturation` | Desaturation |  |
| 7 | expression | `MaterialExpressionAdd` | Add |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionMultiply` | Multiply(,4) |  |
| 10 | expression | `MaterialExpressionAdd` | Add |  |
| 11 | expression | `MaterialExpressionClamp` | Clamp |  |
| 12 | expression | `MaterialExpressionMultiply` | Multiply(,-5) |  |
| 13 | expression | `MaterialExpressionAdd` | Add(,-0.4) |  |
| 14 | expression | `MaterialExpressionClamp` | Clamp |  |
| 15 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 16 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 17 | expression | `MaterialExpressionAdd` | Add |  |
| 18 | expression | `MaterialExpressionMultiply` | Multiply(,12.5) |  |
| 19 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 20 | expression | `MaterialExpressionSubtract` | Subtract |  |

## Exact Input Wiring

### [0] Output BaseColor

- `A` <= [16] Lerp output 0

### [1] Output Metallic

- `A` <= [15] Multiply output 0

### [2] Output Specular

- `A` <= [18] Multiply(,12.5) output 0

### [5] Desaturation

- `Input` <= [3] Input DiffuseColor (Vector3) output 0

### [6] Desaturation

- `Input` <= [4] Input SpecularColor (Vector3) output 0

### [7] Add

- `A` <= [5] Desaturation output 0
- `B` <= [6] Desaturation output 0

### [8] Divide

- `A` <= [6] Desaturation output 0
- `B` <= [7] Add output 0

### [9] Multiply(,4)

- `A` <= [20] Subtract output 0

### [10] Add

- `A` <= [9] Multiply(,4) output 0
- `B` <= [19] 0.5 output 0

### [11] Clamp

- `Input` <= [10] Add output 0

### [12] Multiply(,-5)

- `A` <= [13] Add(,-0.4) output 0

### [13] Add(,-0.4)

- `A` <= [5] Desaturation output 0

### [14] Clamp

- `Input` <= [12] Multiply(,-5) output 0

### [15] Multiply

- `A` <= [14] Clamp output 0
- `B` <= [11] Clamp output 0

### [16] Lerp

- `A` <= [3] Input DiffuseColor (Vector3) output 0
- `B` <= [17] Add output 0
- `Alpha` <= [15] Multiply output 0

### [17] Add

- `A` <= [3] Input DiffuseColor (Vector3) output 0
- `B` <= [4] Input SpecularColor (Vector3) output 0

### [18] Multiply(,12.5)

- `A` <= [6] Desaturation output 0

### [20] Subtract

- `A` <= [8] Divide output 0
- `B` <= [19] 0.5 output 0

## Connection List

- [16] Lerp output 0 -> Output BaseColor.A
- [15] Multiply output 0 -> Output Metallic.A
- [18] Multiply(,12.5) output 0 -> Output Specular.A
- [3] Input DiffuseColor (Vector3) output 0 -> Desaturation.Input
- [4] Input SpecularColor (Vector3) output 0 -> Desaturation.Input
- [5] Desaturation output 0 -> Add.A
- [6] Desaturation output 0 -> Add.B
- [6] Desaturation output 0 -> Divide.A
- [7] Add output 0 -> Divide.B
- [20] Subtract output 0 -> Multiply(,4).A
- [9] Multiply(,4) output 0 -> Add.A
- [19] 0.5 output 0 -> Add.B
- [10] Add output 0 -> Clamp.Input
- [13] Add(,-0.4) output 0 -> Multiply(,-5).A
- [5] Desaturation output 0 -> Add(,-0.4).A
- [12] Multiply(,-5) output 0 -> Clamp.Input
- [14] Clamp output 0 -> Multiply.A
- [11] Clamp output 0 -> Multiply.B
- [3] Input DiffuseColor (Vector3) output 0 -> Lerp.A
- [17] Add output 0 -> Lerp.B
- [15] Multiply output 0 -> Lerp.Alpha
- [3] Input DiffuseColor (Vector3) output 0 -> Add.A
- [4] Input SpecularColor (Vector3) output 0 -> Add.B
- [6] Desaturation output 0 -> Multiply(,12.5).A
- [8] Divide output 0 -> Subtract.A
- [19] 0.5 output 0 -> Subtract.B

## Output Trace Roots

- `BaseColor` is driven by [16] Lerp
- `Metallic` is driven by [15] Multiply
- `Specular` is driven by [18] Multiply(,12.5)

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


