# SchlickPhase

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Volumetrics/SchlickPhase.SchlickPhase`
- Category: Engine_MaterialFunctions01/Volumetrics
- Use: General material function. Inspect pins and internal nodes before using in production automation.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

The Schlick Phase Function is useful for adding some directionality or anisotropy to volumetric scattering. Useful for fog/water etc.

## Inputs

- `Cosine` [FunctionInput_Scalar] - Use a Dot Product between the desired two vectors.
- `Anisotropy` [FunctionInput_Scalar] - The amount of anisotropy in the scattering. 0 is purely isotropic and 1 is purely forward scattering.

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | custom | `MaterialExpressionCustom` | Custom |  |
| 2 | function_input | `MaterialExpressionFunctionInput` | Input Cosine (Scalar) | Cosine |
| 3 | function_input | `MaterialExpressionFunctionInput` | Input Anisotropy (Scalar) | Anisotropy |

## Exact Input Wiring

### [0] Output Result

- `A` <= [1] Custom output 0

## Connection List

- [1] Custom output 0 -> Output Result.A

## Output Trace Roots

- `Result` is driven by [1] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


