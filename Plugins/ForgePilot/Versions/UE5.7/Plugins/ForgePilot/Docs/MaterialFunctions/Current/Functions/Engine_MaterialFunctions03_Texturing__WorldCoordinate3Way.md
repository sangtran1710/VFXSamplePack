# WorldCoordinate3Way

- Path: `/Engine/Functions/Engine_MaterialFunctions03/Texturing/WorldCoordinate3Way.WorldCoordinate3Way`
- Category: Engine_MaterialFunctions03/Texturing
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Takes textures and projects them in world-space onto the surface.

## Inputs

- `XY Scale` [FunctionInput_Scalar] - Scale of XY projected texture in world units
- `XZ Scale` [FunctionInput_Scalar] - Scale of XZ projected texture in world units
- `YZ Scale` [FunctionInput_Scalar] - Scale of YZ projected texture in world units
- `XY Texture` [FunctionInput_Texture2D]
- `XZ Texture` [FunctionInput_Texture2D]
- `YZ Texture` [FunctionInput_Texture2D]
- `Normal` [FunctionInput_Vector3] - Brings the normal into the blend.
- `Blend Exponent Y Z` [FunctionInput_Scalar] - Sets the power for the blend between the sides.
- `Single Texture` [FunctionInput_StaticBool] - When true, only uses the texture in the XY entry for all three projections.
- `Blend Exponent X` [FunctionInput_Scalar] - Sets the power for the blend between the top and the sides.
- `Blend Mult X` [FunctionInput_Scalar] - Multiplies the blend between the top and the sides
- `Blend Mult Y Z` [FunctionInput_Scalar] - Multiplies the blend between the side projections
- `WorldPosition` [FunctionInput_Vector3] - Optional input for altered World position coordinates

## Outputs

- `XYZ Output` - 3 way blended texturing
- `XY` - XY Plane texturing
- `XZ` - XZ Plane Texturing
- `YZ` - YZ Plane Texturing

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output XYZ Output | XYZ Output |
| 1 | expression | `MaterialExpressionComponentMask` | Mask ( R G ) |  |
| 2 | expression | `MaterialExpressionComponentMask` | Mask ( R B ) |  |
| 3 | expression | `MaterialExpressionComponentMask` | Mask ( G B ) |  |
| 4 | function_input | `MaterialExpressionFunctionInput` | Input XY Scale (Scalar) | XY Scale |
| 5 | function_input | `MaterialExpressionFunctionInput` | Input XZ Scale (Scalar) | XZ Scale |
| 6 | function_input | `MaterialExpressionFunctionInput` | Input YZ Scale (Scalar) | YZ Scale |
| 7 | expression | `MaterialExpressionDivide` | Divide |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | function_input | `MaterialExpressionFunctionInput` | Input XY Texture (Texture2D) | XY Texture |
| 11 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 12 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 13 | function_input | `MaterialExpressionFunctionInput` | Input XZ Texture (Texture2D) | XZ Texture |
| 14 | function_input | `MaterialExpressionFunctionInput` | Input YZ Texture (Texture2D) | YZ Texture |
| 15 | expression | `MaterialExpressionTextureSample` | Texture Sample |  |
| 16 | expression | `MaterialExpressionTextureObject` | Texture Object |  |
| 17 | expression | `MaterialExpressionConstant3Vector` | 0,0,1 |  |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Normal (Vector3) | Normal |
| 19 | expression | `MaterialExpressionTransform` | Tangent Space to World Space TransformVector |  |
| 20 | expression | `MaterialExpressionAbs` | Abs |  |
| 21 | expression | `MaterialExpressionPower` | Power |  |
| 22 | function_input | `MaterialExpressionFunctionInput` | Input Blend Exponent Y Z (Scalar) | Blend Exponent Y Z |
| 23 | expression | `MaterialExpressionComponentMask` | Mask ( R ) |  |
| 24 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 25 | expression | `MaterialExpressionLinearInterpolate` | Lerp |  |
| 26 | expression | `MaterialExpressionComponentMask` | Mask ( B ) |  |
| 27 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 28 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 29 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Single Texture (StaticBool) | Single Texture |
| 31 | function_input | `MaterialExpressionFunctionInput` | Input Blend Exponent X (Scalar) | Blend Exponent X |
| 32 | expression | `MaterialExpressionPower` | Power |  |
| 33 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 34 | function_input | `MaterialExpressionFunctionInput` | Input Blend Mult X (Scalar) | Blend Mult X |
| 35 | function_input | `MaterialExpressionFunctionInput` | Input Blend Mult Y Z (Scalar) | Blend Mult Y Z |
| 36 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 37 | function_output | `MaterialExpressionFunctionOutput` | Output XY | XY |
| 38 | function_output | `MaterialExpressionFunctionOutput` | Output XZ | XZ |
| 39 | function_output | `MaterialExpressionFunctionOutput` | Output YZ | YZ |
| 40 | expression | `MaterialExpressionWorldPosition` | Absolute World Position (Excluding Material Offsets) |  |
| 41 | function_input | `MaterialExpressionFunctionInput` | Input WorldPosition (Vector3) | WorldPosition |
| 42 | expression | `MaterialExpressionClamp` | Clamp |  |
| 43 | expression | `MaterialExpressionClamp` | Clamp |  |

