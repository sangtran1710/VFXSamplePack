# GeneratedOffsetBands

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Procedurals/GeneratedOffsetBands.GeneratedOffsetBands`
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
- `Bands` [FunctionInput_Scalar] - Modifies the number of bands

## Outputs

- `Result`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Result | Result |
| 1 | function_input | `MaterialExpressionFunctionInput` | Input Input Coordinates (Vector2) | Input Coordinates |
| 2 | expression | `MaterialExpressionTextureCoordinate` | TexCoord[0] |  |
| 3 | expression | `MaterialExpressionSphereMask` | SphereMask |  |
| 4 | expression | `MaterialExpressionConstant` | 0.5 |  |
| 5 | expression | `MaterialExpressionComponentMask` | Mask ( G ) |  |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input Width (Scalar) | Width |
| 7 | function_input | `MaterialExpressionFunctionInput` | Input Sharpness (Scalar) | Sharpness |
| 8 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Scalar) | Offset |
| 9 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 10 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 11 | function_input | `MaterialExpressionFunctionInput` | Input Direction Switch (StaticBool) | Direction Switch |
| 12 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input Compare (Scalar) | Compare |
| 14 | expression | `MaterialExpressionAdd` | Add |  |
| 15 | expression | `MaterialExpressionSine` | Sine |  |
| 16 | function_input | `MaterialExpressionFunctionInput` | Input Bands (Scalar) | Bands |
| 17 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 18 | expression | `MaterialExpressionAbs` | Abs |  |

## Exact Input Wiring

### [0] Output Result

- `A` <= [3] SphereMask output 0

### [1] Input Input Coordinates (Vector2)

- `Preview` <= [2] TexCoord[0] output 0

### [3] SphereMask

- `A` <= [10] Switch output 0
- `B` <= [13] Input Compare (Scalar) output 0
- `Radius` <= [6] Input Width (Scalar) output 0
- `Hardness` <= [7] Input Sharpness (Scalar) output 0

### [5] Mask ( G )

- `Input` <= [18] Abs output 0

### [9] Mask ( R )

- `Input` <= [18] Abs output 0

### [10] Switch

- `A` <= [9] Mask ( R ) output 0
- `B` <= [5] Mask ( G ) output 0
- `Value` <= [11] Input Direction Switch (StaticBool) output 0

### [11] Input Direction Switch (StaticBool)

- `Preview` <= [12] Static Bool (False) output 0

### [13] Input Compare (Scalar)

- `Preview` <= [4] 0.5 output 0

### [14] Add

- `A` <= [15] Sine output 0
- `B` <= [8] Input Offset (Scalar) output 0

### [15] Sine

- `Input` <= [17] Multiply output 0

### [17] Multiply

- `A` <= [1] Input Input Coordinates (Vector2) output 0
- `B` <= [16] Input Bands (Scalar) output 0

### [18] Abs

- `Input` <= [14] Add output 0

## Connection List

- [3] SphereMask output 0 -> Output Result.A
- [2] TexCoord[0] output 0 -> Input Input Coordinates (Vector2).Preview
- [10] Switch output 0 -> SphereMask.A
- [13] Input Compare (Scalar) output 0 -> SphereMask.B
- [6] Input Width (Scalar) output 0 -> SphereMask.Radius
- [7] Input Sharpness (Scalar) output 0 -> SphereMask.Hardness
- [18] Abs output 0 -> Mask ( G ).Input
- [18] Abs output 0 -> Mask ( R ).Input
- [9] Mask ( R ) output 0 -> Switch.A
- [5] Mask ( G ) output 0 -> Switch.B
- [11] Input Direction Switch (StaticBool) output 0 -> Switch.Value
- [12] Static Bool (False) output 0 -> Input Direction Switch (StaticBool).Preview
- [4] 0.5 output 0 -> Input Compare (Scalar).Preview
- [15] Sine output 0 -> Add.A
- [8] Input Offset (Scalar) output 0 -> Add.B
- [17] Multiply output 0 -> Sine.Input
- [1] Input Input Coordinates (Vector2) output 0 -> Multiply.A
- [16] Input Bands (Scalar) output 0 -> Multiply.B
- [14] Add output 0 -> Abs.Input

## Output Trace Roots

- `Result` is driven by [3] SphereMask

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


