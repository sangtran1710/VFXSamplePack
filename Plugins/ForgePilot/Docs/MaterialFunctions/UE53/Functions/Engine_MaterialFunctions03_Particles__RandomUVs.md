# RandomUVs

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Particles/RandomUVs.RandomUVs`
- Category: Engine_MaterialFunctions03/Particles
- Use: Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Uses a DynamicParameter with U_Offset and V_Offset as parameter names to move UV coordinates around.  Set up as a uniform distribution, at spawn time only, with a 0-1 distribution per-channel.

## Inputs

- `Coordinate Input` [FunctionInput_Vector2] - Default coordinates are 

## Outputs

- `UV out` - 2-vector UV coordinates offset by DynamicParameter

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output UV out | UV out |
| 1 | expression | `MaterialExpressionDynamicParameter` | Dynamic Parameter |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Coordinate Input (Vector2) | Coordinate Input |
| 3 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 4 | expression | `MaterialExpressionAdd` | Add |  |
| 5 | expression | `MaterialExpressionAppendVector` | Append |  |

## Exact Input Wiring

### [0] Output UV out

- `A` <= [4] Add output 0

### [2] Input Coordinate Input (Vector2)

- `Preview` <= [3] TexCoord[0] output 0

### [4] Add

- `A` <= [2] Input Coordinate Input (Vector2) output 0
- `B` <= [5] Append output 0

### [5] Append

- `A` <= [1] Dynamic Parameter output 1
- `B` <= [1] Dynamic Parameter output 2

## Connection List

- [4] Add output 0 -> Output UV out.A
- [3] TexCoord[0] output 0 -> Input Coordinate Input (Vector2).Preview
- [2] Input Coordinate Input (Vector2) output 0 -> Add.A
- [5] Append output 0 -> Add.B
- [1] Dynamic Parameter output 1 -> Append.A
- [1] Dynamic Parameter output 2 -> Append.B

## Output Trace Roots

- `UV out` is driven by [4] Add

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
