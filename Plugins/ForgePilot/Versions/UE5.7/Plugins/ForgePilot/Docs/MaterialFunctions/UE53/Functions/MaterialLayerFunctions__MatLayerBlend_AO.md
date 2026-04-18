# MatLayerBlend_AO

- Path: `/Engine/Functions/MaterialLayerFunctions/MatLayerBlend_AO.MatLayerBlend_AO`
- Category: MaterialLayerFunctions
- Use: Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Blends an AO map to remove reflection.

## Inputs

- `AO` [FunctionInput_Scalar]
- `Material` [FunctionInput_MaterialAttributes]

## Outputs

- `Blended Material`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_input | `MaterialExpressionFunctionInput` | Input AO (Scalar) | AO |
| 1 | function_output | `MaterialExpressionFunctionOutput` | Output Blended Material | Blended Material |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Material (MaterialAttributes) | Material |
| 3 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 4 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 6 | expression | `MaterialExpressionConstant` | 1 |  |
| 7 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 8 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 10 | expression | `MaterialExpressionConstant` | 1 |  |
| 11 | expression | `MaterialExpressionGetMaterialAttributes` | GetMaterialAttributes |  |
| 12 | expression | `MaterialExpressionSetMaterialAttributes` | SetMaterialAttributes |  |
| 13 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |
| 14 | expression | `MaterialExpressionReroute` | Reroute Node (reroutes wires) |  |

## Exact Input Wiring

### [1] Output Blended Material

- `A` <= [12] SetMaterialAttributes output 0

### [3] Multiply

- `A` <= [4] Lerp output 0
- `B` <= [11] GetMaterialAttributes output 3

### [4] Lerp

- `A` <= [0] Input AO (Scalar) output 0
- `B` <= [6] 1 output 0
- `Alpha` <= [5] Mask ( R ) output 0

### [5] Mask ( R )

- `Input` <= [11] GetMaterialAttributes output 2

### [7] Multiply

- `A` <= [8] Lerp output 0
- `B` <= [11] GetMaterialAttributes output 1

### [8] Lerp

- `A` <= [10] 1 output 0
- `B` <= [0] Input AO (Scalar) output 0
- `Alpha` <= [9] Mask ( R ) output 0

### [9] Mask ( R )

- `Input` <= [11] GetMaterialAttributes output 2

### [12] SetMaterialAttributes

- `Inputs[0]` <= [13] Reroute Node (reroutes wires) output 0
- `Inputs[1]` <= [7] Multiply output 0
- `Inputs[2]` <= [11] GetMaterialAttributes output 2
- `Inputs[3]` <= [3] Multiply output 0

### [13] Reroute Node (reroutes wires)

- `Input` <= [14] Reroute Node (reroutes wires) output 0

### [14] Reroute Node (reroutes wires)

- `Input` <= [11] GetMaterialAttributes output 0

## Connection List

- [12] SetMaterialAttributes output 0 -> Output Blended Material.A
- [4] Lerp output 0 -> Multiply.A
- [11] GetMaterialAttributes output 3 -> Multiply.B
- [0] Input AO (Scalar) output 0 -> Lerp.A
- [6] 1 output 0 -> Lerp.B
- [5] Mask ( R ) output 0 -> Lerp.Alpha
- [11] GetMaterialAttributes output 2 -> Mask ( R ).Input
- [8] Lerp output 0 -> Multiply.A
- [11] GetMaterialAttributes output 1 -> Multiply.B
- [10] 1 output 0 -> Lerp.A
- [0] Input AO (Scalar) output 0 -> Lerp.B
- [9] Mask ( R ) output 0 -> Lerp.Alpha
- [11] GetMaterialAttributes output 2 -> Mask ( R ).Input
- [13] Reroute Node (reroutes wires) output 0 -> SetMaterialAttributes.Inputs[0]
- [7] Multiply output 0 -> SetMaterialAttributes.Inputs[1]
- [11] GetMaterialAttributes output 2 -> SetMaterialAttributes.Inputs[2]
- [3] Multiply output 0 -> SetMaterialAttributes.Inputs[3]
- [14] Reroute Node (reroutes wires) output 0 -> Reroute Node (reroutes wires).Input
- [11] GetMaterialAttributes output 0 -> Reroute Node (reroutes wires).Input

## Output Trace Roots

- `Blended Material` is driven by [12] SetMaterialAttributes

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
