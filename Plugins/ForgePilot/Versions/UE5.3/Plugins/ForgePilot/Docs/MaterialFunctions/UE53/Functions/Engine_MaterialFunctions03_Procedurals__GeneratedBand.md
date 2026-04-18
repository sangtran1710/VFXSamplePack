# GeneratedBand

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/GeneratedBand.GeneratedBand`
- Category: Engine_MaterialFunctions03/Procedurals
- Use: Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.
- Risk: Potentially expensive depending on texture samples or procedural math; keep usage targeted.
- Inspect status: PASS

## Description

Procedurally generates a horizontal or vertical band from the default TextureCoordinates.

## Inputs

- `Input Coordinates` [FunctionInput_Vector2] - Texture Coordinates used to generate the band, the higher the UV value, the more bands.
- `Width` [FunctionInput_Scalar] - The width of the band, default is .25
- `Sharpness` [FunctionInput_Scalar] - Controls the falloff of the band, 100 will be very sharp and aliased.
- `Offset` [FunctionInput_Scalar] - This value will slide the band around.
- `Direction Switch` [FunctionInput_StaticBool] - Bool to set direction of the band, default is horizontal.
- `Compare` [FunctionInput_Scalar] - The value the input is compared against to generate the band.  Default is .5

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Input Coordinates (Vector2) | Input Coordinates |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | expression | `MaterialExpressionFrac` | Frac |  |
| 4 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 5 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 6 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Width (Scalar) | Width |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Sharpness (Scalar) | Sharpness |
| 9 | expression | `MaterialExpressionAdd` | Add |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Scalar) | Offset |
| 11 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 12 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Direction Switch (StaticBool) | Direction Switch |
| 14 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 15 | function_input | `MaterialExpressionFunctionInput` | Input Compare (Scalar) | Compare |

## Exact Input Wiring

### [0] Output Result

- `A` <= [4] SphereMask output 0

### [1] Input Input Coordinates (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [3] Frac

- `Input` <= [9] Add output 0

### [4] SphereMask

- `A` <= [12] Switch output 0
- `B` <= [15] Input Compare (Scalar) output 0
- `Radius` <= [7] Input Width (Scalar) output 0
- `Hardness` <= [8] Input Sharpness (Scalar) output 0

### [6] Mask ( G )

- `Input` <= [3] Frac output 0

### [9] Add

- `A` <= [1] Input Input Coordinates (Vector2) output 0
- `B` <= [10] Input Offset (Scalar) output 0

### [11] Mask ( R )

- `Input` <= [3] Frac output 0

### [12] Switch

- `A` <= [11] Mask ( R ) output 0
- `B` <= [6] Mask ( G ) output 0
- `Value` <= [13] Input Direction Switch (StaticBool) output 0

### [13] Input Direction Switch (StaticBool)

- `Preview` <= [14] Static Bool (False) output 0

### [15] Input Compare (Scalar)

- `Preview` <= [5] 0.5 output 0

## Connection List

- [4] SphereMask output 0 -> Output Result.A
- [2] TexCoord[0] output 0 -> Input Input Coordinates (Vector2).Preview
- [9] Add output 0 -> Frac.Input
- [12] Switch output 0 -> SphereMask.A
- [15] Input Compare (Scalar) output 0 -> SphereMask.B
- [7] Input Width (Scalar) output 0 -> SphereMask.Radius
- [8] Input Sharpness (Scalar) output 0 -> SphereMask.Hardness
- [3] Frac output 0 -> Mask ( G ).Input
- [1] Input Input Coordinates (Vector2) output 0 -> Add.A
- [10] Input Offset (Scalar) output 0 -> Add.B
- [3] Frac output 0 -> Mask ( R ).Input
- [11] Mask ( R ) output 0 -> Switch.A
- [6] Mask ( G ) output 0 -> Switch.B
- [13] Input Direction Switch (StaticBool) output 0 -> Switch.Value
- [14] Static Bool (False) output 0 -> Input Direction Switch (StaticBool).Preview
- [5] 0.5 output 0 -> Input Compare (Scalar).Preview

## Output Trace Roots

- `Result` is driven by [4] SphereMask

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