## Exact Input Wiring

### [0] Output XYZ Output

- `A` <= [25] Lerp output 0

### [1] Mask ( R G )

- `Input` <= [41] Input WorldPosition (Vector3) output 0

### [2] Mask ( R B )

- `Input` <= [41] Input WorldPosition (Vector3) output 0

### [3] Mask ( G B )

- `Input` <= [41] Input WorldPosition (Vector3) output 0

### [7] Divide

- `A` <= [1] Mask ( R G ) output 0
- `B` <= [4] Input XY Scale (Scalar) output 0

### [8] Divide

- `A` <= [2] Mask ( R B ) output 0
- `B` <= [5] Input XZ Scale (Scalar) output 0

### [9] Divide

- `A` <= [3] Mask ( G B ) output 0
- `B` <= [6] Input YZ Scale (Scalar) output 0

### [10] Input XY Texture (Texture2D)

- `Preview` <= [16] Texture Object output 0

### [11] Texture Sample

- `Coordinates` <= [7] Divide output 0
- `TextureObject` <= [10] Input XY Texture (Texture2D) output 0

### [12] Texture Sample

- `Coordinates` <= [8] Divide output 0
- `TextureObject` <= [27] Switch output 0

### [13] Input XZ Texture (Texture2D)

- `Preview` <= [16] Texture Object output 0

### [14] Input YZ Texture (Texture2D)

- `Preview` <= [16] Texture Object output 0

### [15] Texture Sample

- `Coordinates` <= [9] Divide output 0
- `TextureObject` <= [28] Switch output 0

### [18] Input Normal (Vector3)

- `Preview` <= [17] 0,0,1 output 0

### [19] Tangent Space to World Space TransformVector

- `Input` <= [18] Input Normal (Vector3) output 0

### [20] Abs

- `Input` <= [19] Tangent Space to World Space TransformVector output 0

### [21] Power

- `Base` <= [20] Abs output 0
- `Exponent` <= [22] Input Blend Exponent Y Z (Scalar) output 0

### [23] Mask ( R )

- `Input` <= [21] Power output 0

### [24] Lerp

- `A` <= [12] Texture Sample output 0
- `B` <= [15] Texture Sample output 0
- `Alpha` <= [42] Clamp output 0

### [25] Lerp

- `A` <= [24] Lerp output 0
- `B` <= [11] Texture Sample output 0
- `Alpha` <= [43] Clamp output 0

### [26] Mask ( B )

- `Input` <= [32] Power output 0

### [27] Switch

- `A` <= [10] Input XY Texture (Texture2D) output 0
- `B` <= [13] Input XZ Texture (Texture2D) output 0
- `Value` <= [30] Input Single Texture (StaticBool) output 0

### [28] Switch

- `A` <= [10] Input XY Texture (Texture2D) output 0
- `B` <= [14] Input YZ Texture (Texture2D) output 0
- `Value` <= [30] Input Single Texture (StaticBool) output 0

### [30] Input Single Texture (StaticBool)

- `Preview` <= [29] Static Bool (False) output 0

### [32] Power

- `Base` <= [20] Abs output 0
- `Exponent` <= [31] Input Blend Exponent X (Scalar) output 0

### [33] Multiply

- `A` <= [26] Mask ( B ) output 0
- `B` <= [34] Input Blend Mult X (Scalar) output 0

### [36] Multiply

- `A` <= [23] Mask ( R ) output 0
- `B` <= [35] Input Blend Mult Y Z (Scalar) output 0

### [37] Output XY

