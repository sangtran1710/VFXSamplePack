# Engine_MaterialFunctions02/Math

Engine: Unreal Engine 5.3
Function count: 26

## Agent Use

Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.

Risk: Use get_material_function_info and wire by target_input_name for safer automation.

## Functions

### AddComponents

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/AddComponents.AddComponents`
- Deep graph doc: [AddComponents](Functions/Engine_MaterialFunctions02_Math__AddComponents.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 21
- Description: Input a float 2, 3 or 4 and retrieve the sum of the component values.
- Input pins: f2 [FunctionInput_Vector2], f3 [FunctionInput_Vector3], f4 [FunctionInput_Vector4]
- Output pins: f2, f3, f4

### AngleBetweenVectors

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/AngleBetweenVectors.AngleBetweenVectors`
- Deep graph doc: [AngleBetweenVectors](Functions/Engine_MaterialFunctions02_Math__AngleBetweenVectors.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 21
- Description: Find the angle between two normalized vectors in degrees.
- Input pins: Normalized V1 [FunctionInput_Vector3], Normalized V2 [FunctionInput_Vector3]
- Output pins: Angle In Degrees, 0-1

### ArrangePointsEvenlyAroundABox

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ArrangePointsEvenlyAroundABox.ArrangePointsEvenlyAroundABox`
- Deep graph doc: [ArrangePointsEvenlyAroundABox](Functions/Engine_MaterialFunctions02_Math__ArrangePointsEvenlyAroundABox.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 7
- Outputs: 2
- Internal nodes: 83
- Description: This material function will evenly arrange points around a box. The dimensions and basis vectors are inputs along with a 0-1 placement value. This can be useful for evenly distributing models around an analytically described shape. 
- Input pins: XY Phase (0-1) [FunctionInput_Scalar], X Vector [FunctionInput_Vector3], Y Vector [FunctionInput_Vector3], Position [FunctionInput_Vector3], Z Vector [FunctionInput_Vector3], Z Phase (0-1) [FunctionInput_Scalar], Relative Pivot Location [FunctionInput_Vector3]
- Output pins: Point Around Box, World normal

### ConcatenateMatrices

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ConcatenateMatrices.ConcatenateMatrices`
- Deep graph doc: [ConcatenateMatrices](Functions/Engine_MaterialFunctions02_Math__ConcatenateMatrices.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 9
- Outputs: 4
- Internal nodes: 49
- Description: Concatenate 2 matrices
- Input pins: BasisX [FunctionInput_Vector4], BasisY [FunctionInput_Vector4], BasisZ [FunctionInput_Vector4], BasisW [FunctionInput_Vector4], BasisX2 [FunctionInput_Vector4], BasisY2 [FunctionInput_Vector4], BasisZ2 [FunctionInput_Vector4], BasisW2 [FunctionInput_Vector4], ---------- [FunctionInput_StaticBool]
- Output pins: X, Y, Z, W

### CorrectPixelDepthOffsetValue

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/CorrectPixelDepthOffsetValue.CorrectPixelDepthOffsetValue`
- Deep graph doc: [CorrectPixelDepthOffsetValue](Functions/Engine_MaterialFunctions02_Math__CorrectPixelDepthOffsetValue.md)
- Use: Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 8
- Description: Extends pdo distance as the camera aligns with the surface plane.
- Input pins: Pixel Depth Offset Amount [FunctionInput_Scalar]
- Output pins: Angle Correct Depth Value

### CreateThirdOrthogonalVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/CreateThirdOrthogonalVector.CreateThirdOrthogonalVector`
- Deep graph doc: [CreateThirdOrthogonalVector](Functions/Engine_MaterialFunctions02_Math__CreateThirdOrthogonalVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 3
- Internal nodes: 13
- Description: Takes two vectors and generates a third orthogonal vector from them. It also ensures that all of the vectors are orthogonal to each other.  The first vectors directon is favored over the second.
- Input pins: Vector1 [FunctionInput_Vector3], Vector2 [FunctionInput_Vector3]
- Output pins: Vector1, Vector2, Vector3

### DeleteReference4x4Matrix

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/DeleteReference4x4Matrix.DeleteReference4x4Matrix`
- Deep graph doc: [DeleteReference4x4Matrix](Functions/Engine_MaterialFunctions02_Math__DeleteReference4x4Matrix.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 14
- Description: Transform a vector into a new basis
- Input pins: BasisX [FunctionInput_Vector4], BasisY [FunctionInput_Vector4], BasisZ [FunctionInput_Vector4], BasisPosition [FunctionInput_Vector4], VectorToTransform [FunctionInput_Vector4]
- Output pins: Transformed Vector 4

### DeriveNormalZ_Function

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/DeriveNormalZ_Function.DeriveNormalZ_Function`
- Deep graph doc: [DeriveNormalZ_Function](Functions/Engine_MaterialFunctions02_Math__DeriveNormalZ_Function.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 12
- Description: DeriveNormalZ expanded into a material function, including helpful features like safe normalization and a z sign input.
- Input pins: Z sign [FunctionInput_Scalar], XY vector [FunctionInput_Vector2], Default Normalization vector [FunctionInput_Vector3]
- Output pins: Result

### FindSaturation

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/FindSaturation.FindSaturation`
- Deep graph doc: [FindSaturation](Functions/Engine_MaterialFunctions02_Math__FindSaturation.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 13
- Description: Finds the saturation of a color.
- Input pins: Color [FunctionInput_Vector3]
- Output pins: Saturation

### InverseTransformMatrix

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/InverseTransformMatrix.InverseTransformMatrix`
- Deep graph doc: [InverseTransformMatrix](Functions/Engine_MaterialFunctions02_Math__InverseTransformMatrix.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 4
- Outputs: 1
- Internal nodes: 10
- Description: Transform a vector into a new basis
- Input pins: BasisX [FunctionInput_Vector3], BasisY [FunctionInput_Vector3], BasisZ [FunctionInput_Vector3], VectorToTransform [FunctionInput_Vector3]
- Output pins: Transformed Vector 3

### LineIntervalIntersection

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/LineIntervalIntersection.LineIntervalIntersection`
- Deep graph doc: [LineIntervalIntersection](Functions/Engine_MaterialFunctions02_Math__LineIntervalIntersection.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 6
- Outputs: 3
- Internal nodes: 20
- Description: Returns the point of intersection for two lines. The X value for the endpoints of both lines must match. A Y value will be calculated from the interval.
- Input pins: A.x [FunctionInput_Scalar], B.x [FunctionInput_Scalar], Line1 - A.y [FunctionInput_Scalar], Line2 - A.y [FunctionInput_Scalar], Line1 - B.y [FunctionInput_Scalar], Line2 - B.y [FunctionInput_Scalar]
- Output pins: xy, x, y

### MakeVectorsOrthogonal

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/MakeVectorsOrthogonal.MakeVectorsOrthogonal`
- Deep graph doc: [MakeVectorsOrthogonal](Functions/Engine_MaterialFunctions02_Math__MakeVectorsOrthogonal.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 3
- Internal nodes: 14
- Description: Makes vectors orthagonal by using the cross product of the vectors.
- Input pins: Vector1 [FunctionInput_Vector3], Vector2 [FunctionInput_Vector3], Vector3 [FunctionInput_Vector3]
- Output pins: Vector1, Vector2, Vector3

### ManualWorldToScreenUVsTransform

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ManualWorldToScreenUVsTransform.ManualWorldToScreenUVsTransform`
- Deep graph doc: [ManualWorldToScreenUVsTransform](Functions/Engine_MaterialFunctions02_Math__ManualWorldToScreenUVsTransform.md)
- Use: UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 44
- Description: This function will allow one to manually recreate any perspective viewport without the use of any intrinsic view properties. This is helpful for Render to texture passes.
- Input pins: X Camera to World Vector [FunctionInput_Vector3], Y Camera to World Vector [FunctionInput_Vector3], Z Camera to World Vector [FunctionInput_Vector3], Camera Relative Position [FunctionInput_Vector3], Tan(FOV/2)*[1,Screen Res Y/X] [FunctionInput_Vector2]
- Output pins: Screen UVs

### MulM4V4

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/MulM4V4.MulM4V4`
- Deep graph doc: [MulM4V4](Functions/Engine_MaterialFunctions02_Math__MulM4V4.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 16
- Input pins: MatrixRow1 [FunctionInput_Vector4], MatrixRow3 [FunctionInput_Vector4], MatrixRow4 [FunctionInput_Vector4], MatrixRow2 [FunctionInput_Vector4], Vector [FunctionInput_Vector4]
- Output pins: Result

### MultiplyAdd

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/MultiplyAdd.MultiplyAdd`
- Deep graph doc: [MultiplyAdd](Functions/Engine_MaterialFunctions02_Math__MultiplyAdd.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 5
- Description: Modulate the "add" input by by the base input and then add the result to the base input.
- Input pins: Add [FunctionInput_Vector3], Base [FunctionInput_Vector3]
- Output pins: Result

### Pi

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/Pi.Pi`
- Deep graph doc: [Pi](Functions/Engine_MaterialFunctions02_Math__Pi.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 4
- Description: Pi multiplied by an input value.
- Input pins: Multiplier [FunctionInput_Scalar]
- Output pins: Result

### ProjectVectorOntoPlane

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/ProjectVectorOntoPlane.ProjectVectorOntoPlane`
- Deep graph doc: [ProjectVectorOntoPlane](Functions/Engine_MaterialFunctions02_Math__ProjectVectorOntoPlane.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 6
- Description: Projects a vector onto a plane by removing it's deviation along the plane's normal. 
- Input pins: Vector [FunctionInput_Vector3], Plane Normal [FunctionInput_Vector3]
- Output pins: Result

### pythagoreanTheorum

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/pythagoreanTheorum.pythagoreanTheorum`
- Deep graph doc: [pythagoreanTheorum](Functions/Engine_MaterialFunctions02_Math__pythagoreanTheorum.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 5
- Input pins: Side 1 Length [FunctionInput_Scalar]
- Output pins: Side 2 Length

### QuatAxisAngleConversions

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/QuatAxisAngleConversions.QuatAxisAngleConversions`
- Deep graph doc: [QuatAxisAngleConversions](Functions/Engine_MaterialFunctions02_Math__QuatAxisAngleConversions.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 20
- Description: Convert a quat to an angle axis and vice versa.
- Input pins: AxisAngle [FunctionInput_Vector4], Quaternion [FunctionInput_Vector4]
- Output pins: Axis Angle As Quat, Quat as Axis Angle

### Refract

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/Refract.Refract`
- Deep graph doc: [Refract](Functions/Engine_MaterialFunctions02_Math__Refract.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 20
- Input pins: RatioOfIndicesOfRefraction [FunctionInput_Scalar], IncidenceVector [FunctionInput_Vector3], Normal [FunctionInput_Vector3]
- Output pins: Result

### RGBtoHSV

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/RGBtoHSV.RGBtoHSV`
- Deep graph doc: [RGBtoHSV](Functions/Engine_MaterialFunctions02_Math__RGBtoHSV.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 87
- Description: Converts RGB colors to HSV (hue saturation and value)
- Input pins: RGB [FunctionInput_Vector3]
- Output pins: RGB to HSV (0-1)

### TimeWithSpeedVariable

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TimeWithSpeedVariable.TimeWithSpeedVariable`
- Deep graph doc: [TimeWithSpeedVariable](Functions/Engine_MaterialFunctions02_Math__TimeWithSpeedVariable.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 2
- Internal nodes: 7
- Description: Time multiplied by speed with an option to use the remainder or time itself.
- Input pins: Speed [FunctionInput_Scalar], Time [FunctionInput_Vector3]
- Output pins: Time, Frac Time

### Transform3x3Matrix

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/Transform3x3Matrix.Transform3x3Matrix`
- Deep graph doc: [Transform3x3Matrix](Functions/Engine_MaterialFunctions02_Math__Transform3x3Matrix.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 5
- Outputs: 1
- Internal nodes: 13
- Description: Transform a vector into a new basis
- Input pins: BasisX [FunctionInput_Vector3], BasisY [FunctionInput_Vector3], BasisZ [FunctionInput_Vector3], BasisPosition [FunctionInput_Vector3], VectorToTransform [FunctionInput_Vector3]
- Output pins: Transformed Vector

### TransformNormals_Tangent_to_Vertex

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TransformNormals_Tangent_to_Vertex.TransformNormals_Tangent_to_Vertex`
- Deep graph doc: [TransformNormals_Tangent_to_Vertex](Functions/Engine_MaterialFunctions02_Math__TransformNormals_Tangent_to_Vertex.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 2
- Outputs: 1
- Internal nodes: 18
- Description: Useful for when you want a World Aligned texture to pick up on mesh vertex normals but NOT tangent vectors.
- Input pins: Vector to Transform [FunctionInput_Vector3], Squash Vertex Normals [FunctionInput_Vector3]
- Output pins: Result

### TransformToClipSpace

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TransformToClipSpace.TransformToClipSpace`
- Deep graph doc: [TransformToClipSpace](Functions/Engine_MaterialFunctions02_Math__TransformToClipSpace.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 1
- Outputs: 1
- Internal nodes: 7
- Description: Transforms a world position into clip space (render buffer uv space).
- Input pins: World Position [FunctionInput_Vector3]
- Output pins: Result

### TransformToZVector

- Path: `/Engine/Functions/Engine_MaterialFunctions02/Math/TransformToZVector.TransformToZVector`
- Deep graph doc: [TransformToZVector](Functions/Engine_MaterialFunctions02_Math__TransformToZVector.md)
- Use: Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.
- Risk: Use get_material_function_info and wire by target_input_name for safer automation.
- Study status: inspected through get_material_function_info
- Inputs: 3
- Outputs: 1
- Internal nodes: 10
- Description: This transforms a vector based on a Z vector and generated orthogonal X and Y vectors. Useful for transforming worldposition based on Hit normals etc.
- Input pins: Vector to Transform [FunctionInput_Vector3], Z Vector [FunctionInput_Vector3], Center Location [FunctionInput_Vector3]
- Output pins: Result



