# Spherical-Cap-Intersection

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Shading/Spherical-Cap-Intersection.Spherical-Cap-Intersection`
- Category: Engine_MaterialFunctions01/Shading
- Use: Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Mathematically computes the area of two spherecap intersections. This will return the occlusion ratio given a sphere occluder and a light angle. Returns the inverse, so it returns 1 when there was no occlusion of the light cone and 0 when there is full occlusion of the light cone. Units are specified in degrees. Note that this function is used to generate static textures for use in final shading, not meant to be used in realtime.

## Inputs

- `Angle Between Cones` [FunctionInput_Scalar] - This is the angle between the vector to the occluder center and the light vector. In Degrees.
- `View Occlusion Angle` [FunctionInput_Scalar] - This is the angle of occlusion from the occluder. As the occluder gets closer to the camera, occlusion reaches 1. As the occluder gets infinitely far from the camera, the occlusion approaches 0. In Degrees.
- `Light Source Angle` [FunctionInput_Scalar] - If you only care about the spherecap intersection area, it does not matter if you consider this a light vector or just one of the two cone angles. When using the illumination output, this cone determines the illumination fraction. A larger source angle causes a blurrier penumbra. Specified in Degrees.
- `Use Smoothstep` [FunctionInput_StaticBool] - When True, area intersection will be approximated using smoothstep instead of being calculated analytically. Smoothstep gives cleaner results with small light source angles (less than 1.0).
- `Use Radians` [FunctionInput_StaticBool] - By default the angles are read as degrees. This option lets you specify the angles in radians to save a few instructions or if you are just hardcore.

## Outputs

- `Illumination` - Returns 1 when there was no occlusion of the light cone and 0 when there is full occlusion of the light cone.
- `Area of intersection` - Returns 1 when there was no occlusion of the light cone and 0 when there is full occlusion of the light cone.

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | function_output | `MaterialExpressionFunctionOutput` | Output Illumination | Illumination |
| 1 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 2 | expression | `MaterialExpressionDivide` | Divide |  |
| 3 | expression | `MaterialExpressionCosine` | Cosine |  |
| 4 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 5 | expression | `MaterialExpressionConstant` | 2 |  |
| 6 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 7 | custom | `MaterialExpressionCustom` | SphereCap Intersection Area |  |
| 8 | expression | `MaterialExpressionDivide` | Divide |  |
| 9 | expression | `MaterialExpressionDivide` | Divide |  |
| 10 | expression | `MaterialExpressionConstant` | 57.3 |  |
| 11 | expression | `MaterialExpressionDivide` | Divide |  |
| 12 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 13 | expression | `MaterialExpressionIf` | If |  |
| 14 | expression | `MaterialExpressionMin` | Min |  |
| 15 | expression | `MaterialExpressionMax` | Max |  |
| 16 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 17 | expression | `MaterialExpressionIf` | If |  |
| 18 | expression | `MaterialExpressionAdd` | Add |  |
| 19 | expression | `MaterialExpressionConstant` | 0 |  |
| 20 | expression | `MaterialExpressionCosine` | Cosine |  |
| 21 | function_call | `MaterialExpressionMaterialFunctionCall` | Pi |  |
| 22 | expression | `MaterialExpressionConstant` | 2 |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 24 | expression | `MaterialExpressionOneMinus` | 1-x |  |
| 25 | function_input | `MaterialExpressionFunctionInput` | Input Angle Between Cones (Scalar) | Angle Between Cones |
| 26 | function_input | `MaterialExpressionFunctionInput` | Input View Occlusion Angle (Scalar) | View Occlusion Angle |
| 27 | function_input | `MaterialExpressionFunctionInput` | Input Light Source Angle (Scalar) | Light Source Angle |
| 28 | custom | `MaterialExpressionCustom` | Smoothstep version |  |
| 29 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 30 | function_input | `MaterialExpressionFunctionInput` | Input Use Smoothstep (StaticBool) | Use Smoothstep |
| 31 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 32 | function_output | `MaterialExpressionFunctionOutput` | Output Area of intersection | Area of intersection |
| 33 | function_input | `MaterialExpressionFunctionInput` | Input Use Radians (StaticBool) | Use Radians |
| 34 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 35 | expression | `MaterialExpressionConstant` | 1 |  |

## Exact Input Wiring

### [0] Output Illumination

- `A` <= [1] 1-x output 0

### [1] 1-x

- `Input` <= [2] Divide output 0

### [2] Divide

- `A` <= [13] If output 0
- `B` <= [23] Multiply output 0

### [3] Cosine

- `Input` <= [14] Min output 0

### [6] Multiply

- `A` <= [12] 1-x output 0
- `B` <= [4] Pi output 0

### [8] Divide

- `A` <= [26] Input View Occlusion Angle (Scalar) output 0
- `B` <= [34] Switch output 0

### [9] Divide

- `A` <= [27] Input Light Source Angle (Scalar) output 0
- `B` <= [34] Switch output 0

### [11] Divide

- `A` <= [25] Input Angle Between Cones (Scalar) output 0
- `B` <= [34] Switch output 0

### [12] 1-x

- `Input` <= [3] Cosine output 0

### [13] If

- `A` <= [14] Min output 0
- `B` <= [16] Subtract output 0
- `AGreaterThanB` <= [17] If output 0
- `AEqualsB` <= [6] Multiply output 0
- `ALessThanB` <= [6] Multiply output 0

### [14] Min

- `A` <= [8] Divide output 0
- `B` <= [9] Divide output 0

### [15] Max

- `A` <= [8] Divide output 0
- `B` <= [9] Divide output 0

### [16] Subtract

- `A` <= [15] Max output 0
- `B` <= [11] Divide output 0

### [17] If

- `A` <= [18] Add output 0
- `B` <= [11] Divide output 0
- `AGreaterThanB` <= [29] Switch output 0
- `AEqualsB` <= [19] 0 output 0
- `ALessThanB` <= [19] 0 output 0

### [18] Add

- `A` <= [8] Divide output 0
- `B` <= [9] Divide output 0

### [20] Cosine

- `Input` <= [9] Divide output 0

### [23] Multiply

- `A` <= [24] 1-x output 0
- `B` <= [21] Pi output 0

### [24] 1-x

- `Input` <= [20] Cosine output 0

### [29] Switch

- `A` <= [28] Smoothstep version output 0
- `B` <= [7] SphereCap Intersection Area output 0
- `Value` <= [30] Input Use Smoothstep (StaticBool) output 0

### [30] Input Use Smoothstep (StaticBool)

- `Preview` <= [31] Static Bool (False) output 0

### [32] Output Area of intersection

- `A` <= [13] If output 0

### [33] Input Use Radians (StaticBool)

- `Preview` <= [31] Static Bool (False) output 0

### [34] Switch

- `A` <= [35] 1 output 0
- `B` <= [10] 57.3 output 0
- `Value` <= [33] Input Use Radians (StaticBool) output 0

## Connection List

- [1] 1-x output 0 -> Output Illumination.A
- [2] Divide output 0 -> 1-x.Input
- [13] If output 0 -> Divide.A
- [23] Multiply output 0 -> Divide.B
- [14] Min output 0 -> Cosine.Input
- [12] 1-x output 0 -> Multiply.A
- [4] Pi output 0 -> Multiply.B
- [26] Input View Occlusion Angle (Scalar) output 0 -> Divide.A
- [34] Switch output 0 -> Divide.B
- [27] Input Light Source Angle (Scalar) output 0 -> Divide.A
- [34] Switch output 0 -> Divide.B
- [25] Input Angle Between Cones (Scalar) output 0 -> Divide.A
- [34] Switch output 0 -> Divide.B
- [3] Cosine output 0 -> 1-x.Input
- [14] Min output 0 -> If.A
- [16] Subtract output 0 -> If.B
- [17] If output 0 -> If.AGreaterThanB
- [6] Multiply output 0 -> If.AEqualsB
- [6] Multiply output 0 -> If.ALessThanB
- [8] Divide output 0 -> Min.A
- [9] Divide output 0 -> Min.B
- [8] Divide output 0 -> Max.A
- [9] Divide output 0 -> Max.B
- [15] Max output 0 -> Subtract.A
- [11] Divide output 0 -> Subtract.B
- [18] Add output 0 -> If.A
- [11] Divide output 0 -> If.B
- [29] Switch output 0 -> If.AGreaterThanB
- [19] 0 output 0 -> If.AEqualsB
- [19] 0 output 0 -> If.ALessThanB
- [8] Divide output 0 -> Add.A
- [9] Divide output 0 -> Add.B
- [9] Divide output 0 -> Cosine.Input
- [24] 1-x output 0 -> Multiply.A
- [21] Pi output 0 -> Multiply.B
- [20] Cosine output 0 -> 1-x.Input
- [28] Smoothstep version output 0 -> Switch.A
- [7] SphereCap Intersection Area output 0 -> Switch.B
- [30] Input Use Smoothstep (StaticBool) output 0 -> Switch.Value
- [31] Static Bool (False) output 0 -> Input Use Smoothstep (StaticBool).Preview
- [13] If output 0 -> Output Area of intersection.A
- [31] Static Bool (False) output 0 -> Input Use Radians (StaticBool).Preview
- [35] 1 output 0 -> Switch.A
- [10] 57.3 output 0 -> Switch.B
- [33] Input Use Radians (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `Illumination` is driven by [1] 1-x
- `Area of intersection` is driven by [13] If

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.


