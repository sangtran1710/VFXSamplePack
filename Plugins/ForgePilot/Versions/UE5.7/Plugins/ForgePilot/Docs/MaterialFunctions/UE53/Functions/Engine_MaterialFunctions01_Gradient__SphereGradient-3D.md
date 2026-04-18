# SphereGradient-3D

- Path: `/Engine/Functions/Engine_MaterialFunctions01/Gradient/SphereGradient-3D.SphereGradient-3D`
- Category: Engine_MaterialFunctions01/Gradient
- Use: Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Inspect status: PASS

## Description

Generates a gradient representing the depth of a shere centered on the desired point. 

## Inputs

- `Radius` [FunctionInput_Scalar]
- `Location` [FunctionInput_Vector3]
- `Offset` [FunctionInput_Vector3]
- `Calculate Camera Inside` [FunctionInput_StaticBool] - Extra calculations need to be done to calculate sphere intersection when the camera is inside and use real world depths for intersection.
- `Depth Biased Alpha` [FunctionInput_StaticBool] - Extra calculations need to be done to calculate sphere intersection when the camera is inside, so enable only when needed.
- `FadeDistance` [FunctionInput_Scalar] - Extra calculations need to be done to calculate sphere intersection when the camera is inside, so enable only when needed.

## Outputs

- `Result 0-1`
- `Result Diameter`
- `Result Radius`

## Internal Nodes

| Index | Kind | Class | Caption | Semantic Name |
|---:|---|---|---|---|
| 0 | expression | `MaterialExpressionAdd` | Add |  |
| 1 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 2 | expression | `MaterialExpressionAdd` | Add |  |
| 3 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 4 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 5 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 6 | expression | `MaterialExpressionSquareRoot` | Sqrt |  |
| 7 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 8 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 9 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 10 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 11 | expression | `MaterialExpressionAdd` | Add |  |
| 12 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 13 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 14 | function_output | `MaterialExpressionFunctionOutput` | Output Result 0-1 | Result 0-1 |
| 15 | function_output | `MaterialExpressionFunctionOutput` | Output Result Diameter | Result Diameter |
| 16 | expression | `MaterialExpressionDivide` | Divide |  |
| 17 | function_input | `MaterialExpressionFunctionInput` | Input Radius (Scalar) | Radius |
| 18 | function_input | `MaterialExpressionFunctionInput` | Input Location (Vector3) | Location |
| 19 | function_input | `MaterialExpressionFunctionInput` | Input Offset (Vector3) | Offset |
| 20 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 21 | expression | `MaterialExpressionConstant` | 0 |  |
| 22 | expression | `MaterialExpressionIf` | If |  |
| 23 | expression | `MaterialExpressionMultiply` | Multiply(,2) |  |
| 24 | expression | `MaterialExpressionDistance` | Distance |  |
| 25 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 26 | expression | `MaterialExpressionConstant` | 0 |  |
| 27 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 28 | expression | `MaterialExpressionDistance` | Distance |  |
| 29 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 30 | expression | `MaterialExpressionIf` | If |  |
| 31 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 32 | custom | `MaterialExpressionCustom` | Custom |  |
| 33 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 34 | expression | `MaterialExpressionNormalize` | Normalize |  |
| 35 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 36 | expression | `MaterialExpressionIf` | If |  |
| 37 | expression | `MaterialExpressionConstant` | 0 |  |
| 38 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 39 | function_input | `MaterialExpressionFunctionInput` | Input Calculate Camera Inside (StaticBool) | Calculate Camera Inside |
| 40 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 41 | function_output | `MaterialExpressionFunctionOutput` | Output Result Radius | Result Radius |
| 42 | expression | `MaterialExpressionDivide` | Divide(,2) |  |
| 43 | expression | `MaterialExpressionClamp` | Clamp |  |
| 44 | expression | `MaterialExpressionSceneDepth` | Scene Depth |  |
| 45 | expression | `MaterialExpressionActorPositionWS` | Actor Position (Absolute) |  |
| 46 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 47 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 48 | expression | `MaterialExpressionDistance` | Distance |  |
| 49 | expression | `MaterialExpressionConstant` | 0 |  |
| 50 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 51 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 52 | expression | `MaterialExpressionDivide` | Divide |  |
| 53 | expression | `MaterialExpressionDotProduct` | Dot |  |
| 54 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 55 | function_call | `MaterialExpressionMaterialFunctionCall` | CameraDirectionVector |  |
| 56 | expression | `MaterialExpressionMultiply` | Multiply(,1) |  |
| 57 | expression | `MaterialExpressionAbs` | Abs |  |
| 58 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 59 | function_input | `MaterialExpressionFunctionInput` | Input Depth Biased Alpha (StaticBool) | Depth Biased Alpha |
| 60 | function_input | `MaterialExpressionFunctionInput` | Input FadeDistance (Scalar) | FadeDistance |
| 61 | expression | `MaterialExpressionStaticSwitch` | Switch |  |
| 62 | expression | `MaterialExpressionStaticBool` | Static Bool (False) |  |
| 63 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 64 | expression | `MaterialExpressionMin` | Min |  |
| 65 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 66 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 67 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 68 | expression | `MaterialExpressionCameraVectorWS` | Camera Vector |  |
| 69 | expression | `MaterialExpressionAdd` | Add |  |
| 70 | expression | `MaterialExpressionDistance` | Distance |  |
| 71 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 72 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 73 | expression | `MaterialExpressionMax` | Max(,0) |  |
| 74 | expression | `MaterialExpressionCameraPositionWS` | Camera Position |  |
| 75 | expression | `MaterialExpressionSubtract` | Subtract |  |
| 76 | custom | `MaterialExpressionCustom` | Custom |  |
| 77 | function_call | `MaterialExpressionMaterialFunctionCall` | WorldPositionBehindTranslucency |  |
| 78 | expression | `MaterialExpressionConstant` | 1 |  |
| 79 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 80 | expression | `MaterialExpressionMultiply` | Multiply |  |
| 81 | expression | `MaterialExpressionStaticSwitch` | Switch |  |

