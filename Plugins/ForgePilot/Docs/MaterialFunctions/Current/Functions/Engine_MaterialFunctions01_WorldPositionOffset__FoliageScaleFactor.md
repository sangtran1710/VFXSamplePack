# FoliageScaleFactor

- Path: `/Engine/Functions/Engine_MaterialFunctions01/WorldPositionOffset/FoliageScaleFactor.FoliageScaleFactor`
- Category: Engine_MaterialFunctions01/WorldPositionOffset
- Use: Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

A way to get object scale for Foliage meshes where easier solutions like "ObjectScale" do not work.

## Inputs

- `Initial Radius` [FunctionInput_Scalar] - This method allows you to input the object initial radius and it will multiply by the Scale Factor. Otherwise it will output the Scale Factor of the mesh.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionTransformPosition` | Local Space to Local Space TransformPosition |  |
| 3 | expression | `MaterialExpressionTransformPosition` | Local Space to Local Space TransformPosition |  |
| 4 | expression | `MaterialExpressionDistance` | Distance |  |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input Initial Radius (Scalar) | Initial Radius |
| 6 | expression | `MaterialExpressionConstant3Vector` | 0,0,0 |  |
| 7 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Multiply output 0

### [1] Multiply

- `A` <= [4] Distance output 0
- `B` <= [5] Input Initial Radius (Scalar) output 0

### [2] Local Space to Local Space TransformPosition

- `Input` <= [6] 0,0,0 output 0

### [3] Local Space to Local Space TransformPosition

- `Input` <= [7] 0,0,1 output 0

### [4] Distance

- `A` <= [2] Local Space to Local Space TransformPosition output 0
- `B` <= [3] Local Space to Local Space TransformPosition output 0

## Connection List

- [1] Multiply output 0 -> Output Result.A
- [4] Distance output 0 -> Multiply.A
- [5] Input Initial Radius (Scalar) output 0 -> Multiply.B
- [6] 0,0,0 output 0 -> Local Space to Local Space TransformPosition.Input
- [7] 0,0,1 output 0 -> Local Space to Local Space TransformPosition.Input
- [2] Local Space to Local Space TransformPosition output 0 -> Distance.A
- [3] Local Space to Local Space TransformPosition output 0 -> Distance.B

## Output Trace Roots

- `Result` is driven by [1] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


