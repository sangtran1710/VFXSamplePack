# Substrate-IOR-To-F0

- Path: `/Engine/Functions/Strata/Substrate-IOR-To-F0.Substrate-IOR-To-F0`
- Category: Strata
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: May require specific material setup or UE shading model support; do not use blindly for particle materials.
- Inspect status: PASS

## Description

Convert a dieletric IOR into a F0 value.

## Inputs

- `IOR` [FunctionInput_Scalar]

## Outputs

- `F0` - F0 value based on input IOR

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output F0 | F0 |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input IOR (Scalar) | IOR |
| 2 | custom | `MaterialExpressionCustom` | Custom |  |

## Exact Input Wiring

### [0] Output F0

- `A` <= [2] Custom output 0

## Connection List

- [2] Custom output 0 -> Output F0.A

## Output Trace Roots

- `F0` is driven by [2] Custom

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