- `A` <= [11] Texture Sample output 0

### [38] Output XZ

- `A` <= [12] Texture Sample output 0

### [39] Output YZ

- `A` <= [15] Texture Sample output 0

### [41] Input WorldPosition (Vector3)

- `Preview` <= [40] Absolute World Position (Excluding Material Offsets) output 0

### [42] Clamp

- `Input` <= [36] Multiply output 0

### [43] Clamp

- `Input` <= [33] Multiply output 0

## Connection List

- [25] Lerp output 0 -> Output XYZ Output.A
- [41] Input WorldPosition (Vector3) output 0 -> Mask ( R G ).Input
- [41] Input WorldPosition (Vector3) output 0 -> Mask ( R B ).Input
- [41] Input WorldPosition (Vector3) output 0 -> Mask ( G B ).Input
- [1] Mask ( R G ) output 0 -> Divide.A
- [4] Input XY Scale (Scalar) output 0 -> Divide.B
- [2] Mask ( R B ) output 0 -> Divide.A
- [5] Input XZ Scale (Scalar) output 0 -> Divide.B
- [3] Mask ( G B ) output 0 -> Divide.A
- [6] Input YZ Scale (Scalar) output 0 -> Divide.B
- [16] Texture Object output 0 -> Input XY Texture (Texture2D).Preview
- [7] Divide output 0 -> Texture Sample.Coordinates
- [10] Input XY Texture (Texture2D) output 0 -> Texture Sample.TextureObject
- [8] Divide output 0 -> Texture Sample.Coordinates
- [27] Switch output 0 -> Texture Sample.TextureObject
- [16] Texture Object output 0 -> Input XZ Texture (Texture2D).Preview
- [16] Texture Object output 0 -> Input YZ Texture (Texture2D).Preview
- [9] Divide output 0 -> Texture Sample.Coordinates
- [28] Switch output 0 -> Texture Sample.TextureObject
- [17] 0,0,1 output 0 -> Input Normal (Vector3).Preview
- [18] Input Normal (Vector3) output 0 -> Tangent Space to World Space TransformVector.Input
- [19] Tangent Space to World Space TransformVector output 0 -> Abs.Input
- [20] Abs output 0 -> Power.Base
- [22] Input Blend Exponent Y Z (Scalar) output 0 -> Power.Exponent
- [21] Power output 0 -> Mask ( R ).Input
- [12] Texture Sample output 0 -> Lerp.A
- [15] Texture Sample output 0 -> Lerp.B
- [42] Clamp output 0 -> Lerp.Alpha
- [24] Lerp output 0 -> Lerp.A
- [11] Texture Sample output 0 -> Lerp.B
- [43] Clamp output 0 -> Lerp.Alpha
- [32] Power output 0 -> Mask ( B ).Input
- [10] Input XY Texture (Texture2D) output 0 -> Switch.A
- [13] Input XZ Texture (Texture2D) output 0 -> Switch.B
- [30] Input Single Texture (StaticBool) output 0 -> Switch.Value
- [10] Input XY Texture (Texture2D) output 0 -> Switch.A
- [14] Input YZ Texture (Texture2D) output 0 -> Switch.B
- [30] Input Single Texture (StaticBool) output 0 -> Switch.Value
- [29] Static Bool (False) output 0 -> Input Single Texture (StaticBool).Preview
- [20] Abs output 0 -> Power.Base
- [31] Input Blend Exponent X (Scalar) output 0 -> Power.Exponent
- [26] Mask ( B ) output 0 -> Multiply.A
- [34] Input Blend Mult X (Scalar) output 0 -> Multiply.B
- [23] Mask ( R ) output 0 -> Multiply.A
- [35] Input Blend Mult Y Z (Scalar) output 0 -> Multiply.B
- [11] Texture Sample output 0 -> Output XY.A
- [12] Texture Sample output 0 -> Output XZ.A
- [15] Texture Sample output 0 -> Output YZ.A
- [40] Absolute World Position (Excluding Material Offsets) output 0 -> Input WorldPosition (Vector3).Preview
- [36] Multiply output 0 -> Clamp.Input
- [33] Multiply output 0 -> Clamp.Input

## Output Trace Roots

- `XYZ Output` is driven by [25] Lerp
- `XY` is driven by [11] Texture Sample
- `XZ` is driven by [12] Texture Sample
- `YZ` is driven by [15] Texture Sample

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