## Exact Input Wiring

### [0] Add

- `A` <= [1] Multiply output 0
- `B` <= [27] Camera Position output 0

### [1] Multiply

- `A` <= [2] Add output 0
- `B` <= [35] Camera Vector output 0

### [2] Add

- `A` <= [8] Dot output 0
- `B` <= [3] Sqrt output 0

### [3] Sqrt

- `Input` <= [4] Subtract output 0

### [4] Subtract

- `A` <= [13] Multiply output 0
- `B` <= [5] Multiply output 0

### [5] Multiply

- `A` <= [6] Sqrt output 0
- `B` <= [6] Sqrt output 0

### [6] Sqrt

- `Input` <= [7] Subtract output 0

### [7] Subtract

- `A` <= [33] Multiply output 0
- `B` <= [31] Multiply output 0

### [8] Dot

- `A` <= [9] Subtract output 0
- `B` <= [25] Camera Vector output 0

### [9] Subtract

- `A` <= [10] Camera Position output 0
- `B` <= [11] Add output 0

### [11] Add

- `A` <= [18] Input Location (Vector3) output 0
- `B` <= [19] Input Offset (Vector3) output 0

### [13] Multiply

- `A` <= [17] Input Radius (Scalar) output 0
- `B` <= [17] Input Radius (Scalar) output 0

### [14] Output Result 0-1

- `A` <= [63] Multiply output 0

### [15] Output Result Diameter

- `A` <= [79] Multiply output 0

### [16] Divide

- `A` <= [36] If output 0
- `B` <= [20] Multiply(,2) output 0

### [18] Input Location (Vector3)

- `Preview` <= [12] Actor Position (Absolute) output 0

### [20] Multiply(,2)

- `A` <= [17] Input Radius (Scalar) output 0

### [22] If

- `A` <= [24] Distance output 0
- `B` <= [17] Input Radius (Scalar) output 0
- `AGreaterThanB` <= [73] Max(,0) output 0
- `AEqualsB` <= [73] Max(,0) output 0
- `ALessThanB` <= [64] Min output 0

### [23] Multiply(,2)

- `A` <= [3] Sqrt output 0

### [24] Distance

- `A` <= [11] Add output 0
- `B` <= [10] Camera Position output 0

### [28] Distance

- `A` <= [29] Camera Position output 0
- `B` <= [0] Add output 0

### [30] If

- `A` <= [8] Dot output 0
- `B` <= [26] 0 output 0
- `AGreaterThanB` <= [23] Multiply(,2) output 0
- `AEqualsB` <= [23] Multiply(,2) output 0
- `ALessThanB` <= [21] 0 output 0

### [31] Multiply

- `A` <= [8] Dot output 0
- `B` <= [8] Dot output 0

### [33] Multiply

- `A` <= [32] Custom output 0
- `B` <= [32] Custom output 0

### [34] Normalize

- `VectorInput` <= [25] Camera Vector output 0

### [36] If

- `A` <= [6] Sqrt output 0
- `B` <= [17] Input Radius (Scalar) output 0
- `AGreaterThanB` <= [37] 0 output 0
- `AEqualsB` <= [37] 0 output 0
- `ALessThanB` <= [38] Switch output 0

### [38] Switch

- `A` <= [22] If output 0
- `B` <= [30] If output 0
- `Value` <= [39] Input Calculate Camera Inside (StaticBool) output 0

### [39] Input Calculate Camera Inside (StaticBool)

- `Preview` <= [40] Static Bool (False) output 0

### [41] Output Result Radius

- `A` <= [80] Multiply output 0

### [42] Divide(,2)

- `A` <= [36] If output 0

### [43] Clamp

- `Input` <= [52] Divide output 0

### [47] Subtract

- `A` <= [45] Actor Position (Absolute) output 0
- `B` <= [46] Camera Position output 0

### [48] Distance

- `A` <= [47] Subtract output 0
- `B` <= [49] 0 output 0

### [50] Subtract

- `A` <= [58] Multiply output 0
- `B` <= [56] Multiply(,1) output 0

### [51] Subtract

- `A` <= [44] Scene Depth output 0
- `B` <= [50] Subtract output 0

### [52] Divide

- `A` <= [51] Subtract output 0
- `B` <= [60] Input FadeDistance (Scalar) output 0

### [53] Dot

- `A` <= [54] Camera Vector output 0
- `B` <= [55] CameraDirectionVector output 0

### [56] Multiply(,1)

- `A` <= [42] Divide(,2) output 0

### [57] Abs

- `Input` <= [53] Dot output 0

### [58] Multiply

- `A` <= [48] Distance output 0
- `B` <= [57] Abs output 0

### [59] Input Depth Biased Alpha (StaticBool)

- `Preview` <= [62] Static Bool (False) output 0

### [61] Switch

- `A` <= [81] Switch output 0
- `B` <= [78] 1 output 0
- `Value` <= [59] Input Depth Biased Alpha (StaticBool) output 0

### [63] Multiply

- `A` <= [61] Switch output 0
- `B` <= [16] Divide output 0

### [64] Min

- `A` <= [28] Distance output 0
- `B` <= [76] Custom output 0

### [65] Subtract

- `A` <= [8] Dot output 0
- `B` <= [3] Sqrt output 0

### [66] Multiply

- `A` <= [65] Subtract output 0
- `B` <= [68] Camera Vector output 0

### [69] Add

- `A` <= [66] Multiply output 0
- `B` <= [67] Camera Position output 0

### [70] Distance

- `A` <= [71] Camera Position output 0
- `B` <= [69] Add output 0

### [72] Subtract

- `A` <= [64] Min output 0
- `B` <= [70] Distance output 0

### [73] Max(,0)

- `A` <= [72] Subtract output 0

### [75] Subtract

- `A` <= [74] Camera Position output 0
- `B` <= [77] WorldPositionBehindTranslucency output 0

### [79] Multiply

- `A` <= [61] Switch output 0
- `B` <= [36] If output 0

### [80] Multiply

- `A` <= [61] Switch output 0
- `B` <= [42] Divide(,2) output 0

### [81] Switch

- `A` <= [78] 1 output 0
- `B` <= [43] Clamp output 0
- `Value` <= [39] Input Calculate Camera Inside (StaticBool) output 0

## Connection List

- [1] Multiply output 0 -> Add.A
- [27] Camera Position output 0 -> Add.B
- [2] Add output 0 -> Multiply.A
- [35] Camera Vector output 0 -> Multiply.B
- [8] Dot output 0 -> Add.A
- [3] Sqrt output 0 -> Add.B
- [4] Subtract output 0 -> Sqrt.Input
- [13] Multiply output 0 -> Subtract.A
- [5] Multiply output 0 -> Subtract.B
- [6] Sqrt output 0 -> Multiply.A
- [6] Sqrt output 0 -> Multiply.B
- [7] Subtract output 0 -> Sqrt.Input
- [33] Multiply output 0 -> Subtract.A
- [31] Multiply output 0 -> Subtract.B
- [9] Subtract output 0 -> Dot.A
- [25] Camera Vector output 0 -> Dot.B
- [10] Camera Position output 0 -> Subtract.A
- [11] Add output 0 -> Subtract.B
- [18] Input Location (Vector3) output 0 -> Add.A
- [19] Input Offset (Vector3) output 0 -> Add.B
- [17] Input Radius (Scalar) output 0 -> Multiply.A
- [17] Input Radius (Scalar) output 0 -> Multiply.B
- [63] Multiply output 0 -> Output Result 0-1.A
- [79] Multiply output 0 -> Output Result Diameter.A
- [36] If output 0 -> Divide.A
- [20] Multiply(,2) output 0 -> Divide.B
- [12] Actor Position (Absolute) output 0 -> Input Location (Vector3).Preview
- [17] Input Radius (Scalar) output 0 -> Multiply(,2).A
- [24] Distance output 0 -> If.A
- [17] Input Radius (Scalar) output 0 -> If.B
- [73] Max(,0) output 0 -> If.AGreaterThanB
- [73] Max(,0) output 0 -> If.AEqualsB
- [64] Min output 0 -> If.ALessThanB
- [3] Sqrt output 0 -> Multiply(,2).A
- [11] Add output 0 -> Distance.A
- [10] Camera Position output 0 -> Distance.B
- [29] Camera Position output 0 -> Distance.A
- [0] Add output 0 -> Distance.B
- [8] Dot output 0 -> If.A
- [26] 0 output 0 -> If.B
- [23] Multiply(,2) output 0 -> If.AGreaterThanB
- [23] Multiply(,2) output 0 -> If.AEqualsB
- [21] 0 output 0 -> If.ALessThanB
- [8] Dot output 0 -> Multiply.A
- [8] Dot output 0 -> Multiply.B
- [32] Custom output 0 -> Multiply.A
- [32] Custom output 0 -> Multiply.B
- [25] Camera Vector output 0 -> Normalize.VectorInput
- [6] Sqrt output 0 -> If.A
- [17] Input Radius (Scalar) output 0 -> If.B
- [37] 0 output 0 -> If.AGreaterThanB
- [37] 0 output 0 -> If.AEqualsB
- [38] Switch output 0 -> If.ALessThanB
- [22] If output 0 -> Switch.A
- [30] If output 0 -> Switch.B
- [39] Input Calculate Camera Inside (StaticBool) output 0 -> Switch.Value
- [40] Static Bool (False) output 0 -> Input Calculate Camera Inside (StaticBool).Preview
- [80] Multiply output 0 -> Output Result Radius.A
- [36] If output 0 -> Divide(,2).A
- [52] Divide output 0 -> Clamp.Input
- [45] Actor Position (Absolute) output 0 -> Subtract.A
- [46] Camera Position output 0 -> Subtract.B
- [47] Subtract output 0 -> Distance.A
- [49] 0 output 0 -> Distance.B
- [58] Multiply output 0 -> Subtract.A
- [56] Multiply(,1) output 0 -> Subtract.B
- [44] Scene Depth output 0 -> Subtract.A
- [50] Subtract output 0 -> Subtract.B
- [51] Subtract output 0 -> Divide.A
- [60] Input FadeDistance (Scalar) output 0 -> Divide.B
- [54] Camera Vector output 0 -> Dot.A
- [55] CameraDirectionVector output 0 -> Dot.B
- [42] Divide(,2) output 0 -> Multiply(,1).A
- [53] Dot output 0 -> Abs.Input
- [48] Distance output 0 -> Multiply.A
- [57] Abs output 0 -> Multiply.B
- [62] Static Bool (False) output 0 -> Input Depth Biased Alpha (StaticBool).Preview
- [81] Switch output 0 -> Switch.A
- [78] 1 output 0 -> Switch.B
- [59] Input Depth Biased Alpha (StaticBool) output 0 -> Switch.Value
- [61] Switch output 0 -> Multiply.A
- [16] Divide output 0 -> Multiply.B
- [28] Distance output 0 -> Min.A
- [76] Custom output 0 -> Min.B
- [8] Dot output 0 -> Subtract.A
- [3] Sqrt output 0 -> Subtract.B
- [65] Subtract output 0 -> Multiply.A
- [68] Camera Vector output 0 -> Multiply.B
- [66] Multiply output 0 -> Add.A
- [67] Camera Position output 0 -> Add.B
- [71] Camera Position output 0 -> Distance.A
- [69] Add output 0 -> Distance.B
- [64] Min output 0 -> Subtract.A
- [70] Distance output 0 -> Subtract.B
- [72] Subtract output 0 -> Max(,0).A
- [74] Camera Position output 0 -> Subtract.A
- [77] WorldPositionBehindTranslucency output 0 -> Subtract.B
- [61] Switch output 0 -> Multiply.A
- [36] If output 0 -> Multiply.B
- [61] Switch output 0 -> Multiply.A
- [42] Divide(,2) output 0 -> Multiply.B
- [78] 1 output 0 -> Switch.A
- [43] Clamp output 0 -> Switch.B
- [39] Input Calculate Camera Inside (StaticBool) output 0 -> Switch.Value

## Output Trace Roots

- `Result 0-1` is driven by [63] Multiply
- `Result Diameter` is driven by [79] Multiply
- `Result Radius` is driven by [80] Multiply

## Agent Guidance

- Instantiate with dd_material_expression using expression_type='MaterialFunctionCall' and this function path.
- Inspect with get_material_function_info before production wiring.
- Wire call-site inputs by 	arget_input_name; do not guess pin order from screenshots.
- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap.
